#include "MenuItemFrameSprite.h"
#include "FrameSprite.h"

USING_NS_CC;

bool MenuItemFrameSprite::init(const std::string& strImage, unsigned uFrames, const ccMenuCallback& callback)
{
	auto pFrameSprite = FrameSprite::create(strImage, 1, uFrames);
	if (pFrameSprite)
	{
		std::vector<Sprite*> vecPSprite;
		for (auto i = 0; i < uFrames; i++)
		{
			vecPSprite.push_back(pFrameSprite->GetSprite(i));
		}
		while (vecPSprite.size() < 3)
		{
			vecPSprite.push_back(pFrameSprite->GetSprite(uFrames - 1));
		}
		return MenuItemSprite::initWithNormalSprite(vecPSprite[0], vecPSprite[1], vecPSprite[2], callback);
	}
	return false;
}

MenuItemFrameSprite *MenuItemFrameSprite::create(const std::string& strImage, unsigned uFrames, const ccMenuCallback& callback)
{
	auto pRet = new MenuItemFrameSprite();
	if (pRet->init(strImage, uFrames, callback))
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
