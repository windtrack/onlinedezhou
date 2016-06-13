#ifndef __DOORMENU_H__
#define __DOORMENU_H__

#include "cocos2d.h"
#include "GameDialog.h"

class MenuItemSpriteWithLabel;

class DoorMenu : public GameDialog
{
public:
	static DoorMenu* creatWithLayer(cocos2d::Node* node);
	bool initWithLayer(cocos2d::Node* node) ;
	virtual bool init();
	CREATE_FUNC(DoorMenu);

	void SetCBShowCardType(std::function<void()> funCB)
	{
		m_funCBShowCardType = funCB;
	}
protected:
	virtual void onGrabTouchBegan(bool bHit) override;
	virtual void onSetVisible(bool bVisible) override;
	virtual void onKeyEscPressed() override;
	void onMenuItemSelected(cocos2d::Ref *pItem);

	void Hide();

private:
	cocos2d::Node* m_nodePos;

	MenuItemSpriteWithLabel *m_pMenuItemReturn;
	MenuItemSpriteWithLabel *m_pMenuItemChangeTable;
	MenuItemSpriteWithLabel *m_pMenuItemStandUp;
	MenuItemSpriteWithLabel *m_pMenuItemShowCardType;
	MenuItemSpriteWithLabel *m_pMenuItemTaskSet;
	std::function<void()> m_funCBShowCardType;
};

class CardTypeView : public GameDialog
{
public:
	virtual bool init();
	CREATE_FUNC(CardTypeView);

protected:
	virtual void onGrabTouchBegan(bool bHit) override;
	virtual void onSetVisible(bool bVisible) override;
	virtual void onKeyEscPressed() override;

	void Hide();
};

#endif