#ifndef __GRABTOUCH_H__
#define __GRABTOUCH_H__

#include "cocos2d.h"

bool IsNodeSeeAble(cocos2d::Node*);

bool TouchHitNode(cocos2d::Touch *touch, cocos2d::Node *node);
bool TouchHitNodeAnchor(cocos2d::Touch *touch, cocos2d::Node *node);
bool TouchHitNodeV(cocos2d::Touch *touch, cocos2d::Node *node); // 判断当前node是否visible
bool TouchHitNodeAllV(cocos2d::Touch *touch, cocos2d::Node *node); // 判断所有父节点，如果有一个false，择返回false
cocos2d::EventListenerTouchOneByOne * QuickGrabTouch(cocos2d::Node *pTarget, bool bSwallow,
		std::function<bool(cocos2d::Touch *touch, cocos2d::Event*)> tbcb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Event*)> tmcb = nullptr,
		std::function<void(cocos2d::Touch *touch, cocos2d::Event*)> tecb = nullptr,
		std::function<void(cocos2d::Touch *touch, cocos2d::Event*)> tccb = nullptr);

class TouchGrabber : public cocos2d::Ref
{
public:
	static TouchGrabber *create(cocos2d::Node *pTarget, bool bSwallow, 
		std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tbcb = nullptr,
		std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tmcb = nullptr,
		std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tecb = nullptr,
		std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tccb = nullptr);

	void SetTouchEnable(bool bEnable);
	bool IsTouchEnable();
	
	void SetTouchSwallow(bool bSwallow);
	bool IsTouchSwallow();

	void SetTouchBeganCB(std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> = nullptr);
	void SetTouchMovedCB(std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> = nullptr);
	void SetTouchEndedCB(std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> = nullptr);
	void SetTouchCancelledCB(std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> = nullptr);

protected:
	TouchGrabber();
	virtual ~TouchGrabber();
	bool init(cocos2d::Node *pTarget, bool bSwallow, 
		std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tbcb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tmcb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tecb,
		std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> tccb);

	virtual bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
    virtual void OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

protected:
	cocos2d::Node *m_pTarget;
	cocos2d::EventListenerTouchOneByOne *m_pTouchLisenter;
	std::function<bool(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> m_funcTouchBeganCB;
	std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> m_funcTouchMovedCB;
	std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> m_funcTouchEndedCB;
	std::function<void(cocos2d::Touch *touch, cocos2d::Node*, TouchGrabber *touchGrabber)> m_funcTouchCancelledCB;
};

template <bool IsAlwayReturnsHit>
struct STouchEmptyProcer
{
	static bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){return IsAlwayReturnsHit;}
	static void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	static void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	static void OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
};

template <bool IsAlwayReturnsHit>
struct STouchHitProcer
{
	static bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		bool bHit = TouchHitNodeAllV(touch, pNode);
		if (IsAlwayReturnsHit)
		{
			bHit =  true;
		}
		return bHit;
	}
	static void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	static void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	static void OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
};

template <bool IsAlwayReturnsHit>
struct STouchDialogProcer
{
	static bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		bool bHit = TouchHitNodeV(touch, pNode);
		if (!bHit)
		{
			pNode->setVisible(false);
			touchGrabber->SetTouchEnable(false);
		}
		
		if (IsAlwayReturnsHit)
		{
			bHit =  true;
		}
		return bHit;
	}
	static void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	static void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	static void OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
};

template <typename StaticTouchProcer>
TouchGrabber *CreateTouchGrabberWithStaticProcer(cocos2d::Node *pTarget, bool bSwallow)
{
	return TouchGrabber::create(pTarget, bSwallow,
		&StaticTouchProcer::OnTouchBegan, 
		&StaticTouchProcer::OnTouchMoved, 
		&StaticTouchProcer::OnTouchEnded, 
		&StaticTouchProcer::OnTouchCancelled);
}

struct DTouchEmptyProcer
{
	DTouchEmptyProcer()
		: m_bAlwaysRetHit(false)
	{}
	virtual ~DTouchEmptyProcer(){}
	void SetAlwaysRetHit(bool bAlwaysRetHit){m_bAlwaysRetHit = bAlwaysRetHit;}

	virtual bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){return m_bAlwaysRetHit;}
	virtual void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	virtual void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
	virtual void OnTouchCancelled(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber){}
protected:
	bool m_bAlwaysRetHit;
};

struct DTouchDialogProcer : public DTouchEmptyProcer
{
	DTouchDialogProcer(){}
	DTouchDialogProcer(std::function<void(bool, cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cb):m_funCb(cb){}
	void SetCB(std::function<void(bool, cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cb = nullptr)
	{
		m_funCb = cb;
	}
	virtual bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) override
	{
		bool bHit = TouchHitNodeV(touch, pNode);
		if (m_funCb)
		{
			m_funCb(bHit, touch, pNode, touchGrabber);
		}
		if (m_bAlwaysRetHit)
		{
			return true;
		}
		return bHit;
	}
	std::function<void(bool, cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> m_funCb;
};

inline bool IsTouchMoving(cocos2d::Touch *touch, float fMoveDist)
{
	cocos2d::Vec2 delta = touch->getDelta();
	return delta.x * delta.x + delta.y * delta.y > fMoveDist;
}

struct DTouchHitProcer : public DTouchEmptyProcer
{
	DTouchHitProcer(): m_bSelected(false){}
	DTouchHitProcer(
		std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbSelected, 
		std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbHited = nullptr)
		: m_bSelected(false)
		, m_cbSelected(cbSelected)
		, m_cbHited(cbHited)
	{}
	DTouchHitProcer(
		std::function<bool(cocos2d::Touch*, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbDiscardTouch,
		std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbSelected = nullptr, 
		std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbHited = nullptr)
		: m_bSelected(false)
		, m_cbDiscardTouch(cbDiscardTouch)
		, m_cbSelected(cbSelected)
		, m_cbHited(cbHited)
	{}

	void SetHitCB(
		std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbSelected = nullptr, 
		std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbHited = nullptr)
	{
		m_cbSelected = cbSelected;
		m_cbHited = cbHited;
	}

	void SetDiscardCB(std::function<bool(cocos2d::Touch*, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> cbDiscardTouch)
	{
		m_cbDiscardTouch = cbDiscardTouch;
	}

	virtual bool OnTouchBegan(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) override
	{
		bool bHitted = TouchHitNodeV(touch, pNode);
		if (bHitted)
		{
			Select(pNode, touchGrabber);
		}
		if (m_bAlwaysRetHit)
		{
			return true;
		}
		return bHitted;
	}
	virtual void OnTouchMoved(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) override
	{
		if (m_bSelected)
		{	
			bool bHitted = TouchHitNodeV(touch, pNode);
			if (bHitted)
			{
				if (m_cbDiscardTouch && m_cbDiscardTouch(touch, pNode, touchGrabber))
				{
					CancelSelect(pNode, touchGrabber);
				}
			}
			else
			{
				CancelSelect(pNode, touchGrabber);
			}
		}
	}
	virtual void OnTouchEnded(cocos2d::Touch *touch, cocos2d::Node*pNode, TouchGrabber *touchGrabber) override
	{
		if (m_cbHited)
		{
			m_cbHited(m_bSelected, pNode, touchGrabber);
		}	
		if (m_bSelected)
		{		
			CancelSelect(pNode, touchGrabber);
		}
	}

	void Select(cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		if (!m_bSelected)
		{
			m_bSelected = true;
			if (m_cbSelected)
			{
				m_cbSelected(true, pNode, touchGrabber);
			}
		}
	}
	void CancelSelect(cocos2d::Node*pNode, TouchGrabber *touchGrabber)
	{
		if (m_bSelected)
		{
			m_bSelected = false;
			if (m_cbSelected)
			{
				m_cbSelected(false, pNode, touchGrabber);
			}
		}
	}

	bool m_bSelected;
	std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> m_cbSelected;
	std::function<void(bool, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> m_cbHited;
	std::function<bool(cocos2d::Touch*, cocos2d::Node*pNode, TouchGrabber *touchGrabber)> m_cbDiscardTouch;
};

template <typename TouchProcer>
class TouchGrabberEx : public TouchGrabber
{
public:
	static TouchGrabberEx<TouchProcer> *create(cocos2d::Node *pTarget, bool bSwallow)
	{
		auto pRet = new TouchGrabberEx<TouchProcer>;
		if (pRet->init(pTarget, bSwallow))
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
	static TouchGrabberEx<TouchProcer> *create(cocos2d::Node *pTarget, bool bSwallow, const TouchProcer& touchProcer)
	{
		auto pRet = new TouchGrabberEx<TouchProcer>;
		if (pRet->init(pTarget, bSwallow, touchProcer))
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

	TouchProcer& GetTouchProcer()
	{
		return m_touchProcer;
	}

protected:
	virtual bool init(cocos2d::Node *pTarget, bool bSwallow)
	{
		if (TouchGrabber::init(pTarget, bSwallow, 
			std::bind(&TouchProcer::OnTouchBegan, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TouchProcer::OnTouchMoved, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TouchProcer::OnTouchEnded, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TouchProcer::OnTouchCancelled, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
			))
		{
			return true;
		}
		return false;
	}

	virtual bool init(cocos2d::Node *pTarget, bool bSwallow, const TouchProcer& touchProcer)
	{
		m_touchProcer = touchProcer;
		if (TouchGrabber::init(pTarget, bSwallow,
			std::bind(&TouchProcer::OnTouchBegan, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TouchProcer::OnTouchMoved, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TouchProcer::OnTouchEnded, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&TouchProcer::OnTouchCancelled, &m_touchProcer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
			))
		{
			return true;
		}
		return false;
	}

	TouchProcer m_touchProcer;
};

#endif