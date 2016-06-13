#include "MenuItemScale9Sprite.h"
#include "FrameSprite.h"
#include "MenuButtonFilePath.h"
USING_NS_CC;


MenuItemScale9Sprite *MenuItemScale9Sprite::create(const std::string& strImage, const std::string& strSImage,const std::string& strEImage,
																	 Rect rectOut,
																	 Rect rectIn,
																	 cocos2d::Size contentSize, const ccMenuCallback& callback)
{
	auto pRet = new MenuItemScale9Sprite();
	if (pRet->init(strImage,strSImage, strEImage,rectOut,rectIn,contentSize,callback))
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
/************************************************************************/
/* Ë÷Òý´´½¨                                                                     */
/************************************************************************/
MenuItemScale9Sprite *MenuItemScale9Sprite::create(unsigned index,cocos2d::Size contentSize,const cocos2d::ccMenuCallback& callback)
{
	auto pRet = new MenuItemScale9Sprite();
	if (pRet->init(sButtonPath[index][0],sButtonPath[index][1], sButtonPath[index][2],sBtRect[index][0],sBtRect[index][1],contentSize,callback))
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



bool MenuItemScale9Sprite::init(const std::string& strImage,const std::string& strSImage,const std::string& strEImage,
										 Rect rectOut,
										 Rect rectIn,
										 cocos2d::Size contentSize,const ccMenuCallback& callback)
{



	Scale9Sprite *vecPSprite0 = Scale9Sprite::create(strImage,rectOut,rectIn);
	Scale9Sprite *vecPSprite1  = Scale9Sprite::create(strSImage,rectOut,rectIn);
	Scale9Sprite *vecPSprite2  = Scale9Sprite::create(strEImage,rectOut,rectIn);


	vecPSprite0->setPreferredSize(contentSize) ;
	vecPSprite1->setPreferredSize(contentSize) ;
	vecPSprite2->setPreferredSize(contentSize) ;

	vecPSprite.push_back(vecPSprite0);
	vecPSprite.push_back(vecPSprite1);
	vecPSprite.push_back(vecPSprite2);

	return MenuItemSprite::initWithNormalSprite(vecPSprite0, vecPSprite1, vecPSprite2, callback);

}
