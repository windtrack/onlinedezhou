#include "GameFriendList.h"
#include "FrameSprite.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "GameCommon.h"
#include "HttpSprite.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "FrameScale9SpriteButton.h"
#include "Tools.h"
USING_NS_CC;

enum SrcollView	//	设置默认的滚动视图的大小
{
    FriendListScrollW = 700,
    FriendListScrollH = 440
};

void GameFriendList::UpdateFollowList(unsigned uType)
{
	if (m_uType == uType)
	{
		Clear();
		UpdateListView(uType);
	}
}


bool GameFriendList::init()
{
    if(!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }


    //auto title = CreateNode1AndPosAddTo<Sprite>("gamefriendlist/titlefriendlist.png", 470, 568, this, 0);

    auto dialogSz = this->getContentSize();
    m_pLayerTop = GameLayer::create(dialogSz.width, dialogSz.height - FriendListScrollH - 60, false, false);
    m_pLayerTop->setPosition(Vec2(0, FriendListScrollH + 60));
    this->addChild(m_pLayerTop, 1);

    auto title = CreateNode1AndPosAddTo<Sprite>("gamefriendlist/titlefriendlist.png", 470, 568, this, 0);


    m_ScrollList = CreateNode1AndPosAddTo<extension::ScrollView>(Size(FriendListScrollW, FriendListScrollH), 210, 60, this, 0);
    m_ScrollList->setContentSize(Size(700, 1100));
    m_ScrollList->setDirection(extension::ScrollView::Direction::VERTICAL);

    //我的好友
    m_btMyLady = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(GameFriendList::OnMyLady, this));
    m_btMyLady->setPosition(117,469);
    m_btMyLady->setEnabled(false);
	auto menu = Menu::create(m_btMyLady, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    auto label = CreateLabelMSYHBD(24, "我的德女郎");
    m_btMyLady->SetLabel(label);
    m_btMyLady->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_btMyLady->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
    m_btMyLady->setEnabled(false);

    //我关注的
    m_btMyAttention = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(GameFriendList::OnMyAttention, this));
    m_btMyAttention->setPosition(117,401);
    menu = Menu::create(m_btMyAttention, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    label = CreateLabelMSYHBD(24, "我关注的");
    m_btMyAttention->SetLabel(label);
    m_btMyAttention->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_btMyAttention->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

    //关注我的
    m_btAttentMine = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(GameFriendList::OnAttentMine, this));
    m_btAttentMine->setPosition(117, 333);
    menu = Menu::create(m_btAttentMine, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    label = CreateLabelMSYHBD(24, "关注我的");
    m_btAttentMine->SetLabel(label);
    m_btAttentMine->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_btAttentMine->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

    m_btPrePage = FrameSpriteButton::createWithVertFrameSprite("commonbt/pageup.png", false, true);
    m_btNextPage = FrameSpriteButton::createWithVertFrameSprite("commonbt/pagedown.png", false, true);
    m_ScrollList->addChild(m_btNextPage);
    m_ScrollList->addChild(m_btPrePage);
    UpdateListView(2);

	m_uType = 2;

    return true;
}

void GameFriendList::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		m_pLayerTop->setVisible(true);
	}
	else
	{
		m_pLayerTop->setVisible(false);
	}
}

void GameFriendList::OnMyAttention(Ref* pSender)
{
    m_btMyLady->setEnabled(true);
    m_btMyAttention->setEnabled(false);
    m_btAttentMine->setEnabled(true);
	SwitchTo(0);
}

void GameFriendList::OnAttentMine(Ref* pSender)
{
    m_btMyLady->setEnabled(true);
    m_btMyAttention->setEnabled(true);
    m_btAttentMine->setEnabled(false);
	SwitchTo(1);
}

void GameFriendList::OnMyLady(Ref* pSender)
{
    m_btMyLady->setEnabled(false);
    m_btMyAttention->setEnabled(true);
    m_btAttentMine->setEnabled(true);
	SwitchTo(2);
}

void GameFriendList::SwitchTo(unsigned uType)
{
	Clear();
	m_uType = uType;
	//if (g_globalFollowUserInfo.m_vecFollowUser[uType].empty() || uType ==1)//每次点击关注我的都刷新按钮状态
	//{
		Json::Value param;
		param["followListType"] = m_uType;
		param["page"] = 1;
		SendClientRequest(ClientRequest::kGetFollowList, param);
	//}
	//else
	//{
	//	UpdateListView(m_uType);
	//}
}

void GameFriendList::UpdateListView(unsigned uType)
{
    bool bpageUp = false, bpageDown = false, bpageSmall = false;
    Size sz = m_btNextPage->getContentSize();

    unsigned uAllMyLady = g_globalFollowUserInfo.m_vecFollowUser[uType].size();
    unsigned uItemH = 110;
    unsigned uAllItemH  = uItemH*uAllMyLady;
    float fX = FriendListScrollW / 2, fY = 55;

    if (g_globalFollowUserInfo.m_uPageInfo[uType][0] < g_globalFollowUserInfo.m_uPageInfo[uType][1])
    {
        uAllItemH += sz.height;
        bpageDown = true;
    }
    if (g_globalFollowUserInfo.m_uPageInfo[uType][0] > 1)
    {
        uAllItemH += sz.height;
        bpageUp = true;
    }
    if (uAllItemH < FriendListScrollH)
    {
        fY = FriendListScrollH - uAllItemH + fY;
        uAllItemH = FriendListScrollH;
        bpageSmall = true;
    }
    double fy = 440 - (int)uAllItemH;
    m_ScrollList->setContentOffset(Vec2(0, fy), false);

    if (bpageDown)
    {
        fY = sz.height / 2;
        //当前页小于总页数，创建下一页按钮
        m_btNextPage->setVisible(true);
        m_btNextPage->setPosition(fX, fY);
        m_btNextPage->SetHitCB(std::bind(&GameFriendList::OnPageTurnDown, this, g_globalFollowUserInfo.m_uPageInfo[uType][0]));
        fY += uItemH / 2 + fY;
    }
    else
    {
        m_btNextPage->setVisible(false);
    }

    for (auto followList : g_globalFollowUserInfo.m_vecFollowUser[uType])
    {
        auto item = GetListItem(followList.m_uID, uType);
        UpdateListItem(item, followList, uType);
        item->m_pSpriteBgItem->setPosition(fX, fY);
        item->m_pSpriteBgItem->setVisible(true);
        m_vecMyListNew.push_back(item);
        fY += uItemH;
    }

    if (g_globalFollowUserInfo.m_uPageInfo[uType][0] > 1)
    {
        //当前页大于第一页，创建上一页按钮
        fY = fY - (uItemH - uItemH / 2 - sz.height / 2);
        m_btPrePage->setVisible(true);
        m_btPrePage->setPosition(fX, fY);
        m_btPrePage->SetHitCB(std::bind(&GameFriendList::OnPageTurnUp, this, g_globalFollowUserInfo.m_uPageInfo[uType][0]));
    }
    else
    {
        m_btPrePage->setVisible(false);
    }

    m_ScrollList->setContentSize(Size(FriendListScrollW, uAllItemH));
}

void GameFriendList::Clear()
{
	for (auto item : m_vecMyListNew)
    {
        item->m_pSpriteBgItem->setVisible(false);
        m_vecMyListOld.push_back(item);
    }
    m_vecMyListNew.clear();
}

GameFriendList::MyListItem *GameFriendList::GetListItem(unsigned uUID, unsigned uType)
{
    MyListItem *myItem = nullptr;
    if (m_vecMyListOld.empty())
    {
        auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("messagelist/bgitem.png", 1, 1, 330, 46, m_ScrollList, 0);

        //盈利
        auto moneyMark = CreateNode1AndPosAddTo<Sprite>("dt_lebi.png", 349, 52, bgItem, 0);
        auto lableprofit = CreateLabelMSYHAndAnchorPosClrAddTo(22 ,"", Vec2(0, 0.5), 368, 52, Color4B(0xff,0xd2,0x00,0xff), bgItem, 0);

        //名字
        auto lableName = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0, 0.5), 108, 72, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);

        //头像
        auto pHeadPic = CreateUserHeadSprite("defaulticon.png", "gamefriendlist/iconclipmask.png", bgItem, Vec2(10, 8), "gamefriendlist/iconround.png");

        //取消关注
        //auto cancelAttention = FrameSpriteButton::createWithVertFrameSprite("gamefriendlist/ellipse.png", false, true);
        auto cancelAttention = FrameScale9SpriteButton::createWithScale9Sprite(3, Size(114, 86), false, true);
        cancelAttention->setPosition(618, 54);
        bgItem->addChild(cancelAttention);

        //查看详情
        auto btDetail = LayerButton::create(false, true);
        bgItem->addChild(btDetail);
        auto szBgItem = bgItem->getContentSize();
        btDetail->setContentSize(szBgItem);

        auto labelAttent =  CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "", Vec2(0.5,0.5), 0, 0, Color4B::WHITE, cancelAttention, 0);
        labelAttent->setWidth(60);//自动换行
        labelAttent->setLineBreakWithoutSpace(true);

        auto pItem = new MyListItem;
        pItem->m_uUserID = uUID;
        pItem->m_pSpriteBgItem = bgItem;
        pItem->m_pLableProfit = lableprofit;
        pItem->m_pLableUserName = lableName;
        pItem->m_pHeadPic = pHeadPic;
        pItem->m_pBtCancelAttention = cancelAttention;
        pItem->m_pBtDetail = btDetail;
        pItem->m_pLabelBtnName = labelAttent;
        myItem = pItem;
    }
    else
    {
        myItem = m_vecMyListOld.back();
        m_vecMyListOld.pop_back();
    }

    if (uType == 0)//我关注的按钮操作：取消关注
    {
        myItem->m_pBtCancelAttention->SetHitCB([uUID](Ref*){
            Json::Value param;
            param["tid"] = uUID;
            param["op"] = 0;
            SendClientRequest(ClientRequest::kFollowUser, param);
        });
    }
    else if (uType == 1)//关注我的按钮操作：添加关注
    {
        myItem->m_pBtCancelAttention->SetHitCB([uUID, myItem](Ref*){
            Json::Value param;
            param["type"] = 1;
            param["data"] = 1;
            param["tuid"] = uUID;
            SendClientRequest(ClientRequest::kOperateOtherPlayer, param);
            myItem->m_pBtCancelAttention->setVisible(false);
        });
    }

    myItem->m_pBtDetail->SetHitCB([uUID](Ref*){
		Json::Value param;
		param["tid"] = uUID;
		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
    });

    return myItem;
}

void GameFriendList::UpdateListItem(GameFriendList::MyListItem* item, const FollowUserInfo& followUserInfo, unsigned uType)
{
    if (uType == 0 || uType == 1)
    {
        item->m_pBtCancelAttention->setVisible(true);
        auto szBt = item->m_pBtCancelAttention->getContentSize();
        auto sz = item->m_pSpriteBgItem->getContentSize();
        item->m_pBtDetail->setContentSize(Size(sz.width - szBt.width, sz.height));
        if (uType == 0)
            SetLabelString(item->m_pLabelBtnName, "取消关注");
        else
            SetLabelString(item->m_pLabelBtnName, "关注");
        if (followUserInfo.m_uAttendStatus == 1)
        {
            item->m_pBtCancelAttention->setVisible(false);
        }
    }
    else
    {
        item->m_pBtCancelAttention->setVisible(false);
        auto sz = item->m_pSpriteBgItem->getContentSize();
        item->m_pBtDetail->setContentSize(sz);
    }

	item->m_uUserID = followUserInfo.m_uID;
    SetLabelString(item->m_pLableUserName, followUserInfo.m_strNickName);

    std::string strProfit = FormatCash4(followUserInfo.m_dCash);
    SetLabelString(item->m_pLableProfit, strProfit);

	SetIconUrl(item->m_pHeadPic, followUserInfo.m_strIcon);
}

void GameFriendList::OnPageTurnUp(unsigned uCurPage)
{
	Clear();
	Json::Value param;
	param["followListType"] = m_uType;
	param["page"] = g_globalFollowUserInfo.m_uPageInfo[m_uType][0] - 1;;
	SendClientRequest(ClientRequest::kGetFollowList, param);
}
void GameFriendList::OnPageTurnDown(unsigned uCurPage)
{
	Clear();
	Json::Value param;
	param["followListType"] = m_uType;
	param["page"] = g_globalFollowUserInfo.m_uPageInfo[m_uType][0] + 1;// m_uPage;
	SendClientRequest(ClientRequest::kGetFollowList, param);
}

void GameFriendList::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		Clear();
		Json::Value param;
		param["followListType"] = m_uType;
		param["page"] = 1;
		SendClientRequest(ClientRequest::kGetFollowList, param);

		/* auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		//GameDialog::setVisible(false);
		callHideAction() ;
       /* auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
        runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
    }
}