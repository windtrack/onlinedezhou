#ifndef __GAMEBUYDIAMOND_H__
#define __GAMEBUYDIAMOND_H__

#include "GameDialog.h"
#include "cocos2d.h"
#include "CCScrollView.h"
#include "BuyAndPay.h"

class TextureNumber;
class FrameScale9SpriteButton;
class HttpSprite;
struct ItemInShopInfo;
struct GoodsInfo;

class GameBuyDiamond : public GameDialog
{
public:
    CREATE_FUNC(GameBuyDiamond);
    void updateDiamondInfo();
    virtual void setVisible(bool Visible) override;
    void OnCancleGameLayer(bool bVisible);
protected:
    bool init();
    struct DiamondItem
    {
        cocos2d::Sprite         *m_spriteBg;        //背景
        cocos2d::Sprite         *m_spriteShineBg;   //发光背景
        cocos2d::Sprite         *m_spriteFont;      //“钻”字
        cocos2d::Label          *m_labelName;       //按钮文字
        TextureNumber           *m_textureNum;      //钻石价格
        HttpSprite              *m_httpSprite;      //钻石图片
        FrameScale9SpriteButton    *m_btnScale9Sprite; //按钮
    };

    DiamondItem* GetDiamondItem(ItemInShopInfo itemInfo);
    void UpdateDiamondItem(DiamondItem* item, ItemInShopInfo itemInfo);

    void onMenuItemSelect(const ItemInShopInfo & itemInfo);
private:
    cocos2d::extension::ScrollView     *m_pScrollView;
    std::vector<DiamondItem*>           m_vecDiamondNew;
    std::vector<DiamondItem*>           m_vecDiamondOld;
    PaymentMethod                      *m_pPaymentMethod;
};
#endif