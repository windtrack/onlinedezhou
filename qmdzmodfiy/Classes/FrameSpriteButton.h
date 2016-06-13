#ifndef __FRAMESPRITEBUTTON_H__
#define __FRAMESPRITEBUTTON_H__

#include "cocos2d.h"

class FrameSprite;
class TouchGrabber;

class FrameSpriteButton : public cocos2d::Layer
{
public:
	static FrameSpriteButton * createWithHoriFrameSprite(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove);
	static FrameSpriteButton * createWithVertFrameSprite(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove);
	static FrameSpriteButton * createWithVertFrameSpriteByOne(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove);
	void SetSelectCB(std::function<void(cocos2d::Ref*, bool)> cbSelected);
	void SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted);
	void SetEnable(bool bEnable);
	bool IsEnable();

protected:
	virtual bool init(FrameSprite *frameSprite, bool bSwallowTouch, bool bCheckMove);
		
	virtual void onEnter() override;
	virtual void onExit() override;


	bool isSeeable();
	bool isSelected();
	void setSelected(bool bSelect);

private:
	FrameSprite *m_pFrameSprite;
	TouchGrabber *m_pTouchGrabber;
	std::function<void(cocos2d::Ref*, bool)> m_cbSelected;
	std::function<void(cocos2d::Ref*)> m_cbHitted;
};

class LayerButton : public cocos2d::Layer
{
public:
	static LayerButton * create(bool bSwallowTouch, bool bCheckMove);

	void SetSelectCB(std::function<void(cocos2d::Ref*, bool bSel)> cbSelected);
	void SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted);
	void SetEnable(bool bEnable);
	bool IsEnable();

	bool isSelected();
private:
	virtual bool init(bool bSwallowTouch, bool bCheckMove);
	virtual void onEnter() override;
	virtual void onExit() override;


	bool isSeeable();
	
	void setSelected(bool bSelect);

	bool m_bSelected;
	TouchGrabber *m_pTouchGrabber;
	std::function<void(cocos2d::Ref*, bool)> m_cbSelected;
	std::function<void(cocos2d::Ref*)> m_cbHitted;
};


#endif