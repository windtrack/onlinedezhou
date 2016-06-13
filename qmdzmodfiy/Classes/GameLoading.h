#ifndef __GAMELOADING_H__
#define __GAMELOADING_H__

#include "GameDialog.h"

class FrameSprite;
class AnimateViewer;

class GameLoading : public GameLayer
{
public:
	CREATE_FUNC(GameLoading);
	static GameLoading * create(const cocos2d::Color4B& clr);
	virtual void setVisible(bool bVisible) override;
    
    void setTips(const std::string& szTips);
protected:
	virtual bool init();
	virtual bool init(const cocos2d::Color4B& clr);

	void ResetTip();

protected:
	//FrameSprite *m_pSpriteLoading;
	//cocos2d::Animate *m_pAniLoading;
	AnimateViewer *m_pAniViewer;
	cocos2d::Label *m_pLabelLoading;
};

class PaymentLoading : public GameLayer
{
public:
    CREATE_FUNC(PaymentLoading);
    virtual void setVisible(bool bVisible) override;
protected:
    virtual bool init();
    virtual void onKeyEscPressed();
private:
    AnimateViewer *m_pAniViewer;
};

#endif