#include "AnimateViewer.h"

USING_NS_CC;

AnimateViewer *AnimateViewer::create(const std::string& strAni, bool bRestoreOriginalFrame, float fInterval, unsigned uXFrame, unsigned uYFrame, unsigned uAllFrame /*= 0*/, unsigned uRepeatTime)
{
	auto ret = new AnimateViewer;
	if (uAllFrame == 0)
	{
		uAllFrame = uXFrame * uYFrame;
	}
	if (ret->init(strAni, bRestoreOriginalFrame, fInterval, uXFrame, uYFrame, uAllFrame, uRepeatTime))
	{
		ret->autorelease();
	}
	else
	{
		delete ret;
		ret = nullptr;
	}
	return ret;
}

void AnimateViewer::setVisible(bool bVisible)
{
	if (isVisible() != bVisible)
	{
		Sprite::setVisible(bVisible);
		if (bVisible)
		{
			if (m_pAnimation == nullptr)
			{
				auto sz = getContentSize();
				m_pAnimation = Animation::create();
				for (unsigned i = 0; i < m_uAllFrame; ++i)
				{
					auto spriteFrame = SpriteFrame::createWithTexture(m_pTex2D, Rect((i % m_uXFrame) * sz.width, (i / m_uXFrame * sz.height), sz.width, sz.height));
					m_pAnimation->addSpriteFrame(spriteFrame);
				}
				m_pAnimation->setDelayPerUnit(m_fInterval);
				m_pAnimation->setRestoreOriginalFrame(m_bRestoreOriginalFrame);
				m_pAnimation->retain();
				auto animate = Animate::create(m_pAnimation);
				m_pRepeat = Repeat::create(animate, m_uRepeatTime);
			}
			else
			{
				auto animate = Animate::create(m_pAnimation);
				m_pRepeat = Repeat::create(animate, m_uRepeatTime);
			}			
			if (m_cbFinished)
			{
				runAction(Sequence::create(m_pRepeat, CallFunc::create(m_cbFinished), nullptr));
			}
			else
			{
				runAction(m_pRepeat);
			}
		}
		else
		{
			if (m_pRepeat)
			{
				stopAction(m_pRepeat);
				m_pRepeat = nullptr;
			}
		}
	}
}

void AnimateViewer::SetFinishCB(std::function<void()> cbFinished)
{
	m_cbFinished = cbFinished;
}
bool AnimateViewer::init(const std::string& strAni, bool bRestoreOriginalFrame, float fInterval, unsigned uXFrame, unsigned uYFrame, unsigned uAllFrame, unsigned uRepeatTime)
{
	m_fInterval = fInterval;
	m_bRestoreOriginalFrame = bRestoreOriginalFrame;
	m_uXFrame = uXFrame;
	m_uYFrame = uYFrame;
	m_uAllFrame = uAllFrame;
	m_uRepeatTime = uRepeatTime;
	m_pRepeat = nullptr;
	m_pAnimation = nullptr;
	m_pTex2D = nullptr;

	Image *pImg = new Image;
	if (pImg->initWithImageFile(strAni))
	{
		pImg->autorelease();
		Texture2D *pTex2d = new Texture2D;
		if (pTex2d->initWithImage(pImg, pImg->getRenderFormat()))
		{
			pTex2d->autorelease();
			float fFrameW = pImg->getWidth() / uXFrame;
			float fFrameH = pImg->getHeight() / uYFrame;
			Rect rc(0, 0, fFrameW, fFrameH);
			bool bOk = Sprite::initWithTexture(pTex2d, rc, false);
			if (bOk)
			{
				m_pTex2D = pTex2d;
				pTex2d->retain();
				pImg->retain();
				setVisible(false);
				return true;
			}
		}
	}
	return false;
}
