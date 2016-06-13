#ifndef __GRABKEYBOARD_H__
#define __GRABKEYBOARD_H__

#include "cocos2d.h"
#include <functional>
#include <vector>

cocos2d::EventListenerKeyboard * QuickGrabKeyboard(cocos2d::Node *pTarget,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*)> onKeyPressed,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*)> onKeyReleased);

cocos2d::EventListenerKeyboard *QuickGrabKeyboardBackEscapeAndHide(cocos2d::Node *pTarget);
cocos2d::EventListenerKeyboard *QuickGrabKeyboardBackEscape(cocos2d::Node *pTarget, std::function<void(cocos2d::Node*)>);

class GlobalKeyGrabber : public cocos2d::Layer
{
public:
	bool init(cocos2d::Node *pTarget, cocos2d::EventKeyboard::KeyCode keycode, bool bGrabPressOrRelease);
	
	void SetEmptyCB(std::function<void()>);
	void Push(cocos2d::Ref *pTarget, std::function<void(cocos2d::Ref *pTarget)>);
	void Remove(cocos2d::Ref *pTarget);

protected:
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);

private:
	cocos2d::EventKeyboard::KeyCode m_keycodeGrab;
	bool m_bGrabPressOrRelease;
	std::vector<cocos2d::Ref *> m_vecTargets;
	std::vector<std::function<void(cocos2d::Ref *pTarget)>> m_vecCBs;
	cocos2d::EventListenerKeyboard *m_pKeyboardListener;
	std::function<void()> m_emptyCB;
};



/*
class KeyboardGrabber : public cocos2d::Ref
{
public:
	static KeyboardGrabber *create(cocos2d::Node *pTarget);

	void SetEnable(bool bEnable);
	bool IsEnable();

	void SetCB(
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*, KeyboardGrabber*)> kpcb,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*, KeyboardGrabber*)> krcb);

protected:
	KeyboardGrabber();
	virtual ~KeyboardGrabber();
	bool init(cocos2d::Node *pTarget,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*, KeyboardGrabber*)> kpcb,
		std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*, KeyboardGrabber*)> krcb);

    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*); 
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*); 

protected:
	cocos2d::Node *m_pTarget;
	cocos2d::EventListenerKeyboard *m_pTouchLisenter;
	std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*, KeyboardGrabber*)> m_kpcb;
	std::function<void(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*, KeyboardGrabber*)> m_krcb;
};
*/
#endif