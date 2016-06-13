#include "ActionBar.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "GameCommon.h"
#include "Tools.h"
#include "GlobalUserData.h"
#include "AnimationManager.h"
#include "GameTable.h"
USING_NS_CC;

ActionBar* ActionBar::creatWithLayer(cocos2d::Node* node)
{
	auto pRet = new(std::nothrow) ActionBar();
	if (pRet &&  pRet->initWithLayer(node))
	{
		pRet->autorelease();
		return pRet ;
	}
	else
	{
		CC_SAFE_DELETE(pRet) ;
		return NULL ;
	}

}

bool ActionBar::initWithLayer(cocos2d::Node* node)
{
	do 
	{
		if (!Layer::init()) {
			return false;
		}
		m_uiroot = node ;
		m_bHasDispatchPublicCard = false;
		m_dLittleBlind = m_dBigBlind = 0;
		for (auto& item : m_dQuickRaise)
		{
			item = 0;
		}
		for (auto& item : m_dRaisePartPot)
		{
			item = 0;
		}

		m_actionbarleft =  static_cast<Layout*>(Tools::getWidgetByName(node,"Panel_actionbar"));
		m_actionbaright =  static_cast<Layout*>(Tools::getWidgetByName(node,"Panel_actionbar2"));


		m_pSpriteBgBar = static_cast<ImageView*>(Tools::getWidgetByName(node,"gt_reaseblock"));
		m_pSpriteBgBar->setVisible(false);

		//m_pSpriteBgBar = Sprite::create("actionbar/bgaction.png");
		//m_pSpriteBgBar->setVisible(false);
		auto sz = m_pSpriteBgBar->getContentSize();
		setContentSize(sz);
		//m_pSpriteBgBar->setPosition(sz.width / 2, sz.height / 2);
		//addChild(m_pSpriteBgBar);
		
		layerSlider =  static_cast<Layout*>(Tools::getWidgetByName(node,"panel_rease"));
		layerSlider->setVisible(false);


		//gt_rease_control
		Layout*	rease_control =  static_cast<Layout*>(Tools::getWidgetByName(node,"gt_rease_control"));

		m_pSpriteBgSlider = Sprite::create("actionbar/bgraise.png");
		m_pSpriteBgSlider->setVisible(false);
		m_pSpriteBgSlider->setPosition(rease_control->getPosition());
		addChild(m_pSpriteBgSlider);

		



		
		//底池
		std::string raiseName[3] = {"gt_bt_dichi0","gt_bt_dichi1","gt_bt_dichi2",} ;
		std::string raiseTextName[3] = {"gt_text_dichi0","gt_text_dichi1","gt_text_dichi2",} ;
		for (int i = 0; i < 3; i++)
		{
			m_pMenuItemRaisePartPot[i] = static_cast<Button*>(Tools::getWidgetByName(node,raiseName[i]));
			m_pMenuItemRaisePartPot[i]->setVisible(false);
			m_pMenuItemRaisePartPot[i]->setBright(false);
			//m_pMenuItemRaisePartPot[i]->setVisible(true);
			m_pMenuItemRaisePartPot[i]->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemQuickRaisePartPort) ) ;
			m_pMenuItemRaisePartPotText[i] = static_cast<Text*>(Tools::getWidgetByName(node,raiseTextName[i]));
		}
		
		m_pMenuItemFold = static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_qipai"));
		m_pMenuItemFold->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemAction) ) ;
		m_pMenuItemFold->setVisible(false);

	


		m_pMenuItemCheck = static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_rangpai"));
		m_pMenuItemCheck->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemAction) ) ;
		m_pMenuItemCheck->setVisible(false);

		
		m_pMenuItemCall = static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_genzhu"));
		m_pMenuItemCall->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemAction) ) ;
		m_pMenuItemCallText = static_cast<Text*>(Tools::getWidgetByName(node,"gt_text_gengzhuv"));

		m_pMenuItemCall->setVisible(false);


		//加注
		m_pMenuItemRaise = static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_jiazhu"));
		m_pMenuItemRaise->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemSelected) ) ;
		m_pMenuItemRaise->setVisible(false);

		m_pMenuItemRaiseText = static_cast<Text*>(Tools::getWidgetByName(node,"gt_text_action_jiazhu"));
		
		m_pMenuItemAllIn = static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_allin"));
		m_pMenuItemAllIn->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemAction) ) ;
		m_pMenuItemAllIn->setVisible(false);

	

		std::string raiseNameadd[5] = {"gt_button_rease0","gt_button_rease1","gt_button_rease2","gt_button_rease3","gt_button_rease4"} ;
		std::string raiseNameaddText[5] = {"gt_text_rease0","gt_text_rease1","gt_text_rease2","gt_text_rease3","gt_text_rease4",} ;
		for (int i = 0; i < 5; i++)
		{
			m_pMenuItemQuickRaise[i] =  static_cast<Button*>(Tools::getWidgetByName(node,raiseNameadd[i]));
			m_pMenuItemQuickRaise[i]->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemQuickRaise) ) ;
			m_pMenuItemQuickRaise[i]->setVisible(false);
			m_pMenuItemQuickRaise[i]->setBright(false);
			m_pMenuItemQuickRaiseText[i] =  static_cast<Text*>(Tools::getWidgetByName(node,raiseNameaddText[i]));
		}

		

		m_pMenuItemSlideRaise =  static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_rease"));
		m_pMenuItemSlideRaise->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemAction) ) ;
		m_pMenuItemSlideRaise->setVisible(false);
		m_pMenuItemSlideRaiseText =  static_cast<Text*>(Tools::getWidgetByName(node,"gt_text_reastlight"));

		//加注allin
		m_pMenuItemSlideRaiseAllIn =  static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_rease_allin"));
		m_pMenuItemSlideRaiseAllIn->addTouchEventListener(this,toucheventselector(ActionBar::onMenuItemAction) ) ;
		m_pMenuItemSlideRaiseAllIn->setVisible(false);




		//Slider_1
		//m_pSliderRaise = static_cast<Slider*>(Tools::getWidgetByName(node,"Slider_1"));
		//m_pSliderRaise->addEventListenerSlider(this,sliderpercentchangedselector())

		Layout*	rease_control_slider =  static_cast<Layout*>(Tools::getWidgetByName(node,"Slider_1"));
		rease_control_slider->setVisible(false);
		m_pSliderRaise = extension::ControlSlider::create("actionbar/sliderraisebg.png", "actionbar/sliderraiseprogress.png", "actionbar/sliderraisethumb.png");
		m_pSliderRaise->setAnchorPoint(Vec2(0.5, 0.5));
		m_pSliderRaise->setPosition(rease_control_slider->getPosition());
		m_pSliderRaise->addTargetWithActionForControlEvents(this, cccontrol_selector(ActionBar::onSliderChanged), extension::Control::EventType::VALUE_CHANGED);
		m_pSliderRaise->setVisible(false);

		//gt_rease_control
		auto	rease_control_sliderf =  Tools::getWidgetByName(node,"gt_rease_bg");
		rease_control_sliderf->addChild(m_pSliderRaise);

		m_pSliderRaise->setMinimumValue(0);
		m_pSliderRaise->setMaximumValue(1);
		m_pSliderRaise->setRotation(-90);
		m_dCurrentTime = 0;
		
		m_pSpriteAllIn = AnimationManager::creatArmature("aniallin");
		//m_pSpriteAllIn = CreateFrameSprite("actionbar/aniallin.png", 1, 2);
		m_pSpriteAllIn->setVisible(false);
		m_pSpriteAllIn->ignoreAnchorPointForPosition(false);
		m_pSpriteAllIn->setAnchorPoint(Point(0.5,0.5));
		m_pSpriteAllIn->setPosition(m_pMenuItemSlideRaiseAllIn->getContentSize().width/2,m_pMenuItemSlideRaiseAllIn->getContentSize().height/2);
		m_pMenuItemSlideRaiseAllIn->addChild(m_pSpriteAllIn);
		m_pSpriteAllIn->playByIndexAndRepatForever(0);


		m_table = nullptr ;
		setVisible(false) ;

		REG_TOUCH_ONEBYONE_SWALLOW_B(ActionBar::onTouchBegan, this);

		return true ;
	} while (0);

	return false ;
}



bool ActionBar::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//REG_TOUCH_ONEBYONE_SWALLOW_B(ActionBar::onTouchBegan, this);

	return true;
}

void ActionBar::SetBlind(double dLittleBlind, double dBigBlind)
{
	m_dLittleBlind = dLittleBlind;
	m_dBigBlind = dBigBlind;

	m_dQuickRaise[0] = 10 * dLittleBlind;
	m_dQuickRaise[1] = 20 * dLittleBlind;
	m_dQuickRaise[2] = 50 * dLittleBlind;
	m_dQuickRaise[3] = 100 * dLittleBlind;
	m_dQuickRaise[4] = 200 * dLittleBlind;
	for (int i = 0; i < 5; i++)
	{		
		//SetLabelString(m_pMenuItemQuickRaise[i]->GetLabel(), FormatCash(m_dQuickRaise[i]));
		setUITextString(m_pMenuItemQuickRaiseText[i],FormatCash(m_dQuickRaise[i])) ;
	}
}

void ActionBar::Update(double dMinCall, bool bNeedRaise, double dMinRaise, double dMaxRaise, double dCash, double dAllPot, bool bHasDispatchedPublicCard)
{	
	//if (dMaxRaise!=dCash) //测试不能显示allin
	//{
	//	int aa =0 ;
	//}

	m_bNeedRaise = bNeedRaise;
	m_dMinCall = dMinCall;
	m_dMinRaise = dMinRaise;
	m_dMaxRaise = dMaxRaise;
	m_dCash = dCash;
	m_dAllPot = dAllPot;

	m_pMenuItemSlideRaise->setVisible(false);
	m_pMenuItemSlideRaiseAllIn->setVisible(false);
	m_pSliderRaise->setVisible(false);
	setVisible(true);

	bool bNeedCall = dMinCall > 1;

	bool bCanCall = dCash >= dMinCall;
	//bCanCall = dCash>dMinRaise ;//#2239
	// 先看前两个按钮 弃牌是必须的
	m_pMenuItemFold->setVisible(true);
	if (bNeedCall) // 需跟
	{
		m_pMenuItemCheck->setVisible(false);
		if (bCanCall) // 能跟
		{
			m_pMenuItemCall->setVisible(true);
			//SetLabelString(m_pMenuItemCall->GetLabel(), std::string("跟 ") + FormatCash(dMinCall)); // 弃-跟-
			setUITextString(m_pMenuItemCallText, std::string("跟 ") + FormatCash(dMinCall)); // 弃-跟-
		}
		else // 跟不起
		{
			m_pMenuItemCall->setVisible(false);
			m_pMenuItemAllIn->setPosition(m_pMenuItemCheck->getPosition()); // 跟都跟不起 弃-All-加(灰)
			m_pMenuItemAllIn->setVisible(true);
		}
	}
	else // 不需要跟 让牌
	{
		m_pMenuItemCheck->setVisible(true);
		m_pMenuItemCall->setVisible(false);
	}

	if (bNeedRaise)
	{
		bool bCanRaise = (dCash >= dMinRaise && bCanCall && ((dMaxRaise - dMinRaise) > 1));	// CAODANDE: 最低加注竟然小于最低跟注	
		
		// 第三个按钮
		m_pMenuItemRaise->setEnabled(bCanRaise);
		m_pMenuItemRaise->setBright(bCanRaise);
		if (bCanRaise) // 能加注
		{		
			m_pMenuItemRaise->setVisible(true);
			for (int i = 0; i < 5; i++)
			{		
				m_pMenuItemQuickRaise[i]->setEnabled(m_dQuickRaise[i] >= dMinRaise && m_dQuickRaise[i] <= dMaxRaise);
				//m_pMenuItemQuickRaise[i]->setTouchEnabled(false);
				m_pMenuItemQuickRaise[i]->setBright(!(m_dQuickRaise[i] >= dMinRaise && m_dQuickRaise[i] <= dMaxRaise));
				if (!(m_dQuickRaise[i] >= dMinRaise && m_dQuickRaise[i] <= dMaxRaise))
				{
						m_pMenuItemQuickRaiseText[i]->setOpacity(255.0f/100.0f*40);
				}
				else
				{
					m_pMenuItemQuickRaiseText[i]->setOpacity(255);
				}
			
			}

			setUITextString(m_pMenuItemSlideRaiseText,FormatCash(dMinRaise));
			m_pMenuItemAllIn->setVisible(false);
			m_sliderStartValue = 50.0f * 0.5 / 364.0f;
			m_sliderEndValue = 1.0 - m_sliderStartValue;

			m_pSliderRaise->setMinimumAllowedValue(m_sliderStartValue);
			m_pSliderRaise->setMaximumAllowedValue(m_sliderEndValue);
			m_pSliderRaise->setValue(m_sliderStartValue);

			//double dRaiseValue = m_pSliderRaise->getValue();
			//double dRaise = m_dMinRaise + (int)((m_dMaxRaise - m_dMinRaise) * (dRaiseValue - m_sliderStartValue) / (m_sliderEndValue - m_sliderStartValue));
			//m_dCurRaise = dRaise;
            m_dCurRaise = dMinRaise;
		}
		else // 不能加注
		{		
			if (bNeedCall)
			{
				if (!bCanCall)
				{
					m_pMenuItemRaise->setVisible(true);
				}
				else
				{
					//m_pMenuItemRaise->setBright(true);
					m_pMenuItemRaise->setVisible(false);
					m_pMenuItemAllIn->setPosition(m_pMenuItemRaise->getPosition());
					m_pMenuItemAllIn->setVisible(true);
				}
			}
			else
			{
				m_pMenuItemRaise->setVisible(false);
				m_pMenuItemAllIn->setPosition(m_pMenuItemRaise->getPosition());
				m_pMenuItemAllIn->setVisible(true);
			}
		}
		
		m_bHasDispatchPublicCard = bHasDispatchedPublicCard;
		if (m_bHasDispatchPublicCard)
		{
			setUITextString(m_pMenuItemRaisePartPotText[0], "1/2X底池");
			setUITextString(m_pMenuItemRaisePartPotText[1], "2/3X底池");
			setUITextString(m_pMenuItemRaisePartPotText[2], "1X底池");
		}
		else
		{
			setUITextString(m_pMenuItemRaisePartPotText[0], "3X大盲");
			setUITextString(m_pMenuItemRaisePartPotText[1], "4X大盲");
			setUITextString(m_pMenuItemRaisePartPotText[2], "1X底池");
		}
		if (m_bHasDispatchPublicCard)
		{
			double exp = 1e-10;
			m_dRaisePartPot[0] = dAllPot / 2;
			m_dRaisePartPot[1] = dAllPot * 2 / 3;
			if(!doubleIsInt(m_dRaisePartPot[0]))
			{
				m_dRaisePartPot[0] = getUpInt(m_dRaisePartPot[0], m_dLittleBlind);
			}
			if(!doubleIsInt(m_dRaisePartPot[1]))
			{
				m_dRaisePartPot[1] = getUpInt(m_dRaisePartPot[1], m_dLittleBlind);
			}
			m_dRaisePartPot[2] = dAllPot;
		}
		else
		{
			m_dRaisePartPot[0] = m_dBigBlind * 3;
			m_dRaisePartPot[1] = m_dBigBlind * 4;
			m_dRaisePartPot[2] = dAllPot;
		}
		for (int i = 0; i < 3; i++)
		{
			m_pMenuItemRaisePartPot[i]->setEnabled(m_dRaisePartPot[i] >= dMinRaise && m_dRaisePartPot[i] <= dMaxRaise);
			m_pMenuItemRaisePartPot[i]->setBright(!(m_dRaisePartPot[i] >= dMinRaise && m_dRaisePartPot[i] <= dMaxRaise));

			if (!(m_dRaisePartPot[i] >= dMinRaise && m_dRaisePartPot[i] <= dMaxRaise))
			{
				m_pMenuItemRaisePartPotText[i]->setOpacity(255.0f/100.0f*40);
			}
			else
			{
				m_pMenuItemRaisePartPotText[i]->setOpacity(255);
			}

		}
	}
	else
	{

		//m_pMenuItemRaise->setVisible(true) ;
		//m_pMenuItemRaise->setEnabled(true);

		//m_pMenuItemRaise->setVisible(true);
		//m_pMenuItemRaise->setEnabled(false);
		//m_pMenuItemRaise->setBright(false);

		if ((bNeedCall && bCanCall) ||
			!bNeedCall)
		{
			m_pMenuItemAllIn->setVisible(false);
		}


		if (bNeedCall) // 需跟
		{

			if (bCanCall) // 能跟
			{
				m_pMenuItemRaise->setVisible(true) ;
				m_pMenuItemRaise->setEnabled(true);
			}
			else // 跟不起
			{
				m_pMenuItemRaise->setVisible(true);
				m_pMenuItemRaise->setEnabled(false);
				m_pMenuItemRaise->setBright(false);
			}
		}



	}
	layerSlider->setVisible(false);
	m_pSpriteBgBar->setVisible(false);
	m_pSpriteBgSlider->setVisible(false);
	m_pSpriteAllIn->stopAllActions();
	m_pSpriteAllIn->setVisible(false);



	for (int i = 0; i < 3; i++)
	{
		m_pMenuItemRaisePartPot[i]->setVisible(true);

	}


	for (int i = 0; i < 5; i++)
	{
		m_pMenuItemQuickRaise[i]->setVisible(false);
	}


	if (!m_pMenuItemRaise->isBright())
	{
		m_pMenuItemRaiseText->setOpacity(255.0f/100*40);
	}
	else
	{
		m_pMenuItemRaiseText->setOpacity(255);
	}
}

bool ActionBar::CanCheckNow()
{
	return m_dMinCall < 1;
}
bool ActionBar::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	//log("ActionBar touch began:(%d,%d)", touch->getLocation().x, touch->getLocation().y);
	bool bHitted = false;
	if (isVisible())
	{
		Vec2 pos = convertTouchToNodeSpace(touch);
		Size sz = getContentSize();
		Rect rc(0, 0, sz.width, sz.height);
		if (rc.containsPoint(pos))
		{
			bHitted = true;
		}

		if (m_pMenuItemSlideRaise->isVisible())
		{
			rc.size = m_pSpriteBgSlider->getContentSize();
			rc.origin = m_pSpriteBgSlider->getPosition() - Vec2(rc.size.width / 2, rc.size.height / 2);
			if (rc.containsPoint(pos))
			{
				bHitted = true;
			}
		}

		if (!bHitted)
		{
			if (m_pMenuItemSlideRaise->isVisible())
			{
				ShowAction(true);
			}
		}
	}

	return false;
}

void ActionBar::onMenuItemSelected(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (pItem == m_pMenuItemRaise)
		{
			ShowAction(false);
		}
	}

}

void ActionBar::onMenuItemQuickRaisePartPort(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		for (int i = 0; i < 3; i++)
		{
			if (pItem == m_pMenuItemRaisePartPot[i])
			{
				Json::Value msg;
				msg["betJettonNum"] = m_dRaisePartPot[i];
				msg["betType"] = 2;
				msg["mangTag"] = 0;
				SendClientRequest(ClientRequest::kAddScore, msg);
				break;
			}
		}
	}

}

void ActionBar::onMenuItemQuickRaise(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		for (int i = 0; i < 5; i++)
		{
			if (pItem == m_pMenuItemQuickRaise[i]&&!(m_pMenuItemQuickRaise[i]->isBright()))
			{
				Json::Value msg;
				msg["betJettonNum"] = m_dQuickRaise[i];
				msg["betType"] = 2;
				msg["mangTag"] = 0;
				SendClientRequest(ClientRequest::kAddScore, msg);
				ShowAction(true);
				break;
			}
		}
	}


}

void ActionBar::onMenuItemAction(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (pItem == m_pMenuItemFold)
		{
			Json::Value param;
			param["strTip"] = 2;
			SendClientRequest(ClientRequest::kGiveUp, param);
	        g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskFold);
		}
		else if (pItem == m_pMenuItemCall)
		{
			Json::Value msg;
			msg["betJettonNum"] = m_dMinCall;
			msg["betType"] = 1;
			msg["mangTag"] = 0;
			SendClientRequest(ClientRequest::kAddScore, msg);
		     g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskCall);
		}
		else if (pItem == m_pMenuItemAllIn || pItem == m_pMenuItemSlideRaiseAllIn)
		{
			Json::Value msg;
			msg["betJettonNum"] = m_dMaxRaise;
			msg["betType"] = 3;
			msg["mangTag"] = 0;
			SendClientRequest(ClientRequest::kAddScore, msg);

			if (pItem == m_pMenuItemSlideRaiseAllIn)
			{
				ShowAction(true);
			}

		}
		else if (pItem == m_pMenuItemSlideRaise)
		{
	

			//
			if (m_dCurRaise == m_dCash)
			{
				Json::Value msg;
				msg["betJettonNum"] = m_dMaxRaise;
				msg["betType"] = 3;
				msg["mangTag"] = 0;
				SendClientRequest(ClientRequest::kAddScore, msg);

				if (pItem == m_pMenuItemSlideRaiseAllIn)
				{
					ShowAction(true);
				}
			}
			else
			{
				Json::Value msg;
				//msg["betJettonNum"] = m_pSliderRaise->getValue();
				msg["betJettonNum"] = m_dCurRaise;
				msg["betType"] = 2;
				msg["mangTag"] = 0;
				SendClientRequest(ClientRequest::kAddScore, msg);
				ShowAction(true);
			}


		}
		else if (pItem == m_pMenuItemCheck)
		{
			Json::Value msg;
			msg["betJettonNum"] = 0;
			msg["betType"] = 4;
			msg["mangTag"] = 0;
			SendClientRequest(ClientRequest::kAddScore, msg);
			//doActionMoveOut();
		}
	}

}

void ActionBar::onSliderChanged(Ref* sender, extension::Control::EventType controlEvent)
{
	if (m_pMenuItemSlideRaise->isVisible())
	{
		double dRaiseValue = m_pSliderRaise->getValue();
		double dRaise = m_dMinRaise + (int)((m_dMaxRaise - m_dMinRaise) * (dRaiseValue - m_sliderStartValue) / (m_sliderEndValue - m_sliderStartValue));
        if (!doubleIsInt(dRaise / m_dBigBlind))
        {
            if (dRaise <= m_dMinRaise)
            {
                dRaise = m_dMinRaise;
            }
            else if (getUpInt(dRaise, m_dBigBlind) >= m_dMaxRaise)
            {
                dRaise = m_dMaxRaise;
            }
            else
            {
                dRaise = getUpInt(dRaise, m_dBigBlind);
            }
        }
        



		bool bAllIn = (dRaise >= m_dCash);
		if (bAllIn)
		{
			if (!m_pSpriteAllIn->isVisible())
			{
				m_pSpriteAllIn->setVisible(true);
				m_pSpriteAllIn->playByIndexAndRepatForever(0);
				//m_pSpriteAllIn->runAction(RepeatForever::create(CreateAnimate("actionbar/aniallin.png", 1, 2, 2, 0.3, false)));
				m_pMenuItemSlideRaiseAllIn->setVisible(true);
				if (m_pSliderRaise->isVisible()) {
					#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
						PlayEffectSound("sound/table/allin.ogg");
					#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
						PlayEffectSound("sound/table/allin.mp3");
					#endif
				}
			}
		}
		else
		{
			m_pSpriteAllIn->stopAllActions();
			m_pSpriteAllIn->setVisible(false);
			m_pMenuItemSlideRaiseAllIn->setVisible(false);
		}

        double dCurrentTime = getCurrentTime();
        if (dCurrentTime - m_dCurrentTime > 20)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            PlayEffectSound("sound/table/addchip.ogg");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			PlayEffectSound("sound/table/addchip.mp3");
#endif
        }
        m_dCurrentTime = dCurrentTime;

        m_dCurRaise = dRaise;
		//SetLabelString(m_pMenuItemSlideRaise->GetLabel(), FormatCash(dRaise));
		setUITextString(m_pMenuItemSlideRaiseText,FormatCash(dRaise));
	}
}

void ActionBar::ShowAction(bool bShowAction)
{

	layerSlider->setVisible(!bShowAction) ;

	m_pMenuItemSlideRaise->setVisible(!bShowAction);
	m_pMenuItemSlideRaiseAllIn->setVisible(false);
	//m_pMenuItemSlideRaiseAllIn->setVisible(!bShowAction);
	m_pSliderRaise->setVisible(!bShowAction);

	//m_pSpriteBgBar->setVisible(!bShowAction);

	//m_pSpriteBgSlider->setVisible(!bShowAction);
	for (int i = 0; i < 3; i++)
	{
		m_pMenuItemRaisePartPot[i]->setVisible(bShowAction);
		m_pMenuItemRaisePartPot[i]->setBright(false);
	}
	if (!bShowAction)
	{
		m_bBtActionVisible[0] = m_pMenuItemFold->isVisible();
		m_bBtActionVisible[1] = m_pMenuItemCheck->isVisible();
		m_bBtActionVisible[2] = m_pMenuItemCall->isVisible();
		m_bBtActionVisible[3] = m_pMenuItemRaise->isVisible();
		m_bBtActionVisible[4] = m_pMenuItemAllIn->isVisible();
		m_pMenuItemFold->setVisible(false);
		m_pMenuItemCheck->setVisible(false);
		m_pMenuItemCall->setVisible(false);
		m_pMenuItemRaise->setVisible(false);
		m_pMenuItemAllIn->setVisible(false);	
		
	}
	else
	{
		m_pMenuItemFold->setVisible(m_bBtActionVisible[0]);
		m_pMenuItemCheck->setVisible(m_bBtActionVisible[1]);
		m_pMenuItemCall->setVisible(m_bBtActionVisible[2]);
		m_pMenuItemRaise->setVisible(m_bBtActionVisible[3]);
		m_pMenuItemAllIn->setVisible(m_bBtActionVisible[4]);

		m_pSliderRaise->setValue(m_dMinRaise);

		m_pSpriteAllIn->stopAllActions();
		m_pSpriteAllIn->setVisible(false);


		m_pSliderRaise->setMinimumAllowedValue(m_sliderStartValue);
		m_pSliderRaise->setMaximumAllowedValue(m_sliderEndValue);
		m_pSliderRaise->setValue(m_sliderStartValue);


		double dRaiseValue = m_pSliderRaise->getValue();
		double dRaise = m_dMinRaise + (int)((m_dMaxRaise - m_dMinRaise) * (dRaiseValue - m_sliderStartValue) / (m_sliderEndValue - m_sliderStartValue));
		m_dCurRaise = dRaise;
		setUITextString(m_pMenuItemSlideRaiseText,FormatCash(dRaise));
	}

	for (int i=0; i<5; i++)
	{
		m_pMenuItemQuickRaise[i]->setVisible(!bShowAction);

	}
}

void ActionBar::setVisible(bool bVisible)
{
	if (isVisible() != bVisible)
	{

		if (bVisible)
		{
				Layer::setVisible(bVisible);
		}


		if (!bVisible)
		{
				setButtonVisible(bVisible) ;
				Layer::setVisible(bVisible);
			//doActionMoveOut();
		}

		if (m_table!=nullptr)
		{
			m_table->callUserControl(bVisible) ;
		}
		
	}

}

void ActionBar::doActionMoveIn()
{

	float s_curx ,s_cury;
	s_curx = m_actionbarleft->getPositionX() ;
	s_cury = m_actionbarleft->getPositionY();

	m_actionbarleft->setPositionY(s_cury-200);
	m_actionbarleft->runAction(MoveTo::create(0.4,Point(s_curx,s_cury))) ;

	s_curx = m_actionbaright->getPositionX() ;
	s_cury = m_actionbaright->getPositionY();

	m_actionbaright->setPositionY(s_cury-200);
	m_actionbaright->runAction(MoveTo::create(0.4,Point(s_curx,s_cury))) ;


}
void ActionBar::doActionMoveOut()
{
	float s_curx ,s_cury;
	s_curx = m_actionbarleft->getPositionX() ;
	s_cury = m_actionbarleft->getPositionY();

	//m_actionbarleft->setPositionY(s_cury-200);
	m_actionbarleft->runAction(MoveTo::create(0.2,Point(s_curx,s_cury-300))) ;

	s_curx = m_actionbaright->getPositionX() ;
	s_cury = m_actionbaright->getPositionY();


	auto funEndCollecting = [this](){this->setVisible(false);setButtonVisible(false) ;}; // 结束后腰设置标志，以便后面的回调比如发公牌

	m_actionbaright->runAction(  Sequence::create(MoveTo::create(0.2,Point(s_curx,s_cury-300)),funEndCollecting,nullptr)) ;

}



void ActionBar::setButtonVisible(bool bVisible)
{
	for (int i=0;i<3;i++)
	{
		m_pMenuItemRaisePartPot[i]->setVisible(bVisible);
		m_pMenuItemRaisePartPot[i]->setBright(false);
	}
	m_pMenuItemFold->setVisible(bVisible);
	m_pMenuItemCheck->setVisible(bVisible);
	m_pMenuItemCall->setVisible(bVisible);
	m_pMenuItemRaise->setVisible(bVisible);
	m_pMenuItemAllIn->setVisible(bVisible);
	
	m_pMenuItemRaise->setBright(true);
	for (int i=0;i<5;i++)
	{
		m_pMenuItemQuickRaise[i]->setVisible(bVisible);
		m_pMenuItemQuickRaise[i]->setBright(false);
	}
	m_pMenuItemSlideRaise->setVisible(bVisible);
	m_pMenuItemSlideRaiseAllIn->setVisible(bVisible);

	layerSlider->setVisible(bVisible);


}




/************************************************************************/
/*
PreActionBar
*/
/************************************************************************/


PreActionBar* PreActionBar::creatWithLayer(cocos2d::Node* node)
{
	PreActionBar* pRet = PreActionBar::create();
	if (pRet &&  pRet->initWithLayer(node))
	{
		pRet->retain() ;

		return pRet ;
	}
	else
	{
		CC_SAFE_DELETE(pRet) ;
		return NULL ;
	}
}

bool PreActionBar::initWithLayer(cocos2d::Node* node)
{
	do 
	{
		m_dMinCall = 0;



		//让或弃
		m_pMenuItemCheckOrFold =  static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_preactionbar_ranghuoqi"));
		m_pMenuItemCheckOrFold->addTouchEventListener(this,toucheventselector(PreActionBar::onMenuItemSelected) ) ;
		//自动让牌
		m_pMenuItemCall =  static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_preactionbar_autorangpai"));
		m_pMenuItemCall->addTouchEventListener(this,toucheventselector(PreActionBar::onMenuItemSelected) ) ;	
		//自动跟住
		m_pMenuItemCallAny =  static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_preactionbar_autogengzhu"));
		m_pMenuItemCallAny->addTouchEventListener(this,toucheventselector(PreActionBar::onMenuItemSelected) ) ;


		m_pMenuItemCheckOrFoldText =  static_cast<Text*>(Tools::getWidgetByName(node,"gt_text_preactionbar_ranghuoqi"));
		m_pMenuItemCallText =  static_cast<Text*>(Tools::getWidgetByName(node,"gt_text_preactionbar_rautorangpai"));
		m_pMenuItemCallAnyText =  static_cast<Text*>(Tools::getWidgetByName(node,"gt_text_preactionbar_aotugengzhu"));



		setVisible(false) ;
		return true;

	} while (0);
	return false ;
}

//
bool PreActionBar::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_dMinCall = 0;

	
	return true;
}

int PreActionBar::GetCurSelectedAction()
{
	if (m_pMenuItemCheckOrFold->isBright())
	{
		return 0;
	}
	else if (m_pMenuItemCall->isBright())
	{
		return 1;
	}
	else if (m_pMenuItemCallAny->isBright())
	{
		return 2;
	}
	//if (m_pMenuItemCheckOrFold->getSelectedIndex() == 1)
	//{
	//	return 0;
	//}
	//else if (m_pMenuItemCall->getSelectedIndex() == 1)
	//{
	//	return 1;
	//}
	//else if (m_pMenuItemCallAny->getSelectedIndex() == 1)
	//{
	//	return 2;
	//}
	return -1;
}

void PreActionBar::Update(double dMinCall)
{
	if (fabs(m_dMinCall - dMinCall) >= 1)
	{
		if (dMinCall < 1)
		{
			//SetLabelString(m_pMenuItemCallSelected->GetLabel(), "自动让牌");
			//SetLabelString(m_pMenuItemCallUnSelected->GetLabel(), "自动让牌");
			setUITextString(m_pMenuItemCallText,"自动让牌");
		}
		else
		{
			std::string str("跟 ");
			str += FormatCash(dMinCall);
	/*		SetLabelString(m_pMenuItemCallSelected->GetLabel(), str);
			SetLabelString(m_pMenuItemCallUnSelected->GetLabel(), str);*/
			setUITextString(m_pMenuItemCallText,str);
		}
		//m_pMenuItemCall->setSelectedIndex(0);
		m_pMenuItemCall->setBright(false) ;

		m_dMinCall = dMinCall;
	}
}

void PreActionBar::onMenuItemSelected(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		Button* button = (Button*)pItem;
		if(button->isBright())
		{
			reSetPreActionBar();
		}
		else
		{
			reSetPreActionBar();
			button->setBright(true);
		}



		//if (pItem == m_pMenuItemCheckOrFold && m_pMenuItemCheckOrFold->getSelectedIndex() == 1)
		//{
		//	m_pMenuItemCall->setSelectedIndex(0);
		//	m_pMenuItemCallAny->setSelectedIndex(0);
		//}
		//else if (pItem == m_pMenuItemCall && m_pMenuItemCall->getSelectedIndex() == 1)
		//{
		//	m_pMenuItemCheckOrFold->setSelectedIndex(0);
		//	m_pMenuItemCallAny->setSelectedIndex(0);
		//}
		//else if (pItem == m_pMenuItemCallAny && m_pMenuItemCallAny->getSelectedIndex() == 1)
		//{
		//	m_pMenuItemCall->setSelectedIndex(0);
		//	m_pMenuItemCheckOrFold->setSelectedIndex(0);
		//}
	}

}

void PreActionBar::reSetPreActionBar()
{
	if (m_pMenuItemCall->isBright())
	{
			m_pMenuItemCall->setBright(false) ;
	}
	if (m_pMenuItemCheckOrFold->isBright())
	{
			m_pMenuItemCheckOrFold->setBright(false) ;
	}
	if (m_pMenuItemCallAny->isBright())
	{
			m_pMenuItemCallAny->setBright(false) ;
	}


	
}

void PreActionBar::setVisible(bool bVisible)
{
	if (isVisible() != bVisible)
	{
		Layer::setVisible(bVisible);
		setButtonVisable(bVisible) ;
		if (!bVisible)
		{
	/*		m_pMenuItemCall->setSelectedIndex(0);
			m_pMenuItemCheckOrFold->setSelectedIndex(0);
			m_pMenuItemCallAny->setSelectedIndex(0);*/
			reSetPreActionBar() ;
		}

	}
}

void PreActionBar::setButtonVisable(bool bVisible)
{
	m_pMenuItemCall->setVisible(bVisible) ;
	m_pMenuItemCheckOrFold->setVisible(bVisible) ;
	m_pMenuItemCallAny->setVisible(bVisible) ;
}








/************************************************************************/
/*     

BrightActionBar
*/
/************************************************************************/


BrightActionBar* BrightActionBar::creatWithLayer(cocos2d::Node* node)
{
	BrightActionBar* pRet = BrightActionBar::create();
	if (pRet &&  pRet->initWithLayer(node))
	{
		pRet->retain() ;

		return pRet ;
	}
	else
	{
		CC_SAFE_DELETE(pRet) ;
		return NULL ;
	}

}

bool BrightActionBar::initWithLayer(cocos2d::Node* node)
{
	do 
	{

		m_dMinCall = 0;
	/*	m_pMenuItemCallBrightCardSelected = MenuItemSpriteWithLabel::create("actionbar/btpreactionsel.png", 3, CC_CALLBACK_1(BrightActionBar::onMenuItemSelected, this));
		m_pMenuItemCallBrightCardUnSelected = MenuItemSpriteWithLabel::create("actionbar/btpreactionunsel.png", 3, CC_CALLBACK_1(BrightActionBar::onMenuItemSelected, this));

		auto pLabel = CreateLabelMSYHBD(22);
		m_pMenuItemCallBrightCardUnSelected->SetEnabledColor(Color4B(0xff, 0xff, 0xff, 0xff));
		m_pMenuItemCallBrightCardUnSelected->SetLabel(pLabel);
		pLabel->setPositionX(108);
		SetLabelString(pLabel, "结束时亮牌");

		pLabel = CreateLabelMSYHBD(22);
		m_pMenuItemCallBrightCardSelected->SetEnabledColor(Color4B(0xff, 0xff, 0xff, 0xff));
		m_pMenuItemCallBrightCardSelected->SetLabel(pLabel);
		pLabel->setPositionX(108);
		SetLabelString(pLabel, "结束时亮牌");*/

		//m_pMenuItemCallAutoBright = MenuItemToggle::createWithCallback(CC_CALLBACK_1(BrightActionBar::onMenuItemSelected, this), m_pMenuItemCallBrightCardUnSelected, m_pMenuItemCallBrightCardSelected, nullptr);
		//m_pMenuItemCallAutoBright->setPosition(863, 39);

		//auto pMenu = Menu::create(m_pMenuItemCallAutoBright, nullptr);
		//pMenu->setPosition(Vec2::ZERO);
		//addChild(pMenu);
		m_pMenuItemCallAutoBright  =  static_cast<Button*>(Tools::getWidgetByName(node,"gt_bt_BrightActionBar_zidongliangpai"));
		m_pMenuItemCallAutoBright->addTouchEventListener(this,toucheventselector(BrightActionBar::onMenuItemSelected) ) ;
		m_pMenuItemCallAutoBright->setVisible(true);


		setVisible(false) ;
		return true ;
	} while (0);

	return false ;
}



bool BrightActionBar::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//m_dMinCall = 0;
	//m_pMenuItemCallBrightCardSelected = MenuItemSpriteWithLabel::create("actionbar/btpreactionsel.png", 3, CC_CALLBACK_1(BrightActionBar::onMenuItemSelected, this));
	//m_pMenuItemCallBrightCardUnSelected = MenuItemSpriteWithLabel::create("actionbar/btpreactionunsel.png", 3, CC_CALLBACK_1(BrightActionBar::onMenuItemSelected, this));

	//auto pLabel = CreateLabelMSYHBD(22);
	//m_pMenuItemCallBrightCardUnSelected->SetEnabledColor(Color4B(0xff, 0xff, 0xff, 0xff));
	//m_pMenuItemCallBrightCardUnSelected->SetLabel(pLabel);
	//pLabel->setPositionX(108);
	//SetLabelString(pLabel, "结束时亮牌");

	//pLabel = CreateLabelMSYHBD(22);
	//m_pMenuItemCallBrightCardSelected->SetEnabledColor(Color4B(0xff, 0xff, 0xff, 0xff));
	//m_pMenuItemCallBrightCardSelected->SetLabel(pLabel);
	//pLabel->setPositionX(108);
	//SetLabelString(pLabel, "结束时亮牌");

	//m_pMenuItemCallAutoBright = MenuItemToggle::createWithCallback(CC_CALLBACK_1(BrightActionBar::onMenuItemSelected, this), m_pMenuItemCallBrightCardUnSelected, m_pMenuItemCallBrightCardSelected, nullptr);
	//m_pMenuItemCallAutoBright->setPosition(863, 39);

	//auto pMenu = Menu::create(m_pMenuItemCallAutoBright, nullptr);
	//pMenu->setPosition(Vec2::ZERO);
	//addChild(pMenu);
	return true;
}


int BrightActionBar::GetCurSelectedAction()
{
	if (m_pMenuItemCallAutoBright->isBright())
	{
		return 1;
	}
	else
	{
		return 0 ;
	}
	//return m_pMenuItemCallAutoBright->getSelectedIndex();
}

void BrightActionBar::setVisible(bool bVisible)
{
	if (isVisible() != bVisible)
	{
		Layer::setVisible(bVisible);
		setButtonVisable(bVisible) ;
		if (!bVisible)
		{
			//m_pMenuItemCallAutoBright->setSelectedIndex(0);
			m_pMenuItemCallAutoBright->setBright(false);
		}
		if (!bVisible)
		{
				setButtonVisable(bVisible) ;
		}

	}
}


void BrightActionBar::setButtonVisable(bool bVisible)
{
	m_pMenuItemCallAutoBright->setVisible(bVisible);
}
void BrightActionBar::onMenuItemSelected(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if(pItem == m_pMenuItemCallAutoBright/* && m_pMenuItemCallAutoBright->getSelectedIndex() == 1*/)
		{
			m_pMenuItemCallAutoBright->setBright(!m_pMenuItemCallAutoBright->isBright());
		}
	}

}
