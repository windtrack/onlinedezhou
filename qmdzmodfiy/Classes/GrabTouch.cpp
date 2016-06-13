#include "GrabTouch.h"

USING_NS_CC;

bool IsNodeSeeAble(cocos2d::Node*pNode)
{
    if (pNode->isRunning())
    {
        if (!pNode->isVisible())
        {
            return false;
        }
        for (Node *c = pNode->getParent(); c != nullptr; c = c->getParent())
        {
            if (c->isVisible() == false)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
bool TouchHitNode(cocos2d::Touch *touch, cocos2d::Node *node)
{
	bool bHitted = false;
	Vec2 pos = node->convertTouchToNodeSpace(touch);
	Size sz = node->getContentSize();
	Rect rc(0, 0, sz.width, sz.height);
	if (rc.containsPoint(pos))
	{
		bHitted = true;
	}
	return bHitted;
}

bool TouchHitNodeAnchor(cocos2d::Touch *touch, cocos2d::Node *node)
{
	bool bHitted = false;
	Vec2 pos = node->convertTouchToNodeSpaceAR(touch);
	Size sz = node->getContentSize();
	Rect rc(0, 0, sz.width, sz.height);
	if (rc.containsPoint(pos))
	{
		bHitted = true;
	}
	return bHitted;
}

bool TouchHitNodeV(cocos2d::Touch *touch, cocos2d::Node *node)
{
	bool bHitted = false;
	if (node->isVisible())
	{
		Vec2 pos = node->convertTouchToNodeSpace(touch);
		Size sz = node->getContentSize();
		Rect rc(0, 0, sz.width, sz.height);
		if (rc.containsPoint(pos))
		{
			bHitted = true;
		}
	}
	return bHitted;
}

bool TouchHitNodeAllV(cocos2d::Touch *touch, cocos2d::Node *node)
{
    bool bHitted = false;
    bool bVisible = IsNodeSeeAble(node);
    if (bVisible)
    {
        Vec2 pos = node->convertTouchToNodeSpace(touch);
        Size sz = node->getContentSize();
        Rect rc(0, 0, sz.width, sz.height);
        if (rc.containsPoint(pos))
        {
            bHitted = true;
        }
    }
    return bHitted;
}

EventListenerTouchOneByOne * QuickGrabTouch(cocos2d::Node *pTarget, bool bSwallow,
		std::function<bool(cocos2d::Touch *touch, cocos2d::Event*)> tbcb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Event*)> tmcb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Event*)> tecb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Event*)> tccb)
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = tbcb;
	touchListener->onTouchMoved = tmcb;
	touchListener->onTouchEnded = tecb;
	touchListener->onTouchCancelled = tccb;
	touchListener->setSwallowTouches(bSwallow);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pTarget);
	return touchListener;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// TouchGrabber
TouchGrabber *TouchGrabber::create(cocos2d::Node *pTarget, bool bSwallow, 
								   std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tbcb,
								   std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tmcb,
								   std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tecb,
								   std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tccb)
{
	auto pRet = new TouchGrabber;
	if (pRet->init(pTarget, bSwallow, tbcb, tmcb, tecb, tccb))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
	}
	return pRet;
}

void TouchGrabber::SetTouchEnable(bool bEnable)
{		
	m_pTouchLisenter->setEnabled(bEnable);
}
bool TouchGrabber::IsTouchEnable()
{
	return m_pTouchLisenter->isEnabled();
}

void TouchGrabber::SetTouchSwallow(bool bSwallow)
{
	m_pTouchLisenter->setSwallowTouches(bSwallow);
}
bool TouchGrabber::IsTouchSwallow()
{
	return m_pTouchLisenter->isSwallowTouches();
}

void TouchGrabber::SetTouchBeganCB(std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> funcCB)
{
	m_funcTouchBeganCB = funcCB;
}
void TouchGrabber::SetTouchMovedCB(std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> funcCB)
{
	m_funcTouchMovedCB = funcCB;
}
void TouchGrabber::SetTouchEndedCB(std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> funcCB)
{
	m_funcTouchEndedCB = funcCB;
}
void TouchGrabber::SetTouchCancelledCB(std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> funcCB)
{
	m_funcTouchCancelledCB = funcCB;
}


TouchGrabber::TouchGrabber()
	: m_pTarget(nullptr)
	, m_pTouchLisenter(nullptr)
{
}

TouchGrabber::~TouchGrabber()
{
}

bool TouchGrabber::init(cocos2d::Node *pTarget, bool bSwallow, 
						std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tbcb,
						std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tmcb,
						std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tecb,
						std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tccb)
{
	m_funcTouchBeganCB = tbcb;
	m_funcTouchMovedCB = tmcb;
	m_funcTouchEndedCB = tecb;
	m_funcTouchCancelledCB = tccb;

	m_pTarget = pTarget;
	m_pTouchLisenter = QuickGrabTouch(pTarget, bSwallow, 
		CC_CALLBACK_2(TouchGrabber::OnTouchBegan, this),
		CC_CALLBACK_2(TouchGrabber::OnTouchMoved, this),
		CC_CALLBACK_2(TouchGrabber::OnTouchEnded, this),
		CC_CALLBACK_2(TouchGrabber::OnTouchCancelled, this));

//	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pTouchLisenter, m_pTarget);	
	return true;
}

bool TouchGrabber::OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchBeganCB)
	{
		return m_funcTouchBeganCB(touch, m_pTarget, this);
	}
	return false;
}

void TouchGrabber::OnTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchMovedCB)
	{
		m_funcTouchMovedCB(touch, m_pTarget, this);
	}
}

void TouchGrabber::OnTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchEndedCB)
	{
		m_funcTouchEndedCB(touch, m_pTarget, this);
	}
}

void TouchGrabber::OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_funcTouchCancelledCB)
	{
		m_funcTouchCancelledCB(touch, m_pTarget, this);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//STouchGrabber