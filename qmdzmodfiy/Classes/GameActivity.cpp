#include "GameActivity.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "GameCommon.h"
#include "HttpSprite.h"
#include "Util.h"
#include "GameData.h"
#include "GameTipsManager.h"
#include "GameFrame.h"
#include<algorithm>
#include "Tools.h"

USING_NS_CC;

enum BtnSrcollView	//	设置默认的滚动视图的大小
{
    BtnScrollW = 244,
    BtnScrollH = 440
};
enum ContextSrcollView	//	设置默认的滚动视图的大小
{
    ContextScrollW = 584,
    ContextScrollH = 249
};

bool compare(const ActivityInfo &a, const ActivityInfo &b)
{
    return a.m_uType > b.m_uType;

}

bool GameActivity::init()
{
    if(!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }

    auto title = CreateNode1AndPosAddTo<Sprite>("activities/title.png", 470, 568, this, 0);
    m_bgContextItem = CreateNode1AndPosAddTo<Sprite>("activities/bgitem.png", 587, 290, this, 0);
    //auto sz = m_bgContextItem->getContentSize();
    //标题
    auto szContext = m_bgContextItem->getContentSize();
    m_LableTitle = CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "", Vec2(0.5, 0.5), szContext.width/2, szContext.height-72, Color4B(0xf2,0xb8,0x54,0xff), m_bgContextItem, 0);
    m_LabelTips = CreateLabelMSYHAndAnchorPosClrAddTo(24, "哎哟，暂时还没有活动哦，敬请期待下一个活动吧！", Vec2(0.5, 0.5), this->getContentSize().width/2, this->getContentSize().height/2, Color4B(0xff,0xff,0xff,0xff), this, 0);
    //按钮滚动视图
    m_btnScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(BtnScrollW, BtnScrollH), 28, 70, this, 0);
    m_btnScrollView->setContentSize(Size(BtnScrollW, BtnScrollH));
    m_btnScrollView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    //m_btnScrollView->setBounceable(false);
    //活动内容滚动视图
    m_contextScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ContextScrollW + 45, ContextScrollH), 2, 81, m_bgContextItem, 0);
    m_contextScrollView->setContentSize(Size(ContextScrollW + 45, ContextScrollH));
    m_contextScrollView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    m_contextScrollView->setBounceable(true);
    //内容
    m_LableContext = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0, 0), 30, 0, Color4B(0x98,0xdf,0xb3,0xff), m_contextScrollView, 0);
    m_LableContext->setHorizontalAlignment(TextHAlignment::LEFT);
	m_LableContext->setLineBreakWithoutSpace(true);
    m_LableContext->setLineHeight(30);
    m_LableContext->setWidth(576);
    m_bgContextItem->setVisible(false);
    //参与按钮
    //auto takePartBt = MenuItemSpriteWithLabel::create("activities/takepart.png", 3, CC_CALLBACK_1(GameActivity::TakePartImmediately, this));
    m_takePartBt = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(GameActivity::TakePartImmediately, this));
    Size szBt = m_takePartBt->getContentSize();
    m_takePartBt->setPosition(szContext.width/2, szBt.height/2 + 5);
    CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "立即参与", Vec2(0.5, 0.5), szBt.width/2, szBt.height/2, Color4B::WHITE, m_takePartBt, 0);
    auto menuTakePart = Menu::create(m_takePartBt, nullptr);
    menuTakePart->setPosition(Vec2::ZERO);
    m_bgContextItem->addChild(menuTakePart);
    //takePartBt->setVisible(false);

    m_uRedefineIndex = -1;
    m_uSceondRedefineIndex = 0;
    return true;
}

void GameActivity::onSetVisible(bool bVisible)
{
    //PlayVisibleSound(bVisible);
}

void GameActivity::TakePartImmediately(cocos2d::Ref *pSend)
{
    //0：不跳转，1：商城，2：消息，3：任务，4：活动，5：俱乐部，6：常规场，7：SNG场，8：MTT场
    switch (m_uRedefineIndex)
    {
    case 0:{}break;
    case 1:{
        Json::Value param;
        param["idx"] = m_uSceondRedefineIndex-1;
        SendClientRequest(ClientRequest::kPopMall, param);
           }break;
    case 2:{g_pGameFrame->popHallBottomDialog(2);}break;
    case 3:{g_pGameFrame->popHallBottomDialog(3);}break;
    case 4:{g_pGameFrame->popHallBottomDialog(4);}break;
    case 5:{g_pGameFrame->popHallBottomDialog(5);}break;
    case 6:{
        SendClientRequest(ClientRequest::kEnterHall2);
        GameTipsManager::getInstance()->clean();
           }break;
    case 7:{
        SendClientRequest(ClientRequest::kEnterGameSngHall);
        GameTipsManager::getInstance()->clean();
           }break;
    case 8:{
        SendClientRequest(ClientRequest::kEnterGameMttHall);
        GameTipsManager::getInstance()->clean();
           }break;
    }
    this->setVisible(false);
}

void GameActivity::UpdateActivityView()
{
    for (auto activity : m_vecBtnItemInfo)
    {
        activity->m_pBtActivity->removeFromParent();;
    }
    if(m_vecBtnItemInfo.size() > 0)
        m_vecBtnItemInfo.clear();
    unsigned uAllItem = g_globalActivityInfo.m_vecActivityInfo.size();
    unsigned uOneItemH = 84 + 4; //按钮高度+间隔
    unsigned uAllItemH = uOneItemH*uAllItem;
    unsigned fX = BtnScrollW/2, fY = uOneItemH/2;
    if (uAllItemH < BtnScrollH)
    {
        fY = BtnScrollH - uAllItemH + fY;
        uAllItemH = BtnScrollH;
    }
    float offSet = BtnScrollH - (float)uAllItemH;
    m_btnScrollView->setContentOffset(Vec2(0, offSet), false);

    if(g_globalActivityInfo.m_vecActivityInfo.size() > 0)
    {
        m_bgContextItem->setVisible(true);
        m_LabelTips->setVisible(false);
        std::sort(g_globalActivityInfo.m_vecActivityInfo.begin(), g_globalActivityInfo.m_vecActivityInfo.end(), compare);
    }
    else
    {
        m_LabelTips->setVisible(true);
        m_bgContextItem->setVisible(false);
    }
    for (auto activity : g_globalActivityInfo.m_vecActivityInfo)
    {
        auto btItem = FrameSpriteButton::createWithVertFrameSprite("activities/activitybtn.png", false, true);
        m_btnScrollView->addChild(btItem);
        btItem->SetHitCB(std::bind(&GameActivity::PopTaskOn, this, activity.m_uID, activity.m_uRedefine, activity.m_uSecondRedfine,activity.m_strTitle));
        Label *lableTitleContext = nullptr;
        HttpSprite *httpSprite = nullptr;
		if(activity.m_bIsShow)//按钮上显不显示图标
		{
			lableTitleContext = CreateLabelMSYHBDAndAnchorPosClrAddTo(26, activity.m_strTitle, Vec2(0.5, 0.5), 46, 0, Color4B(0x37,0xa4,0x5c,0xff), btItem, 0);
            httpSprite = CreateNode1AndPosAddTo<HttpSprite>("activities/activeicon.png", -75, 0, btItem, 0);
            httpSprite->ignoreAnchorPointForPosition(false);
            httpSprite->setAnchorPoint(Vec2(0.5, 0.5));
			btItem->setPosition(fX, fY);
			lableTitleContext->setWidth(150);//自动换行
			lableTitleContext->setLineBreakWithoutSpace(true);
			lableTitleContext->setHorizontalAlignment(TextHAlignment::LEFT);
			lableTitleContext->setLineHeight(30);
            SetHttpSpriteURL(httpSprite, activity.m_strIcon);
		}
		else
		{
			auto lableTitle = CreateLabelMSYHBDAndAnchorPosClrAddTo(26, activity.m_strTitle, Vec2(0.5, 0.5), 0, 0, Color4B(0x65,0xae,0x7c,0xff), btItem, 0);
			btItem->setPosition(fX, fY);
			lableTitle->setWidth(200);//自动换行
			lableTitle->setLineBreakWithoutSpace(true);
		}

        auto btnInfo = new ActivityBtnItemInfo;
        btnInfo->m_strTitle = activity.m_strTitle;
        btnInfo->m_strContext = activity.m_strDescription;
        btnInfo->m_pBtActivity = btItem;
        btnInfo->m_pHttpSprite = httpSprite;
        btnInfo->m_pLableTitle = lableTitleContext;
        btnInfo->m_uID = activity.m_uID;
        m_vecBtnItemInfo.push_back(btnInfo);
        fY += uOneItemH;
    }

    if (g_globalActivityInfo.m_vecActivityInfo.size() > 0)
    {
        ActivityInfo actInfo;
        actInfo = g_globalActivityInfo.m_vecActivityInfo.back();
        PopTaskOn(actInfo.m_uID, actInfo.m_uRedefine,actInfo.m_uSecondRedfine, actInfo.m_strTitle);
    }

    m_btnScrollView->setContentSize(Size(BtnScrollW, uAllItemH));
}

void GameActivity::PopTaskOn(unsigned uID, unsigned uType, unsigned uType1,std::string strTitle)
{
    for (auto btnItem : m_vecBtnItemInfo)
    {
        if (btnItem->m_uID == uID)
        {
            m_uRedefineIndex = uType;
            m_uSceondRedefineIndex = uType1;
            btnItem->m_pBtActivity->SetEnable(false);
            btnItem->m_pLableTitle->setTextColor(Color4B(0xff,0xff,0xff,0xff));
            SetLabelString(m_LableContext, btnItem->m_strContext);
            Size sz = m_LableContext->getContentSize();
            unsigned uAllItemH = sz.height;
            unsigned fX = ContextScrollW/2, fY = uAllItemH/2;
            if (uAllItemH < ContextScrollH)
            {
                fY = ContextScrollH - uAllItemH;
                uAllItemH = ContextScrollH;
            }
            else
            {
                fY = 0;
            }
            m_LableContext->setPosition(30, fY);
            float offSet = ContextScrollH - (float)uAllItemH;
            m_contextScrollView->setContentOffset(Vec2(0, offSet), false);
            m_contextScrollView->setContentSize(Size(ContextScrollW, uAllItemH));
            if (uType == 0)
            {
                m_takePartBt->setVisible(false);
            }
            else if (uType > 0)
            {
                m_takePartBt->setVisible(true);
            }
        }
        else
        {
            btnItem->m_pBtActivity->SetEnable(true);
            btnItem->m_pLableTitle->setTextColor(Color4B(0x65,0xae,0x7c,0xff));
        }
    }

    SetLabelString(m_LableTitle, strTitle);
}

void GameActivity::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		//SendClientRequest(ClientRequest::kGetActivityInfo);
   /*     auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
        runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
		UpdateActivityView();
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		//GameDialog::setVisible(false);
		callHideAction() ;
      /*  auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
        runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
    }

}

//----------------------------------------------------推送活动消息、广告----------------------------------------------------
void PushActivities::onKeyEscPressed()
{
}

bool PushActivities::init()
{
    if(!GameLayer::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()))
    {
        return false;
    }
    //弹窗背景
    auto layerBG = Layer::create();
    layerBG->ignoreAnchorPointForPosition(false);
    layerBG->setAnchorPoint(Vec2(0.5,0.5));
    layerBG->setPosition(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight()/2);
    this->addChild(layerBG);
    auto bgLeft = CreateNode1AndPosAddTo<Sprite>("activities/ad_bggreen_half.png",GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 2, layerBG, 0);
    auto bgRight = CreateNode1AndPosAddTo<Sprite>("activities/ad_bggreen_half.png",GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 2, layerBG, 0);
    Size sz = bgLeft->getContentSize();
    layerBG->setContentSize(Size(sz.width*2, sz.height));
    auto szBg = layerBG->getContentSize();
    bgLeft->ignoreAnchorPointForPosition(false);
    bgLeft->setAnchorPoint(Vec2(1, 0.5));
    bgLeft->setPosition(szBg.width/2, szBg.height/2);
    bgRight->setFlipX(true);
    bgRight->ignoreAnchorPointForPosition(false);
    bgRight->setAnchorPoint(Vec2(0, 0.5));
    bgRight->setPosition(szBg.width/2, szBg.height/2);

    auto spriteTitle = CreateNode1AndPosAddTo<Sprite>("activities/ad_title.png", szBg.width/2, szBg.height-10, layerBG, 2);
    //活动海报
    m_httpActivityBg = HttpSprite::create("activities/contentbg.png", Size(695, 370));
    auto szSelf = m_httpActivityBg->getContentSize();
    m_httpActivityBg->setPosition((szBg.width-szSelf.width)/2, 93);
    layerBG->addChild(m_httpActivityBg, 2);
    //标题
    m_labelTitle = CreateLabelMSYHBDAndAnchorPosClrAddTo(26, "", Vec2(0.5, 0.5), 830/2, 134/2, Color4B(0x4d,0x28,0x00,0xff), spriteTitle, 2);

    auto closeBt = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(PushActivities::OnClose, this));
    closeBt->setPosition(szBg.width - 13, szBg.height - 52);
    auto menuClose = Menu::create(closeBt, nullptr);
    menuClose->setPosition(Vec2::ZERO);
    layerBG->addChild(menuClose, 2);
    //closeBt->setVisible(false);

    m_btnReadDetails = MenuItemScale9SpriteWithLabel::create(0, Size(210, 70), CC_CALLBACK_1(PushActivities::OnMenuReadDetails, this));
    m_btnReadDetails->setPosition(0, 48);
    auto btnName = CreateLabelMSYHBD(28, "立即查看");
    m_btnReadDetails->SetLabel(btnName);
    auto menuDetails = Menu::create(m_btnReadDetails, nullptr);
    menuDetails->setPosition(Vec2::ZERO);
    bgRight->addChild(menuDetails);
    m_btnReadDetails->setVisible(false);
    m_uVisibleCount = 0;


    return true;
}
//---------------------------------------------------------------------------------------------------------------
// 更新活动消息
void PushActivities::UpdateActivateRewards(const PushActivityInfo &activityInfo)
{
    char szCurrenContent[512] = {0};
    sprintf(szCurrenContent, "%s",activityInfo.m_pActivityTitle.c_str());
    SetLabelString(m_labelTitle, szCurrenContent);
    SetHttpSpriteURL(m_httpActivityBg, activityInfo.m_pActivityUrl);
    m_uRedefineIndex = activityInfo.m_uRedefine;
    if (activityInfo.m_uRedefine == 0)//不跳转页面
    {
        m_btnReadDetails->setVisible(false);
    }
    else if (activityInfo.m_uRedefine > 0)
    {
        m_btnReadDetails->setVisible(true);
    }
}

void PushActivities::OnMenuReadDetails(cocos2d::Ref* pSend)
{
    //推送消息按钮回调
    //0：不跳转，1：商城，2：消息，3：任务，4：活动，5：俱乐部，6：常规场，7：SNG场，8：MTT场
    //GameTipsManager::getInstance()->hideTips() ;
    switch (m_uRedefineIndex)
    {
    case 0:{}break;
    case 1:{
                Json::Value param;
                param["idx"] = 0;
                SendClientRequest(ClientRequest::kPopMall, param);
                GameTipsManager::getInstance()->clean() ;
           }break;
    case 2:{
                g_pGameFrame->popHallBottomDialog(2);
                GameTipsManager::getInstance()->clean() ;
           }break;
    case 3:{
                g_pGameFrame->popHallBottomDialog(3);
                GameTipsManager::getInstance()->clean() ;
           }break;
    case 4:{
                g_pGameFrame->popHallBottomDialog(4);
                GameTipsManager::getInstance()->clean() ;
           }break;
    case 5:{
                g_pGameFrame->popHallBottomDialog(5);
                GameTipsManager::getInstance()->clean() ;
           }break;
    case 6:{
                SendClientRequest(ClientRequest::kEnterHall2);
                GameTipsManager::getInstance()->clean();
           }break;
    case 7:{
                SendClientRequest(ClientRequest::kEnterGameSngHall);
                GameTipsManager::getInstance()->clean();
           }break;
    case 8:{
                SendClientRequest(ClientRequest::kEnterGameMttHall);
                GameTipsManager::getInstance()->clean();
           }break;
    }
    this->setVisible(false);
    //removeFromParentAndCleanup(true);
}

void PushActivities::OnClose(cocos2d::Ref *pSend)
{
    this->setVisible(false);
    GameTipsManager::getInstance()->hideTips() ;
    //removeFromParentAndCleanup(true);
}

void PushActivities::setVisible(bool bVisible)
{
 
    if (bVisible)
    {
     /*   this->setScale(0.5f) ;
        ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
        CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
        this->runAction(easeBounceOut);*/
		   GameLayer::setVisible(bVisible) ;
		Tools::callScaleAction(this);
    }
	else
	{
		   GameLayer::setVisible(bVisible) ;

           //暂时这样， 后面服务端整改一起替换
           if (m_uVisibleCount == 2)
           {
               Json::Value param;
               param["type"] = "welfare";
               SendClientRequest(ClientRequest::kPopActReward, param);//更新活动奖励-》针对累计登陆
           }
           m_uVisibleCount += 1;
	}
}