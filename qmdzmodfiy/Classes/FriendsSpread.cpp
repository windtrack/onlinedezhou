#include "FriendsSpread.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "LineLabel.h"
#include "Tools.h"
#include "Util.h"
USING_NS_CC;

enum SrcollView	//	设置默认的滚动视图的大小
{
    ListScrollW = 620,
    ListScrollH = 325
};

bool FriendsSpread::init()
{
    if(!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }

    //屏蔽滚动视图外部的触摸事件
    //auto dialogSz = this->getContentSize();
    //m_pLayerTop = GameLayer::create(dialogSz.width, dialogSz.height - ListScrollH - 60, false, false);
    //m_pLayerTop->setPosition(Vec2(0, ListScrollH + 60));
    //this->addChild(m_pLayerTop, 1);

    //内容的暗色背景
    m_pScale9ContentBg = ui::Scale9Sprite::create("gamemtt/matchstatebg.png", Rect(0,0,140,140), Rect(40,40,60,60));
    m_pScale9ContentBg->setPreferredSize(Size(630, 460));
    m_pScale9ContentBg->setPosition(575, 278);
    this->addChild(m_pScale9ContentBg);

    //推广标题
    auto title = CreateNode1AndPosAddTo<Sprite>("friendspread/tuiguang_title.png", 470, 568, this, 0);

    m_menuSpreadPerson = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(FriendsSpread::OnSpreadPerson, this));
    m_menuSpreadPerson->setPosition(117,469);
    m_menuSpreadPerson->setEnabled(false);
    auto menu = Menu::create(m_menuSpreadPerson, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    auto label = CreateLabelMSYHBD(24, "推广员");
    m_menuSpreadPerson->SetLabel(label);
    m_menuSpreadPerson->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_menuSpreadPerson->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
    m_menuSpreadPerson->setEnabled(false);

    m_menuMySpread = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(FriendsSpread::OnMySpread, this));
    m_menuMySpread->setPosition(117,401);
    menu = Menu::create(m_menuMySpread, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    label = CreateLabelMSYHBD(24, "我的推广");
    m_menuMySpread->SetLabel(label);
    m_menuMySpread->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_menuMySpread->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

    //推广员绑定前层
    auto bgSz = this->getContentSize();
    m_layerNotBind = Layer::create();
    m_layerNotBind->setContentSize(m_pScale9ContentBg->getContentSize());
    m_layerNotBind->setPosition(0, 0);
    m_pScale9ContentBg->addChild(m_layerNotBind);
    std::string strDes = "1、您在游戏过程中，推广员可获得奖励，详情加官方QQ群咨询\n推广专员\n2、绑定邀请者ID后不可更改哦！";
    CreateLabelMSYHBDAndAnchorPosClrAddTo(24, "推广员ID:", Vec2(0, 0.5), 40, 355, Color4B::WHITE, m_layerNotBind, 0);
    auto labelDes = CreateLabelMSYHAndAnchorPosClrAddTo(20, strDes, Vec2(0, 0), 40, 30, Color4B(0x50,0xbd,0x2e,0xff), m_layerNotBind, 0);
    labelDes->setAlignment(TextHAlignment::LEFT);

    m_editSpreadID = CreateNode2AndPosAddTo<ui::EditBox>(Size(275, 70), ui::Scale9Sprite::create("friendspread/textbg.png") , 295, 355, m_layerNotBind, 0);
    m_editSpreadID->setFont("fzlt.ttf", 26);
    m_editSpreadID->setFontColor(Color4B(0x50, 0xbd, 0x2e, 0xff));
    m_editSpreadID->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editSpreadID->setInputMode(ui::EditBox::InputMode::NUMERIC);
    m_editSpreadID->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editSpreadID->setDelegate(this);

    auto btBind = MenuItemScale9SpriteWithLabel::create(0, Size(140, 70), CC_CALLBACK_1(FriendsSpread::OnBindSpread, this));
    btBind->setPosition(517,355);
    menu = Menu::create(btBind, nullptr);
    menu->setPosition(Vec2::ZERO);
    m_layerNotBind->addChild(menu);
    label = CreateLabelMSYHBD(30, "绑定");
    btBind->SetLabel(label);
    //btBind->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    //btBind->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

    //绑定后的层
    m_layerAlreayBind = Layer::create();
    m_layerAlreayBind->setContentSize(m_pScale9ContentBg->getContentSize());
    m_layerAlreayBind->setPosition(0, 0);
    m_pScale9ContentBg->addChild(m_layerAlreayBind);
    m_pHttpSpreadHead = CreateUserHeadSprite("defaulticon.png", "gamefriendlist/iconclipmask.png", m_layerAlreayBind, Vec2(112, 300), "gamefriendlist/iconround.png");
    m_pHttpMyHead = CreateUserHeadSprite("defaulticon.png", "gamefriendlist/iconclipmask.png", m_layerAlreayBind, Vec2(422, 300), "gamefriendlist/iconround.png");
    CreateNode1AndPosAddTo<Sprite>("friendspread/tuiguang_arrow.png", 312, 345, m_layerAlreayBind, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(22, "推广员", Vec2(0, 0.5), 127, 280, Color4B(0xff,0xd2,0x00,0xff), m_layerAlreayBind, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(22, "我", Vec2(0, 0.5), 457, 280, Color4B::WHITE, m_layerAlreayBind, 0);
    m_labelSpreadName = CreateLabelMSYHAndAnchorPosClrAddTo(24,"昵称:", Vec2(0, 0.5), 127, 250, Color4B::WHITE, m_layerAlreayBind, 0);
    m_labelSpreadID = CreateLabelMSYHAndAnchorPosClrAddTo(24,"推广员ID:", Vec2(0, 0.5), 127, 220, Color4B::WHITE, m_layerAlreayBind, 0);
    labelDes = CreateLabelMSYHAndAnchorPosClrAddTo(20, strDes, Vec2(0, 0), 40, 30, Color4B(0x50,0xbd,0x2e,0xff), m_layerAlreayBind, 0);
    labelDes->setAlignment(TextHAlignment::LEFT);


    //推广列表滚动视图
    char buff[64] = {0};
    m_layerSpreadList = Layer::create();
    m_layerSpreadList->setContentSize(m_pScale9ContentBg->getContentSize());
    m_layerSpreadList->setPosition(0, 0);
    m_pScale9ContentBg->addChild(m_layerSpreadList);
    m_layerSpreadList->setVisible(false);

    m_pLineLabelMyID = LineLabel::create();
    m_pLineLabelMyID->ignoreAnchorPointForPosition(false) ;
    m_pLineLabelMyID->setAnchorPoint(Point(0,0.5));

    m_pLineLabelMyID->setPosition(40, 415);
    m_layerSpreadList->addChild(m_pLineLabelMyID);

    m_pLineLabelNum = LineLabel::create();
    m_pLineLabelNum->ignoreAnchorPointForPosition(false) ;
    m_pLineLabelNum->setAnchorPoint(Point(0,0.5));
    m_pLineLabelNum->setPosition(40, 380);
    m_layerSpreadList->addChild(m_pLineLabelNum);

    CreateLabelMSYHAndAnchorPosClrAddTo(18, "您在游戏过程中，推广员可获得奖励，详情加官方QQ群咨询推广专员", Vec2(0, 0.5), 40, 355, Color4B(0x50,0xbd,0x2e,0xff), m_layerSpreadList, 0);
    m_ScrollList = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 0, 5, m_layerSpreadList, 0);
    m_ScrollList->setContentSize(Size(ListScrollW, ListScrollH));
    m_ScrollList->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_labelNoSpread = CreateLabelMSYHAndAnchorPosClrAddTo(24, "您还没有向好友分享全民德州呢\n赶紧复制推广ID告诉好友吧~", Vec2(0.5,0.5), 310, 170, Color4B(0x50,0xbd,0x2e,0xff), m_layerSpreadList, 0);

    m_pFaildDialog = nullptr;

    m_labelEditRetFaild = CreateLabelMSYHAndAnchorPosClrAddTo(20, "请确认推广员ID是否正确", Vec2(0, 0.5), 160, 310, Color4B(0x50,0xbd,0x2e,0xff), m_layerNotBind, 0);
    m_labelEditRetFaild->setVisible(false);

    updateSpreadInfo();

    return true;
}

void FriendsSpread::updateSpreadInfo()
{
    if (m_menuSpreadPerson->isEnabled())
    {
        return;
    }

    if (!g_globalFriendsSpread.m_bIsSpread)
    {
        m_layerNotBind->setVisible(true);
        m_layerAlreayBind->setVisible(false);
    }
    else
    {
        m_layerNotBind->setVisible(false);
        m_layerAlreayBind->setVisible(true);
        char buff[64] = {0};
        sprintf(buff, "推广ID:%d", g_globalFriendsSpread.m_iUserID);
        SetLabelString(m_labelSpreadID, buff);
        SetLabelString(m_labelSpreadName, g_globalFriendsSpread.m_strName);
        SetIconUrl(m_pHttpMyHead, g_globalMyData.GetIconURL());
        SetIconUrl(m_pHttpSpreadHead, g_globalFriendsSpread.m_strIcon);
    }
}

void FriendsSpread::OnSpreadPerson(Ref *pSend)
{
    m_menuSpreadPerson->setEnabled(false);
    m_menuMySpread->setEnabled(true);
    updateSpreadInfo();
    m_layerSpreadList->setVisible(false);
}

void FriendsSpread::OnMySpread(Ref *pSend)
{
    m_menuSpreadPerson->setEnabled(true);
    m_menuMySpread->setEnabled(false);
    m_layerSpreadList->setVisible(true);
    m_layerNotBind->setVisible(false);
    m_layerAlreayBind->setVisible(false);
    //updateListView();
}

void FriendsSpread::OnBindSpread(Ref *pSend)
{
    Json::Value param;
    param["spreadID"] = m_editSpreadID->getText();
    SendClientRequest(ClientRequest::kBindSpread, param);
    //popBindFaild();
}

void FriendsSpread::updateListView()
{
    for (auto item : m_vecMyListNew)
    {
        item->m_pSpriteBgItem->setVisible(false);
        m_vecMyListOld.push_back(item);
    }
    m_vecMyListNew.clear();

    char buff[32] = {0};
    m_pLineLabelMyID->resetLable();
    m_pLineLabelNum->resetLable();
    m_pLineLabelMyID->Appand(24,"fzlt.ttf","我的推广ID: ",Color4B::WHITE);
    sprintf(buff, "%d", g_globalMyData.m_iUserId);
    m_pLineLabelMyID->Appand(24,"fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));

    m_pLineLabelNum->Appand(24, "fzlt.ttf", "我推广的好友", Color4B::WHITE);
    m_pLineLabelNum->Appand(24, "fzlt.ttf", "（", Color4B::WHITE);
    sprintf(buff, "%d人", g_globalFriendsSpread.m_vecFriendsSpread.size());
    m_pLineLabelNum->Appand(24, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
    m_pLineLabelNum->Appand(24, "fzlt.ttf", "）", Color4B::WHITE);

    if (g_globalFriendsSpread.m_vecFriendsSpread.size() > 0)
        m_labelNoSpread->setVisible(false);
    else
        m_labelNoSpread->setVisible(true);
    unsigned uOneItemH = 110;//条目宽度
    unsigned uAllItemH = g_globalFriendsSpread.m_vecFriendsSpread.size() * uOneItemH;
    float fY = uAllItemH - 53;//item高等于106
    float fX = ListScrollW/2;

    uAllItemH = uAllItemH - ListScrollH >= 0 ? uAllItemH : ListScrollH;

    for (auto listItem : g_globalFriendsSpread.m_vecFriendsSpread)
    {
        auto item = GetListItem(listItem.m_uID);
        UpdateListItem(item, listItem);
        item->m_pSpriteBgItem->setPosition(fX, fY);
        item->m_pSpriteBgItem->setVisible(true);
        m_vecMyListNew.push_back(item);
        fY -= uOneItemH;
    }
    float offset = ListScrollH - (float)uAllItemH;
    m_ScrollList->setContentOffset(Vec2(0, offset));
    m_ScrollList->setContentSize(Size(ListScrollW, uAllItemH));
}

FriendsSpread::MyListItem *FriendsSpread::GetListItem(unsigned uUID)
{
    MyListItem *myItem = nullptr;
    if (m_vecMyListOld.empty())
    {
        //auto bgItem = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 0, 0, m_ScrollList, 0);
        auto bgItem = ui::Scale9Sprite::create("messagelist/bgitem.png", Rect(0,0,686,106), Rect(100,45,486,16));
        bgItem->setPreferredSize(Size(590, 106));
        bgItem->setPosition(0, 0);
        m_ScrollList->addChild(bgItem);

        auto labelUserID = CreateLabelMSYHAndAnchorPosClrAddTo(22 ,"", Vec2(0, 0.5), 368, 52, Color4B(0x79,0xdc,0x98,0xff), bgItem, 0);
        //名字
        auto lableName = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0, 0.5), 108, 52, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);
        //头像
        auto pHeadPic = CreateUserHeadSprite("defaulticon.png", "gamefriendlist/iconclipmask.png", bgItem, Vec2(10, 8), "gamefriendlist/iconround.png");

        //查看详情
        auto btDetail = LayerButton::create(false, true);
        bgItem->addChild(btDetail);
        auto szBgItem = bgItem->getContentSize();
        btDetail->setContentSize(szBgItem);

        auto pItem = new MyListItem;
        pItem->m_uUserID = uUID;
        pItem->m_pSpriteBgItem = bgItem;
        pItem->m_pLableUserID = labelUserID;
        pItem->m_pLableUserName = lableName;
        pItem->m_pHeadPic = pHeadPic;
        pItem->m_pBtDetail = btDetail;
        myItem = pItem;
    }
    else
    {
        myItem = m_vecMyListOld.back();
        m_vecMyListOld.pop_back();
    }

    myItem->m_pBtDetail->SetHitCB([uUID](Ref*){
        Json::Value param;
        param["tid"] = uUID;
        SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
    });

    return myItem;
}

void FriendsSpread::UpdateListItem(FriendsSpread::MyListItem* item, const FollowUserInfo& userInfo)
{
    SetLabelString(item->m_pLableUserName, userInfo.m_strNickName);

    char buff[32] = {0};
    sprintf(buff, "ID:%d", userInfo.m_uID);
    SetLabelString(item->m_pLableUserID, buff);

    SetIconUrl(item->m_pHeadPic, userInfo.m_strIcon);
}

void FriendsSpread::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    std::string strID = editBox->getText();
    if (strID.length() != 9)
    {
        m_labelEditRetFaild->setVisible(true);
    }
    else
    {
        m_labelEditRetFaild->setVisible(false);
    }
}

void FriendsSpread::OnBindResult(Ref *pSend)
{
    m_pFaildDialog->setVisible(false);
}

void FriendsSpread::popBindResult(bool bSuccess)
{
    if (m_pFaildDialog == nullptr)
    {
        auto bgSz = this->getContentSize();
        m_pFaildDialog = GameDialog::create("userinfo/bgdialog0.png", false);
        m_pFaildDialog->ignoreAnchorPointForPosition(false);
        m_pFaildDialog->setAnchorPoint(Vec2(0.5,0.5));
        m_pFaildDialog->setPosition(bgSz.width/2, bgSz.height/2);
        this->addChild(m_pFaildDialog);

        auto popDlgSz = m_pFaildDialog->getContentSize();
        /*std::string strTips = "";
        std::string strBtnName = "";
        if (!bSuccess)
        {
            strTips = "哎呀！绑定失败了！\n请检查ID是否正确";
            strBtnName = "知道了";
        }
        else
        {
            strTips = "恭喜您绑定成功！";
            strBtnName = "确定";
            updateSpreadInfo();
        }*/
        m_labelTips = CreateLabelMSYHAndAnchorPosClrAddTo(28, "", Vec2(0.5, 0.5), popDlgSz.width/2, popDlgSz.height/2+20, Color4B::WHITE, m_pFaildDialog, 0);

        auto btOK = MenuItemScale9SpriteWithLabel::create(0, Size(200, 70), CC_CALLBACK_1(FriendsSpread::OnBindResult, this));
        btOK->setPosition(popDlgSz.width/2, 55);
        auto menu = Menu::create(btOK, nullptr);
        menu->setPosition(Vec2::ZERO);
        m_pFaildDialog->addChild(menu);
        m_labelBtnName = CreateLabelMSYHBD(30, "");
        btOK->SetLabel(m_labelBtnName);
    }
    std::string strTips = "";
    std::string strBtnName = "";
    if (!bSuccess)
    {
        strTips = "哎呀！绑定失败了！\n请检查ID是否正确";
        strBtnName = "知道了";
    }
    else
    {
        strTips = "恭喜您绑定成功！";
        strBtnName = "确定";
        updateSpreadInfo();
    }
    SetLabelString(m_labelTips, strTips);
    SetLabelString(m_labelBtnName, strBtnName);
    m_pFaildDialog->setVisible(true);
}

bool FriendsSpread::getVisible()
{
    return m_bIsVisible;
}

void FriendsSpread::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    m_bIsVisible = bVisible;
    if (bVisible)
    {
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		callHideAction() ;
    }
}