#ifndef __LOGOLAYER_H__
#define __LOGOLAYER_H__

#include "cocos2d.h"
#include "UICommon.h"

class AnimateViewer;

class LogoLayer : cocos2d::Layer
{
public:
	LogoLayer();
	~LogoLayer();

	CREATE_FUNC(LogoLayer);

	static cocos2d::Scene* createScene();

	virtual bool init();  

	virtual void update(float dt);
		
	void StartUpLogo(float fDelat);
protected:
	cocos2d::EventListenerCustom* m_updateListener;
private:
	cocos2d::Label* ttf ;
    AnimateViewer *m_animate;
	/*cocos2d::ui::LoadingBar* _progressBar;
	cocos2d::Sprite* _spriteLoading;*/
    //cocos2d::Sprite *m_spriteBg;
    cocos2d::Layer  *m_layerSpriteBg;
    cocos2d::Sprite *m_spriteBg1;
    cocos2d::Sprite *m_logoSprite;
    //cocos2d::Sprite *m_textSprite;

	 cocos2d::Sprite *m_spriteLoadingPro;
	int curPrecent;
};

#endif