#include "FrameSprite.h"

USING_NS_CC;

FrameSprite *FrameSprite::create(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx)
{
	auto pRet = new FrameSprite;
	if (pRet->init(strImgFile, uXFrames, uYFrames, uFrameIdx))
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

Sprite *FrameSprite::GetSprite(unsigned uFrameIdx)
{
	return Sprite::createWithTexture(m_pTexture2D, CalcFrameRect(uFrameIdx));
}
SpriteFrame *FrameSprite::GetSpriteFrame(unsigned uFrameIdx)
{
	return SpriteFrame::createWithTexture(m_pTexture2D, CalcFrameRect(uFrameIdx), false, Vec2::ZERO, Size::ZERO);
}

void FrameSprite::SetFrameIdx(unsigned uFrameIdx)
{
	setTextureRect(CalcFrameRect(uFrameIdx));
	m_uFrameIdx = uFrameIdx;
}

unsigned FrameSprite::GetFrameIdx()
{
	return m_uFrameIdx;
}

bool FrameSprite::init(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx)
{
	m_uXFrames = uXFrames;
	m_uYFrames = uYFrames;
	m_uFrameIdx = uFrameIdx;
	m_pTexture2D = Director::getInstance()->getTextureCache()->addImage(strImgFile);
	m_szImg = m_pTexture2D->getContentSizeInPixels();
	m_szFrame.width = m_szImg.width / uXFrames;
	m_szFrame.height = m_szImg.height / uYFrames;
	setContentSize(m_szFrame);
	return Sprite::initWithTexture(m_pTexture2D, CalcFrameRect(uFrameIdx), false);
}

Rect FrameSprite::CalcFrameRect(unsigned uFrameIdx)
{
	Rect rc;
	rc.size = m_szFrame;
	rc.origin.x = (uFrameIdx % m_uXFrames) * m_szFrame.width;
	rc.origin.y = (uFrameIdx / m_uXFrames) * m_szFrame.height;
	return rc;
}
