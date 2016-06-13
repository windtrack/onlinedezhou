#ifndef __TEXTTIP_H__
#define __TEXTTIP_H__

#include "cocos2d.h"

class TextTip : public cocos2d::Layer
{
public:
	virtual bool init(); 
	CREATE_FUNC(TextTip);

	void AddTip(const std::string& strTip);

	void OnActionCheckNextTip();

private:
	TextTip();
	~TextTip();

	cocos2d::Sprite *m_pSpriteBgTip;
	cocos2d::Label *m_pLabelTip;
	std::deque<std::string> m_deqStrTip;
};

class TextTip2 : public cocos2d::Layer
{
public:
	virtual bool init(); 
	CREATE_FUNC(TextTip2);

	void AddTip(const std::string& strTip);
    void AddTip(const std::string& strTip, float delayTime);
	void OnActionCheckNextTip();

private:
	TextTip2();
	~TextTip2();

	cocos2d::LayerColor *m_pBg;
	cocos2d::Label *m_pLabelTip;
	std::deque<std::string> m_deqStrTip;
};



#endif