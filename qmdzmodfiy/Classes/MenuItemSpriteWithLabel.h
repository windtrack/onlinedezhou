#ifndef __MENUITEMSPRITEWITHLABEL_H__
#define __MENUITEMSPRITEWITHLABEL_H__
#include "extensions/cocos-ext.h"  
#include "cocos2d.h"
class MenuItemSpriteWithLabel : public cocos2d::MenuItemSprite
{
public:
	bool init(const std::string& strImage, unsigned uFrames, const cocos2d::ccMenuCallback& callback);
	static MenuItemSpriteWithLabel *create(const std::string& strImage, unsigned uFrames, const cocos2d::ccMenuCallback& callback);

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
	MenuItemSpriteWithLabel();

	cocos2d::Color4B m_enabledColor, m_disabledColor;
	cocos2d::Label *m_pLabel;

	std::vector<cocos2d::Sprite*> vecPSprite;
};

#endif