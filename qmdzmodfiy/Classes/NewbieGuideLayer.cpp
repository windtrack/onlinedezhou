#include "NewbieGuideLayer.h"
#include "GrabTouch.h"
#include "Util.h"
#include "GrabKeyboard.h"
#include "GameCommon.h"
#include "NewbieGuideManager.h"
#include "GameData.h"
#include "GameMessageIO.h"
#include "JsonValueAdapter.h"
#include "UserView.h"
#include "jsonrpc/json/reader.h"
#include "MenuItemFrameSprite.h"
#include "PopGuideDialog.h"
#include "GameData.h"
/*#include "TextTip.h"*/
#include "GameFrame.h"
#include "GlobalUserData.h"
#include "ShowLabelInLineSequence.h"
#include "GameTipsManager.h"
USING_NS_CC;

enum 
{
    CardTypeZOrder = 5,
    TouchLayerZOrder,
    TrainTipsZOrder,
    BackGameZOrder,
    NewerTrainZOrder,
    TextTipZOrder
};

enum 
{
    TopArrow,
    BottomArrow,
    LeftArrow,
    RightArrow
};

void NewbieGuideLayer::onGrabTouchBegan(bool bHitDialog)
{
	cocos2d::log("test");
}

void NewbieGuideLayer::onKeyEscPressed()
{
	onMenuItemLeave();
}

void NewbieGuideLayer::execCommandItem(int step)
{
	NewbieStep* ns = NewbieMgr->getNewbieStep(step);
	if (ns == nullptr)
		return;

	if (ns->cmdType == 1) {
		Json::Value& value = ns->value;
		auto cfPlay = [this, value]() {
			StartPlay(value);
		};
		auto cfDispatchCard = [this]() {
			OnCmd(m_vecCmdItem[0]);
		};

		auto cfStep = [this]() {
			//NewbieMgr->m_curInTableStep++;
			m_stayInStep = false;
		};

		auto seq = Sequence::create(CallFunc::create(cfPlay), DelayTime::create(0.2), CallFunc::create(cfDispatchCard), DelayTime::create(1.5), CallFunc::create(cfStep), nullptr);
		this->runAction(seq);
	}
	else if (ns->cmdType == 2) {
		parseCmdItems(ns->value);
		schedule(schedule_selector(NewbieGuideLayer::OnScheduleCmdItem),  0.5f, m_vecTmpCmdItems.size(), 0.0f);
	}
}

bool NewbieGuideLayer::execCommandFinished()
{
	return m_vecTmpCmdIndex >= m_vecTmpCmdItems.size();
}

void NewbieGuideLayer::onEnter()
{
	GameDialog::onEnter();
	if (nullptr != m_pTouchGrabberDialog) {
		m_pTouchGrabberDialog->SetTouchSwallow(true);
	}
}

void NewbieGuideLayer::onExit()
{
	GameDialog::onExit();
}

void NewbieGuideLayer::StartPlay(const Json::Value& gameRecord)
{
	log("%s", gameRecord.toStyledString().c_str());

	setVisible(true);
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
	//m_pGameTable->SetGameID(m_strRecordID);

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
            if (player.m_uChairID == 0)
            {
                 player.m_strIcon = g_globalMyData.GetIconURL();
                 player.m_strName = g_globalMyData.GetNickName();
                 player.m_uSkin = g_globalMyData.GetSkinID();
            }
            else
            {
			    player.m_strIcon = players[j]["icon"].asString();
			    player.m_strName = players[j]["name"].asString();
                player.m_uSkin = JsonValueAdapter(players[j]["voiceid"]);
            }
			player.m_uUserLastAction = JsonValueAdapter(players[j]["action"]);
			player.m_dUserBeted = JsonValueAdapter(players[j]["bet"]);
			player.m_dRestCash = JsonValueAdapter(players[j]["cash"]);
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
		/*if (cmdItem.m_uCmd == GameMsgCmdS::GameStart)
		{
			unsigned uCidOfGameStart = JsonValueAdapter(cmds[i]["cid"]);
			if (uCidOfGameStart != uChairID)
			{
				continue;
			}
		}*/
		m_vecCmdItem.push_back(cmdItem);
	}
	std::sort(std::begin(m_vecCmdItem), std::end(m_vecCmdItem));

	// onMenuItemPlay();
	JumpToPoint(0);
}

void NewbieGuideLayer::parseCmdItems(Json::Value& value)
{
	m_vecTmpCmdItems.clear();
	auto& cmds = value["cmds"];
	GameRecordCmdItem cmdItem;
	for (unsigned i = 0; i < cmds.size(); ++i)
	{
		cmdItem.m_uID = JsonValueAdapter(cmds[i]["id"]);
		cmdItem.m_uCmd = JsonValueAdapter(cmds[i]["cmd"]);
		cmdItem.m_cmdData = cmds[i]["data"];
		/*if (cmdItem.m_uCmd == GameMsgCmdS::GameStart)
		{
			unsigned uCidOfGameStart = JsonValueAdapter(cmds[i]["cid"]);
			if (uCidOfGameStart != uChairID)
			{
				continue;
			}
		}*/
		m_vecTmpCmdItems.push_back(cmdItem);
	}
	std::sort(std::begin(m_vecTmpCmdItems), std::end(m_vecTmpCmdItems));

	m_vecTmpCmdIndex = 0;
}

void NewbieGuideLayer::OnScheduleCmdItem(float delta)
{
	if (m_vecTmpCmdIndex < m_vecTmpCmdItems.size()) {
		OnCmd(m_vecTmpCmdItems[m_vecTmpCmdIndex]);
		m_vecTmpCmdIndex++;
	}
}

bool NewbieGuideLayer::init()
{
	if (!GameDialog::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), false))
	{
		return false;
	}

	NewbieMgr->loadConfigs();

	m_vecTmpCmdIndex = 0;
	m_stayInStep = false;
    m_pTextTip = nullptr;

	m_pGameTable = GameTableViewer::create();
	addChild(m_pGameTable);
    m_pGameTable->setTableType(NormalTable) ;

    m_spriteVideoplay[0] = CreateNode1AndPosAddTo<Sprite>("video/btopenvideo.png", GameData::getInstance()->s_ptUserViewDefault[3].x+76, GameData::getInstance()->s_ptUserViewDefault[3].y+10, this, 0);
    m_spriteVideoplay[1] = CreateNode1AndPosAddTo<Sprite>("video/btopenvideo.png", GameData::getInstance()->s_ptDPosition[4].x-4, GameData::getInstance()->s_ptDPosition[4].y-32, this, 0);
    m_spriteVideoplay[2] = CreateNode1AndPosAddTo<Sprite>("video/btopenvideo.png", GameData::getInstance()->s_ptUserViewDefault[8].x-80, GameData::getInstance()->s_ptUserViewDefault[8].y-65, this, 0);
    for (int i=0; i<3; i++)
        m_spriteVideoplay[i]->setVisible(false);
    m_spriteHeadBorder = CreateNode1AndPosAddTo<Sprite>("newbieguide/headframe.png", GameData::getInstance()->s_ptUserViewDefault[3].x, GameData::getInstance()->s_ptUserViewDefault[3].y, this, 0);
    m_spriteHeadBorder->setVisible(false);

    m_pGameTable->SetCallback(std::bind(&NewbieGuideLayer::OnMenuQiPai,this), std::bind(&NewbieGuideLayer::OnMenuGenZhu,this), std::bind(&NewbieGuideLayer::OnMenuJiaZhu,this));

	/*auto menuItem = MenuItemFrameSprite::create("playback/btplay.png", 3, CC_CALLBACK_0(NewbieGuideLayer::onMenuPlay, this));
	menuItem->setPosition(58, 62);*/

    auto pMenuItemLeave = MenuItemFrameSprite::create("playback/btback.png", 2, CC_CALLBACK_0(NewbieGuideLayer::onMenuItemLeave, this));
    pMenuItemLeave->setPosition(49, GameData::getInstance()->getGameHeight()-40);

	auto menu = Menu::create(pMenuItemLeave, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, BackGameZOrder);

    m_trainTips = TrainTips::create();
    auto sz = m_trainTips->getContentSize();
    m_trainTips->setPosition(GameData::getInstance()->getGameWidth() - sz.width/2 - 155, GameData::getInstance()->getGameHeight() - sz.height/2 - 10);
    this->addChild(m_trainTips, TrainTipsZOrder);
    m_trainTips->SetFunCallBack(std::bind(&NewbieGuideLayer::OnMenuMaxCardType, this), std::bind(&NewbieGuideLayer::OnMenuMaxCardType, this), std::bind(&NewbieGuideLayer::OnMenuMaxCardType, this));
    m_trainTips->setVisible(false);

    //牌型介绍
    m_spriteCardType = Sprite::create("doormenu/bgcardtype.png");
    auto spriteSz = m_spriteCardType->getContentSize();
    m_spriteCardType->setPosition(spriteSz.width/2 + 10, GameData::getInstance()->getGameHeight()/2);
    m_spriteCardType->setVisible(false);
    this->addChild(m_spriteCardType, CardTypeZOrder);
    
    //最大牌型上面的动画
    m_CardTypeEffect = Sprite::create();
    auto posCardType = m_pGameTable->m_imgCardType->getPosition();
    m_CardTypeEffect->setPosition(posCardType.x + 18, posCardType.y);
    auto animate = CreateAnimate("newbieguide/efect.png", 4, 4, 16, 1/12.0f, false);
    m_CardTypeEffect->runAction(RepeatForever::create(animate));
    m_CardTypeEffect->setVisible(false);
    m_pGameTable->ShowMaxCardType(false);
    this->addChild(m_CardTypeEffect);

    //操作箭头动画
    m_spriteArrow = Sprite::create("newbieguide/optarrow.png");
    m_spriteArrow->setPosition(m_pGameTable->m_btnGenZhu->getPosition().x, m_pGameTable->m_btnGenZhu->getPosition().y + 100);
    this->addChild(m_spriteArrow);
    auto jump = JumpBy::create(0.5f, Vec2::ZERO, 15.0, 1);
    m_spriteArrow->runAction(RepeatForever::create(jump));
    m_spriteArrow->setVisible(false);

    auto gameMall = CreateNode1AndPosAddTo<Sprite>("newbieguide/gamemall.png", GameData::getInstance()->getGameWidth()-45, GameData::getInstance()->getGameHeight()-45, this, 0);
    auto gameInvite = CreateNode1AndPosAddTo<Sprite>("newbieguide/invitefriend.png", GameData::getInstance()->getGameWidth()-130, GameData::getInstance()->getGameHeight()-45, this, 0);
    auto gameChat = CreateNode1AndPosAddTo<Sprite>("newbieguide/chat.png", 68, 47, this, 0);
    //黄色边框
    float ptX = 0, ptY = 0;
    m_sprite9Border[0] = createBorder(Size(90*3, 120), GameData::getInstance()->s_ptPublicCard[2], this);//公共牌
    ptX = (GameData::getInstance()->s_ptMyCard[0].x + GameData::getInstance()->s_ptMyCard[1].x) / 2;
    ptY = GameData::getInstance()->s_ptMyCard[0].y;
    m_sprite9Border[1] = createBorder(Size(140, 170), Vec2(ptX, ptY), this);//手牌
    ptX = m_pGameTable->m_btnGenZhu->getContentSize().width;
    ptY = m_pGameTable->m_btnGenZhu->getContentSize().height;
    m_sprite9Border[2] = createBorder(Size(ptX-1, ptY), Vec2(m_pGameTable->m_btnGenZhu->getPosition().x-1, m_pGameTable->m_btnGenZhu->getPosition().y+2), this);//单个按钮
    ptX = ptX*3+5;
    m_pGameTable->m_btnGenZhu->getParent()->setVisible(true);
     
    m_sprite9Border[3] = createBorder(Size(ptX, ptY), Vec2(m_pGameTable->m_btnGenZhu->getPosition().x-1, m_pGameTable->m_btnGenZhu->getPosition().y+2), this);//三个按钮
    m_sprite9Border[4] = createBorder(Size(330, 45), Vec2(176, 343), m_spriteCardType);//牌型提示
    for(int i=0; i<4; i++)
    {
        m_sprite9Border[i]->setVisible(false);
    }
    
    //界面提示
    m_sprite9Tips[0] = PopMinTips(RightArrow, "底池", Size(86,54), 40, Vec2(GameData::getInstance()->s_ptDiChi.x-140, GameData::getInstance()->s_ptDiChi.y), this);
    m_sprite9Tips[1] = PopMinTips(LeftArrow, "公共牌", Size(110,54), 40, Vec2(GameData::getInstance()->s_ptPublicCard[4].x+150, GameData::getInstance()->s_ptPublicCard[4].y), this);
    m_sprite9Tips[2] = PopMinTips(BottomArrow, "您的手牌", Size(150,54), 20, Vec2(GameData::getInstance()->s_ptMyCard[1].x+30, GameData::getInstance()->s_ptMyCard[1].y+90), this);
    m_sprite9Tips[3] = PopMinTips(RightArrow, "庄家", Size(86,54), 40, Vec2(GameData::getInstance()->s_ptDPosition[4].x-105, GameData::getInstance()->s_ptDPosition[4].y), this);
    m_sprite9Tips[4] = PopMinTips(RightArrow, "您携带的筹码", Size(190,54), 40, Vec2(GameData::getInstance()->s_ptUserViewDefault[4].x-210, GameData::getInstance()->s_ptUserViewDefault[4].y-65), this);
    m_sprite9Tips[5] = PopMinTips(LeftArrow, "牌型提示", Size(140,54), 40, Vec2(posCardType.x+120, posCardType.y-20), this);
    m_sprite9Tips[6] = PopMinTips(TopArrow, "点击玩家头像查看详\n细信息与TA互动", Size(270,88), 100, Vec2(GameData::getInstance()->s_ptUserViewDefault[8].x+25, GameData::getInstance()->s_ptUserViewDefault[8].y-150), this);
    m_sprite9Tips[7] = PopMinTips(TopArrow, "邀请好友", Size(140,54), 180, Vec2(gameInvite->getPosition().x, gameInvite->getPosition().y-235), this);
    m_sprite9Tips[8] = PopMinTips(TopArrow, "增加筹码", Size(120,54), 40, Vec2(gameMall->getPosition().x-15, gameMall->getPosition().y-100), this);
    m_sprite9Tips[9] = PopMinTips(RightArrow, "现场视频", Size(140,54), 40, Vec2(GameData::getInstance()->s_ptDPosition[4].x-136, GameData::getInstance()->s_ptDPosition[4].y-32), this);
    m_sprite9Tips[10] = PopMinTips(LeftArrow, "聊天系统", Size(140,54), 40, Vec2(gameChat->getPosition().x+140, gameChat->getPosition().y), this);
    for(int i=0; i<11; i++)
    {
        m_sprite9Tips[i]->setVisible(false);
    }

    m_pTouchHandler = NewbieTouchLayer::create();
	this->addChild(m_pTouchHandler, TouchLayerZOrder);

	m_pTouchHandler->setTouchCallback([this]() {
		//onMenuPlay();
        PlayGuideStep(m_uCurStep);
	});

    m_newerTrain = NewerTrain::create(1);
    m_newerTrain->SetFunCallBack(std::bind(&NewbieGuideLayer::OnNewTrainBtn, this),std::bind(&NewbieGuideLayer::OnNewTrainLable, this),std::bind(&NewbieGuideLayer::OnStartGameBtn, this),std::bind(&NewbieGuideLayer::OnStartGameLable, this));
    m_newerTrain->setPosition((GameData::getInstance()->getGameWidth()- m_newerTrain->getContentSize().width)/2, (GameData::getInstance()->getGameHeight() - m_newerTrain->getContentSize().height)/2);
    this->addChild(m_newerTrain, NewerTrainZOrder);
    m_newerTrain->setVisible(false);

	return true;
}

void NewbieGuideLayer::restart()
{
    this->setVisible(true);
    this->setTouchEnabled(true);
    m_pGameTable->Clear();

    m_uCurStep = 1;
    m_uSpiltCurStep = 1;
    m_IsVisibleMenu = false;
    
    scheduleOnce(schedule_selector(NewbieGuideLayer::DelayShowLayer), 0.5f);
}


void NewbieGuideLayer::onMenuItemLeave()
{
	GameTipsManager::getInstance()->hideTips() ;
    PlayBackGroundSound("sound/bghall1.mp3", true);
    m_pTouchHandler->setVisible(false);
    m_pGameTable->Clear();
    
	if (g_pGameFrame != nullptr) {
		g_pGameFrame->DestroyNewbieGuide();
	}
}

void NewbieGuideLayer::ClearTable()
{
	m_pGameTable->Clear();
}

void NewbieGuideLayer::JumpToPoint(unsigned uGamePoint)// 0-3 开局、翻牌、转牌、河牌
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
            if (m_uCurStep == 1)
            {
			    m_pGameTable->SetUser(uCidView, user.m_strName, user.m_strIcon, user.m_dRestCash, user.m_uUserID,user.m_uSkin, user.m_uSkin, true);
            }
            else
                m_pGameTable->SetUser(uCidView, user.m_strName, user.m_strIcon, user.m_dRestCash, user.m_uUserID, user.m_uSkin,user.m_uSkin, false);
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
		//m_dNextCmdWaitTime = -1;
		//m_iCurCmd = 0;
		//do
		//{
		//	if (m_vecCmdItem[m_iCurCmd].m_dTick > pSence->m_dTick)
		//	{
		//		m_dNextCmdWaitTime = m_vecCmdItem[m_iCurCmd].m_dTick - pSence->m_dTick;
		//		break;
		//	}
		//	++m_iCurCmd;
		//} while (m_iCurCmd < m_vecCmdItem.size());
	}
}
void NewbieGuideLayer::OnCmd(const GameRecordCmdItem& cmd)
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

			std::vector<unsigned char> myHandCards;
			auto card = msg["cardData"];
			for (int i = 0; i < 6; i++) {
				myHandCards.push_back(static_cast<unsigned>(JsonValueAdapter(card[i])));
			}
			
			auto func1 = [this, vecDispatchPlayers, myHandCards]() {
				unsigned char cbMyHandCard1[2] = {myHandCards[0], myHandCards[1]};
				m_pGameTable->DispatchHandCard(vecDispatchPlayers, cbMyHandCard1);
			};

			auto func2 = [this, vecDispatchPlayers, myHandCards]() {
				unsigned char cbMyHandCard1[2] = {myHandCards[2], myHandCards[3]};
				m_pGameTable->DispatchHandCard(vecDispatchPlayers, cbMyHandCard1);
			};

			auto func3 = [this, vecDispatchPlayers, myHandCards]() {
				unsigned char cbMyHandCard1[2] = {myHandCards[4], myHandCards[5]};
				m_pGameTable->DispatchHandCard(vecDispatchPlayers, cbMyHandCard1);
			};
			
			auto publicCards = msg["publicCardData"];
			std::vector<unsigned char> vecPublicCard;
			for (int i = 0; i < publicCards.size(); i++)
			{
				vecPublicCard.push_back(static_cast<unsigned>(JsonValueAdapter(publicCards[i])));
			}

			auto func4 = [this, vecPublicCard]() {
				m_pGameTable->DispatchPublicCard(vecPublicCard);	
			};
			
			auto seq = Sequence::create(CallFunc::create(func1), DelayTime::create(0.1),
										CallFunc::create(func2), DelayTime::create(0.1), 
										CallFunc::create(func3), DelayTime::create(0.1),
										CallFunc::create(func4), nullptr);
			this->runAction(seq);
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
					//g_globalTableConfig.m_uSound = 1;
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
			if (uBlindType != 1 && uBlindType != 100)
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
			//m_pGameTable->WaitUsrAction(uCurChairView);
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
				//g_globalTableConfig.m_uSound = JsonValueAdapter(msg["sound"]);
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

void NewbieGuideLayer::onSetVisible(bool bVisible)
{
	if (!bVisible)
	{
		ClearTable();
	}
}

void NewbieGuideLayer::onScheduleProgress(float fDelta)
{
	//float fPer = m_pTimeLine->getPercentage();
	//fPer += (fDelta * 1000.0f / float(m_dAllTimeInMs) * 100.0f);
	//m_pTimeLine->setPercentage(fPer);
	//if (m_iCurCmd < m_vecCmdItem.size() && m_dNextCmdWaitTime >= 0)
	{
		//unschedule(schedule_selector(GameTablePlayer::onSchedultGetNextCmd));
		//scheduleOnce(schedule_selector(GameTablePlayer::onSchedultGetNextCmd), m_dNextCmdWaitTime / 1000.0f);
		m_dNextCmdWaitTime = -1;
	}
}

void NewbieGuideLayer::onSchedultGetNextCmd(float fDelta)
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
		}
	}
}

void NewbieGuideLayer::PlayGuideStep(int step)
{
    if (step == 4 || step == 7 || step == 9)
        return;
    if (m_IsVisibleMenu)
        return;
    for(int i=0; i<4; i++)
    {
        m_sprite9Border[i]->setVisible(false);
    }
    m_trainTips->setVisible(false);
    m_spriteArrow->setVisible(false);
    m_spriteCardType->setVisible(false);
    m_CardTypeEffect->setVisible(false);
    setTouchDelegateEnabled(false);
    m_pGameTable->ShowMaxCardType(false);
    m_pGameTable->m_btnQiPai->setVisible(false);
    m_pGameTable->m_btnGenZhu->setVisible(false);
    m_pGameTable->m_btnJiaZhu->setVisible(false);
    switch(step)
    {
    case 1:
        {
            execCommandItem(1);
            scheduleOnce(schedule_selector(NewbieGuideLayer::ShowTips), 1.0f);
            scheduleOnce(schedule_selector(NewbieGuideLayer::SetTouchEnableVisible), 4.0f);
        }break;
    case 2:
        {
            for (int i=0; i<6; i++)
            {
                m_sprite9Tips[i]->setVisible(false);
            }
            m_pGameTable->ShowMaxCardType(true);
            m_trainTips->setVisible(true);
            m_spriteCardType->setVisible(true);
            m_sprite9Border[0]->setVisible(true);
            m_sprite9Border[1]->setVisible(true);
            scheduleOnce(schedule_selector(NewbieGuideLayer::SetTouchEnableVisible), 1.0f);
            m_uCurStep++;
        }break;
    case 3:
        {
            m_trainTips->setVisible(true);
            m_spriteArrow->setVisible(true);
            m_pGameTable->m_btnQiPai->setVisible(true);
            m_pGameTable->m_btnGenZhu->setVisible(true);
            m_pGameTable->m_btnJiaZhu->setVisible(true);
            m_pGameTable->m_btnQiPai->setEnabled(false);
            m_pGameTable->m_btnJiaZhu->setEnabled(false);
            setTouchDelegateEnabled(false);
            setUITextString(m_pGameTable->m_textGenZhu, "跟40");
            GetTextTip()->SystemTip("我们来对局一次吧，牌局开始，先发两张手牌", 1.5f);
            m_sprite9Border[2]->setVisible(true);
            execCommandItem(2);
            m_IsVisibleMenu = true;
        }break;
    case 4:
        {
        }break;
    case 5:
        {
            schedule(schedule_selector(NewbieGuideLayer::SpiltStep),  1.5f, 1, 0.5f);
            setTouchDelegateEnabled(false);
            m_IsVisibleMenu = true;
        }break;
    case 6:
        {
            scheduleOnce(schedule_selector(NewbieGuideLayer::SetTouchEnableVisible), 2.0f);
            m_trainTips->setVisible(true);
            m_spriteHeadBorder->setVisible(false);
            m_uCurStep++;
            step++;
        }break;
    case 7:
        {
        }break;
    case 8:
        {
            m_trainTips->setVisible(true);
            m_spriteArrow->setVisible(true);
            m_pGameTable->m_btnQiPai->setVisible(true);
            m_pGameTable->m_btnGenZhu->setVisible(true);
            m_pGameTable->m_btnJiaZhu->setVisible(true);
            m_pGameTable->m_btnQiPai->setEnabled(false);
            m_pGameTable->m_btnJiaZhu->setEnabled(false);
            setUITextString(m_pGameTable->m_textGenZhu, "跟50");
            m_sprite9Border[2]->setVisible(true);
            setTouchDelegateEnabled(false);
        }break;
    case 9:
        {
        }break;
    case 10:
        {
            execCommandItem(12);
            GetTextTip()->SystemTip("Tina下注了50", 1.5f);
            scheduleOnce(schedule_selector(NewbieGuideLayer::ShowTips), 2.0f);
            m_IsVisibleMenu = true;
            m_pGameTable->ShowMaxCardType(true);
        }break;
    case 11:
        {
            for (int i=6; i<11; i++)
            {
                m_sprite9Tips[i]->setVisible(true);
            }
            m_trainTips->setVisible(true);
            setTouchDelegateEnabled(true);
            m_uCurStep++;
            step++;
            g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskGuide);
        }break;
    case 12:
        {
            for (int i=6; i<11; i++)
            {
                m_sprite9Tips[i]->setVisible(false);
            }
            m_newerTrain->setVisible(true);
            setTouchDelegateEnabled(true);
        }break;
    }
    m_trainTips->UpdateTipsData(step - 1);
}

void NewbieGuideLayer::OnMenuQiPai()
{
    if(m_uCurStep == 5)
    {
        m_trainTips->UpdateBottomTips("您中了一对，不要现在就弃牌");
    }
}
void NewbieGuideLayer::OnMenuGenZhu()
{
    if(m_uCurStep == 3)
    {
        execCommandItem(3);
        m_trainTips->setVisible(false);
        m_sprite9Border[2]->setVisible(false);
        schedule(schedule_selector(NewbieGuideLayer::SpiltStep),  1.5f, 1, 1.5f);
        setTouchDelegateEnabled(true);
    }
    else if(m_uCurStep == 5)
    {
        execCommandItem(8);
        m_trainTips->setVisible(false);
        m_sprite9Border[3]->setVisible(false);
        schedule(schedule_selector(NewbieGuideLayer::SpiltStep),  1.5f, 1, 1.5f);
        m_IsVisibleMenu = false;
        setTouchDelegateEnabled(true);
        GetTextTip()->SystemTip("第二轮牌局结束，第三轮发第四张公共牌", 1.2f);
    }
    else if (m_uCurStep == 8)
    {
        execCommandItem(11);
        m_sprite9Border[2]->setVisible(false);
        setTouchDelegateEnabled(true);
        GetTextTip()->SystemTip("第三轮牌局结束，现在发第五张也是最后一张公共牌", 1.5f);
    }
    else if (m_uCurStep == 10)
    {
        execCommandItem(13);
        m_trainTips->setVisible(false);
        m_sprite9Border[2]->setVisible(false);
        setTouchDelegateEnabled(false);
        scheduleOnce(schedule_selector(NewbieGuideLayer::ShowTips), 1.5f);
        scheduleOnce(schedule_selector(NewbieGuideLayer::SetTouchEnableVisible), 7.0f);
        m_IsVisibleMenu = false;
    }
    m_trainTips->UpdateTipsData(m_uCurStep);
    m_uCurStep++;
    m_spriteArrow->setVisible(false);
    m_pGameTable->m_btnQiPai->setVisible(false);
    m_pGameTable->m_btnGenZhu->setVisible(false);
    m_pGameTable->m_btnJiaZhu->setVisible(false);
}
void NewbieGuideLayer::OnMenuJiaZhu()
{
    if(m_uCurStep == 5)
    {
        m_trainTips->UpdateBottomTips("有人可能拿到两张黑桃组成了同花牌型，比您现在的牌力强，保险点先别加注");
    }
}

void NewbieGuideLayer::OnMenuMaxCardType()
{
    m_CardTypeEffect->setVisible(true);
    m_pGameTable->ShowMaxCardType(true);
    if (m_uCurStep == 4 || m_uCurStep == 7 ||m_uCurStep == 9)
    {
        m_uCurStep++;
        m_IsVisibleMenu = false;
    }
}

void NewbieGuideLayer::ShowTips(float fDelta)
{
    m_trainTips->setVisible(true);
    if (m_uCurStep == 1)
    {
        m_pGameTable->ShowMaxCardType(true);
        for (int i=0; i<3; i++)
        {
            m_spriteVideoplay[i]->setVisible(true);
        }
        for (int i=0; i<6; i++)
        {
            m_sprite9Tips[i]->setVisible(true);
        }
        m_uCurStep++;
    }
    if (m_uCurStep == 5)
    {
        m_spriteArrow->setVisible(true);
        m_pGameTable->m_btnQiPai->setVisible(true);
        m_pGameTable->m_btnGenZhu->setVisible(true);
        m_pGameTable->m_btnJiaZhu->setVisible(true);
        m_pGameTable->m_btnQiPai->setEnabled(true);
        m_pGameTable->m_btnJiaZhu->setEnabled(true);
        setUITextString(m_pGameTable->m_textGenZhu, "让牌");
        m_sprite9Border[3]->setVisible(true);
    }
    else if (m_uCurStep == 6)
    {
        m_spriteHeadBorder->setVisible(true);
    }
    else if (m_uCurStep == 10)
    {
        m_spriteArrow->setVisible(true);
        m_pGameTable->m_btnQiPai->setVisible(true);
        m_pGameTable->m_btnGenZhu->setVisible(true);
        m_pGameTable->m_btnJiaZhu->setVisible(true);
        setUITextString(m_pGameTable->m_textGenZhu, "All in");
        m_sprite9Border[2]->setVisible(true);
    }
    else if (m_uCurStep == 11)
    {
        execCommandItem(14);
        scheduleOnce(schedule_selector(NewbieGuideLayer::StopWinAction), 5.0f);
    }
    if (m_uSpiltCurStep == 3 && m_uCurStep == 4)
    {
        GetTextTip()->SystemTip("第一轮下注结束，第二轮发三张公共牌", 1.5);
    }
}

void NewbieGuideLayer::SetTouchEnableVisible(float fDelta)
{
    setTouchDelegateEnabled(true);
}

void NewbieGuideLayer::StopWinAction(float fDelta)
{
    m_pGameTable->StopWinAction();
    execCommandItem(15);
}

void NewbieGuideLayer::SpiltStep(float fDelta)
{
    if (m_uSpiltCurStep == 1)
    {
        execCommandItem(4);
        GetTextTip()->SystemTip("Tina跟注", 1.2f);
    }
    else if (m_uSpiltCurStep == 2)
    {
        execCommandItem(5);
        GetTextTip()->SystemTip("Games让牌", 1.2f);
        scheduleOnce(schedule_selector(NewbieGuideLayer::ShowTips), 1.5f);
    }
    else if (m_uSpiltCurStep == 3)
    {
        execCommandItem(6);
    }
    else if (m_uSpiltCurStep == 4)
    {
        execCommandItem(7);
        scheduleOnce(schedule_selector(NewbieGuideLayer::ShowTips), 1.0f);
    }
    else if (m_uSpiltCurStep == 5)
    {
        execCommandItem(9);
        GetTextTip()->SystemTip("Tina下注了50", 1.2f);
    }
    else if (m_uSpiltCurStep == 6)
    {
        execCommandItem(10);
        GetTextTip()->SystemTip("Games弃牌", 1.2f);
        scheduleOnce(schedule_selector(NewbieGuideLayer::ShowTips), 0.5f);
    }

    m_uSpiltCurStep++;
}

ui::Scale9Sprite* NewbieGuideLayer::PopMinTips(unsigned direction, std::string strContent, cocos2d::Size szContent, unsigned arrowLen, Vec2 pt, Node* parent)
{	
    //内容区域
    ui::Scale9Sprite *content9Sprite = ui::Scale9Sprite::create("newbieguide/tipscontent.png", Rect(0, 0, 86, 54), Rect(16,16, 54, 22));
    content9Sprite->setPreferredSize(szContent);	content9Sprite->setAnchorPoint(Vec2(0.5, 0.5));
    CreateLabelMSYHBDAndAnchorPosClrAddTo(26, strContent, Vec2(0.5, 0.5), szContent.width/2, szContent.height/2, Color4B(0x4d,0x4d,0x4d,0xff),content9Sprite, 0);
    //水平箭头区域
    ui::Scale9Sprite *hori9Sprite = ui::Scale9Sprite::create("newbieguide/body1.png", Rect(0, 0, 23, 4), Rect(0,1, 23, 3));
    hori9Sprite->setPreferredSize(Size(arrowLen, 4));
    //垂直箭头区域
    ui::Scale9Sprite *verti9Sprite = ui::Scale9Sprite::create("newbieguide/body2.png", Rect(0, 0, 4, 23), Rect(1,0, 3, 23));
    verti9Sprite->setPreferredSize(Size(4, arrowLen));
    auto sz = hori9Sprite->getContentSize();
    sz = verti9Sprite->getContentSize();
    auto head = Sprite::create("newbieguide/head.png");
    if (direction == TopArrow)
    {		
        verti9Sprite->setAnchorPoint(Vec2(0.5, 0));
        verti9Sprite->addChild(head);
        head->setPosition(2, arrowLen);
        verti9Sprite->setPosition(szContent.width/2, szContent.height - 3);
        content9Sprite->addChild(verti9Sprite);
    }
    else if (direction == BottomArrow)
    {
        verti9Sprite->setFlippedY(true);
        verti9Sprite->setAnchorPoint(Vec2(0.5, 0));
        verti9Sprite->addChild(head);
        head->setPosition(2, arrowLen);
        verti9Sprite->setPosition(szContent.width/2, 6);
        content9Sprite->addChild(verti9Sprite);
    }	
    else if (direction == LeftArrow)
    {
        hori9Sprite->setAnchorPoint(Vec2(1, 0.5));
        hori9Sprite->addChild(head);
        head->setPosition(0, 2);
        hori9Sprite->setPosition(4, szContent.height/2);
        content9Sprite->addChild(hori9Sprite);
    }	
    else if (direction == RightArrow)
    {		
        hori9Sprite->setFlippedX(true);
        hori9Sprite->setAnchorPoint(Vec2(1, 0.5));
        hori9Sprite->addChild(head);
        head->setPosition(0, 2);
        hori9Sprite->setPosition(szContent.width - 4, szContent.height/2);
        content9Sprite->addChild(hori9Sprite);
    }
    content9Sprite->setPosition(pt);
    parent->addChild(content9Sprite);
    return content9Sprite;
}

cocos2d::ui::Scale9Sprite* NewbieGuideLayer::createBorder(Size sz, Vec2 pt, Node* parent)
{
    ui::Scale9Sprite *scale9Sprite = ui::Scale9Sprite::create("newbieguide/selectframe.png", Rect(0, 0, 96, 70), Rect(16,16, 64, 38));
    sz.width += 20;sz.height += 18;
    scale9Sprite->setContentSize(sz);
    scale9Sprite->setPosition(pt);
    parent->addChild(scale9Sprite);
    return scale9Sprite;
}

void NewbieGuideLayer::OnNewTrainBtn()//30秒新手特训
{
    log("秒新手特训");
}

void NewbieGuideLayer::OnNewTrainLable()//跳过教学
{
    //log("跳过教学");
}

void NewbieGuideLayer::OnStartGameBtn()//开始对局
{
    onMenuItemLeave();
    Json::Value param;
    param["enterType"] = -1;
    SendClientRequest(ClientRequest::kEnterTable, param);
}

void NewbieGuideLayer::OnStartGameLable()//再来一遍
{
    m_uCurStep = 1;
    m_uSpiltCurStep = 1;
    PlayGuideStep(m_uCurStep);
}

void NewbieGuideLayer::setTouchDelegateEnabled(bool enabled)
{
    if (nullptr != m_pTouchHandler) {
        m_pTouchHandler->setVisible(enabled);
    }
}

void NewbieGuideLayer::DelayShowLayer(float fDelta)
{
    PlayGuideStep(m_uCurStep);
}

TableTip* NewbieGuideLayer::GetTextTip()
{
    if (m_pTextTip == nullptr)
    {
        m_pTextTip = TableTip::create("tabletip/bg.png", "fzlt.ttf", 25, 800);
        auto szTip = m_pTextTip->getContentSize();
        auto sz = getContentSize();
        m_pTextTip->setPosition((sz.width - szTip.width) / 2, sz.height - szTip.height);
        addChild(m_pTextTip, TextTipZOrder);
        m_pTextTip->setTouchEnabled(false) ;
    }
    
    return m_pTextTip;
}

bool NewbieTouchLayer::init()
{
    if (!GameDialog::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), false)) {
        return false;
    }

    return true;
}

void NewbieTouchLayer::onGrabTouchBegan(bool bHitDialog)
{
    if (bHitDialog) {
        m_touchDelegate();
    }
    log("test");
}

void NewbieTouchLayer::setTouchCallback(std::function<void()> touchDelegate)
{
    m_touchDelegate = touchDelegate;
}

void NewbieTouchLayer::onKeyEscPressed()
{
    this->setVisible(false);
    SendClientRequest(ClientRequest::kEscNewbieGuide);
}
