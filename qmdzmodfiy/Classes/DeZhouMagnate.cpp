#include "DeZhouMagnate.h"
#include "FrameSprite.h"
#include "HttpSprite.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "GameCommon.h"
#include <stdlib.h>
#include "Util.h"
#include "Tools.h"
#include "GameData.h"
USING_NS_CC;
using namespace cocos2d::ui;

#define  HalfDeskWidth 264/2
#define  HalfDeskHeight 203/2

enum ScrowFlip
{
    ScrollFlipW = 1060,
    ScrollFlipH = 429
};

DeZhouMagnate* DeZhouMagnate::create(cocos2d::Node *node)
{
    auto pRet = new DeZhouMagnate;
    if (pRet->init(node))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

bool DeZhouMagnate::init(cocos2d::Node *node)
{
    if (!Layer::init())
    {
        return false;
    }
    this->setContentSize(Size(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()));
    this->setAnchorPoint(Vec2(0, 0));

	// 背景重设
	ImageView* img_bg = static_cast<ImageView*>(Tools::getWidgetByName(node,"Image_line"));
	img_bg->setVisible(true);
	// 通过判断当前屏幕的宽度来设置层遮罩的位置
	Size screenSize = Director::getInstance()->getOpenGLView()->getFrameSize(); 
	if(screenSize.width == 1280)
		screenSize.height = screenSize.height - 220;
	else if(screenSize.width == 960)
		screenSize.height = screenSize.height - 22;
	else
		screenSize.height = 497;
    m_layerTop = GameLayer::create(GameData::getInstance()->getGameWidth(), 143, false, false);
    m_layerTop->setPosition(Vec2(0, screenSize.height));
    this->addChild(m_layerTop, 1);

    m_layerBottom = GameLayer::create(GameData::getInstance()->getGameWidth(), 68, false, false);
    m_layerBottom->setPosition(Vec2(0, 0));
    this->addChild(m_layerBottom, 1);

    
    auto selectBg = CreateNode1AndPosAddTo<Sprite>("magnate/selectbg.png", 595, 34, m_layerBottom, 3);
    auto pageViewBg = CreateNode1AndPosAddTo<Sprite>("magnate/pageviewbg.png", 595, 34, m_layerBottom, 3);

	//统计当前页总人数及总桌数
	auto playerNum = static_cast<Text*>(Tools::getWidgetByName(node,"Text_showuser"));
	auto posPlayer = playerNum->getWorldPosition();
    //滚动视图

	m_pView = static_cast<ScrollView*>(Tools::getWidgetByName(node,"Panel_active3"));

	m_spriteSegregate = CreateNode1AndPosAddTo<Sprite>("magnate/segregate.png", 568, posPlayer.y -  23, m_pView, 2);
	m_spriteSegregate->setVisible(false);
	auto szView = m_pView->getContentSize();
	m_ScrollFlip = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollFlipW, ScrollFlipH), (szView.width - ScrollFlipW) / 2, posPlayer.y - ScrollFlipH - 21, m_pView, 0);
    m_ScrollFlip->setContentSize(Size(ScrollFlipW, ScrollFlipH));
	m_ScrollFlip->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_ScrollFlip->setDelegate(this);

    //左翻页按钮
    m_btPageLeft = MenuItemSpriteWithLabel::create("magnate/pageleft.png", 3, CC_CALLBACK_1(DeZhouMagnate::OnPageLeft, this));
    m_btPageLeft->setPosition(37, 28);
    auto menuPageLeft = Menu::create(m_btPageLeft, nullptr);
    menuPageLeft->setPosition(Vec2::ZERO);
    selectBg->addChild(menuPageLeft);
    //右翻页按钮
    m_btPageRight = MenuItemSpriteWithLabel::create("magnate/pageright.png", 3, CC_CALLBACK_1(DeZhouMagnate::OnPageRight, this));
    m_btPageRight->setPosition(473, 28);
    auto menuPageRight = Menu::create(m_btPageRight, nullptr);
    menuPageRight->setPosition(Vec2::ZERO);
    selectBg->addChild(menuPageRight);
    //pageView
    Size sz = pageViewBg->getContentSize();
    m_pageView = PageView::create();
    m_pageView->setAnchorPoint(Vec2(0.5, 0.5));
    m_pageView->setContentSize(Size(sz.width, sz.height));
    m_pageView->setPosition(Vec2(sz.width/2, sz.height/2));

    m_pageView->addEventListener(CC_CALLBACK_2(DeZhouMagnate::pageViewEvent, this));
    pageViewBg->addChild(m_pageView);

    //快速加入按钮
    //auto quickJoinBt = MenuItemSpriteWithLabel::create("magnate/quickjoin.png", 3, CC_CALLBACK_1(DeZhouMagnate::OnQuickJoin, this));
    auto  quickJoinBt = MenuItemScale9SpriteWithLabel::create(1, Size(156, 56), CC_CALLBACK_1(DeZhouMagnate::OnQuickJoin, this));
    quickJoinBt->setPosition(123, 34);
    auto menuQuickJoin = Menu::create(quickJoinBt, nullptr);
    menuQuickJoin->setPosition(Vec2::ZERO);
    m_layerBottom->addChild(menuQuickJoin, 1);
    Size szbt = quickJoinBt->getContentSize();
    CreateLabelMSYHAndAnchorPosClrAddTo(24, "快速加入", Vec2(0.5,0.5), szbt.width/2, szbt.height/2, Color4B::WHITE, quickJoinBt, 0);
    //搜索按钮
    auto searchtBt = MenuItemSpriteWithLabel::create("magnate/search.png", 3, CC_CALLBACK_1(DeZhouMagnate::OnSearch, this));
    searchtBt->setPosition(1075, 34);
    auto menuSelect = Menu::create(searchtBt, nullptr);
    menuSelect->setPosition(Vec2::ZERO);
    m_layerBottom->addChild(menuSelect, 1);
    //刷新按钮
    auto updateBt = MenuItemSpriteWithLabel::create("magnate/update.png", 3, CC_CALLBACK_1(DeZhouMagnate::OnUpdate, this));
    updateBt->setPosition(970, 34);
    auto menuUpdate = Menu::create(updateBt, nullptr);
    menuUpdate->setPosition(Vec2::ZERO);
    m_layerBottom->addChild(menuUpdate, 1);
   
    m_labelCurPagePlayers = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), posPlayer.x - 60, posPlayer.y, Color4B(225, 210, 0, 255), this, 0);
    m_labelCurPageDesks = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), posPlayer.x + 120, posPlayer.y, Color4B(225, 210, 0, 255), this, 0);
    m_dOffSet = 0.0;
    m_uCurPageNum = 0;
    return true;
}

void DeZhouMagnate::UpdatePageView()
{
    //m_pageView->getPageCount();
    m_pageView->removeAllPages();
    m_uMaxPageNum = g_globalTableAreaInfo.m_areaDZPage.size();
    Size sz = m_pageView->getContentSize();
    int pageNum = 0;
    for (auto pageInfo : g_globalTableAreaInfo.m_areaDZPage)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(Size(sz.width, sz.height));
        CreateLabelMSYHAndAnchorPosClrAddTo(24, pageInfo.m_strBlindName, Vec2(0.5, 0.5), layout->getContentSize().width / 2.0f,
            layout->getContentSize().height / 2.0f, Color4B(0xff,0xff,0xff,0xff), layout, 0);
        m_pageView->insertPage(layout, pageNum);
        pageNum++;
    }
}

void DeZhouMagnate::ReceiveDataAndUpdate()
{
    UpdataMagnateView(m_uCurPageNum);
    if (m_uCurPageNum == 0)
    {
        m_btPageLeft->setVisible(false);
        m_btPageRight->setVisible(true);
    }
    else if (m_uCurPageNum == m_uMaxPageNum-1)
    {
        m_btPageLeft->setVisible(true);
        m_btPageRight->setVisible(false);
    }
    else
    {
        m_btPageLeft->setVisible(true);
        m_btPageRight->setVisible(true);
    }
}

void DeZhouMagnate::pageViewEvent(Ref* pSender, PageView::EventType type)
{
    PageView* pageView = dynamic_cast<PageView*>(pSender);
    unsigned pageIndex = pageView->getCurPageIndex();
    Json::Value param;
    if (m_uCurPageNum != pageIndex )
    {
        param["type"] = pageIndex + 4;
        param["num"] = 0;
        SendClientRequest(ClientRequest::kGetHallInfo, param);
        UpdataMagnateView(pageIndex);
        m_uCurPageNum = pageIndex;
    }
    if (pageIndex == 0)
    {
        m_btPageLeft->setVisible(false);
        m_btPageRight->setVisible(true);
    }
    else if (pageIndex == m_uMaxPageNum-1)
    {
        m_btPageLeft->setVisible(true);
        m_btPageRight->setVisible(false);
    }
    else
    {
        m_btPageLeft->setVisible(true);
        m_btPageRight->setVisible(true);
    }
}

void DeZhouMagnate::UpdataMagnateView(unsigned pageNum)
{
    //判断页数是否正确，防止访问数组越界
    if (pageNum < 0  || pageNum > m_uMaxPageNum)
    {
        return ;
    }
    for (auto item : m_vecMagnateNew)
    {
        item->m_spriteBgItem->setVisible(false);
        item->m_spriteNormalDesk->setVisible(false);
        item->m_spriteVipDesk->setVisible(false);
        m_vecMagnateOld.push_back(item);
    }
    m_vecMagnateNew.clear();

    unsigned uDeskNum = g_globalTableAreaInfo.m_areaDZDH.size();
    unsigned uCurPagePlayers = 0;
    unsigned uDeskCount = 0;
    unsigned uOneItemH = 224;
    unsigned uOneItemW = 300;
    unsigned uAllItemH = 0;
    unsigned fX = uOneItemW / 2, fY = uOneItemH / 2;
    if (uDeskNum%3 == 0 && uDeskNum > 0)
    {
        uAllItemH = (uDeskNum/3)*uOneItemH;
    }
    else
    {
        uAllItemH = (uDeskNum/3)*uOneItemH + uOneItemH;
    }
    if (uAllItemH < ScrollFlipH)
    {
        uAllItemH = ScrollFlipH;
    }
    m_dOffSet= 0.0;
    double offSet = ScrollFlipH - (double)uAllItemH;
    m_dOffSet = offSet;
    m_ScrollFlip->setContentOffset(Vec2(0, offSet), false);
    fY = uAllItemH - uOneItemH / 2;
    unsigned uHorizontalNum = 0;
    int fxOffSet = uOneItemW/2 + (ScrollFlipW - uOneItemW*3)/2;//左边桌子中心到左边框的距离
    for (auto dataItem : g_globalTableAreaInfo.m_areaDZDH)
    {
        auto magnateItem = GetMagnateItem(dataItem);
        UpdataMagnateItem(magnateItem, dataItem);
        m_vecMagnateNew.push_back(magnateItem);
        uHorizontalNum = uDeskCount % 3;
        magnateItem->m_spriteBgItem->setPosition(uHorizontalNum*uOneItemW + fxOffSet, fY);
        magnateItem->m_spriteNormalDesk->setPosition(uHorizontalNum*uOneItemW + fxOffSet, fY);
        magnateItem->m_spriteVipDesk->setPosition(uHorizontalNum*uOneItemW + fxOffSet, fY);
        magnateItem->m_spriteBgItem->setVisible(true);
        if ((uDeskCount+1)%3 == 0)
        {
            fY -= uOneItemH;
        }
        uDeskCount++;
        uCurPagePlayers += dataItem.m_uCurPlayer;
    }
    char countBuff[20];
    sprintf(countBuff, "牌桌：%d", uDeskNum);
    SetLabelString(m_labelCurPageDesks, countBuff);
    sprintf(countBuff, "在线玩家数：%d", uCurPagePlayers);
    SetLabelString(m_labelCurPagePlayers, countBuff);
    m_ScrollFlip->setContentSize(Size(ScrollFlipW, uAllItemH));
}

DeZhouMagnate::MagnateItem* DeZhouMagnate::GetMagnateItem(DeZhouMagnateInfo data)
{
    MagnateItem *item = nullptr;
    if (m_vecMagnateOld.empty())
    {
        //背景桌子
        auto bgItem = FrameSpriteButton::createWithVertFrameSprite("magnate/btbg.png", false, true);
        m_ScrollFlip->addChild(bgItem, 1);
        //普通桌背景
        auto spriteNormalDesk = CreateNode1AndPosAddTo<Sprite>("magnate/normaldesk.png", 0, 0, m_ScrollFlip, 0);
        //会员桌背景
        auto spriteVipDesk = CreateNode1AndPosAddTo<Sprite>("magnate/vipdesk.png", 0, 0, m_ScrollFlip, 0);
        //玩家名称背景 左
        auto spriteBgLeft = CreateNode1AndPosAddTo<Sprite>("magnate/playerbg.png", 82 - HalfDeskWidth, 15 - HalfDeskHeight, bgItem, 0);
        //玩家名称背景 右
        auto spriteBgRight = CreateNode1AndPosAddTo<Sprite>("magnate/playerbg.png", 180 - HalfDeskWidth, 15 - HalfDeskHeight, bgItem, 0);
        //房主名称背景
        auto spriteBgOwner = CreateNode1AndPosAddTo<Sprite>("magnate/ownerbg.png", 134 - HalfDeskWidth, 15 - HalfDeskHeight, bgItem, 0);
        //人物头像
        auto spriteHuman = CreateNode1AndPosAddTo<Sprite>("magnate/human.png", 110 - HalfDeskWidth, 125 - HalfDeskHeight, bgItem, 0);
        //摄像头
        auto spriteCamera = CreateNode1AndPosAddTo<Sprite>("magnate/camera.png", 148 - HalfDeskWidth, 97 - HalfDeskHeight, bgItem, 0);
        //眼睛
        auto spriteEye = CreateNode1AndPosAddTo<Sprite>("magnate/eye.png", 98 - HalfDeskWidth, 97 - HalfDeskHeight, bgItem, 0);
        //贵族
        auto spriteVip = CreateNode1AndPosAddTo<Sprite>("magnate/vipicon.png", 30 - HalfDeskWidth, 180 - HalfDeskHeight, bgItem, 0);
        //单挑
        auto spriteChallenge = CreateNode1AndPosAddTo<Sprite>("magnate/challenge.png", 30 - HalfDeskWidth, 180 - HalfDeskHeight, bgItem, 0);
        //必下
        auto spritePour = CreateNode1AndPosAddTo<Sprite>("magnate/mustpour.png", 30 - HalfDeskWidth, 180 - HalfDeskHeight, bgItem, 0);
        //带入
        auto labelMinTake = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), 131 - HalfDeskWidth, 155 - HalfDeskHeight, Color4B(0xd5,0xff,0xc2,0xff), bgItem, 0);
        //玩家数量
        auto labelPlayerNum = CreateLabelMSYHBDAndAnchorPosClrAddTo(26, "", Vec2(0.5,0.5), 150 - HalfDeskWidth, 124 - HalfDeskHeight, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);
        auto labelPlayerFull = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0.5,0.5), 150 - HalfDeskWidth, 124 - HalfDeskHeight, Color4B(0xd5,0xff,0xc2,0xff), bgItem, 0);
        //玩家名称 左
        auto labelPlayerLeft = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), 82 - HalfDeskWidth, 16 - HalfDeskHeight, Color4B(0xa5,0xe7,0x91,0xff), bgItem, 0);
        //玩家名称 右
        auto labelPlayerRight = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), 180 - HalfDeskWidth, 16 - HalfDeskHeight, Color4B(0xa5,0xe7,0x91,0xff), bgItem, 0);
        //房主名称
        auto labelPlayerOwner = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), 136 - HalfDeskWidth, 15 - HalfDeskHeight, Color4B(0xa5,0xe7,0x91,0xff), bgItem, 0);
        //旁观数量
        auto labelOnlookNum = CreateLabelMSYHBDAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), 130 - HalfDeskWidth, 96 - HalfDeskHeight, Color4B(0xff,0xff,0xff,152), bgItem, 0);
        //头像左
        //auto httpUserHeadLeft = CreateUserHeadSprite("defaulticon.png", "magnate/iconmask.png", bgItem, Vec2(58 - HalfDeskWidth,26 - HalfDeskHeight), "magnate/iconround.png");
        auto httpUserHeadLeft = CreateNode2AndPosAddTo<HttpSprite>("defaulticon.png", Size(50,50),58 - HalfDeskWidth, 31 - HalfDeskHeight, bgItem, 0);
        //头像右
       // auto httpUserHeadRight = CreateUserHeadSprite("defaulticon.png", "magnate/iconmask.png", bgItem, Vec2(148 - HalfDeskWidth,26 - HalfDeskHeight), "magnate/iconround.png");
        auto httpUserHeadRight = CreateNode2AndPosAddTo<HttpSprite>("defaulticon.png", Size(50,50),154 - HalfDeskWidth, 31 - HalfDeskHeight, bgItem, 0);

        auto pItem = new MagnateItem;
        pItem->m_spriteBgItem = bgItem;
        pItem->m_spriteNormalDesk = spriteNormalDesk;
        pItem->m_spriteVipDesk = spriteVipDesk;
        pItem->m_spritePlayerBg[0] = spriteBgLeft;
        pItem->m_spritePlayerBg[1] = spriteBgRight;
        pItem->m_spritePlayerBg[2] = spriteBgOwner;
        pItem->m_spriteHuman = spriteHuman;
        pItem->m_spriteCamera = spriteCamera;
        pItem->m_spriteEye = spriteEye;
        pItem->m_spriteClassify[0] = spriteVip;
        pItem->m_spriteClassify[1] = spriteChallenge;
        pItem->m_spriteClassify[2] = spritePour;
        pItem->m_labelMinTake = labelMinTake;
        pItem->m_labelCurPlayer[0] = labelPlayerNum;
        pItem->m_labelCurPlayer[1] = labelPlayerFull;
        pItem->m_labelUserName[0] = labelPlayerLeft;
        pItem->m_labelUserName[1] = labelPlayerRight;
        pItem->m_labelUserName[2] = labelPlayerOwner;
        pItem->m_labelOnlookNum = labelOnlookNum;
        pItem->m_httpUserHead[0] = httpUserHeadLeft;
        pItem->m_httpUserHead[1] = httpUserHeadRight;
        item = pItem;
    }
    else
    {
        item = m_vecMagnateOld.back();
        m_vecMagnateOld.pop_back();
    }

    item->m_spriteBgItem->SetHitCB([this,data](Ref* ){
    if (!data.m_bIsVip || g_globalMyData.m_iUserId == data.m_UserID)  //非会员桌或会员本人进会员桌
    {
        Json::Value param;
        param["enterType"] = data.m_uDeskID;
        SendClientRequest(ClientRequest::kEnterTable, param);
    }
    else
    {
        auto passWordScene = EnterVipRoom::create(data.m_uDeskID);
        passWordScene->setAnchorPoint(Vec2(0.5, 0.5));
		auto pPassWord = passWordScene->getContentSize();
		passWordScene->setPosition((GAME_WIDTH - pPassWord.width) / 2, (GAME_HEIGHT - pPassWord.height) / 2);
        this->addChild(passWordScene);
    }
    });
    

    

    return item;
}

void DeZhouMagnate::UpdataMagnateItem(DeZhouMagnate::MagnateItem *item, const DeZhouMagnateInfo &data)
{
    char buff[50];
    //玩家数量&字体是否加粗
    sprintf(buff, "%d/%d", data.m_uCurPlayer, data. m_uMaxPlayer);
    if (data.m_uCurPlayer < data.m_uMaxPlayer)
    {
        SetLabelString(item->m_labelCurPlayer[0], buff);
        item->m_labelCurPlayer[0]->setVisible(true);
        item->m_labelCurPlayer[1]->setVisible(false);
    }
    else
    {
        SetLabelString(item->m_labelCurPlayer[1], buff);
        item->m_labelCurPlayer[0]->setVisible(false);
        item->m_labelCurPlayer[1]->setVisible(true);
    }
    //玩家名称&头像
    if (!data.m_bIsVip)
    {
        Size sz = item->m_spritePlayerBg[0]->getContentSize(); 
        sprintf(buff, "%s带入", FormatCash(data.m_uRoomName).c_str());
        SetLabelString(item->m_labelMinTake, buff);                                      //房间名
        item->m_labelMinTake->setTextColor(Color4B(0xd5,0xff,0xc2,0xff));                //房间名字体颜色
        //if (data.m_strPlayerName[0] != "")
        if (data.m_strPlayerName[0].length() > 0)
        {
            item->m_httpUserHead[0]->setVisible(true);
            SetIconUrl(item->m_httpUserHead[0], data.m_strIconURL[0]);                   //设置玩家头像1
            //if (data.m_strPlayerName[1] != "")
            if (data.m_strPlayerName[1].length() > 0)
            {
                item->m_spritePlayerBg[0]->setVisible(true);
                item->m_spritePlayerBg[1]->setVisible(true);
                item->m_spritePlayerBg[2]->setVisible(false);
                item->m_labelUserName[0]->setVisible(true);
                item->m_labelUserName[1]->setVisible(true);
                item->m_labelUserName[2]->setVisible(false);
                item->m_httpUserHead[1]->setVisible(true);
                SetIconUrl(item->m_httpUserHead[1], data.m_strIconURL[1]);
                item->m_httpUserHead[0]->setPosition(58 - HalfDeskWidth, 31 - HalfDeskHeight);   //设置玩家1头像位置
                SetLabelStringWithBoundWidth(item->m_labelUserName[0], data.m_strPlayerName[0], sz.width);
                SetLabelStringWithBoundWidth(item->m_labelUserName[1], data.m_strPlayerName[1], sz.width);
            }
            else
            {
                item->m_spritePlayerBg[0]->setVisible(false);
                item->m_spritePlayerBg[1]->setVisible(false);
                item->m_spritePlayerBg[2]->setVisible(true);
                item->m_labelUserName[0]->setVisible(false);
                item->m_labelUserName[1]->setVisible(false);
                item->m_labelUserName[2]->setVisible(true);
                item->m_httpUserHead[1]->setVisible(false);
                Size szbg = item->m_spritePlayerBg[2]->getContentSize();
                item->m_httpUserHead[0]->setPosition(111 - HalfDeskWidth, 31 - HalfDeskHeight);   //设置贵族头像位置
                SetLabelStringWithBoundWidth(item->m_labelUserName[2], data.m_strPlayerName[0], szbg.width);
            }
        }
        else
        {
            item->m_spritePlayerBg[0]->setVisible(false);
            item->m_spritePlayerBg[1]->setVisible(false);
            item->m_spritePlayerBg[2]->setVisible(false);
            item->m_httpUserHead[0]->setVisible(false);
            item->m_httpUserHead[1]->setVisible(false);
            item->m_httpUserHead[1]->setVisible(false);
            item->m_labelUserName[0]->setVisible(false);
            item->m_labelUserName[1]->setVisible(false);
            item->m_labelUserName[2]->setVisible(false);
            
        }
        item->m_spriteNormalDesk->setVisible(true);
        item->m_spriteVipDesk->setVisible(false);
    }
    else
    {
        Size sz = item->m_spritePlayerBg[2]->getContentSize();
        SetLabelStringWithBoundWidth(item->m_labelMinTake, data.m_sRoomVipName.c_str(),165);                         //房间名
        item->m_labelMinTake->setTextColor(Color4B(0xfc,0xfb,0xac,0xff));                 //房间名字体颜色
        //if (data.m_strPlayerName[0] != "")
        if (data.m_strPlayerName[0].length() > 0 )
        {
            item->m_spritePlayerBg[2]->setVisible(true);
            item->m_httpUserHead[0]->setVisible(true);
            item->m_labelUserName[2]->setVisible(true);
            sprintf(buff, "房主：%s", data.m_strPlayerName[0].c_str());
            SetLabelStringWithBoundWidth(item->m_labelUserName[2], buff, sz.width);
            SetIconUrl(item->m_httpUserHead[0], data.m_strIconURL[0]);                  //设置贵族头像
        }
        else
        {
            item->m_spritePlayerBg[2]->setVisible(false);
            item->m_httpUserHead[0]->setVisible(false);
            item->m_labelUserName[2]->setVisible(false);
        }
        item->m_spriteNormalDesk->setVisible(false);
        item->m_spriteVipDesk->setVisible(true);
        item->m_httpUserHead[0]->setPosition(110 - HalfDeskWidth, 31 - HalfDeskHeight);   //设置贵族头像位置
        item->m_httpUserHead[1]->setVisible(false);
        item->m_labelUserName[0]->setVisible(false);
        item->m_labelUserName[1]->setVisible(false);
        item->m_spritePlayerBg[0]->setVisible(false);
        item->m_spritePlayerBg[1]->setVisible(false);
    }
    //摄像头&旁观数量
    if (!data.m_bIsCamera)
    {
        sprintf(buff, "%d", data.m_uOnlookNum);
        item->m_spriteEye->setPosition(119 - HalfDeskWidth, 97 - HalfDeskHeight);
        SetLabelString(item->m_labelOnlookNum, buff);
        item->m_labelOnlookNum->setPosition(150 - HalfDeskWidth, 98 - HalfDeskHeight);
        item->m_spriteCamera->setVisible(false);
    }
    else
    {
        sprintf(buff, "%d", data.m_uOnlookNum);
        item->m_spriteEye->setPosition(107 - HalfDeskWidth, 97 - HalfDeskHeight);
        SetLabelString(item->m_labelOnlookNum, buff);
        item->m_labelOnlookNum->setPosition(129 - HalfDeskWidth, 98 - HalfDeskHeight);
        item->m_spriteCamera->setVisible(true);
    }
    //贵族 | 单挑 | 必下
    if (data.m_bIsVip)
    {
        item->m_spriteClassify[0]->setVisible(true);
        item->m_spriteClassify[1]->setVisible(false);
        item->m_spriteClassify[2]->setVisible(false);
    }
    else if (data.m_bIsChallenge)
    {
        item->m_spriteClassify[0]->setVisible(false);
        item->m_spriteClassify[1]->setVisible(true);
        item->m_spriteClassify[2]->setVisible(false);
    }
    else if (data.m_bIsMustPour)
    {
        item->m_spriteClassify[0]->setVisible(false);
        item->m_spriteClassify[1]->setVisible(false);
        item->m_spriteClassify[2]->setVisible(true);
    }
    else
    {
        item->m_spriteClassify[0]->setVisible(false);
        item->m_spriteClassify[1]->setVisible(false);
        item->m_spriteClassify[2]->setVisible(false);
    }
    if (data.m_bIsMustPour && !data.m_bIsVip)
    {
        sprintf(buff, "%s必下", FormatCash(data.m_uMustBet).c_str());
        SetLabelString(item->m_labelMinTake, buff);
    }
}

void DeZhouMagnate::OnPageLeft(cocos2d::Ref* pSender)
{
    if (m_uCurPageNum - 1 >= 0)
    {
        m_uCurPageNum -= 1;
        m_pageView->scrollToPage(m_uCurPageNum);
        Json::Value param;
        param["type"] = m_uCurPageNum + 4;
        param["num"] = 0;
        SendClientRequest(ClientRequest::kGetHallInfo, param);
        //UpdataMagnateView(m_uCurPageNum);
    }
}

void DeZhouMagnate::OnPageRight(cocos2d::Ref* pSender)
{
    if (m_uCurPageNum + 1 <= m_uMaxPageNum)
    {
        m_uCurPageNum += 1;
        m_pageView->scrollToPage(m_uCurPageNum);
        Json::Value param;
        param["type"] = m_uCurPageNum + 4;
        param["num"] = 0;
        SendClientRequest(ClientRequest::kGetHallInfo, param);
        //UpdataMagnateView(m_uCurPageNum);
    }
}

void DeZhouMagnate::SetLayerVisable(bool visable)
{
    m_layerTop->setVisible(visable);
    m_layerBottom->setVisible(visable);
	m_pView->setVisible(visable);
}


void DeZhouMagnate::OnQuickJoin(cocos2d::Ref* pSender)
{
    Json::Value param;
    param["enterType"] = -2;
    SendClientRequest(ClientRequest::kEnterTable, param);
}

void DeZhouMagnate::OnSearch(cocos2d::Ref* pSender)
{

}

void DeZhouMagnate::OnUpdate(cocos2d::Ref* pSender)
{
    Json::Value param;
    param["type"] = m_uCurPageNum + 4;
    param["num"] = 0;
    SendClientRequest(ClientRequest::kGetHallInfo, param);
}

void DeZhouMagnate::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    auto offSet = m_ScrollFlip->getContentOffset();
    if (offSet.y > m_dOffSet)
    {
        m_spriteSegregate->setVisible(true);
    }
    else
    {
        m_spriteSegregate->setVisible(false);
    }
}

EnterVipRoom* EnterVipRoom::create(unsigned uDeskID)
{
    auto pRet = new EnterVipRoom;
    if (pRet->init(uDeskID))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

bool EnterVipRoom::init(unsigned uDeskID)
{
    if (!GameLayer::init("magnate/passwordbg.png", true, true))
    {
        return false;
    }
    m_uDeskID = uDeskID;
	Size sz = this->getContentSize();
	m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(EnterVipRoom::onMenuItemCloseSelected, this));
	m_btMenuItemClose->setPosition(sz.width - 20, sz.height - 20);
	auto menu = Menu::create(m_btMenuItemClose, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);

	//this->GetMenuItemClose()->setPosition(pEnterVipRoom.width - 25, pEnterVipRoom.height - 25);
    CreateLabelMSYHAndAnchorPosClrAddTo(24, "此房间是贵族房，请输入四位密码", Vec2(0.5,0.5), 276, 228, Color4B::WHITE, this, 0);

    m_btEnterRoom = MenuItemScale9SpriteWithLabel::create(0, Size(180, 60), CC_CALLBACK_1(EnterVipRoom::EnterRoom, this));
    m_btEnterRoom->setPosition(188,36);
    auto menuEnterRoom = Menu::create(m_btEnterRoom, nullptr);
    menuEnterRoom->setPosition(Vec2::ZERO);
    this->addChild(menuEnterRoom, 1);
    Size szbt = m_btEnterRoom->getContentSize();
    CreateLabelMSYHBDAndAnchorPosClrAddTo(28 ,"确定", Vec2(0.5,0.5), szbt.width/2, szbt.height/2, Color4B::WHITE, m_btEnterRoom, 0);

    m_btCancelRoom = MenuItemScale9SpriteWithLabel::create(0, Size(180, 60), CC_CALLBACK_1(EnterVipRoom::CancelEnter, this));
    m_btCancelRoom->setPosition(366,36);
    auto menuCancelRoom = Menu::create(m_btCancelRoom, nullptr);
    menuCancelRoom->setPosition(Vec2::ZERO);
    this->addChild(menuCancelRoom, 1);
    CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "取消", Vec2(0.5,0.5), szbt.width/2, szbt.height/2, Color4B::WHITE, m_btCancelRoom, 0);

    m_editPassWord = ui::EditBox::create(Size(460, 58), cocos2d::ui::Scale9Sprite::create("magnate/bgedit.png"));
    m_editPassWord->setInputMode(ui::EditBox::InputMode::NUMERIC);
    m_editPassWord->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editPassWord->setFont("fzlt.ttf", 30);
    m_editPassWord->setFontColor(Color3B(5,77,29));
    m_editPassWord->setMaxLength(4);
    m_editPassWord->setDelegate(this);
    m_editPassWord->setPosition(Vec2(276, 156));
    this->addChild(m_editPassWord);

    m_btEnterRoom->setEnabled(false);
    return true;
}

void EnterVipRoom::onMenuItemCloseSelected(cocos2d::Ref* pSend)
{
	this->setVisible(false);
}

void EnterVipRoom::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    const char *buff = m_editPassWord->getText();
    m_strPassword = buff;
    if (m_strPassword.size() == 4)
    {
        m_btEnterRoom->setEnabled(true);
    }
}

void EnterVipRoom::EnterRoom(cocos2d::Ref* pSend)
{
    Json::Value param;
    param["enterType"] = m_uDeskID;
    param["passWord"] = m_strPassword.c_str();
    SendClientRequest(ClientRequest::kEnterTable, param);
	setVisible(false);
}
void EnterVipRoom::CancelEnter(cocos2d::Ref* pSend)
{
    this->setVisible(false);
}


bool EnterVipRoomEx::init()
{
    if (!GameDialog::init("magnate/passwordbg.png", true))
    {
        return false;
    }
	Size pEnterVipRoom = this->getContentSize();
	this->GetMenuItemClose()->setPosition(pEnterVipRoom.width - 25, pEnterVipRoom.height - 25);

    CreateLabelMSYHAndAnchorPosClrAddTo(24, "此房间是贵族房，请输入四位密码", Vec2(0.5,0.5), 276, 228, Color4B::WHITE, this, 0);

    m_btEnterRoom = MenuItemScale9SpriteWithLabel::create(0, Size(180, 60), CC_CALLBACK_1(EnterVipRoomEx::EnterRoom, this));
    m_btEnterRoom->setPosition(188,36);
    auto menuEnterRoom = Menu::create(m_btEnterRoom, nullptr);
    menuEnterRoom->setPosition(Vec2::ZERO);
    this->addChild(menuEnterRoom, 1);
    Size szbt = m_btEnterRoom->getContentSize();
    CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "确定", Vec2(0.5,0.5), szbt.width/2, szbt.height/2, Color4B::WHITE, m_btEnterRoom, 0);

    //m_btCancelRoom = MenuItemSpriteWithLabel::create("magnate/optbt.png", 3, CC_CALLBACK_1(EnterVipRoomEx::CancelEnter, this));
    m_btCancelRoom = MenuItemScale9SpriteWithLabel::create(0, Size(180, 60), CC_CALLBACK_1(EnterVipRoomEx::CancelEnter, this));
    m_btCancelRoom->setPosition(366,36);
    auto menuCancelRoom = Menu::create(m_btCancelRoom, nullptr);
    menuCancelRoom->setPosition(Vec2::ZERO);
    this->addChild(menuCancelRoom, 1);
    CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "取消", Vec2(0.5,0.5), szbt.width/2, szbt.height/2, Color4B::WHITE, m_btCancelRoom, 0);

    m_editPassWord = ui::EditBox::create(Size(460, 58), cocos2d::ui::Scale9Sprite::create("magnate/bgedit.png"));
    m_editPassWord->setInputMode(ui::EditBox::InputMode::NUMERIC);
    m_editPassWord->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editPassWord->setFont("fzlt.ttf", 30);
    m_editPassWord->setFontColor(Color3B(5,77,29));
    m_editPassWord->setMaxLength(4);
    m_editPassWord->setDelegate(this);
    m_editPassWord->setPosition(Vec2(276, 156));
    this->addChild(m_editPassWord);

    m_btEnterRoom->setEnabled(false);
    return true;
}

void EnterVipRoomEx::SetHitCB(std::function<void(const std::string&)> cbBt)
{
	m_cbBt = cbBt;
}

void EnterVipRoomEx::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    const char *buff = m_editPassWord->getText();
    m_strPassword = buff;
    if (m_strPassword.size() == 4)
    {
        m_btEnterRoom->setEnabled(true);
    }
}

void EnterVipRoomEx::EnterRoom(cocos2d::Ref* pSend)
{
    //Json::Value param;
    //param["enterType"] = m_uDeskID;
    //param["passWord"] = m_strPassword.c_str();
    //SendClientRequest(ClientRequest::kEnterTable, param);
	if (m_cbBt)
	{
		m_cbBt(m_strPassword);
	}
	setVisible(false);
}
void EnterVipRoomEx::CancelEnter(cocos2d::Ref* pSend)
{
    this->setVisible(false);
}
