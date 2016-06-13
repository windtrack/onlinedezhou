#include "UserView.h"
#include "HttpSprite.h"
#include "Util.h"
#include "Card.h"
#include "FrameSprite.h"
#include "GameTable.h"
#include "GlobalUserData.h"
#include "AnimateViewer.h"
#include "AnimationManager.h"
#include "Tools.h"
USING_NS_CC;

enum
{
	UserViewW = 112,
	UserViewH = 164,
};


UserView* UserView::creatWithLayer(UserPos &pos)
{
	UserView* userVier = new UserView() ;
	if (userVier && userVier->initWithLayer(pos))
	{
		userVier->autorelease() ;
		return userVier ;
	}
	else
	{
		CC_SAFE_DELETE(userVier) ;
		return NULL ;
	}
}

bool UserView::initWithLayer(UserPos &pos)
{
	do 
	{
		if (!Layer::init())
		{
			return false ;
		}
		m_ligheMoveing = false ;
		m_clipNode = nullptr ;
		m_userPos = pos ;
		m_vipEffect = nullptr ;
		m_christmasEffect = nullptr ;
		m_spriteLight = nullptr ;
		m_aniNewYear = nullptr ;
		m_aniMonkey = nullptr ;
		m_bActivate = false ;
		m_isEffectShow = false ;
		setContentSize(Size(UserViewW, UserViewH));

	/*	this -> ignoreAnchorPointForPosition(false);
		this -> setAnchorPoint(Point(0.5,0.5));*/

		m_pSpriteBg = Sprite::create("userview/bgusrview.png");
		m_pSpriteBg->setPosition(UserViewW / 2, UserViewH / 2);
		addChild(m_pSpriteBg);

		m_pHttpSprite = HttpSprite::create("defaulticon.png", Size(100, 100));
		m_pHttpSprite -> ignoreAnchorPointForPosition(false);
		m_pHttpSprite -> setAnchorPoint(Point(0.5,0.5));

		//m_pHttpSprite->setPosition(8, 30);
		m_pHttpSprite->setPosition(pos.u_headPos);
		m_pHttpSprite->setCascadeOpacityEnabled(true);
		addChild(m_pHttpSprite);

		m_pAniShowWin = nullptr;


		m_pLabelNickName = CreateLabelMSYH(20);
		m_pLabelNickName->setTextColor(Color4B(255, 255, 255, 255));
		//m_pLabelNickName->setBMFontFilePath("fzlt.ttf");
		m_pLabelNickName -> ignoreAnchorPointForPosition(false);
		m_pLabelNickName -> setAnchorPoint(Point(0.5,0.5));
		//m_pLabelNickName->setPosition(UserViewW / 2, 122);
		m_pLabelNickName->setPosition(pos.u_namePos) ;
		m_pLabelNickName->setVisible(false);
		addChild(m_pLabelNickName, 1);
			
		m_pLabelUsrAction = CreateLabelMSYH(22);
		m_pLabelUsrAction->setTextColor(Color4B(0xff, 0xd2, 0, 255));
		m_pLabelUsrAction -> ignoreAnchorPointForPosition(false);
		m_pLabelUsrAction -> setAnchorPoint(Point(0.5,0.5));
		//m_pLabelUsrAction->setPosition(UserViewW / 2, 122);
		m_pLabelUsrAction->setPosition(pos.u_thinkPos);
		m_pLabelUsrAction->setVisible(false);
		addChild(m_pLabelUsrAction, 1);

		m_pLabelThinking = CreateLabelMSYH(22);
		m_pLabelThinking->setTextColor(Color4B(0xff, 0xd2, 0, 255));
		m_pLabelThinking -> ignoreAnchorPointForPosition(false);
		m_pLabelThinking -> setAnchorPoint(Point(0.5,0.5));
		//m_pLabelThinking->setBMFontFilePath("fzlt.ttf");
		//m_pLabelThinking->setPosition(UserViewW / 2, 122);
		m_pLabelThinking->setPosition(pos.u_thinkPos);
		m_pLabelThinking->setVisible(false);
		SetLabelString(m_pLabelThinking, "思考中");
		addChild(m_pLabelThinking, 1);

		m_pLabelCash = CreateLabelMSYH(22, "0");
		m_pLabelCash->setTextColor(Color4B(0xff, 0xd2, 00, 255));
		//m_pLabelCash->setTextColor(Color4B(0xff, 0xd2, 0, 255));
		//m_pLabelCash->setPosition(47, 14);
		m_pLabelCash -> ignoreAnchorPointForPosition(false);
		m_pLabelCash -> setAnchorPoint(Point(0.5,0.5));
		m_pLabelCash->setPosition(pos.u_cashPos);
		addChild(m_pLabelCash, 1);

		m_pLabelFold = CreateLabelMSYH(22);
		m_pLabelFold->setTextColor(Color4B(255, 255, 255, 255));
		//m_pLabelFold->setPosition(UserViewW / 2, 122);
		//m_pLabelFold->setBMFontFilePath("fzlt.ttf");
		m_pLabelFold -> ignoreAnchorPointForPosition(false);
		m_pLabelFold -> setAnchorPoint(Point(0.5,0.5));
		m_pLabelFold->setPosition(pos.u_thinkPos);
		m_pLabelFold->setVisible(false);
		SetLabelString(m_pLabelFold, "弃牌");
		addChild(m_pLabelFold, 1);

		// m_pSpriteAniThinking = CreateFrameSprite("thinking.png", 13, 10, 120);
		//m_pSpriteAniThinking = CreateFrameSprite("userview/thinking.png", 21, 12, 241);
		//m_pSpriteAniThinking->setVisible(false);
		//m_pSpriteAniThinking->setPosition(UserViewW / 2, UserViewH / 2);
		//addChild(m_pSpriteAniThinking, 1);


		initThinkingData();
		m_pSpriteAniThinking = Node::create() ;
		m_pSpriteAniThinking->addChild(s_thinkingLayer);
		m_pSpriteAniThinking->setVisible(false);
		m_pSpriteAniThinking->setPosition(UserViewW / 2, UserViewH / 2);
		addChild(m_pSpriteAniThinking, 1);


			
		m_pSpriteAniAllIn = AnimationManager::creatArmature("allin") ;
		//m_pSpriteAniAllIn = CreateFrameSprite("userview/allin.png", 7, 3);
		m_pSpriteAniAllIn->setVisible(false);
		//m_pSpriteAniAllIn->setPosition(UserViewW / 2, 122);
		m_pSpriteAniAllIn -> ignoreAnchorPointForPosition(false);
		m_pSpriteAniAllIn -> setAnchorPoint(Point(0.5,0.5));
		m_pSpriteAniAllIn->setPosition(pos.u_thinkPos);
		addChild(m_pSpriteAniAllIn, 1);

		m_pSpriteAniTipShowCard = AnimationManager::creatArmature("anitipshowcard") ;
		//m_pSpriteAniTipShowCard = CreateFrameSprite("userview/anitipshowcard.png", 10, 3);
		m_pSpriteAniTipShowCard->setVisible(false);
		//m_pSpriteAniTipShowCard->setPosition(UserViewW / 2, 122);
		m_pSpriteAniTipShowCard -> ignoreAnchorPointForPosition(false);
		m_pSpriteAniTipShowCard -> setAnchorPoint(Point(0.5,0.5));
		m_pSpriteAniTipShowCard->setPosition(pos.u_thinkPos);
		addChild(m_pSpriteAniTipShowCard, 1);

		static Vec2 s_ptCard[] = {Vec2(56, 82), Vec2(86, 82)};
		//static Vec2 s_ptCard[] = {Vec2(43, 82), Vec2(73, 82)};
		for (unsigned i = 0; i < 2; ++i)
		{
			m_pSpriteCard[i] = FrameSprite::create("publiccard.webp", 13, 5);// m_pCardFactory->CreateCard();
			m_pSpriteCard[i]->setScale(0.9);
			m_pSpriteCard[i]->setVisible(false);
			m_pSpriteCard[i]->setPosition(s_ptCard[i]);
			addChild(m_pSpriteCard[i], 1);

			Size sz = m_pSpriteCard[i]->getContentSize();
			m_pSpriteCardCash[i] = CreateNode1AndPosAddTo<Sprite>("publiccardshade.png", sz.width/2, sz.height/2, m_pSpriteCard[i], 0);
			//m_pSpriteCardCash[i]->setScale(0.9);
			m_pSpriteCardCash[i]->setVisible(false);
		}

		m_pSpriteWinType = CreateFrameSprite("userview/wintype.png", 1, 10);
		m_pSpriteWinType->setVisible(false);
		//m_pSpriteWinType->setPosition(UserViewW / 2, 122);
		m_pSpriteWinType -> ignoreAnchorPointForPosition(false);
		m_pSpriteWinType -> setAnchorPoint(Point(0.5,0.5));
		m_pSpriteWinType->setPosition(pos.u_thinkPos);
		addChild(m_pSpriteWinType, 1);

		m_pSpriteAniShowCard1 = CreateFrameSprite("userview/animateshowcard1.png", 4, 1);
		m_pSpriteAniShowCard1->setVisible(false);
		m_pSpriteAniShowCard1->setPosition(54, 82);
		addChild(m_pSpriteAniShowCard1, 1);

		m_pSpriteAniShowCard2 = CreateFrameSprite("userview/animateshowcard2.png", 2, 1);
		m_pSpriteAniShowCard2->setVisible(false);
		m_pSpriteAniShowCard2->setPosition(54, 82);
		addChild(m_pSpriteAniShowCard2, 1);

		m_pSpriteAniOpenCard = CreateFrameSprite("userview/animateopencard.png", 4, 1);
		m_pSpriteAniOpenCard->setVisible(false);
		m_pSpriteAniOpenCard->setPosition(54, 82);
		addChild(m_pSpriteAniOpenCard, 1);

		//m_pLayerAnimation = Layer::create();
		//m_pLayerAnimation->setContentSize(Size(UserViewW, UserViewH));
		//m_pLayerAnimation->setCascadeOpacityEnabled(false);
		//m_pLayerAnimation->setVisible(false);
		//addChild(m_pLayerAnimation);

		auto bgAniLayer = Layer::create();
		bgAniLayer->setContentSize(Size(UserViewW, UserViewH));
		bgAniLayer->setCascadeOpacityEnabled(false);
		addChild(bgAniLayer, 1);

		//m_pSpriteAnimation = Sprite::create();
		//m_pSpriteAnimation->setVisible(false);
		//m_pSpriteAnimation->setPosition(Vec2(UserViewW / 2, UserViewH / 2));
		//bgAniLayer->addChild(m_pSpriteAnimation, 1);
		//vip
		m_pSpriteVipSign = CreateNode3AndPosAddTo<FrameSprite>("vip.png", 6, 1, 7, 35, this, 10);
		m_pSpriteVipSign -> ignoreAnchorPointForPosition(false);
		m_pSpriteVipSign -> setAnchorPoint(Point(0.5,0.5));
		m_pSpriteVipSign->setPosition(pos.u_vipPos);
		m_pSpriteVipSign->setVisible(false);

		//标记
		m_pSpriteBiaoJi = CreateNode3AndPosAddTo<FrameSprite>("userinfo/playericons.png", 8, 1, pos.u_biaojiPos.x, pos.u_biaojiPos.y, this, 10);
		m_pSpriteBiaoJi -> ignoreAnchorPointForPosition(false);
		m_pSpriteBiaoJi -> setAnchorPoint(Point(0.5,0.5));
		m_pSpriteBiaoJi->setPosition(pos.u_biaojiPos);
		m_pSpriteBiaoJi->setVisible(false);

		//的女郎
		m_pSpriteLady = CreateNode3AndPosAddTo<FrameSprite>("quickfriend/level.png", 5, 1, pos.u_denvlangPos.x, pos.u_denvlangPos.y, this, 10);
		m_pSpriteLady -> ignoreAnchorPointForPosition(false);
		m_pSpriteLady -> setAnchorPoint(Point(0.5,0.5));
		m_pSpriteLady->setPosition(pos.u_denvlangPos);
		m_pSpriteLady->setVisible(false);

		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateNickName, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateThinking, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateActionCheck, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateActionCall, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateActionRaise, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateActionAllIn, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateActionFold, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateShowCard, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateOpenCard, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateWinType, this, std::placeholders::_1)));
		m_radioGroupTopView.AddGroupItem(RadioGroupItem(std::bind(&UserView::ActivateWaitNextGame, this, std::placeholders::_1)));

		m_radioGroupTopView.Activate(10);


		m_spriteRank = Sprite::create("gamesng/rank/sngrank2.png") ;
		m_spriteRank->setPosition(Vec2(UserViewW / 2, UserViewH / 2));
		this->addChild(m_spriteRank) ;
		m_spriteRank->setVisible(false) ;

		m_spriteRank->setCascadeColorEnabled(false) ;


		m_pAniShowWin = AnimationManager::creatArmature("showwin");
		m_pAniShowWin->setPosition(Vec2(UserViewW / 2, UserViewH / 2));
		m_pAniShowWin->setVisible(false);
		addChild(m_pAniShowWin);

		//m_pAniShowWin->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(UserView::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 



		//初始化表情动画


		return true ;
	} while (0);
	return false ;
}


bool UserView::init()
{
	if (!Layer::init())
	{
		return false;
	}
	

	return true;
}

void UserView::SetIconUrl(const std::string& strIconUrl)
{
    if (strIconUrl == "")
    {
        ::SetIconUrl(m_pHttpSprite, "zhongnan_3.jpg");
        m_strIconUrl = "zhongnan_3.jpg";
    }
	else if (/*!strIconUrl.empty() && */m_strIconUrl != strIconUrl) // 内网测试时，头像地址可能为空。。。
	{
		m_strIconUrl = strIconUrl;
		::SetIconUrl(m_pHttpSprite, m_strIconUrl);
	}
}

void UserView::SetVipSign(bool bIfVipSign, unsigned uLevel)
{
	if(uLevel >= 1 && uLevel <= 5)
	{
		m_pSpriteVipSign->SetFrameIdx(uLevel - 1);
		m_pSpriteVipSign->setVisible(true);
	}
	else
	{
		m_pSpriteVipSign->setVisible(false);
	}
	//if(bIfVipSign)
	//{
	//	m_pSpriteVipSign->setPosition(Vec2(2,35));
	//}
	//else
	//{
	//	m_pSpriteVipSign->setPosition(Vec2(95,35));	
	//}
	m_pSpriteVipSign->setPosition(m_userPos.u_vipPos);	
	
}

void UserView::SetNickName(const std::string& strNickName)
{
	if (m_strNickName != strNickName)
	{
		m_strNickName = strNickName;
		SetLabelStringWithBoundWidth(m_pLabelNickName, m_strNickName, UserViewW);
	}
}

void UserView::SetBiaoJi(int uBiaoJiIdx)
{
	if (uBiaoJiIdx >= 0)
	{
		//m_pSpriteBiaoJi->SetFrameIdx(uBiaoJiIdx-1);
        m_pSpriteBiaoJi->SetFrameIdx(uBiaoJiIdx);
		m_pSpriteBiaoJi->setVisible(true);
	}
	else
	{
		m_pSpriteBiaoJi->setVisible(false);
	}
}

void UserView::SetLady(unsigned uLadyIdx)
{

    if (uLadyIdx > 0)
    {
        m_pSpriteLady->SetFrameIdx(uLadyIdx - 1);
        m_pSpriteLady->setVisible(true);
    }
    else
    {
        m_pSpriteLady->setVisible(false);
    }
}

void UserView::ShowNickName()
{
	m_radioGroupTopView.Activate(0);
}

void UserView::ShowWaitNextGame()
{
	m_radioGroupTopView.Activate(10);
}

void UserView::SetCash(double dCash)
{
	SetLabelString(m_pLabelCash, FormatCash(dCash));
}

void UserView::ShowThinking()
{
	m_radioGroupTopView.Activate(1);
}

void UserView::ShowAction(UserAction userAction)
{
	m_radioGroupTopView.Activate(as_integer(userAction) + 2);
}
//亮牌
void UserView::ShowCard(unsigned char cbCardData[2])
{

	
	ResetCardData(m_pSpriteCard[0], cbCardData[0]);
	ResetCardData(m_pSpriteCard[1], cbCardData[1]);
	Vec2 s_ptCard[] = {Vec2(44, 82), Vec2(74, 77)};
	//static Vec2 s_ptCard[] = {Vec2(43, 82), Vec2(73, 82)};
	for (unsigned i = 0; i < 2; ++i)
	{
		m_pSpriteCard[i]->setPosition(s_ptCard[i]);
	}
	m_radioGroupTopView.Activate(7);
}
//开牌
void UserView::ShowOpenCard(unsigned char cbCardData[2])
{
	ResetCardData(m_pSpriteCard[0], cbCardData[0]);
	ResetCardData(m_pSpriteCard[1], cbCardData[1]);
	Vec2 s_ptCard[] = {Vec2(56, 92), Vec2(86, 92)};
	for (unsigned i = 0; i < 2; ++i)
	{
		m_pSpriteCard[i]->setPosition(s_ptCard[i]);
	}
	m_radioGroupTopView.Activate(8);
}

void UserView::ShowWinType(unsigned uWinType)
{
	m_uWinType = uWinType;
	if (m_uWinType)
	{
		Size sz = m_pSpriteWinType->getContentSize();
		m_pSpriteWinType->setTextureRect(Rect(0, (uWinType - 1) * sz.height, sz.width, sz.height));
	}
	m_radioGroupTopView.Activate(9);
}

void UserView::ActivateStatus(unsigned uActivateStatus)
{
	if (uActivateStatus == US_NORMAL)
	{
		m_radioGroupTopView.Activate(0);
	}
	else if (uActivateStatus == US_THINK)
	{
		m_radioGroupTopView.Activate(1);
	}
	else if (uActivateStatus == US_CHECK)
	{
		m_radioGroupTopView.Activate(2);
	}
	else if (uActivateStatus == US_CALL)
	{
		m_radioGroupTopView.Activate(3);
	}
	else if (uActivateStatus == US_RAISE)
	{
		m_radioGroupTopView.Activate(4);
	}
	else if (uActivateStatus == US_ALLIN)
	{
		m_radioGroupTopView.Activate(5);
	}
	else if (uActivateStatus == US_FOLD)
	{
		m_radioGroupTopView.Activate(6);
	}
	else if (uActivateStatus == US_OPENCARD)
	{
		m_radioGroupTopView.Activate(7);
	}
	else if (uActivateStatus == US_SHOWCARD)
	{
		m_radioGroupTopView.Activate(8);
	}
	else if (uActivateStatus == US_WINTYPE)
	{
		m_radioGroupTopView.Activate(9);
	}
	else if (uActivateStatus == US_WAITNEXTGAME)
	{
		m_radioGroupTopView.Activate(10);
	}
}
unsigned UserView::GetActivateFlag()
{
	return m_uActivateFlag;
}



void UserView::ShowExpress(unsigned uIdx)
{

	//m_biaoqingAni[0]->stopAni();
	//m_biaoqingAni[1]->stopAni();


	//int putongAni[2][16]={
	//	{
	//		0,9,14,8,11,10,12,2,1,7,3,15,4,5,6,13
	//	},
	//	{
	//		2,14,12,10,1,9,4,3,5,8,11,13,15,7,0,6
	//	},
	//} ;

	//m_biaoqingAni[uIdx/16]->playOnceByIndex(putongAni[uIdx/16][uIdx%16]);
}

void UserView::ActivateNickName(bool bActivate)
{
	m_pLabelNickName->setVisible(bActivate);
	if (bActivate)
	{
		m_uActivateFlag = US_NORMAL;
	}
}
void UserView::ActivateThinking(bool bActivate)
{
	m_pLabelUsrAction->setVisible(bActivate);
	if (bActivate)
	{		
		m_pLabelUsrAction->setTextColor(Color4B(0xff, 0xd2, 0, 255));
		SetLabelString(m_pLabelUsrAction, "思考中");
		// m_pSpriteAniThinking->runAction(Sequence::create(Show::create(), CreateAnimate("thinking.png", 13, 10, 120, 0.1f, false), Repeat::create(Blink::create(1.0f, 1), 1000), nullptr));
		//m_pSpriteAniThinking->runAction(Sequence::create(Show::create(), CreateAnimate("userview/thinking.png", 21, 12, 241, 0.05f, false), Repeat::create(Blink::create(1.0f, 1), 10000000), nullptr));

		runThinking();
		m_pSpriteAniThinking->setVisible(true) ;
		m_uActivateFlag = US_THINK;
	}
	else
	{
		stopThinking();
		m_pSpriteAniThinking->stopAllActions();
		m_pSpriteAniThinking->setVisible(false);
	}
}
void UserView::ActivateActionCheck(bool bActivate)
{
	if (bActivate)
	{
		m_pLabelUsrAction->setTextColor(Color4B(0x62, 0xc9, 0xff, 255));
		SetLabelString(m_pLabelUsrAction, "让牌");
		m_uActivateFlag = US_CHECK;
	}
	
	m_pLabelUsrAction->setVisible(bActivate);
}
void UserView::ActivateActionCall(bool bActivate)
{
	if (bActivate)
	{
		m_pLabelUsrAction->setTextColor(Color4B(0x36,0xe2,0x2c,0xff)) ;
		SetLabelString(m_pLabelUsrAction, "跟注");
		m_uActivateFlag = US_CALL;
	}
	m_pLabelUsrAction->setVisible(bActivate);
}
void UserView::ActivateActionRaise(bool bActivate)
{
	if (bActivate)
	{
		m_pLabelUsrAction->setTextColor(Color4B(0xff,0x87,0x00,0xff)) ;
		SetLabelString(m_pLabelUsrAction, "加注");
		m_uActivateFlag = US_RAISE;
	}
	m_pLabelUsrAction->setVisible(bActivate);
}
void UserView::ActivateActionAllIn(bool bActivate)
{
	if (bActivate)
	{
		m_pSpriteAniAllIn->setVisible(true);
		//m_pSpriteAniAllIn->runAction(Repeat::create(CreateAnimate("userview/allin.png", 7, 3, 21, 0.1f, false), 1000));
		m_pSpriteAniAllIn->playByIndexAndRepatForever(0);
		m_uActivateFlag = US_ALLIN;
	}
	else
	{
		m_pSpriteAniAllIn->stopAni();
		m_pSpriteAniAllIn->stopAllActions();
		m_pSpriteAniAllIn->setVisible(false);
	}
}
void UserView::ActivateActionFold(bool bActivate)
{
	m_pLabelFold->setVisible(bActivate);
	if (bActivate)
	{
		m_uActivateFlag = US_FOLD;
	}
	m_bActivate = bActivate ;
	setCascadeOpacityEnabled(bActivate);
	
	doFade() ;

}
void UserView::ActivateShowCard(bool bActivate)
{	
	if (bActivate)
	{
		m_uActivateFlag = US_SHOWCARD;
		auto pFunShow1 = [this]() {
			m_pSpriteCard[0]->setRotation3D(Vec3(0, 0, -8));
			m_pSpriteCard[0]->setVisible(true);
		};

		auto pFunShow2 = [this]() {
			m_pSpriteCard[1]->setRotation3D(Vec3(0, 0, 8));
			m_pSpriteCard[1]->setVisible(true);
		};

		m_pSpriteAniShowCard1->runAction(Sequence::create(
			Show::create(),
			CreateAnimate("userview/animateshowcard1.png", 4, 1, 4, 0.1f, false), 
			Hide::create(),
			CallFunc::create(pFunShow1), nullptr));
		m_pSpriteAniShowCard2->runAction(Sequence::create(
			DelayTime::create(0.4),
			Show::create(),
			CreateAnimate("userview/animateshowcard2.png", 2, 1, 2, 0.1f, false), 
			Hide::create(),
			CallFunc::create(pFunShow2), nullptr));
/*		m_pSpriteAniTipShowCard->runAction(Sequence::create(
			Show::create(),
			Repeat::create(CreateAnimate("userview/anitipshowcard.png", 10, 3, 30, 0.1f, false), 100), nullptr))*/;
		m_pSpriteAniTipShowCard->playByIndexAndRepatForever(0);
	}
	else
	{
		m_pSpriteAniShowCard1->stopAllActions();
		m_pSpriteAniShowCard1->setVisible(false);
		m_pSpriteAniShowCard2->stopAllActions();
		m_pSpriteAniShowCard2->setVisible(false);
		m_pSpriteCard[0]->setVisible(false);
		m_pSpriteCard[1]->setVisible(false);

		m_pSpriteAniTipShowCard->stopAni();
		m_pSpriteAniTipShowCard->stopAllActions();
		m_pSpriteAniTipShowCard->setVisible(false);
	}	
}

void UserView::ActivateOpenCard(bool bActivate)
{
	//m_pLabelNickName->setVisible(bActivate);
	if (bActivate)
	{
        m_pLabelUsrAction->setVisible(bActivate);
        if (m_uActivateFlag == US_ALLIN)
        {
            m_pSpriteAniAllIn->setVisible(true);
            m_pSpriteAniAllIn->playByIndexAndRepatForever(0);
            m_pLabelUsrAction->setVisible(false);
        }
        else if (m_uActivateFlag == US_CHECK)
        {
			m_pLabelUsrAction->setTextColor(Color4B(0xff, 0xd2, 0, 255));
            SetLabelString(m_pLabelUsrAction, "让牌");
        }
        else if (m_uActivateFlag == US_CALL)
        {
			m_pLabelUsrAction->setTextColor(Color4B(0x36,0xe2,0x2c,0xff)) ;
            SetLabelString(m_pLabelUsrAction, "跟注");
        }
        else if (m_uActivateFlag == US_RAISE)
        {
			m_pLabelUsrAction->setTextColor(Color4B(0xff,0x87,0x00,0xff)) ;
            SetLabelString(m_pLabelUsrAction, "加注");
        }
		m_uActivateFlag = US_OPENCARD;

		auto pFunOpen = [this]() {
            SendClientRequest(ClientRequest::kCallHideHandCard);//开牌前先隐藏手牌(MTT场开局升盲金币不足，导致自动all in发手牌和开牌动画一起执行，导致手牌部分没有隐藏)
			m_pSpriteCard[0]->setRotation3D(Vec3(0, 0, 0));
			m_pSpriteCard[1]->setRotation3D(Vec3(0, 0, 0));
			m_pSpriteCard[0]->setVisible(true);
			m_pSpriteCard[1]->setVisible(true);
			m_pSpriteCard[0]->setPosition(37, 82);
			m_pSpriteCard[1]->setPosition(74, 82);
			//m_pSpriteCard[0]->setPosition(33, 70);
			//m_pSpriteCard[1]->setPosition(64, 70);
            //SendClientRequest(ClientRequest::kCallHideHandCard);//开牌前先隐藏手牌(MTT场开局升盲金币不足，导致自动all in发手牌和开牌动画一起执行，导致手牌部分没有隐藏)
			m_pSpriteCard[1]->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-30, 0)), MoveBy::create(0.6f, Vec2(33, 0)), MoveBy::create(0.2f, Vec2(-3, 0)), nullptr));
		};
		m_pSpriteAniOpenCard->runAction(Sequence::create(
			Show::create(),
			CreateAnimate("userview/animateopencard.png", 4, 1, 4, 0.1f, false), 
			Hide::create(),
			CallFunc::create(pFunOpen), 
			nullptr));
	}
	else
	{
        m_pLabelUsrAction->setVisible(bActivate);
        m_pSpriteAniAllIn->stopAni();
        m_pSpriteAniAllIn->stopAllActions();
        m_pSpriteAniAllIn->setVisible(false);

		m_pSpriteAniOpenCard->stopAllActions();
		m_pSpriteCard[1]->stopAllActions();
		m_pSpriteCard[0]->setVisible(false);
		m_pSpriteCard[1]->setVisible(false);
		m_pSpriteAniOpenCard->setVisible(false);
	}
}

void UserView::ActivateWinType(bool bActivate)
{
	if (m_uWinType)
	{
		m_pSpriteCard[0]->setVisible(bActivate);
		m_pSpriteCard[1]->setVisible(bActivate);
		m_pSpriteWinType->setVisible(bActivate);
	}
	else
	{
		m_pLabelNickName->setVisible(bActivate);
	}
	unsigned uAni = 0;
	if (0 == m_uWinType)
	{
		uAni = 0;
	}
	else if (m_uWinType >= 1 && m_uWinType <= 6)
	{
		uAni = 1;
	}
	else
	{
		uAni = 2;
	}
	if (bActivate)
	{
		m_uActivateFlag = US_WINTYPE;


		//m_pAniShowWin->setVisible(true);
		//m_pAniShowWin->getAnimation()->playByIndex(2-uAni) ;
		m_pAniShowWin->playByIndexAndRepatForever(2-uAni);

	}
	else
	{
		m_pAniShowWin->stopAni();
		m_pAniShowWin->setVisible(false);

	}
}

void UserView::StopWinAction()
{
    m_pAniShowWin->stopAni();
    m_pAniShowWin->setVisible(false);
    m_pSpriteWinType->setVisible(false);
    m_pLabelNickName->setVisible(true);
    m_pLabelUsrAction->setVisible(false);
}

void UserView::ActivateWaitNextGame(bool bActivate)
{
	m_pLabelNickName->setVisible(bActivate);
	if (bActivate)
	{
		m_uActivateFlag = US_WAITNEXTGAME;
	}
	m_bActivate = bActivate ;
	doFade() ;
}

void UserView::DryCashMinHandCard(unsigned idx)
{
    if (idx == 0)
    {
        m_pSpriteCardCash[0]->setVisible(true);
    }
    else if (idx == 1)
    {
        m_pSpriteCardCash[1]->setVisible(true);
    }
    else if (idx == 2)
    {
        m_pSpriteCardCash[0]->setVisible(false);
        m_pSpriteCardCash[1]->setVisible(false);
    }
    
}




/************************************************************************/
/* 时间进度                                                                     */
/************************************************************************/


void UserView::initThinkingData()
{
	s_maxThinkingTime = 12.3 ;
	float s_width = 118 ;
	float s_height = 172 ;

	s_thinkingLayer = Node::create() ;

	//载入资源
	//SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();  
	//cache->addSpriteFramesWithFile("userview/usethinking.plist", "userview/usethinking.png");  
	s_thinkingLayer->setContentSize(Size(s_width,s_height)) ;

	s_thinkingLayer->setScaleX(-1) ;//镜像显示

	float s_allWidth[THINKINGPART] ;
	for (int i=0;i<THINKINGPART;i++)
	{
		char imgName[64] ;
		sprintf(imgName,"userview/usethinking/thinking%d.png",i) ;
		//spriteThinking[i] = Sprite::createWithSpriteFrame(cache->spriteFrameByName(imgName));  
		spriteThinking[i] = Sprite::create(imgName);
		if (i==2||i==6)
		{
			s_allWidth[i] = spriteThinking[i]->getContentSize().height ;
		}
		else
		{
			s_allWidth[i] = spriteThinking[i]->getContentSize().width ;
		}

		s_ThinkingTime[i] = s_allWidth[i]*s_maxThinkingTime/(s_width*2+s_height*2-12*4);
	}



	spriteThinking[0]->setPosition(0 -spriteThinking[0]->getContentSize().width/2, s_height/2-spriteThinking[1]->getContentSize().height/2) ;
	spriteThinking[1]->setPosition(-s_width/2+spriteThinking[1]->getContentSize().width/2 ,s_height/2-spriteThinking[1]->getContentSize().height/2) ;
	spriteThinking[2]->setPosition(-s_width/2+spriteThinking[2]->getContentSize().width/2 ,0) ;
	spriteThinking[3]->setPosition(-s_width/2+spriteThinking[3]->getContentSize().width/2 ,-s_height/2+spriteThinking[3]->getContentSize().height/2) ;
	spriteThinking[4]->setPosition(0 ,-s_height/2+spriteThinking[4]->getContentSize().height/2) ;
	spriteThinking[5]->setPosition(s_width/2-spriteThinking[5]->getContentSize().width/2 ,-s_height/2+spriteThinking[4]->getContentSize().height/2) ;
	spriteThinking[6]->setPosition(s_width/2-spriteThinking[5]->getContentSize().width/2,0) ;
	spriteThinking[7]->setPosition(s_width/2-spriteThinking[6]->getContentSize().width/2 ,s_height/2-spriteThinking[7]->getContentSize().height/2) ;
	spriteThinking[8]->setPosition(spriteThinking[0]->getContentSize().width/2,s_height/2-spriteThinking[1]->getContentSize().height/2) ;


	//第一阶段
	s_ProgressTimer[0] =creatProgressTimeBar(spriteThinking[0],ProgressTimer::Type::BAR,Vec2(0,0),Vec2(1, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[0]);
	s_ProgressTimer[0]->setPosition(0 -spriteThinking[0]->getContentSize().width/2, s_height/2-spriteThinking[1]->getContentSize().height/2) ;

	//第二阶段//第一个拐角
	s_ProgressTimer[1] =creatProgressTimeBar(spriteThinking[1],ProgressTimer::Type::RADIAL,Vec2(0.9999,0.00001),Vec2(0, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[1]);
	s_ProgressTimer[1]->setPosition(-s_width/2+spriteThinking[1]->getContentSize().width/2 ,s_height/2-spriteThinking[1]->getContentSize().height/2) ;

	//第三阶段
	s_ProgressTimer[2] =creatProgressTimeBar(spriteThinking[2],ProgressTimer::Type::BAR,Vec2(0,0),Vec2(0, 1)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[2]);
	s_ProgressTimer[2]->setPosition(-s_width/2+spriteThinking[2]->getContentSize().width/2 ,0) ;

	//第四阶段//第二个拐角
	s_ProgressTimer[3] =creatProgressTimeBar(spriteThinking[3],ProgressTimer::Type::RADIAL,Vec2(0.999,0.999),Vec2(0, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[3]);
	s_ProgressTimer[3]->setPosition(-s_width/2+spriteThinking[3]->getContentSize().width/2 ,-s_height/2+spriteThinking[3]->getContentSize().height/2) ;

	//第五阶段
	s_ProgressTimer[4] =creatProgressTimeBar(spriteThinking[4],ProgressTimer::Type::BAR,Vec2(1,0),Vec2(1, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[4]);
	s_ProgressTimer[4]->setPosition(0 ,-s_height/2+spriteThinking[4]->getContentSize().height/2) ;

	//第六阶段//第3个拐角
	s_ProgressTimer[5] =creatProgressTimeBar(spriteThinking[5],ProgressTimer::Type::RADIAL,Vec2(0.0001,0.9999),Vec2(0, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[5]);
	s_ProgressTimer[5]->setPosition(s_width/2-spriteThinking[5]->getContentSize().width/2 ,-s_height/2+spriteThinking[4]->getContentSize().height/2) ;

	//第七阶段
	s_ProgressTimer[6] =creatProgressTimeBar(spriteThinking[6],ProgressTimer::Type::BAR,Vec2(0,1),Vec2(0, 1)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[6]);
	s_ProgressTimer[6]->setPosition(s_width/2-spriteThinking[5]->getContentSize().width/2,0) ;

	//第八阶段//第四个拐角
	s_ProgressTimer[7] =creatProgressTimeBar(spriteThinking[7],ProgressTimer::Type::RADIAL,Vec2(0.0001,0.0001),Vec2(0, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[7]);
	s_ProgressTimer[7]->setPosition(s_width/2-spriteThinking[6]->getContentSize().width/2 ,s_height/2-spriteThinking[7]->getContentSize().height/2) ;

	//第九阶段
	s_ProgressTimer[8] =creatProgressTimeBar(spriteThinking[8],ProgressTimer::Type::BAR,Vec2(0,0),Vec2(1, 0)) ;
	s_thinkingLayer->addChild(s_ProgressTimer[8]);
	s_ProgressTimer[8]->setPosition(spriteThinking[0]->getContentSize().width/2,s_height/2-spriteThinking[1]->getContentSize().height/2) ;

	//最后闪点
	//spritePoint = Sprite::createWithSpriteFrame(cache->spriteFrameByName("point.png"));
	spritePoint = Sprite::create("userview/usethinking/point.png") ;
	//userview/usethinking/
	spritePoint->setPosition(0,s_height/2-spritePoint->getContentSize().height/2) ;
	spritePoint->setVisible(false) ;
	s_thinkingLayer->addChild(spritePoint,10) ;

	//颜色改变的控制器
	nodeColorAction = Node::create() ;
	s_thinkingLayer->addChild(nodeColorAction) ;

	reSetThinkingBar() ;


}

void UserView::runThinking()
{
	reSetThinkingBar() ;
	updateThinkingColor(0) ;
	s_thinkingLayer->setVisible(true) ;
	CallFuncN * funcall = CallFuncN::create(CC_CALLBACK_1(UserView::updateThinking, this,1));
	FiniteTimeAction* s_seq = Sequence::create(ProgressTo::create(s_ThinkingTime[0], 0),funcall,NULL);  
	s_ProgressTimer[0]->runAction(s_seq);
}

void UserView::stopThinking()
{
	for (int i = 0; i < THINKINGPART; i++)
	{
		s_ProgressTimer[i]->stopAllActions() ;
		s_ProgressTimer[i]->setVisible(false);
	}
	nodeColorAction->stopAllActions() ;
	s_thinkingLayer->setVisible(false);

}

void UserView::updateThinkingColor(int index)
{
	if (index>=7)
	{
		return ;
	}

	ActionInterval * tintby[THINKINGPART] ;
	float s_colorTime[8]= {0.05,0.08,0.2,1.4,4.0,6.4,8.8,10} ;//颜色改变的时间轴
	int s_colorRGB[7][3] =
	{
		{255,0,0},	
		{0,255,23},
		{56,255,23},
		{216,255,23},
		{254,171,10},
		{255,0,0},
		{255,0,0},
	};

	float changetime = s_colorTime[index+1]-s_colorTime[index];
	for (int i=0;i<THINKINGPART;i++)
	{
		tintby[i]	= TintTo::create(changetime, s_colorRGB[index][0], s_colorRGB[index][1], s_colorRGB[index][2]);  
	}

	CallFunc * funcall=CallFunc::create(CC_CALLBACK_0(UserView::updateThinkingColor, this,index+1));
	FiniteTimeAction * seq =Sequence::create( DelayTime::create(changetime),funcall,NULL); 

	for (int i = 0; i < THINKINGPART; i++)
	{
		s_ProgressTimer[i]->runAction( tintby[i]);
	}
	nodeColorAction->runAction( seq);

}

void UserView::updateThinking(Ref* pSender,int index)
{

	if(index>=THINKINGPART)
	{
		callPointShow() ;
		return ;
	}

	CallFuncN * funcall2 = CallFuncN::create(CC_CALLBACK_1(UserView::updateThinking, this,index+1));
	FiniteTimeAction* seq = Sequence::create(ProgressTo::create(s_ThinkingTime[index], 0),funcall2,NULL);  
	if (index==1)
	{
		seq = Sequence::create(ProgressFromTo::create(s_ThinkingTime[index],100,75),funcall2,NULL);  
	}
	if (index==3)
	{
		seq = Sequence::create(ProgressFromTo::create(s_ThinkingTime[index],75,50),funcall2,NULL);  
	}
	if (index==5)	
	{
		seq = Sequence::create(ProgressFromTo::create(s_ThinkingTime[index],50,25),funcall2,NULL);  
	}
	if (index==7)
	{
		seq = Sequence::create(ProgressFromTo::create(s_ThinkingTime[index],25,0),funcall2,NULL);  
	}
	s_ProgressTimer[index-1]->setVisible(false) ;
	s_ProgressTimer[index]->runAction( seq);

}

ProgressTimer* UserView::creatProgressTimeBar(cocos2d::Sprite* spriteImg,cocos2d::ProgressTimer::Type type, cocos2d::Vec2 midpoint,  cocos2d::Vec2 barChangeRate)
{
	ProgressTimer* proGresstime= ProgressTimer::create(spriteImg);;
	proGresstime->setType(type);
	proGresstime->setMidpoint(midpoint);
	proGresstime->setBarChangeRate(barChangeRate);
	return proGresstime ;
}


void UserView::reSetThinkingBar()
{
	spritePoint->setVisible(false) ;
	spritePoint->stopAllActions() ;
	for (int i = 0; i <THINKINGPART; i++)
	{
		s_ProgressTimer[i]->setPercentage(100) ;
		s_ProgressTimer[i]->setVisible(true) ;
		s_ProgressTimer[i]->setColor(Color3B(255,0,0)) ;
	}
}

void UserView::callPointShow( )
{
	spritePoint->setVisible(true) ;
	spritePoint->runAction( Repeat::create(Blink::create(1.0f, 1), 10000000)) ;
}



void UserView::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{

	//if (movementType == COMPLETE)
	//{
	//	return ;
	//}
	if (movementType == LOOP_COMPLETE||movementType == COMPLETE)
	{
		ShowNickName() ;
		return ;
	}

}

void UserView::reset()
{
	//m_spriteRank->setVisible(false) ;
	this->stopActionByTag(99) ;
	this->stopActionByTag(101) ;
	showSngRank(false,nullptr) ;
	stopAllActions();
	stopThinking();
	//m_pHttpSprite->ResetToDefaultSprite();
    ActivateStatus(US_NORMAL);

	if (m_christmasEffect != nullptr)
	{
		m_christmasEffect->setVisible(false) ;
	}
	if (m_vipEffect != nullptr)
	{
		m_vipEffect->setVisible(false) ;
	}
	m_ligheMoveing = false ;
	if (m_spriteLight!=nullptr)
	{
		m_spriteLight->setVisible(false) ;
	}
	

	this->setVisible(false);
	m_ligheMoveing = false ;
	m_isEffectShow = false ;
	m_bActivate = false ;
}


void UserView::showSngRank(bool flag ,cocos2d::Texture2D* spriteRank)
{
	m_spriteRank->setVisible(flag) ;
	//m_spriteRank->setContentSize(spriteRank->getContentSize()) ;

	if (flag)
	{
			m_spriteRank->setTexture(spriteRank) ;
	}

	
}


void UserView::showJoinTableEffect(int vipLev, int skinid)
{
	if (skinid ==15)
	{
		m_pHttpSprite->ResetToLocalSprite("effect/icon_monkeyheader.png") ;
	}


	m_isEffectShow = true ;

	doFade();

	this->setScale(1.8) ;

	auto scaleAction = ScaleTo::create(0.1,1.0f) ;
	auto shankAction = [](){
		g_pGameFrame->doShankeTable(0,-5,0.07,3) ;
	} ;

	float m_dealyTime = 0.0f;

	if (m_clipNode == nullptr)
	{
		auto mmoban = Sprite::create("effect/defaulticonsmall.png") ;
		mmoban->setContentSize(Size(100,100)) ;
		auto sz = mmoban->getContentSize();
		mmoban->setPosition(0,0);
		mmoban->setAnchorPoint(Point(0.5,0.5)) ;  
		mmoban->retain();

		m_clipNode = ClippingNode::create();
		m_clipNode->setStencil(mmoban);     //设置模板
		//m_clipNode->setInverted(true);        //设置底板可见
		m_clipNode->setAlphaThreshold(0.5);         //设置绘制底板的Alpha值为0
		m_clipNode->setPosition(m_userPos.u_headPos) ;
		m_clipNode->setContentSize(sz);
		this->addChild(m_clipNode,20);

		m_spriteLight = Sprite::create("effect/viplight.png") ;
		m_spriteLight->ignoreAnchorPointForPosition(false) ;
		m_spriteLight->setAnchorPoint(Point(0.5,0.5)) ;
		m_spriteLight->setPosition(-m_spriteLight->getContentSize().width/2,m_spriteLight->getContentSize().height/2) ;
		m_spriteLight->setRotation(-45) ;
		m_clipNode->addChild(m_spriteLight);
		m_spriteLight->setTag(101) ;
		m_spriteLight->setVisible(false) ;
	}


	auto effectAction = [this,vipLev,skinid](){
		Size sz = this->getContentSize() ;

		if (skinid ==14)//圣诞女
		{
		
			if (m_christmasEffect == nullptr)
			{
				m_christmasEffect =  AnimateViewer::create("effect/christmasSkin.png", true, 1/ 15.0f, 4, 3, 12,1);
				m_christmasEffect->setPosition(sz.width/2, sz.height/2);
				this->addChild(m_christmasEffect,10);
				m_christmasEffect->SetFinishCB(std::bind(&UserView::callChristmasEffectFinished,this)) ;
			}
			m_christmasEffect->setVisible(true);

			PlayEffectSound("sound/skin/jointable/goin.mp3", false);
		}

		else if (skinid ==15) //新春猴子男
		{
			
			m_pHttpSprite->ResetToLocalSprite("effect/icon_monkeyheader.png") ;


			if (m_aniMonkey == nullptr)
			{
				m_aniMonkey = AnimationManager::creatArmature("xinchun_yanwu") ;
				m_aniMonkey->setPosition(sz.width/2, sz.height/2);
				this->addChild(m_aniMonkey,11);
				m_aniMonkey->SetFinishCB(std::bind(&UserView::callNewYearEffectOver,this)) ;
			}
				m_aniMonkey->playOnceByIndex(0) ;

				auto actionchangeHead = [this](){
					::SetIconUrl(m_pHttpSprite, m_strIconUrl);

				} ;
				this->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create(actionchangeHead),NULL)) ;

				PlayEffectSound("sound/skin/jointable/goin.mp3", false);
		}


		else
		{

		}

	} ;

	if (skinid ==14)//圣诞女
	{
		m_dealyTime = 0 ;
	}else if (skinid ==15)
	{
		m_dealyTime = 0.3f ;
	}

	this->runAction(Sequence::create(DelayTime::create(m_dealyTime),
		EaseSineOut::create(scaleAction),CallFunc::create(shankAction),CallFunc::create(effectAction),NULL)) ;


}

void UserView::callVipEffectFinished()
{
	m_vipEffect->stopAllActions() ;
	m_vipEffect->setVisible(false) ;

	callLightMove() ;


}

void UserView::callChristmasEffectFinished()
{
	m_christmasEffect->stopAllActions() ;
	m_christmasEffect->setVisible(false) ;
	callLightMove() ;
}


void UserView::callNewYearEffectOver()
{

	//::SetIconUrl(m_pHttpSprite, m_strIconUrl);
	//callLightMove() ;
	creatVipLight() ;

}


void UserView::creatVipLight()
{
	Size sz = this->getContentSize() ;
	if (m_vipEffect == nullptr)
	{
		m_vipEffect =  AnimateViewer::create("effect/vipjointable.png", true, 1/ 25.0f, 3, 3, 9,1);
		m_vipEffect->setPosition(sz.width/2, sz.height/2);
		this->addChild(m_vipEffect,9);
		m_vipEffect->SetFinishCB(std::bind(&UserView::callVipEffectFinished,this)) ;
	}
	m_vipEffect->setVisible(true);
}

void UserView::callLightMove()
{


	if (!m_ligheMoveing)
	{
		m_spriteLight->stopAllActions() ;
		Size sz = this->getContentSize() ;
		m_ligheMoveing = true ;
		m_spriteLight->setVisible(true) ;
		m_spriteLight->setPosition(Point(-m_spriteLight->getContentSize().width/2-35,100)) ;

		auto actionHide =  [this](){

			m_isEffectShow = false ;
			doFade() ;
			m_spriteLight->setVisible(false) ;
			m_ligheMoveing = false ;
		} ;
		m_spriteLight->runAction(Sequence::create(MoveTo::create(0.65f,Point(100,0)),CallFunc::create(actionHide),NULL)) ;
	}
}

void UserView::doFade()
{

	if (m_isEffectShow)
	{
		this->stopActionByTag(99) ;
		setCascadeOpacityEnabled(true);
		ActionInterval* action = FadeTo::create(0.2f,255);
		action->setTag(99) ;
		this->runAction(action);
	}
	else
	{
		this->stopActionByTag(99) ;
		setCascadeOpacityEnabled(m_bActivate);
		ActionInterval* action = FadeTo::create(0.3f,m_bActivate ? 100 : 255);
		action->setTag(99) ;
		this->runAction(action);
	}


}