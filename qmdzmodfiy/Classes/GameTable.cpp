#include "GameTable.h"
#include "Util.h"
#include "UserView.h"
#include <algorithm>
#include "Card.h"
#include "Jetton.h"
#include "TextureNumber.h"
#include "Delaer.h"
#include <sstream>
#include "MenuItemSpriteWithLabel.h"
#include "ActionBar.h"
#include "DoorMenu.h"
#include "QuickShop.h"
#include "ChatBox.h"
#include "ShowLabelInLineSequence.h"
#include "GrabTouch.h"
#include "UserInfo.h"
#include "FrameSprite.h"
#include "GrabKeyboard.h"
#include "GlobalUserData.h"
#include "GameLogic.h"
#include "QuickMissionList.h"
#include "QuickInviteList.h"
#include "HttpDownloader.h"
#include "AnimateViewer.h"
#include "MenuItemFrameSprite.h"
#include "FrameSpriteButton.h"
#include "WinningRanking.h"
#include "PlatformSpec.h"
#include "LogonLayerEx.h"
#include "AnimationManager.h"
#include "Tools.h"
#include "GameData.h"
#include "HttpSprite.h"
#include "TaskSet.h"
#include "ShowFamily.h"
#include "CfgReader.h"
#include "GameFrame.h"
#include "SngRankView.h"
#include "SngMatchState.h"
#include "LineLabel.h"
#include "MoveLabel.h"
#include "GuideTips.h"
#include "GameTipsManager.h"
#include "MttRankView.h"
#include "PrivateRoomInfo.h"
#include "TableTips.h"
#include "MTTMatchState.h"
#include "GameConfig.h"
USING_NS_CC;

#define MY_CHAIR_ID (4)

enum
{
	TableBGZOrder,
	DealerZOrder,
	UserViewZOrder,
	ThinkingBarZOrder,
	SngRank,
	ExpressTools,
	SngAuto,
	DispatchingCardZOrder,

	DFlagZOrder,
	MyCardTypeZOrder,
	MyCardZOrder,
	AniShowMaxCardZOrder,
	PublicCardZOrder,
    PublicCardCashZOrder,
	AniShowMeWinZOrder,
	SidePotZOrder, // 边池
	UserPotZOrder, // 用户池
	BetJettonZOrder,
	FoldingCardZOrder,
	WinJettonZOrder,
	WinNumberZOrder,
    VideoPlayerZOrder,
	GameTipsZOrder,
	FollowFlagZOrder,
	ChatTipZOrder,
	QuickMissionListZOrder,
	//QuickInviteListZOrder,
	WinningRakingZOrder,
	RewardZOrder,
	ToolZOrder,
	ActionBarZOrder,
	ActionBrightZOrder,
	PrivateRoomInfoZOrder,
	DoorMenuZOrder,
	CardTypeViewZOrder,
	QuickShopZOrder,
    NewPlayerTaskZOrder,
    AttentionGuide,
    QuickFriendGuide,
	ChatBoxZOrder,
	UserInfoZOrder,
	
	ShowFamilyZOrder,
    QuickInviteListZOrder,
	MttRebuyTipsZOrder,
	MttAddonTipsZOrder,
    MTTRankViewZOrder,
	TableTipZOrder,

};



bool GameTable::init()
{

	for (auto& item : m_bUserValid)
	{
		item = false;
	}
	m_curTableType = NullType ;

	//if (!GameDialog::init("bgtable.png", false))
	//{
	//	return false;
	//}
	if (!GameDialog::init())
	{
		return false;
	}
	m_pNormalSpriteBG = nullptr ;
	m_spritetableTitle =nullptr;
	m_sprite_tableLeft = nullptr ;
	m_sprite_tableRight = nullptr ;
    m_RewardBubble = nullptr;
    m_MTTRankView = nullptr;

	m_sngInitialized = false;

	Size sizebg = Director::getInstance()->getWinSize();



	Point s_curPos ;
	//载入gametable ui布局
	//载入布局
	s_uiRoot = CSLoader::createNode("res/ui/gametable.csb");
	//s_uiRoot->setTag(100001);
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	s_uiRoot->setContentSize(frameSize); 
	ui::Helper::doLayout(s_uiRoot);  
	addChild(s_uiRoot);
	setContentSize(frameSize); 

	GameData::getInstance()->initGameTablePos(s_uiRoot);

	s_gameLayer = Tools::getWidgetByName(s_uiRoot,"Panel_GameTable");
	m_layer_GameTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"tb_gametable"));
	Layout* m_colorBlock = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_1"));
	m_colorBlock->setVisible(false) ;

	m_img_tableLeft =  static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"gt_lefttable1"));
	m_img_tableRight =  static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"gt_righttable1"));


	m_imgtitle = static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"Node_sngtitle"));
	if (m_spritetableTitle==nullptr)
	{
		m_spritetableTitle = Sprite::create("sng_title.png");
		//this->addChild(m_spritetableTitle,50) ;
		
		m_imgtitle->addChild(m_spritetableTitle) ;
	}


	//取消ui中玩家的显示
	Node* nodeAllUser = Tools::getWidgetByName(m_layer_GameTable,"Panel_alluser");
	nodeAllUser->setVisible(false);

	//取消ui中 奖池的显示
	Node* nodeAllUserallin = Tools::getWidgetByName(s_uiRoot,"Panel_userallin");
	nodeAllUserallin->setVisible(false);

	

	Text* m_pLabelGameID =  static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_huifang_id"));
	m_pLabelGameID->setVisible(false);

	//更新坐标   寻找好麻烦  实例化一次 保存下来
	for (int i=0; i< GAME_PLAYER; i++)
	{
		char pathUserBlock[64];
		sprintf(pathUserBlock,"user_block%d",i) ;
		m_rootUser[i] = static_cast<ImageView*>(Tools::getWidgetByName(nodeAllUser,pathUserBlock));
		m_usePosInfo[i] = GameData::getInstance()->m_usePosInfo[i] ;
		s_ptJettonDst[i] = GameData::getInstance()->s_ptJettonDst[i];
		s_ptLabelBet[i] = GameData::getInstance()->s_ptLabelBet[i];
		s_ptBgBet[i] = GameData::getInstance()->s_ptBgBet[i];
		s_ptDPosition[i] = GameData::getInstance()->s_ptDPosition[i];
		s_ptUserViewDefault[i] = GameData::getInstance()->s_ptUserViewDefault[i];
		s_ptUserViewDefaultCenter[i] = GameData::getInstance()->s_ptUserViewDefaultCenter[i];
		s_ptPots[i] = GameData::getInstance()->s_ptPots[i];
		s_ptChatTip[i] = GameData::getInstance()->s_ptChatTip[i];
		s_ptFoldingCard[i] = GameData::getInstance()->s_ptFoldingCard[i];
		s_ptUserViewOutTable[i]= GameData::getInstance()->s_ptUserViewOutTable[i];
	}


	for (int i=0 ;i<5 ;i++)
	{
		s_ptPublicCard[i] = GameData::getInstance()->s_ptPublicCard[i];
	}

	for (int i=0 ; i< GAME_PLAYER; i++)
	{
			s_ptDispatchedCard[i] = GameData::getInstance()->s_ptDispatchedCard[i];
			s_ptDispatchedCard[i+9] = GameData::getInstance()->s_ptDispatchedCard[i+9];
			s_allinChizi[i+9] = GameData::getInstance()->s_allinChizi[i+9];
	}

	long localTime4 = getCurrentTime();
	//log("GameTable 3: %ld", localTime4 - localTime3);

	s_ptMyCard[0]= GameData::getInstance()->s_ptMyCard[0];
	s_ptMyCard[1]= GameData::getInstance()->s_ptMyCard[1];



    m_IsWaitingSitDown = false;
	m_uDispatchingIdx = 0;
	m_uCurSidePotCount = 0;
	m_uBetingCount = 0;
	m_funCBAllBetingFinished = nullptr; 
	m_bCollecting = false;
	m_funCBCollectingFinished = nullptr;
	m_uOverTime = 0;
	m_cbMyCardData[0] = m_cbMyCardData[1] = 0;

	//if (!GameDialog::init())
	//{
	//	return false;
	//}
	//setContentSize(Size(GAME_WIDTH, GAME_HEIGHT));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float fCenterX = origin.x + visibleSize.width / 2;
	float fCenterY = origin.y + visibleSize.height / 2;

	//游戏房间信息
	m_pLabelTableDesc = static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_gameinfo"));
	m_pLabelTableDesc->setVisible(true);
	setUITextString(m_pLabelTableDesc, g_globalTableConfig.m_strRoomName) ;

	m_dAllPlacedJettonValue = 0;
	m_pLabelAllPlacedJettonValue = static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_dichi"));
	//m_pLabelAllPlacedJettonValue->setTextColor(Color4B(0x63, 0xb2, 0x73, 255));
	m_pLabelAllPlacedJettonValue->setVisible(true);
	setUITextString(m_pLabelAllPlacedJettonValue,"底池：0") ;

	//时间
	m_pLabelTime = static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_gametime"));
	m_pLabelTime->setTextColor(Color4B(125, 125, 125, 255));
	schedule(schedule_selector(GameTable::onScheduleUpdateTime), 31, 360000, 0);
	m_pLabelTime->setVisible(true);

	onScheduleUpdateTime(0);
	//打赏
	m_pMenuItemReward = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_dashang"));
	m_pMenuItemReward->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemReward->setVisible(true);

	//菜单
	m_pMenuItemPopMenu = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_menu"));
	m_pMenuItemPopMenu->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemPopMenu->setVisible(true);


	//对话
	m_pMenuItemPopChat = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_talkchat"));
	m_pMenuItemPopChat->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemPopChat->setVisible(true);

	//语音
	m_pMenuItemRecordSound = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_talksound"));
	m_pMenuItemRecordSound->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
#if OpenSoundRecorder
		m_pMenuItemRecordSound->setVisible(true);
#else
		m_pMenuItemRecordSound->setVisible(false);
#endif 


	//任务宝箱
	m_pMenuItemPopMissionReward = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_baoxiang"));
	m_pMenuItemPopMissionReward->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemPopMissionReward->setVisible(true) ;


	//宝箱动画
	//btpopmissionreward1
	m_pRewardAnimateViewer = AnimationManager::creatArmature("renwu_baoxiang_dakai");

	m_pRewardAnimateViewer -> ignoreAnchorPointForPosition(false);
	m_pRewardAnimateViewer -> setAnchorPoint(Point(0.5,0.5));
	//m_pRewardAnimateViewer->setPosition(m_pMenuItemPopMissionReward->getPosition());

	m_pRewardAnimateViewer->playByIndexAndRepatForever(0);
	m_pMenuItemPopMissionRewardAni = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_baoxiang_0"));
    auto szbox = m_pMenuItemPopMissionRewardAni->getContentSize();
    m_pRewardAnimateViewer->setPosition(szbox.width/2+5, szbox.height/2+15);
	m_pMenuItemPopMissionRewardAni->addChild(m_pRewardAnimateViewer);




	// 对有可以领取的任务的宝箱可以监听
	auto nBgItemCashListen = LayerButton::create(true, true);
	nBgItemCashListen->ignoreAnchorPointForPosition(false);
	nBgItemCashListen->setAnchorPoint(Point(0.5,0.5));

	m_pRewardAnimateViewer->addChild(nBgItemCashListen);
	auto szBt = m_pRewardAnimateViewer->getContentSize();
	nBgItemCashListen->setContentSize(szBt);
	nBgItemCashListen->SetHitCB([this](Ref*)
	{
		if (m_pQuickMissionList == nullptr)
		{
			m_pQuickMissionList = QuickMissionList::create();
			m_pQuickMissionList -> ignoreAnchorPointForPosition(false);
			m_pQuickMissionList -> setAnchorPoint(Point(1,1));
			m_pQuickMissionList->setPosition(Director::getInstance()->getVisibleSize().width-10,
				m_pMenuItemPopMissionReward->getPosition().y+m_pMenuItemPopMissionReward->getContentSize().height/2);

			addChild(m_pQuickMissionList, QuickMissionListZOrder);
		}
		m_pQuickMissionList->setVisible(true);
	});


	m_layerRightUpNormal = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup"));
	m_layerRightUpSng = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup_0"));
	//商城
	m_pMenuItemPopMall = static_cast<Button*>(Tools::getWidgetByName(m_layerRightUpNormal,"gt_bt_buy"));
	m_pMenuItemPopMall->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemPopMall->setVisible(true);
	//任务
	m_pMenuItemPopMission = static_cast<Button*>(Tools::getWidgetByName(m_layerRightUpNormal,"gt_bt_task"));
	m_pMenuItemPopMission->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemPopMission->setVisible(true);
	//邀请
	m_pMenuItemPopInvite = static_cast<Button*>(Tools::getWidgetByName(m_layerRightUpNormal,"gt_tb_friend"));
	m_pMenuItemPopInvite->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemPopInvite->setVisible(true);




    m_pSpriteInviteFlag = CreateNode1AndPosAddTo<Sprite>("inviteflag.png", 46, 41, m_pMenuItemPopInvite, 0);
    if (g_globalQuickFollowInfo.m_bIsReceiveInvite)
    {
        m_pSpriteInviteFlag->setVisible(true);
        g_globalQuickFollowInfo.m_bIsReceiveInvite = false;
        m_bIsReceiveInvite = true;
    }
    else
    {
        m_pSpriteInviteFlag->setVisible(false);
        m_bIsReceiveInvite = false;
    }


	//奖励
	m_pMenuItemWinningRanking = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_jiangshi"));
	m_pMenuItemWinningRanking->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;


	Size sz = m_pMenuItemWinningRanking->getContentSize();
	m_pRewardNumber = TextureNumber::create("rewardranklist/num.png", false);
	m_pRewardNumber->setPosition(sz.width / 2, sz.height / 2 + 1);
	m_pMenuItemWinningRanking->addChild(m_pRewardNumber);


	m_pMenuItemSitDown = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_zuoxia"));
	m_pMenuItemSitDown->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemSitDown->setVisible(false);

	m_pMenuItemAutoSitDown = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_zidongzuoxia"));
	m_pMenuItemAutoSitDown->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemClicked) ) ;
	m_pMenuItemAutoSitDown->setVisible(false);

	m_pLabelWaitPlayerCount =  static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_dengdairenshu"));
	m_pLabelWaitPlayerCount->setTextColor(Color4B(0xff, 0xd2, 0, 0xff));
	m_pMenuItemAutoSitDown->setVisible(false);


	
	//等待加入动画
	m_pAniWaitSitDown = AnimateViewer::create("login/waitbegin.png", true, 0.1f, 3, 3, 8);

	
	m_pAniWaitSitDown->setPosition(Tools::getWidgetByName(s_uiRoot,"gt_node_sitani")->getPosition());
	//m_pAniWaitSitDown->setPosition(1100, 34);
	addChild(m_pAniWaitSitDown);


	//等待坐下
	m_pLabelWaitSitDown =static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_wait_fordit"));
	m_pLabelWaitSitDown->setVisible(false);

	//亮牌
	m_pMenuItemBrightCard =static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_liangpai"));
	m_pMenuItemBrightCard->addTouchEventListener(this,toucheventselector(GameTable::onMenuItemAction) ) ;
	m_pMenuItemBrightCard->setVisible(false);


	//牌型提示
	m_pSpriteMaxCardType = FrameSprite::create("userview/cardtype.png", 1, 11);
	m_pSpriteMaxCardType -> ignoreAnchorPointForPosition(false);
	m_pSpriteMaxCardType -> setAnchorPoint(Point(0.22,0.5));
	m_pSpriteMaxCardType->setPosition(Tools::getWidgetByName(s_uiRoot,"px")->getPosition());
	s_gameLayer->addChild(m_pSpriteMaxCardType, MyCardTypeZOrder);
    // m_pSpriteMaxCardType->setVisible(false); // 默认第一帧是透明的，不需要隐藏

	//的女郎
	m_pDealer = Dealer::create();
	m_node_denvlang= Tools::getWidgetByName(s_uiRoot,"node_denvlang") ;
	s_curPos = Tools::getWidgetByName(s_uiRoot,"node_denvlang")->getPosition() ;
	//m_pDealer->setPosition(Tools::getWidgetByName(s_uiRoot,"node_denvlang")->getPosition());
	//s_gameLayer->addChild(m_pDealer, DealerZOrder);
	m_node_denvlang->addChild(m_pDealer, DealerZOrder);
	m_pSpriteDFlag = nullptr;

	for (auto& pSprite : m_pSpriteDispatchingCard)
	{
		pSprite = nullptr;
	}

	for (auto& pSprite : m_pSpriteFoldingCard)
	{
		pSprite = nullptr;
	}

	for (auto& pUser : m_pUsers)
	{
		pUser = nullptr;
	}

	for (auto& item : m_pUsersViewTouchGrabber)
	{
		item = nullptr;
	}

	m_pSpriteShowMaxCard = Sprite::create();
	m_pSpriteShowMaxCard->setPosition(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight()/2+114);
	m_pSpriteShowMaxCard->setVisible(false);
	s_gameLayer->addChild(m_pSpriteShowMaxCard, AniShowMaxCardZOrder);

	for (auto& item : m_pAniShowMaxCard)
	{
		item = nullptr;
	}
	const char* effectCard[4] = 
	{
		"hulu" ,"jingang" ,"tonghuashun" ,"huangjiatonghuashun" ,
	} ;
	for (int i=0; i<4; i++)
	{
 
		m_pAniShowMaxCard[i] = AnimationManager::creatArmature(effectCard[i]);

		m_pAniShowMaxCard[i]->setPosition(0,0) ;
		m_pSpriteShowMaxCard->addChild(m_pAniShowMaxCard[i]);  
		m_pAniShowMaxCard[i]->setVisible(false) ;

	}
	m_pSpriteShowMaxCard->setVisible(true) ;


	m_pSpriteShowMeWin = Sprite::create();
	m_pSpriteShowMeWin->setPosition(GameData::getInstance()->getGameWidth()/2, 266+50);
	m_pSpriteShowMeWin->setVisible(false);
	addChild(m_pSpriteShowMeWin, AniShowMeWinZOrder);

	m_pAniShowMeWin = nullptr;	
	m_pAniShowMeWin = AnimationManager::creatArmature("niyingle");
	m_pSpriteShowMeWin->addChild(m_pAniShowMeWin);  
	m_pAniShowMeWin->setVisible(false) ;

	m_strOldChat = "";

	for (int i = 0; i < 5; i++)
	{
		m_pSpritePublicCard[i] = nullptr ;
		m_hasPublicCard[i] = false ;
	}
	//for (auto& pSprite : m_pSpritePublicCard)
	//{
	//	pSprite = nullptr;
	//}
	
    for (auto& pSpriteCash : m_pSpritePublicCardCash)
    {
        pSpriteCash = nullptr;
    }

	for (auto& pSprite : m_pSpriteMyCard)
	{
		pSprite = nullptr;
	}

	for (auto& pitem : m_dUsrPotValue)
	{
		pitem = 0;
	}

	for (auto& pitem : m_pSpriteJetton2UsrPot)
	{
		pitem = nullptr;
	}

	for (auto& pitem : m_pSpriteJetton2SidePot)
	{
		pitem = nullptr;
	}

	for (auto& pitem : m_pSpriteBgUsrPot)
	{
		pitem = nullptr;
	}

	for (auto& pitem : m_pLabelUsrPotValue)
	{
		pitem = nullptr;
	}

	for (auto& pitem : m_pWinNumber)
	{
		pitem = nullptr;
	}

	m_pJettonFactory = new JettonFactory();

	m_pActionBar = nullptr;
	m_pPreActionBar = nullptr;
	m_pBrightActionBar = nullptr;
	m_pDoorMenu = nullptr;
	m_pCardTypeView = nullptr;
	m_pQuickShop = nullptr;
	m_pChatBox = nullptr;
    m_pNewPlayerTask = nullptr;
    m_uPreThinkChairID = -1;

	for (auto& item : m_pUsrChatTip)
	{
		item = nullptr;
	}
	m_pTableTip = nullptr;
	m_pUserInfoView = nullptr;

	m_targetFamily = nullptr;
	m_pShowFamily = nullptr;
	m_pQuickMissionList = nullptr;
	m_pQuickInviteList = nullptr;
	m_pWinningRaking = nullptr;
    m_WaitBeginGame = nullptr;
    m_strNickName = "";

	m_videoPlayMgr.InitTable(s_gameLayer, VideoPlayerZOrder);


    if (m_pNewPlayerTask == nullptr)
    {
        /*m_pNewPlayerTask = NewPlayerTask::create();
        m_pNewPlayerTask -> ignoreAnchorPointForPosition(false);
        m_pNewPlayerTask -> setAnchorPoint(Point(0.5,0.5));
        m_pNewPlayerTask->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
        addChild(m_pNewPlayerTask, NewPlayerTaskZOrder);
        m_pNewPlayerTask->setVisible(false);*/
    }

	if (m_pActionBar == nullptr)
	{
		//m_pActionBar = ActionBar::create();
		m_pActionBar = ActionBar::creatWithLayer(s_uiRoot);
		m_pActionBar->setTable(this) ;
		m_pActionBar->setVisible(false);
		//addChild(m_pActionBar);
		s_gameLayer->addChild(m_pActionBar, ActionBarZOrder);
	}

	if (nullptr == m_pPreActionBar)
	{
		//m_pPreActionBar = PreActionBar::create();
		m_pPreActionBar = PreActionBar::creatWithLayer(s_uiRoot);
		m_pPreActionBar->setVisible(false);
		s_gameLayer->addChild(m_pPreActionBar, ActionBarZOrder);
	}

	if(nullptr == m_pBrightActionBar)
	{
		//m_pBrightActionBar = BrightActionBar::create();
		m_pBrightActionBar = BrightActionBar::creatWithLayer(s_uiRoot);
		s_gameLayer->addChild(m_pBrightActionBar, ActionBrightZOrder);
		m_pBrightActionBar->setVisible(false);
	}

	for (int i = 0; i < 12; i++)
	{
		m_aniTools[i] = nullptr ;
	}

	m_aniToolsRuhua = nullptr;

	setVisible(false);

	long localTime5 = getCurrentTime();
	log("GameTable 4: %ld", localTime5 - localTime4);

	initUsrExpress();

	//long localTime6 = getCurrentTime();
	//log("GameTable 4-1: %ld", localTime6 - localTime5);

	initSngTable();
	initMttTable() ;
	initPrivateRoomInfo() ;
	showSngMatchState(false);

	return true;
}

void GameTable::cleanAllPaijuAction()
{
	for (int i=0; i<4; i++)
	{
		if (m_pAniShowMaxCard[i])
		{
				m_pAniShowMaxCard[i]->setVisible(false) ;
		}

	}
}

void GameTable::CaoDanDeSendReady()
{
	if (g_globalTableConfig.GetTableMode() == TablePlaying)
	{
		Json::Value param;
		param["cmd"] = 101;
		param["msg"] = Json::Value();
		SendClientRequest(ClientRequest::kCaoDanDeSendGameMsg, param);
	}
}

void GameTable::CaoDanDeSendLittleBlind()
{
	Json::Value param;
	param["cmd"] = 713;
	Json::Value msg;
	msg["betJettonNum"] = g_globalTableConfig.m_dLittleBlind;
	msg["betType"] = 2;
	msg["mangTag"] = 1;
	param["msg"] = msg;
	SendClientRequest(ClientRequest::kCaoDanDeSendGameMsg, param);
}

void GameTable::CaoDanDeSendBigBlind()
{
	Json::Value param;
	param["cmd"] = 713;
	Json::Value msg;
	msg["betJettonNum"] = g_globalTableConfig.m_dBigBlind;
	msg["betType"] = 2;
	msg["mangTag"] = 2;
	param["msg"] = msg;
	SendClientRequest(ClientRequest::kCaoDanDeSendGameMsg, param);
}

void GameTable::CleanTableAfterEnd()
{
	bool bLookOn = g_globalTableConfig.GetTableMode() == TableLooking;
	if (bLookOn)
	{
		for (int i = 0; i < 9; i++)
		{
			if (m_pUsers[i] != nullptr)
			{
				m_pUsers[i]->ShowWaitNextGame();
			}
		}
        for (auto& pSprite : m_pSpriteDispatchingCard)
        {
            if (pSprite)
            {
                pSprite->stopAllActions();
                pSprite->setVisible(false);
            }		
        }
        for (auto& pSprite : m_pSpritePublicCard)
        {
            if (pSprite)
            {
                pSprite->stopAllActions();
                pSprite->setVisible(false);
            }
        }
		return; // 有可能游戏结束后，迅速站起，导致这里还会被执行到，但是这个时候游戏可能已经开始了，如果清理导致桌面出错，干脆不清理了
	}

	if (g_globalGameSerialization.IsInGameNow()) // 刚刚进入桌子，然后游戏结束后2.5s开始执行到这里，但是有可能游戏已经开始了,正常情况下，游戏结束后，这里clean完了之后，再发送准备消息，游戏才会开始的
	{
		return;
	}
    if (m_WaitBeginGame == nullptr)
    {//等待开局
        m_WaitBeginGame = WaitBeginGame::create();
		m_WaitBeginGame -> ignoreAnchorPointForPosition(false);
		m_WaitBeginGame -> setAnchorPoint(Point(0.5,0.5));
        m_WaitBeginGame->setPosition(GameData::getInstance()->getGameWidth()/2, 419);
        m_WaitBeginGame->setVisible(false);
        s_gameLayer->addChild(m_WaitBeginGame, 0);
    }
    //if (g_globalTableConfig.m_vecPlayerInfo.size() > 1)


    m_WaitBeginGame->setVisible(true);


	ActivateUserStatus(US_WAITNEXTGAME, 0);
	for (auto& pSprite : m_pSpriteDispatchingCard)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}		
	}

	for (auto& pSprite : m_pSpriteMyCard)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}
	}
	for (auto& pSprite : m_pSpritePublicCard)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}
	}
	for (auto& item : m_pWinNumber)
	{
		if (item)
		{
			item->stopAllActions();
			item->setVisible(false);
		}
	}

	if (m_pMenuItemBrightCard)
	{
		m_pMenuItemBrightCard->setVisible(false);
	}
	if (m_pSpriteMaxCardType)
	{
		m_pSpriteMaxCardType->SetFrameIdx(0);
	}
	SetAllPot(0);

	for (auto item : m_vecPSidePot)
	{
		if (nullptr != item)
			item->removeFromParentAndCleanup(true);
	}
	m_vecPSidePot.clear();

	m_pSpriteShowMaxCard->setVisible(false);
	m_pSpriteShowMaxCard->stopAllActions();
	cleanAllPaijuAction() ;


	m_pSpriteShowMeWin->setVisible(false);
	m_pSpriteShowMeWin->stopAllActions();
	
	for (int i = 0; i < 9; i++)
	{
		if (m_pUsers[i] != nullptr)
		{
			m_pUsers[i]->ShowWaitNextGame();
		}
	}

	//sng
	if (getTableType() == SngTable || getTableType() == MttTable)
	{
		if (isSngAutoMove)
		{
			isSngAutoMove = false ;
			//showGameAutoTrust(false);
		}
	}


}

void GameTable::CleanTable()
{
	m_vecLayer.clear() ;
	m_CurTouchChairId = -1 ;
    if (m_WaitBeginGame != nullptr)
        m_WaitBeginGame->setVisible(false);
	m_uDispatchingIdx = 0;
	m_vecDispatchingChairID.clear();
	m_vecPublicCardData.clear();
	m_deqDispatchingPublicCardData.clear();
	m_bAllPublicCardDispatched = false;

    m_uPreThinkChairID = -1;

	m_pSpriteMaxCardType->SetFrameIdx(0);

	m_dAllPlacedJettonValue = 0;
	//SetLabelString(m_pLabelAllPlacedJettonValue, "0");
	setUITextString(m_pLabelAllPlacedJettonValue,"底池：0") ;
	m_uBetingCount = 0;
	m_funCBAllBetingFinished = nullptr;

	m_bCollecting = false;
	m_funCBCollectingFinished = nullptr;

	m_funCBPublicCardDispatched = nullptr;

	m_bOpenCarding = false;
	m_bOpenCardFinished = false;
	m_uWinType = 0;
	m_vecWinChairs.clear();
	m_vecUsrWinInfo.clear();

	m_pSpriteShowMaxCard->setVisible(false);
	m_pSpriteShowMaxCard->stopAllActions();
	cleanAllPaijuAction() ;

	m_pSpriteShowMeWin->setVisible(false);
	m_pSpriteShowMeWin->stopAllActions();

	for (auto& pSprite : m_pSpriteDispatchingCard)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}		
	}
	for (auto& pSprite : m_pSpriteFoldingCard)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}
	}
	for (auto& item : m_pUsers)
	{
		if (item)
		{
			//item->reset();
			item->ShowNickName();
		}
	}
	for (int i=0; i<5; i++)
	{
		if (m_hasPublicCard[i]&&m_pSpritePublicCard[i])
		{
			m_pSpritePublicCard[i]->removeFromParentAndCleanup(true);
			m_pSpritePublicCard[i] = nullptr ;
			m_hasPublicCard[i] = false ;
		}
	}

	//for (auto& pSprite : m_pSpritePublicCard)
	//{
	//	if (pSprite)
	//	{
	//		pSprite->stopAllActions();
	//		pSprite->setVisible(false);

	//		pSprite->removeFromParentAndCleanup(true);
	//		pSprite = nullptr;
	//	}
	//}

    for (auto& pSpriteCash : m_pSpritePublicCardCash)
    {
		pSpriteCash = nullptr;
        //if (pSpriteCash)
        //{
        //    //pSprite->stopAllActions();
        //    pSpriteCash->setVisible(false);
        //}
    }

	m_cbMyCardData[0] = m_cbMyCardData[1] = 0;
	for (auto& pSprite : m_pSpriteMyCard)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}
	}

	for (auto& item : m_dUsrPotValue)
	{
		item = 0;
	}

	for (auto& pSprite : m_pSpriteJetton2UsrPot)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}
	}

	for (auto& pSprite : m_pSpriteJetton2SidePot)
	{
		if (pSprite)
		{
			pSprite->stopAllActions();
			pSprite->setVisible(false);
		}
	}

	for (auto& pSprite : m_pSpriteBgUsrPot)
	{
		if (pSprite)
		{
			pSprite->setVisible(false);
		}
	}

	for (auto& item : m_pLabelUsrPotValue)
	{
		if (item)
		{
			SetLabelString(item, "");
		}
	}

	m_vecSidePotValue.clear();

	for (auto& item : m_vecPSidePot)
	{
		if (item)
		{
			item->UpdatePotValue(0);
		}
	}
	m_uCurSidePotCount = 0;

	for (auto& item : m_pWinNumber)
	{
		if (item)
		{
			item->stopAllActions();
			item->setVisible(false);
		}
	}

	if (m_pActionBar)
	{
		m_pActionBar->setVisible(false);
	}
	if (m_pPreActionBar)
	{
		m_pPreActionBar->setVisible(false);
	}
	if(m_pBrightActionBar)
	{
		m_pBrightActionBar->setVisible(false);
		m_pMenuItemBrightCard->setVisible(false);
	}


	if (m_pUserInfoView!=nullptr&&m_pUserInfoView->isVisible())
	{
		m_pUserInfoView->setVisible(false) ;
		m_pUserInfoView->setMoveUp(false) ;
	}
	if (m_sngMatchState != nullptr &&m_sngMatchState->isVisible())
	{
		showSngMatchState(false) ;
	}

	//m_sngAutoLayer->setVisible(false) ;

	if (m_MTTRankView !=nullptr &&m_MTTRankView->isVisible()&&!g_globaMttGameInfo.m_isGameStart)
	{
		showMttRankView(false) ;
	}


	if (m_mttAddonTips != nullptr)
	{
		showMttAddonTips(false) ;
	}
	if (m_mttRebuyTips != nullptr)
	{
		showMttRebuyTips(false) ;
	}
	if (m_mttRebuyTipsBySercer != nullptr &&m_mttRebuyTipsBySercer->isVisible()&&!g_globaMttGameInfo.m_isGameStart)
	{
		showMttRebuyTipsByServer(false) ;
	}

	if (m_privateRoomInfo !=nullptr && m_privateRoomInfo->isVisible())
	{
		m_privateRoomInfo->setVisible(false) ;
	}


	for (int i = 0; i < 9; i++)
	{
		if (m_pUsers[i]!=NULL)
		{
			m_pUsers[i]->ShowWaitNextGame();
		}
	}
	this->unscheduleAllSelectors();
	schedule(schedule_selector(GameTable::onScheduleUpdateTime));

	this->stopAllActions();
}

void GameTable::CleanPlayer()
{
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (m_pUsers[i])
		{
			UsrStandUp(i, false);
		}
	}
}

void GameTable::SendTip(const std::string& strTip)
{
	GetTableTip()->SystemTip(strTip);
}



void GameTable::SetLookingOn(bool bLookingOn)
{
	if (bLookingOn)
	{
		
	}
}

void GameTable::SetDPosition(unsigned uChairID)
{

	if (m_pSpriteDFlag == nullptr)
	{
		m_pSpriteDFlag = Sprite::create("dflag.png");
		m_pSpriteDFlag->setPosition(s_ptDPosition[uChairID].x, s_ptDPosition[uChairID].y);
		s_gameLayer->addChild(m_pSpriteDFlag, DFlagZOrder);
	}
	else
	{
		float fTimeMoveD = g_cfgReader.GetDMove();//0.72f; // adjust rhythm
		m_pSpriteDFlag->runAction(MoveTo::create(fTimeMoveD, s_ptDPosition[uChairID]));
	}

	m_pSpriteDFlag->setVisible(true);
}

void GameTable::SetSidePot(const std::vector<double>& vecCurAllPot)
{
	m_vecSidePotValue = vecCurAllPot;
	m_uCurSidePotCount = vecCurAllPot.size();
	for (auto item : m_vecPSidePot)
	{
		if (nullptr != item)
			item->removeFromParentAndCleanup(true);
	}
	m_vecPSidePot.clear();
	for (int i = 0; i < m_uCurSidePotCount; i++)
	{
		auto pSidePot = SidePot::create(std::bind(&GameTable::GetJettonIdxs, this, std::placeholders::_1, std::placeholders::_2));
		pSidePot->setCascadeOpacityEnabled(true);
		pSidePot->setPosition(GetSidePotPosition(m_uCurSidePotCount, i));
		pSidePot->UpdatePotValue(m_vecSidePotValue[i]);
		s_gameLayer->addChild(pSidePot, SidePotZOrder);
		m_vecPSidePot.push_back(pSidePot);
	}
}

void GameTable::SetAllPot(double dAllPot)
{
	m_dAllPlacedJettonValue = dAllPot;
	//SetLabelString(m_pLabelAllPlacedJettonValue, FormatCash(m_dAllPlacedJettonValue));
	setUITextString(m_pLabelAllPlacedJettonValue, "底池："+FormatCash(m_dAllPlacedJettonValue)) ;
}

void GameTable::SetUser(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID, unsigned uVoice,unsigned uSkin, unsigned m_uLevel, int uBiaoJiIdx, unsigned uLadyId, unsigned uGender, const std::string& strVideoStreamUrl)
{
	UsrSitDown(uChairID, strNickName, strIcon, dCash, uUID, uVoice,uSkin, m_uLevel,uBiaoJiIdx, uLadyId, uGender, strVideoStreamUrl, false);
}
void GameTable::SetUserShowName(unsigned uChairID, int playerStatus)
{
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		if (getTableType() == SngTable|| getTableType() == MttTable)
		{
			pUserView->ShowWaitNextGame() ;
		}
		else
		{
			if (playerStatus == 0)
			{
				pUserView->ActivateStatus(US_FOLD);
			}
			else
			{
				pUserView->ShowNickName();
			}
		}


		
	}
}

void GameTable::SetUsrBetedJetton(unsigned uChairID, double dBetCash)
{
	m_dUsrPotValue[uChairID] = dBetCash;

	auto& pSpriteBg = m_pSpriteBgUsrPot[uChairID];
	if (pSpriteBg == nullptr)
	{
		pSpriteBg = Sprite::create("bgusrbet.png");
		pSpriteBg->setPosition(s_ptBgBet[uChairID]);
		s_gameLayer->addChild(pSpriteBg, UserPotZOrder);
	}
	pSpriteBg->setVisible(true);
		
	auto& pLabelBet = m_pLabelUsrPotValue[uChairID];
	if (pLabelBet == nullptr)
	{
		pLabelBet = CreateLabelMSYH(20);
		pLabelBet->setPosition(s_ptLabelBet[uChairID].x, s_ptLabelBet[uChairID].y + 1);
		pLabelBet->setTextColor(Color4B(0xff, 0xd2, 0, 255));
		s_gameLayer->addChild(pLabelBet, UserPotZOrder);
	}
	SetLabelString(pLabelBet, FormatCash(m_dUsrPotValue[uChairID]));
	pLabelBet->setVisible(true);

	auto& pSpriteJettonPot = m_pSpriteJetton2SidePot[uChairID];
	if (pSpriteJettonPot == nullptr)
	{
		pSpriteJettonPot = m_pJettonFactory->CreateJetton(GetJettonIdx(m_dUsrPotValue[uChairID]));
		pSpriteJettonPot->setVisible(false);
		s_gameLayer->addChild(pSpriteJettonPot, BetJettonZOrder);
	}
	ResetJetton(pSpriteJettonPot, GetJettonIdx(m_dUsrPotValue[uChairID]));
	pSpriteJettonPot->setPosition(s_ptJettonDst[uChairID]);
	pSpriteJettonPot->setOpacity(255);
	pSpriteJettonPot->setVisible(true);
}

void GameTable::SetUsrCash(unsigned uChairID, double dCash)
{
	auto& pUserView = m_pUsers[uChairID];
	if (pUserView)
	{
		pUserView->SetCash(dCash);
	}
	m_usrInfo[uChairID].m_dCash = dCash;

}

void GameTable::SetPublicCard(std::vector<unsigned char> vecPublicCard)
{
	m_bAllPublicCardDispatched = true;
	m_vecPublicCardData = vecPublicCard;
	for (int i = 0; i < m_vecPublicCardData.size(); i++)
	{
		GetSpritePublicCard(i)->SetFrameIdx(GetCardFrameIdx(vecPublicCard[i]));
		GetSpritePublicCard(i)->setVisible(true);
	}
}

void GameTable::UsrSitDown(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID, unsigned uVoiceid, unsigned uSkinID, unsigned uLevel, int uBiaoJiIdx, unsigned uLadyId, unsigned uGender, const std::string& strVideoStreamUrl, bool bAni, bool newPlayer)
{
	auto& pUserView = m_pUsers[uChairID];

	if (pUserView == nullptr)
	{
		pUserView = UserView::creatWithLayer(m_usePosInfo[uChairID]);
		pUserView -> ignoreAnchorPointForPosition(false);
		pUserView -> setAnchorPoint(Point(0.5,0.5));
	

		if (bAni)
		{
			s_gameLayer->addChild(pUserView, UserViewZOrder);
			pUserView->runAction(Sequence::create(
				Show::create(),
				Place::create(s_ptUserViewOutTable[uChairID]),
				MoveTo::create(g_cfgReader.GetPlayerComeIn(), s_ptUserViewDefault[uChairID]),
				nullptr));
		}
		else
		{			
			pUserView->setPosition(s_ptUserViewDefault[uChairID]);
			s_gameLayer->addChild(pUserView, UserViewZOrder);
			pUserView->setVisible(true);
		}

		auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(pUserView, true/*, hitProcer*/);
		m_pUsersViewTouchGrabber[uChairID] = touchGrabber;
		touchGrabber->GetTouchProcer().SetHitCB(nullptr, [this, uChairID](bool bSelect, Node*, TouchGrabber*){PopUserInfo(uChairID);});
		touchGrabber->retain();
	}
	else
	{
		pUserView->stopAllActions();
		if (bAni)
		{
			pUserView->runAction(Sequence::create(
				Show::create(),
				MoveTo::create(g_cfgReader.GetPlayerComeIn(), s_ptUserViewDefault[uChairID]),
				nullptr));
		}
		else
		{
			pUserView->setVisible(true);
			pUserView->setPosition(s_ptUserViewDefault[uChairID]);
		}
	}
	
	




	pUserView->SetIconUrl(strIcon);
	pUserView->SetCash(dCash);
	pUserView->SetNickName(strNickName);
	pUserView->ShowWaitNextGame();
	pUserView->SetBiaoJi(uBiaoJiIdx);
    pUserView->SetLady(uLadyId);
	m_usrInfo[uChairID].m_strName = strNickName;
	m_usrInfo[uChairID].m_strIcon = strIcon;
	m_usrInfo[uChairID].m_dCash = dCash;
	m_usrInfo[uChairID].m_uUID = uUID;
	m_usrInfo[uChairID].m_uSkin = uVoiceid;
	m_usrInfo[uChairID].m_uGender = uGender;
	m_usrInfo[uChairID].m_uShowGirlLvl = uLadyId;
	m_usrInfo[uChairID].m_strVideoStreamUrl = strVideoStreamUrl;

	m_bUserValid[uChairID] = true;
	if(uChairID >= 0 && uChairID <= 4)
		ifVipSign = true;
	if(uChairID > 4 && uChairID <= 8)
		ifVipSign = false;
	pUserView->SetVipSign(ifVipSign, uLevel);

	if (uUID != g_globalMyData.m_iUserId) {
		GetChatBox()->AppandUser(strIcon, strNickName, uUID);
	}
	//PlayEffectSound2("sound/act/sit.mp3");

	if (uChairID == g_globalTableConfig.GetMyViewChairID() && g_globalTableConfig.GetTableMode() == TablePlaying)
	{
		m_pMenuItemSitDown->setVisible(false);
		m_pMenuItemAutoSitDown->setVisible(false);
		m_pAniWaitSitDown->setVisible(false);
		m_pLabelWaitSitDown->setVisible(false);
	}

	callShowJoinTableEffect(uChairID,uLevel,uVoiceid,newPlayer);

	if (getTableType() == NormalTable)
	{

		// 显示加载视频
		if (g_globalTableConfig.IsTableHasVideo())
		{
			m_videoPlayMgr.PrePlay(uChairID, strVideoStreamUrl);
			if (g_globalTableConfig.GetTableMode() == TablePlaying)
			{
				if (uLadyId != 0 && uChairID != g_globalTableConfig.GetMyViewChairID())
				{
					m_videoPlayMgr.StartPlay(uChairID, strVideoStreamUrl);
				}
			}
			else
			{
				if (uLadyId != 0)
				{
					m_videoPlayMgr.StartPlay(uChairID, strVideoStreamUrl);
				}
			}
		}
	}





}

void GameTable::UsrStandUp(unsigned uChairID, bool bAni)
{
 	if (g_globalTableConfig.IsTableHasVideo())
 	{
 		m_videoPlayMgr.RemovePlay(uChairID);
 	}


	auto pUserView = m_pUsers[uChairID];



	if (pUserView)
	{
		pUserView->stopAllActions();
		pUserView->ShowWaitNextGame();
		//pUserView->reset() ;


		if (getTableType() == NormalTable)
		{
			if (bAni)
			{
				pUserView->runAction(Sequence::create(
					MoveTo::create(g_cfgReader.GetPlayerLeave(), s_ptUserViewOutTable[uChairID]),
					Hide::create(),
					nullptr));
			}
			else
			{
				pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
				pUserView->setVisible(false);
			}
		}
		if (getTableType() == SngTable )
		{
			if (!g_globalSngTableInfo.m_isGameStart)
			{
				if (bAni)
				{
					pUserView->runAction(Sequence::create(
						MoveTo::create(g_cfgReader.GetPlayerLeave(), s_ptUserViewOutTable[uChairID]),
						Hide::create(),
						nullptr));
				}
				else
				{
					pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
					pUserView->setVisible(false);
				}
			}
			//离开桌子后 自动弃牌
			if (uChairID == MY_CHAIR_ID)
			{
				showGameAutoTrust(false) ;
			}

		}

		if (getTableType() == MttTable )
		{

		
				if (bAni)
				{
					pUserView->runAction(Sequence::create(
						MoveTo::create(g_cfgReader.GetPlayerLeave(), s_ptUserViewOutTable[uChairID]),
						Hide::create(),
						nullptr));
				}
				else
				{
					pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
					pUserView->setVisible(false);
				}
			//if (!g_globaMttGameInfo.m_isGameStart)
			//{
			//	if (bAni)
			//	{
			//		pUserView->runAction(Sequence::create(
			//			MoveTo::create(g_cfgReader.GetPlayerLeave(), s_ptUserViewOutTable[uChairID]),
			//			Hide::create(),
			//			nullptr));
			//	}
			//	else
			//	{
			//		pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
			//		pUserView->setVisible(false);
			//	}
			//}
			////离开桌子后 自动弃牌
			//if (uChairID == MY_CHAIR_ID)
			//{
			//	showSngAuto(false) ;
			//}

		}
		if (getTableType() == PrivateTable)
		{
			if (bAni)
			{
				pUserView->runAction(Sequence::create(
					MoveTo::create(g_cfgReader.GetPlayerLeave(), s_ptUserViewOutTable[uChairID]),
					Hide::create(),
					nullptr));
			}
			else
			{
				pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
				pUserView->setVisible(false);
			}
		}
	}

	GetChatBox()->RemoveUser(m_usrInfo[uChairID].m_uUID);
	m_bUserValid[uChairID] = false;

	if (g_globalTableConfig.GetTableMode() == TableLooking || uChairID != MY_CHAIR_ID)
	{
		if (m_pSpriteDispatchingCard[uChairID*2])
		{		
			m_pSpriteDispatchingCard[uChairID*2]->setVisible(false);
		}
		if (m_pSpriteDispatchingCard[uChairID*2+1])
		{		
			m_pSpriteDispatchingCard[uChairID*2+1]->setVisible(false);
		}

		if (nullptr != m_pWinNumber[uChairID]) {
			m_pWinNumber[uChairID]->setVisible(false);
		}
	}
	else
	{
		if (m_pSpriteMyCard[0])
		{
			m_pSpriteMyCard[0]->setVisible(false);
			m_pSpriteMyCard[1]->setVisible(false);
		}
		if (m_pSpriteMaxCardType)
		{
			m_pSpriteMaxCardType->SetFrameIdx(0);
		}		
		if (m_pActionBar)
		{
			m_pActionBar->setVisible(false);
		}
		if (m_pPreActionBar)
		{
			m_pPreActionBar->setVisible(false);
		}
		if(m_pBrightActionBar)
		{
			m_pBrightActionBar->setVisible(false);
			m_pMenuItemBrightCard->setVisible(false);
		}
	}

	if (uChairID == g_globalTableConfig.GetMyViewChairID() && g_globalTableConfig.GetTableMode() == TablePlaying)
	{
        g_globalTableConfig.SetTableMode(TableLooking);
		checkSitdown();

	}
}

void GameTable::DispatchHandCard(const std::vector<unsigned>& vecDispatchingChairID, unsigned char cbMyCardData[2])
{
	if (IsJettonCollecting())
	{
		SetJettonCollectFinishedCB(std::bind(&GameTable::DispatchHandCard, this, vecDispatchingChairID, cbMyCardData));
		return;
	}
	m_bDispatchingHandCard = true;
	m_vecDispatchingChairID = vecDispatchingChairID;
	m_vecDispatchingChairID.insert(m_vecDispatchingChairID.end(), vecDispatchingChairID.begin(), vecDispatchingChairID.end());
	m_cbMyCardData[0] = cbMyCardData[0];
	m_cbMyCardData[1] = cbMyCardData[1];
	schedule(schedule_selector(GameTable::OnScheduleDispatchHandCard),  g_cfgReader.GetDispatchHandCardInterval(), m_vecDispatchingChairID.size(), 0.0f);
	
	for (auto itm : vecDispatchingChairID)
	{
		auto user = GetUserView(itm);
		if(user != nullptr){
			user->DryCashMinHandCard(2);
			user->ShowNickName();
		}
	}
}

void GameTable::DispatchHandCard2(const std::vector<unsigned>& vecDispatchingChairID, unsigned char cbMyCardData[2])
{
	if (IsJettonCollecting())
	{
		SetJettonCollectFinishedCB(std::bind(&GameTable::DispatchHandCard2, this, vecDispatchingChairID, cbMyCardData));
		return;
	}
	m_vecDispatchingChairID = vecDispatchingChairID;
	m_cbMyCardData[0] = cbMyCardData[0];
	m_cbMyCardData[1] = cbMyCardData[1];

	static float s_fFlyTimes[2] = {0.28f, 0.4f}; 
	static char *s_aniShowMyCard[2] = {"showmycard1.png", "showmycard2.png"};
	static unsigned s_uShowMyCardX[2] = {5, 5};
	static unsigned s_uShowMyCardY[2] = {2, 2};
	static unsigned s_uShowMyCardAll[2] = {10, 10};
	for (int uRoundIdx = 0; uRoundIdx < 2; uRoundIdx++)
	{
		float fFlyTime = s_fFlyTimes[uRoundIdx];
		for (auto i = 0; i < vecDispatchingChairID.size(); i++)
		{
			unsigned uCurDispatchChairID = vecDispatchingChairID[i];
			Sprite *pSpriteDispatchingCard = GetSpriteDispatchingCard(uRoundIdx, uCurDispatchChairID);
			if ((g_globalTableConfig.GetTableMode() != TableLooking) && uCurDispatchChairID == MY_CHAIR_ID)
			{
				auto pActionMove = Spawn::create(FadeIn::create(fFlyTime), MoveTo::create(fFlyTime, s_ptMyCard[uRoundIdx]), nullptr);
				auto aniShowMyHandCard = CreateAnimate(s_aniShowMyCard[uRoundIdx], s_uShowMyCardX[uRoundIdx], s_uShowMyCardY[uRoundIdx], s_uShowMyCardAll[uRoundIdx], 1 / 25.0f);//AnimateViewer::create(s_aniShowMyCard[uRoundIdx], true, 1 / 24.0f, 6, 4, 0, 1);
				auto funShowMyCard = [this, uRoundIdx](){auto pCard = GetSpriteMyCard(uRoundIdx); pCard->SetFrameIdx(GetCardFrameIdx(m_cbMyCardData[uRoundIdx])); pCard->setVisible(true);
				m_bDispatchingHandCard = false;};
				pSpriteDispatchingCard->runAction(Sequence::create(Show::create(), pActionMove, aniShowMyHandCard, Hide::create(), CallFunc::create(funShowMyCard), nullptr));
			}
			else
			{
				auto pActionMove = Spawn::create(FadeIn::create(fFlyTime), MoveTo::create(fFlyTime, s_ptDispatchedCard[uRoundIdx * GAME_PLAYER + uCurDispatchChairID]), nullptr);
				pSpriteDispatchingCard->runAction(Sequence::create(Show::create(), pActionMove, nullptr));
			}
		}
		
		const char *szEffect[2] = {"sound/table/handcard0.mp3", "sound/table/handcard1.mp3"};
		PlayEffectSound2(szEffect[uRoundIdx]);
	}

	for (auto itm : vecDispatchingChairID)
	{
		auto user = GetUserView(itm);
		if(user != nullptr){
			user->ShowNickName();
			user->DryCashMinHandCard(2);
		}
	}
}

void GameTable::DispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard)
{
	ActivateUserStatus(US_NORMAL, US_ALLIN | US_FOLD | US_WAITNEXTGAME);

	if (IsJettonCollecting())
	{
		if (PreDispatchPublicCard(vecDispatchingPublicCard))
		{			
			SetJettonCollectFinishedCB(std::bind(&GameTable::StartDispatchPublicCard, this));
		}
	}
	else
	{
		DispatchPublicCardImpl(vecDispatchingPublicCard);
	}
}
bool GameTable::PreDispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard)
{
	unsigned uDispatchingCardCount = vecDispatchingPublicCard.size();
	unsigned uDispatchedCardCount = m_vecPublicCardData.size();

	bool bOk = false;
	if (uDispatchingCardCount == 5)
	{		
		bOk = true;		
	}
	else if (uDispatchingCardCount == 3)
	{
		bOk = true;	
	}
	else if (uDispatchingCardCount == 2)
	{
		bOk = true;	
	}
	else if (uDispatchingCardCount == 1)
	{
		bOk = true;	
	}
	if (bOk)
	{
		m_deqDispatchingPublicCardData.clear();
		for (unsigned i = 0; i < uDispatchingCardCount; i++)
		{
			 m_deqDispatchingPublicCardData.push_back(vecDispatchingPublicCard[i]);		
		}
	}
	return bOk;
}
void GameTable::StartDispatchPublicCard()
{
	if (!m_deqDispatchingPublicCardData.empty())
	{
		DishpatchPublicCard(m_vecPublicCardData.size(), m_deqDispatchingPublicCardData.front(), false);
	}	
}
void GameTable::DispatchPublicCardImpl(const std::vector<unsigned char>& vecDispatchingPublicCard)
{
	if (PreDispatchPublicCard(vecDispatchingPublicCard))
	{
		StartDispatchPublicCard();
	}
}

bool GameTable::IsDispatchingPublicCard()
{
	return !m_deqDispatchingPublicCardData.empty();
}

int GameTable::GetCurThinkingUsr()
{
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		auto userView = GetUserView(i);
		if (userView)
		{
			auto flag = userView->GetActivateFlag();
			if (flag == US_THINK)
			{
				return i;
			}
		}
	}
	return -1;
}

void GameTable::WaitUsrAction(unsigned uChairID)
{
	// CAODANDE 给"协议"打补丁：玩家站起时514弃牌里面的nextchairid会导致同时对2个玩家WaitUsrAction
	// amd管微软要DirectX部分问题处理方案，微软说我先看看intel怎么实现的？
    if (m_uPreThinkChairID == uChairID )//最后一个弃牌的玩家下找不到下家，导致导致赢牌时，此玩家还在读秒
    {
        return;
    }

	auto userView = GetUserView(uChairID);

	if(userView == nullptr)
	{
		CCLOG("***********userview is null**********%d",uChairID) ;
		return ;
	}

	auto flag = userView->GetActivateFlag();
	if (flag ==US_FOLD ||flag ==US_ALLIN)
	{
		return ;
	}
		


    m_uPreThinkChairID = uChairID;

	bool bHasPlayerThinking = false;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		auto userView = GetUserView(i);
		if (userView)
		{
			auto flag = userView->GetActivateFlag();
			if (flag == US_THINK)
			{
				bHasPlayerThinking = true;
				break;
			}
		}
	}






	if (bHasPlayerThinking)
	{
		return;
	}
	if (IsDispatchingPublicCard())
	{
		SetPublicCardDispatchedCB([this,uChairID](unsigned){WaitUsrAction(uChairID);});
		return ;
	}
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		pUserView->ShowThinking();

	}
}

void GameTable::onScheduleShakeHandCard(float fDelta)
{
	auto pCard0 = GetSpriteMyCard(0);
	auto pCard1 = GetSpriteMyCard(1);

	auto funcShakeCard0 = [pCard0,this](){
		float x = s_ptMyCard[0].x;
		float y = s_ptMyCard[0].y;
		int iScope = 3;
		x -= iScope;
		y -= iScope;
		pCard0->setPosition(Vec2(x + rand()%iScope, y + rand()%iScope));
	};
	pCard0->runAction(Repeat::create(Sequence::create(CallFunc::create(funcShakeCard0), DelayTime::create(0.08f), nullptr), 300));
	auto funcShakeCard1 = [pCard1,this](){
		float x = s_ptMyCard[1].x;
		float y = s_ptMyCard[1].y;
		int iScope = 3;
		x -= iScope;
		y -= iScope;
		pCard1->setPosition(Vec2(x + rand()%iScope, y + rand()%iScope));
		m_bDispatchingHandCard = false;
	};
	pCard1->runAction(Repeat::create(Sequence::create(CallFunc::create(funcShakeCard1), DelayTime::create(0.08f), nullptr), 300));
	PlayEffectSound2("sound/table/dangdang.mp3");
	vibrate(2000);
}

void GameTable::onScheduleFoldCard(float fDelta)
{





	if (getTableType() == NormalTable || getTableType() == PrivateTable)
	{
		if (m_pActionBar->CanCheckNow())
		{
			Json::Value msg;
			msg["betJettonNum"] = 0;
			msg["betType"] = 4;
			msg["mangTag"] = 0;
			SendClientRequest(ClientRequest::kAddScore, msg);
		}
		else
		{
			Json::Value param;
			param["strTip"] = 1;
			SendClientRequest(ClientRequest::kGiveUp, param);
		}
		++m_uOverTime;


		if (m_uOverTime == 3)
		{
			m_uOverTime = 0;
			Json::Value param;
			param["needconfirm"] = false;
			SendClientRequest(ClientRequest::kStandUp);
			PopTipWithBt1("您连续三次下注超时，是不是太累呢？先旁观休息下吧。", "确定");
		}
	}

	if (getTableType() == SngTable||getTableType() == MttTable)
	{
		/************************************************************************/
		/* 第一次自动弃牌
		第二次 自动弃牌并出现托管
		下一局脱托管消失
		第三次 永久自动托管 并显示后面的画面
		*/
		/************************************************************************/
	
		//if (m_pActionBar->CanCheckNow())
		//{
		//	Json::Value msg;
		//	msg["betJettonNum"] = 0;
		//	msg["betType"] = 4;
		//	msg["mangTag"] = 0;
		//	SendClientRequest(ClientRequest::kAddScore, msg);
		//}
		//else
		//{
		//	Json::Value param;
		//	param["strTip"] = 1;
		//	SendClientRequest(ClientRequest::kGiveUp, param);
		//}

		++m_uOverTime;

		//两次自动下注后托管
		if (m_uOverTime == 2)
		{
			isSngAutoMove = true ;
			//showGameAutoTrust(true);
		}
		if (m_uOverTime == 3)
		{
			isSngAutoMoveFoever = true ;
			//showGameAutoTrust(true);
		}

	}


}




void GameTable::SetSelfActionInfo(unsigned uBetType ,double dBalance, double dMinRaise, double dMaxRaise, bool bHasDispatchPublicCard)
{
	if (IsDispatchingPublicCard())
	{
		SetPublicCardDispatchedCB([this,uBetType,dBalance, dMinRaise, dMaxRaise, bHasDispatchPublicCard](unsigned){SetSelfActionInfo(uBetType,dBalance, dMinRaise, dMaxRaise, bHasDispatchPublicCard);});
		return ;
	}
	// 如果处于旁观状态，全部隐藏
	if (g_globalTableConfig.GetTableMode() == TableLooking)
	{
		if (m_pActionBar)
		{
			m_pActionBar->setVisible(false);
		}
		if (m_pPreActionBar)
		{
			m_pPreActionBar->setVisible(false);
		}
		m_pMenuItemPopChat->setVisible(true);
#if OpenSoundRecorder
		m_pMenuItemRecordSound->setVisible(true);
#else
		m_pMenuItemRecordSound->setVisible(false);
#endif 
        WaitUsrAction(g_globalTableConfig.GetMyViewChairID());//旁观时本家位置读秒
		return;
	}
	if (!g_globalGameSerialization.IsInGameNow())
	{
		if (m_pActionBar)
		{
			m_pActionBar->setVisible(false);
		}
		if (m_pPreActionBar)
		{
			m_pPreActionBar->setVisible(false);
		}
		m_pMenuItemPopChat->setVisible(true);
#if OpenSoundRecorder
		m_pMenuItemRecordSound->setVisible(true);
#else
		m_pMenuItemRecordSound->setVisible(false);
#endif 
		return;
	}
	WaitUsrAction(g_globalTableConfig.GetMyViewChairID());
	if (m_pPreActionBar)
	{
		int iPreAct = m_pPreActionBar->GetCurSelectedAction();
		bool bPreAct = false;
		if (iPreAct >= 0)
		{
			// TODO:
			if (iPreAct == 0) // 让或弃
			{
				if (dBalance - m_dUsrPotValue[MY_CHAIR_ID] > 1)
				{
					// 弃牌
					SendClientRequest(ClientRequest::kGiveUp);
					bPreAct = true;
				}
				else
				{
					// 让牌
					Json::Value msg;
					msg["betJettonNum"] = 0;
					msg["betType"] = 4;
					msg["mangTag"] = 0;
					SendClientRequest(ClientRequest::kAddScore, msg);
					bPreAct = true;
				}
			}
			else if (iPreAct == 1) // 自动让牌 跟xxx
			{
				if (dBalance - m_dUsrPotValue[MY_CHAIR_ID] > 1)
				{
					// 跟注
					if (m_usrInfo[MY_CHAIR_ID].m_dCash > (dBalance - m_dUsrPotValue[MY_CHAIR_ID]))
					{
						Json::Value msg;
						msg["betJettonNum"] = dBalance - m_dUsrPotValue[MY_CHAIR_ID];
						msg["betType"] = 1;
						msg["mangTag"] = 0;
						SendClientRequest(ClientRequest::kAddScore, msg);
						bPreAct = true;
					}
				}
				else
				{
					// 让牌
					Json::Value msg;
					msg["betJettonNum"] = 0;
					msg["betType"] = 4;
					msg["mangTag"] = 0;
					SendClientRequest(ClientRequest::kAddScore, msg);
					bPreAct = true;
				}
			}
			else if (iPreAct == 2) // 跟任何注
			{
				// 跟注
				if (m_usrInfo[MY_CHAIR_ID].m_dCash > (dBalance - m_dUsrPotValue[MY_CHAIR_ID]))
				{
					if (dBalance - m_dUsrPotValue[MY_CHAIR_ID] > 1)
					{
						// 跟注
						if (m_usrInfo[MY_CHAIR_ID].m_dCash > (dBalance - m_dUsrPotValue[MY_CHAIR_ID]))
						{
							Json::Value msg;
							msg["betJettonNum"] = dBalance - m_dUsrPotValue[MY_CHAIR_ID];
							msg["betType"] = 1;
							msg["mangTag"] = 0;
							SendClientRequest(ClientRequest::kAddScore, msg);
							bPreAct = true;
						}
					}
					else
					{
						// 让牌
						Json::Value msg;
						msg["betJettonNum"] = 0;
						msg["betType"] = 4;
						msg["mangTag"] = 0;
						SendClientRequest(ClientRequest::kAddScore, msg);
						bPreAct = true;
					} 
				}
				else // Allin
				{
					Json::Value msg;
					msg["betJettonNum"] = m_usrInfo[MY_CHAIR_ID].m_dCash;
					msg["betType"] = 3;
					msg["mangTag"] = 0;
					SendClientRequest(ClientRequest::kAddScore, msg);
					bPreAct = true;
				}
			}
			if (bPreAct)
			{
				m_pPreActionBar->setVisible(false);
				return;
			}
		}
	}

	if (!m_autoTrust)
	{
		scheduleOnce(schedule_selector(GameTable::onScheduleShakeHandCard), 6.0f);
		scheduleOnce(schedule_selector(GameTable::onScheduleFoldCard), 13.0f);
	}
	


	float s_curx ;
	float s_cury ;
	m_pActionBar->stopAllActions();
	m_pActionBar->SetBlind(g_globalTableConfig.m_dLittleBlind, g_globalTableConfig.m_dBigBlind);

	bool needRaise = true ;

	
	m_pActionBar->Update(dBalance - m_dUsrPotValue[MY_CHAIR_ID], needRaise, dMinRaise, dMaxRaise, 
		m_usrInfo[MY_CHAIR_ID].m_dCash, m_dAllPlacedJettonValue, bHasDispatchPublicCard);
	
	s_curx = m_pActionBar->getPositionX();
	s_cury = m_pActionBar->getPositionY();
	m_pActionBar->setPosition(s_curx, s_cury-5000);
	m_pActionBar->setOpacity(0);
	//m_pActionBar->setVisible(true);
	auto actMov = Spawn::create(EaseOut::create(MoveTo::create(0.2f, Vec2(s_curx, s_cury)), 3.5), FadeIn::create(0.2f), nullptr);
	//m_pActionBar->runAction(Sequence::create(actMov, nullptr));
	m_pActionBar->doActionMoveIn();
	if (m_pPreActionBar)
	{
		m_pPreActionBar->setVisible(false);
	}

	m_pMenuItemPopChat->setVisible(false);


	m_pMenuItemRecordSound->setVisible(false);

	PlayEffectSound2("sound/table/myturn.mp3");



	if (getTableType() == SngTable || getTableType() == MttTable)
	{
		if (isSngAutoMove || isSngAutoMoveFoever)
		{
			//GameTable::onScheduleSngFoldCard(0) ;
		}
			
	}


}

void GameTable::UpdateSelfMinCall(double dBalance)
{
	// 如果处于旁观状态，全部隐藏
	if (g_globalTableConfig.GetTableMode() == TableLooking)
	{
		if (m_pActionBar)
		{
			m_pActionBar->setVisible(false);
		}
		if (m_pPreActionBar)
		{
			m_pPreActionBar->setVisible(false);
		}
		m_pMenuItemPopChat->setVisible(true);
#if OpenSoundRecorder
		m_pMenuItemRecordSound->setVisible(true);
#else
		m_pMenuItemRecordSound->setVisible(false);
#endif 
		return;
	}
	if (!g_globalGameSerialization.IsInGameNow())
	{
		if (m_pActionBar)
		{
			m_pActionBar->setVisible(false);
		}
		if (m_pPreActionBar)
		{
			m_pPreActionBar->setVisible(false);
		}
		m_pMenuItemPopChat->setVisible(true);
#if OpenSoundRecorder
		m_pMenuItemRecordSound->setVisible(true);
#else
		m_pMenuItemRecordSound->setVisible(false);
#endif 
		return;
	}
    //if (GetUserView(MY_CHAIR_ID) == nullptr)
    //    return ;
	if (GetUserView(MY_CHAIR_ID)->GetActivateFlag() == US_ALLIN || GetUserView(MY_CHAIR_ID)->GetActivateFlag() == US_FOLD ||GetUserView(MY_CHAIR_ID)->GetActivateFlag() == US_WAITNEXTGAME)
	{
		if (m_pActionBar)
		{
			m_pActionBar->setVisible(false);
		}
		if (m_pPreActionBar)
		{
			m_pPreActionBar->setVisible(false);
		}
		m_pMenuItemPopChat->setVisible(true);
#if OpenSoundRecorder
		m_pMenuItemRecordSound->setVisible(true);
#else
		m_pMenuItemRecordSound->setVisible(false);
#endif 
		return;
	}
	//if (nullptr == m_pPreActionBar)
	//{
	//	//m_pPreActionBar = PreActionBar::create();
	//	m_pPreActionBar = PreActionBar::creatWithLayer(s_uiRoot);
	//	m_pPreActionBar->setVisible(false);
	//	addChild(m_pPreActionBar, ActionBarZOrder);
	//}
	if (!m_pPreActionBar->isVisible())
	{
		m_pPreActionBar->stopAllActions();
		m_pPreActionBar->setPosition(0, -110);
		m_pPreActionBar->setVisible(true);
		m_pPreActionBar->setOpacity(0);
		float fActMoveT = g_cfgReader.GetActionBarUp();
		auto actMov = Spawn::create(EaseOut::create(MoveBy::create(fActMoveT, Vec2(0, 110)), 3.5), FadeIn::create(fActMoveT), nullptr);
		m_pPreActionBar->runAction(actMov);
	}
    if (m_usrInfo[MY_CHAIR_ID].m_dCash < dBalance - m_dUsrPotValue[MY_CHAIR_ID])
    {
        m_pPreActionBar->Update(m_usrInfo[MY_CHAIR_ID].m_dCash);
    }
    else
    {
        m_pPreActionBar->Update(dBalance - m_dUsrPotValue[MY_CHAIR_ID]);
    }
	if (m_pActionBar)
	{
		m_pActionBar->setVisible(false);
	}

	m_pMenuItemPopChat->setVisible(true);
#if OpenSoundRecorder
	m_pMenuItemRecordSound->setVisible(true);
#else
	m_pMenuItemRecordSound->setVisible(false);
#endif 
}

void GameTable::UsrCheck(unsigned uChairID)
{
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		pUserView->ShowAction(UserAction::Check);
	}
	// PlayEffectSound2("sound/act/check.mp3");
	PlayUsrActSound(uChairID, 0);
}

void GameTable::UsrCall(unsigned uChairID, double dBetValue)
{
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		pUserView->ShowAction(UserAction::Call);
	}
	UsrPlaceJetton(uChairID, dBetValue);
	PlayUsrActSound(uChairID, 1);
}

void GameTable::UsrRaise(unsigned uChairID, double dBetValue)
{
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		pUserView->ShowAction(UserAction::Raise);
	}
	UsrPlaceJetton(uChairID, dBetValue);
	PlayUsrActSound(uChairID, 2);
}

void GameTable::UsrAllIn(unsigned uChairID, double dBetValue)
{
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		pUserView->ShowAction(UserAction::AllIn);
	}
	UsrPlaceJetton(uChairID, dBetValue);
	if(g_globalTableConfig.m_uSound != 0)
	{
		PlayUsrActSound(uChairID, 3);
	}
}

void GameTable::UsrFoldCard(unsigned uChairID)
{
	auto pUserView = GetUserView(uChairID);
	bool bHasFlod = false;
	if (pUserView)
	{
		bHasFlod = (pUserView->GetActivateFlag() == US_FOLD);
	}
	if (bHasFlod) // 有可能2次弃牌
	{
		return;
	}
	
	if (pUserView)
	{
		pUserView->ShowAction(UserAction::Fold);
	}
	if ((g_globalTableConfig.GetTableMode() == TablePlaying || 
		g_globalTableConfig.GetTableMode() == TablePlayback || 
		g_globalTableConfig.GetTableMode() == TableTeaching) &&
		uChairID == MY_CHAIR_ID)
	{

	}
	else
	{
		if (m_pSpriteDispatchingCard[uChairID*2])
		{
			m_pSpriteDispatchingCard[uChairID*2]->setVisible(false);
		}
		if (m_pSpriteDispatchingCard[uChairID*2+1])
		{
			m_pSpriteDispatchingCard[uChairID*2+1]->setVisible(false);
		}
		for (unsigned i = 0; i < 2; i++)
		{
			auto pFoldingCard = GetSpriteFoldingCard(uChairID, i);
			pFoldingCard->setVisible(true);
			float fTime = g_cfgReader.GetFoldCard();// 1.28f; // adjust rhythm
			auto pAction = Sequence::create(Spawn::create(EaseOut::create(MoveTo::create(fTime, Vec2(s_ptPublicCard[2].x, s_ptPublicCard[2].y)), 4.5f), FadeOut::create(fTime), nullptr), Hide::create(), nullptr);
			pFoldingCard->runAction(pAction);
		}		
	}
	// PlayEffectSound2("sound/act/foldb.mp3");
	PlayUsrActSound(uChairID, 4);
}

void GameTable::PlayUsrActSound(unsigned uChairID, unsigned uAct)
{
	std::string strBase = "sound/skin/";

	unsigned uCounty = 0;
	unsigned uGender = 1;

	unsigned uSkin = m_usrInfo[uChairID].m_uSkin;

   if (uSkin ==0||uSkin ==1)// 中男
	{
		uCounty = 0;
		uGender = 1;
	}
   else if (uSkin ==2)// 中女
   {
	   uCounty = 0;
	   uGender = 0;
   }
	else if (uSkin >= 3 && uSkin <= 5) // 中女
	{
		uCounty = 0;
		uGender = 0;
	}
	else if (uSkin >= 6 && uSkin <= 8)// 中男
	{
		uCounty = 0;
		uGender = 1;
	}
	else if (uSkin >= 9 && uSkin <= 11)// 外女
	{
		uCounty = 1;
		uGender = 0;
	}
	else if (uSkin >= 12 && uSkin < 14)// 外男
	{
		uCounty = 1;
		uGender = 1;
	}else if (uSkin ==14 )// 圣诞女
	{
		uCounty = 2;
		uGender = 0;
	}

	static char *szCounty[] = {"china/", "foreign/","christmas/"};
	strBase += szCounty[uCounty];
	static char *szGender[] = {"woman/", "man/"};
	strBase += szGender[uGender];

	static const char *s_szActSoundName[] = {
		"dongdong.mp3",
		"call.mp3",
		"raise.mp3",
		"allin.mp3",
		"fold.mp3"
	};
	strBase += s_szActSoundName[uAct];
	PlayEffectSound2(strBase);

	//std::string strFullUrl = GAME_RESOURCE_BASE_URL + strSoundDir + s_szActSoundName[uAct];
	//std::string strLocalFullPath = ConvertUrl2LocalFullPath(strFullUrl);
	//// strLocalFullPath += ".mp3";
	//if (IsFileExist(strLocalFullPath))
	//{		
	//	PlayEffectSound2(strLocalFullPath);
	//}
	//else
	//{
	//	auto httpDownloader = HttpDownloader::getInstance();
	//	if (httpDownloader->IsDownloading(strFullUrl))
	//	{
	//	}
	//	else
	//	{
	//		httpDownloader->AddRequest(strFullUrl, strLocalFullPath, nullptr);
	//	}
	//	static const char *s_szDefActSoundName[] = {
	//		"sound/skin/check.mp3",
	//		"sound/skin/call.mp3",
	//		"sound/skin/raise.mp3",
	//		"sound/skin/allin.mp3",
	//		"sound/skin/fold.mp3"
	//	};
	//	PlayEffectSound2(s_szDefActSoundName[uAct]);
	//}
}

void GameTable::ShowMeHasActioned()
{
	unschedule(schedule_selector(GameTable::onScheduleShakeHandCard));
	unschedule(schedule_selector(GameTable::onScheduleFoldCard));

	auto pCard0 = GetSpriteMyCard(0);
	auto pCard1 = GetSpriteMyCard(1);
	if(m_bDispatchingHandCard == true)
	{
		/*pCard0->stopAllActions();
		pCard1->stopAllActions();*/
		pCard0->setPosition(s_ptMyCard[0]);
		pCard1->setPosition(s_ptMyCard[1]);
	}
	else
	{
		pCard0->stopAllActions();
		pCard1->stopAllActions();
		pCard0->setPosition(s_ptMyCard[0]);
		pCard1->setPosition(s_ptMyCard[1]);
	}	
}

void GameTable::ShowMeHasFold(unsigned strTip)
{
	if (m_cbMyCardData[0] != 0 && m_cbMyCardData[1] != 0)
	{

		m_pBrightActionBar->setVisible(true);
		//显示聊天
		m_pMenuItemPopChat->setVisible(true);
	}

	//if(g_globalTableConfig.GetTableMode() != TableLooking && strTip == 2)
	//{
	//	m_pBrightActionBar->setVisible(true);
	//}
	//else if(g_globalTableConfig.GetTableMode() != TableLooking && strTip == 4)
	//{
	//	m_pBrightActionBar->setVisible(false);
	//}
}

void GameTable::ShowMeHasWin()
{

	//显示聊天
	m_pMenuItemPopChat->setVisible(true);
	m_pSpriteShowMeWin->setVisible(true);
	m_pAniShowMeWin->playOnceByIndexAndStopAtLastFrame(0);
	PlayEffectSound2("sound/table/win.mp3");
}

void GameTable::ShowGameHasEnd(bool bOpenedCard)
{
	if (m_cbMyCardData[0] != 0 && m_cbMyCardData[1] != 0) // 手上有发牌，证明从游戏开始就在
	{
		if (g_globalTableConfig.GetTableMode() != TableLooking) // 可能中途站起了，还没有坐下（坐下的时候会清空m_cbMyCardData） #1726
		{
			if(m_pBrightActionBar && m_pBrightActionBar->isVisible()) //中途弃牌的
			{
				if(m_pBrightActionBar->GetCurSelectedAction() == 1)
				{
					m_pBrightActionBar->setVisible(true);
					m_pMenuItemBrightCard->setVisible(false);
					SendClientRequest(ClientRequest::kShowBrightBrand);
				}
				else
				{
					m_pBrightActionBar->setVisible(false);
					m_pMenuItemBrightCard->setVisible(true);
				}
			}
			else  if (!bOpenedCard) // 自己坚持到了最后一把，别人都弃牌了，这时也可以亮牌
			{
				if (m_pBrightActionBar)
				{
					m_pBrightActionBar->setVisible(false);
				}
				m_pMenuItemBrightCard->setVisible(true);
			}
		}
		
	}

	SetPublicCardDispatchedCB(nullptr);
}

void GameTable::onMenuItemAction(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		SendClientRequest(ClientRequest::kShowBrightBrand);
		m_pMenuItemBrightCard->setVisible(false);
	}

}

void GameTable::CollectUsrBetToPot(const std::vector<double>& vecCurAllPot)
{



	SetJettonCollecting(true);
	if (GetUsrBeting() > 0)
	{
		std::vector<double> vecNewAllPot = vecCurAllPot;
		SetAllBetFinishedCB(std::bind(&GameTable::CollectUsrBetToPot, this, vecNewAllPot));
		return;
	}
	// 记录下新的边池情况
	m_vecSidePotValue = vecCurAllPot;
	Vec2 nSide = GetSidePotPosition(m_uCurSidePotCount == 0 ? 1 : m_uCurSidePotCount, m_uCurSidePotCount == 0 ? 0 : (m_uCurSidePotCount - 1));
	Vec2 ptDstSidePot = Vec2(10 + nSide.x, nSide.y + 16);
	// bool bHasCollectedOne = false;
	bool bNeedCollect = false;
	float fTimeCollectMove = g_cfgReader.GetCollectJetton();// 1.0f; // adjust rhythm
	for (unsigned i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_dUsrPotValue[i] >= 1.0f)
		{
			bNeedCollect = true;
			if (m_pSpriteJetton2UsrPot[i])
			{
				m_pSpriteJetton2UsrPot[i]->stopAllActions();
				m_pSpriteJetton2UsrPot[i]->setVisible(false);
			}
			m_pSpriteBgUsrPot[i]->setVisible(false);
			m_pLabelUsrPotValue[i]->setVisible(false);
			auto actMov = Spawn::create(FadeOut::create(fTimeCollectMove), EaseOut::create(MoveTo::create(fTimeCollectMove, ptDstSidePot), 3.0f), nullptr);
			m_pSpriteJetton2SidePot[i]->runAction(Sequence::create(actMov, Hide::create(),  nullptr)); // 讲用户池的筹码收集到中央边池的过程
			m_dUsrPotValue[i] = 0.0f;
		}
	}
	// 用户筹码池移动中当前边池隐藏后，如果需要增加边池就将当前边池移动到新位置，并在该位置生成新的边池滑动到其对应的位置，否则只需将当前边池de下注额刷新
	auto funProcPot = [this]()
	{
		bool bNeedExpandSidePot = m_uCurSidePotCount < m_vecSidePotValue.size();
		if (bNeedExpandSidePot) // 需要增加边池
		{
			unsigned uCurSidePotCount = m_uCurSidePotCount;
			while (uCurSidePotCount < m_vecSidePotValue.size())
			{
				auto pSidePot = SidePot::create(std::bind(&GameTable::GetJettonIdxs, this, std::placeholders::_1, std::placeholders::_2));
				pSidePot->setCascadeOpacityEnabled(true);
				pSidePot->setVisible(false);
				s_gameLayer->addChild(pSidePot, SidePotZOrder);
				m_vecPSidePot.push_back(pSidePot);
				++uCurSidePotCount;
			}
			auto funEndCollecting = [this](){SetJettonCollecting(false);}; // 结束后腰设置标志，以便后面的回调比如发公牌
			float fTimeSidePotMove = 0.3f;
			// 第一次下注，边池还没有产生, 先显示第一个边池，然后移动到其新位置，在该位置生成其子边池，并移动到其对应的位置
			if (m_uCurSidePotCount == 0)
			{
				for (unsigned j = 0; j < uCurSidePotCount; ++j)
				{
					m_vecPSidePot[j]->UpdatePotValue(m_vecSidePotValue[j]);
					if (0 == j)
					{
						auto actMove = MoveTo::create(fTimeSidePotMove, GetSidePotPosition(uCurSidePotCount, 0));
						m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(1, 0)), Show::create(), actMove, nullptr));
					}
					else
					{
						m_vecPSidePot[j]->setOpacity(0);
						auto actMove = Spawn::create(FadeIn::create(fTimeSidePotMove), MoveTo::create(fTimeSidePotMove, GetSidePotPosition(uCurSidePotCount, j)), nullptr);
						if (j == uCurSidePotCount - 1)
						{
							m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(uCurSidePotCount, 0)), Show::create(), actMove, CallFunc::create(funEndCollecting), nullptr));
						}
						else
						{
							m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(uCurSidePotCount, 0)), Show::create(), actMove, nullptr));
						}						
					}
				}
			}
			else
			{
				// 将当前边池及其父边池全部移动到其对应的新位置，并在当前边池的新位置生成子边池，移动到其对应的新位置
				unsigned uLastPotIdx = m_uCurSidePotCount - 1;
				for (unsigned j = 0; j < uCurSidePotCount ; ++j)
				{
					if (m_vecPSidePot.size()>j)
					{
						m_vecPSidePot[j]->UpdatePotValue(m_vecSidePotValue[j]);
						if (j <= uLastPotIdx)
						{
							auto actMove = MoveTo::create(fTimeSidePotMove, GetSidePotPosition(uCurSidePotCount, j));
							m_vecPSidePot[j]->runAction(actMove);
						}
						else
						{
							m_vecPSidePot[j]->setOpacity(0);
							auto actMove = Spawn::create(FadeIn::create(fTimeSidePotMove), MoveTo::create(fTimeSidePotMove, GetSidePotPosition(uCurSidePotCount, j)), nullptr);					
							m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(uCurSidePotCount, uLastPotIdx)), Show::create(), actMove, CallFunc::create(funEndCollecting), nullptr));
						}
					}
				
				}
			}
			m_uCurSidePotCount = uCurSidePotCount;
		}
		else 
		{
			// 仅仅需要更新下当前最后一个池子的下注额
			if(m_uCurSidePotCount > 0 && m_vecSidePotValue.size() >= m_uCurSidePotCount)
			{

	/*			char buff[64] ;
				sprintf(buff,"%d",m_uCurSidePotCount) ;
				CCLog("dsfsdf",buff) ;*/
				if (m_vecPSidePot.size()>=m_uCurSidePotCount)
				{
					m_vecPSidePot[m_uCurSidePotCount-1]->UpdatePotValue(m_vecSidePotValue[m_uCurSidePotCount-1]);
				}
				else
				{
		
					CCLOG("%d,%d",m_uCurSidePotCount,m_vecPSidePot.size()) ;
				}
			}	
			SetJettonCollecting(false);
		}	
	};
	
	if (bNeedCollect)
	{
		runAction(Sequence::create(DelayTime::create(fTimeCollectMove + 0.5f), CallFunc::create(funProcPot), nullptr));
		PlayEffectSound2("sound/table/collectjetton.mp3");
	}
	else
	{
		auto funEndCollecting = [this](){SetJettonCollecting(false);};
		runAction(Sequence::create(DelayTime::create(fTimeCollectMove + 0.5f), CallFunc::create(funEndCollecting), nullptr));
	}
}


void GameTable::ActivateUserStatus(unsigned uS, unsigned uExceptS)
{
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		auto userView = GetUserView(i);
		if (userView)
		{
			if ((userView->GetActivateFlag() & uExceptS) > 0)
			{
			}
			else
			{
				userView->ActivateStatus(uS);
			}
		}
	}
}

void GameTable::HideHandCard()
{
    auto pCard = GetSpriteMyCard(0);
    if (pCard)
    {
        pCard->setVisible(false);
    }
    pCard = GetSpriteMyCard(1);
    if (pCard)
    {
        pCard->setVisible(false);
    }
}

void GameTable::OpenUsrCard(const std::vector<unsigned>& vecOpenCardUsr, const std::vector<unsigned>& vecOpenCardData)
{
    m_UsrHandCard.clear();
	unsigned char cbCardData[2];
	for (int i = 0; i < vecOpenCardUsr.size(); i++)
	{
		auto uChairID = vecOpenCardUsr[i];
		cbCardData[0] = vecOpenCardData[i*2];
		cbCardData[1] = vecOpenCardData[i*2+1];
        UsrHandCard usrHandCard;
        usrHandCard.uChair = uChairID;
        usrHandCard.handCard[0] = cbCardData[0];
        usrHandCard.handCard[1] = cbCardData[1];
        m_UsrHandCard.push_back(usrHandCard);
		auto userView = GetUserView(uChairID);
		if (userView)
		{
			userView->ShowOpenCard(cbCardData);
		}
		if (uChairID == MY_CHAIR_ID && g_globalTableConfig.GetTableMode() != TableLooking)
		{
			auto pCard = GetSpriteMyCard(0);
			if (pCard)
			{
				pCard->setVisible(false);
			}
			pCard = GetSpriteMyCard(1);
			if (pCard)
			{
				pCard->setVisible(false);
			}

			//开牌后  手牌清掉
			for (auto& pSprite : m_pSpriteMyCard)
			{
				if (pSprite)
				{
					pSprite->stopAllActions();
					pSprite->setVisible(false);
				}
			}


		}
		else
		{
			auto pCard = GetHandCard(uChairID, 0);
			if (pCard)
			{
				pCard->setVisible(false);
			}
			pCard = GetHandCard(uChairID, 1);
			if (pCard)
			{
				pCard->setVisible(false);
			}
		}
	}
	m_bOpenCarding = true;
	m_bOpenCardFinished = false;
	scheduleOnce(schedule_selector(GameTable::OnScheduleOpenUsrCardFinished), 1.6f);
}

//void GameTable::SetUsrWinType(unsigned uChairID, unsigned uWinType)
//{
//	auto pUserView = GetUserView(uChairID);
//	pUserView->ShowWinType(uWinType);
//}

void GameTable::ShowUsrWinInfo(unsigned uWinType, const std::vector<unsigned>& vecWinChairs, bool bWaitOpenCard, const std::vector<UsrWinInfo>& vecUsrWinInfo)
{
	if (m_pActionBar)
	{
		m_pActionBar->setVisible(false);
	}
	if (m_pPreActionBar)
	{
		m_pPreActionBar->setVisible(false);
	}

	if (bWaitOpenCard)
	{
		m_uWinType = uWinType;
		m_vecWinChairs = vecWinChairs;
		m_vecUsrWinInfo = vecUsrWinInfo;
		schedule(schedule_selector(GameTable::OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched), 0.1f, 100, 0);
	}
	else
	{
		for (int i = 0; i < vecWinChairs.size(); i++)
		{
			auto pUserView = GetUserView(vecWinChairs[i]);
			if (pUserView)
			{
				pUserView->ShowWinType(0);
			}
			if (vecWinChairs[i] == MY_CHAIR_ID && m_cbMyCardData[0] != 0 && m_cbMyCardData[1] != 0)
			{
				ShowMeHasWin();
                g_globalGameSerialization.AddAtomicAction(AtomicAction::WinGame);
			}
		}
		DispatchWinJettonToUsr(vecUsrWinInfo, false);
	}
	PlayEffectSound2("sound/table/gameend.mp3");
    if (g_globalTableConfig.m_enterMoney == 80 && g_globalTableConfig.GetTableMode() != TableLooking)
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task800);
    else if (g_globalTableConfig.m_enterMoney == 200 && g_globalTableConfig.GetTableMode() != TableLooking)
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task2000);
    else if (g_globalTableConfig.m_enterMoney == 400 && g_globalTableConfig.GetTableMode() != TableLooking)
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task4000);
    else if (g_globalTableConfig.m_enterMoney == 600 && g_globalTableConfig.GetTableMode() != TableLooking)
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task6000);
    else if (g_globalTableConfig.m_enterMoney == 1000 && g_globalTableConfig.GetTableMode() != TableLooking)
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task10000);
    else if (g_globalTableConfig.m_enterMoney == 2000 && g_globalTableConfig.GetTableMode() != TableLooking)
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task20000);
    else if (g_globalTableConfig.m_enterMoney == 3000 && g_globalTableConfig.GetTableMode() != TableLooking && !g_globalTableConfig.IsTableHasVideo())
        g_globalGameSerialization.AddAtomicAction(AtomicAction::Task30000);
    else if (g_globalTableConfig.m_enterMoney == 3000 && g_globalTableConfig.GetTableMode() != TableLooking && g_globalTableConfig.IsTableHasVideo())
        g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskV30000);
}

void GameTable::DispatchWinJettonToUsr(const std::vector<UsrWinInfo>& vecUsrWinInfo, bool bOpenedCard)
{
	if (IsJettonCollecting())
	{
		SetJettonCollectFinishedCB(std::bind(&GameTable::DispatchWinJettonToUsr, this, vecUsrWinInfo, bOpenedCard));
		return;
	}

	float fDelay = 0.0f;
 	// adjust rhythm
	float fTimeDispatchJetton = g_cfgReader.GetDispatchWinJetton();//= 0.72f; // 筹码飞行的时间
	float fTimePreShowWinNumber = g_cfgReader.GetDelayBeforeShowWinNum();//0.0f; // 显示赢钱数字前稍微停顿下
	float fTimeShowWinNumber = g_cfgReader.GetShowWinNum();//0.72f; // 筹码飞到后显示赢的数字的时间
	for (unsigned i = 0; i < vecUsrWinInfo.size(); ++i)
	{
		auto usrWinInfo = vecUsrWinInfo[i];

		// 将一组边池里指定数量的筹码发给指定赢家
		auto funDispatchPots2Usr = [this, usrWinInfo, fTimeDispatchJetton]()
		{
			for (unsigned j = 0; j < usrWinInfo.uWinPots; ++j)
			{
				//当座位上的玩家退出后 不显示飞筹码的动画 sjf 2015-12-1
				if (!m_bUserValid[usrWinInfo.uChair])
				{
					continue;
				}

 				unsigned uPotIdx = usrWinInfo.uWinPotsIdx[j];
				double dWinValue = usrWinInfo.dPotsWinValue[j];

				if (dWinValue >= 1.0f && uPotIdx < m_vecPSidePot.size()) // CAODANDE: 后端数据可能传过来错误的值，dWinValue0，同时要命的是uPotIdx可能大于m_vecPSidePot.size()，导致下一句崩溃。所以这里必须检查下，兼容后端错误，否则崩溃
				{
					auto pSidePot = m_vecPSidePot[uPotIdx];
					auto ptJetton = pSidePot->getPosition();
					pSidePot->UpdatePotValue(pSidePot->GetPotValue() - dWinValue);
					std::vector<int> vecJettonIdx;
					GetJettonIdxs(dWinValue, vecJettonIdx);
					std::vector<Sprite*> vecJetton;
					for (auto iJettonIdx : vecJettonIdx)
					{
						auto pJetton = m_pJettonFactory->CreateJetton(iJettonIdx);
						pJetton->setPosition(ptJetton);
						ptJetton.y += 2;
						s_gameLayer->addChild(pJetton, WinJettonZOrder);
						vecJetton.push_back(pJetton);
					}
					auto ptDst = s_ptUserViewDefaultCenter[usrWinInfo.uChair];	
					float fEaseFactor = 0.8;
					for (auto jetton : vecJetton)
					{
						auto funRemoveJettonSelf = [jetton]()
						{
							jetton->removeFromParent();
						};
						auto actMov = Spawn::create(EaseIn::create(MoveTo::create(fTimeDispatchJetton, ptDst), fEaseFactor), FadeOut::create(fTimeDispatchJetton), nullptr);
						jetton->runAction(Sequence::create(actMov, CallFunc::create(funRemoveJettonSelf), nullptr));
						fEaseFactor += 0.5;
					}
				}				
			}
			PlayEffectSound2("sound/table/dispatchjetton.mp3");
		};
       
		// 显示赢取数字动画
		auto funcShowWinNumber = [this, usrWinInfo, fTimeShowWinNumber, fTimePreShowWinNumber]()
		{
				auto pWinNumber = GetUserWinNumber(usrWinInfo.uChair);
				auto actMove = Spawn::create(FadeIn::create(fTimeShowWinNumber), MoveBy::create(fTimeShowWinNumber, Vec2(0, 50+33)), nullptr);
				pWinNumber->UpdateNumber(usrWinInfo.dAllWinValue);
				pWinNumber->runAction(Sequence::create(DelayTime::create(fTimePreShowWinNumber), Show::create(), actMove, DelayTime::create(2.5), Hide::create(), nullptr));
		};
		if (bOpenedCard)
		{
			 auto funDryAshMinCard = [this, usrWinInfo]()
			{
				for (int k=0; k<m_UsrHandCard.size(); k++)
				{
					if (m_UsrHandCard[k].uChair == usrWinInfo.uChair /*&& find(m_vecWinChairs.begin(), m_vecWinChairs.end(), usrWinInfo.uChair) != m_vecWinChairs.end()*/)
					{
						for (int p=0; p<m_vecUsrWinInfo.size(); p++)
						{
							GetUserView(m_vecUsrWinInfo[p].uChair)->DryCashMinHandCard(2);
						}
						DryAshTwoMinCard(m_UsrHandCard[k].uChair, m_UsrHandCard[k].handCard);
					}
				}
			};
             if (usrWinInfo.bIsWinner)
             {
                 GetUserView(usrWinInfo.uChair)->runAction(Sequence::create(DelayTime::create(fDelay), CallFunc::create(funDryAshMinCard), CallFunc::create(funDispatchPots2Usr), DelayTime::create(fTimeDispatchJetton), CallFunc::create(funcShowWinNumber), nullptr));
             }
             else
             {
                 GetUserView(usrWinInfo.uChair)->runAction(Sequence::create(DelayTime::create(fDelay), CallFunc::create(funDryAshMinCard), CallFunc::create(funDispatchPots2Usr), DelayTime::create(fTimeDispatchJetton), nullptr));
             }
			
		}
		else // 不需要亮牌型了
		{
			if (GetUserView(usrWinInfo.uChair))
			{
				if (usrWinInfo.bIsWinner)
				{
					GetUserView(usrWinInfo.uChair)->runAction(Sequence::create(DelayTime::create(fDelay), CallFunc::create(funDispatchPots2Usr), DelayTime::create(fTimeDispatchJetton), CallFunc::create(funcShowWinNumber), nullptr));
				}
				else
				{
					GetUserView(usrWinInfo.uChair)->runAction(Sequence::create(DelayTime::create(fDelay), CallFunc::create(funDispatchPots2Usr), DelayTime::create(fTimeDispatchJetton), nullptr));
				}
			}
			else
			{
				//当前人物离开了
			}
			
			
		}

		fDelay += fTimePreShowWinNumber;
		fDelay += fTimeDispatchJetton;
        if (usrWinInfo.bIsWinner)
        {
            fDelay += fTimeShowWinNumber;
        }
	}

	if (g_globalTableConfig.GetTableMode() == TablePlaying)
	{
		float fWaitAutoCleanTable = g_cfgReader.GetDelayBeforeCleanTable();
		runAction(Sequence::create(DelayTime::create(fDelay + fWaitAutoCleanTable), CallFunc::create(std::bind(&GameTable::CleanTableAfterEnd, this)), CallFunc::create(std::bind(&GameTable::CaoDanDeSendReady, this)), nullptr));
	}
	else if(g_globalTableConfig.GetTableMode() == TableLooking)
	{
		float fWaitAutoCleanTable = g_cfgReader.GetDelayBeforeCleanTable();
		runAction(Sequence::create(DelayTime::create(fDelay + fWaitAutoCleanTable), CallFunc::create(std::bind(&GameTable::CleanTableAfterEnd, this)), CallFunc::create(std::bind(&GameTable::CaoDanDeSendReady, this)), nullptr));
	}
	else
	{

	}

	
}

void GameTable::UsrShowCard(unsigned uChairID, unsigned char cbCardData[2])
{
	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
		pUserView->ShowCard(cbCardData);
	}
	if (uChairID == MY_CHAIR_ID && g_globalTableConfig.GetTableMode() != TableLooking)
	{
		auto pHandCard = GetSpriteMyCard(0);
		if (pHandCard)
		{
			pHandCard->setVisible(false);
		}
		pHandCard = GetSpriteMyCard(1);
		if (pHandCard)
		{
			pHandCard->setVisible(false);
		}

		//开牌后  手牌清掉
		for (auto& pSprite : m_pSpriteMyCard)
		{
			if (pSprite)
			{
				pSprite->stopAllActions();
				pSprite->setVisible(false);
			}
		}

	}
	else
	{
		auto pHandCard = GetHandCard(uChairID, 0);
		if (pHandCard)
		{
			pHandCard->setVisible(false);
		}
		pHandCard = GetHandCard(uChairID, 1);
		if (pHandCard)
		{
			pHandCard->setVisible(false);
		}
	}
}

void GameTable::UsrChat(unsigned uChairID, const std::string& strChat)
{
	if (nullptr == m_pUsrChatTip[uChairID])
	{
		static const char *s_szBgChatTip[GAME_PLAYER] = {
			"chattip/bgchattipt.png", "chattip/bgchattiprt.png", "chattip/bgchattiprb.png",
			"chattip/bgchattipb.png", "chattip/bgchattipb.png", "chattip/bgchattipb.png",
			"chattip/bgchattiplb.png", "chattip/bgchattiplt.png", "chattip/bgchattipt.png"
		};


		auto lable = CreateLabelMSYH(24, "");
		m_pUsrChatTip[uChairID] = ShowLabelInLineSequence::create(s_szBgChatTip[uChairID], lable, 305);
		m_pUsrChatTip[uChairID]->ignoreAnchorPointForPosition(false);
		m_pUsrChatTip[uChairID]->setAnchorPoint(Point(0.5,0.5));
		

		m_pUsrChatTip[uChairID]->setPosition(s_ptChatTip[uChairID]);
		m_pUsrChatTip[uChairID]->SetHAlignment(TextHAlignment::CENTER);
		m_pUsrChatTip[uChairID]->SetLastLineFinishedCB([](ShowLabelInLineSequence*node){node->setVisible(false);});
		s_gameLayer->addChild(m_pUsrChatTip[uChairID], ChatTipZOrder);
	}
	m_pUsrChatTip[uChairID]->SetText(strChat);
	m_pUsrChatTip[uChairID]->setVisible(true);

	if(m_strOldChat != strChat)
	{
		m_strOldChat = strChat;
	}
	else
	{
		return ;
	}
	
	GetChatBox()->AppandWord(m_usrInfo[uChairID].m_strIcon, strChat, m_usrInfo[uChairID].m_uUID, (uChairID != MY_CHAIR_ID));
	// GetTableTip()->UserChat(m_usrInfo[uChairID].m_strName, strChat);
}
/************************************************************************/
/* 气泡提示                                                                     */
/************************************************************************/
void GameTable::RewardBubble()
{
    if (m_RewardBubble == nullptr)
    {
        auto lable = CreateLabelMSYH(20, "");
        lable->setContentSize(Size(78, 120));
        m_RewardBubble = ShowLabelInLineSequence::create("delaer/dashang.png", lable, 120, 2);
        m_RewardBubble->ignoreAnchorPointForPosition(false);
        m_RewardBubble->setAnchorPoint(Vec2(0.5, 0.5));
        m_RewardBubble->setPosition(Tools::getWidgetByName(s_uiRoot,"node_denvlang")->getPosition().x+115, Tools::getWidgetByName(s_uiRoot,"node_denvlang")->getPosition().y+25);
        m_RewardBubble->SetHAlignment(TextHAlignment::CENTER);
        m_RewardBubble->SetLastLineFinishedCB([](ShowLabelInLineSequence*node){node->setVisible(false);});
        s_gameLayer->addChild(m_RewardBubble, DealerZOrder+1);
    }
    const char* strTips1[] = {"谢谢，很高兴为您服务，祝您玩的开心",
        "我猜，你这局是个顺子",
        "真帅",
        "听说，打赏的越多，赢的也越多哦",
        "彪悍的人生不需要解释，ALLIN吧骚年",
        "独乐乐不如众乐乐，叫上好友一起玩吧",
        "那个给钱的真帅",
        "万水千山总是情，再给一次行不行"};
    const char* strTips2[] = {"谢谢，很高兴为您服务，祝您玩的开心",
        "我猜，你这局是个顺子",
        "真帅",
        "万水千山总是情，再给一次行不行",
        "谢谢老板！谢谢打赏！",
        "常规场有很多美女哦，不要忘了去搭讪啊",
        "牌风不佳时，试着打赏打赏，也许会时来运转哦，我只是说也许",
        "我看少侠骨骼清奇，维护世界和平的重任就交给你了！"};
    const char* strTips3[] = {"谢谢，很高兴为您服务，祝您玩的开心",
        "独乐乐不如众乐乐，叫上好友一起玩吧",
        "那个给钱的真帅",
        "万水千山总是情，再给一次行不行",
        "谢谢老板！谢谢打赏！",
        "常规场有很多美女哦，不要忘了去搭讪啊",
        "牌风不佳时，试着打赏打赏，也许会时来运转哦，我只是说也许",
        "我看少侠骨骼清奇，维护世界和平的重任就交给你了！"};
    const char* strTips4[] = {"谢谢，很高兴为您服务，祝您玩的开心",
        "我猜，你这局是个顺子",
        "听说，打赏的越多，赢的也越多哦",
        "彪悍的人生不需要解释，ALLIN吧骚年",
        "独乐乐不如众乐乐，叫上好友一起玩吧",
        "那个给钱的真帅",
        "万水千山总是情，再给一次行不行",
        "我看少侠骨骼清奇，维护世界和平的重任就交给你了！"};
    if (g_globalTableConfig.m_uRoomType == 0)
        m_RewardBubble->MultLineSetText(strTips1[rand()%(sizeof(strTips1)/sizeof(strTips1[0]))]);
    else if (g_globalTableConfig.m_uRoomType == 1 || g_globalTableConfig.m_uRoomType == 2)
        m_RewardBubble->MultLineSetText(strTips2[rand()%(sizeof(strTips2)/sizeof(strTips2[0]))]);
    else if (g_globalTableConfig.m_uRoomType == 3)
        m_RewardBubble->MultLineSetText(strTips3[rand()%(sizeof(strTips3)/sizeof(strTips3[0]))]);
    else if (g_globalTableConfig.m_uRoomType == 4)
        m_RewardBubble->MultLineSetText(strTips4[rand()%(sizeof(strTips4)/sizeof(strTips4[0]))]);
    else
    {
        m_RewardBubble->MultLineSetText(strTips4[rand()%(sizeof(strTips4)/sizeof(strTips4[0]))]);
    }
    m_RewardBubble->setVisible(true);
}

void GameTable::UsrExpress(unsigned uChairID, unsigned uIdx, unsigned uType) // uType: 0-common 1-贵族
{
	auto userView = GetUserView(uChairID);
	if (userView)
	{
		//userView->ShowExpress(uIdx + uType * 16);


		unsigned uExpressIdx = uIdx + uType * 16 ;

		if (m_biaoqingAni[uChairID][0]!=nullptr)
		{
			m_biaoqingAni[uChairID][0]->stopAni();
		}
		if (m_biaoqingAni[uChairID][1]!=nullptr)
		{
			m_biaoqingAni[uChairID][1]->stopAni();
		}

		int putongAni[2][16]={
			{
				0,9,14,8,11,10,12,2,1,7,3,15,4,5,6,13
			},
			{
				2,14,12,10,1,9,4,3,5,8,11,13,15,7,0,6
			},
		} ;

		std::string sEcpressName[2] = {
			"putongbiaoqing",
			"guzubiaoqing",
		} ;
		//表情零时创建
		if (m_biaoqingAni[uChairID][uExpressIdx/16] == nullptr)
		{
			m_biaoqingAni[uChairID][uExpressIdx/16] = AnimationManager::creatArmature(sEcpressName[uExpressIdx/16]);
			s_gameLayer->addChild(m_biaoqingAni[uChairID][uExpressIdx/16],ExpressTools) ;
		}

		m_biaoqingAni[uChairID][uExpressIdx/16]->setPosition(userView->getPosition()) ;
		m_biaoqingAni[uChairID][uExpressIdx/16]->playOnceByIndex(putongAni[uExpressIdx/16][uExpressIdx%16]);
	}
	if (uChairID == MY_CHAIR_ID)
	{
		g_globalMyData.AppandExpress(uType, uIdx);
	}
}

void GameTable::LookonUsrChat(unsigned uUID, const std::string& strNickName, const std::string& strIcon, const std::string& strChat)
{
	GetTableTip()->UserChat(strNickName, strChat);
	GetChatBox()->AppandWord(strIcon, strChat, uUID, (uUID == g_globalMyData.m_iUserId));
}

void GameTable::LookonUsrExpress(const std::string& strNickName, unsigned uIdx, unsigned uType)
{
	GetTableTip()->UserExpress(strNickName, uIdx + uType * 16);
}

void GameTable::UsrTool(unsigned uChairIDSrc, unsigned uChairIDDst, unsigned uToolIdx)
{
	//char szAni0[64] = {0};
	//char szAni1[64] = {0};
	//sprintf(szAni0, "tools/toolani%d0.webp", uToolIdx);
	//sprintf(szAni1, "tools/toolani%d1.webp", uToolIdx);
	//static unsigned s_uToolAni0XFrames[] = {3,3,2,2,2,2,2,2,2,2,2,2,3};
	//static unsigned s_uToolAni0YFrames[] = {1,1,3,3,3,3,3,3,3,3,3,3,1};
	//static unsigned s_uToolAni0Frames[]  = {3,3,6,6,6,6,6,6,6,6,6,6,3};
	//static unsigned s_uToolAni0Fps[] = {10, 13, 24, 24, 24, 24, 24,24, 24, 24, 24, 24, 10};
	//
	//static unsigned s_uToolAni1XFrames[] = {9,  7,  8,  8,  8,  8,  8, 8, 8,  8,  7,  8,  5};
	//static unsigned s_uToolAni1YFrames[] = {3,  5,  8,  8,  8,  10, 8, 8, 8,  8,  10, 8,  6};
	//static unsigned s_uToolAni1Frames[]  = {27, 35, 64, 64, 64, 80, 64,64, 61, 64, 64, 64, 26};
	//static unsigned s_uToolAni1Fps[] =	   {10, 13, 24, 24, 24, 24, 24, 24, 24,24, 24, 24, 10};
	//

	auto ptSrc = s_ptUserViewDefaultCenter[uChairIDSrc];
	auto ptDst = s_ptUserViewDefaultCenter[uChairIDDst];


	float fAni0Time = 0.3f;
	auto actMov = Spawn::create(MoveTo::create(fAni0Time, ptDst), nullptr);

	auto funcPlaySound2 = [this, uToolIdx](){
		const char *szSound[] = {
			"sound/magic/0.mp3",
			"sound/magic/1.mp3",
			"sound/magic/2.mp3", 
			"sound/magic/3.mp3", 
			"sound/magic/4.mp3",
			"sound/magic/5.mp3",
			"sound/magic/6.mp3", 

			"sound/magic/7.mp3", 
			"sound/magic/8.mp3",
			"sound/magic/9.mp3", 
			"sound/magic/10.mp3", 
			"sound/magic/11.mp3"
		};
		PlayEffectSound2(szSound[uToolIdx]);
	};

	PlayEffectSound2("sound/magic/fly.mp3");





	const char *toolsAniPath[] = {
		"toolani0",
		"toolani1",
		"toolani2",
		"toolani3",
		"toolani4",
		"toolani5",
		"toolani6",
		//"ceshiya",
		"toolani7",
		"toolani8",
		"toolani9",
		"toolani10",
		"toolani11",
	};









	//更新表情动画为编辑器
	if (m_aniToolsRuhua == nullptr) {
		m_aniToolsRuhua = AnimationManager::creatArmature("toolani2");
		m_aniToolsRuhua->setVisible(false) ;
		s_gameLayer->addChild(m_aniToolsRuhua,ToolZOrder);
	}

	if (m_aniTools[uToolIdx] == nullptr)
	{
		m_aniTools[uToolIdx] = AnimationManager::creatArmature(toolsAniPath[uToolIdx]);
		s_gameLayer->addChild(m_aniTools[uToolIdx],ToolZOrder);
		m_aniTools[uToolIdx]->setVisible(false);
	}

	m_aniTools[uToolIdx]->setPosition(ptSrc);

	auto aniMgr = m_aniTools[uToolIdx];


	//如花特殊
	if (uToolIdx==2)
	{
		m_aniToolsRuhua->setPosition(ptSrc);
		auto aniMgr2 = m_aniToolsRuhua;
	
		aniMgr->playOnceByIndexAndStopAtLastFrame(0);
		aniMgr->runAction(Sequence::create(actMov, CallFunc::create(funcPlaySound2), 
			CallFunc::create([aniMgr,aniMgr2](){
			aniMgr2->playOnceByIndex(1),
			aniMgr->playOnceByIndex(2);        

		}), nullptr));

	} /*else if (uToolIdx==7)
	  {
	  aniMgr->playOnceByIndex(0) ;

	  }*/
	else
	{
		aniMgr->playOnceByIndexAndStopAtLastFrame(0);
		aniMgr->runAction(Sequence::create(actMov, CallFunc::create(funcPlaySound2), 
			//CallFunc::create([m_aniToolsRuhua](){m_aniToolsRuhua->playOnceByIndex(1),
			CallFunc::create([aniMgr](){aniMgr->playOnceByIndex(1);        

		}), nullptr));
	}

	aniMgr->setZOrder(ToolZOrder);  
}

void GameTable::UsrAward(unsigned uChairID, double dCash)
{
	auto pAward = Reward::create();
	pAward->setVisible(false);
	pAward->setCascadeOpacityEnabled(true);
	s_gameLayer->addChild(pAward, RewardZOrder);
	pAward->SetValue(dCash);
	
	//auto actMov = Spawn::create(MoveTo::create(1.0f, m_pDealer->getPosition()), FadeOut::create(1.0f), nullptr);
	auto actMov = Spawn::create(MoveTo::create(1.0f, m_node_denvlang->getPosition()), FadeOut::create(1.0f), nullptr);
	auto funRemoveAwardAndPlayDealer = [this, pAward, uChairID]()
	{
		m_pDealer->PlayAniKiss();
		auto pBackKiss = Sprite::create("delaer/dealerbackkiss.png");
		pBackKiss->setVisible(false);
		s_gameLayer->addChild(pBackKiss, RewardZOrder);

		auto actMovBack = Spawn::create(MoveTo::create(1.0f, s_ptUserViewDefaultCenter[uChairID]), FadeOut::create(1.0f), ScaleBy::create(1.0f, 2.0f), nullptr);
		auto funcRemoveBackKiss = [this, pBackKiss]()
		{
			pBackKiss->removeFromParentAndCleanup(true);
		};
		//pBackKiss->runAction(Sequence::create(DelayTime::create(0.4f), Place::create(m_pDealer->getPosition()), Show::create(), actMovBack, Hide::create(), CallFunc::create(funcRemoveBackKiss), nullptr));
		pBackKiss->runAction(Sequence::create(DelayTime::create(0.4f), Place::create(m_node_denvlang->getPosition()), Show::create(), actMovBack, Hide::create(), CallFunc::create(funcRemoveBackKiss), nullptr));
		pAward->removeFromParentAndCleanup(true);
		PlayEffectSound2("sound/table/kiss.mp3");
	};
	pAward->ignoreAnchorPointForPosition(false) ;
	pAward->setAnchorPoint(Point(0.5,0.5));
	pAward->runAction(Sequence::create(Place::create(s_ptUserViewDefault[uChairID]), Show::create(), actMov, Hide::create(), CallFunc::create(funRemoveAwardAndPlayDealer), nullptr));

	PlayEffectSound2("sound/table/reward.mp3");

	RewardBubble() ;
}

void GameTable::UserFollowUser(unsigned uSrcCh, unsigned uDstCh)
{
	auto sprite = Sprite::create("userinfo/followflag.png");
	sprite->setPosition(s_ptUserViewDefaultCenter[uSrcCh]);
	s_gameLayer->addChild(sprite, FollowFlagZOrder);
	sprite->runAction(Sequence::create(DelayTime::create(0.2f), MoveTo::create(0.5f, s_ptUserViewDefaultCenter[uDstCh]), Spawn::create(FadeOut::create(0.5f), nullptr),CallFunc::create([sprite](){sprite->removeFromParentAndCleanup(true);}),nullptr));
}

void GameTable::PopUserInfo(const UserDetailInfo& usrInfo)
{

	//if (m_pUserInfoView->isVisible())
	//{
	//	return ;
	//}

		m_pUserInfoView->SetToolPrice(g_globalTableConfig.m_dMagicExpressPrice);

		if (getTableType() == SngTable)
		{
			m_pUserInfoView->UpdateUserInfo(usrInfo, false, usrInfo.m_uUID == g_globalMyData.m_iUserId);
		}
		else
		{
			m_pUserInfoView->UpdateUserInfo(usrInfo, g_globalTableConfig.GetTableMode() == TableLooking, usrInfo.m_uUID == g_globalMyData.m_iUserId,m_globaPrivateRoomInfo.m_isOwner);
		}
		

        m_strNickName = usrInfo.m_strName;
}

void GameTable::SetGuanZhuResult(unsigned uTUID, int iGuanZhuType, int iRet)
{
	if (m_pUserInfoView)
	{
		m_pUserInfoView->SetGuanZhuResult(uTUID, iGuanZhuType, iRet);
	}
}

void GameTable::SetPingBiResult(unsigned uTUID, int iPingBiType, int iRet)
{
	if (m_pUserInfoView)
	{
		m_pUserInfoView->SetPingBiResult(uTUID, iPingBiType, iRet);
	}
}

void GameTable::SetBiaoJiOrBeiZhuResult(unsigned uTUID, const std::string& strBeiZhu, int iBiaoJi, const std::string& strBiaoJi)
{
	if (m_pUserInfoView)
	{
		m_pUserInfoView->SetBiaoJiOrBeiZhuResult(uTUID, strBeiZhu, iBiaoJi, strBiaoJi);
	}
	auto player = g_globalTableConfig.GetPlayerByID(uTUID);
	if (player)
	{
		unsigned uChairView = g_globalTableConfig.GetViewChairID(player->m_uChairID);
		auto userView = GetUserView(uChairView);
		if (userView)
		{
			userView->SetBiaoJi(iBiaoJi);
		}
	}
}
void GameTable::UpdateMissionList()
{
	if (m_pQuickMissionList)
	{
		m_pQuickMissionList->UpdateMissionView();
	}
}

void GameTable::UpdateNewPlayerTask()
{
    /*if (m_pNewPlayerTask)
    {
        m_pNewPlayerTask->UpdateTask();
    }*/
}

void GameTable::UpdateOnlineFollow(unsigned uType)
{
	if (m_pQuickInviteList)
	{
		m_pQuickInviteList->UpdateView(uType);
	}
}

void GameTable::SetValue(double dFee)
{
	m_pRewardNumber->UpdateZeroNumber(dFee, 10);
}

void GameTable::UpdateItemBox()
{
    m_vecMissionInfo.clear();
	bool bMissionStatus = false; // g_globalMissionInfo.GetMission(0).size();
    unsigned uTableType = g_pGameFrame->getTableType();
    for (auto& mission : g_globalMissionInfo.GetMissions(0))//过滤每日任务
    {
        if ((mission.uMissonStatus != 2 && mission.uScence == uTableType+1) || (mission.uScence == 4))
        {
            m_vecMissionInfo.push_back(mission);
        }
    }
    for (auto& mission : g_globalMissionInfo.GetMissions(3))//过滤每周任务
    {
        if ((mission.uMissonStatus != 2 && mission.uScence == uTableType+1) || (mission.uScence == 4))
        {
            m_vecMissionInfo.push_back(mission);
        }
    }
    for (auto& mission : g_globalMissionInfo.GetMissions(2))//过滤成就任务
    {
        if (mission.uMissonStatus != 2)
        {
            m_vecMissionInfo.push_back(mission);
        }
    }

    for (auto itemInfo : m_vecMissionInfo)
    {
        if (itemInfo.uMissonStatus == 1)
        {
            bMissionStatus = true;
            break;
        }
        
    }
	if(bMissionStatus)
	{
		m_pMenuItemPopMissionRewardAni->setVisible(true);
		m_pRewardAnimateViewer->setVisible(true);
		m_pRewardAnimateViewer->playByIndexAndRepatForever(0);
		m_pMenuItemPopMissionReward->setVisible(false);
	}
	else
	{
		m_pRewardAnimateViewer->stopAni();
		m_pRewardAnimateViewer->setVisible(false);
		m_pMenuItemPopMissionRewardAni->setVisible(false);
		m_pMenuItemPopMissionReward->setVisible(true);
	}


}

void GameTable::OperateOnlineFollow(unsigned uOpType, unsigned uType, const QuickFollowInfo& quickFollowInfo)
{
    if (uOpType == 3 &&  g_globalQuickFollowInfo.m_bIsReceiveInvite == true)
    {
        m_pSpriteInviteFlag->setVisible(true);
        m_bIsReceiveInvite = true;
    }
	if (m_pQuickInviteList)
	{
		if (uOpType == 2)
		{
			m_pQuickInviteList->UpdateItemView(uType, quickFollowInfo);
		}
		else if (uOpType == 1)
		{
			m_pQuickInviteList->AppadnItemView(uType, quickFollowInfo);
		}
		else if (uOpType == 0)
		{
			m_pQuickInviteList->RemoveItemView(uType, quickFollowInfo.m_uUserID);
		}	
		else if (uOpType == 3)
		{
			m_pQuickInviteList->ReplaceAndTopItemView(uType, quickFollowInfo);
		}
	}
}

GameTable::GameTable()
{
	log("GameTable()");
}

GameTable::~GameTable()
{
	delete m_pJettonFactory;
	m_pJettonFactory = nullptr;

	log("~GameTable()");
}

void GameTable::SetUsrBeting(unsigned uChairID)
{
	++m_uBetingCount;
}

void GameTable::SetUsrBetFinished(unsigned uChairID)
{
	--m_uBetingCount;
	if (m_uBetingCount == 0)
	{
		if (m_funCBAllBetingFinished)
		{
			m_funCBAllBetingFinished();
			m_funCBAllBetingFinished = nullptr;
		}
	}
}

unsigned GameTable::GetUsrBeting()
{
	return m_uBetingCount;
}

void GameTable::SetAllBetFinishedCB(std::function<void()>cbAllBetingFinished)
{
	m_funCBAllBetingFinished = cbAllBetingFinished;
}

void GameTable::SetJettonCollecting(bool bCollecting)
{
	if (m_bCollecting != bCollecting)
	{
		m_bCollecting = bCollecting;
		if (!m_bCollecting && m_funCBCollectingFinished)
		{
			m_funCBCollectingFinished();
			m_funCBCollectingFinished = nullptr;
		}
	}
}

bool GameTable::IsJettonCollecting()
{
	return m_bCollecting;
}

void GameTable::SetJettonCollectFinishedCB(std::function<void()>cbJettonCollectFinished)
{
	m_funCBCollectingFinished = cbJettonCollectFinished;
}


void GameTable::SetPublicCardDispatchedCB(std::function<void(unsigned uAllPublicCard)>cbPublicCardDispatched)
{
	m_funCBPublicCardDispatched = cbPublicCardDispatched;
}

void GameTable::onGrabTouchBegan(bool bHitDialog)
{
}

void GameTable::onSetVisible(bool bVisible)
{
	for (auto item : m_pUsersViewTouchGrabber)
	{
		if (item)
		{
			item->SetTouchEnable(bVisible);
		}
	}
	if (!bVisible)
	{
		PlayVisibleSound(bVisible);
	}
    else
    {
        if (getTableType() == NormalTable)
        {
            //GameTipsManager::getInstance()->clean();
            auto guideTips = [this](){
                createFriendGuide();
                createAttentionGuide();
                GameTipsManager::getInstance()->callNextTips();
            };
            this->runAction(Sequence::create(DelayTime::create(0.7), CallFunc::create(guideTips), nullptr));
        }
    }
}

void GameTable::onKeyEscPressed()
{

	if (getTableType() == SngTable ||getTableType() == MttTable)
	{
		callSngExit() ;
	}
	else
	{
		SendClientRequest(ClientRequest::kLeaveTable);
	}

	
}

void GameTable::GetJettonIdxs(double dValue, std::vector<int>& vecJettonIdx)
{
	if (g_globalTableConfig.m_dLittleBlind < 1 || dValue < g_globalTableConfig.m_dLittleBlind) // 
	{
		return;
	}
	unsigned uLittleBlindTime[] = {1, 5, 10, 25, 100, 500, 1000, 2000, 5000, 10000};
	std::vector<unsigned> vecLittleBlindTime(uLittleBlindTime, uLittleBlindTime + sizeof(uLittleBlindTime) / sizeof(uLittleBlindTime[0]));

	std::vector<double> vecJettonValue;
	for (unsigned i = 0; i < vecLittleBlindTime.size(); ++i)
	{
		vecJettonValue.push_back(vecLittleBlindTime[i] * g_globalTableConfig.m_dLittleBlind);
	}

	int iCurIdx = vecJettonValue.size() - 1;
	do
	{
		double dN = dValue / vecJettonValue[iCurIdx];
		
		if (dN >= 1.0f)
		{
			unsigned uN = static_cast<unsigned>(dN);
			for (unsigned i = 0; i < uN; ++i)
			{
				vecJettonIdx.push_back(iCurIdx);
				dValue -= vecJettonValue[iCurIdx];
			}
		}
		--iCurIdx;
	} while (iCurIdx >= 0 && dValue >= vecJettonValue[0]);
}
unsigned GameTable::GetJettonIdx(double dValue)
{
	std::vector<int> vecJettonIdx;
	GetJettonIdxs(dValue, vecJettonIdx);
	if (!vecJettonIdx.empty())
	{
		return vecJettonIdx[0];
	}
	log("无法表示的错误筹码值:%f", dValue);
	return 0;
}

void GameTable::UsrPlaceJetton(unsigned uChairID, double dBetValue)
{
	SetUsrBeting(uChairID);
	m_dUsrPotValue[uChairID] += dBetValue;
	if (nullptr == m_pSpriteJetton2UsrPot[uChairID])
	{
		m_pSpriteJetton2UsrPot[uChairID] = m_pJettonFactory->CreateJetton(GetJettonIdx(dBetValue));
		m_pSpriteJetton2UsrPot[uChairID]->setVisible(false);
		m_pSpriteJetton2UsrPot[uChairID]->setPosition(s_ptUserViewDefaultCenter[uChairID]);
		s_gameLayer->addChild(m_pSpriteJetton2UsrPot[uChairID], BetJettonZOrder);
	}
	cocos2d::Sprite *pJetton = m_pSpriteJetton2UsrPot[uChairID];
	pJetton->setOpacity(128);
	pJetton->setPosition(s_ptUserViewDefaultCenter[uChairID]);
	pJetton->setVisible(true);
	float fTimeJettonMove = g_cfgReader.GetBetJetton();// 0.32f;  // adjust rhythm
	auto pActMoveAndFadeOut = Spawn::create(MoveTo::create(fTimeJettonMove, s_ptJettonDst[uChairID]), FadeIn::create(fTimeJettonMove), nullptr);
	auto pFunUpdateBet = [this, uChairID](){	
		SetUsrBetedJetton(uChairID, m_dUsrPotValue[uChairID]);
		SetUsrBetFinished(uChairID);
	};
	pJetton->runAction(Sequence::create(pActMoveAndFadeOut, Hide::create(), CallFunc::create(pFunUpdateBet), nullptr));
	
	m_dAllPlacedJettonValue += dBetValue;
	//SetLabelString(m_pLabelAllPlacedJettonValue, FormatCash(m_dAllPlacedJettonValue));
		setUITextString(m_pLabelAllPlacedJettonValue, "底池："+FormatCash(m_dAllPlacedJettonValue)) ;
	PlayEffectSound2("sound/table/bet.mp3");
}

//交叠的卡帕位置
Vec2 GameTable::GetSidePotPosition(unsigned uPotCount, unsigned uPotIdx)
{
	//int posX[GAME_PLAYER];

	//for(int i=0;i<GAME_PLAYER; i++)
	//{
	//	posX[i]= s_allinChizi[i].x;
	//}

	//int posY[2];
	//posY[0] = s_allinChizi[0].y;
	//posY[1] = s_allinChizi[5].y;


	//static Vec2 s_ptPot1[GAME_PLAYER] = {
	//	Vec2(posX[2],posY[0]),
	//	Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	//};
	//static Vec2 s_ptPot2[GAME_PLAYER] = {
	//	Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),
	//	Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	//};
	//static Vec2 s_ptPot3[GAME_PLAYER] = {
	//	Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),
	//	Vec2(), Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	//};
	//static Vec2 s_ptPot4[GAME_PLAYER] = {
	//Vec2(posX[5],posY[0]),Vec2(posX[6],posY[0]),Vec2(posX[7],posY[0]),Vec2(posX[8],posY[0]),
	//	Vec2(), Vec2(), Vec2(), Vec2(), Vec2(),
	//};
	//static Vec2 s_ptPot5[GAME_PLAYER] = {
	//	Vec2(posX[0],posY[0]),Vec2(posX[1],posY[0]),
	//	Vec2(posX[2],posY[0]),
	//	Vec2(posX[3],posY[0]),Vec2(posX[4],posY[0]),	
	//	Vec2(), Vec2(), Vec2(), Vec2(),
	//};
	//static Vec2 s_ptPot6[GAME_PLAYER] = {
	//Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),
	//Vec2(posX[1],posY[1]),Vec2(posX[2],posY[1]),Vec2(posX[3],posY[1]),	
	//	Vec2(), Vec2(), Vec2(),
	//};

	//static Vec2 s_ptPot7[GAME_PLAYER] = {
	//	Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),
	//	Vec2(posX[6],posY[1]),Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),Vec2(posX[8],posY[1]),
	//	Vec2(), Vec2(),
	//};
	//static Vec2 s_ptPot8[GAME_PLAYER] = {
	//	Vec2(posX[5],posY[0]),Vec2(posX[6],posY[0]),Vec2(posX[7],posY[0]),Vec2(posX[8],posY[0]),
	//	Vec2(posX[5],posY[1]),Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),Vec2(posX[8],posY[1]),
	//	Vec2(),
	//};
	//static Vec2 s_ptPot9[GAME_PLAYER] = {
	//	Vec2(posX[0],posY[0]),Vec2(posX[1],posY[0]),Vec2(posX[2],posY[0]),Vec2(posX[3],posY[0]),Vec2(posX[4],posY[0]),
	//	Vec2(posX[5],posY[1]),Vec2(posX[6],posY[1]),Vec2(posX[7],posY[1]),Vec2(posX[8],posY[1]),
	//};

	//static Vec2* s_ptPots[GAME_PLAYER] = {
	//	s_ptPot1, s_ptPot2, s_ptPot3, s_ptPot4,
	//	s_ptPot5,
	//	s_ptPot6, s_ptPot7, s_ptPot8, s_ptPot9,
	//};
	return s_ptPots[uPotCount-1][uPotIdx];
}
/************************************************************************/
/* 主逻辑                                                                     */
/************************************************************************/
void GameTable::onMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (m_pUserInfoView!=nullptr)
		{
				m_pUserInfoView->setVisible(false) ;
		}
	
		if (pItem == m_pMenuItemRecordSound)
		{
			callSoundRecorder() ;
		}
		else
		if (pItem == m_pMenuItemPopMenu)
		{
		/*	if (m_curTableType == NormalTable)
			{*/
				if (nullptr == m_pDoorMenu)
				{
					m_pDoorMenu = DoorMenu::creatWithLayer(m_pMenuItemPopMenu);
					m_pDoorMenu->setVisible(false);
					addChild(m_pDoorMenu, DoorMenuZOrder);

					auto funCBDoorMenuShowCardType = [this]()
					{
						if (m_pCardTypeView == nullptr)
						{
							m_pCardTypeView = CardTypeView::create();
							m_pCardTypeView->ignoreAnchorPointForPosition(false);
							m_pCardTypeView->setAnchorPoint(Point(0,0.5));
							m_pCardTypeView->setVisible(false);
							addChild(m_pCardTypeView, CardTypeViewZOrder);
						}
						m_pCardTypeView->setVisible(true);
					};
					m_pDoorMenu->SetCBShowCardType(funCBDoorMenuShowCardType);
				}
				m_pDoorMenu->setVisible(true);
			//}

	
		}


		else if (m_pMenuItemReward == pItem)
		{
			//	UsrAward(rand()%9, 20);
			SendClientRequest(ClientRequest::kReward);
            //if (g_globalTableConfig.GetTableMode() == TablePlaying)
            
		}
		else if (m_pMenuItemPopMall == pItem)
		{
			m_vecLayer.clear() ;
			if (nullptr == m_pQuickShop)
			{
				m_pQuickShop = QuickShop::create();
				m_pQuickShop -> ignoreAnchorPointForPosition(false);
				m_pQuickShop -> setAnchorPoint(Point(0.5,0.5));

			
				addChild(m_pQuickShop, QuickShopZOrder);
			}
			m_pQuickShop->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
			m_pQuickShop->UpdateJetton(g_globalMyData.GetCash(), g_globalTableConfig.m_dAutoSupplyMoney, g_globalTableConfig.m_dMinBringMoney, g_globalTableConfig.m_dMaxBringMoney, g_globalTableConfig.m_uAutoSupplyToMaxMoney);
			m_pQuickShop->setVisible(true);

				
		}

		else if (pItem == m_pMenuItemPopChat)
		{
			auto pChatBox = GetChatBox();
			pChatBox->setVisible(true);
		}
		else if (pItem == m_pMenuItemPopMissionReward)
		{
			if (m_pQuickMissionList == nullptr)
			{
				m_pQuickMissionList = QuickMissionList::create();
				m_pQuickMissionList -> ignoreAnchorPointForPosition(false);
				m_pQuickMissionList -> setAnchorPoint(Point(1,1));
				m_pQuickMissionList->setPosition(Director::getInstance()->getVisibleSize().width-10,
					m_pMenuItemPopMissionReward->getPosition().y+m_pMenuItemPopMissionReward->getContentSize().height/2);

				addChild(m_pQuickMissionList, QuickMissionListZOrder);
			}
			m_pQuickMissionList->setVisible(true);


		}
		else if (pItem == m_pMenuItemPopInvite)
		{
			if (m_pQuickInviteList == nullptr)
			{
				m_pQuickInviteList = QuickInviteList::create();
				m_pQuickInviteList -> ignoreAnchorPointForPosition(false);
				m_pQuickInviteList -> setAnchorPoint(Point(1,1));
				m_pQuickInviteList->setPosition(Director::getInstance()->getVisibleSize().width,
					m_pMenuItemPopInvite->getPositionY()-m_pMenuItemPopInvite->getContentSize().height/2-5);
				//m_pQuickInviteList->setVisible(false);

				addChild(m_pQuickInviteList, QuickInviteListZOrder);
			}
			if (m_bIsReceiveInvite)
			{
				m_pQuickInviteList->ShowInviteFlag(true);
				m_bIsReceiveInvite = false;
			}
			//g_pGameFrame->callUpdateInviteList();
			//m_gameMsgIO.SendGameMessage(GameMsgCmdC::GetFollowUserInfo);
			g_globalQuickFollowInfo.m_bIsReceiveInvite = false;
			m_pSpriteInviteFlag->setVisible(false);
			m_pQuickInviteList->setVisible(true);
		}
		else if(pItem == m_pMenuItemWinningRanking)
		{
			if(m_pWinningRaking == nullptr)
			{
				m_pWinningRaking = WinningRaking::create();
				m_pWinningRaking -> ignoreAnchorPointForPosition(false);
				m_pWinningRaking -> setAnchorPoint(Point(1,1));
				auto posnode = Tools::getWidgetByName(s_uiRoot,"gt_listjiangchi");
				m_pWinningRaking->setPosition(Director::getInstance()->getVisibleSize().width,
					m_pMenuItemWinningRanking->getPositionY()-m_pMenuItemWinningRanking->getContentSize().height/2);
				m_pWinningRaking->setVisible(false);
				addChild(m_pWinningRaking, WinningRakingZOrder);
			}
			m_pWinningRaking->UpdateView();
			m_pWinningRaking->setVisible(true);
		}
		else if (pItem == m_pMenuItemAutoSitDown)
		{
			SendClientRequest(ClientRequest::kRequestAutoSitDown);
			m_pMenuItemAutoSitDown->setVisible(false);
			m_pLabelWaitSitDown->setVisible(true);
			m_pAniWaitSitDown->setVisible(true);
            m_IsWaitingSitDown = true;
		}
		else if (pItem == m_pMenuItemSitDown)
		{
			SendClientRequest(ClientRequest::kRequestSitDown);

			if (getTableType() == PrivateTable)
			{
				m_bt_Private_startOrEndGame->setVisible(false);
			}

			//		m_pMenuItemSitDown->setVisible(false);
		}
        else if (pItem == m_pMenuItemPopMission)
        {
			m_vecLayer.clear() ;
			SendClientRequest(ClientRequest::kShowNewPlayerTask);
			//showPrivateRoomInfoLayer(true) ;
        }
	}

}



/************************************************************************/
/* 主逻辑                                                                     */
/************************************************************************/
void GameTable::onSngMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (m_pUserInfoView!=nullptr)
		{
			m_pUserInfoView->setVisible(false) ;
		}
		if (m_buttonSngRank == pItem)
		{

			m_vecLayer.clear() ;
			if (getTableType() == MttTable )
			{
				if (g_globaMttGameInfo.m_getRecord)
				{
					g_pGameFrame->showMttMatchState(true,0);

				}
				
			}else if(getTableType() == SngTable)
			{
				showSngMatchState(true) ;
			}
			
		}
		else if (m_sngReturn == pItem)
		{
			callSngExit() ;	

		}
		else if (m_buttomSngAuto == pItem)
		{
			//取消托管
			resetTimeOve() ;
			showGameAutoTrust(false);
			SendClientRequest(ClientRequest::kCallClientCancelAutoTrust) ;
		}
	}

}



void GameTable::onScheduleUpdateTime(float fDelta)
{
	time_t tCur = time(nullptr);
	tm *tDesc = localtime(&tCur);
	char szTime[128] = {0};
	sprintf(szTime, "%02d:%02d", tDesc->tm_hour, tDesc->tm_min);
	setUITextString(m_pLabelTime, szTime);
}

void GameTable::OnScheduleDispatchHandCard(float fDelta)
{
	//static Vec2 s_ptDispatchedCard[GAME_PLAYER*2] = {
	//	Vec2(746, 445/*770,445*/),Vec2(919,347),Vec2(919,181),Vec2(770,125),
	//	Vec2(520,125),
	//	Vec2(190,125),Vec2(41,181),Vec2(41,347),Vec2(/*190*/214,445),

	//	Vec2(755, 441/*778,443*/),Vec2(927,345),Vec2(927,179),Vec2(778,123),
	//	Vec2(528,123),
	//	Vec2(198,123),Vec2(49,179),Vec2(49,345),Vec2(/*198,443*/223, 441),
	//};
	if (m_uDispatchingIdx < m_vecDispatchingChairID.size())
	{
		unsigned uRoundIdx = m_uDispatchingIdx < (m_vecDispatchingChairID.size() / 2) ? 0 : 1;
		unsigned uCurDispatchChairID = m_vecDispatchingChairID[m_uDispatchingIdx];

		Sprite *pSpriteDispatchingCard = GetSpriteDispatchingCard(uRoundIdx, uCurDispatchChairID);

        auto FoldHandCardFun = [this ,uRoundIdx, uCurDispatchChairID]()
        {
            if (uRoundIdx == 1 && !m_bUserValid[uCurDispatchChairID])
            {
                GetSpriteDispatchingCard(0, uCurDispatchChairID)->setVisible(false);
                GetSpriteDispatchingCard(1, uCurDispatchChairID)->setVisible(false);
            }
        };

		// adjust rhythm
		float runTime = g_cfgReader.GetDispatchHandCard(uRoundIdx); // 0.28f;
		//if (uRoundIdx == 1) {
		//	runTime = 0.4f;
		//}

		if ((g_globalTableConfig.GetTableMode() != TableLooking) && uCurDispatchChairID == MY_CHAIR_ID)
		{
			auto pActionMove = Spawn::create(FadeIn::create(runTime), MoveTo::create(runTime, s_ptDispatchedCard[uRoundIdx * GAME_PLAYER + uCurDispatchChairID]), nullptr);
			auto pActShowMyCard = CallFunc::create([this, uRoundIdx](){auto pMyCard = this->GetSpriteMyCard(uRoundIdx); PlayAnimateOpenHandCard(pMyCard, this->m_cbMyCardData[uRoundIdx], uRoundIdx);});
			pSpriteDispatchingCard->runAction(Sequence::create(Show::create(),pActionMove, Hide::create(), pActShowMyCard, nullptr));
		}
		else
		{
			auto pActionMove = Spawn::create(FadeIn::create(runTime), MoveTo::create(runTime, s_ptDispatchedCard[uRoundIdx * GAME_PLAYER + uCurDispatchChairID]), nullptr);
			pSpriteDispatchingCard->runAction(Sequence::create(Show::create(), pActionMove, CallFunc::create(FoldHandCardFun), nullptr));
		}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		const char *szEffect[2] = {"sound/table/handcard0.ogg", "sound/table/handcard1.ogg"};
#else
		const char *szEffect[2] = {"sound/table/handcard0.mp3", "sound/table/handcard1.mp3"};
#endif
		PlayEffectSound2(szEffect[uRoundIdx]);
		++m_uDispatchingIdx;
	}
}

void GameTable::DishpatchPublicCard(unsigned uIdx, unsigned char cbCardData, bool bDelay)
{

	//{
	//	PlayEffectSound2("sound/table/publiccard45.mp3");
	//}
	unsigned uDispatchedCardCount = m_vecPublicCardData.size();
	//ReSetPublicCardPos(uDispatchedCardCount);
	m_vecPublicCardData.push_back(cbCardData);
	m_deqDispatchingPublicCardData.pop_front();

	auto ani = CreateAnimate("anishowcard.png", 4, 2, 7, 1.0f / 24.0f);
	auto pCard = GetSpritePublicCard(uDispatchedCardCount); 

	auto funcPlaySound = [this, uIdx](){
		if (uIdx < 3)
		{
			if (uIdx == 0)
			{
				PlayEffectSound2("sound/table/fanpai13.mp3");	
			}	
		}
		else if (uIdx == 3)
		{
			PlayEffectSound2("sound/table/fanpai4.mp3");
		}
		else if (uIdx == 4)
		{
			PlayEffectSound2("sound/table/fanpai5.mp3");
		}
	};

	auto funCB = [this, uDispatchedCardCount, cbCardData, uIdx, pCard](){
		pCard->SetFrameIdx(GetCardFrameIdx(cbCardData)); 
		OnPublicCardDispatched(uIdx);
	};
	if (bDelay)
	{		
        // adjust rhythm
		pCard->runAction(Sequence::create(DelayTime::create(g_cfgReader.GetPublicCardDelay()), Show::create(), ani, CallFunc::create(funcPlaySound), CallFunc::create(funCB), nullptr));
	}
	else
	{
		pCard->runAction(Sequence::create(Show::create(), ani, CallFunc::create(funcPlaySound), CallFunc::create(funCB), nullptr));
	}
    m_uPreThinkChairID = -1;
}

void GameTable::OnPublicCardDispatched(unsigned uIdx)
{
	
	if (m_cbMyCardData[0] != 0 && m_cbMyCardData[1] != 0 && g_globalTableConfig.GetTableMode() == TablePlaying)
	{
		CGameLogic gameLogic;
		if (m_vecPublicCardData.size() == 3)
		{
			std::vector<unsigned char> vecAllCard(m_vecPublicCardData);
			vecAllCard.push_back(m_cbMyCardData[0]);
			vecAllCard.push_back(m_cbMyCardData[1]);
			gameLogic.SortCardList(&vecAllCard[0], vecAllCard.size());
			unsigned uMaxCardT = gameLogic.GetCardType(&vecAllCard[0], vecAllCard.size());
			m_pSpriteMaxCardType->SetFrameIdx(uMaxCardT);
		}
		else if (m_vecPublicCardData.size() == 4)
		{
			unsigned char cbMaxCard[5];
			gameLogic.FiveFromSix(m_cbMyCardData, 2, &m_vecPublicCardData[0], 4, cbMaxCard, 5);
			unsigned uMaxCardT = gameLogic.GetCardType(cbMaxCard, 5);
			m_pSpriteMaxCardType->SetFrameIdx(uMaxCardT);
		}
		else if (m_vecPublicCardData.size() == 5)
		{
			unsigned char cbMaxCard[5];
			gameLogic.FiveFromSeven(m_cbMyCardData, 2, &m_vecPublicCardData[0], 5, cbMaxCard, 5);
			unsigned uMaxCardT = gameLogic.GetCardType(cbMaxCard, 5);
			m_pSpriteMaxCardType->SetFrameIdx(uMaxCardT);
		}
	}

	if (uIdx == 4)
	{
		if (m_vecPublicCardData.size() == 5)
		{
			m_bAllPublicCardDispatched = true;
		}
	}
	if (m_funCBPublicCardDispatched)
	{
		m_funCBPublicCardDispatched(m_vecPublicCardData.size());
	}

	if (!m_deqDispatchingPublicCardData.empty())
	{
		bool bDelay = uIdx >= 2;
		DishpatchPublicCard(uIdx + 1, m_deqDispatchingPublicCardData.front(), bDelay);
	}
}

void GameTable::OnScheduleOpenUsrCardFinished(float fDelta)
{
	if (m_bOpenCarding)
	{
		m_bOpenCardFinished = true;
	}
}


void GameTable::OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched(float fDelta)
{
	if (m_bOpenCarding && m_bOpenCardFinished && m_bAllPublicCardDispatched)
	{
		unschedule(schedule_selector(GameTable::OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched));
		for (int i = 0; i < m_vecWinChairs.size(); i++)
		{
			
            if (GetUserView(m_vecWinChairs[i]) == nullptr)
                return;
            else
            {
                GetUserView(m_vecWinChairs[i])->ShowWinType(m_uWinType);
            }
			if (m_vecWinChairs[i] == MY_CHAIR_ID && m_cbMyCardData[0] != 0 && m_cbMyCardData[1] != 0)
			{
				ShowMeHasWin();
			}
		}
		if (m_uWinType>=7)
		{
			unsigned uWinType = m_uWinType - 7;
	

			//const char *szAniWinType[] = {
			//	"effects/maxcardhulu.webp",
   //             "effects/maxcardjinggang.webp",
			//	"effects/maxcardtonghuashun.webp",
			//	"effects/maxcardhuangjiatonghuashun.webp"
			//};
			//auto& ani = m_pAniShowMaxCard[uWinType];
			//if (ani == nullptr)
			//{
			//	ani = CreateAnimate(szAniWinType[uWinType], 3, 6, 17, 1.0f / 10.0f, false);
			//	ani->retain();
			//}
			//else
			//{
			//	ani->update(0);
			//}
			m_pSpriteShowMaxCard->setVisible(true);
			m_pAniShowMaxCard[uWinType]->playOnceByIndexAndStopAtLastFrame(0);


			const char *szSoundWinType[] = {
				"sound/table/hulu.mp3",
                "sound/table/jinggang.mp3",
				"sound/table/tonghuashun.mp3",
				"sound/table/huangjiatonghuashun.mp3"
			};
			PlayEffectSound2(szSoundWinType[uWinType]);
		}

		scheduleOnce(schedule_selector(GameTable::OnScheduleDispatchWinJettonToUsr), g_cfgReader.GetDelayBeforeDispatchWinJetton()/* 1.5f*/);
	}
}
void GameTable::OnScheduleDispatchWinJettonToUsr(float fDelta)
{
	DispatchWinJettonToUsr(m_vecUsrWinInfo, true);
}

Sprite *GameTable::GetSpriteDispatchingCard(unsigned uRoundId, unsigned uChairId)
{	
	//Sprite *pSprite = nullptr;
	auto& pSprite = m_pSpriteDispatchingCard[uChairId*2+uRoundId];
	const char *pSpritePath[] = {"dispatchingcard0.png", "dispatchingcard1.png"};
	if (pSprite == nullptr)
	{
		pSprite = Sprite::create(pSpritePath[uRoundId]);
		pSprite->setVisible(false);
		s_gameLayer->addChild(pSprite, DispatchingCardZOrder);
	}

	pSprite->setVisible(false);
	pSprite->setPosition(s_ptPublicCard[2].x, s_ptPublicCard[2].y);
	pSprite->setOpacity(0);

	return pSprite;
}

cocos2d::Sprite *GameTable::GetHandCard(unsigned uChairID, unsigned uCardIdx)
{
	return m_pSpriteDispatchingCard[uChairID*2+uCardIdx];
}

FrameSprite *GameTable::GetSpriteMyCard(unsigned uCardId)
{
	const char *s_szMyCard[2] = {"handcard0.png", "handcard1.png"};
	if (m_pSpriteMyCard[uCardId] == nullptr)
	{
		m_pSpriteMyCard[uCardId] = FrameSprite::create(s_szMyCard[uCardId], 13, 5);
		m_pSpriteMyCard[uCardId]->setVisible(false);
		m_pSpriteMyCard[uCardId]->setPosition(s_ptMyCard[uCardId]);
		s_gameLayer->addChild(m_pSpriteMyCard[uCardId], MyCardZOrder);
	}
	return m_pSpriteMyCard[uCardId];
}

Sprite *GameTable::GetSpriteFoldingCard(unsigned uChairId, unsigned uIdx)
{

	auto& pSprite = m_pSpriteFoldingCard[uChairId*2+uIdx];
	if (pSprite == nullptr)
	{
		pSprite = Sprite::create("foldingcard.png");
		s_gameLayer->addChild(pSprite, FoldingCardZOrder);
	}
	else
	{		
	}
	pSprite->setOpacity(255);
	pSprite->setVisible(false);
	pSprite->setPosition(s_ptFoldingCard[uChairId]);
	if (uIdx == 0)
	{
		pSprite->setPositionX(s_ptFoldingCard[uChairId].x - 4);
	}
	else
	{
		pSprite->setPositionX(s_ptFoldingCard[uChairId].x + 4);
	}

	return pSprite;
}

FrameSprite *GameTable::GetSpritePublicCard(unsigned uCardId)
{
	// TODO:
	if (m_pSpritePublicCard[uCardId] == nullptr)
	{
		m_pSpritePublicCard[uCardId] = FrameSprite::create("publiccard.webp", 13, 5); //// m_pPublicCardFactory->CreateCard();
		m_pSpritePublicCard[uCardId]->setVisible(false);
		s_gameLayer->addChild(m_pSpritePublicCard[uCardId], PublicCardZOrder);

        Size sz = m_pSpritePublicCard[uCardId]->getContentSize();
        m_pSpritePublicCardCash[uCardId] = CreateNode1AndPosAddTo<Sprite>("publiccardshade.png", sz.width/2, sz.height/2, m_pSpritePublicCard[uCardId], 0);
        m_pSpritePublicCardCash[uCardId]->setVisible(false);
		m_hasPublicCard[uCardId] = true ;
	}
	m_pSpritePublicCard[uCardId]->setPosition(s_ptPublicCard[uCardId]);
	return m_pSpritePublicCard[uCardId];
}

UserView* GameTable::GetUserView(unsigned uChairID)
{
	return m_pUsers[uChairID];
}

TextureNumber *GameTable::GetUserWinNumber(unsigned uChairID)
{
	if (m_pWinNumber[uChairID] == nullptr)
	{
		m_pWinNumber[uChairID] = TextureNumber::create("winnumber.png", "winnumberdot.png", "winnumberunit.png", true);
		m_pWinNumber[uChairID]->setVisible(false);
		m_pWinNumber[uChairID]->setCascadeOpacityEnabled(true);
		s_gameLayer->addChild(m_pWinNumber[uChairID], WinNumberZOrder);
	}
	else
	{
		m_pWinNumber[uChairID]->setOpacity(255);
		m_pWinNumber[uChairID]->setVisible(false);
	}
	m_pWinNumber[uChairID]->setPosition(s_ptUserViewDefaultCenter[uChairID]);
	m_pWinNumber[uChairID]->setPositionY(m_pWinNumber[uChairID]->getPositionY()+20);
	if (uChairID == 0 || uChairID == 8)
	{
		m_pWinNumber[uChairID]->setPositionY(s_ptUserViewDefaultCenter[uChairID].y - 73);
	}
	m_pWinNumber[uChairID]->setOpacity(0);
	return m_pWinNumber[uChairID];
}

void GameTable::PopChatBox()
{
    m_strNickName = "@" + m_strNickName;
    auto pChatBox = GetChatBox();
    pChatBox->SetEditText(m_strNickName);
    pChatBox->setVisible(true);
    m_pUserInfoView->setVisible(false);
}

ChatBox *GameTable::GetChatBox()
{
	if (nullptr == m_pChatBox)
	{
		m_pChatBox = ChatBox::create();
		m_pChatBox->setPosition(-1, 5);
		addChild(m_pChatBox, ChatBoxZOrder);
	}
	return m_pChatBox;
}

TableTip *GameTable::GetTableTip()
{
	if (m_pTableTip == nullptr)
	{
		m_pTableTip = TableTip::create("tabletip/bg.png", "fzlt.ttf", 25, 500);
		auto szTip = m_pTableTip->getContentSize();
		auto sz = getContentSize();
		m_pTableTip->setPosition((sz.width - szTip.width) / 2, sz.height - szTip.height);
		addChild(m_pTableTip, TableTipZOrder);
		m_pTableTip->setTouchEnabled(false) ;
	}
	return m_pTableTip;
}

void GameTable::PopUserInfo(unsigned uChairID)
{
	m_vecLayer.clear() ;
	m_CurTouchChairId = uChairID ;

	if (m_pUserInfoView == nullptr)
	{
		m_pUserInfoView = UserInfoView::create();
		//m_pUserInfoView->setPosition(202, 36);
		m_pUserInfoView -> ignoreAnchorPointForPosition(false);
		m_pUserInfoView -> setAnchorPoint(Point(0.5,0.5));
		m_pUserInfoView->setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
		addChild(m_pUserInfoView, UserInfoZOrder);
	}
	else
	{
		m_pUserInfoView->setVisible(true);
	}	
	//m_pUserInfoView->setPosition(GAME_WIDTH/2,GAME_HEIGHT/2);
	m_pUserInfoView->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
	m_pUserInfoView->setMoveUp(false) ;

	Json::Value param;
	param["tuid"] = m_usrInfo[uChairID].m_uUID;
	SendClientRequest(ClientRequest::kGetTableUserDetailInfo, param);
	return;

}

void GameTable::UpdateWaitPlayerNum(unsigned uWaitNum)
{
	if (g_globalTableConfig.GetTableMode() == TableLooking)
	{
		//checkSitdown();
		//if (m_pMenuItemSitDown->isVisible())
		//{
		//	m_pMenuItemSitDown->setVisible(false);
		//	m_pMenuItemAutoSitDown->setVisible(true);
		//}
		if (m_pMenuItemAutoSitDown->isVisible())
		{
			char szTxt[128] = {0};
			sprintf(szTxt, "已有(%d人)等待", uWaitNum);
			//SetLabelString(m_pLabelWaitPlayerCount, szTxt);
			setUITextString(m_pLabelWaitPlayerCount, szTxt);
			//m_pMenuItemAutoSitDown->setVisible(false);
			//m_pLabelWaitSitDown->setVisible(true);
			//m_pAniWaitSitDown->setVisible(true);
		}
	}
}

void GameTable::DryAshTwoMinCard(unsigned uChairId, unsigned char *handCard)
{
    CGameLogic gameLogic;
    unsigned char cbMaxCardType[5];
    if (m_vecPublicCardData.size() == 5)
    {
        gameLogic.FiveFromSeven(handCard, 2, &m_vecPublicCardData[0], 5, cbMaxCardType, 5);
    }
    else
    {
        return;
    }
	auto userView = GetUserView(uChairId);
	if (userView && userView->GetActivateFlag() == US_WAITNEXTGAME)
	{
		return;
	}
    for (int i=0; i<2; i++)
    {
        bool bHandSame = false;
        for (int j=0; j<5; j++)
        {
            if (handCard[i] == cbMaxCardType[j])
            {
                bHandSame = true;
            }
        }
        if (!bHandSame)
        {
            userView->DryCashMinHandCard(i);
        }
    }

    for (int i=0; i<5; i++)
    {
        bool bPublicSame = false;
        for (int j=0; j<5; j++)
        {
            if (m_vecPublicCardData[i] == cbMaxCardType[j])
            {
                bPublicSame = true;
            }
        }
        if (!bPublicSame)
        {
			m_pSpritePublicCard[i]->setPosition(s_ptPublicCard[i].x, s_ptPublicCard[i].y);
            m_pSpritePublicCardCash[i]->setVisible(true);
        }
        else
        {
            m_pSpritePublicCard[i]->setPosition(s_ptPublicCard[i].x, s_ptPublicCard[i].y + 12);
            m_pSpritePublicCardCash[i]->setVisible(false);
        }
    }
}

void GameTable::ReSetPublicCardPos(unsigned dispatchCount)
{
    for (int i=0; i<dispatchCount; i++)
    {
        m_pSpritePublicCard[i]->setPosition(s_ptPublicCard[i]);
        m_pSpritePublicCardCash[i]->setVisible(false);
    }    
}

void GameTable::GetShowFamily()
{
	//if(m_pShowFamily == nullptr)
	//{
	//	m_pShowFamily = ShowFamily::create();
	//	m_pShowFamily->setVisible(false);
	//	m_pShowFamily->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	//	s_gameLayer->addChild(m_pShowFamily, ShowFamilyZOrder);
	//}
	//m_pShowFamily->setVisible(true);
}

Node* GameTable::GetGameHallNode()
{
	return s_gameLayer;
}

void GameTable::showTargetFamily(bool useZoom,bool flag)
{



}

void GameTable::callUpdateTargetFamilyInfo()
{
}

int GameTable::checkSitdown()
{
	if (getTableType() == SngTable)
	{
		return 0;
	}
	if (getTableType() == MttTable)
	{
		return 0;
	}


	int retval = 0;
	if (g_globalTableConfig.GetTableMode() != TableLooking)
		return retval;

	if (getTableType() == NormalTable)
	{
		if (g_globalTableConfig.m_reason==1)
		{
			retval = 1;
			m_pMenuItemSitDown->setVisible(false);
			m_pMenuItemAutoSitDown->setVisible(false);
			m_pAniWaitSitDown->setVisible(false);
			m_pLabelWaitSitDown->setVisible(false);
			return retval;
		}
	}
	


	if (g_globalMyData.GetCash() < g_globalTableConfig.m_enterMoney) {
		// 钱币不足，不能坐下
		retval = 1;
		m_pMenuItemSitDown->setVisible(false);
		m_pMenuItemAutoSitDown->setVisible(false);
		m_pAniWaitSitDown->setVisible(false);
		m_pLabelWaitSitDown->setVisible(false);
	}
	else if (g_globalTableConfig.m_vecPlayerInfo.size() < g_globalTableConfig.m_uMaxPlayer) {
		// 人数小于最大人数，坐下
		retval = 2;
		m_pMenuItemSitDown->setVisible(true);
		m_pMenuItemAutoSitDown->setVisible(false);
		m_pAniWaitSitDown->setVisible(false);
		m_pLabelWaitSitDown->setVisible(false);
	}
	else if (g_globalTableConfig.m_vecPlayerInfo.size() == g_globalTableConfig.m_uMaxPlayer) {
		// 人数已满自动坐下
		retval = 3;
        if (m_IsWaitingSitDown)
        {
            return retval;
        }
		m_pMenuItemSitDown->setVisible(false);
		m_pMenuItemAutoSitDown->setVisible(true);
		m_pAniWaitSitDown->setVisible(false);
		m_pLabelWaitSitDown->setVisible(false);
	}
	return retval;
}

void GameTable::SetWaitSitDownStatus(bool IsWait)
{
    m_IsWaitingSitDown = IsWait;
}

void GameTable::OnCancleHideQuickShopUI(bool bVisible)
{
	if(m_pQuickShop != nullptr)
		m_pQuickShop->OnHideUi(bVisible);
}

void GameTable::ResetTable()
{
	
	CleanTable();
	for (auto& view : m_pUsers) {
		if (nullptr != view)
			view->reset();
	}

	for (int uChairID = 0; uChairID < GAME_PLAYER; uChairID++) {
		if (nullptr != m_pSpriteDispatchingCard[uChairID*2])		
			m_pSpriteDispatchingCard[uChairID*2]->setVisible(false);

		if (nullptr != m_pSpriteDispatchingCard[uChairID*2+1])	
			m_pSpriteDispatchingCard[uChairID*2+1]->setVisible(false);
	}

	if (nullptr != m_pSpriteDFlag) {
		m_pSpriteDFlag->setVisible(false);
	}

	m_videoPlayMgr.SetVisible(false);

	setUITextString(m_pLabelTableDesc, g_globalTableConfig.m_strRoomName) ;
	m_pMenuItemPopChat->setVisible(true);
	GetChatBox()->resetAt();
	resetTimeOve() ;
	showGameAutoTrust(false) ;
	reSetScorlllTips() ;
	m_curPlayerCount = 0 ;
	m_text_sngNeedRoleTips->setVisible(false) ;

	for (int i = 0; i < 9; i++)
	{
		if (i>=2&&m_sprtieRankView[i]!=nullptr)
		{
			m_sprtieRankView[i]->setVisible(false) ;
		
		}
			spriteRank[i]->setVisible(false) ;
	}

	//显示聊天
	m_pMenuItemPopChat->setVisible(true);
	showSngRankView(false);
	
	
	//if (getTableType() == MttTable && !g_globaMttGameInfo.m_isGameStart)
	//{
	//	m_pMenuItemPopChat->setVisible(false);
	//}
}

void GameTable::PlayEffectSound2(const std::string& strSound, bool bLoop)
{
	if (isVisible()) {
		PlayEffectSound(strSound, bLoop);
	}
}

void GameTable::SetHandCard(unsigned uChairID, int status)
{
	auto pUserView = GetUserView(uChairID);
	if (nullptr == pUserView || false == pUserView->isVisible())
		return;

	if (status != 0)
	{
		auto spCard1 = GetSpriteDispatchingCard(0, uChairID);
		if (spCard1)
		{		
			spCard1->setOpacity(255);
			spCard1->setVisible(true);
			spCard1->setPosition(s_ptDispatchedCard[0 * GAME_PLAYER + uChairID]);
		}

		auto spCard2 = GetSpriteDispatchingCard(1, uChairID);
		if (spCard2)
		{		
			spCard2->setOpacity(255);
			spCard2->setVisible(true);
			spCard2->setPosition(s_ptDispatchedCard[1 * GAME_PLAYER + uChairID]);
		}
	}
}

void GameTable::SetActiveStatus(unsigned uChairID, unsigned status)
{
	auto pUserView = GetUserView(uChairID);
	if (nullptr != pUserView) {
		pUserView->ActivateStatus(status);
	}
}

void GameTable::setTableType(TableType type)
{
	if (m_curTableType == type)
	{
		return ;
	}
	m_preTableType = m_curTableType ;

	m_curTableType = type ;

	

	//清理台桌背景
	if (m_pNormalSpriteBG!=nullptr)
	{
		m_pNormalSpriteBG->removeFromParentAndCleanup(true) ;
		m_pNormalSpriteBG = nullptr ;
		
	}
	if (m_sprite_tableLeft != nullptr )
	{
		m_sprite_tableLeft->removeFromParentAndCleanup(true) ;
		m_sprite_tableLeft = nullptr ;
	}
	if (m_sprite_tableRight != nullptr )
	{
		m_sprite_tableRight->removeFromParentAndCleanup(true) ;
		m_sprite_tableRight = nullptr ;
	}

	if (m_spritetableTitle!=nullptr)
	{
		m_spritetableTitle->removeFromParentAndCleanup(true) ;
		m_spritetableTitle = nullptr ;
	}
	m_layerRightUpNormal->setVisible(false) ; 
	m_pMenuItemPopMenu->setVisible(false) ;
	//sng相关隐藏
	m_layerRightUpSng->setVisible(false) ;
	m_sngReturn->setVisible(false) ;
	//mtt相关按钮隐藏
	m_bt_mtt_rebuy->setVisible(false) ;
	m_bt_mtt_addon->setVisible(false) ;
	m_layer_mtt_time->setVisible(false) ;
	m_text_mttRankF->setVisible(false) ;
	m_text_mttRankAfter->setVisible(false) ;
	m_text_sngRank->setVisible(false) ;
	m_text_sngRankbg->setVisible(false) ;

	//私密房按钮隐藏
	m_layout_PrivateTable->setVisible(false) ;
	m_image_Private_roomInfo->setVisible(false) ;
	m_bt_Private_RoomInfo->setVisible(false) ;
	m_layout_Private_btBox->setVisible(false) ;
	m_bt_Private_startOrEndGame->setVisible(false) ;
	
	m_pMenuItemReward->setVisible(true) ;


	//初始化新的背景
	if (m_curTableType == TableType::NormalTable)
	{
		m_videoPlayMgr.SetVisible(true);
		m_layerRightUpNormal->setVisible(true) ;
		m_pMenuItemPopMenu->setVisible(true) ;
		if (m_spritetableTitle!=nullptr)
		{
			m_spritetableTitle->setVisible(false) ;
		}
		m_pNormalSpriteBG = Sprite::create("bgtable.png");
		m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

		m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
		addChild(m_pNormalSpriteBG,-1);

		if (m_sprite_tableLeft == nullptr )
		{
			m_sprite_tableLeft = Sprite::create("desk_normal.png");
			m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
		}
		if (m_sprite_tableRight == nullptr )
		{
			m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
			m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
		}

		m_img_tableLeft->addChild(m_sprite_tableLeft);
		m_img_tableRight->addChild(m_sprite_tableRight) ;
		//m_spritetableTitle->setVisible(false) ;
	}
	else if (m_curTableType == TableType::SngTable)
	{
		m_text_sngRank->setVisible(true) ;
		m_text_sngRankbg->setVisible(true) ;

		m_videoPlayMgr.SetVisible(false);
		m_layerRightUpSng->setVisible(true) ;
		m_sngReturn->setVisible(true) ;

		m_pNormalSpriteBG = Sprite::create("bgtable_sng.png");
		m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

		m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
		addChild(m_pNormalSpriteBG,-1);

		if (m_sprite_tableLeft == nullptr )
		{
			m_sprite_tableLeft = Sprite::create("desk_sng.png");
			m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
		}
		if (m_sprite_tableRight == nullptr )
		{
			m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
			m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
		}
		
		m_img_tableLeft->addChild(m_sprite_tableLeft);
		m_img_tableRight->addChild(m_sprite_tableRight) ;


		if (m_spritetableTitle==nullptr)
		{
			m_spritetableTitle = Sprite::create("sng_title.png");
			m_imgtitle->addChild(m_spritetableTitle) ;
		}
		m_spritetableTitle->setVisible(true) ;

	}
	else if (m_curTableType == TableType::MttTable )
	{
		
		if (!g_globaMttGameInfo.m_isGameStart)
		{
			//关闭聊天 打赏 排行
			m_pMenuItemPopChat->setVisible(false) ;
			m_pMenuItemReward->setVisible(false) ;
			m_layerRightUpSng->setVisible(true) ;
		}
		else
		{
			updateMttButtonShow() ;
		}
		m_text_mttRankF->setVisible(true) ;
		m_text_mttRankAfter->setVisible(true) ;
		m_videoPlayMgr.SetVisible(false);
		m_sngReturn->setVisible(true) ;

		m_pNormalSpriteBG = Sprite::create("bgtable_mtt.png");
		m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

		m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
		addChild(m_pNormalSpriteBG,-1);

		if (m_sprite_tableLeft == nullptr )
		{
			m_sprite_tableLeft = Sprite::create("desk_mtt.png");
			m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
		}
		if (m_sprite_tableRight == nullptr )
		{
			m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
			m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
		}

		m_img_tableLeft->addChild(m_sprite_tableLeft);
		m_img_tableRight->addChild(m_sprite_tableRight) ;

		if (m_spritetableTitle==nullptr)
		{
			m_spritetableTitle = Sprite::create("mtt_title.png");
			//this->addChild(m_spritetableTitle,50) ;

			m_imgtitle->addChild(m_spritetableTitle) ;
		}
		m_spritetableTitle->setVisible(true) ;
			
	}

	//私密房
	else if ( m_curTableType == PrivateTable)
	{
		m_layout_PrivateTable->setVisible(true) ;
		m_layout_Private_btBox->setVisible(false) ;
		//m_bt_Private_RoomInfo->setVisible(true) ;
	
		m_pMenuItemPopMenu->setVisible(true) ;

		m_bt_Private_startOrEndGame->setVisible(true) ;


	/*	if (m_globaPrivateRoomInfo.m_isOwner)
		{*/
			m_image_Private_roomInfo->setVisible(true) ;
			m_bt_Private_RoomInfo->setVisible(true) ;
		//}


		m_pNormalSpriteBG = Sprite::create("bgtable_private.png");
		m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

		m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
		addChild(m_pNormalSpriteBG,-1);

		if (m_sprite_tableLeft == nullptr )
		{
			m_sprite_tableLeft = Sprite::create("desk_private.png");
			m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
		}
		if (m_sprite_tableRight == nullptr )
		{
			m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
			m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
		}

		m_img_tableLeft->addChild(m_sprite_tableLeft);
		m_img_tableRight->addChild(m_sprite_tableRight) ;

		if (m_spritetableTitle==nullptr)
		{
			m_spritetableTitle = Sprite::create("private_title.png");
			m_imgtitle->addChild(m_spritetableTitle) ;
		}

	}

}

TableType GameTable::getTableType()
{
	return  m_curTableType ;
}

void GameTable::initSngTable()
{
	m_sngMatchState = nullptr ;
	m_sngRankView =nullptr ;
	m_curLineLabel = nullptr ;
	m_curPlayerCount = 0 ;
	
	for (int i = 0; i < 9; i++)
	{
		m_sprtieRankView[i] = nullptr ;
		if (i>=2)
		{
			char buff[64] ;
			sprintf(buff,"gamesng/rank/sngrank%d.png",i) ;
			m_sprtieRankView[i] = Sprite::create(buff);

			s_gameLayer->addChild(m_sprtieRankView[i],SngRank) ;
			m_sprtieRankView[i]->setVisible(false) ;


		}
		spriteRank[i] = Sprite::create("gamesng/rank/sngrank3.png") ;

		s_gameLayer->addChild(spriteRank[i],SngRank) ;
		spriteRank[i]->setVisible(false) ;
		
	}


	//返回
	m_sngReturn = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"Button_sngreturn"));
	m_sngReturn->addTouchEventListener(this,toucheventselector(GameTable::onSngMenuItemClicked) ) ;
	m_sngReturn->setVisible(true);


	//sng 排名
	m_buttonSngRank = static_cast<Button*>(Tools::getWidgetByName(m_layerRightUpSng,"Button_sngrank"));
	m_buttonSngRank->addTouchEventListener(this,toucheventselector(GameTable::onSngMenuItemClicked) ) ;
	m_buttonSngRank->setVisible(true);

	m_text_sngRank =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_sngrank"));
	m_text_sngRankbg =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_sngrankbg"));
	//托管
	isSngAutoMove = false ;
	isSngAutoMoveFoever = false ;
	m_sngAutoLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_sngauto"));
	m_sngAutoLayer->setVisible(false) ;
	m_buttomSngAuto = static_cast<Button*>(Tools::getWidgetByName(m_sngAutoLayer,"Button_sngauto"));
	m_buttomSngAuto->addTouchEventListener(this,toucheventselector(GameTable::onSngMenuItemClicked) ) ;
	//滚动提示
	m_text_sngScrollTips =  static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_sng_scrollwtext"));
	m_MaxScrollTextWidth = m_text_sngScrollTips->getContentSize().width;

	CCDrawNode* shap = CCDrawNode::create();
	CCPoint point[4] = {ccp(0,0), ccp(m_text_sngScrollTips->getContentSize().width, 0), 
		
		ccp(m_text_sngScrollTips->getContentSize().width, m_text_sngScrollTips->getContentSize().height), ccp(0, m_text_sngScrollTips->getContentSize().height)};
	shap->drawPolygon(point, 4, ccc4f(355, 255, 255, 255), 2, ccc4f(255, 255, 255, 255));

	m_cliper = ClippingNode::create();
	m_cliper->setStencil(shap);
	m_cliper->setAnchorPoint(ccp(0.5, 0.5));
	//m_cliper->setPosition(ccp(0, 0));
	m_cliper->setPosition(Point(m_text_sngScrollTips->getPosition().x-m_text_sngScrollTips->getContentSize().width/2,
		
		m_text_sngScrollTips->getPosition().y-m_text_sngScrollTips->getContentSize().height/2
		
		));
	s_gameLayer->addChild(m_cliper,GameTipsZOrder);


	//需要人的提示
	m_text_sngNeedRoleTips =  static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_sng_lastroleTips"));
	m_text_curSngRoleCount =  static_cast<Text*>(Tools::getWidgetByName(m_text_sngNeedRoleTips,"Text_sngcurrole"));
	m_text_sngNeedRoleTips->setVisible(false) ;

	m_spriteSngAuto = Sprite::create("gamesng/sngauto.png") ;
	m_spriteSngAuto->setPosition(Vec2(112 / 2, 164 / 2));
	s_gameLayer->addChild(m_spriteSngAuto,SngAuto) ;
	m_spriteSngAuto->setVisible(false) ;




	showGameAutoTrust(false);
}


void GameTable::initMttTable()
{
	m_mttAddonTips = nullptr ;
	m_mttRebuyTips = nullptr ;
	m_mttRebuyTipsBySercer = nullptr ;
	//cocos2d::ui::Button*  m_bt_mtt_rebuy ;
	//cocos2d::ui::Button*  m_bt_mtt_addon ;
	//cocos2d::ui::Layout*  m_layer_mtt_time ;
	//cocos2d::ui::Text*	  m_text_mtttime[4] ;
	//cocos2d::ui::Text*	  m_text_mttRankF ;
	//cocos2d::ui::Text*	  m_text_mttRankAfter ;
	//sng 排名

	m_text_mttRankF =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_mttrank_f"));
	m_text_mttRankAfter =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_mttrank_after"));

	m_layer_mtt_time = static_cast<Layout*>(Tools::getWidgetByName(m_layer_GameTable,"Panel_mttdaojishi"));
	m_layer_mtt_time->setVisible(false) ;

	m_bt_mtt_rebuy = static_cast<Button*>(Tools::getWidgetByName(m_layer_GameTable,"gt_mtt_rebuy"));
	m_bt_mtt_rebuy->addTouchEventListener(this,toucheventselector(GameTable::onMttMenuItemClicked) ) ;
	m_bt_mtt_rebuy->setVisible(true);

	m_bt_mtt_addon = static_cast<Button*>(Tools::getWidgetByName(m_layer_GameTable,"gt_mtt_addon"));
	m_bt_mtt_addon->addTouchEventListener(this,toucheventselector(GameTable::onMttMenuItemClicked) ) ;
	m_bt_mtt_addon->setVisible(true);
	for (int i=0;i<4;i++)
	{
		char buff[64] ;
		sprintf(buff,"Text_mtt_time%d",i) ;
		m_text_mtttime[i] = static_cast<Text*>(Tools::getWidgetByName(m_layer_mtt_time,buff));
	}
}

void GameTable::showSngMatchState(bool flag)
{


	if (nullptr == m_sngMatchState)
	{
		m_sngMatchState = SngMatchState::create();
		m_sngMatchState -> ignoreAnchorPointForPosition(false);
		m_sngMatchState -> setAnchorPoint(Point(0.5,0.5));

	
		addChild(m_sngMatchState, QuickShopZOrder);
	}
		m_sngMatchState->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
	if (flag)
	{
			m_sngMatchState->updateData() ;
			if (m_pUserInfoView!=nullptr)
			{
				m_pUserInfoView->setVisible(false) ;
			}
	}

	m_sngMatchState->setVisible(flag);

}

/************************************************************************/
/* 更新当前桌面排名                                                                     */
/************************************************************************/
void GameTable::updateSngTableRankTips()
{
	
	char buff[64] ;
	sprintf(buff,"%d",g_globalSngTableInfo.m_rank) ;
	setUITextString(m_text_sngRank,buff) ;
}


/************************************************************************/
/* 玩家输了后显示排名                                                                     */
/************************************************************************/
void GameTable::callShowSngRankInFailed(unsigned uChairID,unsigned rankid)
{
	if (uChairID<0)
	{
		return ;
	}

	if (g_globalSngTableInfo.m_playerCount==6)
	{
		if (rankid<=2)
		{
			return ;
		}
	}
	if (g_globalSngTableInfo.m_playerCount==9)
	{
		if (rankid<=3)
		{
			return ;
		}
	}

	unsigned id = g_globalTableConfig.GetViewChairID(uChairID);
	auto pUserView = GetUserView(id);
	if (nullptr != pUserView) {

		//pUserView->showSngRank(true ,m_sprtieRankView[rankid-1]->getTexture()) ;
		spriteRank[id]->setTexture(m_sprtieRankView[rankid-1]->getTexture()) ;
		spriteRank[id]->setPosition(pUserView->getPosition()) ;
		spriteRank[id]->setVisible(true) ;
	}


	//m_sprtieRankView[rankid-1]->setPosition(pUserView->getPosition()) ;
	//m_sprtieRankView[rankid-1]->setVisible(true) ;


}


/************************************************************************/
/* sng当前结算                                                                     */
/************************************************************************/
void GameTable::showSngRankView(bool flag ,int rankId /* =0 */,int cash/* =0 */,int score /* = 0 */)
{
	if (m_sngRankView == nullptr)
	{
		m_sngRankView = SngRankView::create();
		m_sngRankView -> ignoreAnchorPointForPosition(false);
		m_sngRankView -> setAnchorPoint(Point(0.5,0.5));

		m_sngRankView->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_sngRankView, QuickShopZOrder);
	}

	m_sngRankView->setVisible(flag) ;
	if (flag)
	{
		showGameAutoTrust(false) ;//胜利失败时候关闭界面
		m_sngRankView->updateSngRankView(g_globalSngTableInfo.m_playerCount, rankId,cash,score) ;
	}

}

/************************************************************************/
/* 更新牌桌盲注信息                                                                     */
/************************************************************************/
void GameTable::updateMatchBindTipsInTable()
{
	setUITextString(m_pLabelTableDesc, g_globalTableConfig.m_strRoomName.c_str()) ;
	
}
/************************************************************************/
/* 更新ui中盲注                                                                     */
/************************************************************************/
void GameTable::updateSngUIBlind()
{

	if (m_sngMatchState!=nullptr)
	{
			m_sngMatchState->updateBlindData() ;
	}

}
/************************************************************************/
/* 刷新sng 排行                                                               */
/************************************************************************/
void GameTable::updateSngRankList()
{

	if (m_sngMatchState!=nullptr)
	{
		m_sngMatchState->updateSngMatchRankList();
	}
}



void GameTable::cleanRankShow()
{
	for (int i = 0; i < 9; i++)
	{
		if (m_pUsers[i]!=NULL)
		{
			//m_pUsers[i]->setRankViewShow(false,nullptr) ;
		}
	}
}
/************************************************************************/
/* sng中托管逻辑                                                                     */
/************************************************************************/
void GameTable::onScheduleSngFoldCard(float fDelta)
{
	if (m_pActionBar->CanCheckNow())
	{
		Json::Value msg;
		msg["betJettonNum"] = 0;
		msg["betType"] = 4;
		msg["mangTag"] = 0;
		SendClientRequest(ClientRequest::kAddScore, msg);
	}
	else
	{
		Json::Value param;
		param["strTip"] = 1;
		SendClientRequest(ClientRequest::kGiveUp, param);
	}
}
/************************************************************************/
/* 显示自动托管                                                                     */
/************************************************************************/
void GameTable::showGameAutoTrust(bool flag)
{
	m_sngAutoLayer->setVisible(flag) ;
	auto muView = GetUserView(MY_CHAIR_ID);
	if (muView)
	{
			m_spriteSngAuto->setPosition(muView->getPosition()) ;
	}
	m_spriteSngAuto->setVisible(flag) ;

	m_autoTrust = flag ;
}
/************************************************************************/
/* 通知sng结束        
旁观者 直接弹退出提示
如果正显示排行  关闭排行后自动弹出
*/
/************************************************************************/
void GameTable::showSngGameOver(bool flag)
{

	if (m_sngRankView->isVisible())
	{
		return ;
	}
	showGameAutoTrust(false) ;
	PopTipWithBt1("游戏结束，请返回大厅", "确定", [this](unsigned uBt){
		if (uBt == 0)
		{
			SendClientRequest(ClientRequest::kCallLevelTable) ;
		}
	});

}
/************************************************************************/
/* 重制托管计数器                                                                     */
/************************************************************************/
void GameTable::resetTimeOve()
{
	m_uOverTime =0 ; 
	isSngAutoMove = false ;
	isSngAutoMoveFoever = false ;
	unschedule(schedule_selector(GameTable::onScheduleSngFoldCard));
}

/************************************************************************/
/* 通知下一个滚动提示                                                                     */
/************************************************************************/
void GameTable::callNextScrollText()
{
	if (!GameData::getInstance()->hasMoveLable())
	{
		m_text_sngScrollTips->setVisible(false) ;
		return ;
	}
	if (!m_isScrollTextMove)
	{
		m_text_sngScrollTips->setVisible(true) ;
		m_curLineLabel = GameData::getInstance()->getOneMoveLable() ;
		m_curLineLabel->setPositionX(m_MaxScrollTextWidth+m_curLineLabel->getContentSize().width/2+20) ;
		m_curLineLabel->setPositionY(m_text_sngScrollTips->getContentSize().height/2) ;
		m_cliper->addChild(m_curLineLabel) ;
		GameData::getInstance()->removeGameTips(m_curLineLabel) ;
		m_curLineLabel->setVisible(true) ;
		m_isScrollTextMove = true ;
		m_curScrollTextWidth = m_curLineLabel->getContentSize().width ;
		auto action = [this](){
			m_isScrollTextMove = false ;
			m_cliper->removeChild(m_curLineLabel) ;
			m_curLineLabel = nullptr ;
			callNextScrollText() ;
		} ;

		ActionInterval* moveTo = MoveTo::create(10,Point( -m_curScrollTextWidth/2-20,m_curLineLabel->getPosition().y)) ;
		m_curLineLabel->runAction(Sequence::create(moveTo,CallFunc::create(action),NULL)) ;

	}
	
}
/************************************************************************/
/* 重置滚动提示         进入牌桌是将                                                             */
/************************************************************************/
void GameTable::reSetScorlllTips()
{
	if (m_curLineLabel!=nullptr)
	{
		m_curLineLabel->stopAllActions() ;
		m_cliper->removeChild(m_curLineLabel) ;
		
	}
	m_curLineLabel = nullptr ;
	m_isScrollTextMove = false ;
	m_text_sngScrollTips->setVisible(false) ;
	
}

/************************************************************************/
/* 更新sng人数
* 控制是否显示
/************************************************************************/
void GameTable::updateCurSngPlayerCount()
{

	if (getTableType() == SngTable)
	{
		if (g_globalSngTableInfo.m_isGameStart)
		{
			return ;
		}
		if (g_globalTableConfig.m_vecPlayerInfo.size()>=g_globalSngTableInfo.m_playerCount)
		{
			m_text_sngNeedRoleTips->setVisible(false) ;
			return ;
		}
		
		char buff[64] ;

		sprintf(buff,"%d",g_globalSngTableInfo.m_playerCount - g_globalTableConfig.m_vecPlayerInfo.size()) ;

		setUITextString(m_text_curSngRoleCount,buff) ;

		if (g_globalSngTableInfo.m_playerCount == g_globalTableConfig.m_vecPlayerInfo.size())
		{
			m_text_sngNeedRoleTips->setVisible(false) ;
		}
		else
		{
			m_text_sngNeedRoleTips->setVisible(true) ;
		}

	}

	if (g_globalSngTableInfo.m_playerCount <= 1)
	{
		if (m_pSpriteDFlag !=nullptr )
		{
			m_pSpriteDFlag->setVisible(false) ;
		}
		
	}

}

void GameTable::callSngExit()
{

	if (getTableType() == SngTable)
	{
		if (g_globalSngTableInfo.m_isSnglose||!g_globalSngTableInfo.m_isGameStart)
		{
			SendClientRequest(ClientRequest::kCallLevelTable) ;
		}
		else
		{
			PopTipWithBt2("比赛中途退出，您的报名费将不会返还，并且无法回来，是否确认退出?","退出比赛","取消",[this](unsigned ubt)
			{
				if(ubt == 0)
				{
					SendClientRequest(ClientRequest::kCallLevelTable) ;
				}
				if(ubt == 1)
				{
				}
			});
		}
	}else if (getTableType() == MttTable)
	{
		if (g_globaMttGameInfo.m_isMttlose)
		{
			SendClientRequest(ClientRequest::kCallLevelTable) ;
		}
		else
		{
			PopTipWithBt2("比赛中途退出，您的报名费将不会返还，并且无法回来，是否确认退出?","退出比赛","取消",[this](unsigned ubt)
			{
				if(ubt == 0)
				{
					SendClientRequest(ClientRequest::kCallLevelTable) ;
				}
				if(ubt == 1)
				{
				}
			});
		}
	}


}

void GameTable::initUsrExpress()
{
	//for (int i=0 ;i< GAME_PLAYER; i++)
	//{
	//	m_biaoqingAni[i][0] = AnimationManager::creatArmature("putongbiaoqing");
	//	m_biaoqingAni[i][1] = AnimationManager::creatArmature("guzubiaoqing");
	//	m_biaoqingAni[i][0]->setVisible(false);
	//	m_biaoqingAni[i][1]->setVisible(false);
	//	s_gameLayer->addChild(m_biaoqingAni[i][0],ExpressTools) ;
	//	s_gameLayer->addChild(m_biaoqingAni[i][1],ExpressTools) ;

	//}

	for (int i=0 ;i< GAME_PLAYER; i++)
	{
		m_biaoqingAni[i][0] = nullptr;
		m_biaoqingAni[i][1] = nullptr;
		//m_biaoqingAni[i][0]->setVisible(false);
		//m_biaoqingAni[i][1]->setVisible(false);
		//s_gameLayer->addChild(m_biaoqingAni[i][0],ExpressTools) ;
		//s_gameLayer->addChild(m_biaoqingAni[i][1],ExpressTools) ;

	}
}

bool GameTable::getOtherPlayerPos(unsigned &uChair, Vec2 &pos)
{
    unsigned tempChair = -1;
    for (int i=3; i>=0;i--)
    {
        if (m_pUsers[i] != nullptr && m_pUsers[i]->isVisible())
        {
            tempChair = i;
            break;
        }
    }
    for (int i=5; i<=8;i++)
    {
        if (m_pUsers[i] != nullptr && m_pUsers[i]->isVisible())
        {
            tempChair = i;
            break;
        }
    }

    if (tempChair == -1)
        return false;
    else
    {
        uChair = tempChair;

        if (tempChair<4)
        {
            pos.x = s_ptUserViewDefault[tempChair].x - 80;
        }
        else
        {
            pos.x = s_ptUserViewDefault[tempChair].x + 80;
        }
        pos.y = s_ptUserViewDefault[tempChair].y;
        return true;
    }
}

//好友按钮界面引导
void GameTable::createFriendGuide()
{
    if(GuideInfo->readGuideInfo(FriendListGuide))
        return ;

    auto guideTips = GuideTips::create("guidetips/friendtips.png", "guidetips/friendflag.png");
    auto pos = m_pMenuItemPopInvite->convertToWorldSpace(Vec2(m_pMenuItemPopInvite->getContentSize().width/2, m_pMenuItemPopInvite->getContentSize().height/2));
    guideTips->setFlagPosition(pos);
    guideTips->setBgPosition(Vec2(pos.x-160, pos.y-150));
    auto pClip = CreateClippingMask("guidetips/friendmask.png", pos, guideTips);
    this->addChild(pClip, QuickFriendGuide);
    guideTips->setTipsID(FriendListGuide);
    GameTipsManager::getInstance()->addTips(pClip);
}
//增加关注界面引导
void GameTable::createAttentionGuide()
{
    if(GuideInfo->readGuideInfo(AddAttentionGuide))
        return ;

    Vec2 pos;
    unsigned uChair = -1;
    if (getOtherPlayerPos(uChair, pos) && g_globalTableConfig.GetTableMode() == TablePlaying)
    {
        auto guideTips = GuideTips::create("guidetips/attentiontips.png", "guidetips/userbg.png");
        //
        auto arrow = Sprite::create("guidetips/attentionflag.png");
        arrow->setPosition(pos);
        if ((uChair < 4 && uChair != 0) || uChair == 8)
            arrow->setFlipX(true);
        guideTips->addChild(arrow);
        guideTips->setFlagPosition(s_ptUserViewDefault[uChair]);
        guideTips->setBgPosition(Vec2(getContentSize().width/2, getContentSize().height/2-20));

        auto pClip = CreateClippingMask("guidetips/userbg.png", s_ptUserViewDefault[uChair], guideTips);
        this->addChild(pClip, AttentionGuide);
        guideTips->setTipsID(AddAttentionGuide);
        GameTipsManager::getInstance()->addTips(pClip);
    }

}

/************************************************************************/
/* 时间 秒                                                                     */
/************************************************************************/
void GameTable::updateMttTime(int time)
{
	if (time ==0)
	{
		m_layer_mtt_time->stopAllActions() ;
		m_layer_mtt_time->setVisible(false) ;
		return ;
	}

	m_layer_mtt_time->stopAllActions() ;
	m_layer_mtt_time->setVisible(true) ;
	m_mttTime = time ;

	auto updateTimeAction = [this](){

		if (m_mttTime <0)
		{
			m_layer_mtt_time->stopAllActions() ;
			m_layer_mtt_time->setVisible(false) ;
			return ;
		}

		int i_min = m_mttTime/60%60;
		int i_sec = m_mttTime%60;

		m_mtttimeShow[0] = i_min/10 ;
		m_mtttimeShow[1] = i_min%10 ;

		m_mtttimeShow[2] = i_sec/10 ;
		m_mtttimeShow[3] = i_sec%10 ;

		for (int i=0; i<4; i++)
		{
			char buff[64];
			sprintf(buff,"%d",m_mtttimeShow[i]) ;
			setUITextString(m_text_mtttime[i],buff) ;
		}
		m_mttTime-=1;
	} ;
	CCFiniteTimeAction *repeat = CCRepeatForever::create(Sequence::create(CallFunc::create(updateTimeAction),DelayTime::create(1.0f),NULL));
	m_layer_mtt_time->runAction(repeat) ;
}



/************************************************************************/
/* MTT                                                                     */
/************************************************************************/
void GameTable::showMttAddonTips(bool flag)
{

	if (m_mttAddonTips == nullptr)
	{
		m_mttAddonTips = MttAddonTips::create();
		m_mttAddonTips -> ignoreAnchorPointForPosition(false);
		m_mttAddonTips -> setAnchorPoint(Point(0.5,0.5));

		m_mttAddonTips->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_mttAddonTips, MttAddonTipsZOrder);
	}
	if (flag)
	{
		m_mttAddonTips->updateMttAddonTips() ;
	}
	m_mttAddonTips->setVisible(flag) ;

}

void GameTable::showMttRebuyTips(bool flag)
{
	if (m_mttRebuyTips == nullptr)
	{
		m_mttRebuyTips = MttRebuyTips::create();
		m_mttRebuyTips -> ignoreAnchorPointForPosition(false);
		m_mttRebuyTips -> setAnchorPoint(Point(0.5,0.5));

		m_mttRebuyTips->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_mttRebuyTips, MttRebuyTipsZOrder);
	}
	if (flag)
	{
		m_mttRebuyTips->updateMttRebuyTips() ;
	}
	m_mttRebuyTips->setVisible(flag) ;
}

void GameTable::onMttMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (m_pUserInfoView!=nullptr)
		{
			m_pUserInfoView->setVisible(false) ;
		}
		if (m_bt_mtt_rebuy == pItem)
		{
			showMttRebuyTips(true) ;
		}
		else if (m_bt_mtt_addon == pItem)
		{
			showMttAddonTips(true) ;
		}
	}

}

void GameTable::onPrivateMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (m_bt_Private_touch == pItem)
		{
			m_layout_Private_btBox->setVisible(false) ;
		}

		else if (m_bt_Private_startOrEndGame == pItem)
		{
			if (m_globaPrivateRoomInfo.m_isGameState == 0)
			{
				Json::Value parm ;
				parm["con"] = 1 ;
				parm["roomid"] = m_globaPrivateRoomInfo.m_roomid ;
				SendClientRequest(ClientRequest::kCallPrivateGameController,parm);
			}
			else  if (m_globaPrivateRoomInfo.m_isGameState == 1)
			{
				PopTipWithBt2("确认结束牌局？点击确定后将在本局结束后解散该私密房！","确定","取消",[this](unsigned ubt)
				{
					if(ubt == 0)
					{
						Json::Value parm ;
						parm["con"] = 0 ;
						parm["roomid"] = m_globaPrivateRoomInfo.m_roomid ;
						SendClientRequest(ClientRequest::kCallPrivateGameController,parm);
					}
				});
				//parm["con"] = 0 ;
			}
			//parm["roomid"] = m_globaPrivateRoomInfo.m_roomid ;
			//SendClientRequest(ClientRequest::kCallPrivateGameController,parm);
		}
		else if (pItem == m_bt_Private_Box)
		{
			//m_layout_Private_btBox->setVisible(true) ;
			if (m_pQuickInviteList == nullptr)
			{
				m_pQuickInviteList = QuickInviteList::create();
				m_pQuickInviteList -> ignoreAnchorPointForPosition(false);
				m_pQuickInviteList -> setAnchorPoint(Point(1,1));
				m_pQuickInviteList->setPosition(Director::getInstance()->getVisibleSize().width,
					m_pMenuItemPopInvite->getPositionY()-m_pMenuItemPopInvite->getContentSize().height/2-5);
				//m_pQuickInviteList->setVisible(false);

				addChild(m_pQuickInviteList, QuickInviteListZOrder);
			}
			if (m_bIsReceiveInvite)
			{
				m_pQuickInviteList->ShowInviteFlag(true);
				m_bIsReceiveInvite = false;
			}

			g_globalQuickFollowInfo.m_bIsReceiveInvite = false;
			m_pSpriteInviteFlag->setVisible(false);
			m_pQuickInviteList->setVisible(true);

		}
		else if (pItem == m_bt_Private_InvateFriends)
		{
			m_layout_Private_btBox->setVisible(false) ;
			if (m_pQuickInviteList == nullptr)
			{
				m_pQuickInviteList = QuickInviteList::create();
				m_pQuickInviteList -> ignoreAnchorPointForPosition(false);
				m_pQuickInviteList -> setAnchorPoint(Point(1,1));
				m_pQuickInviteList->setPosition(Director::getInstance()->getVisibleSize().width,
					m_pMenuItemPopInvite->getPositionY()-m_pMenuItemPopInvite->getContentSize().height/2-5);
				//m_pQuickInviteList->setVisible(false);

				addChild(m_pQuickInviteList, QuickInviteListZOrder);
			}
			if (m_bIsReceiveInvite)
			{
				m_pQuickInviteList->ShowInviteFlag(true);
				m_bIsReceiveInvite = false;
			}

			g_globalQuickFollowInfo.m_bIsReceiveInvite = false;
			m_pSpriteInviteFlag->setVisible(false);
			m_pQuickInviteList->setVisible(true);


		}
		else if (pItem == m_bt_Private_Share)
		{
		}
		else if (pItem == m_bt_Private_RoomInfo)
		{
			showPrivateRoomInfoLayer(true) ;
		}
		else if (pItem == m_bt_Private_task)
		{
			m_vecLayer.clear() ;
			SendClientRequest(ClientRequest::kShowNewPlayerTask);
		}
		else if (pItem == m_bt_Private_shop)
		{
			m_vecLayer.clear() ;
			if (nullptr == m_pQuickShop)
			{
				m_pQuickShop = QuickShop::create();
				m_pQuickShop -> ignoreAnchorPointForPosition(false);
				m_pQuickShop -> setAnchorPoint(Point(0.5,0.5));


				addChild(m_pQuickShop, QuickShopZOrder);
			}
			m_pQuickShop->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
			m_pQuickShop->UpdateJetton(g_globalMyData.GetCash(), g_globalTableConfig.m_dAutoSupplyMoney, g_globalTableConfig.m_dMinBringMoney, g_globalTableConfig.m_dMaxBringMoney, g_globalTableConfig.m_uAutoSupplyToMaxMoney);
			m_pQuickShop->setVisible(true);
		}


		

	}
}


void GameTable::updateMttButtonShow()
{
	m_bt_mtt_rebuy->setVisible(false);
	m_bt_mtt_addon->setVisible(false);
	if (!g_globaMttGameInfo.m_showMttbutton)
	{
		return ;
	}
	if(getTableType()!=MttTable)
	{
		return ;
	}

	if (!g_globaMttGameInfo.m_isGameStart)
	{
		m_bt_mtt_rebuy->setVisible(false);
		m_bt_mtt_addon->setVisible(false);
		m_pMenuItemReward->setVisible(false) ;
		m_pMenuItemAutoSitDown->setVisible(false);
		return ;
	}

	//旁观状态不显示按钮
	if (g_globalTableConfig.GetTableMode() == TableLooking)
	{
		m_bt_mtt_rebuy->setVisible(false);
		m_bt_mtt_addon->setVisible(false);
		return;
	}


	m_layer_mtt_time->setVisible(false) ; //关闭倒计时界面
	

	int rebuy_levmin = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyMin ;
	int rebuy_levmax = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyMax ;
	int rebuyCount = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyCount ;


	int addon_levmin = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnLevelMin ;
	int addon_levmax = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnLevelMax ;
	int addonCount = g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnCount ;

	int curRound = g_globaMttGameInfo.m_mangzhuIndex ;


	if (curRound>=rebuy_levmin && curRound<=rebuy_levmax &&rebuyCount>0  &&g_globaMttGameInfo.m_rebuyCount>0)
	{
		m_bt_mtt_rebuy->setVisible(true);
	}
	else
	{
		m_bt_mtt_rebuy->setVisible(false);
	}
	//优先显示重购
	if (!m_bt_mtt_rebuy->isVisible())
	{
		if (curRound>=addon_levmin && curRound<=addon_levmax &&addonCount>0  &&g_globaMttGameInfo.m_addonCount>0)
		{
			m_bt_mtt_addon->setVisible(true);
		}
		else
		{
			m_bt_mtt_addon->setVisible(false);
		}
	}
	

}

void GameTable::updateMttRankInTable(bool flag)
{
	//if (flag)
	//{
		char buff[64] ;
		sprintf(buff,"%d",g_globaMttGameInfo.m_rank) ;
		setUITextString(m_text_mttRankF,buff) ;
		sprintf(buff,"/%d",g_globaMttGameInfo.m_playerCount) ;
		setUITextString(m_text_mttRankAfter,buff) ;
	//}

	//m_text_mttRankF->setVisible(flag) ;
	//m_text_mttRankAfter->setVisible(flag) ;
	//m_layerRightUpSng->setVisible(flag) ;
}
/************************************************************************/
/* 比赛结束后排名显示                                                                     */
/************************************************************************/
void GameTable::showMttRankView(bool flag,unsigned uRankId/* =0 */,std::string desc,std::string icon/* ="" */,std::string name/* ="" */,unsigned cash/* =0 */,unsigned score/* =0 */)
{
    if (m_MTTRankView == nullptr)
    {
        m_MTTRankView = MttRankView::create();
        auto rankViewSz = m_MTTRankView->getContentSize();
        m_MTTRankView->setPosition((GameData::getInstance()->getGameWidth()-rankViewSz.width)/2,(GameData::getInstance()->getGameHeight()-rankViewSz.height)/2);
		this->addChild(m_MTTRankView,QuickShopZOrder) ;
	
	}
	if (flag)
	{
		m_layerRightUpSng->setVisible(false) ;
		showGameAutoTrust(false) ;//胜利失败时候关闭界面
		m_MTTRankView->updateMttRankView(uRankId,desc,icon,name,cash,score) ;
	}
    m_MTTRankView->setVisible(flag);
}


/************************************************************************/
/* 通知sng结束        
旁观者 直接弹退出提示
如果正显示排行  关闭排行后自动弹出
*/
/************************************************************************/
void GameTable::showMttGameOver(bool flag)
{

	if (m_MTTRankView->isVisible())
	{
		return ;
	}
	showMttRankView(false) ;
	showGameAutoTrust(false) ;//胜利失败时候关闭界面
	PopTipWithBt1("游戏结束，请返回大厅", "确定", [this](unsigned uBt){
		if (uBt == 0)
		{
			SendClientRequest(ClientRequest::kCallLevelTable) ;
		}
	});

}



void GameTable::showMttRebuyTipsByServer(bool flag,int time)
{
	if (m_mttRebuyTipsBySercer == nullptr)
	{
		m_mttRebuyTipsBySercer = MttRebuyTipsByServer::create();
		m_mttRebuyTipsBySercer -> ignoreAnchorPointForPosition(false);
		m_mttRebuyTipsBySercer -> setAnchorPoint(Point(0.5,0.5));

		m_mttRebuyTipsBySercer->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_mttRebuyTipsBySercer, MttRebuyTipsZOrder);
	}
	if (flag)
	{
		m_mttRebuyTipsBySercer->updateMttRebuyTips(time) ;
	}
	m_mttRebuyTipsBySercer->setVisible(flag) ;
}

void GameTable::reSetMttTable(bool flag)
{   
	if (flag)
	{
		updateMttButtonShow() ;
		m_layer_mtt_time->setVisible(false) ;
		m_pMenuItemReward->setVisible(true) ;
		m_pMenuItemPopChat->setVisible(true);
		m_pMenuItemAutoSitDown->setVisible(false) ;
	}
	else
	{
		m_bt_mtt_rebuy->setVisible(false) ;
		m_bt_mtt_addon->setVisible(false) ;
		m_pMenuItemReward->setVisible(false) ;
		m_pMenuItemPopChat->setVisible(false);
		m_pMenuItemAutoSitDown->setVisible(false) ;

	}

}

void GameTable::doDismassMttTable()
{
	if (m_MTTRankView->isVisible())
	{
		return ;
	}
	SendClientRequest(ClientRequest::kCallLevelTable) ;
	PopTipWithBt1("比赛已合桌，旁观用户请返回大厅","确定",nullptr) ;
}


/************************************************************************/
/* 弹窗管理                                                                     */
/************************************************************************/
void GameTable::callUserControl(bool flag,cocos2d::Node* node)
{
	if (flag)
	{
		while (m_vecLayer.size()>0)
		{
			cocos2d::Node* pRef = m_vecLayer.at(0) ;
			pRef->setVisible(false) ;
			m_vecLayer.erase(m_vecLayer.begin()) ;
		}
		if (m_pUserInfoView!=nullptr && m_pUserInfoView->isVisible())
		{
			m_vecLayer.push_back(m_pUserInfoView);
		}
		if (g_pGameFrame->getMttMatchState()!=nullptr && g_pGameFrame->isMttMatchStateVisible())
		{
			m_vecLayer.push_back(g_pGameFrame->getMttMatchState());
		}
		if (m_sngMatchState!=nullptr && m_sngMatchState->isVisible())
		{
			m_vecLayer.push_back(m_sngMatchState);
		}

		if (g_pGameFrame->getNewPlayerTask()!=nullptr && g_pGameFrame->isNewPlayerTaskVisiable())
		{
			m_vecLayer.push_back(g_pGameFrame->getNewPlayerTask());
		}

		if (m_pQuickShop!=nullptr && m_pQuickShop->isVisible())
		{
			m_vecLayer.push_back(m_pQuickShop);
		}

		if (m_privateRoomInfo!=nullptr && m_privateRoomInfo->isVisible())
		{
			m_vecLayer.push_back(m_privateRoomInfo);
		}


		if (m_vecLayer.size()>0)
		{
			Node* curP = m_vecLayer.back() ;
			ActionInterval* actionMove = MoveTo::create(0.35f,Point(GAME_WIDTH/2,GAME_HEIGHT/2+GAME_HEIGHT)) ;

			auto pp = curP ;
			auto hideAciton = [pp](){
				pp->setVisible(false) ;
			}; 
			curP->runAction(Sequence::create(actionMove,CallFunc::create(hideAciton),NULL));
		}
	}
	else
	{
		if (m_vecLayer.size()>0)
		{
			Node* curP = m_vecLayer.back() ;
			if (m_pUserInfoView!=nullptr && curP == m_pUserInfoView)
			{
				Json::Value param;
				param["tuid"] = m_usrInfo[m_CurTouchChairId].m_uUID;
				SendClientRequest(ClientRequest::kGetTableUserDetailInfo, param);	
			}
					
			curP->setVisible(true) ;
			ActionInterval* actionMove = MoveTo::create(0.35f,Point(GAME_WIDTH/2,GAME_HEIGHT/2)) ;
			curP->runAction(actionMove);
			//curP->setMoveUp(false) ;
			m_vecLayer.pop_back() ;
		}
	
	}
	
		

}

void GameTable::callShowJoinTableEffect(int uChairID,int vipLev, int skinid, bool newPalyer)
{

	auto& pUserView = m_pUsers[uChairID];


	if (pUserView!=nullptr && newPalyer)
	{
		if ( ( skinid>=14))
		{
			pUserView->setVisible(true);
			pUserView->stopAllActions();
			pUserView->setPosition(s_ptUserViewDefault[uChairID]);
			pUserView->showJoinTableEffect(vipLev,skinid) ;
		}

	}

}

/************************************************************************/
/* 初始化私密房相关界面配置                                                                     */
/************************************************************************/
void GameTable::initPrivateRoomInfo()
{
	m_privateRoomInfo = nullptr ;

	m_layout_PrivateTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup_private"));
	m_layout_PrivateTable->setVisible(false) ;

	m_bt_Private_startOrEndGame = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_startGame"));
	m_bt_Private_startOrEndGame->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_startOrEndGame->setVisible(true);

	m_text_private_startOrEndGame = static_cast<Text*>(Tools::getWidgetByName(m_bt_Private_startOrEndGame,"gt_text_startgame"));

	m_bt_Private_Box = static_cast<Button*>(Tools::getWidgetByName(m_layout_PrivateTable,"gt_tb_private_friend"));
	m_bt_Private_Box->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_Box->setVisible(true);

	m_bt_Private_task = static_cast<Button*>(Tools::getWidgetByName(m_layout_PrivateTable,"gt_bt_private_task"));
	m_bt_Private_task->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_task->setVisible(true);

	m_bt_Private_shop = static_cast<Button*>(Tools::getWidgetByName(m_layout_PrivateTable,"gt_bt_private_shop"));
	m_bt_Private_shop->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_shop->setVisible(true);

	m_layout_Private_btBox = static_cast<Layout*>(Tools::getWidgetByName(m_layout_PrivateTable,"Panel_private_share"));

	m_bt_Private_touch = static_cast<Button*>(Tools::getWidgetByName(m_layout_Private_btBox,"Button_privateTouch"));
	m_bt_Private_touch->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_touch->setVisible(true);


	m_bt_Private_InvateFriends = static_cast<Button*>(Tools::getWidgetByName(m_layout_PrivateTable,"Button_private_invatefriends"));
	m_bt_Private_InvateFriends->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_InvateFriends->setVisible(true);

	m_bt_Private_Share = static_cast<Button*>(Tools::getWidgetByName(m_layout_PrivateTable,"Button_private_share"));
	m_bt_Private_Share->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_Share->setVisible(true);

	m_bt_Private_RoomInfo = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"Button_privateroominfo"));
	m_bt_Private_RoomInfo->addTouchEventListener(this,toucheventselector(GameTable::onPrivateMenuItemClicked) ) ;
	m_bt_Private_RoomInfo->setVisible(true);

	m_image_Private_roomInfo = static_cast<ImageView*>(Tools::getWidgetByName(m_bt_Private_RoomInfo,"Image_private_info"));
	m_image_Private_roomInfo->setVisible(true) ;



}

void GameTable::showPrivateRoomInfoLayer(bool flag)
{
	m_vecLayer.clear() ;
	if (m_privateRoomInfo == nullptr)
	{
		m_privateRoomInfo = PrivateRoomInfo::create();
		m_privateRoomInfo -> ignoreAnchorPointForPosition(false);
		m_privateRoomInfo -> setAnchorPoint(Point(0.5,0.5));
		addChild(m_privateRoomInfo, PrivateRoomInfoZOrder);
	}
	m_privateRoomInfo->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
	
	m_privateRoomInfo->setVisible(flag) ;
	if (flag)
	{
		m_privateRoomInfo->updateCurRoomInfo(m_globaPrivateRoomInfo.m_isOwner) ;
	}

	Json::Value parm ;

	//if (m_globaPrivateRoomInfo.m_hasGetData)
	//{
	//	
	//	parm["type"] = 1 ;
	//}
	//else
	//{
		parm["type"] = 0 ;
	//}
	SendClientRequest(ClientRequest::kRequsetPrivateRoomInfo,parm);

}

//0等待  1开始 2结束
void GameTable::doControllPrivateTable(bool isOwner,unsigned state)
{
	m_globaPrivateRoomInfo.m_isGameState = state;

	if (isOwner)
	{
		if (state == 0)
		{
			m_bt_Private_startOrEndGame->setVisible(false);
			setUITextString(m_text_private_startOrEndGame,"开始牌局") ;
		}else if (state == 1)
		{

			m_bt_Private_startOrEndGame->setVisible(false);
			setUITextString(m_text_private_startOrEndGame,"结束牌局") ;

			if (g_globalTableConfig.GetTableMode() == TableLooking)
			{
				m_bt_Private_startOrEndGame->setVisible(true);
			}

		}else if (state == 2)
		{
			m_bt_Private_startOrEndGame->setVisible(false);
			setUITextString(m_text_private_startOrEndGame,"结束牌局") ;
		}
	}
	else
	{
		m_bt_Private_startOrEndGame->setVisible(false);
	}
	
			
}

void GameTable::setPrivateGameControllButtonVisiable(bool flag)
{
	if (getTableType() == PrivateTable && m_globaPrivateRoomInfo.m_isOwner )
	{


		m_bt_Private_startOrEndGame->setVisible(flag);
		if (m_globaPrivateRoomInfo.m_isGameState == 0)
		{
			setUITextString(m_text_private_startOrEndGame,"开始牌局") ;
		}else if (m_globaPrivateRoomInfo.m_isGameState == 1)
		{
			setUITextString(m_text_private_startOrEndGame,"结束牌局") ;
		}else if (m_globaPrivateRoomInfo.m_isGameState == 2)
		{
			m_bt_Private_startOrEndGame->setVisible(false);
		}
	}
	else
	{
		m_bt_Private_startOrEndGame->setVisible(false);
	}
	
}

void GameTable::updatePrivateTipsPosition()
{
	int length = g_globalTableConfig.m_strRoomName.size() ;
	
	m_image_Private_roomInfo->setPositionX(898/2-(897-543)/2+m_pLabelTableDesc->getContentSize().width/2+32) ;
}

void GameTable::updatePrivateFund(bool flag)
{
	if (m_privateRoomInfo!=nullptr)
	{
		m_privateRoomInfo->updateCurRoomInfo(flag) ;
	}
	
}