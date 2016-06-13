#include "GameHall.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "GrabTouch.h"
#include "HttpSprite.h"
#include "GameCommon.h"
#include "GameSet.h"
#include "UserInfo.h"
#include "FrameSprite.h"
#include "GrabKeyboard.h"
#include "GlobalUserData.h"
#include "TaskSet.h"
#include "FrameSpriteButton.h"
#include "GameFriendList.h"
#include "RankList.h"
#include "GameActivity.h"
#include "MessageList.h"
#include "QuickFollowList.h"
#include "AchievementUi.h"
#include "PersonalInfomation.h"
#include "PrivilegesRewards.h"
#include "OpenUpVip.h"
#include "DailySign.h"
#include "GameData.h"
#include "GameDialog.h"
#include "ChooseFamilyIcon.h"
#include "EditFamilyRole.h"
#include "ChangeViceShaikh.h"
#include "InviteFriends.h"
#include "MobileBind.h"
#include "MobileCheck.h"
#include "DealApplyFamily.h"
#include "FamilyInfoTips.h"
#include "PopGuideDialog.h"
#include "UserItem.h"
#include "GameTipsManager.h"
#include "SngOnline.h"
#include "MoveLabel.h"
#include "Tools.h"
#include "GameConfig.h"
#include "ScrollingBanners.h"
#include "TrackManager.h"
#include "GuideTips.h"
#include "FullScreenAds.h"
#include "AnimationManager.h"
#include "ActivityManage.h"
#include "PlatformSpec.h"
#include "AnimateViewer.h"
#include "PlatformSpec.h"
#include "GameClubHall.h"
#include "SelfClub.h"
#include "AllotFund.h"
#include "FriendsSpread.h"

USING_NS_CC;

enum
{

   
 
	TaskSetZOrder = 1,
	SngOnlineZOrder ,
	FollowListZOrder,

	RankListZOrder,
	GameMessageListZOrder,
	GrowUpToVipZorder,



	GameActivityZOrder,	
    FriendsSpreadZOrder,
    DelaerGuideTips,
	PrivilegesRewardsZOrder,
    NewerTrainZOrder,
	
	WaveGirlRewardsZPrder,
	PushActivitiesZOrder,
	DailySignZOrder,
	GetDailySignZOrder,
	GameFamilyZOrder,
	TargetFamilyZOrder,
	MyFamilyZOrder,

	ContritionZOrder, 
	EditRoleZOrder,

	UserInfoZOrder,
	PersonalInfoZOrder,
	ChooseIconZOrder,
	AllotFundZOrder,
	AchievementInfoZOrder,

	ScrollTipsLayer,
	GameLayerTouchZorder,
};

const char *g_pStatus[] = {
	"正在闲逛...",
	"正在游戏..."
};

void GameHall::UpdateMissionInfo()
{
	if (m_pTaskSet != nullptr)
	{
		m_pTaskSet->UpdateDayTask();
        m_pTaskSet->UpdateWeeklyTask();
        m_pTaskSet->UpdateAchievementTask();
        m_pSpriteTaskFlag->setVisible(getTaskStatus() > 0 ? true:false); 
	}
}

void GameHall::UpdateFollowList(unsigned uType)
{
	if (m_pFriendList)
	{
		m_pFriendList->UpdateFollowList(uType);
	}
}

void GameHall::UpdateRankList(unsigned uRankType, unsigned uWinType)
{
	if (m_pRankList && m_pRankList->isVisible())
	{
		m_pRankList->UpdateRankList(uRankType, uWinType);
	}
}

void GameHall::PopUserDetailInfo(unsigned uTID)
{
	if (m_pUserInfo == nullptr)
	{
		m_pUserInfo = UserInfoHallView::create();
		m_pUserInfo -> ignoreAnchorPointForPosition(false);
		m_pUserInfo -> setAnchorPoint(Point(0.5,0.5));
		m_pUserInfo->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_pUserInfo, UserInfoZOrder);
	}
	//else
	//{
        m_layerColorBg->setVisible(true);
		m_pUserInfo->setVisible(true);
	//}
	
	
	
		//m_userInfoZorder = UserInfoZOrder ;
	
	m_pUserInfo->Preload(uTID == g_globalMyData.m_iUserId);
	Json::Value param;
	param["tuid"] = uTID;
	SendClientRequest(ClientRequest::kGetUserDetailInfo, param);
	SendClientRequest(ClientRequest::kCallSngHallInfo, param);
	SendClientRequest(ClientRequest::kCallMttHallInfo, param);
}

void GameHall::PopUserAchievementInfo(unsigned uTID)
{
	if (m_pAchievementDlg == nullptr)
	{
		m_pAchievementDlg = AchievementUi::create();
		m_pAchievementDlg->ignoreAnchorPointForPosition(false);
		m_pAchievementDlg->setAnchorPoint(Point(0.5,0.5));
		m_pAchievementDlg->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		m_pAchievementDlg->setVisible(false);
		addChild(m_pAchievementDlg, AchievementInfoZOrder);
	}
	m_pAchievementDlg->setVisible(true);
}

void GameHall::hideBtSkin()
{
	//if (m_pBtSkin!=nullptr)
	//{
	//	m_pBtSkin->setVisible(false) ;
	//	m_pBtSkin->stopAllActions() ;

	//}

}

void GameHall::showBtSkin()
{
	m_pBtSkin->stopAllActions() ;
	m_pBtSkin->setVisible(true) ;
	Tools::callScaleAction(m_pBtSkin,true,1.2,0.8,1.05,0.95) ;

}

void GameHall::callPopPersonalInfo(Ref* pSender)
{
	PopPersonalInfo() ;
}

void GameHall::setShowBtSkinBig()
{

		//CallFuncN * funcall = CallFuncN::create(CC_CALLBACK_0(GameHall::setShowBtSkillSmall, this));
		//FiniteTimeAction* s_seq = Sequence::create(ScaleTo::create(1.2,1.1),funcall,NULL);  
		//m_pBtSkin->runAction(s_seq) ;

}
void GameHall::setShowBtSkillSmall()
{
	//CallFuncN * funcall = CallFuncN::create(CC_CALLBACK_0(GameHall::setShowBtSkinBig, this));
	//FiniteTimeAction* s_seq = Sequence::create(ScaleTo::create(0.8,1),funcall,NULL);  
	//m_pBtSkin->runAction(s_seq) ;
}


void GameHall::PopPersonalInfo()
{
	if (m_layout_match->isVisible())
	{
		return ;
	}


	if (m_pPersonalInfomation == nullptr)
	{
		m_pPersonalInfomation = PersonalInfomation::create();
		m_pPersonalInfomation->setVisible(false);

		m_pPersonalInfomation -> ignoreAnchorPointForPosition(false);
		m_pPersonalInfomation -> setAnchorPoint(Point(0.5,0.5));
		m_pPersonalInfomation->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_pPersonalInfomation, PersonalInfoZOrder);
	}
	m_pPersonalInfomation->UpdatePersonalInfo();
	m_pPersonalInfomation->setVisible(true);
    TrackMgr->event("PopDialog", "编辑头像弹窗");


	
	//GameData::getInstance()->setHasTeachSkinButton(g_globalMyData.m_iUserId) ;
	//hideBtSkin();

}

void GameHall::UpdateAchievementInfo()
{
	if (m_pAchievementDlg)
	{
		m_pAchievementDlg->UpdateAchievementList();
	}
}

void GameHall::UpdateUserInfo(const UserDetailInfo& userInfo)
{
	if (m_pUserInfo && m_pUserInfo->isVisible())
	{
		m_pUserInfo->UpdateUserInfo(userInfo, userInfo.m_uUID == g_globalMyData.m_iUserId);
	}
}

void GameHall::UpdateUserSngInfo(const UserSngInfo& userInfo)
{
	if (m_pUserInfo && m_pUserInfo->isVisible())
	{
		m_pUserInfo->UpdateUserSngInfo(userInfo);
	}
}

void GameHall::UpdateUserMttInfo(const UserMTTInfo& userInfo)
{
	if (m_pUserInfo && m_pUserInfo->isVisible())
	{
		m_pUserInfo->UpdateUserMTTInfo(userInfo);
	}
}

void GameHall::UpdateQuickFollowList(unsigned uType)
{
	m_pQuickFriend->UpdateView(uType);
}

void GameHall::OperateQuickFollowList(unsigned uOpType, unsigned uType, const QuickFollowInfo& quickFollowInfo)
{
	if (m_pQuickFriend==nullptr)
	{
		return ;
	}
	if (uOpType == 2)
	{
		m_pQuickFriend->UpdateItemView(uType, quickFollowInfo);
	}
	else if (uOpType == 1)
	{
		m_pQuickFriend->AppadnItemView(uType, quickFollowInfo);
	}
	else if (uOpType == 0)
	{
		m_pQuickFriend->RemoveItemView(uType, quickFollowInfo.m_uUserID);
	}
}

void GameHall::UpdateActivityInfo()
{
	if (m_pGameActivity && m_pGameActivity->isVisible())
	{
		m_pGameActivity->UpdateActivityView();
	}
}

void GameHall::UpdateMessageInfo(unsigned uType)
{
	if (m_pMessageList && m_pMessageList->isVisible())
	{
		m_pMessageList->UpdateView(uType);
	}
}

void GameHall::SetEnterGameResult(bool bSuccess)
{
	if (bSuccess)
	{
		setVisible(false);
	}
	else
	{
	}
}

void GameHall::ShowAnimateEntering()
{
}

bool GameHall::init()
{
	//载入布局
	auto rootNode = CSLoader::createNode("res/ui/gamehall1.csb");
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	rootNode->setContentSize(frameSize); 
	ui::Helper::doLayout(rootNode);  
	//rootNode->setScale(0.5);
	addChild(rootNode);

	m_spriteSngOnline = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_sngonline"));


	m_pHttpSpriteBG = HttpSprite::create("hall/img_bg.png");
	m_pHttpSpriteBG->ignoreAnchorPointForPosition(false);
	m_pHttpSpriteBG->setAnchorPoint(Point(0.5,0.5));
	//m_pHttpSpriteBG = HttpSprite::create("hall/bg.webp");
	//auto sz = m_pHttpSpriteBG->getContentSize();
	//setContentSize(sz);
	//背景重设
	ImageView* img_bg = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_beijing"));
	img_bg->ignoreAnchorPointForPosition(false);
	img_bg->setAnchorPoint(Point(0.5,0.5));


	//m_pHttpSpriteBG->setContentSize(Size(GameData::getInstance()->getGameWidth(),GameData::getInstance()->getGameHeight()));

	m_pHttpSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

	m_pHttpSpriteBG->setScale(GameData::getInstance()->getGameScale());

	img_bg->addChild(m_pHttpSpriteBG);

	char szBt[64] = {0};

	
	// 左上的关注和德女郎界面
	imgScrollList = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_liebiao"));
	m_pQuickFriend = QuickFollowList::creatWithLayer(imgScrollList);
	addChild(m_pQuickFriend);
    m_christmasShow[0] = static_cast<ImageView*>(Tools::getWidgetByName(imgScrollList, "Image_liebiao_snow"));

	//CheckBox* box ;

    auto imgLogo = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "Image_room"));
    imgLogo->setVisible(true);

	m_fXingXiangHeight = 0;
	//m_pHttpSpriteBG->setScale(img_bg->getScale());

	// 用户形象
	const char *szXingXiang[3] = {
		"hall/xingxiang1.webp",
		"hall/xingxiang2.webp",
		"hall/xingxiang3.webp",
	};
	/*ImageView* */img_roleBody = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_lihui"));
	m_pHttpSpriteXingXiang = HttpSprite::create("hall/xingxiang2.png");

	//人物按钮触摸区域
	m_pBtXingXiang = LayerButton::create(true, true);
	m_pBtXingXiang->setPosition(img_roleBody->getPosition());
	m_pBtXingXiang->setContentSize(Size(325, 393));
	m_pBtXingXiang->ignoreAnchorPointForPosition(false);
	m_pBtXingXiang->setAnchorPoint(Point(0.5,0.5));
	addChild(m_pBtXingXiang);

	m_pBtXingXiang->SetSelectCB([this](Ref*, bool bSel){

		if (m_layout_match->isVisible())
		{
			return ;
		}
		m_pHttpSpriteXingXiang->setScale(bSel ? 0.9f : 1.0f);
		if(bSel)
		{
			m_pHttpSpriteXingXiang->setPositionY(m_fXingXiangScaleLowHeight);
		}
		else
		{
			m_pHttpSpriteXingXiang->setPositionY(m_fXingXiangScaleTopHeight);
		}
		
	});
	m_pBtXingXiang->SetHitCB([this](Ref*){PopPersonalInfo();
	});

	//人物身上家换装按钮
	m_pBtSkin =  static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_huanzhuang"));
	m_pBtSkin->setVisible(true) ;

	Sprite* m_qqTips = Sprite::create("hall/bt_gameTips.png") ;
	m_pBtSkin->addChild(m_qqTips) ;

	//人物形象

	m_pHttpSpriteXingXiang->setPosition(-m_pHttpSpriteXingXiang->getContentSize().width/2,-m_pHttpSpriteXingXiang->getContentSize().height/2);
	m_fXingXiangHeight = m_pHttpSpriteXingXiang->getPositionY() - 35.5 ;
	m_fXingXiangScaleLowHeight = m_pHttpSpriteXingXiang->getPositionY() - 60 ;
	m_fXingXiangScaleTopHeight = m_pHttpSpriteXingXiang->getPositionY() - 35.5 ;
	img_roleBody->addChild(m_pHttpSpriteXingXiang, 1);

	// 桌子重设
	m_pSpriteTable = static_cast<Layout*>(Tools::getWidgetByName(rootNode,"Panel_dating_zhuozi"));
    m_christmasShow[1] = static_cast<ImageView*>(Tools::getWidgetByName(m_pSpriteTable, "Image_zhuozi_snow2"));
    m_christmasShow[2] = static_cast<ImageView*>(Tools::getWidgetByName(m_pSpriteTable, "Image_zhuozi_snow1"));

	//m_pSpriteTable->setScale(0.5);
	
	// 右边开始游戏菜单
	float fInterval[4] = {1.45, 2.3,1.3,1.45};
	//float fInterval[4] = {1.8, 1.9, 1.5, 1.6};
	unsigned uXFrames[4] = {6, 6,6, 6 };
	unsigned uYFrames[4] = {7,6,7,7 };
	unsigned uAllFrames[4] = {37,15,40,37};

	//unsigned uXFrames[4] = {6, 6,6, 6 };
	//unsigned uYFrames[4] = {5,3,7,7 };
	//unsigned uAllFrames[4] = {27,15,40,37};

	const char* gameTypeName[4]= {"Button_mtt","Button_sng","Button_changgui","Button_shipin"} ;
	for (int i = 0; i < 4; i++)
	{
		//sprintf(szBt, "hall/entergame%d.webp", i);
		m_pSpriteEnterGame[i] = static_cast<Button*>(Tools::getWidgetByName(rootNode,gameTypeName[i]));
		m_pSpriteEnterGame[i]->addTouchEventListener(this,toucheventselector(GameHall::onGameTypeSelect)) ;

	
			sprintf(szBt, "hall/anientergame%d.png", i);
			m_pSpriteAniEnterGame[i] = CreateFrameSprite(szBt, uXFrames[i], uYFrames[i]);
			m_pSpriteAniEnterGame[i]->setPosition(213+8, 52-2);
			m_pSpriteEnterGame[i]->addChild(m_pSpriteAniEnterGame[i]);

			auto ani = CreateAnimate(szBt, uXFrames[i], uYFrames[i], uAllFrames[i], 1.0f / 24.0f);
			m_pAniEnterGame[i] = RepeatForever::create(Sequence::create(ani, Hide::create(), DelayTime::create(fInterval[i]), Show::create(), nullptr));
			m_pSpriteAniEnterGame[i]->runAction(m_pAniEnterGame[i]);
			m_pAniEnterGame[i]->retain();
			
	}

	//m_aniPrivateRoom = AnimationManager::creatArmature("simichang");
	//m_pSpriteEnterGame[0]->addChild(m_aniPrivateRoom);
	//m_aniPrivateRoom->setPosition(221, 62);
	//m_aniPrivateRoom->playByIndexAndRepatForever(0) ;



    auto layerRoom = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_room"));
    m_christmasShow[4] = static_cast<ImageView*>(Tools::getWidgetByName(layerRoom, "Image_shipin_snow"));

	const char* str_buttonName[8]= {"Button_shangcheng","Button_xiaoxi", "Button_paiming", "Button_renwu", "Button_haoyou", "Button_jiazu", "Button_shezhi", "Button_gengduo"} ;

	for (int i=0; i<8 ;i++)
	{
		m_pMenuItemBottom[i]= static_cast<Button*>(Tools::getWidgetByName(rootNode,str_buttonName[i]));
		m_pMenuItemBottom[i]->addTouchEventListener(this,toucheventselector(GameHall::onMenuItemBottomSelect)) ;
		
	}
    m_pSpriteTaskFlag = CreateNode1AndPosAddTo<Sprite>("inviteflag.png", 16,92,m_pMenuItemBottom[3], 1);

	m_newMessageTips = Sprite::create("inviteflag.png") ;
	m_pMenuItemBottom[1]->addChild(m_newMessageTips) ;
	m_newMessageTips->setVisible(false) ;
	m_newMessageTips->setPosition(16,92) ;


	// 昵称
	m_pLabelName =  static_cast<Text*>(Tools::getWidgetByName(rootNode,"Text_name"));
	SetUITextStringWithBoundWidth(m_pLabelName, g_globalMyData.GetNickName(), 165) ;
	//m_pLabelName->setTextColor(Color4B::WHITE);
	m_pLabelName->setTextHorizontalAlignment(TextHAlignment::RIGHT) ;

    m_pImageCashBg = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "Image_gold_bg"));
    m_pImageDiamondBg = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "Image_Diamond_bg"));
	
	m_pLabelCash =  static_cast<Text*>(Tools::getWidgetByName(m_pImageCashBg,"Text_gold"));
	setUITextString(m_pLabelCash,FormatCash4(g_globalMyData.GetCash()));
	m_pLabelCash->setTextHorizontalAlignment(TextHAlignment::RIGHT) ;
	m_pSprite$ = static_cast<ImageView*>(Tools::getWidgetByName(m_pImageCashBg,"Image_jinbi"));
    auto shineCash = CreateNode1AndPosAddTo<Sprite>("hall/moneyshine.png",29, 15, m_pSprite$, 0);
    shineCash->setScale(0);
    auto action1 = Sequence::create(ScaleTo::create(0.3, 1.2, 1.2, 0), DelayTime::create(0.3),ScaleTo::create(0.3, 0,0,0), DelayTime::create(1.5), nullptr);
    auto action2 = RepeatForever::create(Spawn::create(RotateBy::create(1.5f, Vec3(0,0,150)), action1, nullptr));
    shineCash->runAction(action2);

    m_pLabelDiamond = static_cast<Text*>(Tools::getWidgetByName(m_pImageDiamondBg, "Text_diamond"));
    m_pSpriteDiamond = static_cast<ImageView*>(Tools::getWidgetByName(m_pImageDiamondBg, "Image_diamond"));
    auto shineDiamond = CreateNode1AndPosAddTo<Sprite>("hall/moneyshine.png",39, 21, m_pSpriteDiamond, 0);
    shineDiamond->setScale(0);
    auto action3 = Sequence::create(ScaleTo::create(0.3, 1.2, 1.2, 0), DelayTime::create(0.3),ScaleTo::create(0.3, 0,0,0), DelayTime::create(1.5), nullptr);
    auto action4 = RepeatForever::create(Spawn::create(RotateBy::create(1.5f, Vec3(0,0,150)), action3, nullptr));
    shineDiamond->runAction(action4);
	// 头像
    auto panelGerenziliao = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_gerenziliao"));
    m_pLayoutTouXiang = static_cast<Layout*>(Tools::getWidgetByName(panelGerenziliao,"Panel_clip_head"));
	imgHead =  static_cast<ImageView*>(Tools::getWidgetByName(panelGerenziliao,"Button_gerenziliao"));
    m_pMenuItemHitTouXiang = static_cast<Button*>(Tools::getWidgetByName(panelGerenziliao,"Button_clickhead"));
    m_pMenuItemHitTouXiang->addTouchEventListener(this, toucheventselector(GameHall::onMenuItemTouXiangSelect)) ;

	auto pClipMask = Sprite::create("hall/iconclipmask.png");
	pClipMask->setPosition(52.9, 53.5);
	pClipMask->retain();

	m_pClipTouXiang = CreateNodeAndPosAddTo<ClippingNode>(-53+imgHead->getContentSize().width/2, -53+imgHead->getContentSize().height/2, imgHead, 0);
	m_pClipTouXiang->setContentSize(Size(106, 106));

	m_pSpriteTouXiang = CreateNode2AndPosAddTo<HttpSprite>("defaulticon.png", Size(106, 106), 0, 0, m_pClipTouXiang, 0);
	//m_pSpriteIconRound = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_waiquan"));


	m_pClipTouXiang->setStencil(pClipMask);
	m_pClipTouXiang->setAlphaThreshold(0.5);

    //auto personInfo = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_gerenziliao"));
    m_christmasShow[3] = static_cast<ImageView*>(Tools::getWidgetByName(panelGerenziliao, "Image_gerenziliao_hat"));
	
	m_pVipSign = CreateNode3AndPosAddTo<FrameSprite>("vip.png", 6, 1, 15, 100, m_pMenuItemHitTouXiang, 0);

	m_pVipSign->SetFrameIdx(g_globalMyData.GetVipLevel() - 1);
	m_pVipSign->setVisible(false);



//#if OpenNewSkin
//	if (g_globalMyData.GetSkinID() <2) {
//		SetLocalIconUrl(m_pSpriteTouXiang, g_globalMyData.GetIconURL());
//	} else {
//		UserSkin skin = g_globalMyData.GetSkin(g_globalMyData.GetSkinID()) ;
//		SetIconUrl(m_pSpriteTouXiang, skin.m_strIconUrl);
//	}
//#else
//	if (g_globalMyData.GetSkinID() ==0) {
//		SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetIconURL());
//	} else {
//		UserSkin skin = g_globalMyData.GetSkin(g_globalMyData.GetSkinID()) ;
//		SetIconUrl(m_pSpriteTouXiang, skin.m_strIconUrl);
//	}
//#endif

	if (g_globalMyData.GetSkinID() ==0) {
		SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetIconURL());
	} else {
		UserSkin skin = g_globalMyData.GetSkin(g_globalMyData.GetSkinID()) ;
		SetIconUrl(m_pSpriteTouXiang, skin.m_strIconUrl);
	}

    if (ActManage->getSpringFestivalState())
    {
        SetHttpSpriteURL(m_pHttpSpriteBG, ActManage->getSpringFestivalURL());
    }
    else
    {
        SetHttpSpriteURL(m_pHttpSpriteBG, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strAvatorBgUrl);
    }

    SetHttpSpriteURL(m_pHttpSpriteXingXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strAvatorUrl);
	
	//m_pHttpSpriteBG->setContentSize(Size(GameData::getInstance()->getGameWidth(),GameData::getInstance()->getGameHeight()));
	g_globalMyData.WaitChange(WAITCASHCHANGE | WAITSKINCHANGE | WAITNICKNAMECHANGE | WAITVIPCHANGE | WAITDIAMONDCHANGE, std::bind(&GameHall::OnUpdateMyData, this, std::placeholders::_1));

    auto frameSz = Director::getInstance()->getVisibleSize();
    m_layerColorBg = LayerColor::create(Color4B(0,0,0,120), frameSz.width, frameSz.height);
    m_layerColorBg->setPosition(0, 0);
    addChild(m_layerColorBg);
    m_layerColorBg->setVisible(false);

	Size screenSize = Director::getInstance()->getOpenGLView()->getFrameSize(); 
	m_pHallTouchLayer= GameLayer::create(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), false, false);
	m_pHallTouchLayer->setPosition(Vec2(0, 0));
	this->addChild(m_pHallTouchLayer, GameLayerTouchZorder);


	m_moveTipsLayer = ScrollingBanners::creatScrollingBanners() ;

	this->addChild(m_moveTipsLayer,ScrollTipsLayer);
	m_moveTipsLayer->ignoreAnchorPointForPosition(false) ;
	m_moveTipsLayer->setAnchorPoint(Point(0.5,1)) ;
	m_moveTipsLayer->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()) ;
    //圣诞相关的 隐藏
    for (int i=0; i<5; i++)
    {
        m_christmasShow[i]->setVisible(false);
    }



	//赛市场界面
	m_layout_match = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_match"));
	m_layout_match->setVisible(false) ;

	m_layout_matchColorBG = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_matchbgshow"));

	m_button_matchTouch = static_cast<Button*>(Tools::getWidgetByName(m_layout_match, "Button_matchtouch"));
	m_button_matchTouch->addTouchEventListener(this, toucheventselector(GameHall::onMenuMatchItemSelect)) ;

	m_button_sngMatch = static_cast<Button*>(Tools::getWidgetByName(m_layout_match, "Button_match_sng"));
	m_button_sngMatch->addTouchEventListener(this, toucheventselector(GameHall::onMenuMatchItemSelect)) ;

	m_button_mttMatch = static_cast<Button*>(Tools::getWidgetByName(m_layout_match, "Button_match_mtt"));
	m_button_mttMatch->addTouchEventListener(this, toucheventselector(GameHall::onMenuMatchItemSelect)) ;


	m_aniSngMatch = AnimationManager::creatArmature("matchsng") ;
	m_aniSngMatch->setPosition(Point(m_button_sngMatch->getContentSize().width-4,m_button_sngMatch->getContentSize().height/2-40-2) );
	m_button_sngMatch->addChild(m_aniSngMatch);
	m_aniSngMatch->playByIndexAndRepatForever(0) ;

	m_aniMttMatch = AnimationManager::creatArmature("matchmtt") ;
	m_aniMttMatch->setPosition(Point(m_button_mttMatch->getContentSize().width/2-232+2,m_button_mttMatch->getContentSize().height/2-18-6-2) );
	m_button_mttMatch->addChild(m_aniMttMatch);
	m_aniMttMatch->playByIndexAndRepatForever(0) ;


	//m_vecLineLabel.clear() ;
	m_isScrollTextMove = false ;
	//m_moveTipsLayer->setVisible(true) ;
	m_moveLableCount = 0 ;


	m_allotFund = nullptr ;
	m_pUserInfo = nullptr;
	m_pTaskSet = nullptr;
	m_pFriendList = nullptr;
	m_pRankList = nullptr;
	m_pGameActivity = nullptr;
	m_pMessageList = nullptr;
	m_pAchievementDlg = nullptr;
	m_pPersonalInfomation = nullptr;
	m_pPrivilegesRewards = nullptr;
	m_pNewerTrain = nullptr;
	m_pWaveGrilSign = nullptr;
	m_pDailySign = nullptr;
	m_chooseFamilyIcon = nullptr ;
	m_editFamilyRole = nullptr ;
	m_changeViceShaikh = nullptr ;
	m_inviteFriends= nullptr ;
	m_mobileCheck = nullptr;
	m_pMobileBind = nullptr ;
	m_dealApplyFamily = nullptr ;
    m_DailySignReward = nullptr;
    m_userItem = nullptr ;
    m_bPerfectEnter = false;


	m_spriteNewTips = nullptr ;
	m_tipsSngOnline = nullptr ;
    m_aniChristmasTree = nullptr;
    m_aniSpringFestival[0] = nullptr;
    m_aniSpringFestival[1] = nullptr;
	m_GameClubHall = nullptr ;
	m_selfClub = nullptr ;
    m_popMoreItems = nullptr;
    m_pFriendSpread = nullptr;

	//Tools::creatParticleAddTo(0,this,Point(200,200)) ;
	//Sprite* sppp = Sprite::create("test1.png") ;
	//sppp->setPosition(Point(1136/2,640/2));
	//this->addChild(sppp) ;

	//Tools::creatParticleAddTo("particle/xue.plist",this,Point(0,GameData::getInstance()->getGameHeight()),10) ;


	m_snow=CCParticleSnow::create();
	m_snow->setPosition(ccp(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()));
	m_snow->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/snow.png"));
	m_snow->setTotalParticles(300) ;
	m_snow->setGravity(Vec2(0,-5));
	m_snow->setRadialAccelVar(3);
	m_snow->setStartSize(24) ;
	m_snow->setSpeed(25);
	m_snow->setOpacity(100) ;
	m_snow->setLife(8.5f) ;
	addChild(m_snow,0);


	//CCParticleSnow* m_sno1w=CCParticleSnow::create();
	//m_sno1w->setPosition(ccp(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()));
	//m_sno1w->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/snow.png"));
	//m_sno1w->setTotalParticles(150) ;
	//m_sno1w->setGravity(Vec2(0,-5));
	//m_sno1w->setRadialAccelVar(3);
	//m_sno1w->setStartSize(32) ;
	//m_sno1w->setSpeed(25);
	//m_sno1w->setOpacity(100) ;
	//m_sno1w->setLife(8.5f) ;
	//addChild(m_sno1w,1);


	//CCParticleSnow* m_sm_snow=CCParticleSnow::create();
	//m_sm_snow->setPosition(ccp(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()));
	//m_sm_snow->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/Comet_256.png"));
	//m_sm_snow->setTotalParticles(80) ;
	//m_sm_snow->setGravity(Vec2(0,-5));
	//m_sm_snow->setRadialAccelVar(3);
	//m_sm_snow->setStartSize(64) ;
	//m_sm_snow->setSpeed(25);
	//m_sm_snow->setOpacity(100) ;
	//m_sm_snow->setLife(8.5f) ;

	//addChild(m_sm_snow,3);


	//m_snow->setVisible(false) ;

	return true;
}

void GameHall::setVisible(bool bVisible)
{
    if (!m_bPerfectEnter)
    {
        int timeInterval = getSystemTime() - TrackMgr->GetPreTime();
        if (timeInterval <=10)
            TrackMgr->event("LoginTime", "", timeInterval, "秒");
        else if (timeInterval >10 && timeInterval <=15)
            TrackMgr->event("LoginTime", "11-15", 0, "秒");
        else if (timeInterval >15 && timeInterval <=20)
            TrackMgr->event("LoginTime", "16-20", 0, "秒");
        TrackMgr->event("EnterGame", "进入一级大厅");
        TrackMgr->SetPreTime(getSystemTime());
        m_bPerfectEnter = true;
    }
	if (bVisible)
	{
        //请求俱乐部消息
        Json::Value param;
        param["type"] = 2;
        SendClientRequest(ClientRequest::kGetMessageInfo, param);

        //请求绑定推广员消息
        param["spreadID"] = "";
        SendClientRequest(ClientRequest::kBindSpread, param);

		checkNewMessage() ;
		callScrollTextMove() ;
        //圣诞活动
        createChristmas();
        //新春活动
        createSpringFestival();

        setUITextString(m_pLabelDiamond, FormatCash4(g_globalMyData.GetDiamond()));
		m_moveTipsLayer->checkVisible() ;
		if (m_spriteNewTips!=nullptr)
		{
			m_spriteNewTips->setVisible(false) ;
		}
        m_pSpriteTaskFlag->setVisible(getTaskStatus() > 0 ? true:false); 

		double dDelta = 0.3f;
		double dDelay = 0;
		float s_curX =0;
		float s_curY =0;
		auto sz = m_pSpriteTable->getContentSize();

		s_curY = m_pSpriteTable->getPositionY() ;
		s_curX = m_pSpriteTable->getPositionX() ;

		m_pSpriteTable->setPositionY(-sz.height / 2-100);
		m_pSpriteTable->runAction(EaseBackOut::create(MoveTo::create(dDelta, Vec2(m_pSpriteTable->getPositionX(), s_curY))));
		dDelay += dDelta;
		for (int i = 0; i < 8; i++)
		{
			s_curX = m_pMenuItemBottom[i]->getPositionX();
			s_curY = m_pMenuItemBottom[i]->getPositionY();
			sz = m_pMenuItemBottom[i]->getContentSize();
			m_pMenuItemBottom[i]->setPositionY(s_curY-sz.height-100);
			m_pMenuItemBottom[i]->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(0.14, Vec2(s_curX, s_curY))), nullptr));
			dDelay += 0.05;
		}

        s_curY = m_pLayoutTouXiang->getPositionY() ;
        s_curX = m_pLayoutTouXiang->getPositionX() ;
        sz = m_pLayoutTouXiang->getContentSize();
        m_pLayoutTouXiang->setPositionY(-sz.height-100);
        m_pLayoutTouXiang->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), 
            CallFunc::create([this]()
        {
            if (g_globalMyData.GetVipLevel() > 0)
            {
                m_pVipSign->setVisible(true);
                m_pVipSign->SetFrameIdx(g_globalMyData.GetVipLevel()-1);
            }
        }),nullptr));

        /*s_curY = m_pClipTouXiang->getPositionY() ;
        s_curX = m_pClipTouXiang->getPositionX() ;
        sz = m_pClipTouXiang->getContentSize();
        m_pClipTouXiang->setPositionY(-sz.height-100);
        m_pClipTouXiang->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), 
        CallFunc::create([this]()
        {
            if (g_globalMyData.GetVipLevel() > 0)
            {
                m_pVipSign->setVisible(true);
                m_pVipSign->SetFrameIdx(g_globalMyData.GetVipLevel()-1);
            }
        }),nullptr));
		s_curY = m_pMenuItemHitTouXiang->getPositionY() ;
		s_curX = m_pMenuItemHitTouXiang->getPositionX() ;
		sz = m_pMenuItemHitTouXiang->getContentSize();
		m_pMenuItemHitTouXiang->setPositionY(-sz.height-100);
		m_pMenuItemHitTouXiang->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));
*/
		sz = m_pQuickFriend->getContentSize();
		s_curX = imgScrollList->getPositionX() ;
		s_curY = imgScrollList->getPositionY() ;
		imgScrollList->setPositionY(-GameData::getInstance()->getGameHeight());
		imgScrollList->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));
		dDelay += 0.1;

		sz = m_pHttpSpriteXingXiang->getContentSize();

		s_curX = m_pHttpSpriteXingXiang->getPositionX() ;
		//s_curY = m_pHttpSpriteXingXiang->getPositionY() ;
		m_pHttpSpriteXingXiang->setPositionY(-sz.height*2);

		//CallFunc * funcall=CallFunc::create(CC_CALLBACK_0(GameHall::showBtSkin, this));
		/*FiniteTimeAction * seq = Sequence::create(DelayTime::create(dDelay), 
		EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX,s_curY))),funcall,NULL); */
			
		m_pHttpSpriteXingXiang->runAction(Sequence::create(DelayTime::create(dDelay), 
			EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX,m_fXingXiangHeight))),CallFunc::create(CC_CALLBACK_0(GameHall::showBtSkin, this)),nullptr)) ;
		dDelay += 0.1;

		for (int i = 3; i >= 0; --i)
		{
			s_curX = m_pSpriteEnterGame[i]->getPositionX();
			s_curY = m_pSpriteEnterGame[i]->getPositionY();
			sz = m_pSpriteEnterGame[i]->getContentSize();
			m_pSpriteEnterGame[i]->setPositionY(-sz.height / 2-GameData::getInstance()->getGameHeight());


			m_pSpriteEnterGame[i]->runAction(Sequence::create(DelayTime::create(dDelay), MoveTo::create(0.15, Vec2(s_curX, s_curY)), nullptr));
			dDelay += 0.1;
		}


	
		//s_curY = m_pSpriteIconRound->getPositionY() ;
		//s_curX = m_pSpriteIconRound->getPositionX() ;
		//sz = m_pSpriteIconRound->getContentSize();
		//m_pSpriteIconRound->setPositionY(-sz.height-100);
		//m_pSpriteIconRound->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));

	

		s_curX = m_pLabelName->getPositionX();
		s_curY = m_pLabelName->getPositionY() ;
		sz = m_pLabelName->getContentSize();
		m_pLabelName->setPositionY(-sz.height / 2-100);
		m_pLabelName->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));

        s_curX = m_pImageCashBg->getPositionX();
        s_curY = m_pImageCashBg->getPositionY() ;
        m_pImageCashBg->setPositionY(-sz.height / 2-100);
        m_pImageCashBg->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));

        s_curX = m_pImageDiamondBg->getPositionX();
        s_curY = m_pImageDiamondBg->getPositionY() ;
        m_pImageDiamondBg->setPositionY(-sz.height / 2-100);
        m_pImageDiamondBg->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));

		/*s_curX = m_pLabelCash->getPositionX();
		s_curY = m_pLabelCash->getPositionY() ;
		sz = m_pLabelCash->getContentSize();
		m_pLabelCash->setPositionY(-sz.height / 2-100);
		m_pLabelCash->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))), nullptr));*/
		/*sz = m_pSprite$->getContentSize();

		s_curX = m_pSprite$->getPositionX();
		s_curY = m_pSprite$->getPositionY() ;
		m_pSprite$->setPositionY(-sz.height / 2-100);
		m_pSprite$->runAction(Sequence::create(DelayTime::create(dDelay), EaseBackOut::create(MoveTo::create(dDelta, Vec2(s_curX, s_curY))),
			CallFunc::create([this]()
		{
			


#if !OpenOneByOneTips
			creatSiginLayer() ;
			creatActivityLayer() ;
			creatTeachLayer() ;
			creatGetVipRewardLayer() ;
#else
#endif
		creatTipsLayer() ;
		m_pHallTouchLayer->setVisible(false);
		}),nullptr));*/
#if !OpenOneByOneTips
        creatSiginLayer() ;
        creatActivityLayer() ;
        creatTeachLayer() ;
        creatGetVipRewardLayer() ;
#else
#endif
        creatTipsLayer() ;
        m_pHallTouchLayer->setVisible(false);

		PlayBackGroundSound("sound/bghall1.mp3", true);
		
//		auto tipsNew = [this](){
//			callNewTips() ;
//		} ;
//		this->runAction(Sequence::create(DelayTime::create(dDelay),CallFunc::create(tipsNew),NULL)) ;
	}
	else
	{	
		if (m_pUserInfo)
		{
			m_pUserInfo->setVisible(false);
		}

		if (m_pDailySign!=nullptr)
		{
			this->removeChild(m_pDailySign);
		}
        m_pDailySign = nullptr;

		m_pVipSign->setVisible(false);	
		//StopBackGroundSound(false);
		if (m_pBtSkin != nullptr)
			m_pBtSkin->setVisible(false) ;

		if (m_tipsSngOnline != nullptr)
		{
			m_tipsSngOnline->setVisible(false) ;
		}
	}
	Layer::setVisible(bVisible);
	if (bVisible)
	{
		RegistEscPressCb(this, [](Ref*){SendClientRequest(ClientRequest::kExit);});
	}
	else
	{
		UnRegistEscPressCb(this);
	}
}

void GameHall::onGameTypeSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type)
{
	Button* button = (Button*)pMenuItem;
	if (type == TouchEventType::TOUCH_EVENT_BEGAN)
	{
		button->setScale(0.9);

	/*	for (int i = 0; i < 4; i++)
		{
			if (pMenuItem == m_pSpriteEnterGame[i])
			{
					m_pSpriteAniEnterGame[i]->stopAction(m_pAniEnterGame[i]);
							
			}
		}
*/
	}
	if (type == TouchEventType::TOUCH_EVENT_CANCELED)
	{
		button->setScale(1);
		//for (int i = 0; i < 4; i++)
		//{
		//	if (pMenuItem == m_pSpriteEnterGame[i])
		//	{
		//		m_pAniEnterGame[i]->update(0);
		//		m_pSpriteAniEnterGame[i]->runAction(m_pAniEnterGame[i]);
		//	}
		//}
	}
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		button->setScale(1);
	/*	for (int i = 0; i < 4; i++)
		{
			if (pMenuItem == m_pSpriteEnterGame[i])
			{
				m_pAniEnterGame[i]->update(0);
				m_pSpriteAniEnterGame[i]->runAction(m_pAniEnterGame[i]);
			}
		}*/
		if (pMenuItem == m_pSpriteEnterGame[0])
		{

			//私密房
			Json::Value param;
			SendClientRequest(ClientRequest::kGetVipAllOpenInfo, param);

			return ;
		}
		if (pMenuItem == m_pSpriteEnterGame[1])
		{



			showMathcLayer(true) ;


			return ;
		}
		if (pMenuItem == m_pSpriteEnterGame[2])
		{
			SendClientRequest(ClientRequest::kEnterHall2);
			return ;
		}
		if (pMenuItem == m_pSpriteEnterGame[3])
		{
			Json::Value param;
			param["enterType"] = 2 - 3;
			SendClientRequest(ClientRequest::kEnterTable, param);
			return ;
		}
																			
	}
}

void GameHall::onMenuItemBottomSelect(cocos2d::Ref*pMenuItem,TouchEventType type)
{

	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
        //已960/640=1.5为标准，调整弹框的锚点
	    //float ratioDif = (GameData::getInstance()->getGameWidth() / GameData::getInstance()->getGameHeight() - 1.5)/2.75;
        //if (ratioDif < 0) ratioDif = 0;
		if (pMenuItem == m_pMenuItemBottom[0])
		{
				//showDailySign(true) ;
			Json::Value param;
			param["idx"] = 0;
			SendClientRequest(ClientRequest::kPopMall, param);
		}
		else if (pMenuItem == m_pMenuItemBottom[6])
		{
			Json::Value param;
			param["needconfirm"] = true;
			SendClientRequest(ClientRequest::kPopGameSet, param);
		}
		else if (pMenuItem == m_pMenuItemBottom[5]) // 俱乐部
		{

			//if (g_globalMyData.getJoinFamily())
			//{
			//	//showMyFamily(true,true);
			//	showSelfClub(true) ;
			//}
			//else
			//{
			//	//showGameFamily(true,true);
			//	showGameClub(true) ;
			//}
			showGameClub(true) ;
            m_layerColorBg->setVisible(true);
            TrackMgr->event("PopDialog", "俱乐部弹窗");
		}
		else if (pMenuItem == m_pMenuItemBottom[3])
		{
			if (m_pTaskSet == nullptr)
			{
				m_pTaskSet = TaskSet::create();
				m_pTaskSet -> ignoreAnchorPointForPosition(false);
				m_pTaskSet -> setAnchorPoint(Point(0.5,0.5));
                auto itemSz = m_pTaskSet->getContentSize();
				m_pTaskSet->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
				//m_pTaskSet->setVisible(false);
				addChild(m_pTaskSet, TaskSetZOrder);
                TrackMgr->event("PopDialog", "任务弹窗");

			}
            m_layerColorBg->setVisible(true);
			m_pTaskSet->setVisible(true);
		}
		else if (pMenuItem == m_pMenuItemBottom[4])
		{
			if (m_pFriendList == nullptr)
			{
				m_pFriendList = GameFriendList::create();
				m_pFriendList -> ignoreAnchorPointForPosition(false);
				m_pFriendList -> setAnchorPoint(Point(0.5,0.5));
                auto itemSz = m_pFriendList->getContentSize();
				m_pFriendList->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
				//m_pFriendList->setVisible(false);
				addChild(m_pFriendList, FollowListZOrder);			
                TrackMgr->event("PopDialog", "好友弹窗");
			}
            m_layerColorBg->setVisible(true);
			m_pFriendList->setVisible(true);
		}
		else if (pMenuItem == m_pMenuItemBottom[2])
		{
			if (m_pRankList == nullptr)
			{
				m_pRankList = RankList::create();
				m_pRankList -> ignoreAnchorPointForPosition(false);
				m_pRankList -> setAnchorPoint(Point(0.5,0.5));
                auto itemSz = m_pRankList->getContentSize();
				m_pRankList->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
				//m_pRankList->setVisible(false);
				addChild(m_pRankList, RankListZOrder);			
                TrackMgr->event("PopDialog", "排名弹窗");
			}
            m_layerColorBg->setVisible(true);
			m_pRankList->setVisible(true);
		}
		else if (pMenuItem == m_pMenuItemBottom[7])
		{
			//if (m_pGameActivity == nullptr)
			//{
			//	m_pGameActivity = GameActivity::create();

			//	m_pGameActivity -> ignoreAnchorPointForPosition(false);
			//	m_pGameActivity -> setAnchorPoint(Point(0.5,0.5));
   //             auto itemSz = m_pGameActivity->getContentSize();
			//	m_pGameActivity->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
			//	//m_pGameActivity->setVisible(false);
			//	addChild(m_pGameActivity, GameActivityZOrder);			
   //             TrackMgr->event("PopDialog", "活动弹窗");
			//}
   //         m_layerColorBg->setVisible(true);
			//m_pGameActivity->setVisible(true);
            popMoreBottomItems();
		}
		else if (pMenuItem == m_pMenuItemBottom[1])
		{
			if (m_pMessageList == nullptr)
			{
				m_pMessageList = MessageList::create();
				m_pMessageList -> ignoreAnchorPointForPosition(false);
				m_pMessageList -> setAnchorPoint(Point(0.5,0.5));
                auto itemSz = m_pMessageList->getContentSize();
				m_pMessageList->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
				//m_pMessageList->setVisible(false);
				addChild(m_pMessageList, GameMessageListZOrder);			
                TrackMgr->event("PopDialog", "消息弹窗");
			}
            m_layerColorBg->setVisible(true);
			m_pMessageList->setVisible(true);

			//bool show = GameData::getInstance()->getHasNewMessage() ;
			//m_pMessageList->updateShowFamilyMessageTips(show) ;
		}
	}

	
}

void GameHall::showBottomView(unsigned uType)
{
    //活动跳转页面调用
    //2：消息，3：任务，4：活动，5：俱乐部
    if (uType == 2)
    {
        onMenuItemBottomSelect(m_pMenuItemBottom[1], TouchEventType::TOUCH_EVENT_ENDED);
    }
    else if (uType == 3)
    {
        onMenuItemBottomSelect(m_pMenuItemBottom[3], TouchEventType::TOUCH_EVENT_ENDED);
    }
    else if (uType == 4)
    {
        onMenuItemBottomSelect(m_pMenuItemBottom[7], TouchEventType::TOUCH_EVENT_ENDED);
    }
    else if (uType == 5)
    {
        onMenuItemBottomSelect(m_pMenuItemBottom[5], TouchEventType::TOUCH_EVENT_ENDED);
    }
}

void GameHall::onMenuItemTouXiangSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type)
{
	if (type ==  TOUCH_EVENT_ENDED)
	{
		if (m_layout_match->isVisible())
		{
			return ;
		}
		 PopUserDetailInfo(g_globalMyData.m_iUserId);
		TrackMgr->event("PopDialog", "右下角个人信息弹窗");

		//callShare() ;
		//SendClientRequest(ClientRequest::kCallUpdateCashAndVip);
	}

}

void GameHall::OnUpdateMyData(unsigned uDataType)
{
	if (uDataType == WAITCASHCHANGE)
	{

		setUITextString(m_pLabelCash, FormatCash4(g_globalMyData.GetCash())) ;

	}
	else if (uDataType == WAITSKINCHANGE)
	{


		if (g_globalMyData.GetSkinID() < 1) {
			SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetIconURL());
			//SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
			SetHttpSpriteURL(m_pHttpSpriteXingXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strAvatorUrl);
		} else {
			SetIconUrl(m_pSpriteTouXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
			SetHttpSpriteURL(m_pHttpSpriteXingXiang, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strAvatorUrl);
		}		
		

         if (!ActManage->getSpringFestivalState())
         {
             SetHttpSpriteURL(m_pHttpSpriteBG, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strAvatorBgUrl);
         }
		

	}
	else if (uDataType == WAITNICKNAMECHANGE)
	{
		//SetLabelStringWithBoundWidth(m_pLabelName, g_globalMyData.GetNickName(), 130);
		//SetUITextStringWithBoundWidth() ;
		SetUITextStringWithBoundWidth(m_pLabelName, g_globalMyData.GetNickName(), 165) ;
	}
	else if(uDataType == WAITVIPCHANGE)
	{
		if (g_globalMyData.GetVipLevel() > 0)
		{
			m_pVipSign->setVisible(true);
			m_pVipSign->SetFrameIdx(g_globalMyData.GetVipLevel()-1);
		}
		else
		{
			m_pVipSign->setVisible(false);
		}
	}
    else if (uDataType == WAITDIAMONDCHANGE)
    {
        setUITextString(m_pLabelDiamond, FormatCash4(g_globalMyData.GetDiamond()));
    }
}

void GameHall::HideShowAll()
{
    if(m_pUserInfo != nullptr)
        m_pUserInfo->setVisible(false);
    if(m_pTaskSet != nullptr)
        m_pTaskSet->setVisible(false);
    if(m_pFriendList != nullptr)
        m_pFriendList->setVisible(false);
    if(m_pRankList != nullptr)
        m_pRankList->setVisible(false);
    if(m_pGameActivity != nullptr)
        m_pGameActivity->setVisible(false);
    if(m_pMessageList != nullptr)
        m_pMessageList->setVisible(false);
    if(m_pAchievementDlg != nullptr)
        m_pAchievementDlg->setVisible(false);
    if(m_pPersonalInfomation != nullptr)
        m_pPersonalInfomation->setVisible(false);
    if(m_pPrivilegesRewards != nullptr)
        m_pPrivilegesRewards->setVisible(false);
    if (m_pNewerTrain != nullptr)
        m_pNewerTrain->setVisible(false);
    if(m_pWaveGrilSign != nullptr)
        m_pWaveGrilSign->setVisible(false);
    if(m_pDailySign != nullptr)
        m_pDailySign->setVisible(false);





	if (m_chooseFamilyIcon!=nullptr)
		showChooseFamilyIcon(false) ;



	if (m_editFamilyRole!=nullptr)
		showEditFamilyRole(false) ;

	if (m_changeViceShaikh!=nullptr)
		showChangeShaikh(false) ;

	if (m_inviteFriends!=nullptr)
		showInviteFriends(false) ;
		
	if (m_dealApplyFamily!=nullptr)
		showDealApplyJoinFamily(false,0,0,0,"","");
	if (m_mobileCheck!=nullptr)
		m_mobileCheck->setVisible(false) ;


	if (m_userItem != nullptr)
	{
		m_userItem->setVisible(false) ;
	}
	if (m_GameClubHall != nullptr)
	{
		m_GameClubHall->setVisible(false) ;
	}
	if (m_selfClub != nullptr)
	{
		m_selfClub->setVisible(false) ;
	}

    m_layerColorBg->setVisible(false);
}

void GameHall::ShowLayerBg(bool bVisible)
{
    m_layerColorBg->setVisible(bVisible);
}

void GameHall::cretFamilySuccess()
{
	//m_gameFamily->showMyFamily(true);
	//showMyFamily(true,true);
}


void GameHall::callUpdateFamilyRankList()
{
	if (m_GameClubHall!=nullptr)
	{
		m_GameClubHall->UpdateGameClubView() ;
	}
	
}

void GameHall::callUpdateSelfJoinClubList()
{
	if (m_GameClubHall!=nullptr)
	{
		m_GameClubHall->UpdateSelfClubView() ;
	}
	
}



void GameHall::callUpdateMyFamily()
{

	if (m_selfClub!=nullptr)
	{
		m_selfClub->updateSelfClubInfo() ;
	}
	
}




void GameHall::showChooseFamilyIcon(bool flag)
{
	if (m_chooseFamilyIcon == nullptr)
	{
		m_chooseFamilyIcon = ChooseFamilyIcon::create();
		m_chooseFamilyIcon -> ignoreAnchorPointForPosition(false);
		m_chooseFamilyIcon -> setAnchorPoint(Point(0.5,0.5));
		m_chooseFamilyIcon->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_chooseFamilyIcon, ChooseIconZOrder);
	}
	m_layerColorBg->setVisible(flag);
	m_chooseFamilyIcon->UpdatePersonalInfo() ;
	m_chooseFamilyIcon->setVisible(flag);
}




void GameHall::showEditFamilyRole(bool flag,unsigned u_uid,std::string uName,unsigned  controFlags)
{
	if (m_editFamilyRole == nullptr)
	{
		m_editFamilyRole = EditFamilyRole::create();
		m_editFamilyRole -> ignoreAnchorPointForPosition(false);
		m_editFamilyRole -> setAnchorPoint(Point(0.5,0.5));
		m_editFamilyRole->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_editFamilyRole, EditRoleZOrder);
	}
	if (flag)
	{
		m_editFamilyRole->setUserID(u_uid);
		m_editFamilyRole->setUName(uName);
		m_editFamilyRole->updateView(controFlags);
	}
	m_editFamilyRole->setVisible(flag) ;

}


void GameHall::showChangeShaikh(bool flag)
{
	if (m_changeViceShaikh == nullptr)
	{
		m_changeViceShaikh = ChangeViceShaikh::create();
		m_changeViceShaikh -> ignoreAnchorPointForPosition(false);
		m_changeViceShaikh -> setAnchorPoint(Point(0.5,0.5));
		m_changeViceShaikh->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_changeViceShaikh, EditRoleZOrder);
	}
	m_changeViceShaikh->setVisible(flag) ;
	if (flag)
	{
		

		Json::Value msg;
		msg["ufamilyid"] = g_globalMyData.getFamilyID();
		SendClientRequest(ClientRequest::kGetFamilyNoSKRole,msg);

	}
}
void GameHall::showInviteFriends(bool flag)
{
	if (m_inviteFriends == nullptr)
	{
		m_inviteFriends = InviteFriends::create();
		m_inviteFriends -> ignoreAnchorPointForPosition(false);
		m_inviteFriends -> setAnchorPoint(Point(0.5,0.5));
		m_inviteFriends->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_inviteFriends, EditRoleZOrder);
	}
	m_inviteFriends->setVisible(flag) ;
	if (flag)
	{

		Json::Value msg;
		msg["ufamilyid"] = g_globalMyData.getFamilyID();
		SendClientRequest(ClientRequest::kGetAllFriends,msg);

	}
}


void GameHall::showMobileBind(bool flag)
{
	if(m_pMobileBind == nullptr)
	{
		m_pMobileBind = MobileBind::create();
		//m_pMobileBind->setPosition((938 - 678) / 2,(640-410) / 2);

		m_pMobileBind -> ignoreAnchorPointForPosition(false);
		m_pMobileBind -> setAnchorPoint(Point(0.5,0.5));
		m_pMobileBind->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_pMobileBind,99);
	}

		m_pMobileBind->setVisible(flag);

}


void GameHall::showMobileCheck(bool flag,unsigned id)
{
	if(m_mobileCheck == nullptr)
	{
		m_mobileCheck = MobileCheck::create();
		//m_mobileCheck->setPosition((938 - 678) / 2,(640-410) / 2);
		m_mobileCheck -> ignoreAnchorPointForPosition(false);
		m_mobileCheck -> setAnchorPoint(Point(0.5,0.5));
		m_mobileCheck->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_mobileCheck,99);
	}
	
	m_mobileCheck->setVisible(flag);
	if (flag)
	{
		m_mobileCheck->setNewSkaikhID(id) ;
	}
}



void GameHall::callUpdateAdminList()
{
}


void GameHall::callUpdateTop5RankList()
{
}
void GameHall::callUpdateNoPowerRoleList()
{
	m_changeViceShaikh->updateView();
}

void GameHall::callUpdateFriendsList()
{
	m_inviteFriends->updateView() ;
}


void GameHall::callPushOutRole(unsigned uid)
{
	/*if (m_contributonRankList!=nullptr)
	{
	m_contributonRankList->RemoveItemView(uid);
	}*/
	if (m_selfClub!=nullptr)
	{
		m_selfClub->removeRole(uid) ;
	}
}

void GameHall::callUpdateRanlistItem(const FamilyRoleBaseInfo& baseInfo)
{
	m_selfClub->updateRoleItem( baseInfo) ;
	
}

void GameHall::callGetReward(long time)
{
}

void GameHall::callUpdateMessageFamily(const MessageInfo& info)
{
	m_pMessageList->updateMessageFamily(info);
}

void GameHall::updateShowNewMessage(bool bHasMessage)
{
	//bool show = GameData::getInstance()->getHasNewMessage() ;
	//show = true ;
	m_newMessageTips->setVisible(bHasMessage);

	if (m_pMessageList!=nullptr)
	{
		m_pMessageList->updateShowFamilyMessageTips(bHasMessage) ;
	}
}

void GameHall::checkNewMessage()
{

	Json::Value msg;

	//unsigned fid = g_globalMyData.getFamilyID();

	if (g_globalMyData.getHasCreatFamily())
	{
		msg["fid"] = g_globalMyData.getFamilyID();
	}
	else
	{
		msg["fid"] = 0;
	}
	
	SendClientRequest(ClientRequest::kCheckNewMessage,msg);


}

void GameHall::callCleanPower()
{
}

void GameHall::showDealApplyJoinFamily(bool flag,unsigned fid ,unsigned uid, unsigned mid,std::string uname,std::string uicon)
{
	if(m_dealApplyFamily == nullptr)
	{
		m_dealApplyFamily = DealApplyFamily::create() ;
		//m_mobileCheck->setPosition((938 - 678) / 2,(640-410) / 2);
		m_dealApplyFamily -> ignoreAnchorPointForPosition(false);
		m_dealApplyFamily -> setAnchorPoint(Point(0.5,0.5));
		m_dealApplyFamily->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_dealApplyFamily,99);
	}

	m_dealApplyFamily->setVisible(flag);
	if (flag)
	{
		m_dealApplyFamily->setInfo(fid,uid,mid,uname,uicon) ;
	}

}

void GameHall::removeInviteFriendsView(std::string uid)
{
	m_inviteFriends->removeRoleItem(uid) ;
}

void GameHall::callUpdateMobileCheckPhone(std::string  phone)
{
	m_mobileCheck->setBindPhone(phone) ;
}

void GameHall::callUpdateFamilyNotice(std::string sNotice)
{
	m_selfClub->updateNotice(sNotice) ;
}

void GameHall::callHideMessageLoading()
{
    if (m_pMessageList != nullptr)
	    m_pMessageList->hideMessageLoading() ;


}

bool GameHall::isMyFamilyVisible()
{
	if (m_selfClub == nullptr)
	{
		return false ;
	}
	return m_selfClub->isVisible() ;
}

void GameHall::showUserItem(bool flag)
{
	if (m_userItem == nullptr)
	{
		m_userItem = UserItem::create();
		m_userItem -> ignoreAnchorPointForPosition(false);
		m_userItem -> setAnchorPoint(Point(0.5,0.5));
		m_userItem->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		addChild(m_userItem,99);
	}

	m_userItem->setVisible(flag);
}

void GameHall::doHideUserItemLayer()
{
	if (m_userItem == nullptr)
	{
		return  ;
	}
	showUserItem(false);

}

void GameHall::updateUserItem()
{
	m_userItem->updateItemView() ;
}

void GameHall::PopDailySignReward(bool isCash)
{
    if (m_DailySignReward == nullptr)
    {
        m_DailySignReward = DailySignReward::create();
        auto sz = m_DailySignReward->getContentSize();
        m_DailySignReward->setPosition(GameData::getInstance()->getGameWidth()/2-sz.width/2, GameData::getInstance()->getGameHeight()/2-sz.height/2);
        addChild(m_DailySignReward, GetDailySignZOrder);
    }
	m_DailySignReward->updateView(isCash) ;
	m_DailySignReward->setVisible(true) ;
	
	m_pDailySign->updateView() ;
}

void GameHall::creatSngOnlineTips()
{
	
//	if (!GameData::getInstance()->getHasShowSngOnline())
//	{
//		if (m_tipsSngOnline== nullptr)
//		{
//			m_tipsSngOnline = SngOnline::create() ;
//			m_tipsSngOnline->ignoreAnchorPointForPosition(false) ;
//			m_tipsSngOnline->setAnchorPoint(Point(1.0,0.5)) ;
//			m_spriteSngOnline->addChild(m_tipsSngOnline,SngOnlineZOrder) ;
//			m_tipsSngOnline->setPositionY(20) ;
//		}
//
//
//#if OpenOneByOneTips
//		GameTipsManager::getInstance()->addTips(m_tipsSngOnline);
//#endif
//		
//	}
//	else
//	{
//		if (m_spriteNewTips!=nullptr)
//		{
//			m_spriteNewTips->setVisible(false) ;
//		}
//		if (m_tipsSngOnline != nullptr)
//		{
//			m_tipsSngOnline->setVisible(false) ;
//		}
//	}


}

void GameHall::showGameActivity()
{

}

void GameHall::creatActivityLayer()
{
    for (auto &activity : g_globalActivityInfo.m_vecPushActivityInfo)
    {
        if (activity.m_bIsShow && activity.m_uShowType == 0)//显示弹窗并且是半屏广告
        {
            auto activityRewards= PushActivities::create();
            Size sz = activityRewards->getContentSize();
            activityRewards->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
            addChild(activityRewards,PushActivitiesZOrder);
            activityRewards->UpdateActivateRewards(activity);
            activityRewards->setVisible(false) ;
            activity.m_bIsShow = false;
#if OpenOneByOneTips
            GameTipsManager::getInstance()->addTips(activityRewards);
#endif
        }
        else if (activity.m_bIsShow && activity.m_uShowType == 1)//显示弹窗并且是全屏广告
        {
            auto fullScreenAds = FullScreenAds::create();
            fullScreenAds->setPosition(0, 0);
            fullScreenAds->setVisible(false);
            activity.m_bIsShow = false;
            this->addChild(fullScreenAds, PushActivitiesZOrder+1);
#if OpenOneByOneTips
            GameTipsManager::getInstance()->addTips(fullScreenAds);
#endif
        }
    }
}

void GameHall::creatSiginLayer()
{
	unsigned uSignState = -1;


	for (int i=0;i<g_globalUserSigin.m_vecUserSigin.size();i++)
	{
		SiginDay daysigin = g_globalUserSigin.m_vecUserSigin.at(i) ;
		if (daysigin.m_state ==2)
		{
			uSignState = 1;
		}
	}


	if (uSignState == 1&&!g_globalUserSigin.m_hasShowSiginLayer&&!g_globalUserSigin.m_hasGetSiginReward)
	{
		showDailySign(false) ;
#if OpenOneByOneTips
		GameTipsManager::getInstance()->addTips(m_pDailySign);
#endif
	}
}

void GameHall::showDailySign(bool flag)
{
	if(m_pDailySign == nullptr)
	{
		m_pDailySign = DailySign::create();
		Size sz = m_pDailySign->getContentSize();
		m_pDailySign->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
		addChild(m_pDailySign, DailySignZOrder);
	} 

	m_pDailySign->setVisible(flag);
}

void GameHall::creatTeachLayer()
{
	if (g_globalMissionInfo.GetMissions(1).size() > 0)
	{
		if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_guide" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
		{
			if (m_pNewerTrain == nullptr)
			{
				m_pNewerTrain = NewerTrain::create(0);
				Size sz = m_pNewerTrain->getContentSize();
				m_pNewerTrain->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
				addChild(m_pNewerTrain, NewerTrainZOrder);
			}

			GameTipsManager::getInstance()->addTips(m_pNewerTrain);
		}
	}
}

void GameHall::creatGetVipRewardLayer()
{
	if(g_globalMyData.GetVipLevel() > 0 && g_globalMyData.m_bNeedShowVipAward)
	{
		if(m_pPrivilegesRewards == nullptr)
		{
			m_pPrivilegesRewards = PrivilegesRewards::create();
            Size sz = m_pPrivilegesRewards->getContentSize();
            m_pPrivilegesRewards->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
			addChild(m_pPrivilegesRewards,PrivilegesRewardsZOrder);
		}

		GameTipsManager::getInstance()->addTips(m_pPrivilegesRewards);
	}
}

void GameHall::createDelaerTips()
{
    if(GuideInfo->readGuideInfo(DelaerGuide))
        return;
    auto guideTips = GuideTips::create("guidetips/delaertips.png", "guidetips/delaerflag.png");
    this->addChild(guideTips, DelaerGuideTips);
    guideTips->setFlagPosition(Vec2(320, 490));
    guideTips->setBgPosition(Vec2(450, 335));
    guideTips->setTipsID(DelaerGuide);
    GameTipsManager::getInstance()->addTips(guideTips);
}



void GameHall::callScrollTextMove()
{
	m_moveTipsLayer->callScrollBannersMove() ;
}


/************************************************************************/
/* 重置滚动提示                                                                     */
/************************************************************************/
void GameHall::reSetScorlllTips()
{
	m_moveTipsLayer->reSetScorlllBanners() ;  
}

void GameHall::creatTipsLayer()
{

	if (!GameData::getInstance()->getHasRequestMatch())
	{
		return ;
	}

#if OpenOneByOneTips

	auto gametips = [this](){

		//sng上限
		creatSngOnlineTips() ;
		//活动界面
		creatActivityLayer() ;
		//登录界面
		creatSiginLayer() ;
		//教学界面
		creatTeachLayer() ;
		//
		creatGetVipRewardLayer() ;
		//加入德女郎桌提示

		//createDelaerTips(); 暂时没有德女郎 不给引导
		//德女郎奖励
		creatWaveGirlReward() ;

		GameTipsManager::getInstance()->hideTips() ;
	} ;
	this->runAction(Sequence::create(DelayTime::create(0.1f),CallFunc::create(gametips),NULL)) ;
	//this->runAction(Sequence::create(DelayTime::create(dDelay),CallFunc::create(gametips),NULL)) ;

#endif
}

void GameHall::creatWaveGirlReward()
{
	if(g_globalMyData.m_WaveGrilAward > 0)
	{
		if(m_pWaveGrilSign == nullptr)
		{
			m_pWaveGrilSign = WaveGirlRewards::create();
			Size sz = m_pWaveGrilSign->getContentSize();
			m_pWaveGrilSign->setPosition(GameData::getInstance()->getGameWidth()/2 - sz.width/2, GameData::getInstance()->getGameHeight()/2 - sz.height/2);
			addChild(m_pWaveGrilSign,WaveGirlRewardsZPrder);
		}

		GameTipsManager::getInstance()->addTips(m_pWaveGrilSign);
	}
}

void GameHall::createChristmas()
{
    m_snow->setVisible(false) ;
    if (ActManage->getChristmasState())
    {
        if (m_aniChristmasTree == nullptr)
        {
            ArmatureDataManager::getInstance()->addArmatureFileInfo("res/anims/christmas/shengdanhuodong_shengdanshushanguang.csb");
            m_aniChristmasTree = AnimationManager::creatArmature("shengdanhuodong_shengdanshushanguang");
            m_aniChristmasTree->setPosition(-220, -50);
            m_aniChristmasTree->playByIndexAndRepatForever(0);
            img_roleBody->addChild(m_aniChristmasTree, 0);
        }
        m_snow->setVisible(true) ;
        m_aniChristmasTree->setVisible(true);
        for (int i=0; i<5; i++)
        {
            m_christmasShow[i]->setVisible(true);
        }
    }
}

void GameHall::createSpringFestival()
{
    if (ActManage->getSpringFestivalState())
    {
        std::string aniPath[2] ={"2016newyearhall", "2016text"};
        for (int i=0; i<2; i++)
        {
            if (m_aniSpringFestival[i] == nullptr)
            {
                ArmatureDataManager::getInstance()->addArmatureFileInfo("res/anims/springfestival/"+aniPath[i]+".csb");
                m_aniSpringFestival[i] = AnimationManager::creatArmature(aniPath[i]);
                m_aniSpringFestival[i]->playByIndexAndRepatForever(0);
                m_aniSpringFestival[i]->ignoreAnchorPointForPosition(false);
                m_aniSpringFestival[i]->setAnchorPoint(Vec2(0.5, 0));
                m_aniSpringFestival[i]->setPosition(m_pSpriteTable->getContentSize().width/2, 25);
                if (i == 0)
                {
                    m_aniSpringFestival[i]->setPosition(m_pSpriteTable->getContentSize().width/2, 35);
                    m_pSpriteTable->addChild(m_aniSpringFestival[0]);
                }
                else
                {
                    m_aniSpringFestival[i]->setPosition(-194, -18);
                    img_roleBody->addChild(m_aniSpringFestival[1]);
                }
            }
        }
    }
}


void GameHall::onMenuMatchItemSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (pMenuItem == m_button_matchTouch)
		{
			showMathcLayer(false) ;
		}
		else if (pMenuItem == m_button_sngMatch)
		{
			showMathcLayer(false) ;
#if Open_Sng
			SendClientRequest(ClientRequest::kEnterGameSngHall) ;

#else
			PopTipWithBt1("该房间暂未开放，敬请期待！", "确定");
#endif
		}
		else if (pMenuItem == m_button_mttMatch)
		{
			showMathcLayer(false) ;
#if Open_Mtt
			SendClientRequest(ClientRequest::kEnterGameMttHall) ;

#else
			PopTipWithBt1("该房间暂未开放，敬请期待！", "确定");
#endif
		}
	}
}

void GameHall::showMathcLayer(bool flag)
{
	m_layout_match->setVisible(flag) ;
	if (flag)
	{
		m_layout_matchColorBG->setOpacity(0) ;

		m_layout_matchColorBG->runAction(FadeTo::create(0.5,255)) ;

		float px = 0 ;
		float py = 0 ;
		px = m_button_sngMatch->getPositionX() ;
		py = m_button_sngMatch->getPositionY() ;
		m_button_sngMatch->setPositionX(m_button_sngMatch->getPositionX()-640) ;

		m_button_sngMatch->runAction(MoveTo::create(0.2f,Point(px,py)));

		px = m_button_mttMatch->getPositionX() ;
		py = m_button_mttMatch->getPositionY() ;
		m_button_mttMatch->setPositionX(m_button_mttMatch->getPositionX()+640) ;



		auto actionOver = [this](){
			m_aniSngMatch->playByIndexAndRepatForever(0) ;
			m_aniMttMatch->playByIndexAndRepatForever(0) ;
		};

		m_button_mttMatch->runAction(Sequence::create(MoveTo::create(0.2f,Point(px,py)),CallFunc::create(actionOver),NULL));


	

	

	}
	else
	{
		m_aniSngMatch->stopAni() ;
		m_aniMttMatch->stopAni() ;
	}



}


unsigned GameHall::getTaskStatus()
{
    unsigned uStatus = 0;
    for (auto& mission : g_globalMissionInfo.GetMissions(0))//过滤每日任务
    {
        if (mission.uMissonStatus == 1)
        {
            uStatus = 1;
            break;
        }
    }
    for (auto& mission : g_globalMissionInfo.GetMissions(3))//过滤每周任务
    {
        if (mission.uMissonStatus == 1)
        {
            uStatus = 3;
            break;
        }
    }
    for (auto& mission : g_globalMissionInfo.GetMissions(2))//过滤成就任务
    {
        if (mission.uMissonStatus == 1)
        {
            uStatus = 2;
            break;
        }
    }
    return uStatus;

}

void GameHall::showGameClub(bool flag)
{
	if (m_GameClubHall == nullptr)
	{

		m_GameClubHall = GameClubHall::create();
		m_GameClubHall -> ignoreAnchorPointForPosition(false);
		m_GameClubHall->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		auto itemSz = m_GameClubHall->getContentSize();
		m_GameClubHall -> setAnchorPoint(Point(0.5,0.5));
		m_GameClubHall->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_GameClubHall, GameFamilyZOrder);
	}
	m_GameClubHall->setVisible(flag) ;
}

void GameHall::showSelfClub(bool flag,unsigned fid)
{
	if (m_selfClub == nullptr)
	{

		m_selfClub = SelfClub::create();
		m_selfClub -> ignoreAnchorPointForPosition(false);
		m_selfClub->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		auto itemSz = m_selfClub->getContentSize();
		m_selfClub -> setAnchorPoint(Point(0.5,0.5));
		m_selfClub->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_selfClub, GameFamilyZOrder+1);
	}

	if (flag)
	{
		Json::Value msg;
		msg["ufamilyid"] = fid;
		SendClientRequest(ClientRequest::kGetMyFamilyInfo,msg);
	}

	m_selfClub->setVisible(flag) ;
}



void GameHall::showAllotFund(bool flag,unsigned fid,unsigned uid,unsigned mid,std::string uname)
{
	if (m_allotFund == nullptr)
	{
		m_allotFund = AllotFund::create();
		m_allotFund -> ignoreAnchorPointForPosition(false);
		m_allotFund->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		auto itemSz = m_allotFund->getContentSize();
		m_allotFund -> setAnchorPoint(Point(0.5,0.5));
		m_allotFund->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_allotFund, GameFamilyZOrder+1);
	}
	m_allotFund->setVisible(flag) ;
	if (flag)
	{
		m_allotFund->setInfo(fid,uid,mid,uname) ;
	}
}

void GameHall::updateSelfClubRoleFundInfo(double clubFund,unsigned uid,double uFund)
{
	if (m_selfClub!=nullptr)
	{
		m_selfClub->updateRoleFund(clubFund,uid,uFund) ;
	}
}

void GameHall::updateSelfClubView()
{
	if (m_selfClub!=nullptr)
	{
		m_selfClub->updateSelfClubInfo() ;
	}
}

void GameHall::popMoreCallBack(Ref *pSend)
{
    m_popMoreItems->setVisible(false);
    if (pSend == m_pMenuMoreItem[0])
    {
        if (m_pGameActivity == nullptr)
        {
            m_pGameActivity = GameActivity::create();

            m_pGameActivity -> ignoreAnchorPointForPosition(false);
            m_pGameActivity -> setAnchorPoint(Point(0.5,0.5));
            m_pGameActivity->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
            addChild(m_pGameActivity, GameActivityZOrder);			
            TrackMgr->event("PopDialog", "活动弹窗");
        }
        m_layerColorBg->setVisible(true);
        m_pGameActivity->setVisible(true);
    }
    else if (pSend == m_pMenuMoreItem[1])
    {
        if (m_pFriendSpread == nullptr)
        {
            m_pFriendSpread = FriendsSpread::create();
            m_pFriendSpread->ignoreAnchorPointForPosition(false);
            m_pFriendSpread->setAnchorPoint(Vec2(0.5, 0.5));
            m_pFriendSpread->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
            this->addChild(m_pFriendSpread, FriendsSpreadZOrder);
            TrackMgr->event("PopDialog", "推广");
        }
        m_layerColorBg->setVisible(true);
        m_pFriendSpread->setVisible(true);
        m_pFriendSpread->updateSpreadInfo();
        SendClientRequest(ClientRequest::kGetMySpread);
    }
}

void GameHall::popBindSpreadResult(bool bSuccess)
{
    if (m_pFriendSpread != nullptr && m_pFriendSpread->getVisible())
    {
        m_pFriendSpread->popBindResult(bSuccess);
    }
}

void GameHall::updateSpreadList()
{
    if (m_pFriendSpread != nullptr)
        m_pFriendSpread->updateListView();
}

void GameHall::popMoreBottomItems()
{
    if (m_popMoreItems == nullptr)
    {
        auto posMore = m_pMenuItemBottom[7]->getPosition();
        m_popMoreItems = GameDialog::create("friendspread/dt_morebg.png", false);
        m_popMoreItems->ignoreAnchorPointForPosition(false);
        m_popMoreItems->setPosition(posMore.x+65, posMore.y+20);
        m_popMoreItems->setAnchorPoint(Vec2(1, 0));
        this->addChild(m_popMoreItems);

        m_pMenuMoreItem[0]  = MenuItemSpriteWithLabel::create("friendspread/dt_huodong.png", 2, CC_CALLBACK_1(GameHall::popMoreCallBack, this));
        m_pMenuMoreItem[0]->setPosition(60,64);
        auto label = CreateLabelMSYH(18, "活动");
        m_pMenuMoreItem[0]->SetLabel(label);
        label->setPositionY(label->getPositionY()-30);

        m_pMenuMoreItem[1]  = MenuItemSpriteWithLabel::create("friendspread/dt_tuiguang.png", 2, CC_CALLBACK_1(GameHall::popMoreCallBack, this));
        m_pMenuMoreItem[1]->setPosition(180,64);
        label = CreateLabelMSYH(18, "推广");
        m_pMenuMoreItem[1]->SetLabel(label);
        label->setPositionY(label->getPositionY()-30);

        auto menu = Menu::create(m_pMenuMoreItem[0], m_pMenuMoreItem[1], nullptr);
        menu->setPosition(Vec2::ZERO);
        m_popMoreItems->addChild(menu);
    }
    m_popMoreItems->setVisible(true);

//     Json::Value param;
//     param["spreadID"] = "";
//     SendClientRequest(ClientRequest::kBindSpread, param);
}

//void GameHall::showTargetClub(bool flag)
//{
//	if (m_selfClub == nullptr)
//	{
//
//		m_selfClub = SelfClub::create();
//		m_selfClub -> ignoreAnchorPointForPosition(false);
//		m_selfClub->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
//		auto itemSz = m_selfClub->getContentSize();
//		m_selfClub -> setAnchorPoint(Point(0.5,0.5));
//		m_selfClub->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
//		addChild(m_selfClub, GameFamilyZOrder+1);
//	}
//
//	m_selfClub->setVisible(flag) ;
//}