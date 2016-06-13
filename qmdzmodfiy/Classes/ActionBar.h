#ifndef __ACTIONBAR_H__
#define __ACTIONBAR_H__

#include "cocos2d.h"
#include "CCControlSlider.h"
#include "UICommon.h"
class MenuItemSpriteWithLabel;
class AnimationManager;
class GameTable ;
class ActionBar : public cocos2d::Layer
{
public:

	static ActionBar* creatWithLayer(cocos2d::Node* node) ;
	bool initWithLayer(cocos2d::Node* node) ;
	virtual bool init();
	CREATE_FUNC(ActionBar);

	void SetBlind(double dLittleBlind, double dBigBlind);
	void Update(double dMinCall, bool bNeedRaise, double dMinRaise, double dMaxRaise, double dCash, double dAllPot, bool bHasDispatchedPublicCard);
	bool CanCheckNow();
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onMenuItemSelected(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
	void onMenuItemQuickRaisePartPort(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
	void onMenuItemQuickRaise(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
	void onMenuItemAction(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);

	//void onSliderChanged(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType controlEvent );
	void onSliderChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent );
	void ShowAction(bool bShowAction);
	
	virtual void setVisible(bool bVisible) override;
	void setButtonVisible(bool bVisible) ;

	void doActionMoveIn() ;
	void doActionMoveOut() ;

	inline void setTable(GameTable* table){m_table = table ;} ;
protected:

private:
	GameTable* m_table ;
	cocos2d::Node* m_uiroot;

	cocos2d::ui::Layout* m_actionbarleft;
	cocos2d::ui::Layout* m_actionbaright;

	cocos2d::ui::ImageView *m_pSpriteBgBar;
	cocos2d::Sprite* m_pSpriteBgSlider;
	AnimationManager* m_pSpriteAllIn;

	cocos2d::ui::Layout*  layerSlider ;


	cocos2d::ui::Button *m_pMenuItemRaisePartPot[3];
	cocos2d::ui::Text *m_pMenuItemRaisePartPotText[3];

	//弃牌
	cocos2d::ui::Button *m_pMenuItemFold;
	//让牌
	cocos2d::ui::Button *m_pMenuItemCheck;
	//跟住
	cocos2d::ui::Button *m_pMenuItemCall;
	cocos2d::ui::Text *m_pMenuItemCallText;
	//加注
	cocos2d::ui::Button *m_pMenuItemRaise;
	cocos2d::ui::Text *m_pMenuItemRaiseText;
	//allin
	cocos2d::ui::Button *m_pMenuItemAllIn;

	cocos2d::ui::Button *m_pMenuItemQuickRaise[5];//加注对应的金额
	cocos2d::ui::Text *m_pMenuItemQuickRaiseText[5];
	cocos2d::ui::Button *m_pMenuItemSlideRaise;
	cocos2d::ui::Text *m_pMenuItemSlideRaiseText;
	cocos2d::ui::Button *m_pMenuItemSlideRaiseAllIn;//加注allin



	cocos2d::extension::ControlSlider *m_pSliderRaise;
	//cocos2d::ui::Slider *m_pSliderRaise; 
	bool m_bHasDispatchPublicCard;
	double m_dLittleBlind, m_dBigBlind;
	bool m_bNeedRaise;
	double m_dMinCall, m_dMinRaise, m_dMaxRaise, m_dCash, m_dAllPot;
	double m_dQuickRaise[5];
	double m_dRaisePartPot[3];
    double m_dCurRaise;

	bool m_bBtActionVisible[5];//培养
    double m_dCurrentTime;

	double m_sliderStartValue;
	double m_sliderEndValue;


};

class PreActionBar : public cocos2d::Layer
{
public:
	static PreActionBar* creatWithLayer(cocos2d::Node* node) ;
	bool initWithLayer(cocos2d::Node* node) ;
	virtual bool init();
	CREATE_FUNC(PreActionBar);

	int GetCurSelectedAction();
	void Update(double dMinCall);

	virtual void setVisible(bool bVisible) override;
	void setButtonVisable(bool bVisible) ;
	void reSetPreActionBar() ;
private:
	void onMenuItemSelected(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
private:
	double m_dMinCall;
	//MenuItemSpriteWithLabel *m_pMenuItemCheckOrFoldUnSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCheckOrFoldSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallUnSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallAnyUnSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallAnySelected;

	cocos2d::ui::Button *m_pMenuItemCheckOrFold;
	cocos2d::ui::Button *m_pMenuItemCall;
	cocos2d::ui::Button *m_pMenuItemCallAny;


	cocos2d::ui::Text *m_pMenuItemCheckOrFoldText;
	cocos2d::ui::Text *m_pMenuItemCallText;
	cocos2d::ui::Text *m_pMenuItemCallAnyText;

	//MenuItemSpriteWithLabel *m_pMenuItemCheckOrFoldUnSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCheckOrFoldSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallUnSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallAnyUnSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallAnySelected;

	//cocos2d::MenuItemToggle *m_pMenuItemCheckOrFold;
	//cocos2d::MenuItemToggle *m_pMenuItemCall;
	//cocos2d::MenuItemToggle *m_pMenuItemCallAny;
};

class BrightActionBar : public cocos2d::Layer
{
public:
	static BrightActionBar* creatWithLayer(cocos2d::Node* node) ;
	bool initWithLayer(cocos2d::Node* node) ;

	virtual bool init();
	CREATE_FUNC(BrightActionBar);

	int GetCurSelectedAction();

	virtual void setVisible(bool bVisible) override;
		void setButtonVisable(bool bVisible) ;
private:
	double m_dMinCall;
	void onMenuItemSelected(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);

private:
	MenuItemSpriteWithLabel *m_pMenuItemCallBrightCardSelected;
	MenuItemSpriteWithLabel *m_pMenuItemCallBrightCardUnSelected;
	cocos2d::ui::Button *m_pMenuItemCallAutoBright;

	//MenuItemSpriteWithLabel *m_pMenuItemCallBrightCardSelected;
	//MenuItemSpriteWithLabel *m_pMenuItemCallBrightCardUnSelected;
	//cocos2d::MenuItemToggle *m_pMenuItemCallAutoBright;
};


#endif
