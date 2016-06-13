#ifndef __HITTOUCHGRABLAYER_H__
#define __HITTOUCHGRABLAYER_H__

#include "GrabTouch.h"

class TouchGrabLayer : public cocos2d::Layer
{
public:
	static TouchGrabLayer *create();

	void SetTouchSwallow(bool bSwallow);
	void SetTouchEnable(bool bEnable);
	bool IsTouchSwallow();
	bool IsTouchEnable();

	void SetTouchBeganCB(std::function<bool(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)>);
	void SetTouchMovedCB(std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)>);
	void SetTouchEndedCB(std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)>);
	void SetTouchCancelledCB(std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)>);

protected:
	virtual bool init();
    virtual bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	cocos2d::EventListenerTouchOneByOne *m_pTouchLisenter;
	std::function<bool(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> m_funcTouchBeganCB;
	std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> m_funcTouchMovedCB;
	std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> m_funcTouchEndedCB;
	std::function<void(cocos2d::Node*, cocos2d::Touch *touch, cocos2d::Event *unused_event)> m_funcTouchCancelledCB;
};

class HitTouchGrabLayer : public TouchGrabLayer
{
public:
	static HitTouchGrabLayer *create();

	void SetTouchDiscardCondition(std::function<bool(cocos2d::Node*, cocos2d::Touch *touch)> funcTouchDiscardCondition);

	void SetHitCB(
		std::function<void(cocos2d::Node*)> funCBSelected, 
		std::function<void(cocos2d::Node*)> funCBSelectCancelled,
		std::function<void(cocos2d::Node*, bool)> funCBHitFinished);

protected:
	virtual bool init() override;
	virtual bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override; 
    virtual void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override; 
    virtual void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override; 

	virtual void Select();
	virtual void CancelSelect();
	virtual void FinishHit();

	bool IsSelected() const;

private:
	std::function<bool(cocos2d::Node*, cocos2d::Touch *touch)> m_funcTouchDiscardCondition;
	std::function<void(cocos2d::Node*)> m_funCBSelected;
	std::function<void(cocos2d::Node*)> m_funCBSelectCancelled;
	std::function<void(cocos2d::Node*, bool)> m_funCBHitFinished;
	bool m_bSelected;
};


#endif