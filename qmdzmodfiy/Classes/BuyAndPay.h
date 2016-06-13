#ifndef _BUYANDPAY_H__
#define _BUYANDPAY_H__

#include "GameDialog.h"
#include "ui/UIEditBox/UIEditBox.h"

class HttpSprite;
class PaymentMethod;
class FrameSprite;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class LineLabel;

struct GoodsInfo{
	std::string goodsName;
	std::string onePrice;
	std::string totalMoney;
	unsigned Id;
	double Price;
	unsigned num;
};

class BuyGoods : public GameDialog/*, public cocos2d::ui::EditBoxDelegate*/
{
public:
    CREATE_FUNC(BuyGoods);
    virtual bool init();
    void UpdateGoodTypeInfo(unsigned type, unsigned index, bool bGameMallCall);
    virtual void setVisible(bool bVisible) override;
	//void OnCancleGameLayer(bool bVisible);
    void setQuickShopOpt(std::function<void()> func);
protected:
    //virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    void onMenuSelect(cocos2d::Ref* pSend);
    //void MinusBuy(cocos2d::Ref* pSend);
    //void AddBuy(cocos2d::Ref* pSend);
private:
    /*unsigned                             m_uFruitIdx;
	unsigned							 m_uGoodsId;
    double                               m_dOneGoodsPrice;
    std::string                          m_strGoodsName;
    std::string                          m_strOnePrice;
    std::string                          m_strTotalMoney;
    cocos2d::ui::EditBox				*m_editFruitNum;
    cocos2d::Label                      *m_labelGoodsName;
    cocos2d::Label                      *m_labelGoodsPrice;
    cocos2d::Label                      *m_labelTotalMoney;
    HttpSprite                          *m_httpFruitSprite;
    FrameSprite                         *m_spriteVip;
    PaymentMethod                       *m_pPaymentMethod;
	GoodsInfo							 m_goodsInfo;*/

    MenuItemScale9SpriteWithLabel       *m_pMenuBuy;       //购买确定按钮
    MenuItemScale9SpriteWithLabel       *m_pMenuCancel;    //购买取消按钮
    cocos2d::Label                      *m_pLabelDis;      //操作描述
    LineLabel                           *m_pLineLabel;     //操作描述

    std::string                          m_strGoodsName;   //商品名
    double                               m_dGoodsPrice;    //商品消耗的钻石数量
    unsigned                             m_uGoodsID;       //商品的ID
    bool                                 m_bDiamondPlenty; //钻石是否够买当前操作的商品
    bool                                 m_bGameMallCall;  //商城里更新：true   快速商城：false

    std::function<void()>                m_func;           //快速商城钻石不够时直接切换到快速商城的钻石购买
};

class PaymentMethod : public GameLayer
{
public:
    CREATE_FUNC(PaymentMethod);
    virtual bool init();
    void UpdateGoodTypeInfo(GoodsInfo);
	void onKeyEscPressedVisible(bool bVisible);
protected:
    void BtnExit(cocos2d::Ref* pSend);
    void SelectPayMethod(unsigned idx);
    void DeterminePay(cocos2d::Ref* pSend);
    virtual void onKeyEscPressed();
private:
    cocos2d::Label                      *m_labelGoodsName;
    cocos2d::Label                      *m_laelOnePrice;
    cocos2d::Label                      *m_labelGoodsNum;
    cocos2d::Label                      *m_labelTotalMoney;
    //MenuItemSpriteWithLabel             *m_weixinPayBtn;
    MenuItemSpriteWithLabel             *m_zhifubaoPayBtn;
    MenuItemSpriteWithLabel             *m_shoujiPayBtn;
	MenuItemSpriteWithLabel             *m_weixinPayBtn;
    unsigned                             m_uPayMethod;
    double                               m_dTotalMoney;
	unsigned							 m_uGoodsId;
};

#endif