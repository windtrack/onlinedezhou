#include "GameTipLayer.h"
#include "GrabTouch.h"
#include "Util.h"
#include "GrabKeyboard.h"
#include "GameCommon.h"

USING_NS_CC;

void GameTipLayer::SetBtCB(std::function<void(unsigned)> cbBtHit)
{
	m_cbBtHit = cbBtHit;
}
cocos2d::Label *GameTipLayer::GetContentLabel()
{
	return m_pLabelContent;
}
cocos2d::Label *GameTipLayer::GetButtonLabel(unsigned uIdx)
{
	return m_vecLabelBt[uIdx];
}

void GameTipLayer::setVisible(bool bVisible)
{
	bool isVisible = this->isVisible();
	if (isVisible != bVisible)
	{
		bool bRunning = isRunning();
		if (bRunning)
		{
			if (m_pTouchGrabber)
			{
				m_pTouchGrabber->SetTouchEnable(bVisible);
			}
			if (bVisible)
			{
				RegistEscPressCb(this, std::bind(&GameTipLayer::onKeyEscPressed, this));
			}
			else
			{
				UnRegistEscPressCb(this);
			}			
		}
		cocos2d::LayerColor::setVisible(bVisible);
	}
}


void GameTipLayer::onMenuItemSelect(cocos2d::Ref*pMenuItem)
{
	setVisible(false);
	for (unsigned i = 0; i < m_vecBt.size(); i++)
	{
		if (pMenuItem == m_vecBt[i])
		{
			if (m_cbBtHit)
			{
				m_cbBtHit(i);
			}
			break;
		}
	}
}

void GameTipLayer::onEnter()
{
	cocos2d::LayerColor::onEnter();
	bool bVisible = isVisible();
	if (bVisible)
	{
		RegistEscPressCb(this, std::bind(&GameTipLayer::onKeyEscPressed, this));
	}
	if (m_pTouchGrabber == nullptr)
	{
		m_pTouchGrabber = CreateTouchGrabberWithStaticProcer<STouchEmptyProcer<true>>(this, true);// touchGrabber;	
		m_pTouchGrabber->retain();
	}
	m_pTouchGrabber->SetTouchEnable(bVisible);
}

void GameTipLayer::onExit()
{
	if (m_pTouchGrabber)
	{
		m_pTouchGrabber->SetTouchEnable(false);
		m_pTouchGrabber->release();
		m_pTouchGrabber = nullptr;
	}
	UnRegistEscPressCb(this);
	cocos2d::LayerColor::onExit();
}

void GameTipLayer::onKeyEscPressed()
{
}