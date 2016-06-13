#include "SngRankList.h"
#include "GrabTouch.h"
#include "FrameSprite.h"
#include "GameCommon.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include <vector>
#include "Tools.h"

enum SrcollView	//	设置默认的滚动视图的大小
{
	ListScrollW = 710,
	ListScrollH = 440
};

enum FriendsWorldRankList	//	好友和世界排名
{
	EGoodFriendsRankList,	// 好友排名
	ETheWorldRadnkList		// 世界排名
};

enum RankMenuTag			// 按钮的标记
{
	ERankWeeksRecordMenuTag,	// 大师分周榜
	EWeeksProfitMenuTag,		// SNG大师分周榜
	EDaySingleMenuTag,			// MTT大师分周榜
};

USING_NS_CC;
//---------------------------------------------------------------------------------------------------------------
SngRankList::SngRankList()
{
	nServerCurrentPage = 0;
	nServerMaxPage = 0;
	m_uRecordFriendWorldID = 0;
	m_uRecordMenuCurrentStatusId = 0;
	for (auto& item : m_bHasRequest)
	{
		item = false;
	}
}
//---------------------------------------------------------------------------------------------------------------
SngRankList::~SngRankList()
{

}

//---------------------------------------------------------------------------------------------------------------
void SngRankList::onSetVisible(bool bVisible)
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

bool SngRankList::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}
    // 排行榜的滚动View
    m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 120, 20, this, 0);
    m_pScrollView->setContentSize(Size(658, 1050));
    m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);

    auto dialogSz = this->getContentSize();
    m_pLayerTop = GameLayer::create(dialogSz.width, dialogSz.height - ListScrollH - 60, false, false);
    m_pLayerTop->setPosition(Vec2(0, ListScrollH + 60));
    this->addChild(m_pLayerTop, 1);

	// 好友排名的按钮回调
	m_pGoodFriendMenu = MenuItemSpriteWithLabel::create("ranklist/goodlist.png", 3, CC_CALLBACK_1(SngRankList::GoodFriendMenuCallback, this, 0));
	m_pGoodFriendMenu->setAnchorPoint(Vec2(0,0));
	m_pGoodFriendMenu->setPosition(217,529);
	m_pGoodFriendMenu->setEnabled(false);
	auto goodFriendMenu = Menu::create(m_pGoodFriendMenu,nullptr);
	goodFriendMenu->setPosition(Vec2::ZERO);
	addChild(goodFriendMenu, 2);

	// 世界排名的按钮回调
	m_pTheWorldMenu = MenuItemSpriteWithLabel::create("ranklist/worldlist.png", 3, CC_CALLBACK_1(SngRankList::GoodFriendMenuCallback, this, 1));
	m_pTheWorldMenu->setAnchorPoint(Vec2(0,0));
	m_pTheWorldMenu->setPosition(500,529);
	m_pTheWorldMenu->setEnabled(true);
	auto TheWorldMenu = Menu::create(m_pTheWorldMenu,nullptr);
	TheWorldMenu->setPosition(Vec2::ZERO);
	addChild(TheWorldMenu, 2);

	// 排行榜的滚动View
	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 200, 60, this, 0);
	m_pScrollView->setContentSize(Size(700, 1050));
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);


	// 排名排行榜下的所有按钮
	for(int i = 0;i < 3; i++)
	{
		m_pFriendsWorldMenu[i] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(SngRankList::AllMenuCallback, this, i));
		m_pFriendsWorldMenu[i]->setAnchorPoint(Vec2(0,0));
		m_pFriendsWorldMenu[i]->setPosition(24,430 - 68 * i);
		auto friendsWorldRecord = Menu::create(m_pFriendsWorldMenu[i],nullptr);
		friendsWorldRecord->setPosition(Vec2::ZERO);
		addChild(friendsWorldRecord);
	}
	m_pFriendsWorldMenu[0]->setEnabled(false);
	auto nTheWorldMenu1 = CreateLabelMSYHBD(24,"大师分周榜");
	m_pFriendsWorldMenu[0]->SetLabel(nTheWorldMenu1);
	m_pFriendsWorldMenu[0]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pFriendsWorldMenu[0]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	auto nTheWorldMenu2 = CreateLabelMSYHBD(22,"SNG大师分周榜");
	m_pFriendsWorldMenu[1]->SetLabel(nTheWorldMenu2);
	m_pFriendsWorldMenu[1]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pFriendsWorldMenu[1]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	auto nTheWorldMenu3 = CreateLabelMSYHBD(22,"MTT大师分周榜");
	m_pFriendsWorldMenu[2]->SetLabel(nTheWorldMenu3);
	m_pFriendsWorldMenu[2]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pFriendsWorldMenu[2]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));



	m_nPageUp = FrameSpriteButton::createWithVertFrameSprite("commonbt/pageup.png", false, true);
	m_nPageDown = FrameSpriteButton::createWithVertFrameSprite("commonbt/pagedown.png", false, true);
	m_nPageUp->setVisible(false);
	m_nPageDown->setVisible(false);
	m_pScrollView->addChild(m_nPageUp);
	m_pScrollView->addChild(m_nPageDown);
	return true;
}

//---------------------------------------------------------------------------------------------------------------
// 好友和世界排名的回调函数
void SngRankList::GoodFriendMenuCallback(Ref* pSender, int num)
{
	if(num == 0)
	{
		m_uRecordFriendWorldID = 0;	// 记录好友和世界的ID
		m_pGoodFriendMenu->setEnabled(false);
		m_pTheWorldMenu->setEnabled(true);
	}
	else if(num == 1)
	{
		m_uRecordFriendWorldID = 1;	
		m_pGoodFriendMenu->setEnabled(true);
		m_pTheWorldMenu->setEnabled(false);
	}
	for(int i = 0; i < 3; i++)
	{
		if(i == m_uRecordMenuCurrentStatusId)
		{
			m_pFriendsWorldMenu[i]->setEnabled(false);

			FriendWorldRemainRichList(m_uRecordFriendWorldID, m_uRecordMenuCurrentStatusId);
		}
		else
		{
			m_pFriendsWorldMenu[i]->setEnabled(true);
		}
	}
	return ;
}

// 好友列表下按钮的所有回调函数
void SngRankList::AllMenuCallback(Ref* pSender, int num)
{
	for(int i = 0;i < 3;i++)
	{
		if(i == num)
		{
			m_pFriendsWorldMenu[i]->setEnabled(false);
			m_uRecordMenuCurrentStatusId = i;	// 记录当前按钮的按下的ID
			// 当前是好友和世界列表周战绩排名
			//if((m_uRecordFriendWorldID == EGoodFriendsRankList ||  m_uRecordFriendWorldID == ETheWorldRadnkList) /*&&
			//																									 m_uRecordMenuCurrentStatusId == ERankWeeksRecordMenuTag*/)	// 当记录好友世界排名和按钮事件状态都为0时
			//{
			//	FriendWorldWeeksRecordRichList(m_uRecordFriendWorldID, m_uRecordMenuCurrentStatusId);		// 通知更新好友和世界列表周战绩榜
			//}
			//else
			//{
				FriendWorldRemainRichList(m_uRecordFriendWorldID, m_uRecordMenuCurrentStatusId);			// 通知更新好友列表周赢利、日单局榜、财富榜
			//}
		}
		else
		{
			m_pFriendsWorldMenu[i]->setEnabled(true);
		}
	}
	return ;
}

// 初始化底池周战绩排行榜
void SngRankList::FriendWorldWeeksRecordRichList(unsigned uRankType, unsigned uWinType)
{
	bool nPageUp = false, nPageDown = false, nPageSmall = false;
	ClearWeeksRecordRichList();
	ClearRemainRichList();
	Size size = m_nPageDown->getContentSize();

	RecoderuRankType = uRankType;	// 记录好友还是世界排名
	RecoderuWinType = uWinType;		// 记录按钮的值
	// 对上一页、下一页的按钮的位置设置
	uGoodFriendFirstRankNum = g_globalSngRankInfo.GetRankList(uRankType, uWinType).size();	// 得到1~3的容器的数量
	unsigned uAllMission = uGoodFriendFirstRankNum;											// 列表容器所有任务的数量
	unsigned uItemH = 110;																	// 滚动视图的单个高度 
	unsigned uAllItemH = uItemH * uAllMission;	// 所有滚动视图的高度
	float fX = ListScrollW / 2, fY = 55;
	g_globalSngRankInfo.GetPageInfo(RecoderuRankType, RecoderuWinType, nServerCurrentPage, nServerMaxPage);	// 得到当前页数
	if (nServerCurrentPage < nServerMaxPage)	// 当前页小于最大页
	{
		uAllItemH += size.height;
		nPageDown = true;
	}
	if (nServerCurrentPage > 1)
	{
		uAllItemH += size.height;
		nPageUp = true;
	}
	if (uAllItemH < ListScrollH)
	{
		fY = ListScrollH - uAllItemH + fY;
		uAllItemH = ListScrollH;	
		nPageSmall = true;
	}
	double fy = ListScrollH - (int)uAllItemH;
	m_pScrollView->setContentOffset(Vec2(0, fy), false);

	if (nPageDown)
	{
		auto fY =  size.height / 2;
		//当前页小于总页数，创建下一页按钮
		m_nPageDown->setVisible(true);
		m_nPageDown->setPosition(fX, fY);
		m_nPageDown->SetHitCB(std::bind(&SngRankList::PageDownMenu, this, nServerCurrentPage, nServerMaxPage));	// 下一页的按钮的回调函数
		fY += uItemH / 2 + fY;
	}
	else
	{
		m_nPageDown->setVisible(false);
	}
	// vector的倒序遍历
	auto&& v = g_globalSngRankInfo.GetRankList(uRankType, uWinType);
	for(auto it = v.rbegin(); it != v.rend(); it++ )
	{
		auto item = GetUserWinCashListItem(it->m_uID);
		UpdateUserWinCashListItem(item, *it);
		item->m_pSpriteBGItem->setPosition(fX+5, fY);
		item->m_pSpriteBGItem->setVisible(true);
		m_vecMissionWinCashListItemInValid.push_back(item);
		fY += uItemH;
	}
		if (nServerCurrentPage > 1)
		{
			//当前页大于第一页，创建上一页按钮
			fY = fY - (uItemH - uItemH / 2 - size.height / 2);
			m_nPageUp->setVisible(true);
			m_nPageUp->setPosition(fX, fY);
			m_nPageUp->SetHitCB(std::bind(&SngRankList::PageUpMenu, this, nServerCurrentPage, nServerMaxPage));
		}
		else
		{
			m_nPageUp->setVisible(false);
		}

		m_pScrollView->setContentSize(Size(ListScrollW, uAllItemH));
		return ;
}

// 初始化奖金周盈利、、大师分周榜
void SngRankList::FriendWorldRemainRichList(unsigned uRankType, unsigned uWinType)
{
	bool nPageUp = false, nPageDown = false, nPageSmall = false;
	ClearRemainRichList();
	ClearWeeksRecordRichList();
	Size size = m_nPageDown->getContentSize();

	RecoderuRankType = uRankType;	// 记录好友还是世界排名
	RecoderuWinType = uWinType;		// 记录按钮的值
	// 对上一页、下一页的按钮的位置设置
	uFriendWorldRemainRankNum = g_globalSngRankInfo.GetRankList(uRankType, uWinType).size();			// 得到1~3的容器的数量
	unsigned uAllMission = uFriendWorldRemainRankNum;													// 奖金周盈利、日单局、财富榜容器所有任务的数量
	unsigned uItemH = 110;																				// 滚动视图的单个高度 
	unsigned uAllItemH = uItemH * uAllMission;															// 所有滚动视图的高度
	float fX = ListScrollW / 2, fY = 55;
	g_globalSngRankInfo.GetPageInfo(RecoderuRankType, RecoderuWinType, nServerCurrentPage, nServerMaxPage);	// 得到当前页数
	if (nServerCurrentPage < nServerMaxPage)	// 当前页小于最大页
	{
		uAllItemH += size.height;
		nPageDown = true;
	}
	if (nServerCurrentPage > 1)
	{
		uAllItemH += size.height;
		nPageUp = true;
	}

	if (uAllItemH < ListScrollH)
	{
		fY = ListScrollH - uAllItemH + fY;
		uAllItemH = ListScrollH;	
		nPageSmall = true;
	}
	double fy = ListScrollH - (int)uAllItemH;
	m_pScrollView->setContentOffset(Vec2(0, fy), false);

	if (nPageDown)		// 是否下一页
	{
		auto fY =  size.height / 2;
		//当前页小于总页数，创建下一页按钮
		m_nPageDown->setVisible(true);
		m_nPageDown->setPosition(fX, fY);
		m_nPageDown->SetHitCB(std::bind(&SngRankList::PageDownMenu, this, nServerCurrentPage, nServerMaxPage));
		fY += uItemH / 2 + fY;
	}
	else
	{
		m_nPageDown->setVisible(false);
	}


	bool show = false ;
	if (uWinType==EDaySingleMenuTag)
	{
		show = true ;
	}
	show = true ;
	// vector的倒序遍历
	auto&& v = g_globalSngRankInfo.GetRankList(uRankType, uWinType);			// 奖金周盈利、日单局、财富榜的Vector遍历
	for(auto it = v.rbegin(); it != v.rend(); it++ )
	{
		auto item = GetUserWinPotListItem(it->m_uID);
		UpdateUserWinPotListItem(item, *it);
		item->m_pSpriteBGItem->setPosition(fX+5, fY);
		item->m_pSpriteBGItem->setVisible(true);

		item->m_pSpriteRewordImage->setVisible(!show) ;
		item->m_pSpriteScoreImage->setVisible(show) ;


		m_vecMissionListWinPotItemInValid.push_back(item);
		fY += uItemH;
	}
	if (nServerCurrentPage > 1)
	{
		//当前页大于第一页，创建上一页按钮
		fY = fY - (uItemH - uItemH / 2 - size.height / 2);
		m_nPageUp->setVisible(true);
		m_nPageUp->setPosition(fX, fY);
		m_nPageUp->SetHitCB(std::bind(&SngRankList::PageUpMenu, this, nServerCurrentPage,nServerMaxPage));	// 上一页的按钮的回调函数
	}
	else
	{
		m_nPageUp->setVisible(false);
	}
	m_pScrollView->setContentSize(Size(ListScrollW, uAllItemH));
	return ;
}


//---------------------------------------------------------------------------------------------------------------
// 上一页
void SngRankList::PageUpMenu(unsigned nServerCurrentPage, unsigned nServerMaxPage)
{
	Json::Value param;
	param["winType"] = RecoderuWinType;
	param["rankType"] = RecoderuRankType;
	param["pageID"] = nServerCurrentPage - 1;
	param["pageCount"] = 10;
	SendClientRequest(ClientRequest::kGetSngRankList, param);
}

//---------------------------------------------------------------------------------------------------------------
// 下一页
void SngRankList::PageDownMenu(unsigned nServerCurrentPage, unsigned nServerMaxPage)
{
	Json::Value param;
	param["winType"] = RecoderuWinType;
	param["rankType"] = RecoderuRankType;
	param["pageID"] = nServerCurrentPage + 1;
	param["pageCount"] = 10;
	SendClientRequest(ClientRequest::kGetSngRankList, param);
}

//---------------------------------------------------------------------------------------------------------------
// 获取玩家奖金的列表
SngRankList::UserWinPotListItem* SngRankList::GetUserWinPotListItem(unsigned m_uUserWinPotID)
{
	UserWinPotListItem *item = nullptr; 
	auto labelAnchor = Vec2(0, 0);
	int labelSize = 26;
	int labelFenLeiSize = 24;
	auto labelClr = Color4B::WHITE;
	auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);
	int MAX_Size = 1;
	auto szCur = m_pScrollView->getContentSize();
	if (m_vecMissionListWinPotItem.empty())		// 判断OLD的容器数据是否为空
	{
		// 加载滚动视图行的背景	
		//auto bgItem = FrameSprite::create("taskset/bgitem.png", 1, 2);
		auto bgItemCash = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 358, 239, m_pScrollView, 0);
		// 显示玩家头像背景圆框图片
		auto httpSpriteRankList = CreateUserHeadSprite("defaulticon.png", "ranklist/headportrait.png", bgItemCash, Vec2(20, 17), "ranklist/pictureframe.png");
		// 显示玩家的姓名
		auto nCashCenterNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(26 , "",labelAnchor, 105, 40, Color4B::WHITE, bgItemCash, 0);
		// 显示玩家的奖金数量 
		auto nCashCenterSizelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",Vec2::ZERO, 400, 43, Color4B::YELLOW, bgItemCash, 0);
		// 显示奖金的图片精灵
		auto nCashSpriteImage = CreateNode1AndPosAddTo<Sprite>("ranklist/reword.png", 380, 55, bgItemCash, 0);
		// 显示奖金的图片精灵
		auto nScoreSpriteImage = CreateNode1AndPosAddTo<Sprite>("gamesng/sng_hall_goldCoin-1.png", 380, 55, bgItemCash, 0);

		// 玩家头像的皇冠、银冠、铜冠、绿色背景排名
		auto btSpriteBgRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 88, bgItemCash, 0);
		// 显示玩家的奖金排名Label
		auto nCashSortlabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",Vec2(0.5, 0.5), 20, 88, Color4B(0xff,0xd2,0x00,0xff), bgItemCash, 0);
		// 对滚动视图的监听
		auto nBgItemCashListen = LayerButton::create(false, true);
		bgItemCash->addChild(nBgItemCashListen);
		auto szBt = bgItemCash->getContentSize();
		nBgItemCashListen->setContentSize(szBt);
		auto pItem = new UserWinPotListItem;

		pItem->m_pSpriteIcon = httpSpriteRankList;		// 网上下载的玩家头像
		pItem->m_pLabelName = nCashCenterNamelabel;		// 玩家的称呼
		pItem->m_pLabelRank = nCashSortlabel;			// 玩家名字数字的排名
		pItem->m_pLabelWinPot = nCashCenterSizelabel;	// 玩家获得的奖金数字
		pItem->m_pSpriteRewordImage = nCashSpriteImage;	// 玩家的奖金精灵图片
		pItem->m_pSpriteScoreImage = nScoreSpriteImage;	// 玩家的奖金精灵图片
		pItem->m_pSpriteBGItem = bgItemCash;			// 滚动视图行的背景
		pItem->m_pSpriteBgRank = btSpriteBgRank;		// 玩家头像的皇冠、银冠、铜冠、绿色背景排名
		pItem->m_bgItemWinPotListen = nBgItemCashListen;// 对滚动视图的监听
		item = pItem;

	}
	else
	{
		item = m_vecMissionListWinPotItem.back();
		m_vecMissionListWinPotItem.pop_back();
	}
	item->m_bgItemWinPotListen->SetHitCB([m_uUserWinPotID](Ref*){
		Json::Value param;
		param["tid"] = m_uUserWinPotID;
		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
	});
	return item;
}

//---------------------------------------------------------------------------------------------------------------
SngRankList::UserWinCashListItem* SngRankList::GetUserWinCashListItem(unsigned m_uUserWinCashID)					// 获取玩家底池的列表
{
	UserWinCashListItem *item = nullptr;
	auto labelAnchor = Vec2(0, 0);
	int labelSize = 26;
	int labelFenLeiSize = 24;
	auto labelClr = Color4B::WHITE;
	auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);
	int MAX_Size = 1;
	auto szCur = m_pScrollView->getContentSize();
	if (m_vecMissionWinCashListItem.empty())		// 判断OLD的容器数据是否为空
	{
		// 加载滚动视图行的背景	
		//auto bgItem = FrameSprite::create("taskset/bgitem.png", 1, 2);
		auto bgItemCash = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 358, 239, m_pScrollView, 0);
		// 显示玩家头像背景圆框图片
		auto httpSpriteRankList = CreateUserHeadSprite("defaulticon.png", "ranklist/headportrait.png", 
			bgItemCash, Vec2(20, 17), "ranklist/pictureframe.png");
		// 显示周战绩玩家的姓名
		auto nCashCenterNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(26 , "",labelAnchor, 105, 40, Color4B::WHITE, bgItemCash, 0);
		// 显示周战绩玩家的底池的数量
		auto nCashCenterSizelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",Vec2(0.5, 0.5), 510, 52, Color4B::YELLOW, bgItemCash, 0);
		// 显示底池的图片精灵
		auto nCashSpriteImage = CreateNode1AndPosAddTo<Sprite>("ranklist/weeksrecordlisticon.png", 380, 52, bgItemCash, 0);
		// 玩家头像的皇冠、银冠、铜冠、绿色背景排名
		auto btSpriteBgRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 88, bgItemCash, 0);
		// 显示周战绩玩家的底池排名Label
		auto nCashSortlabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",Vec2(0.5, 0.5), 20, 88, Color4B(0xff,0xd2,0x00,0xff), bgItemCash, 0);
		// 对滚动视图的监听
		auto nBgItemCashListen = LayerButton::create(false, true);
		bgItemCash->addChild(nBgItemCashListen);
		auto szBt = bgItemCash->getContentSize();
		nBgItemCashListen->setContentSize(szBt);
		auto pItem = new UserWinCashListItem;
		pItem->m_pSpriteIcon = httpSpriteRankList;			// 网上下载的玩家头像
		pItem->m_pLabelName = nCashCenterNamelabel;			// 玩家的称呼
		pItem->m_pLabelRank = nCashSortlabel;				// 玩家名字数字的排名
		pItem->m_pSpriteCashImage = nCashSpriteImage;		// 玩家的奖金底池图片
		pItem->m_pLabelWinCash = nCashCenterSizelabel;		// 玩家获得的底池数量
		//pItem->m_pLabelWinCashDesc = nCashCenterDesclabel;// 玩家获得的底池文字描述
		pItem->m_pSpriteBGItem = bgItemCash;				// 滚动视图行的背景
		pItem->m_pSpriteBgRank = btSpriteBgRank;			// 玩家头像的皇冠、银冠、铜冠、绿色背景排名
		pItem->m_bgItemCashListen = nBgItemCashListen;		// 对滚动视图的监听
		item = pItem;
	}
	else
	{
		item = m_vecMissionWinCashListItem.back();
		m_vecMissionWinCashListItem.pop_back();
	}
	item->m_bgItemCashListen->SetHitCB([m_uUserWinCashID](Ref*){
		Json::Value param;
		param["tid"] = m_uUserWinCashID;
		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
	});
	return item;
}

//---------------------------------------------------------------------------------------------------------------
// 更新奖金的列表 
void SngRankList::UpdateUserWinPotListItem(SngRankList::UserWinPotListItem *item, const SngRankInfo& rankuserinfo)
{
	// 得到当前的玩家的称呼
	SetLabelString(item->m_pLabelName, rankuserinfo.m_strNickName);	
	// 得到当前玩家获得的奖金数量
	SetLabelString(item->m_pLabelWinPot, FormatCash4(rankuserinfo.m_dWinValue).c_str());
	// 得到玩家的头像
	SetIconUrl(item->m_pSpriteIcon, rankuserinfo.m_strIconUrl);
    if (rankuserinfo.m_uRank < 3)
    {
        item->m_pLabelRank->setVisible(false);
        item->m_pSpriteBgRank->SetFrameIdx(rankuserinfo.m_uRank);
    }
    else
    {
        item->m_pSpriteBgRank->SetFrameIdx(3);
        // 得到玩家奖金的排序的大小
        item->m_pLabelRank->setVisible(true);
        SetLabelString(item->m_pLabelRank, FormatCash4(rankuserinfo.m_uRank + 1).c_str());
    }
	return ;
}

//---------------------------------------------------------------------------------------------------------------
// 更新底池的列表
void SngRankList::UpdateUserWinCashListItem(SngRankList::UserWinCashListItem *item, const SngRankInfo& rankuserinfo)
{
	// 得到当前的玩家的称呼
	SetLabelString(item->m_pLabelName, rankuserinfo.m_strNickName);	

	// 得到当前玩家获得的底池数量
	char szTxt[100] = {0};
	sprintf(szTxt, "赢得%s个底池", FormatCash4(rankuserinfo.m_dWinValue).c_str());
	SetLabelString(item->m_pLabelWinCash, szTxt);
	// 得到玩家的头像
	SetIconUrl(item->m_pSpriteIcon, rankuserinfo.m_strIconUrl);
    if (rankuserinfo.m_uRank < 3)
    {
        item->m_pLabelRank->setVisible(false);
        item->m_pSpriteBgRank->SetFrameIdx(rankuserinfo.m_uRank);
    }
    else
    {
        item->m_pSpriteBgRank->SetFrameIdx(3);
        // 得到玩家底池的排序的大小
        item->m_pLabelRank->setVisible(true);
        SetLabelString(item->m_pLabelRank, FormatCash4(rankuserinfo.m_uRank + 1).c_str());
    }
	return ;
}

//---------------------------------------------------------------------------------------------------------------
// 通知更新函数
void SngRankList::UpdateRankList(unsigned uRankType, unsigned uWinType)
{
	// 当前是好友和世界列表周战绩排名
	/*if(uRankType == m_uRecordFriendWorldID &&  uWinType == m_uRecordMenuCurrentStatusId && m_uRecordMenuCurrentStatusId == ERankWeeksRecordMenuTag)
	{*/
		//FriendWorldWeeksRecordRichList(uRankType, uWinType);	// 通知更新好友和世界列表周战绩榜
	//}
	//else if(uRankType == m_uRecordFriendWorldID && uWinType == m_uRecordMenuCurrentStatusId && m_uRecordMenuCurrentStatusId != ERankWeeksRecordMenuTag)
	//{
		//FriendWorldRemainRichList(uRankType, uWinType);			// 通知更新好友列表周赢利、日单局榜、财富榜
	//}
		if (uWinType ==ERankWeeksRecordMenuTag && uRankType ==0)
		{
			FriendWorldRemainRichList(uRankType, uWinType);	
		}


	return ;
}

// 清理所有的底池的vector
void SngRankList::ClearWeeksRecordRichList()
{
	for (auto item : m_vecMissionWinCashListItemInValid)			// 对排名周战绩底池容器进行遍历进行
	{
		item->m_pSpriteBGItem->setVisible(false);
		m_vecMissionWinCashListItem.push_back(item);				// PUSH到Old的Vector容器里面
	}
	m_vecMissionWinCashListItemInValid.clear();						// 对New的Vector容器进行清空
}

// 清理所有的奖金的vector
void SngRankList::ClearRemainRichList()
{
	for (auto item : m_vecMissionListWinPotItemInValid)			// 对排名周盈利榜、日单榜、财富榜底池容器进行遍历进行
	{
		item->m_pSpriteBGItem->setVisible(false);
		m_vecMissionListWinPotItem.push_back(item);				// PUSH到Old的Vector容器里面
	}
	m_vecMissionListWinPotItemInValid.clear();					// 对New的Vector容器进行清空
}

void SngRankList::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		Json::Value param;
		for (unsigned i = 0; i < 2; i++)
		{
			for (unsigned j = 0; j < 3; j++)
			{
				param["winType"] = j;
				param["rankType"] = i;
				param["pageID"] = 1;
				param["pageCount"] = 10;
				SendClientRequest(ClientRequest::kGetSngRankList, param);
			}
		}
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;


	for(int i = 0;i < 3;i++)
	{
		m_pFriendsWorldMenu[i]->setEnabled(i!=0);
	}
	m_pGoodFriendMenu->setEnabled(false);
	m_pTheWorldMenu->setEnabled(true);






        //auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
        //runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));
    }
    else
    {
		SendClientRequest(ClientRequest::kOptSNGHallBgLayer);
		SendClientRequest(ClientRequest::kOptMTTHallBgLayer);
		GameDialog::setVisible(false);
        //auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
        //runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));
    }

	//GameDialog::setVisible(bVisible);
}