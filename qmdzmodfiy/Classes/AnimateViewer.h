#ifndef __ANIMATEVIEWER_H__
#define __ANIMATEVIEWER_H__

#include "cocos2d.h"

class AnimateViewer : public cocos2d::Sprite
{
public:
	static AnimateViewer *create(const std::string& strAni, bool bRestoreOriginalFrame, float fInterval, unsigned uXFrame, unsigned uYFrame, unsigned uAllFrame = 0, unsigned uRepeatTime = 0xffffffff);

	virtual void setVisible(bool bVisible) override;
	void SetFinishCB(std::function<void()> cbFinished);

protected:
	bool init(const std::string& strAni, bool bRestoreOriginalFrame, float fInterval, unsigned uXFrame, unsigned uYFrame, unsigned uAllFrame, unsigned uRepeatTime);

	bool m_bRestoreOriginalFrame;
	float m_fInterval;
	unsigned m_uXFrame, m_uYFrame, m_uAllFrame, m_uRepeatTime;
	cocos2d::Texture2D *m_pTex2D;
	cocos2d::Animation *m_pAnimation;
	cocos2d::Repeat *m_pRepeat;
	std::function<void()> m_cbFinished;
};

#endif