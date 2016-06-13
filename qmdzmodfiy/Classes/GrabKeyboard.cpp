#include "GrabKeyboard.h"

USING_NS_CC;

cocos2d::EventListenerKeyboard * QuickGrabKeyboard(cocos2d::Node *pTarget,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*)> onKeyPressed,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*)> onKeyReleased)		
{
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyPressed = onKeyPressed;
	listenerkeyPad->onKeyReleased = onKeyReleased;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkeyPad, pTarget);
	return listenerkeyPad;
}

cocos2d::EventListenerKeyboard *QuickGrabKeyboardBackEscapeAndHide(cocos2d::Node *pTarget)
{
	auto keyreleased = [pTarget](cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* keyevent){
		if (keycode == EventKeyboard::KeyCode::KEY_BACKSPACE || keycode == EventKeyboard::KeyCode::KEY_ESCAPE)  //返回
		{
			pTarget->setVisible(false);
		}
	};
	return QuickGrabKeyboard(pTarget, nullptr, keyreleased);
}

cocos2d::EventListenerKeyboard *QuickGrabKeyboardBackEscape(cocos2d::Node *pTarget, std::function<void(cocos2d::Node*)> cb)
{
	auto keyreleased = [pTarget, cb](cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* keyevent){
		if (keycode == EventKeyboard::KeyCode::KEY_BACKSPACE || keycode == EventKeyboard::KeyCode::KEY_ESCAPE)  //返回
		{
			if (cb)
			{
				cb(pTarget);
			}
		}
	};
	return QuickGrabKeyboard(pTarget, nullptr, keyreleased);
}

bool GlobalKeyGrabber::init(cocos2d::Node *pTarget, cocos2d::EventKeyboard::KeyCode keycode, bool bGrabPressOrRelease)
{
	m_keycodeGrab = keycode;
	m_bGrabPressOrRelease = bGrabPressOrRelease;
	m_pKeyboardListener = QuickGrabKeyboard(pTarget, 
		std::bind(&GlobalKeyGrabber::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2), 
		std::bind(&GlobalKeyGrabber::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2));
	return m_pKeyboardListener != nullptr;
}
void GlobalKeyGrabber::SetEmptyCB(std::function<void()> emptyCB)
{
	m_emptyCB = emptyCB;
}
void GlobalKeyGrabber::Push(cocos2d::Ref *pTarget, std::function<void(cocos2d::Ref *pTarget)> cb)
{
	m_vecTargets.push_back(pTarget);
	m_vecCBs.push_back(cb);
}

void GlobalKeyGrabber::Remove(cocos2d::Ref *pTarget)
{
	for (int i = 0; i < m_vecTargets.size(); ++i)
	{
		if (pTarget == m_vecTargets[i])
		{
			m_vecTargets.erase(m_vecTargets.begin() + i);
			m_vecCBs.erase(m_vecCBs.begin() + i);
			break;
		}
	}
}
void GlobalKeyGrabber::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event*)
{
	char szTxt[64] = {0};
	sprintf(szTxt, "%d键被按下", keycode);
//	SendGameTip(szTxt);
	if (m_bGrabPressOrRelease && keycode == m_keycodeGrab)
	{
		if (m_vecTargets.empty())
		{
			if (m_emptyCB)
			{
				m_emptyCB();
			}
		}
		else
		{
			auto cb = m_vecCBs.back();
			auto target = m_vecTargets.back();

			bool bContinueGrab = false;
			if (cb)
			{
				cb(target);
			}
		}
	}
}

void GlobalKeyGrabber::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event*)
{
	char szTxt[64] = {0};
	sprintf(szTxt, "%d键被松开", keycode);
//	SendGameTip(szTxt);
	if (!m_bGrabPressOrRelease && keycode == m_keycodeGrab)
	{
		if (m_vecTargets.empty())
		{
			if (m_emptyCB)
			{
				m_emptyCB();
			}
		}
		else
		{
			auto cb = m_vecCBs.back();
			auto target = m_vecTargets.back();
			bool bContinueGrab = false;
			if (cb)
			{
				cb(target);
			}
		}
	}
}
