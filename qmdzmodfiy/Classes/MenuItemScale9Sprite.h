#ifndef __MenuItemScale9Sprite_H__
#define __MenuItemScale9Sprite_H__

#include "cocos2d.h"

#include "UICommon.h"
class MenuItemScale9Sprite : public cocos2d::MenuItemSprite
{
public:
	bool init(const std::string& strImage, const std::string& strSImage,const std::string& strEImage, 
		Rect rectOut,
		Rect rectIn,
		cocos2d::Size contentSize,
		const cocos2d::ccMenuCallback& callback);
	static MenuItemScale9Sprite *create(const std::string& strImage,const std::string& strSImage,const std::string& strEImage, 
		Rect rectOut,
		Rect rectIn,
		cocos2d::Size contentSize,
		const cocos2d::ccMenuCallback& callback);


	static MenuItemScale9Sprite *create(unsigned index,cocos2d::Size contentSize,const cocos2d::ccMenuCallback& callback);

private:
	std::vector<cocos2d::ui::Scale9Sprite*> vecPSprite;
};

#endif