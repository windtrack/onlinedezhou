#include "FrameScale9Sprite.h"
#include "MenuButtonFilePath.h"
USING_NS_CC;

//FrameScale9Sprite *FrameScale9Sprite::create(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx)
//{
//	auto pRet = new FrameScale9Sprite;
//	if (pRet->init(strImgFile, uXFrames, uYFrames, uFrameIdx))
//	{
//		pRet->autorelease();
//	}
//	else
//	{
//		delete pRet;
//		pRet = nullptr;
//	}
//	return pRet;
//}


FrameScale9Sprite* FrameScale9Sprite::create(int index ,Size contentSize)
{
	auto pRet = new FrameScale9Sprite;
	if (pRet->init(index, contentSize))
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


//Sprite *FrameScale9Sprite::GetSprite(unsigned uFrameIdx)
//{
//	return Sprite::createWithTexture(m_pTexture2D, CalcFrameRect(uFrameIdx));
//}
//SpriteFrame *FrameScale9Sprite::GetSpriteFrame(unsigned uFrameIdx)
//{
//	return SpriteFrame::createWithTexture(m_pTexture2D, CalcFrameRect(uFrameIdx), false, Vec2::ZERO, Size::ZERO);
//}

void FrameScale9Sprite::SetFrameIdx(unsigned uFrameIdx)
{
	//setTextureRect(CalcFrameRect(uFrameIdx));

	for (int i=0;i<vecPSprite.size();i++)
	{
		Scale9Sprite* sprite = vecPSprite.at(i);
		sprite->setVisible(i == uFrameIdx) ;
	}
	m_uFrameIdx = uFrameIdx;
}

unsigned FrameScale9Sprite::GetFrameIdx()
{
	return m_uFrameIdx;
}

bool FrameScale9Sprite::init(int index ,Size contentSize)
{
	if (!Node::init())
	{
		return false ;
	}

	Scale9Sprite *vecPSprite0 = Scale9Sprite::create(sButtonPath[index][0],sBtRect[index][0],sBtRect[index][1]);
	Scale9Sprite *vecPSprite1  = Scale9Sprite::create(sButtonPath[index][1],sBtRect[index][0],sBtRect[index][1]);
	Scale9Sprite *vecPSprite2  = Scale9Sprite::create(sButtonPath[index][2],sBtRect[index][0],sBtRect[index][1]);


	vecPSprite0->setPreferredSize(contentSize) ;
	vecPSprite1->setPreferredSize(contentSize) ;
	vecPSprite2->setPreferredSize(contentSize) ;

	vecPSprite.push_back(vecPSprite0);
	vecPSprite.push_back(vecPSprite1);
	vecPSprite.push_back(vecPSprite2);

	this->addChild(vecPSprite0);
	this->addChild(vecPSprite1);
	this->addChild(vecPSprite2);
	vecPSprite1->setVisible(false) ;
	vecPSprite2->setVisible(false) ;

	for (int i=0;i<vecPSprite.size();i++)
	{
		Scale9Sprite *sprite = vecPSprite.at(i) ;
		sprite->ignoreAnchorPointForPosition(false) ;
		sprite->setAnchorPoint(Point(0.5,0.5)) ;
		sprite->setPosition(contentSize.width/2,contentSize.height/2) ;
	}


	m_uFrameIdx = 0;
	setContentSize(contentSize);
	return true ;
}
//bool FrameScale9Sprite::init(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx)
//{
//	m_uXFrames = uXFrames;
//	m_uYFrames = uYFrames;
//	m_uFrameIdx = uFrameIdx;
//	m_pTexture2D = Director::getInstance()->getTextureCache()->addImage(strImgFile);
//	m_szImg = m_pTexture2D->getContentSizeInPixels();
//	m_szFrame.width = m_szImg.width / uXFrames;
//	m_szFrame.height = m_szImg.height / uYFrames;
//	setContentSize(m_szFrame);
//	return Sprite::initWithTexture(m_pTexture2D, CalcFrameRect(uFrameIdx), false);
//}

Rect FrameScale9Sprite::CalcFrameRect(unsigned uFrameIdx)
{
	Rect rc;
	rc.size = m_szFrame;
	rc.origin.x = (uFrameIdx % m_uXFrames) * m_szFrame.width;
	rc.origin.y = (uFrameIdx / m_uXFrames) * m_szFrame.height;
	return rc;
}
