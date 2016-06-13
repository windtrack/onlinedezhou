#include "GameTablePlayer.h"
#include "JsonValueAdapter.h"
#include "GameCommon.h"
#include "Util.h"
#include "MenuItemFrameSprite.h"
#include "Jetton.h"
#include "UserView.h"
#include "Card.h"
#include "FrameSprite.h"
#include "GlobalUserData.h"
#include "TextureNumber.h"
#include "Delaer.h"
#include "GameLogic.h"
#include "GameMessageIO.h"
#include "Tools.h"
#include "AnimationManager.h"
#include "ActionBar.h"
#include "GameData.h"
#include "CfgReader.h"


USING_NS_CC;

#define MY_CHAIR_ID 4

enum
{
	ProgressWidth = 796
};

enum
{
	TableBGZOrder,
	DealerZOrder,
	UserViewZOrder,
	DispatchingCardZOrder,
	DFlagZOrder,
	MyCardTypeZOrder,
	MyCardZOrder,
	AniShowMaxCardZOrder,
	PublicCardZOrder,
	AniShowMeWinZOrder,
	SidePotZOrder, // 边池
	UserPotZOrder, // 用户池
	BetJettonZOrder,
	FoldingCardZOrder,
	WinJettonZOrder,
	WinNumberZOrder,
	ChatTipZOrder,
	QuickMissionListZOrder,
	QuickInviteListZOrder,
	RewardZOrder,
	ToolZOrder,
	ActionBarZOrder,
	DoorMenuZOrder,
	CardTypeViewZOrder,
	QuickShopZOrder,
	ChatBoxZOrder,
	UserInfoZOrder,
	TableTipZOrder,
};

//static Vec2 s_ptFoldingCard[GAME_PLAYER] = {
//	Vec2(/*776*/752,441),Vec2(925,343),Vec2(925,177),Vec2(776,121),
//	Vec2(546,140),
//	Vec2(156,121),Vec2(47,177),Vec2(47,343),Vec2(/*196*/220,441)
//};


//static Vec2 s_ptDPosition[GAME_PLAYER] = {
//	Vec2(635, 459), Vec2(808, 315), Vec2(808, 231), Vec2(659, 166),
//	Vec2(410, 166),
//	Vec2(301, 166), Vec2(152, 231), Vec2(152, 315), Vec2(325, 459),
//};
//static Vec2 s_allinChizi[GAME_PLAYER]={
//
//	Vec2(),Vec2(),Vec2(),Vec2(),Vec2(),Vec2(),Vec2(),Vec2(),Vec2(),
//};
//
//static Vec2 s_ptDispatchedCard[GAME_PLAYER*2] = {
//	Vec2(746, 445/*770,445*/),Vec2(919,347),Vec2(919,181),Vec2(770,125),
//	Vec2(520,125),
//	Vec2(190,125),Vec2(41,181),Vec2(41,347),Vec2(/*190*/214,445),
//
//	Vec2(755, 441/*778,443*/),Vec2(927,345),Vec2(927,179),Vec2(778,123),
//	Vec2(528,123),
//	Vec2(198,123),Vec2(49,179),Vec2(49,345),Vec2(/*198,443*/223, 441),
//};
//// TODO:
//static Vec2 s_ptPublicCard[5] = {
//	Vec2(307,300), Vec2(394,300), Vec2(482,300), Vec2(569,300), Vec2(656,300)	
//};
//
//static Vec2 s_ptUserViewDefault[GAME_PLAYER] = {
//	Vec2(658, 393/*681, 393*/), Vec2(830, 295), Vec2(830, 129), Vec2(681, 73),
//	Vec2(432, 73),
//	Vec2(181, 73),	Vec2(32, 129), Vec2(32, 295), Vec2(206, 393/*181, 393*/),
//};
//
//static Vec2 s_ptUserViewOutTable[GAME_PLAYER] = {
//	Vec2(797,660), Vec2(990,394), Vec2(990,10), Vec2(797,-107),
//	Vec2(434,-107),
//	Vec2(98,-107),	Vec2(-119,10), Vec2(-119,395), Vec2(98,659),
//};
//
//static Vec2 s_ptUserViewDefaultCenter[GAME_PLAYER] = {
//	Vec2(706, 462/*730, 459*/),Vec2(879, 364),Vec2(879, 198),Vec2(730, 142),
//	Vec2(481, 142),
//	Vec2(229, 142),Vec2(81, 198),Vec2(81, 364),Vec2(254, 462/*230, 462*/)
//};
//
//static Vec2 s_ptBgBet[GAME_PLAYER] = {
//	Vec2(602, 419),Vec2(774, 344),Vec2(774, 260),Vec2(626, 193),
//	Vec2(376, 193),
//	Vec2(223, 227),Vec2(183, 260),Vec2(185, 344),Vec2(358, 419)
//};
//
//static Vec2 s_ptLabelBet[GAME_PLAYER] = {
//	Vec2(581, 417),Vec2(753, 342),Vec2(753,258),Vec2(605,191),
//	Vec2(355, 191),
//	Vec2(245, 225),Vec2(205, 258),Vec2(205, 342),Vec2(380, 417)
//};
//
//static Vec2 s_ptJettonDst[GAME_PLAYER] = {
//	Vec2(638, 418),Vec2(810, 343),Vec2(810, 259),Vec2(661, 192),
//	Vec2(412, 192),
//	Vec2(187, 226),Vec2(147, 259),Vec2(147, 343),Vec2(322, 418)
//};

//static Vec2 s_ptMyCard[2] = {
//	Vec2(549,147-7+5), Vec2(572-1,138+5)	
//};
void GameTableViewer::ClearPublicCard()
{
    m_vecScencePublicCard.clear();
}

void GameTableViewer::GetPublicCard(GameScenePublicCard scenceItem)
{
    m_vecScencePublicCard.push_back(scenceItem);
}

void GameTableViewer::SetCardType(unsigned scene)
{
    for (auto item : m_vecScencePublicCard)
    {
        if (item.m_uScene == scene)
        {
            CGameLogic gameLogic;
            if (item.m_vecPublicCard.size() == 3)
            {
                std::vector<unsigned char> vecAllCard(item.m_vecPublicCard);
                vecAllCard.push_back(m_cbMyCardData[0]);
                vecAllCard.push_back(m_cbMyCardData[1]);
                gameLogic.SortCardList(&vecAllCard[0], vecAllCard.size());
                unsigned uMaxCardT = gameLogic.GetCardType(&vecAllCard[0], vecAllCard.size());
                m_pSpriteMaxCardType->SetFrameIdx(uMaxCardT);
            }
            else if (item.m_vecPublicCard.size() == 4)
            {
                unsigned char cbMaxCard[5];
                gameLogic.FiveFromSix(m_cbMyCardData, 2, &item.m_vecPublicCard[0], 4, cbMaxCard, 5);
                unsigned uMaxCardT = gameLogic.GetCardType(cbMaxCard, 5);
                m_pSpriteMaxCardType->SetFrameIdx(uMaxCardT);
            }
            else if (item.m_vecPublicCard.size() == 5)
            {
                unsigned char cbMaxCard[5];
                gameLogic.FiveFromSeven(m_cbMyCardData, 2, &item.m_vecPublicCard[0], 5, cbMaxCard, 5);
                unsigned uMaxCardT = gameLogic.GetCardType(cbMaxCard, 5);
                m_pSpriteMaxCardType->SetFrameIdx(uMaxCardT);
            }
        }
    }
}

void GameTableViewer::Clear()
{
	m_uDispatchingIdx = 0;
	m_vecDispatchingChairID.clear();
	m_vecPublicCardData.clear();
	m_deqDispatchingPublicCardData.clear();//放在设置牌型后清空
	m_bAllPublicCardDispatched = false;

	m_pSpriteMaxCardType->SetFrameIdx(0);

	m_dAllPlacedJettonValue = 0;
	setUITextString(m_pLabelAllPlacedJettonValue, "底池：0");

	m_uBetingCount = 0;
	m_funCBAllBetingFinished = nullptr;

	m_bCollecting = false;
	m_funCBCollectingFinished = nullptr;

	m_funCBAllPublicCardDispatched = nullptr;
    m_funCBPublicCardDispatched = nullptr;

	m_bOpenCarding = false;
	m_bOpenCardFinished = false;
	m_uWinType = 0;
	m_vecWinChairs.clear();
	m_vecUsrWinInfo.clear();

	m_pSpriteShowMaxCard->setVisible(false);
	m_pSpriteShowMaxCard->stopAllActions();

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

	for (auto& pSprite : m_pSpritePublicCard)
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

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (m_pUsers[i])
		{
			UsrStandUp(i, false);
		}
	}
}
void GameTableViewer::SetTableInfo(const TableBaseInfo& tableBaseInfo)
{
	m_tableBaseInfo = tableBaseInfo;
}
void GameTableViewer::SetTableName(const std::string& strTableInfo)
{
	setUITextString(m_pLabelTableInfo, strTableInfo);
}
void GameTableViewer::SetGameID(const std::string& strGameID)
{
    setUITextString(m_pLabelGameID, "(编号:"+strGameID+")");
}
void GameTableViewer::SetDPosition(unsigned uChairID)
{

	if (m_pSpriteDFlag == nullptr)
	{
		m_pSpriteDFlag = Sprite::create("dflag.png");
		m_pSpriteDFlag->setPosition(s_ptDPosition[uChairID].x, s_ptDPosition[uChairID].y);
		s_gameLayer->addChild(m_pSpriteDFlag, DFlagZOrder);
	}
	else
	{
		m_pSpriteDFlag->runAction(MoveTo::create(0.5f, s_ptDPosition[uChairID]));
	}
}

void GameTableViewer::SetSidePot(const std::vector<double>& vecCurAllPot)
{
	m_vecSidePotValue = vecCurAllPot;
	m_uCurSidePotCount = vecCurAllPot.size();
	for (auto item : m_vecPSidePot)
	{
		item->removeFromParentAndCleanup(true);
	}
	m_vecPSidePot.clear();
	for (int i = 0; i < m_uCurSidePotCount; i++)
	{
		auto pSidePot = SidePot::create(std::bind(&GameTableViewer::GetJettonIdxs, this, std::placeholders::_1, std::placeholders::_2));
		pSidePot->setCascadeOpacityEnabled(true);
		pSidePot->setPosition(GetSidePotPosition(m_uCurSidePotCount, i));
		pSidePot->UpdatePotValue(m_vecSidePotValue[i]);
		s_gameLayer->addChild(pSidePot, SidePotZOrder);
		m_vecPSidePot.push_back(pSidePot);
	}
}

void GameTableViewer::SetAllPot(double dAllPot)
{
	m_dAllPlacedJettonValue = dAllPot;
	setUITextString(m_pLabelAllPlacedJettonValue,"底池："+ FormatCash(m_dAllPlacedJettonValue));
}

void GameTableViewer::SetUser(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID,  unsigned uVoice,unsigned uSkin, bool bAni)
{
	UsrSitDown(uChairID, strNickName, strIcon, dCash, uVoice,uUID, uSkin, bAni);
}

void GameTableViewer::SetUsrBetedJetton(unsigned uChairID, double dBetCash)
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

void GameTableViewer::SetUsrCash(unsigned uChairID, double dCash)
{
	auto& pUserView = m_pUsers[uChairID];
	if (pUserView)
	{
		pUserView->SetCash(dCash);
	}
}
void GameTableViewer::SetUsrStatus(unsigned uChairID, unsigned uStatus)
{
	GetUserView(uChairID)->ActivateStatus(uStatus);
}

void GameTableViewer::SetPublicCard(std::vector<unsigned char> vecPublicCard)
{
	m_bAllPublicCardDispatched = true;
	m_vecPublicCardData = vecPublicCard;
	for (int i = 0; i < m_vecPublicCardData.size(); i++)
	{
		GetSpritePublicCard(i)->SetFrameIdx(GetCardFrameIdx(vecPublicCard[i]));
		GetSpritePublicCard(i)->setVisible(true);
	}
}

void GameTableViewer::SetHandCard(unsigned char cbHandCard[2])
{
	m_cbMyCardData[0] = cbHandCard[0];
	m_cbMyCardData[1] = cbHandCard[1];

	GetSpriteMyCard(0)->stopAllActions();
	GetSpriteMyCard(1)->stopAllActions();
	GetSpriteMyCard(0)->setRotation3D(Vec3(0, 0, 0));
	GetSpriteMyCard(1)->setRotation3D(Vec3(0, 0, 0));
	GetSpriteMyCard(0)->SetFrameIdx(GetCardFrameIdx(cbHandCard[0]));
	GetSpriteMyCard(1)->SetFrameIdx(GetCardFrameIdx(cbHandCard[1]));
	GetSpriteMyCard(0)->setVisible(true);
	GetSpriteMyCard(1)->setVisible(true);
}

void GameTableViewer::UsrSitDown(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID, unsigned uVoice,unsigned uSkin, bool bAni)
{
	auto& pUserView = m_pUsers[uChairID];
	if (pUserView == nullptr)
	{
		//pUserView = UserView::create();
		pUserView = UserView::creatWithLayer(m_usePosInfo[uChairID]);
		pUserView -> ignoreAnchorPointForPosition(false);
		pUserView -> setAnchorPoint(Point(0.5,0.5));

		if (bAni)
		{
			pUserView->setVisible(false);
			s_gameLayer->addChild(pUserView, UserViewZOrder);
			pUserView->runAction(Sequence::create(
				Place::create(s_ptUserViewOutTable[uChairID]),
				Show::create(),
				MoveTo::create(0.2f, s_ptUserViewDefault[uChairID]),
				nullptr));
		}
		else
		{			
			pUserView->setPosition(s_ptUserViewDefault[uChairID]);
			s_gameLayer->addChild(pUserView, UserViewZOrder);
			pUserView->setVisible(true);
		}
	}
	else
	{
		pUserView->stopAllActions();
		if (bAni)
		{
			pUserView->runAction(Sequence::create(
				Show::create(),
				MoveTo::create(0.2f, s_ptUserViewDefault[uChairID]),
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
    m_usrInfo[uChairID].m_strName = strNickName;
    m_usrInfo[uChairID].m_strIcon = strIcon;
    m_usrInfo[uChairID].m_dCash = dCash;
    m_usrInfo[uChairID].m_uUID = uUID;
    m_usrInfo[uChairID].m_uSkin = uSkin;

	PlayEffectSound("sound/act/sit.mp3");
}

void GameTableViewer::UsrStandUp(unsigned uChairID, bool bAni)
{
    if (m_pUsers[uChairID] == nullptr)
    {
        return;
    }
	auto& pUserView = m_pUsers[uChairID];
	pUserView->stopAllActions();
	pUserView->ShowWaitNextGame();
	if (bAni)
	{
		if (getTableType()==NormalTable )
		{
			pUserView->runAction(Sequence::create(
				MoveTo::create(0.2f, s_ptUserViewOutTable[uChairID]),
				Hide::create(),
				nullptr));
		}
		else
		{
			//pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
			//pUserView->setVisible(false);
		}


	}
	else
	{
		pUserView->setPosition(s_ptUserViewOutTable[uChairID]);
		pUserView->setVisible(false);
	}

	if (uChairID != MY_CHAIR_ID)
	{
		if (m_pSpriteDispatchingCard[uChairID*2])
		{		
			m_pSpriteDispatchingCard[uChairID*2]->setVisible(false);
		}
		if (m_pSpriteDispatchingCard[uChairID*2+1])
		{		
			m_pSpriteDispatchingCard[uChairID*2+1]->setVisible(false);
		}
	}
	else
	{
		if (m_pSpriteMyCard[0])
		{
			m_pSpriteMyCard[0]->setVisible(false);
		}
        if (m_pSpriteMyCard[1])
        {
            m_pSpriteMyCard[1]->setVisible(false);
        }
		if (m_pSpriteMaxCardType && bAni)
		{
			m_pSpriteMaxCardType->SetFrameIdx(0);
		}
	}
}

void GameTableViewer::UsrPlaceJetton(unsigned uChairID, double dBetValue)
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
	pJetton->setOpacity(255);
	pJetton->setPosition(s_ptUserViewDefaultCenter[uChairID]);
	pJetton->setVisible(true);
	auto pActMoveAndFadeOut = MoveTo::create(0.3f, s_ptJettonDst[uChairID]);
	auto pFunUpdateBet = [this, uChairID](){	
		SetUsrBetedJetton(uChairID, m_dUsrPotValue[uChairID]);
		SetUsrBetFinished(uChairID);
	};
	pJetton->runAction(Sequence::create(pActMoveAndFadeOut, Hide::create(), CallFunc::create(pFunUpdateBet), nullptr));
	m_dAllPlacedJettonValue += dBetValue;
	setUITextString(m_pLabelAllPlacedJettonValue, "底池："+FormatCash(m_dAllPlacedJettonValue));

	PlayEffectSound("sound/table/bet.mp3");
}

void GameTableViewer::DispatchHandCard(const std::vector<unsigned>& vecDispatchingChairID, unsigned char cbMyCardData[2])
{
	if (IsJettonCollecting())
	{
		SetJettonCollectFinishedCB(std::bind(&GameTableViewer::DispatchHandCard, this, vecDispatchingChairID, cbMyCardData));
		return;
	}
	m_vecDispatchingChairID = vecDispatchingChairID;
	m_vecDispatchingChairID.insert(m_vecDispatchingChairID.end(), vecDispatchingChairID.begin(), vecDispatchingChairID.end());
	m_cbMyCardData[0] = cbMyCardData[0];
	m_cbMyCardData[1] = cbMyCardData[1];
	schedule(schedule_selector(GameTableViewer::OnScheduleDispatchHandCard),  0.1f, m_vecDispatchingChairID.size(), 0.0f);
	for (auto itm : vecDispatchingChairID)
	{
		GetUserView(itm)->ShowNickName();
	}
}

void GameTableViewer::DispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard)
{
	if (vecDispatchingPublicCard.size() == 0)
		return;

	ActivateUserStatus(US_NORMAL, US_ALLIN | US_FOLD | US_WAITNEXTGAME);

	if (IsJettonCollecting())
	{
		SetJettonCollectFinishedCB(std::bind(&GameTableViewer::DispatchPublicCardImpl, this, vecDispatchingPublicCard));
	}
	else
	{
		DispatchPublicCardImpl(vecDispatchingPublicCard);
	}
}

void GameTableViewer::WaitUsrAction(unsigned uChairID)
{
    if (!m_deqDispatchingPublicCardData.empty())
    {
        SetPublicCardDispatchedCB([this,uChairID](unsigned){WaitUsrAction(uChairID);});
        return;
    }

	auto pUserView = GetUserView(uChairID);
	if (pUserView)
	{
			pUserView->ShowThinking();
	}

}
void GameTableViewer::UsrCheck(unsigned uChairID)
{
	auto pUserView = GetUserView(uChairID);
	pUserView->ShowAction(UserAction::Check);
	PlayUsrActSound(uChairID, 0);
}

void GameTableViewer::UsrCall(unsigned uChairID, double dBetValue)
{
	auto pUserView = GetUserView(uChairID);
	pUserView->ShowAction(UserAction::Call);
	UsrPlaceJetton(uChairID, dBetValue);
	PlayUsrActSound(uChairID, 1);
}

void GameTableViewer::UsrRaise(unsigned uChairID, double dBetValue)
{
	auto pUserView = GetUserView(uChairID);
	pUserView->ShowAction(UserAction::Raise);
	UsrPlaceJetton(uChairID, dBetValue);
	PlayUsrActSound(uChairID, 2);
}

void GameTableViewer::UsrAllIn(unsigned uChairID, double dBetValue)
{
	auto pUserView = GetUserView(uChairID);
	pUserView->ShowAction(UserAction::AllIn);
	UsrPlaceJetton(uChairID, dBetValue);
	if(g_globalTableConfig.m_uSound != 0)
	{
		PlayUsrActSound(uChairID, 3);
	}
}

void GameTableViewer::UsrFoldCard(unsigned uChairID)
{
	auto pUserView = GetUserView(uChairID);
	pUserView->ShowAction(UserAction::Fold);
	if (uChairID == MY_CHAIR_ID)
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
		auto pFoldingCard = GetSpriteFoldingCard(uChairID);
		auto pAction = Sequence::create(Show::create(), Spawn::create(MoveTo::create(0.6f, Vec2(GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 3 * 2)), FadeOut::create(0.6f), nullptr), Hide::create(), nullptr);
		pFoldingCard->runAction(pAction);
	}
	PlayUsrActSound(uChairID, 4);
}
void GameTableViewer::ShowMeHasActioned()
{
	unschedule(schedule_selector(GameTableViewer::onScheduleShakeHandCard));
	unschedule(schedule_selector(GameTableViewer::onScheduleFoldCard));

	auto pCard0 = GetSpriteMyCard(0);
	auto pCard1 = GetSpriteMyCard(1);
	pCard0->stopAllActions();
	pCard1->stopAllActions();
	pCard0->setPosition(s_ptMyCard[0]);
	pCard1->setPosition(s_ptMyCard[1]);
}

void GameTableViewer::ShowMeHasWin()
{
	//if (m_pAniShowMeWin == nullptr)
	//{
	//	m_pAniShowMeWin = CreateAnimate("effects/showmewin.webp", 3, 6, 17, 1.0f / 24.0f, false);
	//	m_pAniShowMeWin->retain();
	//}
	//else
	//{
	//	m_pAniShowMeWin->update(0);
	//}



	m_pSpriteShowMeWin->setVisible(true);
	//m_pSpriteShowMeWin->runAction(Sequence::create(m_pAniShowMeWin, DelayTime::create(1.4f), Hide::create(), nullptr));
	m_pAniShowMeWin->playOnceByIndexAndStopAtLastFrame(0);
	PlayEffectSound("sound/table/win.mp3");
}

void GameTableViewer::CollectUsrBetToPot(const std::vector<double>& vecCurAllPot)
{
	SetJettonCollecting(true);
	if (GetUsrBeting() > 0)
	{
		SetAllBetFinishedCB(std::bind(&GameTableViewer::CollectUsrBetToPot, this, vecCurAllPot));
		return;
	}
	// 记录下新的边池情况
	m_vecSidePotValue = vecCurAllPot;
	Vec2 ptDstSidePot = GetSidePotPosition(m_uCurSidePotCount == 0 ? 1 : m_uCurSidePotCount, m_uCurSidePotCount == 0 ? 0 : (m_uCurSidePotCount - 1));
	bool bNeedCollect = false;
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
			auto actMov = Spawn::create(FadeOut::create(0.2f), MoveTo::create(0.2f, ptDstSidePot), nullptr);
			m_pSpriteJetton2SidePot[i]->runAction(Sequence::create(actMov, Hide::create(),  nullptr));
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
				auto pSidePot = SidePot::create(std::bind(&GameTableViewer::GetJettonIdxs, this, std::placeholders::_1, std::placeholders::_2));
				pSidePot->setCascadeOpacityEnabled(true);
				pSidePot->setVisible(false);
				s_gameLayer->addChild(pSidePot, SidePotZOrder);
				m_vecPSidePot.push_back(pSidePot);
				++uCurSidePotCount;
			}
			// 第一次下注，边池还没有产生, 先显示第一个边池，然后移动到其新位置，在该位置生成其子边池，并移动到其对应的位置
			if (m_uCurSidePotCount == 0)
			{
				for (unsigned j = 0; j < uCurSidePotCount; ++j)
				{
					m_vecPSidePot[j]->UpdatePotValue(m_vecSidePotValue[j]);
					if (0 == j)
					{
						auto actMove = MoveTo::create(0.4f, GetSidePotPosition(uCurSidePotCount, 0));
						m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(1, 0)), Show::create(), actMove, nullptr));
					}
					else
					{
						m_vecPSidePot[j]->setOpacity(0);
						auto actMove = Spawn::create(FadeIn::create(0.4f), MoveTo::create(0.4f, GetSidePotPosition(uCurSidePotCount, j)), nullptr);
						m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(uCurSidePotCount, 0)), Show::create(), actMove, nullptr));
					}
				}
			}
			else
			{
				// 将当前边池及其父边池全部移动到其对应的新位置，并在当前边池的新位置生成子边池，移动到其对应的新位置
				unsigned uLastPotIdx = m_uCurSidePotCount - 1;
				for (unsigned j = 0; j < uCurSidePotCount; ++j)
				{
					m_vecPSidePot[j]->UpdatePotValue(m_vecSidePotValue[j]);
					if (j <= uLastPotIdx)
					{
						auto actMove = MoveTo::create(0.4f, GetSidePotPosition(uCurSidePotCount, j));
						m_vecPSidePot[j]->runAction(actMove);
					}
					else
					{
						m_vecPSidePot[j]->setOpacity(0);
						auto actMove = Spawn::create(FadeIn::create(0.4f), MoveTo::create(0.4f, GetSidePotPosition(uCurSidePotCount, j)), nullptr);
						m_vecPSidePot[j]->runAction(Sequence::create(Place::create(GetSidePotPosition(uCurSidePotCount, uLastPotIdx)), Show::create(), actMove, nullptr));
					}
				}
			}
			m_uCurSidePotCount = uCurSidePotCount;
		}
		else 
		{
			// 仅仅需要更新下当前最后一个池子的下注额
			m_vecPSidePot[m_uCurSidePotCount-1]->UpdatePotValue(m_vecSidePotValue[m_uCurSidePotCount-1]);
		}	
		SetJettonCollecting(false);
	};
	if (bNeedCollect)
	{
		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create(funProcPot), nullptr));
		PlayEffectSound("sound/table/collectjetton.mp3");
	}
	else
	{
		SetJettonCollecting(false);
	}
}


void GameTableViewer::ActivateUserStatus(unsigned uS, unsigned uExceptS)
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

void GameTableViewer::OpenUsrCard(const std::vector<unsigned>& vecOpenCardUsr, const std::vector<unsigned>& vecOpenCardData)
{
	unsigned char cbCardData[2];
	for (int i = 0; i < vecOpenCardUsr.size(); i++)
	{
		auto uChairID = vecOpenCardUsr[i];
		cbCardData[0] = vecOpenCardData[i*2];
		cbCardData[1] = vecOpenCardData[i*2+1];
		GetUserView(uChairID)->ShowOpenCard(cbCardData);
		if (uChairID == MY_CHAIR_ID)
		{
			GetSpriteMyCard(0)->setVisible(false);
			GetSpriteMyCard(1)->setVisible(false);
		}
		else
		{
            if (GetHandCard(uChairID, 0))
			    GetHandCard(uChairID, 0)->setVisible(false);
            if (GetHandCard(uChairID, 1))
			    GetHandCard(uChairID, 1)->setVisible(false);
		}
	}
	m_bOpenCarding = true;
	m_bOpenCardFinished = false;
	scheduleOnce(schedule_selector(GameTableViewer::OnScheduleOpenUsrCardFinished), 1.6f);
}

void GameTableViewer::ShowUsrWinInfo(unsigned uWinType, const std::vector<unsigned>& vecWinChairs, bool bWaitOpenCard, const std::vector<UsrWinInfo>& vecUsrWinInfo)
{
	if (bWaitOpenCard)
	{
		m_uWinType = uWinType;
		m_vecWinChairs = vecWinChairs;
		m_vecUsrWinInfo = vecUsrWinInfo;
		schedule(schedule_selector(GameTableViewer::OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched), 0.1f, 100, 0);
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
		}
	
		DispatchWinJettonToUsr(vecUsrWinInfo);
	}
	PlayEffectSound("sound/table/gameend.mp3");
}

void GameTableViewer::SetUsrBeting(unsigned uChairID)
{
	++m_uBetingCount;
}

void GameTableViewer::SetUsrBetFinished(unsigned uChairID)
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

unsigned GameTableViewer::GetUsrBeting()
{
	return m_uBetingCount;
}

void GameTableViewer::SetAllBetFinishedCB(std::function<void()>cbAllBetingFinished)
{
	m_funCBAllBetingFinished = cbAllBetingFinished;
}

void GameTableViewer::SetJettonCollecting(bool bCollecting)
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

bool GameTableViewer::IsJettonCollecting()
{
	return m_bCollecting;
}

void GameTableViewer::SetJettonCollectFinishedCB(std::function<void()>cbJettonCollectFinished)
{
	m_funCBCollectingFinished = cbJettonCollectFinished;
}
void GameTableViewer::DispatchPublicCardImpl(const std::vector<unsigned char>& vecDispatchingPublicCard)
{
    if (PreDispatchPublicCard(vecDispatchingPublicCard))
    {
        StartDispatchPublicCard();
    }
	/*unsigned uDispatchingCardCount = vecDispatchingPublicCard.size();
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

		schedule(schedule_selector(GameTableViewer::OnScheduleDispatchPublicCard),  0.3f, uDispatchingCardCount, 0.0f);
	}*/
}

bool GameTableViewer::PreDispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard)
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

void GameTableViewer::StartDispatchPublicCard()
{
    if (!m_deqDispatchingPublicCardData.empty())
    {
        DishpatchPublicCard(m_vecPublicCardData.size(), m_deqDispatchingPublicCardData.front(), false);
    }
}

void GameTableViewer::DishpatchPublicCard(unsigned uIdx, unsigned char cbCardData, bool bDelay)
{
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
                PlayEffectSound("sound/table/fanpai13.mp3");	
            }	
        }
        else if (uIdx == 3)
        {
            PlayEffectSound("sound/table/fanpai4.mp3");
        }
        else if (uIdx == 4)
        {
            PlayEffectSound("sound/table/fanpai5.mp3");
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
    //m_uPreThinkChairID = -1;
}

void GameTableViewer::OnPublicCardDispatched(unsigned uIdx)
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

void GameTableViewer::SetPublicCardDispatchedCB(std::function<void(unsigned uAllPublicCard)>cbPublicCardDispatched)
{
    m_funCBPublicCardDispatched = cbPublicCardDispatched;
}

void GameTableViewer::SetAllPublicCardDispatchedCB(std::function<void()>cbAllPublicCardDispatched)
{
	m_funCBAllPublicCardDispatched = cbAllPublicCardDispatched;
}
void GameTableViewer::DispatchWinJettonToUsr(const std::vector<UsrWinInfo>& vecUsrWinInfo)
{
	if (IsJettonCollecting())
	{
		SetJettonCollectFinishedCB(std::bind(&GameTableViewer::DispatchWinJettonToUsr, this, vecUsrWinInfo));
		return;
	}
	
	float fDelay = 0.0f;
	for (unsigned i = 0; i < vecUsrWinInfo.size(); ++i)
	{
		auto usrWinInfo = vecUsrWinInfo[i];

		// 将一组边池里指定数量的筹码发给指定赢家
		auto funDispatchPots2Usr = [this, usrWinInfo]()
		{
			for (unsigned j = 0; j < usrWinInfo.uWinPots; ++j)
			{
				unsigned uPotIdx = usrWinInfo.uWinPotsIdx[j];
				double dWinValue = usrWinInfo.dPotsWinValue[j];
				if (dWinValue >= 1.0f && uPotIdx < m_vecPSidePot.size())
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
					float fEaseFactor = 0.6;
					for (auto jetton : vecJetton)
					{
						auto funRemoveJettonSelf = [jetton]()
						{
							jetton->removeFromParent();
						};
						auto actMov = Spawn::create(EaseIn::create(MoveTo::create(0.6f, ptDst), fEaseFactor), FadeOut::create(0.6f), nullptr);
						jetton->runAction(Sequence::create(actMov, CallFunc::create(funRemoveJettonSelf), nullptr));
						fEaseFactor += 0.5;
					}
				}				
			}
		};
		// 显示赢取数字动画
		auto funcShowWinNumber = [this, usrWinInfo]()
		{
			auto pWinNumber = GetUserWinNumber(usrWinInfo.uChair);
			auto actMove = Spawn::create(FadeIn::create(0.5f), MoveBy::create(0.5f, Vec2(0, 50+30)), nullptr);
			pWinNumber->UpdateNumber(usrWinInfo.dAllWinValue);
			pWinNumber->runAction(Sequence::create(Show::create(), actMove, DelayTime::create(0.5),FadeOut::create(0.5), nullptr));
		};
        if (usrWinInfo.bIsWinner)
        {
			GetUserView(usrWinInfo.uChair)->runAction(Sequence::create(DelayTime::create(fDelay), CallFunc::create(funDispatchPots2Usr), DelayTime::create(0.6), CallFunc::create(funcShowWinNumber), nullptr));
        }

		fDelay += 0.7f;

		PlayEffectSound("sound/table/dispatchjetton.mp3");
	}
}

void GameTableViewer::onScheduleShakeHandCard(float fDelta)
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
	};
	pCard1->runAction(Repeat::create(Sequence::create(CallFunc::create(funcShakeCard1), DelayTime::create(0.08f), nullptr), 300));
}

void GameTableViewer::onScheduleFoldCard(float fDelta)
{
	SendClientRequest(ClientRequest::kGiveUp);
}

void GameTableViewer::OnScheduleDispatchHandCard(float fDelta)
{

	if (m_uDispatchingIdx < m_vecDispatchingChairID.size())
	{
		unsigned uRoundIdx = m_uDispatchingIdx < (m_vecDispatchingChairID.size() / 2) ? 0 : 1;
		unsigned uCurDispatchChairID = m_vecDispatchingChairID[m_uDispatchingIdx];

		Sprite *pSpriteDispatchingCard = GetSpriteDispatchingCard(uRoundIdx, uCurDispatchChairID);

		if (uCurDispatchChairID == MY_CHAIR_ID)
		{
			auto pActionMove = Spawn::create(FadeIn::create(0.2f), MoveTo::create(0.2f, s_ptDispatchedCard[uRoundIdx * GAME_PLAYER + uCurDispatchChairID]), nullptr);
			auto pActShowMyCard = CallFunc::create([this, uRoundIdx](){auto pMyCard = this->GetSpriteMyCard(uRoundIdx); PlayAnimateOpenHandCard(pMyCard, this->m_cbMyCardData[uRoundIdx], uRoundIdx);});
			pSpriteDispatchingCard->runAction(Sequence::create(Show::create(), pActionMove, Hide::create(), pActShowMyCard, nullptr));
		}
		else
		{
			auto pActionMove = Spawn::create(FadeIn::create(0.2f), MoveTo::create(0.2f, s_ptDispatchedCard[uRoundIdx * GAME_PLAYER + uCurDispatchChairID]), nullptr);
			pSpriteDispatchingCard->runAction(Sequence::create(Show::create(), pActionMove, nullptr));
		}
		const char *szEffect[2] = {"sound/table/handcard0.mp3", "sound/table/handcard1.mp3"};
		PlayEffectSound(szEffect[uRoundIdx]);
		++m_uDispatchingIdx;
	}
}

void GameTableViewer::OnScheduleDispatchPublicCard(float fDelta)
{
	if (!m_deqDispatchingPublicCardData.empty())
	{
		unsigned uDispatchedCardCount = m_vecPublicCardData.size();
		auto pCard = GetSpritePublicCard(uDispatchedCardCount);
		unsigned char cbCardData = m_deqDispatchingPublicCardData.front();
		m_vecPublicCardData.push_back(cbCardData);
		if (m_vecPublicCardData.size() == 1)
		{
			PlayEffectSound("sound/table/fanpai13.mp3");
		}
		else if (m_vecPublicCardData.size() == 4)
		{
			PlayEffectSound("sound/table/fanpai4.mp3");
		}
		else if (m_vecPublicCardData.size() == 5)
		{
			PlayEffectSound("sound/table/fanpai5.mp3");
		}

		m_deqDispatchingPublicCardData.pop_front();
		if (m_deqDispatchingPublicCardData.empty())
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
		auto cbPublicCardOpened = [this, cbCardData]()
		{
			if (m_vecPublicCardData.size() == 5)
			{
				m_bAllPublicCardDispatched = true;
				if (m_funCBAllPublicCardDispatched)
				{
					m_funCBAllPublicCardDispatched();
				}
			}
		};
		PlayAnimateOpenCard(pCard, cbCardData, cbPublicCardOpened);		
	}
}

void GameTableViewer::OnScheduleOpenUsrCardFinished(float fDelta)
{
	if (m_bOpenCarding)
	{
		m_bOpenCardFinished = true;
	}
}


void GameTableViewer::OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched(float fDelta)
{
	if (m_bOpenCarding && m_bOpenCardFinished && m_bAllPublicCardDispatched)
	{
		unschedule(schedule_selector(GameTableViewer::OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched));
		for (int i = 0; i < m_vecWinChairs.size(); i++)
		{
			GetUserView(m_vecWinChairs[i])->ShowWinType(m_uWinType);
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
			PlayEffectSound(szSoundWinType[uWinType]);
		}

		scheduleOnce(schedule_selector(GameTableViewer::OnScheduleDispatchWinJettonToUsr), 1.5f);
	}
}
void GameTableViewer::OnScheduleDispatchWinJettonToUsr(float fDelta)
{
	DispatchWinJettonToUsr(m_vecUsrWinInfo);
}

void GameTableViewer::PlayUsrActSound(unsigned uChairID, unsigned uAct)
{
    std::string strBase = "sound/skin/";

    unsigned uCounty = 0;
    unsigned uGender = 1;

    unsigned uSkin = m_usrInfo[uChairID].m_uSkin;
    if (uSkin >= 1 && uSkin <= 3) // 中女
    {
        uCounty = 0;
        uGender = 0;
    }
    else if (uSkin >= 4 && uSkin <= 6)// 中男
    {
        uCounty = 0;
        uGender = 1;
    }
    else if (uSkin >= 7 && uSkin <= 9)// 外女
    {
        uCounty = 1;
        uGender = 0;
    }
    else if (uSkin >= 10 && uSkin <= 12)// 外男
    {
        uCounty = 1;
        uGender = 1;
    }

    static char *szCounty[] = {"china/", "foreign/"};
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
    PlayEffectSound(strBase);
	/*static const char *s_szDefActSoundName[] = {
		"sound/skin/check.mp3",
		"sound/skin/call.mp3",
		"sound/skin/raise.mp3",
		"sound/skin/allin.mp3",
		"sound/skin/fold.mp3"
	};
	PlayEffectSound(s_szDefActSoundName[uAct]);*/
}
void GameTableViewer::GetJettonIdxs(double dValue, std::vector<int>& vecJettonIdx)
{
	unsigned uLittleBlindTime[] = {1, 5, 10, 25, 100, 500, 1000, 2000, 5000, 10000};
	std::vector<unsigned> vecLittleBlindTime(uLittleBlindTime, uLittleBlindTime + sizeof(uLittleBlindTime) / sizeof(uLittleBlindTime[0]));

	std::vector<double> vecJettonValue;
	for (unsigned i = 0; i < vecLittleBlindTime.size(); ++i)
	{
		vecJettonValue.push_back(vecLittleBlindTime[i] * m_tableBaseInfo.m_dSmallBlind);
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
unsigned GameTableViewer::GetJettonIdx(double dValue)
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

Vec2 GameTableViewer::GetSidePotPosition(unsigned uPotCount, unsigned uPotIdx)
{

	return s_ptPots[uPotCount-1][uPotIdx];
}
Sprite *GameTableViewer::GetSpriteDispatchingCard(unsigned uRoundId, unsigned uChairId)
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
	pSprite->setPosition(GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 3 * 2);
	pSprite->setOpacity(0);

	return pSprite;
}

cocos2d::Sprite *GameTableViewer::GetHandCard(unsigned uChairID, unsigned uCardIdx)
{
	return m_pSpriteDispatchingCard[uChairID*2+uCardIdx];
}

FrameSprite *GameTableViewer::GetSpriteMyCard(unsigned uCardId)
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

Sprite *GameTableViewer::GetSpriteFoldingCard(unsigned uChairId)
{
	//static Vec2 s_ptFoldingCard[GAME_PLAYER] = {
	//	Vec2(/*776*/752,441),Vec2(925,343),Vec2(925,177),Vec2(776,121),
	//	Vec2(546,140),
	//	Vec2(156,121),Vec2(47,177),Vec2(47,343),Vec2(/*196*/220,441)
	//};
	auto& pSprite = m_pSpriteFoldingCard[uChairId];
	if (pSprite == nullptr)
	{
		pSprite = Sprite::create("foldingcard.png");
		pSprite->setVisible(false);
		pSprite->setPosition(s_ptFoldingCard[uChairId]);
		s_gameLayer->addChild(pSprite, FoldingCardZOrder);
	}
	else
	{
		pSprite->setVisible(false);
		pSprite->setPosition(s_ptFoldingCard[uChairId]);
	}

	return pSprite;
}

FrameSprite *GameTableViewer::GetSpritePublicCard(unsigned uCardId)
{

	if (m_pSpritePublicCard[uCardId] == nullptr)
	{
		m_pSpritePublicCard[uCardId] = FrameSprite::create("publiccard.webp", 13, 5); //// m_pPublicCardFactory->CreateCard();
		m_pSpritePublicCard[uCardId]->setVisible(false);
		m_pSpritePublicCard[uCardId]->setPosition(s_ptPublicCard[uCardId]);
		s_gameLayer->addChild(m_pSpritePublicCard[uCardId], PublicCardZOrder);
	}
	return m_pSpritePublicCard[uCardId];
}

UserView* GameTableViewer::GetUserView(unsigned uChairID)
{
	return m_pUsers[uChairID];
}

TextureNumber *GameTableViewer::GetUserWinNumber(unsigned uChairID)
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
	m_pWinNumber[uChairID]->setOpacity(0);
    if (uChairID == 0 || uChairID == 8)
    {
        m_pWinNumber[uChairID]->setPositionY(s_ptUserViewDefaultCenter[uChairID].y - 73);
    }

	return m_pWinNumber[uChairID];
}

bool GameTableViewer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_curTableType = NullType ;
	m_pNormalSpriteBG = nullptr ;
	m_spritetableTitle =nullptr;
	m_sprite_tableLeft = nullptr ;
	m_sprite_tableRight = nullptr ;


	//auto bg = Sprite::create("bgtable.png");
	//bg->setPosition(GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight() / 2);
	//setContentSize(Size(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()));
	//bg->setScale(GameData::getInstance()->getGameScale());
 //   addChild(bg);
	Point s_curPos ;
	//载入gametable ui布局
	//载入布局
	s_uiRoot = CSLoader::createNode("res/ui/gametable.csb");
	s_uiRoot->setTag(100001);
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	s_uiRoot->setContentSize(frameSize); 
	ui::Helper::doLayout(s_uiRoot);  
	addChild(s_uiRoot);
    s_uiRoot->setVisible(true) ;
	
	m_imgtitle = static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"Node_sngtitle"));
	if (m_spritetableTitle==nullptr)
	{
		m_spritetableTitle = Sprite::create("sng_title.png");
		m_imgtitle->addChild(m_spritetableTitle) ;
	}

	m_pMenuItemPopMenu = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_menu"));
	m_pMenuItemPopMenu->setVisible(true);


	Layout *m_layer_GameTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"tb_gametable"));
    m_layerRightUpNormal = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup"));

	Layout* m_colorBlock = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_1"));
	m_colorBlock->setVisible(false) ;
	s_gameLayer = Tools::getWidgetByName(s_uiRoot,"Panel_GameTable");
//     s_gameLayer->setVisible(true) ;
// 
//     auto layout = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_actionbar2"));
//     layout->setVisible(true);
//     layout->setPosition(Vec2(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight()/2));

	m_btnQiPai = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_qipai"));
    m_btnGenZhu = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_genzhu"));
    m_btnJiaZhu = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_jiazhu"));
    m_btnQiPai->addTouchEventListener(CC_CALLBACK_2(GameTableViewer::OnMenuAction, this));
    m_btnGenZhu->addTouchEventListener(CC_CALLBACK_2(GameTableViewer::OnMenuAction, this));
    m_btnJiaZhu->addTouchEventListener(CC_CALLBACK_2(GameTableViewer::OnMenuAction, this));
    auto btnAllIn = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot, "gt_bt_allin"));
    auto btnRangPai = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot, "gt_bt_rangpai"));

	m_img_tableLeft =  static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"gt_lefttable1"));
	m_img_tableRight =  static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"gt_righttable1"));

    m_layout_PrivateTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup_private"));
    m_layout_PrivateTable->setVisible(false) ;

    m_bt_Private_RoomInfo = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"Button_privateroominfo"));
    m_bt_Private_RoomInfo->setVisible(false);

	m_textGenZhu = static_cast<Text*>(Tools::getWidgetByName(s_uiRoot, "gt_text_gengzhuv"));
	m_textGenZhu->setAnchorPoint(Vec2(0.5, 0.5));
    m_textGenZhu->setPosition(Vec2(m_btnGenZhu->getContentSize().width/2, m_btnGenZhu->getContentSize().height/2));
    btnAllIn->setVisible(false);
    m_btnQiPai->setVisible(false);
    m_btnGenZhu->setVisible(false);
    m_btnJiaZhu->setVisible(false);
    btnRangPai->setVisible(false);

	//s_gameLayer = Tools::getWidgetByName(s_uiRoot,"Panel_GameTable");

	if (m_spritetableTitle==nullptr)
	{
		m_spritetableTitle = Sprite::create("sng_title.png");
		//this->addChild(m_spritetableTitle,50) ;
		Node* nodeTile = static_cast<ImageView*>(Tools::getWidgetByName(s_uiRoot,"Node_sngtitle"));
		nodeTile->addChild(m_spritetableTitle) ;
	}

	//更新坐标   寻找好麻烦  实例化一次 保存下来
	for (int i=0; i< GAME_PLAYER; i++)
	{
		
		char pathUserBlock[64];
		sprintf(pathUserBlock,"user_block%d",i) ;
		m_rootUser[i] = Tools::getWidgetByName(s_uiRoot,pathUserBlock);

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

	s_ptMyCard[0]= GameData::getInstance()->s_ptMyCard[0];
	s_ptMyCard[1]= GameData::getInstance()->s_ptMyCard[1];

	m_uDispatchingIdx = 0;
	m_uCurSidePotCount = 0;
	m_uBetingCount = 0;
	m_funCBAllBetingFinished = nullptr;
	m_bCollecting = false;
	m_funCBCollectingFinished = nullptr;

	//auto bg = Sprite::create("bgtable.png");
	//bg->setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	//setContentSize(Size(GAME_WIDTH, GAME_HEIGHT));
	//addChild(bg);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float fCenterX = origin.x + visibleSize.width / 2;
	float fCenterY = origin.y + visibleSize.height / 2;

	m_pLabelTableInfo =   static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_gameinfo"));
	//m_pLabelTableInfo->setPosition(480, 228);
	//m_pLabelTableInfo->setTextColor(Color4B(0x63, 0xb2, 0x73, 255));
	//addChild(m_pLabelTableInfo, TableBGZOrder);

    m_pLabelGameID =  static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_huifang_id"));
	m_pLabelGameID->setVisible(true);
    setUITextString(m_pLabelGameID,"");
    //m_pLabelGameID->setPosition(480, 376);
    //m_pLabelGameID->setTextColor(Color4B(0x63, 0xb2, 0x73, 255));
    //addChild(m_pLabelGameID, TableBGZOrder);


	m_dAllPlacedJettonValue = 0;
	m_pLabelAllPlacedJettonValue = static_cast<Text*>(Tools::getWidgetByName(s_uiRoot,"gt_text_dichi"));
	//m_pLabelAllPlacedJettonValue->setTextColor(Color4B(0x63, 0xb2, 0x73, 255));
	setUITextString(m_pLabelAllPlacedJettonValue,"底池：0") ;

	m_pSpriteMaxCardType = FrameSprite::create("userview/cardtype.png", 1, 11);
	m_pSpriteMaxCardType->setAnchorPoint(Vec2::ZERO);
    m_pSpriteMaxCardType -> setAnchorPoint(Point(0.22,0.5));
    m_pSpriteMaxCardType->setPosition(Tools::getWidgetByName(s_uiRoot,"px")->getPosition());
    s_gameLayer->addChild(m_pSpriteMaxCardType, MyCardTypeZOrder);
    m_imgCardType = static_cast<ui::ImageView*>(Tools::getWidgetByName(s_uiRoot,"px"));

	//m_pSpriteMaxCardType->setPosition(Vec2(529, 72));
	//s_gameLayer->addChild(m_pSpriteMaxCardType, MyCardTypeZOrder);

	m_pDealer = Dealer::create();
	//m_pDealer->setPosition(Vec2(475, 419));
	s_curPos = Tools::getWidgetByName(s_uiRoot,"node_denvlang")->getPosition() ;
	m_pDealer->setPosition(s_curPos);
	s_gameLayer->addChild(m_pDealer, DealerZOrder);

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

	m_pSpriteShowMeWin = Sprite::create();
	m_pSpriteShowMeWin->setPosition(GameData::getInstance()->getGameWidth()/2, 266+40);
	m_pSpriteShowMeWin->setVisible(false);
	addChild(m_pSpriteShowMeWin, AniShowMeWinZOrder);

	m_pAniShowMeWin = nullptr;	
	m_pAniShowMeWin = AnimationManager::creatArmature("niyingle");
	m_pSpriteShowMeWin->addChild(m_pAniShowMeWin);  
	m_pAniShowMeWin->setVisible(false) ;

	for (auto& pSprite : m_pSpritePublicCard)
	{
		pSprite = nullptr;
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

	//if (m_pActionBar == nullptr)
	//{
	//	m_pActionBar = ActionBar::creatWithLayer(s_uiRoot);
	//	m_pActionBar->setVisible(false);
	//}

	//if (nullptr == m_pPreActionBar)
	//{
	//	//m_pPreActionBar = PreActionBar::create();
	//	m_pPreActionBar = PreActionBar::creatWithLayer(s_uiRoot);
	//	m_pPreActionBar->setVisible(false);
	//	//addChild(m_pPreActionBar, ActionBarZOrder);
	//}

	//if(nullptr == m_pBrightActionBar)
	//{
	//	//m_pBrightActionBar = BrightActionBar::create();
	//	m_pBrightActionBar = BrightActionBar::creatWithLayer(s_uiRoot);
	//	//addChild(m_pBrightActionBar, ActionBrightZOrder);
	//	m_pBrightActionBar->setVisible(false);
	//}

	//隐藏界面

	Node* nodeActionbar= Tools::getWidgetByName(s_uiRoot,"Panel_actionbar");
	nodeActionbar->setVisible(false);

	Node* nodePreActionbar= Tools::getWidgetByName(s_uiRoot,"Panel_PreActionBar");
	nodePreActionbar->setVisible(false);

	Node* nodeBrightActionbar= Tools::getWidgetByName(s_uiRoot,"Panel_BrightActionBar");
	nodeBrightActionbar->setVisible(false);

	Node* nodeTalk= Tools::getWidgetByName(s_uiRoot,"Panel_talkbox");
	nodeTalk->setVisible(false);

	Node* nodeRease = Tools::getWidgetByName(s_uiRoot,"panel_rease");
	nodeRease->setVisible(false);

	Node* nodeRightbutton = Tools::getWidgetByName(s_uiRoot,"gt_bt_rightdown");
	nodeRightbutton->setVisible(false);

	Node* nodeLeftupbutton = Tools::getWidgetByName(s_uiRoot,"gt_bt_leftup");
	nodeLeftupbutton->setVisible(false);

	Node* noderightupbutton = Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup");
	noderightupbutton->setVisible(false);

	Node* nodeAllUser = Tools::getWidgetByName(s_uiRoot,"Panel_alluser");
	nodeAllUser->setVisible(false);


	Node* nodeAllUserallin = Tools::getWidgetByName(s_uiRoot,"Panel_userallin");
	nodeAllUserallin->setVisible(false);

	Node* nodeAct2 = Tools::getWidgetByName(s_uiRoot,"Panel_actionbar2");
	nodeAct2->setVisible(false);

	Node* nodebaoxiang = Tools::getWidgetByName(s_uiRoot,"gt_bt_baoxiang");
	nodebaoxiang->setVisible(false);
	
	Node* nodedashuang = Tools::getWidgetByName(s_uiRoot,"gt_bt_dashang");
	nodedashuang->setVisible(false);
	

	Node*  m_pMenuItemPopMissionRewardAni = Tools::getWidgetByName(s_uiRoot,"gt_bt_baoxiang_0");
	m_pMenuItemPopMissionRewardAni->setVisible(false);

	//sng
	m_layerRightUpSng = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"gt_bt_rightup_0"));

	//返回
	m_sngReturn = static_cast<Button*>(Tools::getWidgetByName(s_uiRoot,"Button_sngreturn"));
	m_sngReturn->setVisible(false);


	//sng 排名
	m_buttonSngRank = static_cast<Button*>(Tools::getWidgetByName(m_layerRightUpSng,"Button_sngrank"));
	m_buttonSngRank->setVisible(false);

	m_text_sngRank =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_sngrank"));
	//托管
	m_sngAutoLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_sngauto"));
	m_sngAutoLayer->setVisible(false) ;
	m_buttomSngAuto = static_cast<Button*>(Tools::getWidgetByName(m_sngAutoLayer,"Button_sngauto"));
	m_buttomSngAuto->setVisible(false) ;
	//滚动提示
	m_text_sngScrollTips =  static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_sng_scrollwtext"));
	m_text_sngScrollTips->setVisible(false) ;




	//需要人的提示
	m_text_sngNeedRoleTips =  static_cast<Layout*>(Tools::getWidgetByName(s_uiRoot,"Panel_sng_lastroleTips"));
	m_text_curSngRoleCount =  static_cast<Text*>(Tools::getWidgetByName(m_text_sngNeedRoleTips,"Text_sngcurrole"));
	m_text_sngNeedRoleTips->setVisible(false) ;




	Layout*m_layer_mtt_time = static_cast<Layout*>(Tools::getWidgetByName(m_layer_GameTable,"Panel_mttdaojishi"));
	m_layer_mtt_time->setVisible(false) ;




	Text* m_text_mttRankF =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_mttrank_f"));
	m_text_mttRankF->setVisible(false) ;
	Text* m_text_mttRankAfter =  static_cast<Text*>(Tools::getWidgetByName(m_layerRightUpSng,"Text_mttrank_after"));
	m_text_mttRankAfter->setVisible(false) ;

	Button* m_bt_mtt_rebuy = static_cast<Button*>(Tools::getWidgetByName(m_layer_GameTable,"gt_mtt_rebuy"));
	m_bt_mtt_rebuy->setVisible(false);

	Button* m_bt_mtt_addon = static_cast<Button*>(Tools::getWidgetByName(m_layer_GameTable,"gt_mtt_addon"));
	m_bt_mtt_addon->setVisible(false);

	return true;
}







void GameTableViewer::setTableType(TableType type)
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
	//mtt相关按钮隐藏
	m_text_sngRank->setVisible(false) ;

	//初始化新的背景
	if (m_curTableType == TableType::NormalTable)
	{
		m_layerRightUpNormal->setVisible(false) ;
		m_layerRightUpSng->setVisible(false) ;
		m_sngReturn->setVisible(false) ;
		m_pMenuItemPopMenu->setVisible(true) ;
		//m_text_sngScrollTips->setVisible(false) ;
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
        m_layout_PrivateTable->setVisible(false);
        m_bt_Private_RoomInfo->setVisible(false);

	}
	else if (m_curTableType == TableType::SngTable)
	{
		m_text_sngRank->setVisible(getTableType()==SngTable) ;
		//m_text_sngRankbg->setVisible(getTableType()==SngTable) ;

		m_layerRightUpNormal->setVisible(false) ;
		m_layerRightUpSng->setVisible(true) ;
		m_sngReturn->setVisible(true) ;
		m_pMenuItemPopMenu->setVisible(false) ;

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

		m_layerRightUpSng->setVisible(true) ;




		m_layerRightUpNormal->setVisible(false) ;

		m_sngReturn->setVisible(true) ;
		m_pMenuItemPopMenu->setVisible(false) ;

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



	//if (m_curTableType == type)
	//{
	//	return ;
	//}
	//m_preTableType = m_curTableType ;

	//m_curTableType = type ;

	////清理台桌背景
	//if (m_pNormalSpriteBG!=nullptr)
	//{
	//	m_pNormalSpriteBG->removeFromParentAndCleanup(true) ;
	//	m_pNormalSpriteBG = nullptr ;

	//}
	//if (m_sprite_tableLeft != nullptr )
	//{
	//	m_sprite_tableLeft->removeFromParentAndCleanup(true) ;
	//	m_sprite_tableLeft = nullptr ;
	//}
	//if (m_sprite_tableRight != nullptr )
	//{
	//	m_sprite_tableRight->removeFromParentAndCleanup(true) ;
	//	m_sprite_tableRight = nullptr ;
	//}



	////初始化新的背景
	//if (m_curTableType == TableType::NormalTable)
	//{
	//	//m_videoPlayMgr.SetVisible(true);
	//	//m_layerRightUpNormal->setVisible(true) ;
	//	//m_layerRightUpSng->setVisible(false) ;
	//	//m_sngReturn->setVisible(false) ;
	//	//m_pMenuItemPopMenu->setVisible(true) ;

	//	if (m_spritetableTitle!=nullptr)
	//	{
	//		m_spritetableTitle->setVisible(false) ;
	//	}
	//	m_pNormalSpriteBG = Sprite::create("bgtable.png");
	//	m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

	//	m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
	//	addChild(m_pNormalSpriteBG,-1);

	//	if (m_sprite_tableLeft == nullptr )
	//	{
	//		m_sprite_tableLeft = Sprite::create("desk_normal.png");
	//		m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
	//	}
	//	if (m_sprite_tableRight == nullptr )
	//	{
	//		m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
	//		m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
	//	}

	//	m_img_tableLeft->addChild(m_sprite_tableLeft);
	//	m_img_tableRight->addChild(m_sprite_tableRight) ;
	//	m_spritetableTitle->setVisible(false) ;
	//}
	//else if (m_curTableType == TableType::SngTable)
	//{
	//	//m_videoPlayMgr.SetVisible(false);
	//	//m_layerRightUpNormal->setVisible(false) ;
	//	//m_layerRightUpSng->setVisible(true) ;
	//	//m_sngReturn->setVisible(true) ;
	//	//m_pMenuItemPopMenu->setVisible(false) ;



	//	m_pNormalSpriteBG = Sprite::create("bgtable_sng.png");
	//	m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

	//	m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
	//	addChild(m_pNormalSpriteBG,-1);

	//	if (m_sprite_tableLeft == nullptr )
	//	{
	//		m_sprite_tableLeft = Sprite::create("desk_sng.png");
	//		m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
	//	}
	//	if (m_sprite_tableRight == nullptr )
	//	{
	//		m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
	//		m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
	//	}

	//	m_img_tableLeft->addChild(m_sprite_tableLeft);
	//	m_img_tableRight->addChild(m_sprite_tableRight) ;

	//	m_spritetableTitle->setVisible(true) ;


	//}
	//else if (m_curTableType == TableType::MttTable)
	//{
	//	m_pNormalSpriteBG = Sprite::create("bgtable_mtt.png");
	//	m_pNormalSpriteBG->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);

	//	m_pNormalSpriteBG->setScale(GameData::getInstance()->getGameScale());
	//	addChild(m_pNormalSpriteBG,-1);

	//	if (m_sprite_tableLeft == nullptr )
	//	{
	//		m_sprite_tableLeft = Sprite::create("desk_mtt.png");
	//		m_sprite_tableLeft->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
	//	}
	//	if (m_sprite_tableRight == nullptr )
	//	{
	//		m_sprite_tableRight = Sprite::createWithTexture(m_sprite_tableLeft->getTexture());
	//		m_sprite_tableRight->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;
	//	}

	//	m_img_tableLeft->addChild(m_sprite_tableLeft);
	//	m_img_tableRight->addChild(m_sprite_tableRight) ;

	//	if (m_spritetableTitle==nullptr)
	//	{
	//		m_spritetableTitle = Sprite::create("mtt_title.png");
	//		//this->addChild(m_spritetableTitle,50) ;

	//		m_imgtitle->addChild(m_spritetableTitle) ;
	//	}

	//}

}

TableType GameTableViewer::getTableType()
{
	return  m_curTableType ;
}








void GameTablePlayer::StartPlay(const Json::Value& gameRecord)
{
	setVisible(true);

	unsigned gametype = JsonValueAdapter(gameRecord["gametype"]);

	m_pGameTable->setTableType((TableType)gametype) ;

    m_pGameTable->ClearPublicCard();
    m_vecHandCard.clear();
    StopBackGroundSound(false);
	unsigned uChairID = JsonValueAdapter(gameRecord["cid"]);
	//m_chairViewConvertor.Init(6, uChairID, 4);
	m_strRecordID = gameRecord["id"].asString();
	TableBaseInfo tableInfo;
	tableInfo.m_strTableName = gameRecord["name"].asString();
	tableInfo.m_dSmallBlind = JsonValueAdapter(gameRecord["smallBlind"]);
	tableInfo.m_dBigBlind = JsonValueAdapter(gameRecord["bigBlind"]);
	tableInfo.m_dAnti = JsonValueAdapter(gameRecord["anti"]);
	tableInfo.m_uMaxPlayer = JsonValueAdapter(gameRecord["maxPlayer"]);
    m_chairViewConvertor.Init(tableInfo.m_uMaxPlayer, uChairID, 4);
     auto& handCard = gameRecord["handCard"];
     if (!handCard.empty())
     {
         for (unsigned j = 0; j < handCard.size(); j++)
         {
             m_vecHandCard.push_back(static_cast<unsigned>(JsonValueAdapter(handCard[j])));
         }
     }
	m_pGameTable->SetTableInfo(tableInfo);
	m_pGameTable->SetTableName(tableInfo.m_strTableName);
    m_pGameTable->SetGameID(m_strRecordID);

	for (auto bt : m_pMenuItemGamePoint)
	{
		bt->setVisible(false);
	}
	auto& scences = gameRecord["scences"];
	
	// double dTickBeg = -1;
	m_vecScence.clear();
	for (unsigned i = 0; i < scences.size(); ++i)
	{
		GameRecordScence scenceItem;
		scenceItem.m_uGamePoint = JsonValueAdapter(scences[i]["type"]);
		
		scenceItem.m_dTick = JsonValueAdapter(scences[i]["time"]);

		auto& players = scences[i]["vailidplayer"];
		PlayBackPlayerInfo player;
		for (unsigned j = 0; j < players.size(); j++)
		{
			player.m_uUserID = JsonValueAdapter(players[j]["id"]);
			player.m_uChairID = JsonValueAdapter(players[j]["cid"]);
			player.m_strIcon = players[j]["icon"].asString();
			player.m_strName = players[j]["name"].asString();
			player.m_uUserLastAction = JsonValueAdapter(players[j]["action"]);
			player.m_dUserBeted = JsonValueAdapter(players[j]["bet"]);
			player.m_dRestCash = JsonValueAdapter(players[j]["cash"]);
            player.m_uSkin = JsonValueAdapter(players[j]["voiceid"]);
			scenceItem.m_vecPlayer.push_back(player);
		}
        GameScenePublicCard scenePublicCard;
		auto& publicCard = scences[i]["publicCard"];
		if (!publicCard.empty())
		{
			for (unsigned j = 0; j < publicCard.size(); j++)
			{
				scenceItem.m_vecPublicCard.push_back(static_cast<unsigned>(JsonValueAdapter(publicCard[j])));
                scenePublicCard.m_vecPublicCard.push_back(static_cast<unsigned>(JsonValueAdapter(publicCard[j])));
			}
            scenePublicCard.m_uScene = i;
            m_pGameTable->GetPublicCard(scenePublicCard);
		}
		
		auto& allPot = scences[i]["poolArr"];
		if (allPot.size() > 0)
		{
			scenceItem.m_dAllPot = JsonValueAdapter(allPot[0u]);
			for (unsigned j = 1; j < allPot.size(); j++)
			{
				scenceItem.m_vecSidePot.push_back(JsonValueAdapter(allPot[j]));
			}
		}
		else
		{
			scenceItem.m_dAllPot = 0;
		}
		m_vecScence.push_back(scenceItem);
	}

	std::sort(std::begin(m_vecScence), std::end(m_vecScence));
	auto dTickBeg = m_vecScence[0].m_dTick;
	for (auto& scenceItem : m_vecScence)
	{
		scenceItem.m_dTick -= dTickBeg;
	}

	
	m_vecCmdItem.clear();
	auto& cmds = gameRecord["cmds"];
	GameRecordCmdItem cmdItem;
	for (unsigned i = 0; i < cmds.size(); ++i)
	{
		cmdItem.m_uID = JsonValueAdapter(cmds[i]["id"]);
		double dTick = JsonValueAdapter(cmds[i]["time"]);
		cmdItem.m_dTick = dTick - dTickBeg;
		cmdItem.m_uCmd = JsonValueAdapter(cmds[i]["cmd"]);
		cmdItem.m_cmdData = cmds[i]["data"];
		if (cmdItem.m_uCmd == GameMsgCmdS::GameStart)
		{
			unsigned uCidOfGameStart = JsonValueAdapter(cmds[i]["cid"]);
			if (uCidOfGameStart != uChairID)
			{
				continue;
			}
		}
	


		unsigned uid = JsonValueAdapter(cmds[i]["uid"]) ;
		if (uid == g_globalMyData.m_iUserId)
		{

			if (m_pGameTable->getTableType() == SngTable)
			{
				if (cmdItem.m_uCmd == 501)
				{
					tableInfo.m_strTableName = cmdItem.m_cmdData["tableDesc"].asString();
					m_pGameTable->SetTableName(tableInfo.m_strTableName);
				}

			}

		}


		m_vecCmdItem.push_back(cmdItem);
	}



	std::sort(std::begin(m_vecCmdItem), std::end(m_vecCmdItem));

	m_dAllTimeInMs = m_vecCmdItem.back().m_dTick;
	unsigned uAllSec = m_dAllTimeInMs / 1000.0f;
	char szTime[64] = {0};
	sprintf(szTime, "%02d:%02d", uAllSec / 60, uAllSec % 60);
	SetLabelString(m_pLabelAllTime, szTime);

	for (auto scence : m_vecScence)
	{
		auto uGamePoint = scence.m_uGamePoint;
		m_pMenuItemGamePoint[uGamePoint]->setPositionX(184 + scence.m_dTick / m_dAllTimeInMs * ProgressWidth);
		m_pMenuItemGamePoint[uGamePoint]->setVisible(true);
	}

	// onMenuItemPlay();
	JumpToPoint(0);
}








bool GameTablePlayer::init()
{
	if (!GameDialog::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), false))
	{
		return false;
	}
	m_pGameTable = GameTableViewer::create();
	addChild(m_pGameTable);

	auto bgPanel = Sprite::create("playback/bgpanel.png");
	auto sz = bgPanel->getContentSize();
	bgPanel->setPosition(sz.width / 2, sz.height / 2);
	addChild(bgPanel);

	m_pTimeLine = ProgressTimer::create(Sprite::create("playback/progress.png"));
	m_pTimeLine->setPosition(Vec2(578, 22));
	m_pTimeLine->setType(ProgressTimer::Type::BAR);
    m_pTimeLine->setMidpoint(Vec2(0, 0.5));
    m_pTimeLine->setBarChangeRate(Vec2(1, 0));
	addChild(m_pTimeLine);

	m_pMenuItemPlay = MenuItemFrameSprite::create("playback/btplay.png", 3, CC_CALLBACK_0(GameTablePlayer::onMenuItemPlay, this));
	m_pMenuItemPlay->setPosition(58, 62);
	m_pMenuItemPause = MenuItemFrameSprite::create("playback/btpause.png", 3, CC_CALLBACK_0(GameTablePlayer::onMenuItemPause, this));
	m_pMenuItemPause->setPosition(58, 62);
	auto pMenuItemLeave = MenuItemFrameSprite::create("playback/btback.png", 2, CC_CALLBACK_0(GameTablePlayer::onMenuItemLeave, this));
	pMenuItemLeave->setPosition(49, GameData::getInstance()->getGameHeight()-40);
	auto menu = Menu::create(m_pMenuItemPlay, m_pMenuItemPause, pMenuItemLeave, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu);
	char *szJumpTip[] = {"开局", "翻牌", "转牌", "河牌"};
	unsigned uIdx = 0;
	for (auto& bt : m_pMenuItemGamePoint)
	{
		bt = MenuItemFrameSprite::create("playback/btjump.png", 3, CC_CALLBACK_1(GameTablePlayer::onMenuItemJumpToPoint, this));
		bt->setPosition(256, 77);
		bt->setVisible(false);
		auto sz = bt->getContentSize();
		auto label = CreateLabelMSYHBD(24, szJumpTip[uIdx++]);
		label->setTextColor(Color4B(0xc1, 0x9b, 0x3f, 255));
		label->setPosition(sz.width / 2, sz.height / 2 + 2);
		bt->addChild(label);
		menu->addChild(bt);
	}
	
	m_pLabelAllTime = CreateLabelMSYH(18, "");
	m_pLabelAllTime->setPosition(1067, 22);
	addChild(m_pLabelAllTime);

	return true;
}

void GameTablePlayer::ClearTable()
{
	m_pGameTable->Clear();
}
void GameTablePlayer::JumpToPoint(unsigned uGamePoint)// 0-3 开局、翻牌、转牌、河牌
{
	GameRecordScence *pSence = nullptr;
	for (auto& item : m_vecScence)
	{
		if (item.m_uGamePoint == uGamePoint)
		{
			pSence = &item;
			break;
		}
	}
	if (pSence)
	{
		// 清理桌子，恢复场景
		ClearTable();
        m_pGameTable->SetCardType(uGamePoint);
		for (auto user : pSence->m_vecPlayer)
		{
			unsigned uCidView = m_chairViewConvertor.GetViewChairID(user.m_uChairID);
			m_pGameTable->SetUser(uCidView, user.m_strName, user.m_strIcon, user.m_dRestCash, user.m_uUserID,user.m_uVoice, user.m_uSkin);
			m_pGameTable->SetUsrStatus(uCidView, user.m_uUserLastAction); // TODO
			if (user.m_dUserBeted > 1)
			{				
				m_pGameTable->SetUsrBetedJetton(uCidView, user.m_dUserBeted);
			}
            if (uGamePoint != 0)
                m_pGameTable->SetDispatchHandCardVisible(uCidView);
		}
		m_pGameTable->SetPublicCard(pSence->m_vecPublicCard);
		if (!m_vecHandCard.empty()/* && m_vecHandCard[0] != 0 */&& uGamePoint != 0)
		{
			m_pGameTable->SetHandCard(&m_vecHandCard[0]);
		}
		m_pGameTable->SetAllPot(pSence->m_dAllPot);
		m_pGameTable->SetSidePot(pSence->m_vecSidePot);

		// 查找下一条命令
		m_dNextCmdWaitTime = -1;
		m_iCurCmd = 0;
		do
		{
			if (m_vecCmdItem[m_iCurCmd].m_dTick > pSence->m_dTick)
			{
				m_dNextCmdWaitTime = m_vecCmdItem[m_iCurCmd].m_dTick - pSence->m_dTick;
				break;
			}
			++m_iCurCmd;
		} while (m_iCurCmd < m_vecCmdItem.size());

		//// 准备加载命令
		//if (m_iCurCmd < m_vecCmdItem.size() && fWaitNextCmd >= 0)
		//{
		//	unschedule(schedule_selector(GameTablePlayer::onSchedultGetNextCmd));
		//	scheduleOnce(schedule_selector(GameTablePlayer::onSchedultGetNextCmd), fWaitNextCmd);
		//}

		// 设置进度条位置
		if (!isScheduled(schedule_selector(GameTablePlayer::onScheduleProgress)))
		{
			schedule(schedule_selector(GameTablePlayer::onScheduleProgress), 0.05);
		}
		m_pTimeLine->setPercentage(pSence->m_dTick / m_dAllTimeInMs * 100.0f);

		onMenuItemPlay();
	}
}
void GameTablePlayer::OnCmd(const GameRecordCmdItem& cmd)
{
	unsigned uCmd = cmd.m_uCmd;
	auto& msg = cmd.m_cmdData;
	switch (uCmd)
	{
	case GameMsgCmdS::GameStart:
		{
			unsigned uDUser = JsonValueAdapter(msg["bankerCh"]);
			unsigned uDUserView = m_chairViewConvertor.GetViewChairID(uDUser);

			auto players = msg["gameUser"];
			std::deque<unsigned> deqPlayers;
			std::vector<unsigned> vecDispatchPlayers;
			for (int i = 0; i < players.size(); i++)
			{
				unsigned uChair = JsonValueAdapter(players[i]);
				unsigned uViewChair = m_chairViewConvertor.GetViewChairID(uChair);
				deqPlayers.push_back(uViewChair);
			}
			
			while (deqPlayers[0] != uDUserView)
			{
				unsigned u = deqPlayers.front();
				deqPlayers.push_back(u);
				deqPlayers.pop_front();
			}
			for (int i = 0; i < deqPlayers.size(); i++)
			{
				vecDispatchPlayers.push_back(deqPlayers[i]);
			}
			m_pGameTable->SetDPosition(uDUserView);


			auto arrPool = msg["allPool"];
			std::vector<double> vecSidePot;
			double dAllPot = 0;
			for (int i = 0; i < arrPool.size(); i++)
			{
				if (i == 0)
				{
					dAllPot = JsonValueAdapter(arrPool[i]);
				}
				else
				{
					vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
				}
			}
			m_pGameTable->SetAllPot(dAllPot);
			if (!vecSidePot.empty())
			{
				m_pGameTable->SetSidePot(vecSidePot);
			}	

			unsigned char cbMyHandCard[2];
			auto card = msg["cardData"];
			cbMyHandCard[0] = static_cast<unsigned>(JsonValueAdapter(card[0u]));
			cbMyHandCard[1] = static_cast<unsigned>(JsonValueAdapter(card[1u]));
			m_pGameTable->DispatchHandCard(vecDispatchPlayers, cbMyHandCard);
		}
		break;
	case GameMsgCmdS::BetResult:
		{
			unsigned uChair = JsonValueAdapter(msg["betCh"]);
			unsigned uChairView = m_chairViewConvertor.GetViewChairID(uChair);
			double dBetValue = JsonValueAdapter(msg["betJettonNum"]);
			unsigned uBetType = JsonValueAdapter(msg["betType"]);
			unsigned uBlindType = JsonValueAdapter(msg["mangTag"]);
			m_pGameTable->SetUsrCash(uChairView, JsonValueAdapter(msg["addMoney"]));

			if (uBlindType == 1 || uBlindType == 2)
			{
				m_pGameTable->UsrPlaceJetton(uChairView, dBetValue);
			}
			else
			{
				if (uBetType == 0 || uBetType == 2)
				{
					m_pGameTable->UsrRaise(uChairView, dBetValue);
				}
				else if (uBetType == 1)
				{
					m_pGameTable->UsrCall(uChairView, dBetValue);
				}
				else if (uBetType == 3)
				{
					g_globalTableConfig.m_uSound = 1;
					m_pGameTable->UsrAllIn(uChairView, dBetValue);
				}
				else if (uBetType == 4)
				{
					m_pGameTable->UsrCheck(uChairView);
				}
				else if (uBetType == 5)
				{
					// m_pGameTable->UsrFoldCard(uChairView);
				}
			}

			auto arrPool = msg["allPool"];
			std::vector<double> vecSidePot;
			double dAllPot = 0;
			for (int i = 0; i < arrPool.size(); i++)
			{
				if (i == 0)
				{
					dAllPot = JsonValueAdapter(arrPool[i]);
				}
				else
				{
					vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
				}
			}

			bool bTurnOver = JsonValueAdapter(msg["ifTurnOver"]);
			if (bTurnOver)
			{
				m_pGameTable->CollectUsrBetToPot(vecSidePot);

				auto publicCards = msg["cardData"];
				std::vector<unsigned char> vecPublicCard;
				for (int i = 0; i < publicCards.size(); i++)
				{
					vecPublicCard.push_back(static_cast<unsigned>(JsonValueAdapter(publicCards[i])));
				}
				m_pGameTable->DispatchPublicCard(vecPublicCard);				
			}
			else // 一局尚未平衡
			{
			}
			if (uBlindType != 1)
			{
				unsigned uCurChair = JsonValueAdapter(msg["nextChairId"]);
				unsigned uCurChairView = m_chairViewConvertor.GetViewChairID(uCurChair);
				m_pGameTable->WaitUsrAction(uCurChairView);
			}
		}
		break;
	case GameMsgCmdS::FoldCardResult:
		{
			unsigned uChair = JsonValueAdapter(msg["giveUpCh"]);
			unsigned uChairView = m_chairViewConvertor.GetViewChairID(uChair);
			m_pGameTable->UsrFoldCard(uChairView);

			unsigned uCurChair = JsonValueAdapter(msg["nextChairId"]);
			unsigned uCurChairView = m_chairViewConvertor.GetViewChairID(uCurChair);
			m_pGameTable->WaitUsrAction(uCurChairView);
		}
		break;
	case GameMsgCmdS::GameEnd:
		{
			bool bNeedOpenCard = JsonValueAdapter(msg["ifNormalEnd"]);
			if (bNeedOpenCard)
			{
				// CAODANDE 最后一次下注信息以及收集筹码到边池 这个放在游戏结束消息里显然不太好，待协议优化吧
				unsigned uChair = JsonValueAdapter(msg["betCh"]);
				unsigned uChairView = m_chairViewConvertor.GetViewChairID(uChair);
				double dBetValue = JsonValueAdapter(msg["betJettonNum"]);
				unsigned uBetType = JsonValueAdapter(msg["betType"]);
				g_globalTableConfig.m_uSound = JsonValueAdapter(msg["sound"]);
				if (uBetType == 0 || uBetType == 2)
				{
					m_pGameTable->UsrRaise(uChairView, dBetValue);
				}
				else if (uBetType == 1)
				{
					m_pGameTable->UsrCall(uChairView, dBetValue);
				}
				else if (uBetType == 3)
				{
					m_pGameTable->UsrAllIn(uChairView, dBetValue);
				}
				else if (uBetType == 4)
				{
					m_pGameTable->UsrCheck(uChairView);
				}

				auto arrPool = msg["allPool"];
				std::vector<double> vecSidePot;
				double dAllPot = 0;
				for (int i = 0; i < arrPool.size(); i++)
				{
					if (i == 0)
					{
						dAllPot = JsonValueAdapter(arrPool[i]);
					}
					else
					{
						vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
					}
				}
				m_pGameTable->CollectUsrBetToPot(vecSidePot);
				// CAODANDE end

				m_pGameTable->ActivateUserStatus(US_NORMAL, US_NORMAL | US_WAITNEXTGAME);

				auto opencardChair = msg["showCardUser"];
				auto openCard = msg["cardData"];
				std::vector<unsigned> vecOpenChairs;
				std::vector<unsigned> vecOpenCardData;
				for (unsigned i = 0; i < opencardChair.size(); i++)
				{
					unsigned uChair = JsonValueAdapter(opencardChair[i]);
					unsigned uChairV = m_chairViewConvertor.GetViewChairID(uChair);
					vecOpenChairs.push_back(uChairV);
					vecOpenCardData.push_back(JsonValueAdapter(openCard[i*2]));
					vecOpenCardData.push_back(JsonValueAdapter(openCard[i*2+1]));
				}		
				m_pGameTable->OpenUsrCard(vecOpenChairs, vecOpenCardData);
			}
			else
			{
				m_pGameTable->ActivateUserStatus(US_NORMAL, US_FOLD | US_WAITNEXTGAME);
				// CAODANDE begin
				auto arrPool = msg["allPool"];
				std::vector<double> vecSidePot;
				double dAllPot = 0;
				for (int i = 0; i < arrPool.size(); i++)
				{
					if (i == 0)
					{
						dAllPot = JsonValueAdapter(arrPool[i]);
					}
					else
					{
						vecSidePot.push_back(JsonValueAdapter(arrPool[i]));
					}
				}
				// CAODANDE end
				m_pGameTable->CollectUsrBetToPot(vecSidePot);
			}
			double dPotsMoney[GAME_PLAYER];
			for (auto& item : dPotsMoney)
			{
				item = 0;
			}
			auto allPool = msg["allPool"];
			if (allPool.size() > 0) // 这里有可能发过来一个空的allPool，导致下面的代码会溢出崩溃
			{
				unsigned uPots = allPool.size()-1;
				for (unsigned i = 0; i < uPots; i++)
				{
					dPotsMoney[i] = JsonValueAdapter(allPool[i+1]);
				}
			}

			UsrWinInfo usrWinInfos[GAME_PLAYER];
			memset(usrWinInfos, 0, sizeof(usrWinInfos));
			auto usrWinPools = msg["poolsMoney"];
			for (unsigned i = 0; i < usrWinPools.size(); i++)
			{
				auto potWinInfo = usrWinPools[i];
				unsigned uPot = JsonValueAdapter(potWinInfo["id"]);
				auto potWinners = potWinInfo["gameUserCHairs"];
				unsigned uPotWinners = potWinners.size();
				double dWinValue = dPotsMoney[uPot] / uPotWinners;
				for (int j = 0; j < uPotWinners; j++)
				{
					unsigned uChair = JsonValueAdapter(potWinners[j]);
					usrWinInfos[uChair].uChair = uChair;
					usrWinInfos[uChair].uWinPotsIdx[usrWinInfos[uChair].uWinPots] = uPot;
					usrWinInfos[uChair].dPotsWinValue[usrWinInfos[uChair].uWinPots] = dWinValue;
					++usrWinInfos[uChair].uWinPots;
				}
			}

			auto usrWinLose = msg["showWinLoseMoney"];
			std::vector<UsrWinInfo> vecUsrWinInfo;
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				usrWinInfos[i].dAllWinValue = JsonValueAdapter(usrWinLose[i]);
				if (usrWinInfos[i].uWinPots > 0)
				{
					usrWinInfos[i].uChair = m_chairViewConvertor.GetViewChairID(usrWinInfos[i].uChair);
                    usrWinInfos[i].bIsWinner = false;
					vecUsrWinInfo.push_back(usrWinInfos[i]);

					if (usrWinInfos[i].uChair == m_chairViewConvertor.GetMyViewChairID())
					{
						m_pGameTable->ShowMeHasWin();
					}
				}
			}

			unsigned winType = JsonValueAdapter(msg["winMaxType"]);
			std::vector<unsigned> vecWinMaxChairs;
			auto winChairs = msg["winMaxChairs"];
			for (int i = 0; i < winChairs.size(); i++)
			{
				auto ch = JsonValueAdapter(winChairs[i]);
				vecWinMaxChairs.push_back(m_chairViewConvertor.GetViewChairID(ch));
                for (auto& itemUserWinInfo : vecUsrWinInfo)
                {
                    if (itemUserWinInfo.uChair == m_chairViewConvertor.GetViewChairID(ch))
                    {
                        itemUserWinInfo.bIsWinner = true;
                    }
                }
			}
			m_pGameTable->ShowUsrWinInfo(winType, vecWinMaxChairs, bNeedOpenCard, vecUsrWinInfo);
		}
		break;
	case GameMsgCmdS::PlayerLeave:
		{
			int iUserID = JsonValueAdapter(msg["id"]);
			bool bLookOn = JsonValueAdapter(msg["ifLookOn"]);
			if (!bLookOn)
			{				
				unsigned uChairID = JsonValueAdapter(msg["cid"]);
				unsigned uViewChairID = m_chairViewConvertor.GetViewChairID(uChairID);
				m_pGameTable->UsrStandUp(uViewChairID);
			}
		}
		break;

	case GameMsgCmdS::UpdateMoney:
		{
			unsigned uChairID = JsonValueAdapter(msg["cid"]);
			unsigned uViewChairID = m_chairViewConvertor.GetViewChairID(uChairID);
			double dSupply = JsonValueAdapter(msg["addMoney"]);
			double dRest = JsonValueAdapter(msg["acountMoney"]);
			m_pGameTable->SetUsrCash(uViewChairID, dSupply);
		}
		break;

	default:
		break;
	}
}

void GameTablePlayer::onSetVisible(bool bVisible)
{
	if (!bVisible)
	{
		onMenuItemPlay();
		unschedule(schedule_selector(GameTablePlayer::onSchedultGetNextCmd));
		unschedule(schedule_selector(GameTablePlayer::onScheduleProgress));
		ClearTable();
	}
}
void GameTablePlayer::onMenuItemPlay()
{
	if (Director::getInstance()->isPaused())
	{
		Director::getInstance()->resume();
	}
	if (m_iCurCmd == m_vecCmdItem.size())
	{
		JumpToPoint(0);
	}
	m_pMenuItemPause->setVisible(true);
	m_pMenuItemPlay->setVisible(false);
}
void GameTablePlayer::onMenuItemPause()
{
	if (!Director::getInstance()->isPaused())
	{
		Director::getInstance()->pause();
	}
	m_pMenuItemPause->setVisible(false);
	m_pMenuItemPlay->setVisible(true);
}

void GameTablePlayer::onMenuItemLeave()
{
    PlayBackGroundSound("sound/bghall1.mp3", true);
	setVisible(false);
}
void GameTablePlayer::onMenuItemJumpToPoint(cocos2d::Ref*pMenuItem)
{
	for (unsigned i = 0; i < 4; i++)
	{
		if (pMenuItem == m_pMenuItemGamePoint[i])
		{
			JumpToPoint(i);
			break;
		}
	}
}
void GameTablePlayer::onScheduleProgress(float fDelta)
{
	float fPer = m_pTimeLine->getPercentage();
	fPer += (fDelta * 1000.0f / float(m_dAllTimeInMs) * 100.0f);
	m_pTimeLine->setPercentage(fPer);
	if (m_iCurCmd < m_vecCmdItem.size() && m_dNextCmdWaitTime >= 0)
	{
		unschedule(schedule_selector(GameTablePlayer::onSchedultGetNextCmd));
		scheduleOnce(schedule_selector(GameTablePlayer::onSchedultGetNextCmd), m_dNextCmdWaitTime / 1000.0f);
		m_dNextCmdWaitTime = -1;
	}
}

void GameTablePlayer::onSchedultGetNextCmd(float fDelta)
{
	if (m_iCurCmd < m_vecCmdItem.size())
	{
		auto& cmd = m_vecCmdItem[m_iCurCmd];
		OnCmd(cmd);
		++m_iCurCmd;
		if (m_iCurCmd < m_vecCmdItem.size())
		{
			auto& cmdNext = m_vecCmdItem[m_iCurCmd];
			m_dNextCmdWaitTime = cmdNext.m_dTick - cmd.m_dTick;
			if (m_dNextCmdWaitTime < 10)
			{
				m_dNextCmdWaitTime = 100;
			}
		}
		else // 已经到了最后一条命令，显示播放按钮
		{
			m_dNextCmdWaitTime = -1;
			unschedule(schedule_selector(GameTablePlayer::onScheduleProgress));
			m_pMenuItemPause->setVisible(false);
			m_pMenuItemPlay->setVisible(true);
		}
	}
}

void GameTableViewer::SetDispatchHandCardVisible(unsigned uCidView)
{
    if (uCidView != MY_CHAIR_ID)
    {
        if (m_pSpriteDispatchingCard[uCidView*2])
        {		
            m_pSpriteDispatchingCard[uCidView*2]->setVisible(true);
            m_pSpriteDispatchingCard[uCidView*2]->setPosition(s_ptDispatchedCard[uCidView]);
        }
        if (m_pSpriteDispatchingCard[uCidView*2 + 1])
        {		
            m_pSpriteDispatchingCard[uCidView*2 + 1]->setVisible(true);
            m_pSpriteDispatchingCard[uCidView*2 + 1]->setPosition(s_ptDispatchedCard[GAME_PLAYER + uCidView]);
        }
    }
}

void GameTableViewer::OnMenuAction(Ref *pSend, ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        if (pSend == m_btnQiPai)
        {
            funQiPai();
        }
        else if (pSend == m_btnGenZhu)
        {
            funGenZhu();
        }
        else if (pSend == m_btnJiaZhu)
        {
            funJiaZhu();
        }
    }
}

void GameTableViewer::SetCallback(std::function<void()> fun1, std::function<void()> fun2, std::function<void()> fun3)
{
    this->funQiPai = fun1;
    this->funGenZhu = fun2;
    this->funJiaZhu = fun3;
}

void GameTableViewer::StopWinAction()
{   //新手引导中特殊使用
    for (int i=0; i<4; i++)
    {
        if (m_pAniShowMaxCard[i])
        {
            m_pAniShowMaxCard[i]->setVisible(false) ;
        }
    }
    m_pSpriteShowMeWin->setVisible(false);
    m_pUsers[4]->StopWinAction();
    m_pUsers[8]->StopWinAction();
}

void GameTableViewer::ShowMaxCardType(bool IsVisible)
{
    m_pSpriteMaxCardType->setVisible(IsVisible);
}