#include "UserItem.h"
#include "FrameSpriteButton.h"
#include "FrameScale9SpriteButton.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"
#include "HttpSprite.h"
#include "Util.h"
#include "Tools.h"
#include "GameHall.h"
#include "GameFrame.h"
#include "GameConfig.h"
USING_NS_CC ;


enum 
{
	UserItemScrollW = 806,
	UserItemScrollH = 461
};

enum BgItem
{
	ListBgItemW = 806,
	ListBgItemH = 112
};

bool UserItem::init()
{
	if (!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}
	s_dataLoading = nullptr ;
	Size sz = this->getContentSize() ;


	//标题
	Sprite* spriteTitle = Sprite::createWithSpriteFrameName("daoju_title.png");
	spriteTitle->setPosition(469,569);
	this->addChild(spriteTitle);


	//滚动视图
	m_ScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(UserItemScrollW, UserItemScrollH), sz.width/2, 275, this, 0);
	m_ScrollView->ignoreAnchorPointForPosition(false);
	m_ScrollView->setAnchorPoint(Point(0.5,0.5)) ;
	m_ScrollView->setContentSize(Size(UserItemScrollW,UserItemScrollH));
	m_ScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	//m_pScrollView->setContentSize(Size(ListScrollW,ListScrollH));
	m_ScrollView->setBounceable(true);
	m_ScrollView->setSwallowsTouches(false) ;
	m_ScrollView->setVisible(true);

	
	//道具为空的界面
	m_withoutItemLayer = Node::create() ;
	this->addChild(m_withoutItemLayer) ;

	m_spriteGril = Sprite::createWithSpriteFrameName("itembag_heguan.png");
	m_spriteGril->setPosition(m_spriteGril->getContentSize().width/2,m_spriteGril->getContentSize().height/2+20) ;
	m_withoutItemLayer->addChild(m_spriteGril);

#if OpenItemShop
	m_lableWithoutItem = CreateLabelMSYHAndAnchorPosClrAddTo(28,"矮油，你当前没有道具耶，赶紧去商城看看吧",Vec2(0.5, 0.5), sz.width/2+100, sz.height/2,
		Color4B::WHITE, m_withoutItemLayer, 0);
#else
	m_lableWithoutItem = CreateLabelMSYHAndAnchorPosClrAddTo(28,"矮油，你当前没有道具耶",Vec2(0.5, 0.5), sz.width/2+100, sz.height/2,
		Color4B::WHITE, m_withoutItemLayer, 0);
#endif




	//加入按钮
	//m_btItemShop = FrameSpriteButton::createWithVertFrameSprite("userinfo/btgreen2.png", false, true);
    m_btItemShop = FrameScale9SpriteButton::createWithScale9Sprite(1, Size(234, 74),false, true);
	auto buttonDes = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"道具商城",Vec2(0.5,0.5),0,0,Color4B::WHITE,m_btItemShop,0);
	m_btItemShop->setPosition(726,83);
	m_withoutItemLayer->addChild(m_btItemShop);
	m_btItemShop->SetHitCB(std::bind(&UserItem::callItemShop, this));


#if !OpenItemShop
	m_btItemShop->setVisible(false) ;
#endif


	m_withoutItemLayer->setVisible(false) ;

	return true ;
}

void UserItem::cleanView()
{
	std::vector<UItemLabel*>::iterator iter;

	while (!m_vecItemListOld.empty())
	{
		iter = m_vecItemListOld.begin();
		UItemLabel* item = m_vecItemListOld.at(0) ;
		item->m_pSpriteBGItem->removeFromParent() ;
		m_vecItemListOld.erase(iter) ;
	}
}

void UserItem::updateItemView()
{


	unsigned uCount = g_globalUserItemList.m_vecUserItemList.size();

	m_withoutItemLayer->setVisible(uCount==0) ;

	unsigned uItemH = ListBgItemH;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < UserItemScrollH)
	{
		fRealH = UserItemScrollH;
	}
	float fX = UserItemScrollW / 2;
	float fY = fRealH - uItemH / 2 ;


	for(auto& itemLabel:g_globalUserItemList.m_vecUserItemList)
	{
		auto item = GetListItem(itemLabel);		
		UpdateItem(item, itemLabel);
		item->m_pSpriteBGItem->setPosition(fX, fY);
		item->m_pSpriteBGItem->setVisible(true); 
		m_vecItemListOld.push_back(item);
		fY -= uItemH;     
	}
	float temp = fRealH - UserItemScrollH;

	//每次都显示滚动框的最上面
	m_ScrollView->setContentOffset(Vec2(0,-temp-10), false);
	m_ScrollView->setContentSize(Size(UserItemScrollW,fRealH+15));

	setShowDataLoading(false) ;
}

UserItem::UItemLabel* UserItem::GetListItem(const UserItemInfo& itemInfo)
{
	UItemLabel* item = nullptr ;

	//背景
	auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("family/ranklistbg.png", 1, 2, UserItemScrollW / 2, 0, m_ScrollView, 0);
	bgItem->ignoreAnchorPointForPosition(false) ;
	bgItem->setAnchorPoint(Point(0.5,0.5) );
	//图标
	auto  httpIconSprite = CreateNode2AndPosAddTo<HttpSprite>("sigin_default.png", cocos2d::Size::ZERO, 26, 14, bgItem, 0);
	httpIconSprite->ignoreAnchorPointForPosition(false) ;
	httpIconSprite->setAnchorPoint(Point(0.5,0.5) );
	httpIconSprite->setPosition(93,52) ;

	//数量
	auto  spriteCountBlock = Sprite::createWithSpriteFrameName("sigin_itemnumblock.png") ;
	spriteCountBlock->setPosition(156,12) ;
	spriteCountBlock->setAnchorPoint(Point(0.5,0.5)) ;
	httpIconSprite->addChild(spriteCountBlock,1) ;

	auto  lableCount = CreateLabelMSYHAndAnchorPosClrAddTo(16,"",Vec2(0.5,0.5),30,10,Color4B::WHITE,spriteCountBlock,0);

	auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0,0.5),178,73,Color4B::WHITE,bgItem,0);
	auto labelDate = CreateLabelMSYHAndAnchorPosClrAddTo(20,"",Vec2(0,0.5),460,73,Color4B(0x79,0xdc,0x98,0xff),bgItem,0);
	auto labelDes = CreateLabelMSYHAndAnchorPosClrAddTo(20,"",Vec2(0,0.5),177,25,Color4B(0x50,0xbd,0x2e,0xff),bgItem,0);


	//加入按钮
	//auto buttonDeal = FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn1.png", false, true);
    auto buttonDeal = FrameScale9SpriteButton::createWithScale9Sprite(3, Size(114, 86),false, true);
	auto buttonDes = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"加入",Vec2(0.5,0.5),0,0,Color4B::WHITE,buttonDeal,0);
	buttonDeal->setPosition(726,53);
	bgItem->addChild(buttonDeal);
	buttonDeal->SetEnable(true) ;

	auto pItem = new UItemLabel;
	pItem->m_pSpriteBGItem = bgItem;
	pItem->m_pSpriteIcon = httpIconSprite;
	pItem->m_pSpriteCountBlock = spriteCountBlock;
	pItem->m_pLabelCount = lableCount;
	pItem->m_pLabelName = labelName;
	pItem->m_pLabelExpDate = labelDate;
	pItem->m_pLabelDes = labelDes;
	pItem->m_pBt = buttonDeal ;
	pItem->m_pLabelBtDes = buttonDes ;
	item = pItem;

	return item ;
}
void UserItem::UpdateItem(UserItem::UItemLabel *item, const UserItemInfo& itemInfo)
{
	char buff[64] ;
	item->m_pSpriteIcon->useDownLoadSize() ;
	SetHttpSpriteURL(item->m_pSpriteIcon, itemInfo.m_iconUrl);
	item->m_pSpriteIcon->setScale(0.77) ;
	//item->m_pSpriteIcon->setContentSize(item->m_pSpriteIcon->getContentSize()) ;
	//item->m_pSpriteIcon->ignoreAnchorPointForPosition(false) ;
	//item->m_pSpriteIcon->setAnchorPoint(Point(0.5,0.5) );
	//item->m_pSpriteIcon->setPosition(93,58) ;

	SetLabelString(item->m_pLabelName,itemInfo.m_name.c_str()) ;

	if (itemInfo.m_count > 99)
	{
		SetLabelString(item->m_pLabelCount,"99+") ;
	}
	else
	{
		sprintf(buff,"%d",itemInfo.m_count) ;
		SetLabelString(item->m_pLabelCount,buff) ;
	}

	//控制显示数字框
	item->m_pSpriteCountBlock->setVisible(!(itemInfo.m_count<1)) ;
	//描述
	SetLabelString(item->m_pLabelDes,itemInfo.m_des.c_str()) ;

	//控制按钮的显示
	switch (itemInfo.m_state)
	{
		//报名
	case UItemDealType_siginup:
		sprintf(buff,"有效期至%s",itemInfo.m_expDate.c_str()) ;
		SetLabelString(item->m_pLabelExpDate,buff) ;
		item->m_pBt->SetEnable(true) ;
		item->m_pLabelBtDes->setTextColor(Color4B::WHITE) ;
		break ;

		//兑换
	case UItemDealType_exchange:
		sprintf(buff,"有效期至%s",itemInfo.m_expDate.c_str()) ;
		SetLabelString(item->m_pLabelExpDate,buff) ;
		item->m_pBt->SetEnable(true) ;
		item->m_pLabelBtDes->setTextColor(Color4B::WHITE) ;
		break ;

		//未使用
	case UItemDealType_NoUse:
		sprintf(buff,"%s开始生效",itemInfo.m_expDate.c_str()) ;
		SetLabelString(item->m_pLabelExpDate,buff) ;
		item->m_pBt->SetEnable(false) ;
		item->m_pLabelBtDes->setTextColor(Color4B(0x3d,0x9c,0x5a,0xff)) ;
		break ;

		//已生效
	case UItemDealType_HasUse:
		sprintf(buff,"有效期至%s",itemInfo.m_expDate.c_str()) ;
		SetLabelString(item->m_pLabelExpDate,buff) ;
		item->m_pBt->SetEnable(false) ;
		item->m_pLabelBtDes->setTextColor(Color4B(0x3d,0x9c,0x5a,0xff)) ;
		break ;

		//使用
	case UItemDealType_ToUse:
		sprintf(buff,"有效期至%s",itemInfo.m_expDate.c_str()) ;
		SetLabelString(item->m_pLabelExpDate,buff) ;
		item->m_pBt->SetEnable(true) ;
		item->m_pLabelBtDes->setTextColor(Color4B::WHITE) ;
		break ;
	case UItemDealType_Mttsiginup:
		sprintf(buff,"有效期至%s",itemInfo.m_expDate.c_str()) ;
		SetLabelString(item->m_pLabelExpDate,buff) ;
		item->m_pBt->SetEnable(true) ;
		item->m_pLabelBtDes->setTextColor(Color4B::WHITE) ;
		break ;
	default:
		break;
	}
	//按钮提示文字
	SetLabelString(item->m_pLabelBtDes,itemInfo.m_btdes.c_str()) ;
	//回调操作
	item->m_pBt->SetHitCB(std::bind(&UserItem::menuClickItem, this,itemInfo));

}


void UserItem::menuClickItem(UserItemInfo itemInfo)
{


	switch (itemInfo.m_state)
	{
		//报名
	case UItemDealType_siginup:
		{
			//如果数据没过来就坑了
			if (g_globalSngHallInfo.m_sngTableInfo.size()>0 &&itemInfo.m_type>=0)
			{
				GlobalSngTableInfo tableInfo = g_globalSngHallInfo.m_sngTableInfo.at(itemInfo.m_type) ;
				g_pGameFrame->callShowSngSignUp(true,tableInfo);
			}
		}
		break ;

	case UItemDealType_Mttsiginup:
		{
			this->setVisible(false) ;
			g_pGameFrame->EnterGameMttHall() ;
		}
		break ;

		//兑换
	case UItemDealType_exchange:
		//g_pGameFrame->showGameMall(2) ;
		{
			Json::Value param;
			param["idx"] = 2;
			SendClientRequest(ClientRequest::kPopMall, param);
		}
		
		break ;

		//未使用
	case UItemDealType_NoUse:

		break ;

		//已生效
	case UItemDealType_HasUse:

		break ;

		//使用
	case UItemDealType_ToUse:

	default:
		break;
	}



}

void UserItem::setShowDataLoading(bool flag)
{
	if (s_dataLoading == nullptr)
	{

		s_dataLoading = Tools::creatDataLoading(this,476, 277,99) ;
	}
	s_dataLoading->setVisible(flag) ;

}

void UserItem::setVisible(bool bVisible)
{
	if(bVisible)
	{
		cleanView() ;
		setShowDataLoading(true) ;
		SendClientRequest(ClientRequest::kGetSngHallInfo);
		SendClientRequest(ClientRequest::kCallUserItemList);


	}
	GameDialog::setVisible(bVisible) ;
}

void UserItem::callItemShop()
{
	GameDialog::setVisible(false) ;


	g_pGameFrame->showGameMall(3) ;
}