//#include "ContributionRankList.h"
//
//#include "GameData.h"
//#include "MenuItemSpriteWithLabel.h"
//#include "FrameSpriteButton.h"
//#include "GlobalUserData.h"
//#include "FrameSprite.h"
//#include "HttpSprite.h"
//#include "GrabTouch.h"
//#include "Util.h"
//#include "GameDialog.h"
//#include "GameFamily.h"
//#include "GameHall.h"
//#include "EditFamilyRole.h"
//#include "Tools.h"
//
//USING_NS_CC;
//
//enum 
//{
//    RankScrollW = 816,
//    RankScrollH = 461
//};
//
//enum BgItem
//{
//	ListBgItemW = 806,
//	ListBgItemH = 112
//};
//
//
//bool ContributionRankList::init()
//{
//	if(!GameDialog::init("dialog/bg.png",true))
//	{
//		return false;
//	}
//
//	s_dataLoading =nullptr ;
//	Size sz = this->getContentSize() ;
//
//	m_layerTop = GameLayer::create(sz.width, 143, false, false);
//	m_layerTop->setPosition(Vec2(0, 497));
//	this->addChild(m_layerTop, 1);
//
//	
//
//	m_layerBottom = GameLayer::create(sz.width, 10, false, false);
//	m_layerBottom->setPosition(Vec2(0, 0));
//	this->addChild(m_layerBottom, 1);
//
//	auto labelAnchor = Vec2(0, 0);
//	int labelSize = 24;
//	auto laberColor = Color4B(0x50, 0xbd, 0x2e, 0xff);
//	auto whiteColor = Color4B::WHITE;
//
//
//	spriteTitle = Sprite::create("family/family_gongx.png");
//	spriteTitle->setPosition(469,569);
//	this->addChild(spriteTitle);
//
//	sizeListView.width = RankScrollW;
//	sizeListView.height = RankScrollH ;
//
//	//滚动视图
//	m_ScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(RankScrollW, RankScrollH), sz.width/2-10, 255, this, 0);
//	m_ScrollView->ignoreAnchorPointForPosition(false);
//	m_ScrollView->setAnchorPoint(Point(0.5,0.5)) ;
//	m_ScrollView->setContentSize(Size(RankScrollW,RankScrollH));
//	m_ScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
//	//m_pScrollView->setContentSize(Size(ListScrollW,ListScrollH));
//	m_ScrollView->setBounceable(true);
//	m_ScrollView->setSwallowsTouches(false) ;
//	m_ScrollView->setVisible(true);
//
//
//	//警告
//	m_lableTips = CreateLabelMSYH(26, "贡献排行每周三、六更新");
//	m_lableTips->setTextColor(cocos2d::Color4B(0x5c, 0xc3, 0x80, 0xff));
//	m_lableTips->setPosition(sz.width/2, 505);
//	m_lableTips->setWidth(810);
//	m_lableTips->setLineBreakWithoutSpace(true);
//	m_lableTips->setAlignment(TextHAlignment::CENTER);
//	this->addChild(m_lableTips);
//
//    return true;
//}
//
//
//
//void ContributionRankList::onMenuItemCloseSelected(cocos2d::Ref * ref)
//{
//	setVisible(false) ;
//}
//
//
//void ContributionRankList::updateItemView(const FamilyRoleBaseInfo &baseInfo)
//{
//	for  (auto list:m_vecRanRoleOld)
//	{
//		if (list->m_uid == baseInfo.m_uid)
//		{
//			UpdateItem(list,baseInfo);
//		}
//	}
//}
//
//void ContributionRankList::UpdateView()
//{
//
//	std::vector<RankRoleItem*>::iterator iter;
//
//	while (!m_vecRanRoleOld.empty())
//	{
//		iter = m_vecRanRoleOld.begin();
//		RankRoleItem* item = m_vecRanRoleOld.at(0) ;
//		item->m_pSpriteBgItem->removeFromParent() ;
//		m_vecRanRoleOld.erase(iter) ;
//
//	}
//
//	unsigned uCount = g_globalMyFamilyInfo.m_allRankRole.size();
//
//	unsigned uItemH = ListBgItemH;
//	unsigned fRealH = uCount * uItemH;
//	if(fRealH < RankScrollH)
//	{
//		fRealH = RankScrollH;
//	}
//	float fX = RankScrollW / 2;
//	float fY = fRealH - uItemH / 2 ;
//
//
//	for(auto& followList:g_globalMyFamilyInfo.m_allRankRole)
//	{
//		auto item = GetFamilyListItem(followList);		//auto item = GetFamilyListItem(followList.m_uid,followList.m_power);
//		UpdateItem(item, followList);
//
//		item->m_pSpriteBgItem->setPosition(fX+5, fY);
//		item->m_pSpriteBgItem->setVisible(true); 
//		m_vecRanRoleOld.push_back(item);
//		fY -= uItemH;     
//	}
//	float temp = fRealH - RankScrollH;
//
//	//每次都显示滚动框的最上面
//	m_ScrollView->setContentOffset(Vec2(0,-temp-10), false);
//	m_ScrollView->setContentSize(Size(RankScrollW,fRealH+15));
//
//
//	setShowDataLoading(false) ;
//}
//
//
//
//
//ContributionRankList::RankRoleItem *ContributionRankList::GetFamilyListItem(FamilyRoleBaseInfo baseInfo)
//{
//	RankRoleItem *item = nullptr;
//
//		auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("family/ranklistbg.png", 1, 2, RankScrollW / 2, 0, m_ScrollView, 0);
//		auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
//		touchGrabber->retain();
//		touchGrabber->GetTouchProcer().SetHitCB(
//			[bgItem](bool bSel, Node*, TouchGrabber*){
//				bgItem->SetFrameIdx(bSel?1:0);
//		}
//		);
//
//		unsigned m_uid = baseInfo.m_uid ;
//		auto btDetail = LayerButton::create(false, true);
//		bgItem->addChild(btDetail);
//		//m_pScrollView->addChild(bgItem);
//		//头像
//		auto pTouXiang = CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", bgItem, Vec2(18,10), "family/gf_family_icon.png");
//		//人物名称
//		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0,0.5),140,52,Color4B::WHITE,bgItem,0);
//
//		//贡献值
//		auto labelrank = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0,0.5),560,52,Color4B(0xff,0xd2,0x00,0xff),bgItem,0);
//
//		//任务等级
//		auto btSpriteBgRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 63, bgItem, 3);
//		btSpriteBgRank->SetFrameIdx(3);
//		auto labelRankid = CreateLabelMSYHAndAnchorPosClrAddTo(16,"1",Vec2(0.5,0.5),20,92,Color4B(0xff,0xd2,0x00,0xff),bgItem,5);
//	
//		auto pItem = new RankRoleItem;
//		pItem->m_pSpriteBgItem = bgItem;
//		pItem->m_pBtDetail = btDetail;
//		pItem->m_pSpriteRoleIcon = pTouXiang;
//		pItem->m_familyName = labelName;
//		pItem->m_spriteRank = btSpriteBgRank;
//		pItem->m_rankid = labelRankid;
//		pItem->m_contributionValue = labelrank;
//		pItem->m_uid = m_uid ;
//		item = pItem;
//
//
//
//
//	//背景回调
//	item->m_pBtDetail->setContentSize(Size(RankScrollW,106));
//	return item;
//}
//
//void ContributionRankList::UpdateItem(ContributionRankList::RankRoleItem *item, const FamilyRoleBaseInfo& followInfo)
//{
//
//
//
//	SetIconUrl(item->m_pSpriteRoleIcon, followInfo.m_iconUrl);
//	SetLabelStringWithBoundWidth(item->m_familyName,followInfo.m_uname.c_str(), 26*6);
//
//	//贡献值
//	int s_level = followInfo.m_contribution;
//	char s_buf[64] ;
//	sprintf(s_buf,"贡献值：%d",s_level) ;
//	SetLabelStringWithBoundWidth(item->m_contributionValue,s_buf, 200);
//
//
//	sprintf(s_buf,"%d",(followInfo.m_rank+1)) ;
//	//排名
//	if (followInfo.m_rank<=2)
//	{
//		item->m_spriteRank->setPosition(20,96) ;
//		item->m_spriteRank->SetFrameIdx(followInfo.m_rank);
//		SetLabelStringWithBoundWidth(item->m_rankid,"", 120);
//	}
//	else
//	{
//		item->m_spriteRank->setPosition(20,93) ;
//		item->m_spriteRank->SetFrameIdx(3);
//		SetLabelStringWithBoundWidth(item->m_rankid,s_buf, 120);
//	}
//
//
//
//
//	item->m_pBtDetail->setContentSize(Size(RankScrollW,106));
//	item->m_pBtDetail->SetHitCB(std::bind(&ContributionRankList::menuClickBgItem, this,followInfo));
//
//}
//
//
//void ContributionRankList::menuClickBgItem(FamilyRoleBaseInfo baseInfo)
//{
//
//	auto pHall = dynamic_cast<GameHall*>(getParent());
//
//	//点的自己
//	if (g_globalMyData.m_iUserId==baseInfo.m_uid)
//	{
//
//		Json::Value param;
//		param["tid"] = baseInfo.m_uid;
//		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
//		return ;
//	}
//
//	if (g_globalMyData.getFamilyPower()==2)
//	{
//
//	}
//	if (g_globalMyData.getFamilyPower()==1)
//	{
//
//
//		if(baseInfo.m_power==0)
//		{
//			pHall->showEditFamilyRole(true,baseInfo.m_uid,baseInfo.m_uname,(Edit_LookInf0|Edit_Push));
//		}
//		else
//		{
//			Json::Value param;
//			param["tid"] = baseInfo.m_uid;
//			SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
//	
//		}
//
//
//	}
//	if (g_globalMyData.getFamilyPower()==0)
//	{
//		Json::Value param;
//		param["tid"] = baseInfo.m_uid;
//		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
//	}
//
//}
//
////删除记录
//void ContributionRankList:: RemoveItemView( unsigned uUserID)
//{
//
//
//
//	for(int i=0; i<m_vecRanRoleOld.size(); i++)
//	{
//		if(m_vecRanRoleOld[i]->m_uid == uUserID)
//		{
//
//			unsigned uItemH = ListBgItemH;
//			m_vecRanRoleOld[i]->m_pSpriteBgItem->setVisible(false);
//			m_vecRanRoleOld.erase(m_vecRanRoleOld.begin() + i);
//
//			unsigned uCount = m_vecRanRoleOld.size();
//
//			unsigned fRealH = uCount * uItemH;
//			if(fRealH < RankScrollH)
//			{
//				fRealH = RankScrollH;
//			}
//			float fX = RankScrollW / 2;
//			float fY = fRealH - uItemH / 2 ;
//
//
//			for(auto& followList:m_vecRanRoleOld)
//			{
//
//				followList->m_pSpriteBgItem->setPosition(fX, fY);
//				followList->m_pSpriteBgItem->setVisible(true); 
//
//				fY -= uItemH;     
//			}
//			float temp = fRealH - RankScrollH;
//
//			//每次都显示滚动框的最上面
//			m_ScrollView->setContentOffset(Vec2(0,-temp-10), false);
//			m_ScrollView->setContentSize(Size(RankScrollW,fRealH+15));
//
//
//		}
//	}
//}
//
//
//void ContributionRankList::setShowDataLoading(bool flag)
//{
//	if (s_dataLoading == nullptr)
//	{
//
//		s_dataLoading = Tools::creatDataLoading(this,476, 277,99) ;
//	}
//	s_dataLoading->setVisible(flag) ;
//
//}
