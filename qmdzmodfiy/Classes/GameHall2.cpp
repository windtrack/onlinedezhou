#include "GameHall2.h"
#include "MenuItemFrameSprite.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "GameCommon.h"
#include "TextureNumber.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "DeZhouMagnate.h"
#include "GameCommon.h"
#include "AnimateViewer.h"
#include "AnimationManager.h"
#include "Tools.h"
#include "GameTipsManager.h"
#include "GuideTips.h"

USING_NS_CC;

bool GameHall2::init()
{
	if(!GameDialog::init())
	{
		return false;
	}
	//载入布局
	auto rootNode = CSLoader::createNode("res/ui/gamehall2.csb");
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	rootNode->setContentSize(frameSize); 
	ui::Helper::doLayout(rootNode);  
	addChild(rootNode);

	setContentSize(frameSize); 


	char szButton[64] = {0};
    auto magnateLayout = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_active3"));
	m_pDeZhouMagnate = DeZhouMagnate::create(rootNode);
	this->setPosition(0,0);
	magnateLayout->addChild(m_pDeZhouMagnate);

	// 背景重设
	//ImageView* img_bg = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"gamehall2bg"));
	//img_bg->getContentSize();
	// 桌子重设

	m_pSpriteBgTable = static_cast<Layout*>(Tools::getWidgetByName(rootNode,"Panel_bg"));
    auto imageLeft = static_cast<Layout*>(Tools::getWidgetByName(m_pSpriteBgTable,"desk_half0"));
    auto imageRight = static_cast<Layout*>(Tools::getWidgetByName(m_pSpriteBgTable,"desk_half1"));

    Sprite *bgLeft = Sprite::create("hall2half.png") ;
    Sprite *bgRignt = Sprite::createWithTexture(bgLeft->getTexture()) ;
    //bgRignt->setScaleX(-1);
    imageRight->setScaleX(-1) ;
    bgLeft->setPosition(imageLeft->getContentSize().width/2,imageLeft->getContentSize().height/2) ;
    bgRignt->setPosition(imageRight->getContentSize().width/2,imageRight->getContentSize().height/2);
    imageLeft->addChild(bgLeft);
    imageRight->addChild(bgRignt);

	// 左下角的卡牌
	m_pSpriteBgCard = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_card"));

	// 返回按钮
	m_pBtBack = static_cast<Button*>(Tools::getWidgetByName(rootNode,"bt_back"));
	m_pBtBack->addTouchEventListener(this,toucheventselector(GameHall2::onGameHall2TypeSelect)) ;

	// 初出茅庐按钮
	m_pBtSwitch[0] = static_cast<Button*>(Tools::getWidgetByName(rootNode,"bt_0"));
	m_pBtSwitch[0]->addTouchEventListener(this,toucheventselector(GameHall2::onGameHall2TypeSelect)) ;

	auto szBt0 = m_pBtSwitch[0]->getContentSize();
	m_pLabelSwitch[0] = CreateLabelMSYHBD(24, "初出茅庐");
	m_pLabelSwitch[0]->setPosition(szBt0.width/2, szBt0.height/2);
	m_pLabelSwitch[0]->setColor(Color3B(0xff, 0xff, 0xff));
	m_pBtSwitch[0]->addChild(m_pLabelSwitch[0]);

	// 小有成就按钮
	m_pBtSwitch[1] = static_cast<Button*>(Tools::getWidgetByName(rootNode,"bt_1"));
	m_pBtSwitch[1]->addTouchEventListener(this,toucheventselector(GameHall2::onGameHall2TypeSelect)) ;

	auto szBt1 = m_pBtSwitch[0]->getContentSize();
	m_pLabelSwitch[1] = CreateLabelMSYHBD(24, "小有成就");
	m_pLabelSwitch[1]->setPosition(szBt1.width/2, szBt1.height/2);
	m_pLabelSwitch[1]->setColor(Color3B(0x37, 0xa4, 0x5c));
	m_pBtSwitch[1]->addChild(m_pLabelSwitch[1]);

	// 大显身手按钮
	m_pBtSwitch[2] = static_cast<Button*>(Tools::getWidgetByName(rootNode,"bt_2"));
	m_pBtSwitch[2]->addTouchEventListener(this,toucheventselector(GameHall2::onGameHall2TypeSelect)) ;

	auto szBt2 = m_pBtSwitch[0]->getContentSize();
	m_pLabelSwitch[2] = CreateLabelMSYHBD(24, "大显身手");
	m_pLabelSwitch[2]->setPosition(szBt2.width/2, szBt2.height/2);
	m_pLabelSwitch[2]->setColor(Color3B(0x37, 0xa4, 0x5c));
	m_pBtSwitch[2]->addChild(m_pLabelSwitch[2]);

	m_pImageMenuBg = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "img_menu_bg"));

	// cocosstudio的Text会有灰灰的字体效果不好暂时隐藏掉不用
	for(int i = 0;i < 3;i++)
	{
		sprintf(szButton, "bt_Text_%d", i);
		m_pBtText[i] = static_cast<Text*>(Tools::getWidgetByName(rootNode,szButton));
		m_pBtText[i]->setVisible(false);
	}

	// 在线玩家TEXT显示
	m_pLabelAllInfo = static_cast<Text*>(Tools::getWidgetByName(rootNode,"Text_showuser"));

	// 德州币数量TEXT显示
	m_pLabelCash = static_cast<Text*>(Tools::getWidgetByName(rootNode,"Text_cash"));
	setUITextString(m_pLabelCash, FormatCash4(g_globalMyData.GetCash()));

	//  德州币精灵显示
	m_pSprite$ = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"cash"));

	// 进入商城的按钮
	m_pBtEnterMall = static_cast<Button*>(Tools::getWidgetByName(rootNode,"bt_shop"));
	m_pBtEnterMall->addTouchEventListener(this,toucheventselector(GameHall2::onGameHall2TypeSelect)) ;

	// 客户的按钮
	m_pBtEnterKeFu = static_cast<Button*>(Tools::getWidgetByName(rootNode,"bt_kefu"));
	m_pBtEnterKeFu->addTouchEventListener(this,toucheventselector(GameHall2::onGameHall2TypeSelect)) ;


	//播放贵族开房的帧动画
	m_pVipSpriteAni = AnimationManager::creatArmature("vipopenroom");
	m_pVipSpriteAni->ignoreAnchorPointForPosition(false);
	m_pVipSpriteAni->setAnchorPoint(Point(0.5,0.5));
	m_pVipSpriteAni->setPosition(64, -70);
	m_pBtEnterMall->addChild(m_pVipSpriteAni);
	m_pVipSpriteAni->playByIndexAndRepatForever(0);
	m_pVipSpriteAni->setVisible(false) ;


	// 对贵族开房的监听
	auto nBgItemCashListen = LayerButton::create(true, true);
	nBgItemCashListen->ignoreAnchorPointForPosition(false);
	nBgItemCashListen->setAnchorPoint(Point(0.5,0.5));
	m_pVipSpriteAni->addChild(nBgItemCashListen);
	auto szBt = m_pVipSpriteAni->getContentSize();
	nBgItemCashListen->setContentSize(szBt);
	nBgItemCashListen->SetHitCB([this](Ref*)
	{
		/*Json::Value param;
		SendClientRequest(ClientRequest::kGetVipAllOpenInfo, param);*/
		return ;
	});

	// 6人场的按钮
	auto pMenuItem0 = MenuItemFrameSprite::create("hall2/2dt_1video.png", 1, nullptr);
	auto pMenuItem1 = MenuItemFrameSprite::create("hall2/2dt_1six-table.png", 1, nullptr);
	m_pRadio6Or9 = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameHall2::onBtSwitchTable6or9, this), pMenuItem0, pMenuItem1, nullptr);
	m_pRadio6Or9->setPosition(Vec2(1051, 300));

	m_pMenu = Menu::create(m_pRadio6Or9, nullptr);
	m_pMenu->setPosition(Vec2::ZERO);
	rootNode->addChild(m_pMenu, 1);

	static Vec2 s_ptBlindInfo[6] = {
		Vec2(252, 236), Vec2(480, 236), Vec2(708, 236),
		Vec2(252, 24), Vec2(480, 24), Vec2(708, 24)
	};

	for (int i = 0; i < 6; i++)
	{
		sprintf(szButton, "Button_active%d",i);
		m_pBtQuickDoors[i]= static_cast<Button*>(Tools::getWidgetByName(rootNode,szButton));
		m_pBtQuickDoors[i]->addTouchEventListener(this,toucheventselector(GameHall2::onBtQuickEnter)) ;

		sprintf(szButton, "Text_actvie800name%d",i);
		m_pLabelMaxCarry[i]= static_cast<Text*>(Tools::getWidgetByName(rootNode,szButton));
		m_pLabelMaxCarry[i]->setTextColor(Color4B::WHITE);

		sprintf(szButton, "Image_actvie800_pepcount%d",i);
		m_pLabelPlayers[i]= static_cast<Text*>(Tools::getWidgetByName(rootNode,szButton));
		m_pLabelPlayers[i]->setTextHorizontalAlignment(TextHAlignment::LEFT);
		m_pLabelPlayers[i]->setTextColor(Color4B::WHITE);

		sprintf(szButton, "Image_player%d",i);
		m_pSpritePlayersFlag[i]= static_cast<ImageView*>(Tools::getWidgetByName(rootNode,szButton));
		m_pSpritePlayersFlag[i]->setVisible(false);

		sprintf(szButton, "Imageamer%d",i);
		m_pSpriteCamera[i]= static_cast<ImageView*>(Tools::getWidgetByName(rootNode,szButton));
		m_pSpriteCamera[i]->setVisible(true);

		// 大小盲注信背景
		sprintf(szButton, "Imagemangzhu%d",i);
		m_pSpriteBlindInfo[i]= static_cast<ImageView*>(Tools::getWidgetByName(rootNode,szButton));
		m_pSpriteBlindInfo[i]->setCascadeOpacityEnabled(true);

		// 大小盲注信信息
		sprintf(szButton, "textmangzhu%d",i);
		m_pLabelBlindInfo[i]= static_cast<Text*>(Tools::getWidgetByName(rootNode,szButton));
		m_pLabelBlindInfo[i]->setCascadeOpacityEnabled(true);
		m_pLabelBlindInfo[i]->setTextColor(Color4B(0xa2, 0xdf, 0x84, 255));
	}
	g_globalMyData.WaitChange(WAITCASHCHANGE, std::bind(&GameHall2::OnMyDataChanged, this, std::placeholders::_1));
	m_uButtionIndex = 0;

	return true;
}

void GameHall2::onGameHall2TypeSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type)
{
	Button* button = (Button*)pMenuItem;
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		button->setScale(1);
		if (pMenuItem == m_pBtBack)
		{
			SendClientRequest(ClientRequest::kLeaveHall2);
		}
		if (pMenuItem == m_pBtSwitch[0])
		{
			Activate(0);
			m_uButtionIndex = 0;
		}
		if (pMenuItem == m_pBtSwitch[1])
		{
			Activate(1);
			m_uButtionIndex = 1;
            createSwitchVideoOr6Guid();
		}
		if (pMenuItem == m_pBtSwitch[2])
		{
			Activate(2);
			m_uButtionIndex = 2;
		}
		if (pMenuItem == m_pBtEnterMall)
		{
			Json::Value param;
			param["idx"] = 0;
			SendClientRequest(ClientRequest::kPopMall, param);
		}
		if (pMenuItem == m_pBtEnterKeFu)
		{
            SendClientRequest(ClientRequest::kPopCustomerServices);
		}
	}
}

void GameHall2::onGrabTouchBegan(bool bHitDialog)
{
}

void GameHall2::onSetVisible(bool bVisible)
{
	static Vec2 s_ptSrc[6] = {
		Vec2(121, -350), Vec2(407, -350), Vec2(700, -350),
		Vec2(121, -350), Vec2(407, -350), Vec2(700, -350)
	};
	static Vec2 s_ptDst[6] = {
		Vec2(121, 390), Vec2(407, 390), Vec2(700, 390),
		Vec2(121, 155), Vec2(407, 155), Vec2(700, 155)
	};

	if (bVisible)
	{
		Activate(m_uButtionIndex);
		float fDelay = 0.5;
		unsigned uIdxs[] = {0, 1, 2, 3, 5, 4};
		for (int i = 0; i < 6; i++)
		{
			unsigned uIdx = uIdxs[i];
			m_pLabelMaxCarry[uIdx]->runAction(Sequence::create(DelayTime::create(fDelay), Show::create(), nullptr));
			m_pLabelMaxCarry[uIdx]->setVisible(true);
			m_pBtQuickDoors[uIdx]->runAction(Sequence::create(DelayTime::create(fDelay), EaseOut::create(MoveTo::create(0.15f, s_ptDst[uIdx]), 6.0), nullptr));
			m_pSpriteBlindInfo[uIdx]->runAction(Sequence::create(DelayTime::create(fDelay+0.2), FadeIn::create(0.2f), nullptr));
			fDelay += 0.1f;
		}
		float s_curX= m_pSpriteBgTable->getPositionX();
		float s_curY= m_pSpriteBgTable->getPositionY();
		auto sz = m_pSpriteBgTable->getContentSize();
		m_pSpriteBgTable->setPositionY(-sz.height / 2);
		m_pSpriteBgTable->runAction(Sequence::create(DelayTime::create(0.1f), EaseBackOut::create(MoveTo::create(0.3f, Vec2(s_curX, s_curY))), nullptr));


		 s_curX= m_pSpriteBgCard->getPositionX();
		 s_curY= m_pSpriteBgCard->getPositionY();

		 m_pSpriteBgCard->setPosition(Point(s_curX-100,s_curY-100)) ;

		m_pSpriteBgCard->runAction(Sequence::create(DelayTime::create(0.8f), MoveTo::create(0.2f, Vec2(s_curX, s_curY)), nullptr));
		m_pBtSwitch[0]->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pBtSwitch[1]->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pBtSwitch[2]->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pImageMenuBg->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pBtBack->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pBtEnterMall->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pBtEnterKeFu->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));
		m_pLabelAllInfo->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(), nullptr));		
		m_pLabelCash->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(),
			CallFunc::create([this]()
		{
			m_pLabelCash->setVisible(true);
		}),nullptr));
		m_pSprite$->runAction(Sequence::create(DelayTime::create(1.5f), Show::create(),
			CallFunc::create([this]()
		{
			//m_pVipSpriteAni->setVisible(true);
		}),nullptr));

		PlayBackGroundSound("sound/bghall1.mp3", true);
        GameTipsManager::getInstance()->clean();
        auto guideTips = [this](){
            createSceneGuid();
            //createVipDefineGuid();
            GameTipsManager::getInstance()->callNextTips();
        };
        this->runAction(Sequence::create(DelayTime::create(1.7), CallFunc::create(guideTips), NULL)) ;
	}
	else
	{
		unsigned uIdxs[] = {0, 1, 2, 3, 5, 4};
		for (int i = 0; i < 6; i++)
		{
			unsigned uIdx = uIdxs[i];
			SetAreaPlayersCount(uIdx, 0);
			setUITextString(m_pLabelMaxCarry[uIdx], "");
			setUITextString(m_pLabelBlindInfo[uIdx], "");
			m_pSpriteBlindInfo[uIdx]->setOpacity(0);
			m_pBtQuickDoors[uIdx]->setPosition(s_ptSrc[uIdx]);
			m_pLabelMaxCarry[uIdx]->setVisible(false);
		}
		//StopBackGroundSound(false);
		setUITextString(m_pLabelAllInfo, "");
		m_pBtSwitch[0]->setVisible(false);
		m_pBtSwitch[1]->setVisible(false);
		m_pBtSwitch[2]->setVisible(false);
		m_pImageMenuBg->setVisible(false);
		m_pBtBack->setVisible(false);
		m_pBtEnterMall->setVisible(false);
		m_pBtEnterKeFu->setVisible(false);
		m_pRadio6Or9->setVisible(false);
		m_pLabelAllInfo->setVisible(false);
		m_pLabelCash->setVisible(false);
		m_pSprite$->setVisible(false);
		m_pVipSpriteAni->setVisible(false);
	}
}

void GameHall2::onKeyEscPressed()
{
	SendClientRequest(ClientRequest::kLeaveHall2);
}

void GameHall2::onBtSwitchTable6or9(cocos2d::Ref *pBt)
{
	unsigned uIdx = m_pRadio6Or9->getSelectedIndex();
	Json::Value param;
	param["type"] = 2;
	if (uIdx == 0)
	{		
		for (int i=0; i<6; i++)
		{
			m_pSpritePlayersFlag[i]->setVisible(false);
			m_pSpriteCamera[i]->setVisible(true);
		}
		param["num"] = 5;
	}
	else if (uIdx == 1)
	{
		for (int i=0; i<6; i++)
		{
			m_pSpritePlayersFlag[i]->setVisible(true);
			m_pSpriteCamera[i]->setVisible(false);
		}
		param["num"] = 6;
	}	
	SendClientRequest(ClientRequest::kGetHallInfo, param);	
}

void GameHall2::onBtQuickEnter(cocos2d::Ref *pBt, cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		TableAreaInfo * pTableAreaInfo = nullptr;
		if (m_uCurIdx == 0)
		{
			pTableAreaInfo = g_globalTableAreaInfo.m_areaBSQJ;
		}
		else if (m_uCurIdx == 1)
		{
			unsigned uPlayerNum = m_pRadio6Or9->getSelectedIndex();
			if (uPlayerNum == 0)
			{
				pTableAreaInfo = g_globalTableAreaInfo.m_areaZCJJ9;
			}
			else
			{
				pTableAreaInfo = g_globalTableAreaInfo.m_areaZCJJ6;
			}
		}

		if (pTableAreaInfo)
		{
			for (int i = 0; i < 6; i++)
			{
				if (pBt == m_pBtQuickDoors[i])
				{			
					Json::Value param;
					param["enterType"] = pTableAreaInfo[i].m_uAreaID;
					SendClientRequest(ClientRequest::kEnterTable, param);
					break;
				}
			}
		}
	}
}

void GameHall2::Activate(unsigned uIdx)
{
	Json::Value param;
	param["type"] = uIdx + 1;
	param["num"] = 6;
	if (uIdx == 0)
	{		
		m_pBtSwitch[0]->setBright(false);
		m_pBtSwitch[1]->setBright(true);
		m_pBtSwitch[2]->setBright(true);
		m_pLabelSwitch[0]->setColor(Color3B(0xff, 0xff, 0xff));
		m_pLabelSwitch[1]->setColor(Color3B(0x37, 0xa4, 0x5c));
		m_pLabelSwitch[2]->setColor(Color3B(0x37, 0xa4, 0x5c));
		for (int i = 0; i < 6; i++)
		{
			m_pBtQuickDoors[i]->setVisible(true);
			m_pSpriteBlindInfo[i]->setVisible(true);
            m_pSpritePlayersFlag[i]->setVisible(false);
            m_pSpriteCamera[i]->setVisible(true);
		}
		m_pRadio6Or9->setVisible(false);
		m_pDeZhouMagnate->setVisible(false);
		m_pDeZhouMagnate->SetLayerVisable(false);
		
		param["num"] = 5; //白手起家也全部改成5人视频场
		SendClientRequest(ClientRequest::kGetHallInfo, param);
	}
	else if (uIdx == 1)
	{
		m_pBtSwitch[0]->setBright(true);
		m_pBtSwitch[1]->setBright(false);
		m_pBtSwitch[2]->setBright(true);
		m_pLabelSwitch[0]->setColor(Color3B(0x37, 0xa4, 0x5c));
		m_pLabelSwitch[1]->setColor(Color3B(0xff, 0xff, 0xff));
		m_pLabelSwitch[2]->setColor(Color3B(0x37, 0xa4, 0x5c));

		for (int i = 0; i < 6; i++)
		{
			m_pBtQuickDoors[i]->setVisible(true);
			m_pSpriteBlindInfo[i]->setVisible(true);
            m_pSpritePlayersFlag[i]->setVisible(false);
            m_pSpriteCamera[i]->setVisible(true);
		}
		m_pRadio6Or9->setSelectedIndex(0);
		m_pRadio6Or9->setVisible(true);
		m_pDeZhouMagnate->setVisible(false);
		m_pDeZhouMagnate->SetLayerVisable(false);
		param["num"] = 5;
		SendClientRequest(ClientRequest::kGetHallInfo, param);
	}	
	else if (uIdx == 2)
	{
		m_pBtSwitch[0]->setBright(true);
		m_pBtSwitch[1]->setBright(true);
		m_pBtSwitch[2]->setBright(false);
		m_pLabelSwitch[0]->setColor(Color3B(0x37, 0xa4, 0x5c));
		m_pLabelSwitch[1]->setColor(Color3B(0x37, 0xa4, 0x5c));
		m_pLabelSwitch[2]->setColor(Color3B(0xff, 0xff, 0xff));
		setUITextString(m_pLabelAllInfo, "");
		for (int i = 0; i < 6; i++)
		{
			m_pBtQuickDoors[i]->setVisible(false);
			m_pSpriteBlindInfo[i]->setVisible(false);
		}
		m_pRadio6Or9->setVisible(false);
		m_pDeZhouMagnate->SetLayerVisable(true);
		m_pDeZhouMagnate->setVisible(true);
		SendClientRequest(ClientRequest::kGetMagnateInfo, param);

		param["type"] = m_pDeZhouMagnate->m_uCurPageNum + 4;
		param["num"] = 0;
		SendClientRequest(ClientRequest::kGetHallInfo, param);
	}
	m_uCurIdx = uIdx;
}

void GameHall2::UpdateDeZhouPage()
{
	m_pDeZhouMagnate->UpdatePageView();
}

void GameHall2::UpdateView()
{
	TableAreaInfo * pTableAreaInfo = nullptr;
	if (m_uCurIdx == 0)
	{
		pTableAreaInfo = g_globalTableAreaInfo.m_areaBSQJ;
	}
	else if (m_uCurIdx == 1)
	{
		unsigned uPlayerNum = m_pRadio6Or9->getSelectedIndex();
		if (uPlayerNum == 0)
		{
			pTableAreaInfo = g_globalTableAreaInfo.m_areaZCJJ9;
		}
		else
		{
			pTableAreaInfo = g_globalTableAreaInfo.m_areaZCJJ6;
		}
	}
	else if(m_uCurIdx == 2)
	{
		m_pDeZhouMagnate->ReceiveDataAndUpdate();
	}
	if (pTableAreaInfo)
	{
		unsigned uAllPlayer = 0;
		for (int i = 0; i < 6; i++)
		{
			TableAreaInfo& info = pTableAreaInfo[i];
			setUITextString(m_pLabelMaxCarry[i], FormatCash4(info.m_dMaxCarry));
			SetAreaPlayersCount(i, info.m_uPlayers);
			uAllPlayer += info.m_uPlayers;
			std::string strLittleBlind = FormatCash4(info.m_dLittleBlind);
			std::string strBigBlind = FormatCash4(info.m_dBigBlind);
			std::string strBlindInfo = "盲注：";
			strBlindInfo += strLittleBlind;
			strBlindInfo += "/";
			strBlindInfo += strBigBlind;
			setUITextString(m_pLabelBlindInfo[i], strBlindInfo);
		}
		char szAllPlayers[64] = {'\0'};
		sprintf(szAllPlayers, "在线玩家数：%d", uAllPlayer);
		setUITextString(m_pLabelAllInfo, szAllPlayers);
	}
}

void GameHall2::SetAreaPlayersCount(unsigned uArea, unsigned uCount)
{
	char szCount[32] = {'\0'};
	sprintf(szCount, "%d", uCount);
	setUITextString(m_pLabelPlayers[uArea], szCount);
	Size szLabel = m_pLabelPlayers[uArea]->getContentSize();

    float fAllW = 0.0;
    //Size szFlag = m_pSpriteCamera[uArea]->getContentSize();
	auto szFlag = Size(24,24) ;
    fAllW = szLabel.width + szFlag.width;
    m_pSpriteCamera[uArea]->setPositionX(105 - fAllW / 2 + szFlag.width / 2);
    m_pSpritePlayersFlag[uArea]->setPositionX(105 - fAllW / 2 + szFlag.width / 2);
    
	m_pLabelPlayers[uArea]->setPositionX(108 + fAllW / 2 - szLabel.width / 2);
}

void GameHall2::OnMyDataChanged(unsigned uDataType)
{
	if (uDataType == WAITCASHCHANGE)
	{
		setUITextString(m_pLabelCash, FormatCash4(g_globalMyData.GetCash()));
	}
}

void GameHall2::refreshHallInfo()
{
	Activate(m_uButtionIndex);
}

void GameHall2::reSetActivateInsex()
{
	if(g_globalMyData.GetCash() < 800000)
	{
		m_uButtionIndex = 0;
	}
	else
	{
		m_uButtionIndex = 1;
	}
}

//初出茅庐 小有成就。。场次引导
void GameHall2::createSceneGuid()
{
    if (GuideInfo->readGuideInfo(NormalSceneGuide))
        return;

    auto pos = m_pBtSwitch[1]->convertToWorldSpace(Vec2(m_pBtSwitch[1]->getContentSize().width/2, m_pBtSwitch[1]->getContentSize().height/2));
    auto guideTips = GuideTips::create("guidetips/hall2btntips.png", "guidetips/hall2btnflag.png");
    guideTips->setBgPosition(Vec2(pos.x+120, pos.y-130));
    guideTips->setFlagPosition(pos);
    auto pClip = CreateClippingMask("guidetips/hall2btnmask.png", pos, guideTips);
    this->addChild(pClip, 10);
    guideTips->setTipsID(NormalSceneGuide);
    GameTipsManager::getInstance()->addTips(pClip);

}
//贵族开房引导
//void GameHall2::createVipDefineGuid()
//{
//    if(GuideInfo->readGuideInfo(VipDefineRoomGuide))
//        return;
//
//    auto guideTips = GuideTips::create("guidetips/viptips.png", "guidetips/vipflag.png");
//    auto pos = m_pVipSpriteAni->convertToWorldSpace(Vec2(0, 0));
//    guideTips->setFlagPosition(Vec2(pos.x-3, pos.y+1));
//    guideTips->setBgPosition(Vec2(pos.x-160, pos.y - 162));
//    auto pClip = CreateClippingMask("guidetips/vipmask.png", Vec2(pos.x-3, pos.y+1), guideTips);
//    this->addChild(pClip, 9);
//    guideTips->setTipsID(VipDefineRoomGuide);
//    GameTipsManager::getInstance()->addTips(pClip);
//}
//视频桌6人桌切换按钮引导
void GameHall2::createSwitchVideoOr6Guid()
{
    if(GuideInfo->readGuideInfo(SwitchvideoOr6Guide))
        return;
    auto pos = m_pRadio6Or9->getPosition();
    auto guideTips = GuideTips::create("guidetips/videoswitchtips.png", "guidetips/videoswitchflag.png");
    guideTips->setBgPosition(Vec2(pos.x-100, pos.y-183));
    guideTips->setFlagPosition(pos);
    auto pClip = CreateClippingMask("guidetips/videoswitchmask.png", m_pRadio6Or9->getPosition(), guideTips);
    this->addChild(pClip);
    guideTips->setTipsID(SwitchvideoOr6Guide);
}