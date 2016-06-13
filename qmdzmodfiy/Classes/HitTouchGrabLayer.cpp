#include "HitTouchGrabLayer.h"

USING_NS_CC;



///////////////////////////////////////////////////////////////////////////////////////////////////
// static 
TouchGrabLayer *TouchGrabLayer::create()
{
	auto pRet = new TouchGrabLayer;
	if (pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void TouchGrabLayer::SetTouchSwallow(bool bSwallow)
{
	m_pTouchLisenter->setSwallowTouches(bSwallow);
}

void TouchGrabLayer::SetTouchEnable(bool bEnable)
{
	m_pTouchLisenter->setEnabled(bEnable);
}

bool TouchGrabLayer::IsTouchSwallow()
{
	return m_pTouchLisenter->isSwallowTouches();
}

bool TouchGrabLayer::IsTouchEnable()
{
	return m_pTouchLisenter->isEnabled();
}

void TouchGrabLayer::SetTouchBeganCB(std::function<bool(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> funcCB)
{
	m_funcTouchBeganCB = funcCB;
}
void TouchGrabLayer::SetTouchMovedCB(std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> funcCB)
{
	m_funcTouchMovedCB = funcCB;
}
void TouchGrabLayer::SetTouchEndedCB(std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> funcCB)
{
	m_funcTouchEndedCB = funcCB;
}
void TouchGrabLayer::SetTouchCancelledCB(std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> funcCB)
{
	m_funcTouchCancelledCB = funcCB;
}

bool TouchGrabLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_pTouchLisenter = EventListenerTouchOneByOne::create();
	m_pTouchLisenter->onTouchBegan = CC_CALLBACK_2(TouchGrabLayer::OnTouchBegan, this);
	m_pTouchLisenter->onTouchMoved = CC_CALLBACK_2(TouchGrabLayer::OnTouchMoved, this);
	m_pTouchLisenter->onTouchEnded = CC_CALLBACK_2(TouchGrabLayer::OnTouchEnded, this);
	m_pTouchLisenter->onTouchCancelled = CC_CALLBACK_2(TouchGrabLayer::OnTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pTouchLisenter, this);	

	return true;
}

bool TouchGrabLayer::OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchBeganCB)
	{
		return m_funcTouchBeganCB(this, touch, unused_event);
	}
	return false;
}

void TouchGrabLayer::OnTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchMovedCB)
	{
		m_funcTouchMovedCB(this, touch, unused_event);
	}
}

void TouchGrabLayer::OnTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchEndedCB)
	{
		m_funcTouchEndedCB(this, touch, unused_event);
	}
}

void TouchGrabLayer::OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchCancelledCB)
	{
		m_funcTouchCancelledCB(this, touch, unused_event);
	}
}


//static 
HitTouchGrabLayer *HitTouchGrabLayer::create()
{
	auto pRet = new HitTouchGrabLayer;
	if (pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void HitTouchGrabLayer::SetTouchDiscardCondition(std::function<bool(cocos2d::Node*, cocos2d::Touch *touch)> funcTouchDiscardCondition)
{
	m_funcTouchDiscardCondition = funcTouchDiscardCondition;
}

void HitTouchGrabLayer::SetHitCB(
		std::function<void(cocos2d::Node*)> funCBSelected, 
		std::function<void(cocos2d::Node*)> funCBSelectCancelled,
		std::function<void(cocos2d::Node*, bool)> funCBHitFinished)
{
	m_funCBSelected = funCBSelected;
	m_funCBSelectCancelled = funCBSelectCancelled;
	m_funCBHitFinished = funCBHitFinished;
}

bool HitTouchGrabLayer::init()
{
	m_bSelected = false;
	return TouchGrabLayer::init();
}

bool HitTouchGrabLayer::OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	bool bHitted = TouchHitNodeV(touch, this);
	if (bHitted)
	{
		Select();
		if (m_funcTouchDiscardCondition)
		{
			m_funcTouchDiscardCondition(this, touch);
		}		
	}
	return bHitted;
}

void HitTouchGrabLayer::OnTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (IsSelected())
	{	
		bool bHitted = TouchHitNodeV(touch, this);
		if (bHitted)
		{
			if (m_funcTouchDiscardCondition && m_funcTouchDiscardCondition(this, touch))
			{
				CancelSelect();
			}
		}
		else
		{
			CancelSelect();
		}
	}

}

void HitTouchGrabLayer::OnTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	FinishHit();
	if (m_bSelected)
	{		
		CancelSelect();
	}
}

void HitTouchGrabLayer::Select()
{
	if (!m_bSelected)
	{
		m_bSelected = true;
		log("HitTouchGrabLayer selected.");
		if (m_funCBSelected)
		{
			m_funCBSelected(this);
		}
	}
}
void HitTouchGrabLayer::CancelSelect()
{
	if (m_bSelected)
	{
		m_bSelected = false;
		log("HitTouchGrabLayer select cancelled.");
		if (m_funCBSelectCancelled)
		{
			m_funCBSelectCancelled(this);
		}
	}
}
void HitTouchGrabLayer::FinishHit()
{
	log("HitTouchGrabLayer hit finished.");
	if (m_funCBHitFinished)
	{
		m_funCBHitFinished(this, m_bSelected);
	}
}
bool HitTouchGrabLayer::IsSelected() const
{
	return m_bSelected;
}