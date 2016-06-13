#include "GameBuyDiamond.h"
#include "TextureNumber.h"
#include "FrameScale9SpriteButton.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "Tools.h"
#include "Util.h"
#include "GameConfig.h"
USING_NS_CC;
enum
{
    ScrollViewW = 870,
    ScrollViewH = 420
};

bool GameBuyDiamond::init()
{
    if (!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }
    auto bgSz = this->getContentSize();
    //标题
    auto spriteTitle = CreateNode1AndPosAddTo<Sprite>("mall/goumaiz_title.png", bgSz.width/2, bgSz.height-67, this, 0);
    m_pScrollView = extension::ScrollView::create(Size(ScrollViewW, ScrollViewH));
    m_pScrollView->setContentSize(Size(ScrollViewW, ScrollViewH));
    m_pScrollView->setPosition(50, 47);
    m_pScrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    this->addChild(m_pScrollView);

    m_pPaymentMethod = nullptr;

    return true;
}

void GameBuyDiamond::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
        updateDiamondInfo();
    }
    else
    {
		callHideAction() ;
    }
}

void GameBuyDiamond::updateDiamondInfo()
{
    for (auto item: m_vecDiamondNew)
    {
        item->m_spriteBg->setVisible(false);
        m_vecDiamondOld.push_back(item);
    }
    m_vecDiamondNew.clear();

    float oneItemW = 246 + 20;
    float totalItemW = g_globalMallInfo.m_vecDiamond.size()*oneItemW;
    if (totalItemW < ScrollViewW)
    {
        totalItemW = ScrollViewW;
        m_pScrollView->setBounceable(false);
    }
    else
    {
        m_pScrollView->setBounceable(true);
    }

    //float fX = totalItemW-246/2;
    float fX = 246/2;
    for (auto itemInfo : g_globalMallInfo.m_vecDiamond)
    {
        auto item = GetDiamondItem(itemInfo);
        UpdateDiamondItem(item, itemInfo);
        item->m_spriteBg->setVisible(true);
        item->m_spriteBg->setPosition(fX, ScrollViewH-165);
        m_vecDiamondNew.push_back(item);
        fX += oneItemW;
    }
    m_pScrollView->setContentSize(Size(totalItemW, ScrollViewH));
}

void GameBuyDiamond::onMenuItemSelect(const ItemInShopInfo &itemInfo)
{
#if OpenAppStore 




	char buff[32] = {0};
	GoodsInfo goodsInfo;
	sprintf(buff, "%d钻石", itemInfo.m_uDiamondNum);
	goodsInfo.goodsName = buff;
	sprintf(buff, "￥%.0f", itemInfo.m_price);
	goodsInfo.totalMoney = buff;
	goodsInfo.Id = itemInfo.m_uID;
	goodsInfo.num = itemInfo.m_uDiamondNum;
	sprintf(buff, "￥%.0f / %d个", itemInfo.m_price, itemInfo.m_uDiamondNum);
	goodsInfo.onePrice = buff;
	goodsInfo.Price = itemInfo.m_price;

	Json::Value param;
	param["price"] = goodsInfo.Price;
	param["paytype"] = 4;
	param["id"] = goodsInfo.Id; // 商品id，如果为水果，就是水果id，如果是vip就是2001-2006
	param["name"] = goodsInfo.goodsName ;
	param["index"] = itemInfo.m_index ;
	SendClientRequest(ClientRequest::kMobilePay, param);

#elif Use_3rd_Pay
	char buff[32] = {0};
	GoodsInfo goodsInfo;
	sprintf(buff, "%d钻石", itemInfo.m_uDiamondNum);
	goodsInfo.goodsName = buff;
	sprintf(buff, "￥%.0f", itemInfo.m_price);
	goodsInfo.totalMoney = buff;
	goodsInfo.Id = itemInfo.m_uID;
	goodsInfo.num = itemInfo.m_uDiamondNum;
	sprintf(buff, "￥%.0f / %d个", itemInfo.m_price, itemInfo.m_uDiamondNum);
	goodsInfo.onePrice = buff;
	goodsInfo.Price = itemInfo.m_price;


	Json::Value param;
	param["price"] = goodsInfo.Price;
	param["paytype"] = 3;
	param["id"] = goodsInfo.Id; // 商品id，如果为水果，就是水果id，如果是vip就是2001-2006
	param["name"] = goodsInfo.goodsName ;
	param["index"] = itemInfo.m_index ;
	SendClientRequest(ClientRequest::kMobilePay, param);
#else
	if (m_pPaymentMethod == nullptr)
	{
		auto bgSz = this->getContentSize();
		m_pPaymentMethod = PaymentMethod::create();
		m_pPaymentMethod->ignoreAnchorPointForPosition(false);
		m_pPaymentMethod->setAnchorPoint(Vec2(0.5, 0.5));
		m_pPaymentMethod->setPosition(bgSz.width/2, bgSz.height/2);
		this->addChild(m_pPaymentMethod, 103);
	}
	m_pPaymentMethod->setVisible(true);

	char buff[32] = {0};
	GoodsInfo goodsInfo;
	sprintf(buff, "%d钻石", itemInfo.m_uDiamondNum);
	goodsInfo.goodsName = buff;
	sprintf(buff, "￥%.0f", itemInfo.m_price);
	goodsInfo.totalMoney = buff;
	goodsInfo.Id = itemInfo.m_uID;
	goodsInfo.num = itemInfo.m_uDiamondNum;
	sprintf(buff, "￥%.0f / %d个", itemInfo.m_price, itemInfo.m_uDiamondNum);
	goodsInfo.onePrice = buff;
	goodsInfo.Price = itemInfo.m_price;
	m_pPaymentMethod->UpdateGoodTypeInfo(goodsInfo);

#endif

}

GameBuyDiamond::DiamondItem* GameBuyDiamond::GetDiamondItem(ItemInShopInfo itemInfo)
{
    DiamondItem *diamondItem = nullptr;
    if (m_vecDiamondOld.empty())
    {
        //背景
        auto spriteBg = CreateNode1AndPosAddTo<Sprite>("quickshop/bgfruit.png", 0, 0, m_pScrollView, 0);
        auto bgSize = spriteBg->getContentSize();
        //光效背景
        auto shineBg = CreateNode1AndPosAddTo<Sprite>("mall/sc_shine.png", bgSize.width/2, bgSize.height/2, spriteBg, 0);
        //钻石数量及“钻”
        auto textureNum = TextureNumber::create("mall/diamondnum.png", false);
        textureNum->SetHAlignment(TextureNumberHAlignment::Left);
        textureNum->ignoreAnchorPointForPosition(false);
        textureNum->setAnchorPoint(Vec2(0.5,0.5));
        spriteBg->addChild(textureNum);
        textureNum->setPosition(bgSize.width/2-16, 290);
        auto spriteFont = CreateNode1AndPosAddTo<Sprite>("mall/zuanshi.png", 0, 0, textureNum, 0);
        //钻石图片
        auto httpSprite = HttpSprite::create("mall/default_diamond.png", Size::ZERO);
        httpSprite->ignoreAnchorPointForPosition(false);
        httpSprite->setAnchorPoint(Vec2(0.5, 0.5));
        httpSprite->setPosition(bgSize.width/2, 160);
        spriteBg->addChild(httpSprite);
        //购买按钮
        auto menuBtn = FrameScale9SpriteButton::createWithScale9Sprite(0, Size(234, 74), false ,true);
        menuBtn->setPosition(bgSize.width/2, -40);
        spriteBg->addChild(menuBtn);
        auto labelBtn = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"",Vec2(0.5,0.5), 0, 0, Color4B::WHITE, menuBtn, 0);
         
        DiamondItem *item = new DiamondItem;
        item->m_spriteBg = spriteBg;
        item->m_spriteShineBg = shineBg;
        item->m_textureNum = textureNum;
        item->m_btnScale9Sprite = menuBtn;
        item->m_labelName = labelBtn;
        item->m_spriteFont = spriteFont;
        item->m_httpSprite = httpSprite;
        diamondItem = item;
    }
    else
    {
        diamondItem = m_vecDiamondOld.back();
        m_vecDiamondOld.pop_back();
    }
    //购买回调
    diamondItem->m_btnScale9Sprite->SetHitCB(std::bind(&GameBuyDiamond::onMenuItemSelect, this, itemInfo));

    return diamondItem;
}

void GameBuyDiamond::UpdateDiamondItem(DiamondItem* item, ItemInShopInfo itemInfo)
{
    char buff[32] = {0};
    item->m_textureNum->UpdateNumber(itemInfo.m_uDiamondNum);
    auto textureSz = item->m_textureNum->getContentSize();
    item->m_spriteFont->setPosition(textureSz.width+20, -3);
    sprintf(buff, "%.0f元", itemInfo.m_price);
    SetLabelString(item->m_labelName, buff);
    SetHttpSpriteURL(item->m_httpSprite, itemInfo.m_iconUrl);
}

void GameBuyDiamond::OnCancleGameLayer(bool bVisible)
{
    if (m_pPaymentMethod != nullptr)
        m_pPaymentMethod->setVisible(bVisible);
}