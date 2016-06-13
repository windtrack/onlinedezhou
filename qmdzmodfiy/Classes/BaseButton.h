#ifndef __BASEBUTTON_H__
#define __BASEBUTTON_H__

#include "cocos2d.h"
#include "GrabTouch.h"

enum BtViewIdx
{
	BtViewNormal,
	BtViewSelect,
	BtViewDisable
};

template <typename ButtonBackGround>
class BaseButton : public cocos2d::Layer
{
public:
	static BaseButton *create(ButtonBackGround*pBg)
	{
		auto ret = new BaseButton<ButtonBackGround>(pBg);
		if (ret->init())
		{
			ret->autorelease();
		}
		else
		{
			delete ret;
			ret = nullptr;
		}
		return ret;
	}

	void SetTouchFlag(bool bSwallow, bool bCheckMove)
	{
		m_bSwallow = bSwallow;
		m_bCheckMove = bCheckMove;
	}

	void SetSelectCB(std::function<void(cocos2d::Ref*)> cbSelected)
	{
		m_cbSelected = cbSelected;
	}
	void SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted)
	{
		m_cbHitted = cbHitted;
	}

	void SetEnable(bool bEnable)
	{
		if (IsEnable() != bEnable)
		{
			m_pButtonBackGround->Activate(bEnable ? BtViewNormal : BtViewDisable);
		}
	}
	bool IsEnable()
	{
		return m_pButtonBackGround->Activated() != BtViewDisable;
	}

protected:
	BaseButton(ButtonBackGround *pButtonBackGround)
		: m_pButtonBackGround(pButtonBackGround)
		, m_bSwallow(false)
		, m_bCheckMove(false)
	{
	}
	~BaseButton()
	{
		delete m_pButtonBackGround;
		m_pButtonBackGround = nullptr;
	}

	bool init()
	{
		if (!cocos2d::Layer::init())
		{
			return false;
		}
		cocos2d::Node *pNode = m_pButtonBackGround->GetNode();
		addChild(pNode);
		auto sz = pNode->getContentSize();
		setContentSize(sz);
		return true;
	}
	bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		if (isSeeable())
		{
			if (IsEnable())
			{
				bool bHitted = TouchHitNode(touch, m_pButtonBackGround->GetNode());
				if (bHitted)
				{
					setSelected(true);
					return true;
				}
			}
		}
		return false;
	}

	void OnTouchMove(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		if (isSeeable())
		{
			if (IsEnable() && isSelected())
			{
				bool bHitted = TouchHitNode(touch, m_pButtonBackGround->GetNode());
				if (m_bCheckMove)
				{
					setSelected(!IsTouchMoving(touch, 9) && bHitted);
				}
				else
				{
					setSelected(bHitted);
				}
			}	
		}
	}

	void OnTouchEnd(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		if (isSeeable())
		{
			if (IsEnable())
			{
				if (isSelected())
				{		
					setSelected(false);
					m_pButtonBackGround->HitFinished();
					if (m_cbHitted)
					{
						m_cbHitted(this);
					}
				}
			}
		}
	}

	virtual void onEnter() override
	{
		Layer::onEnter();
		auto touchGrabber = TouchGrabber::create(this, m_bSwallow, 
			std::bind(&BaseButton::OnTouchBegan, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&BaseButton::OnTouchMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&BaseButton::OnTouchEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		touchGrabber->retain();
		m_pTouchGrabber = touchGrabber;
	}
	virtual void onExit() override
	{
		m_pTouchGrabber->release();
		m_pTouchGrabber = nullptr;
		Layer::onExit();
	}

	bool isSeeable()
	{
		if (isRunning())
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
		return false;
	}

	bool isSelected()
	{
		return m_pButtonBackGround->Activated() == BtViewSelect;
	}

	void setSelected(bool bSelect)
	{
		if (IsEnable())
		{
			if (isSelected() != bSelect)
			{
				m_pButtonBackGround->Activate(bSelect ? BtViewSelect : BtViewNormal);
				if (bSelect && m_cbSelected)
				{
					m_cbSelected(this);
				}
			}
		}
	}

private:
	bool m_bSwallow, m_bCheckMove;
	ButtonBackGround *m_pButtonBackGround;
	TouchGrabber *m_pTouchGrabber;
	std::function<void(cocos2d::Ref*)> m_cbSelected;
	std::function<void(cocos2d::Ref*)> m_cbHitted;
};

////////////////////////////////////////////////////////////////////////////////////////////
class LableBackGround
{
public:
	LableBackGround(const std::string& str, const std::string& strFont, int iFontSize);
	~LableBackGround();

	cocos2d::Node *GetNode();
	void Activate(BtViewIdx btViewIdx);
	BtViewIdx Activated();
	void HitFinished();

	void SetStatusColor(cocos2d::Color4B clr[3]);
	void SetStatuColor(BtViewIdx btViewIdx, const cocos2d::Color4B& clr);
protected:
	BtViewIdx m_btViewIdx;
	cocos2d::Label *m_pLabel;
	cocos2d::Color4B m_clr[3];
};

typedef BaseButton<LableBackGround> LabelButton;
inline LabelButton *createButton(const std::string& str, const std::string& strFont, int iFontSize)
{
	auto bg = new LableBackGround(str, strFont, iFontSize);
	return LabelButton::create(bg);
}

////////////////////////////////////////////////////////////////////////////////////////////
class FrameSprite;
class FrameSpriteBackGround
{
public:
	FrameSpriteBackGround(const std::string& strFrameSprite, bool bVert);
	virtual ~FrameSpriteBackGround();

	cocos2d::Node *GetNode();
	virtual void Activate(BtViewIdx btViewIdx);
	BtViewIdx Activated();
	virtual void HitFinished();

protected:
	BtViewIdx m_btViewIdx;
	FrameSprite *m_pFrameSprite;
};

typedef BaseButton<FrameSpriteBackGround> FrameSpriteBGButton;
inline FrameSpriteBGButton *createButton(const std::string& strFrameSprite, bool bVert)
{
	auto bg = new FrameSpriteBackGround(strFrameSprite, bVert);
	return FrameSpriteBGButton::create(bg);
}

///////////////////////////////////////////////////////////////////////////////////////////
class AutoDelayEnabledFrameSpriteBackGround : public FrameSpriteBackGround
{
public:
	AutoDelayEnabledFrameSpriteBackGround(const std::string& strFrameSprite, bool bVert, float fDelaySec, const cocos2d::Color4B& clrLeft, const cocos2d::Color4B& clrRight);
	~AutoDelayEnabledFrameSpriteBackGround();
	virtual void Activate(BtViewIdx btViewIdx) override;
	virtual void HitFinished() override;

protected:
	void onScheduleDelayEnable(float fDelta);

protected:
	float m_fDelaySec;
	clock_t m_clockBeg, m_clockAll;
	cocos2d::Color4F m_clrLeft, m_clrRight;
	cocos2d::DrawNode *m_pDrawNode;
};

typedef BaseButton<AutoDelayEnabledFrameSpriteBackGround> AutoDelayEnabledFrameSpriteButton;
inline AutoDelayEnabledFrameSpriteButton *createButton(const std::string& strFrameSprite, bool bVert, float fDelaySec, const cocos2d::Color4B& clrLeft, const cocos2d::Color4B& clrRight)
{
	auto bg = new AutoDelayEnabledFrameSpriteBackGround(strFrameSprite, bVert, fDelaySec, clrLeft, clrRight);
	return AutoDelayEnabledFrameSpriteButton::create(bg);
}

#endif