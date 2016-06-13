
#ifndef __FrameScale9SpriteButton_H__
#define __FrameScale9SpriteButton_H__

#include "cocos2d.h"
#include "UICommon.h"
class FrameSprite;
class FrameScale9Sprite ;
class TouchGrabber;

class FrameScale9SpriteButton : public cocos2d::Layer
{
public:
	//static FrameScale9SpriteButton * createWithHoriFrameSprite(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove);
	//static FrameScale9SpriteButton * createWithVertFrameSprite(const std::string& strFrameSprite, bool bSwallowTouch, bool bCheckMove);


	static FrameScale9SpriteButton * createWithScale9Sprite(int index, cocos2d::Size contentSize, bool bSwallowTouch, bool bCheckMove);

	void SetSelectCB(std::function<void(cocos2d::Ref*, bool)> cbSelected);
	void SetHitCB(std::function<void(cocos2d::Ref*)> cbHitted);
	void SetEnable(bool bEnable);
	bool IsEnable();
	bool isSelected();
protected:
	//virtual bool init(FrameSprite *frameSprite, bool bSwallowTouch, bool bCheckMove);

	virtual bool init(FrameScale9Sprite* scale9Sprite ,int index, cocos2d::Size contentSize, bool bSwallowTouch, bool bCheckMove);

	virtual void onEnter() override;
	virtual void onExit() override;


	bool isSeeable();
	
	void setSelected(bool bSelect);

private:
	FrameScale9Sprite *m_pFrameSprite;
	TouchGrabber *m_pTouchGrabber;
	std::function<void(cocos2d::Ref*, bool)> m_cbSelected;
	std::function<void(cocos2d::Ref*)> m_cbHitted;
	std::vector<cocos2d::ui::Scale9Sprite*> vecPSprite;
};


#endif