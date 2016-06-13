#ifndef __GAMEMALL_H__
#define __GAMEMALL_H__

#include "GameDialog.h"
#include "CCScrollView.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "UICommon.h"
class FrameSprite ;
class BuyGoods;
class HttpSprite;
class FrameSpriteButton ;
class DataLoading;
class FrameScale9SpriteButton;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
struct  GlobalMallInfo;
struct ItemInShopInfo ;
class LayerButton;
class LineLabel;
class GameBuyDiamond;
class GameMall : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(GameMall);
	virtual bool init();
	void SwitchToArea(unsigned uArea);
	//void AddFruit(const std::string& strName, double dSendCash, const std::string& strIcon, unsigned uPrice, unsigned uID);
	//void InitVipInfo(const VIPInfo vipInfo[9]);
	//void AddExchange(const std::string& strName, const std::string& strIcon, unsigned uID);
	void onCancelMenuEnable(unsigned uVipLevel);
	void onCancleBuyGoodsUI(bool bVisible);
    
    void updateExchangeTips(unsigned uType, std::string str1, std::string str2);
    void initExChangeLayer();
    void popExchangeTips();

	void editBoxReturn(cocos2d::ui::EditBox* editBox);
    void editBoxEditingDidBegin(EditBox* editBox);

    void popDiamondBuy();
    void popBuyAndPay();

    void initHeadShopLayer();
protected:
	void onMenuItemSwitchArea(cocos2d::Ref*, cocos2d::ui::TouchEventType type);
	void onMenuItemBuyFruit(cocos2d::Ref*, unsigned idx);
	void onMenuItemBuyVIP(cocos2d::Ref*, unsigned idx);
	void onMenuItemSaveAddr(cocos2d::Ref*);
    void onMenuItemTips(cocos2d::Ref*);//兑换界面提示按钮回调
    //void onMenuBackAddr();

	void onMenuItemClose(cocos2d::Ref* pMenuItem, cocos2d::ui::TouchEventType type);
    void onMenuItemBuyDiamond(cocos2d::Ref* pMenuItem, cocos2d::ui::TouchEventType type);

	void PopAddrSet();
    
	virtual void onSetVisible(bool bVisible) override;

	//道具商城
	void initItemShopLayer() ;
	void updateItemSopLayer() ;
    //头像商城
    //void initHeadShopLayer();
    void PopFigure(unsigned uIndex);//点击头像 弹出形象
    //兑换商城
    //void initExChangeLayer();
    bool judgeEditboxInfo();
    Sprite *GetFrameBgSprite(unsigned uIdx);//uIdx=0:兑换    uIdx=1：头像
    void onUpdateMyData(unsigned uType);
public:
	struct ShopItemLabel
	{
		cocos2d::Sprite		    *m_pSpriteBGItem;	// 滚动视图行的背景
        cocos2d::Sprite         *m_pSpriteDiamond;  //钻石图标
		HttpSprite			*m_pSpriteIcon;			// 头像
		cocos2d::Label		*m_pLabelName;	
		cocos2d::Label		*m_pLabelDes; 
		FrameScale9SpriteButton   *m_pBt;			//按钮分类型显示文字	
		cocos2d::Label		*m_pLabelBtDes;         //在按钮文字描述
	};
	//获取列表显示
	ShopItemLabel* GetListItem(const ItemInShopInfo& itemInfo);
	void UpdateItem(GameMall::ShopItemLabel *item, const ItemInShopInfo& itemInfo);

    //头像商城结构
    struct HeadShopItem
    {
        HttpSprite                  *m_httpSpriteHead;   //人物头像
        FrameScale9SpriteButton     *m_btnBuy;           //购买按钮
        cocos2d::ClippingNode       *m_clipNode;         //遮罩
        cocos2d::Sprite             *m_spriteFrame;     //头像边框
        cocos2d::ui::Scale9Sprite   *m_spriteMask;      //遮罩底
        cocos2d::Label              *m_labelBtnName;    //按钮文字
        LayerButton                 *m_btnFigure;       //形象
        cocos2d::Sprite             *m_spriteShineBg;   //发光背景
        cocos2d::LayerColor         *m_layerColor;      //任务背景色
        cocos2d::Sprite             *m_spriteShadow;    //右侧的阴影
        cocos2d::Sprite             *m_pSpriteDiamond;  //钻石图标
    };
    HeadShopItem* GetHeadShopItem(const ItemInShopInfo& itemInfo);
    void updateHeadShopItem(GameMall::HeadShopItem* headShop, const ItemInShopInfo& itemInfo);
    void menuHeadShopcallBack(ItemInShopInfo baseInfo); //头像下的按钮

    struct ExChangeItem
    {
        cocos2d::Sprite     *m_spriteShineBg;//光效背景
        cocos2d::Label      *m_labelDes;    //兑换描述
        cocos2d::Label      *m_labelBtnName;//兑换按钮名称
        cocos2d::Label      *m_labelInDate;   //有效期
        HttpSprite          *m_httpSpriteChange;//兑换券
        FrameScale9SpriteButton *m_btnExchange;  //兑换操作按钮
        LayerButton         *m_layerBtn;
    };
    ExChangeItem* GetExchangeShopItem(const ItemInShopInfo& itemInfo);
    void updateExchangeItem(GameMall::ExChangeItem* exchangeItem, const ItemInShopInfo& itemInfo);
    void onExchangeBtnClick(const ItemInShopInfo& itemInfo);//

	void menuClickItem(ItemInShopInfo baseInfo);
private:
	cocos2d::ui::Button *m_pMenuItemSwitchArea[5];
	cocos2d::ui::Text *m_pTextLabel[5];
	cocos2d::ui::ImageView *m_pSpriteBg[5];
	unsigned m_uCurSelArea;
	cocos2d::ui::Button *m_pBtclose;
    cocos2d::ui::ImageView *m_pImageDivision;
    cocos2d::Vec2           m_posDivision;
    MenuItemScale9SpriteWithLabel *m_pMenuBuyDiamond[6];

	cocos2d::Label *m_pLabelVIPStatus;
	cocos2d::extension::ScrollView *m_pScrollViewVIPInfo;
	std::vector<cocos2d::MenuItem*> m_pMenuItemBuyVIP;

	std::vector<cocos2d::MenuItem*> m_pMenuItemExchange;

	//兑换商城 收货地址
	GameDialog *m_pDlgAddrSet;
    GameLayer  *m_pDlgExchangeTips;
	cocos2d::Label *m_pLabelAddrSetTitle;	
	cocos2d::ui::EditBox *m_pEdtAddrItem[5];//信息编辑框
    FrameSpriteButton    *m_btnBackAddr;
    MenuItemSpriteWithLabel *m_pMenuItemAddr;//收货地址
	MenuItemScale9SpriteWithLabel *m_pMenuItemSaveAddr;
	MenuItemScale9SpriteWithLabel *m_pMenuitem[6];
    cocos2d::Label                *m_labelEditbox[5];//为扩大编辑框点击区域，单独存放输入
    cocos2d::Label                *m_labelAddrInfo[5];
    MenuItemScale9SpriteWithLabel *m_pMenuExchange[2];//兑换弹窗按钮
    cocos2d::Label                *m_labelFriendlyTips;//友情提示
    LineLabel                     *m_labelContent;//兑换弹窗内容
    cocos2d::Sprite               *m_rewardTitle;//兑换成功后的标题
    unsigned                       m_uTipsType;//兑换提示类型
    bool                           m_bModifyInfo;//是修改还是保存
    int                            m_iCurConsume;//当前兑换消耗
    int                            m_iCurExchangeId;//兑换券id
    int                            m_iCurExchangeType;//1:正常兑换 2:失效兑换直接扣票)

    BuyGoods                *m_pBuyGoods;

	//道具商城
	std::vector<ShopItemLabel*>		            m_vecItemListOld;
	DataLoading* s_dataLoading ;
	cocos2d::extension::ScrollView* m_ScrollView ;
	cocos2d::Size sizeListView;

    //头像商城
    cocos2d::extension::ScrollView* m_scrollHeadShop;
    HttpSprite                     *m_httpSpriteFigure;//点击头像商城时显示 形象
    GameLayer                      *m_layerColor;//形象全屏背景
    FrameScale9SpriteButton        *m_btnBuyHead;
    cocos2d::Label                 *m_labelBtnName;
    cocos2d::Sprite                *m_spriteDiamond;
    std::vector<HeadShopItem*>      m_vecHeadNew;
    std::vector<HeadShopItem*>      m_vecHeadOld;
    //cocos2d::ui::ImageView         *m_imageFrameHead;//商品架
    cocos2d::Sprite                *m_spriteFrameHead;

    //兑换商城
    std::vector<ExChangeItem*>     m_vecExchangeNew;
    std::vector<ExChangeItem*>     m_vecExchangeOld;
    //cocos2d::ui::ImageView         *m_imageFrameExchange;//商品架
    cocos2d::extension::ScrollView *m_scrollExchange;
    cocos2d::ui::Scale9Sprite      *m_scale9SpriteShadow;//最上面的阴影
    cocos2d::Sprite                *m_spriteFrame;
    //兑换&头像的背景 底座
    std::vector<cocos2d::Sprite*>    m_vecSpriteFrameBgNew[2];
    std::vector<cocos2d::Sprite*>    m_vecSpriteFrameBgOld[2];
    //购买钻石按钮
    cocos2d::ui::Button              *m_btnBuyDiamond;
    cocos2d::ui::Text                *m_textCurDiamond;
    GameBuyDiamond                   *m_pGameBuyDiamond;

};

#endif