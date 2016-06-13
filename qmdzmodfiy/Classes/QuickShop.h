#ifndef __QUICKSHOP_H__
#define __QUICKSHOP_H__

#include "cocos2d.h"
#include "CCControlSlider.h"
#include "GameDialog.h"
#include "BuyAndPay.h"

class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class TextureNumber;
struct ItemInShopInfo;

class QuickShop : public GameDialog
{
public:
	virtual bool init();
	CREATE_FUNC(QuickShop);

	void UpdateJetton(double dAllCash, double dBuyJetton, double dBuyJettonMin, double dBuyJettonMax, bool bAutoBuy);
	virtual void setVisible(bool bVisible) override;
	void OnHideUi(bool bVisible);
protected:
	void onSliderChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
	void onMenuItemSelected(cocos2d::Ref *pItem);

    void switchToDiamond();
    void onMenuBuyDiamond(const ItemInShopInfo &itemInfo);
    void popBuyAndPay();
	virtual void onSetVisible(bool bVisible) override;
private:
	double m_dAutoSupply;
	bool m_bAutoSupply;

	MenuItemSpriteWithLabel *m_pMenuItemBuyJettons;
	MenuItemSpriteWithLabel *m_pMenuItemBuyTools;
    MenuItemSpriteWithLabel *m_pMenuItemBuyDiamond;

	cocos2d::Layer *m_pLayerBuyJetton;
	cocos2d::Layer *m_pLayerBuyTool;
    cocos2d::Layer *m_pLayerBuyDiamond;

	cocos2d::Sprite *m_pSpriteBgBuyJetton;
	cocos2d::Label *m_pLabelAllCash;
	TextureNumber *m_pNumBuyJetton;
	cocos2d::extension::ControlSlider *m_pSliderBuyJetton;
	cocos2d::Label *m_pLabelBuyJettonMin;
	cocos2d::Label *m_pLabelBuyJettonMax;

	cocos2d::MenuItemToggle *m_pMenuItemAutoBuyJetton2Max;
	MenuItemScale9SpriteWithLabel *m_pMenuItemOk;
    BuyGoods *m_BuyGoods;
    PaymentMethod *m_pPaymentMethod;


	
};

#endif