#include "FrameScale9SpriteButton.h"

#include "FrameSprite.h"
#include "FrameScale9Sprite.h"
#include "GrabTouch.h"
#include "MenuButtonFilePath.h"
USING_NS_CC;


FrameScale9SpriteButton *FrameScale9SpriteButton::createWithScale9Sprite(int index, Size contentSize, bool bSwallowTouch, bool bCheckMove)
{
	auto pRet = new FrameScale9SpriteButton();
	if (pRet->init(FrameScale9Sprite::create(index,contentSize),index,contentSize,bSwallowTouch,bCheckMove))
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

bool FrameScale9SpriteButton::init(FrameScale9Sprite* frameSprite ,int index, Size contentSize, bool bSwallowTouch, bool bCheckMove)
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
		m_pFrameSprite->setPosition(-sz.width/2,-sz.height/2) ;
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


void FrameScale9SpriteButton::SetSelectCB(std::function<void(cocos2d::Ref*, bool)> cbSelected)
{
	m_cbSelected = cbSelected;
}

void FrameScale9SpriteButton::SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted)
{
	m_cbHitted = cbHitted;
}

void FrameScale9SpriteButton::SetEnable(bool bEnable)
{
	bool bEnabled = IsEnable();
	if (bEnabled != bEnable)
	{
		m_pFrameSprite->SetFrameIdx(bEnable ?  0 : 2);
		m_pTouchGrabber->SetTouchEnable(bEnable);
	}
}

bool FrameScale9SpriteButton::IsEnable()
{
	return m_pFrameSprite->GetFrameIdx() != 2;
}

void FrameScale9SpriteButton::onEnter()
{
	Layer::onEnter();
}

void FrameScale9SpriteButton::onExit()
{
	m_pTouchGrabber->release();
	Layer::onExit();
}

bool FrameScale9SpriteButton::isSeeable()
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

bool FrameScale9SpriteButton::isSelected()
{
	return m_pFrameSprite->GetFrameIdx() == 1;
}

void FrameScale9SpriteButton::setSelected(bool bSelect)
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



