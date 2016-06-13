#include "GameSngHall.h"
#include "Tools.h"
#include "GameFrame.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"

#include "GameData.h"
#include "SngRule.h"
#include "SngSignup.h"
#include "GuideTips.h"
#include "FrameScale9SpriteButton.h"
enum 
{
	TableWidth = 922 ,
	TableHeight = 145 ,
};

enum 
{
	ZOrder_Rule=10 ,
	ZOrder_Rank,
    ZOrder_HelpGuide
};


bool GameSngHall::init()
{
	if(!GameDialog::init())
	{
		return false;
	}
	m_sngRule = nullptr ;
	//m_sngRankList = nullptr ;
	//m_sngSignUp = nullptr ;

	//载入布局
	s_uiroot = CSLoader::createNode("res/ui/snghall.csb");
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	s_uiroot->setContentSize(frameSize); 
	ui::Helper::doLayout(s_uiroot);  
	addChild(s_uiroot);
	setContentSize(frameSize); 






	s_layoutTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_sngtable")) ;

	//s_layoutHelp = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_help")) ;
	//s_layoutHelp->setVisible(false) ;

	m_pSpriteBgTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_sngBG"));
    auto imageLeft = static_cast<Layout*>(Tools::getWidgetByName(m_pSpriteBgTable,"Image_bgleft"));
    auto imageRight = static_cast<Layout*>(Tools::getWidgetByName(m_pSpriteBgTable,"Image_bgright"));

    Sprite *bgLeft = Sprite::create("hall2half.png") ;
    Sprite *bgRignt = Sprite::createWithTexture(bgLeft->getTexture()) ;
    imageRight->setScaleX(-1) ;
    bgLeft->setPosition(imageLeft->getContentSize().width/2,imageLeft->getContentSize().height/2) ;
    bgRignt->setPosition(imageRight->getContentSize().width/2,imageRight->getContentSize().height/2);
    imageLeft->addChild(bgLeft);
    imageRight->addChild(bgRignt);

	m_leftupButton = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_leftupbutton"));
	m_rightupButton = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_rightup"));
	m_middleShow = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_middletips"));
	m_middleTips = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_title"));

	m_SngTips= static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_tips"));


	m_buttonBack = static_cast<Button*>(Tools::getWidgetByName(m_leftupButton,"Button_back"));
	m_buttonBack->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;

	m_buttonhelp = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_help"));
	m_buttonhelp->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;

	m_buttonrank = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_rank"));
	m_buttonrank->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;

	m_buttonshop = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_shop"));
	m_buttonshop->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;

	m_buttonkefu = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_kefu"));
	m_buttonkefu->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;

	m_buttonTips = static_cast<Button*>(Tools::getWidgetByName(m_middleTips,"Button_snghalltips"));
	m_buttonTips->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;
	//遮罩
	m_buttonTouch = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_touch"));
	m_buttonTouch->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;

	m_buttonTouch2 = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_touch_0"));
	m_buttonTouch2->addTouchEventListener(this,toucheventselector(GameSngHall::onMenuItemBottomSelect)) ;
	//人物信息
	m_textName =  static_cast<Text*>(Tools::getWidgetByName(m_middleShow,"Text_rolename"));
	m_textMoney =  static_cast<Text*>(Tools::getWidgetByName(m_middleShow,"Text_1_0_0_0"));
	m_textrank =  static_cast<Text*>(Tools::getWidgetByName(m_middleShow,"text_rolerank"));
	m_textDashiRank =  static_cast<Text*>(Tools::getWidgetByName(m_middleShow,"text_roledashirank"));

	ImageView* imgHero = static_cast<ImageView*>(Tools::getWidgetByName(m_middleShow,"Image_rolehead"));
	m_spriteHero =  CreateUserHeadSprite("defaulticon.png", "family/iconclipmaskbig98.png", imgHero, Vec2(0, 0), "hall/iconround.png");

	//m_spriteHero = CreateUserHeadSprite("defaulticon.png", "userinfo/iconclipmask.png", imgHero, Vec2(0, 0), "userinfo/iconround.png");

	Node* joinFamilyNode = Tools::getWidgetByName(s_layoutTable,"Panel_scroll");
	sizeListView.width = joinFamilyNode->getContentSize().width;
	sizeListView.height = joinFamilyNode->getContentSize().height;

	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,0,+6,joinFamilyNode,0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);
	m_pScrollView->setDelegate(this);


	
	for (int i=0 ;i<5 ;i++)
	{
		char path[64] ;
		sprintf(path,"gamesng/sng_hall_bunting-%d.png",i+1) ;
		m_spriteIcon[i]  = Sprite::create(path) ;
		this->addChild(m_spriteIcon[i],-1) ;
	}




	auto frameSz = Director::getInstance()->getVisibleSize();
	m_layerColorBg = LayerColor::create(Color4B(0,0,0,120), frameSz.width, frameSz.height);
	m_layerColorBg->setPosition(0, 0);
	addChild(m_layerColorBg);
	m_layerColorBg->setVisible(false);


	g_globalMyData.WaitChange(WAITCASHCHANGE , std::bind(&GameSngHall::OnUpdateMyData, this, std::placeholders::_1));




	//模拟个简单数据
	g_globalSngHallInfo.m_sngTableInfo.clear() ;
	std::string str_tableName[6] = {"菜鸟场","普通场","精英场","卓越场","大师场","宗师场",} ;
	int tanbleImage[6] = {1,4,0,4,3,2} ;
	for (int i=0;i<6;i++)
	{
		GlobalSngTableInfo table0;

		table0.m_index = i ;
		table0.m_tableName = str_tableName[i] ;
		table0.m_ImgID = tanbleImage[i] ;
		table0.m_tableID = i ;

		table0.m_maxPlayRoleCount =  6 ;
		table0.m_roleCountInGame = 0 ;
		table0.m_rewardCount =table0.m_maxPlayRoleCount==6?2:3 ;



		for (int j=0; j<table0.m_rewardCount; j++)
		{
			table0.m_reward[j] = 0;
			table0.m_score[j]= 0 ;
		}
		table0.m_entryFee = 0 ;
		table0.m_fee =  0;
		table0.m_freeTicket = 1 ;
		table0.m_myTicket =  0;
		g_globalSngHallInfo.m_sngTableInfo.push_back(table0) ;
	}


	//setOpacity(50);
	//creatTableScrollView() ;
	return true ;
}

void GameSngHall::onSetVisible(bool bVisible)
{
	if (bVisible)
	{

		//Json::Value param;
		//param["num"] = 0;
		//SendClientRequest(ClientRequest::kGetSngHallInfo, param);
		PlayBackGroundSound("sound/bghall1.mp3", true);
		SendClientRequest(ClientRequest::kGetSngHallInfo);

		m_middleTips->setVisible(false) ;

		m_rightupButton->setVisible(false);
		m_leftupButton->setVisible(false);
		m_middleShow->setVisible(false);
		m_SngTips->setVisible(false) ;


		float s_curX= m_pSpriteBgTable->getPositionX();
		float s_curY= m_pSpriteBgTable->getPositionY();
		auto sz = m_pSpriteBgTable->getContentSize();
		m_pSpriteBgTable->setPositionY(-sz.height / 2);

		m_pSpriteBgTable->runAction(Sequence::create(DelayTime::create(0.1f), EaseBackOut::create(MoveTo::create(0.3f, Vec2(s_curX, s_curY))), nullptr));

		//for (int i=0;i<m_vecSngTableListInfoNew.size(); i++)
		//{
		//	SngTableInfo *table = m_vecSngTableListInfoNew.at(i) ;
		//	table->m_pSpriteBgItem->setVisible(false) ;
		//}

		updateTableScrollView() ;
        auto guidefun = [this](){
            createHelpGuide();
        };
        this->runAction(Sequence::create(DelayTime::create(1.5),CallFunc::create(guidefun), nullptr));
	}
	else
	{
		//s_layoutHelp->setVisible(false) ;
		m_rightupButton->setVisible(false);
		m_leftupButton->setVisible(false);
		m_middleShow->setVisible(false);
		m_SngTips->setVisible(false) ;
		//for (int i=0;i<m_vecSngTableListInfoNew.size(); i++)
		//{
		//	SngTableInfo *table = m_vecSngTableListInfoNew.at(i) ;
		//	table->m_pSpriteBgItem->setVisible(false) ;
		//}
		m_pScrollView->setContentOffset(Vec2(0,-m_tempDis), false);
	}
}


void GameSngHall::callHallAction()
{

	float s_curX= 0;
	float s_curY= 0;


	auto layer1 = m_rightupButton ;
	auto layer2 = m_leftupButton ;
	auto layer3 = m_middleShow ;
	auto autoAction = [this,layer1,layer2,layer3](){

		m_rightupButton->setVisible(true);
		m_leftupButton->setVisible(true);
		m_middleShow->setVisible(true);
		m_middleTips->setVisible(true) ;
	} ;


	for (int i=0;i<m_vecSngTableListInfoNew.size(); i++)
	{
		SngTableInfo *table = m_vecSngTableListInfoNew.at(i) ;

		s_curX = table->m_pSpriteBgItem->getPositionX() ;
		s_curY = table->m_pSpriteBgItem->getPositionY() ;
		table->m_pSpriteBgItem->setPositionY(s_curY-640) ;

		table->m_pSpriteBgItem->runAction(Sequence::create(DelayTime::create(0.5f),
			CCEaseSineOut::create(MoveTo::create(0.3+i*0.2f, Vec2(s_curX, s_curY))), nullptr));
	}

	this->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create(autoAction) , nullptr)) ;
}

void GameSngHall::onGrabTouchBegan(bool bHitDialog)
{
	GameDialog::onGrabTouchBegan(bHitDialog) ;
	if (bHitDialog)
	{
		m_SngTips->setVisible(false) ;
	}
}

void GameSngHall::onMenuItemBottomSelect(cocos2d::Ref *pMenuItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (pMenuItem == m_buttonTips)
		{
			m_SngTips->setVisible(!m_SngTips->isVisible()) ;
			return ;
		}
		m_SngTips->setVisible(false) ;
		//提示关闭
		if (pMenuItem == m_buttonTouch || pMenuItem == m_buttonTouch2)
		{
			m_SngTips->setVisible(false) ;
			return ;
		}

		//返回
		if (pMenuItem == m_buttonBack)
		{
			SendClientRequest(kLeaveGameSngHall) ;
			return ;
		}
		//帮助
		if (pMenuItem == m_buttonhelp)
		{
			showSngRule(true) ;
			return ;
		}
		//排行
		if (pMenuItem == m_buttonrank)
		{
			//showSngRankList(true) ;
			g_pGameFrame->showMasterRankList(true) ;
			return ;
		}
		//商城
		if (pMenuItem == m_buttonshop)
		{
			Json::Value param;
			param["idx"] = 0;
			SendClientRequest(ClientRequest::kPopMall, param);
			return ;
		}
		//客服
		if (pMenuItem == m_buttonkefu)
		{
			SendClientRequest(ClientRequest::kPopCustomerServices);
			return ;
		}

	}
}

/************************************************************************/
/* 更新sng数据                                                                     */
/************************************************************************/
void GameSngHall::updateGameSngData()
{
	setUITextString(m_textName, g_globalMyData.GetNickName().c_str()) ;
	setUITextString(m_textMoney, FormatCash4(g_globalMyData.GetCash())) ;

	char tips[64] ;
	int rank = g_globalMyData.getSngMasterRank() ;

	if (rank == 0 )
	{
		setUITextString(m_textrank, "排名：暂无") ;
	}
	else
	{
		sprintf(tips,"排名：%d",rank) ;
		setUITextString(m_textrank, tips) ;
	}

	int score = g_globalMyData.getSngMasterScore() ;
	sprintf(tips,"大师分：%d",score) ;
	setUITextString(m_textDashiRank, tips) ;


	////sprintf(tips,"排名：%d",0) ;
	//setUITextString(m_textrank, "排名：暂无") ;
	////sprintf(tips,"大师分：%d",0) ;
	//setUITextString(m_textDashiRank, "大师分：暂无") ;

	if (g_globalMyData.GetSkinID() == 0) {
		SetIconUrl(m_spriteHero, g_globalMyData.GetIconURL());
	} else {
		SetIconUrl(m_spriteHero, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
	}

	//updateTableScrollView() ;

	for(auto& followList:g_globalSngHallInfo.m_sngTableInfo)
	{
		SngTableInfo* item;
		item = m_vecSngTableListInfoNew.at(followList.m_tableID);
		UpdateItem(item, followList);
		item->m_pSpriteBgItem->setVisible(true); 
	}

}

void GameSngHall::cleanTableScrollView()
{
	std::vector<SngTableInfo*>::iterator iter;

	while (!m_vecSngTableListInfoNew.empty())
	{

		iter = m_vecSngTableListInfoNew.begin();
		SngTableInfo* item = m_vecSngTableListInfoNew.at(0) ;
		item->m_pSpriteBgItem->removeFromParent() ;
		m_vecSngTableListInfoNew.erase(iter) ;

	}
}

void GameSngHall::creatTableScrollView()
{
	//cleanTableScrollView() ;

	//updateGameSngData() ;

	m_vecSngTableListInfoNew.clear();
	unsigned uCount = g_globalSngHallInfo.m_sngTableInfo.size();
	unsigned ListBgItemH = sizeListView.width;
	unsigned uItemH = TableHeight;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2;
	float fY = fRealH - uItemH / 2 - 10;




	for(auto& followList:g_globalSngHallInfo.m_sngTableInfo)
	{
		auto item = GetSngTableListItem(followList.m_tableID);
		UpdateItem(item, followList);
		item->m_pSpriteBgItem->setPosition(fX, fY);
		item->m_pSpriteBgItem->setVisible(true); 
		m_vecSngTableListInfoNew.push_back(item);
		fY -= uItemH;     
	}
	float temp = fRealH - sizeListView.height;

	//每次都显示滚动框的最上面
	m_pScrollView->setContentOffset(Vec2(0,-temp), false);
	m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
}
void GameSngHall::updateTableScrollView()
{
	bool needUpdate = !m_vecSngTableListInfoNew.empty() ;
	if (!needUpdate)
	{
		unsigned uCount = g_globalSngHallInfo.m_sngTableInfo.size();
		unsigned ListBgItemH = sizeListView.width;
		unsigned uItemH = TableHeight;
		unsigned fRealH = uCount * uItemH;
		if(fRealH < sizeListView.height)
		{
			fRealH = sizeListView.height;
		}
		float fX = sizeListView.width / 2;
		float fY = fRealH - uItemH / 2 ;

		for(auto& followList:g_globalSngHallInfo.m_sngTableInfo)
		{
			SngTableInfo* item;
			if (!needUpdate)
			{
				item = GetSngTableListItem(followList.m_tableID);
				UpdateItem(item, followList);
				m_vecSngTableListInfoNew.push_back(item);
		
				item->m_pSpriteBgItem->setPosition(fX, fY);
				item->m_pSpriteBgItem->setVisible(true); 

				fY -= uItemH;    
			}
			else
			{
				/*	item = m_vecSngTableListInfoNew.at(followList.m_tableID);
				UpdateItem(item, followList);*/
			}
		}
		float temp = fRealH - sizeListView.height;
		m_tempDis = temp ;
		//每次都显示滚动框的最上面
		m_pScrollView->setContentOffset(Vec2(0,-temp), false);
		m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
	}
	else
	{
		m_pScrollView->setContentOffset(Vec2(0,-m_tempDis), false);
	}


	callHallAction() ;


}


GameSngHall::SngTableInfo* GameSngHall::GetSngTableListItem(unsigned tableId)
{
	SngTableInfo *item = nullptr;
	//if(m_vecSngTableListInfoNew.empty())
	//{



	//背景
	auto m_pSpriteBgItem = Sprite::create("gamesng/sng_hall_background-1.png") ;
	m_pScrollView->addChild(m_pSpriteBgItem) ;
	//场景标志
	auto m_spriteIcon = Sprite::create("gamesng/sng_hall_bunting-1.png") ;
	m_spriteIcon->setPosition(119,70) ;
	m_pSpriteBgItem->addChild(m_spriteIcon) ;

	auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(36,"",Vec2(0.5,0.5),119,93,Color4B(0xff,0xff,0xff,0xff),m_pSpriteBgItem,10);


	//场次人数
	auto m_spritecounbg = Sprite::create("gamesng/sng_hall_background-7.png") ;
	m_spritecounbg->setPosition(129,62) ;
	m_pSpriteBgItem->addChild(m_spritecounbg,5) ;
	//场次人数
	auto m_labelMaxPlayerCount = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0.5,0.5),129,62,Color4B(0xff,0xff,0xff,0xff),m_pSpriteBgItem,2);

	//但前人数标志
	auto playerIcon = Sprite::create("gamesng/sng_hall_icon-1.png") ;
	playerIcon->setPosition(238,57) ;
	m_pSpriteBgItem->addChild(playerIcon) ;

	//参赛人数
	auto m_LableroleInGame = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),255,44,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);

	//奖杯
	Sprite* m_spriteCup[3] ;

	//奖励
	Label* m_Lablereward[3] ;
	for (int i=0; i<3; i++)
	{
		char path[64] ;
		sprintf(path,"gamesng/sng_hall_cupIcon-%d.png",i+1) ;
		m_spriteCup[i]  = Sprite::create(path) ;
		m_pSpriteBgItem->addChild(m_spriteCup[i]) ;
		m_Lablereward[i] = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),70,119,Color4B(0xff,0xff,0xff,0xff),m_pSpriteBgItem,2);
	}

	//报名费德州币标志
	auto m_spriteFee = Sprite::create("gamesng/sng_hall_goldCoin-2.png") ;
	m_spriteFee->setPosition(585,58) ;
	m_pSpriteBgItem->addChild(m_spriteFee) ;
	//报名费
	auto m_lableFee = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),602,48,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);
	//门票标志
	auto m_spriteFreesticket = Sprite::create("gamesng/sng_hall_ticket-1.png") ;
	m_spriteFreesticket->setPosition(623,83) ;
	m_pSpriteBgItem->addChild(m_spriteFreesticket) ;
	// 门票数量
	auto m_labelFreeStickCount = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),658,69,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);
	auto m_labelMyStickCount=  CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),597,32,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);

	//报名按钮
	//FrameSpriteButton* m_pBtJiaRu =FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn2.png",false,true);
    FrameScale9SpriteButton* m_pBtJiaRu = FrameScale9SpriteButton::createWithScale9Sprite(2 ,Size(112, 252/3),false,true) ;

	auto labelbaoming = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"报名",Vec2(0.5,0.5),0,0,Color4B::WHITE,m_pBtJiaRu,1);
	m_pBtJiaRu->setPosition(828,62);
	m_pSpriteBgItem->addChild(m_pBtJiaRu);

	auto pItem = new SngTableInfo;
	pItem->m_pSpriteBgItem = m_pSpriteBgItem;
	pItem->m_labelName = labelName ;
	pItem->m_spriteIcon = m_spriteIcon ;

	pItem->m_labelMaxPlayerCount = m_labelMaxPlayerCount ;
	pItem->m_LableroleInGame = m_LableroleInGame ;
	for (int i=0;i<3;i++)
	{
		pItem->m_spriteCup[i] = m_spriteCup[i] ;
		pItem->m_Lablereward[i] = m_Lablereward[i] ;
	}

	pItem->m_spriteFee = m_spriteFee ;
	pItem->m_lableFee = m_lableFee ;
	pItem->m_spriteFreesticket = m_spriteFreesticket ;
	pItem->m_labelFreeStickCount = m_labelFreeStickCount ;
	pItem->m_labelMyStickCount = m_labelMyStickCount ;
	pItem->m_pBtJiaRu = m_pBtJiaRu ;
	pItem->m_tableId = tableId ;
	item = pItem;
	item->m_tableId = tableId;


	return item;
}

void GameSngHall::UpdateItem(GameSngHall::SngTableInfo *item, const GlobalSngTableInfo& followInfo)
{

	char buff[64] ;
	item->m_spriteIcon->setTexture(m_spriteIcon[followInfo.m_ImgID]->getTexture()) ;	

	SetLabelString(item->m_labelName,followInfo.m_tableName.c_str()) ;

	sprintf(buff,"%d",followInfo.m_maxPlayRoleCount) ;
	SetLabelString(item->m_labelMaxPlayerCount,FormatCash4(followInfo.m_maxPlayRoleCount)) ;

	sprintf(buff,"%d",followInfo.m_roleCountInGame) ;
	SetLabelString(item->m_LableroleInGame,FormatCash4(followInfo.m_roleCountInGame)) ;

	for (int i=0;i<3;i++)
	{
		item->m_Lablereward[i]->setVisible(false) ;
		item->m_spriteCup[i]->setVisible(false) ;
	}

	if (followInfo.m_rewardCount==2)
	{
		for (int i=0; i<followInfo.m_rewardCount;i++)
		{
			sprintf(buff,"%d",followInfo.m_reward[i]) ;
			SetLabelString(item->m_Lablereward[i],FormatCash4(followInfo.m_reward[i])) ;
			item->m_Lablereward[i]->setPosition(396,64-i*38) ;
			item->m_spriteCup[i]->setPosition(372,78-i*38) ;
			item->m_Lablereward[i]->setVisible(true) ;
			item->m_spriteCup[i]->setVisible(true) ;
		}
	}

	if (followInfo.m_rewardCount==3)
	{
		for (int i=0; i<followInfo.m_rewardCount;i++)
		{
			sprintf(buff,"%d",followInfo.m_reward[i]) ;
			SetLabelString(item->m_Lablereward[i],FormatCash4(followInfo.m_reward[i])) ;
			item->m_Lablereward[i]->setPosition(396,87-i*38) ;
			item->m_spriteCup[i]->setPosition(373,100-i*38) ;
			item->m_Lablereward[i]->setVisible(true) ;
			item->m_spriteCup[i]->setVisible(true) ;
		}
	}
	item->m_needStick = followInfo.m_freeTicket ;//比赛需要的门票
	bool flag = followInfo.m_myTicket >=followInfo.m_freeTicket; //是否显示门票
	item->m_spriteFee->setVisible(!flag) ;
	item->m_lableFee->setVisible(!flag) ;

	item->m_spriteFreesticket->setVisible(flag) ;
	item->m_labelFreeStickCount->setVisible(flag) ;
	item->m_labelMyStickCount->setVisible(flag) ;

	if (flag)
	{
		sprintf(buff,"x%d",followInfo.m_freeTicket) ;
		SetLabelString(item->m_labelFreeStickCount,buff) ;
		sprintf(buff,"您拥有%d张",followInfo.m_myTicket) ;
		SetLabelString(item->m_labelMyStickCount,buff) ;
	}
	else
	{
		//sprintf(buff,"%d+%d",followInfo.m_entryFee,followInfo.m_fee) ;

		sprintf(buff,"%s+%s",FormatCash4(followInfo.m_entryFee).c_str(),FormatCash4(followInfo.m_fee).c_str()) ;
		SetLabelString(item->m_lableFee,buff) ;
	}

	//item->
	//加入回调
	item->m_pBtJiaRu->SetHitCB(std::bind(&GameSngHall::menuClickJiaRu, this, followInfo));


}

void GameSngHall::menuClickJiaRu( const GlobalSngTableInfo& followInfo)
{
	

	bool flag = false ;
	int allcash = followInfo.m_fee +followInfo.m_entryFee ;

	//是否有钱
	if (g_globalMyData.GetCash()>=allcash)
	{
		flag = true ;
	}
	//是否有票
	if (followInfo.m_myTicket>0)
	{
		flag = true ;
	}

	if (flag)
	{
		g_pGameFrame->callShowSngSignUp(true,followInfo) ;
		//SendClientRequest(ClientRequest::kCallShowSngSignUp) ;
		/*showSngSignUp(true) ;
		m_sngSignUp->UpdateView(followInfo) ;*/
	}
	else
	{
		PopTipWithBt2("您的德州币不足，是否购买进入商城购买德州币！","取消","商城",[](unsigned ubt)
		{
			if(ubt == 1)
			{
				Json::Value param;
				param["idx"] = 0;
				SendClientRequest(ClientRequest::kPopMall, param);
			}
		});
	}



}

bool GameSngHall::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return false ;
}

void GameSngHall::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

	if (view == m_pScrollView)
	{
		if (m_SngTips->isVisible())
		{
			m_SngTips->setVisible(false) ;
		}

	}
}

void GameSngHall::showBlack(bool flag)
{
	m_layerColorBg->setVisible(flag) ;
}

void GameSngHall::showSngRule(bool flag)
{
	if (m_sngRule == nullptr)
	{
	
		m_sngRule = SngRule::create() ;
		m_sngRule -> ignoreAnchorPointForPosition(false);
		auto itemSz = m_sngRule->getContentSize();;
		m_sngRule -> setAnchorPoint(Point(0.5,0.5));
		m_sngRule->setPosition(GameData::getInstance()->getGameWidth()/2 ,
			GameData::getInstance()->getGameHeight()/2 );

		addChild(m_sngRule,ZOrder_Rule);	
	}

	m_sngRule->setVisible(flag) ;
	showBlack(flag) ;
}

//void GameSngHall::showSngRankList(bool flag)
//{
//	if (m_sngRankList == nullptr)
//	{
//
//		m_sngRankList = SngRankList::create() ;
//		m_sngRankList -> ignoreAnchorPointForPosition(false);
//		auto itemSz = m_sngRankList->getContentSize();;
//		m_sngRankList -> setAnchorPoint(Point(0.5,0.5));
//		m_sngRankList->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
//		addChild(m_sngRankList,ZOrder_Rule);	
//	}
//	m_sngRankList->setVisible(flag) ;
//	showBlack(flag) ;
//}

//void GameSngHall::showSngSignUp(bool flag)
//{
//	SendClientRequest(ClientRequest::kCallShowSngSignUp) ;
//	//if (m_sngSignUp == nullptr)
//	//{
//	//	float ratioDif = (GameData::getInstance()->getGameWidth() / GameData::getInstance()->getGameHeight() - 1.5)/2.75;
//	//	if (ratioDif < 0) ratioDif = 0;
//	//	m_sngSignUp = SngSignUp::create() ;
//	//	m_sngSignUp -> ignoreAnchorPointForPosition(false);
//	//	auto itemSz = m_sngSignUp->getContentSize();;
//	//	m_sngSignUp -> setAnchorPoint(Point(0.5,0.5));
//	//	m_sngSignUp->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
//
//	//	addChild(m_sngSignUp,ZOrder_Rule);	
//	//}
//
//	//m_sngSignUp->setVisible(flag) ;
//	showBlack(flag) ;
//}


//改变对应的记录
void GameSngHall::updateItemList( const GlobalSngTableInfo& userInfo)
{
	for(auto item:m_vecSngTableListInfoNew)
	{
		if(item->m_tableId == userInfo.m_tableID)
		{
			UpdateItem(item, userInfo);
		}
	}
}

void GameSngHall::OnUpdateMyData(unsigned uDataType)
{
	if (uDataType == WAITCASHCHANGE)
	{
		setUITextString(m_textMoney, FormatCash4(g_globalMyData.GetCash())) ;

	}

}

//void GameSngHall::UpdateSngRankList(unsigned uRankType, unsigned uWinType)
//{
//	if (m_sngRankList && m_sngRankList->isVisible())
//	{
//		m_sngRankList->UpdateRankList(uRankType, uWinType);
//	}
//}

void GameSngHall::onKeyEscPressed()
{
	SendClientRequest(ClientRequest::kLeaveGameSngHall);
}

void GameSngHall::createHelpGuide()
{
    if(GuideInfo->readGuideInfo(SNGGuide))
        return;

    auto guideTips = GuideTips::create("guidetips/sngtips.png", "guidetips/sngflag.png");
    //this->addChild(guideTips, ZOrder_HelpGuide);
    auto pos = m_buttonhelp->convertToWorldSpace(Vec2(m_buttonhelp->getContentSize().width/2, m_buttonhelp->getContentSize().height/2));
    guideTips->setFlagPosition(pos);
    guideTips->setBgPosition(Vec2(pos.x-95, pos.y-135));
    auto pClip = CreateClippingMask("guidetips/sngmask.png", pos, guideTips);
    this->addChild(pClip, ZOrder_HelpGuide);
    guideTips->setTipsID(SNGGuide);
}