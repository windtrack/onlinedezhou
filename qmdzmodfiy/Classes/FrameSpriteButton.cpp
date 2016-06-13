#include "FrameSpriteButton.h"
#include "FrameSprite.h"
#include "GrabTouch.h"

USING_NS_CC;

FrameSpriteButton * FrameSpriteButton::createWithHoriFrameSprite(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove)
{
	auto pRet = new FrameSpriteButton;
	if (pRet && pRet->init(FrameSprite::create(strFrameSprite, 3, 1), bSwallowTouch, bCheckMove))
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

FrameSpriteButton * FrameSpriteButton::createWithVertFrameSprite(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove)
{
	auto pRet = new FrameSpriteButton;
	if (pRet && pRet->init(FrameSprite::create(strFrameSprite, 1, 3), bSwallowTouch, bCheckMove))
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
FrameSpriteButton * FrameSpriteButton::createWithVertFrameSpriteByOne(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove)
{
	auto pRet = new FrameSpriteButton;
	if (pRet && pRet->init(FrameSprite::create(strFrameSprite, 1, 1), bSwallowTouch, bCheckMove))
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
void FrameSpriteButton::SetSelectCB(std::function<void(cocos2d::Ref*, bool)> cbSelected)
{
	m_cbSelected = cbSelected;
}

void FrameSpriteButton::SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted)
{
	m_cbHitted = cbHitted;
}

void FrameSpriteButton::SetEnable(bool bEnable)
{
	bool bEnabled = IsEnable();
	if (bEnabled != bEnable)
	{
		m_pFrameSprite->SetFrameIdx(bEnable ?  0 : 2);
		m_pTouchGrabber->SetTouchEnable(bEnable);
	}
}

bool FrameSpriteButton::IsEnable()
{
	return m_pFrameSprite->GetFrameIdx() != 2;
}

bool FrameSpriteButton::init(FrameSprite *frameSprite, bool bSwallowTouch, bool bCheckMove)
{
	if (!Layer::init())
	{
		return false;
	}

	if (frameSprite)
	{
		m_pFrameSprite = frameSprite;
		auto sz = m_pFrameSprite->getContentSize();
		setContentSize(sz);
		addChild(m_pFrameSprite);

		auto touchGrabber = TouchGrabber::create(m_pFrameSprite, bSwallowTouch,
			[this](cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)->bool // onTouchBegan
			{
				if (isSeeable() && IsEnable())
				{
					bool bHitted = TouchHitNode(touch, pNode);
					if (bHitted)
					{
						setSelected(true);
						return true;
					}
				}
				return false;
			},
			[this, bCheckMove](cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) // onTouchMove
			{
				if (IsEnable() && isSeeable() && isSelected())
				{
					bool bHitted = TouchHitNode(touch, pNode);
					if (bCheckMove)
					{
						setSelected(!IsTouchMoving(touch, 9) && bHitted);
					}
					else
					{
						setSelected(bHitted);
					}
				}							
			},
			[this](cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) // onTouchEnd
			{
				if (IsEnable() && isSeeable())
				{
					if (isSelected() && m_cbHitted)
					{
						m_cbHitted(this);
					}	
					if (isSelected())
					{		
						setSelected(false);
					}
				}							
			}
		);
		touchGrabber->retain();
		m_pTouchGrabber = touchGrabber;
		return true;
	}
	
	return false;
}

void FrameSpriteButton::onEnter()
{
	Layer::onEnter();
}

void FrameSpriteButton::onExit()
{
	m_pTouchGrabber->release();
	Layer::onExit();
}

bool FrameSpriteButton::isSeeable()
{
	if (!isVisible())
	{
		return false;
	}
	for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
	return true;
}

bool FrameSpriteButton::isSelected()
{
	return m_pFrameSprite->GetFrameIdx() == 1;
}

void FrameSpriteButton::setSelected(bool bSelect)
{
	if (IsEnable())
	{
		m_pFrameSprite->SetFrameIdx(bSelect ? 1 : 0);
		if (m_cbSelected)
		{
			m_cbSelected(this, bSelect);
		}
	}
}



LayerButton * LayerButton::create(bool bSwallowTouch, bool bCheckMove)
{
	auto pRet = new LayerButton;
	if (pRet && pRet->init(bSwallowTouch, bCheckMove))
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

void LayerButton::SetSelectCB(std::function<void(cocos2d::Ref*, bool bSel)> cbSelected)
{
	m_cbSelected = cbSelected;
}

void LayerButton::SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted)
{
	m_cbHitted = cbHitted;
}

void LayerButton::SetEnable(bool bEnable)
{
	m_pTouchGrabber->SetTouchEnable(bEnable);
}
bool LayerButton::IsEnable()
{
	return m_pTouchGrabber->IsTouchEnable();
}

bool LayerButton::init(bool bSwallowTouch, bool bCheckMove)
{
	if (!Layer::init())
	{
		return false;
	}

	m_bSelected = false;

	auto touchGrabber = TouchGrabber::create(this, bSwallowTouch,
		[this](cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)->bool // onTouchBegan
		{
			if (isSeeable() && IsEnable())
			{
				bool bHitted = TouchHitNode(touch, pNode);
				if (bHitted)
				{
					setSelected(true);
					return true;
				}
			}
			return false;
		},
		[this, bCheckMove](cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) // onTouchMove
		{
			if (IsEnable() && isSeeable() && isSelected())
			{
				bool bHitted = TouchHitNode(touch, pNode);
				if (bCheckMove)
				{
					setSelected(!IsTouchMoving(touch, 9) && bHitted);
				}
				else
				{
					setSelected(bHitted);
				}
			}							
		},
		[this](cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) // onTouchEnd
		{
			if (IsEnable() && isSeeable())
			{
				if (isSelected() && m_cbHitted)
				{
					m_cbHitted(this);
				}	
				if (isSelected())
				{		
					setSelected(false);
				}
			}							
		}
	);
	touchGrabber->retain();
	m_pTouchGrabber = touchGrabber;
	return true;
}
void LayerButton::onEnter()
{
	Layer::onEnter();
}

void LayerButton::onExit() 
{
	m_pTouchGrabber->release();
	Layer::onExit();
}


bool LayerButton::isSeeable()
{
	if (!isVisible())
	{
		return false;
	}
	for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
	return true;
}
bool LayerButton::isSelected()
{
	return m_bSelected;
}

void LayerButton::setSelected(bool bSelect)
{
	if (IsEnable() && m_bSelected != bSelect)
	{
		m_bSelected = bSelect;
		if (m_cbSelected)
		{
			m_cbSelected(this, bSelect);
		}
	}
}