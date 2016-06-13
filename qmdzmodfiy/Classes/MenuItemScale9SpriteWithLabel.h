#ifndef __MENUITEMSCALE9SPRITEWITHLABEL_H__
#define __MENUITEMSCALE9SPRITEWITHLABEL_H__
#include "extensions/cocos-ext.h"  
#include "cocos2d.h"
#include "UICommon.h"





class MenuItemScale9SpriteWithLabel : public cocos2d::MenuItemSprite
{
public:
	bool init(const std::string& strImage, const std::string& strSImage,const std::string& strEImage, 
		Rect rectOut,
		Rect rectIn,

		cocos2d::Size contentSize,
		const cocos2d::ccMenuCallback& callback);
	static MenuItemScale9SpriteWithLabel *create(const std::string& strImage,const std::string& strSImage,const std::string& strEImage, 
		Rect rectOut,
		Rect rectIn,
		cocos2d::Size contentSize,
		const cocos2d::ccMenuCallback& callback);


	static MenuItemScale9SpriteWithLabel *create(unsigned index,cocos2d::Size contentSize,const cocos2d::ccMenuCallback& callback);


	void SetEnabledColor(const cocos2d::Color4B& color);
	void SetDisabledColor(const cocos2d::Color4B& color);

	void SetLabel(cocos2d::Label* pLabel);
	cocos2d::Label *GetLabel();    

	virtual void setEnabled(bool bEnabled);

	void setButtonSize(cocos2d::Size contentSize) ;

protected:
	virtual void selected();
	virtual void unselected();

protected:
	MenuItemScale9SpriteWithLabel();

	cocos2d::Color4B m_enabledColor, m_disabledColor;
	cocos2d::Label *m_pLabel;

	std::vector<cocos2d::ui::Scale9Sprite*> vecPSprite;
};

#endif