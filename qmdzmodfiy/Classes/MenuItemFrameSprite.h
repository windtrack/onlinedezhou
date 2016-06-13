#ifndef __MENUITEMFRAMESPRITE_H__
#define __MENUITEMFRAMESPRITE_H__

#include "cocos2d.h"

class MenuItemFrameSprite : public cocos2d::MenuItemSprite
{
public:
	bool init(const std::string& strImage, unsigned uFrames, const cocos2d::ccMenuCallback& callback);
	static MenuItemFrameSprite *create(const std::string& strImage, unsigned uFrames, const cocos2d::ccMenuCallback& callback);
};

#endif