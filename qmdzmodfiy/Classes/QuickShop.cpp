#include "QuickShop.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "FrameScale9SpriteButton.h"
#include "TextureNumber.h"
#include "GrabKeyboard.h"
#include "GrabTouch.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "CCScrollView.h"
#include "HttpSprite.h"
#include "LineLabel.h"
#include "BaseButton.h"
#include "BuyAndPay.h"
#include "Tools.h"
#include "TextureNumber.h"
#include "GameConfig.h"
USING_NS_CC;

bool QuickShop::init()
{
	if (!GameDialog::init("quickshop/bgquickshop.png", true))
	{
		return false;
	}

	Size pQuickShop = this->getContentSize();
	this->GetMenuItemClose()->setPosition(pQuickShop.width - 25, pQuickShop.height - 25);

	auto sz = getContentSize();
	m_pLayerBuyJetton = Layer::create();
	m_pLayerBuyJetton->setContentSize(sz);
	addChild(m_pLayerBuyJetton, 0);

	m_pLayerBuyTool = Layer::create();
	m_pLayerBuyTool->setContentSize(sz);
	addChild(m_pLayerBuyTool, 0);
	m_pLayerBuyTool->setVisible(false);

	float fH = 385;
	auto scrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(578, fH), 24, 0, m_pLayerBuyTool, 0);
	unsigned uTools = 6;
	float fXSpace = 260;
	float fX = 140;
	const float fY = 200;
	float fAllW = (uTools - 1) * fXSpace + fX * 2;	
	scrollView->setContentSize(Size(fAllW, fH));
	scrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);

    m_BuyGoods = nullptr;

    unsigned index = 0;
	for (auto& fruit : g_globalMallInfo.m_fruitInfo)
	{
		auto spriteBgFruit = Sprite::create("quickshop/bgfruit.png");
		spriteBgFruit->setPosition(fX, fY);
		scrollView->addChild(spriteBgFruit);
        auto bgSz = spriteBgFruit->getContentSize();
		auto spriteFruit = HttpSprite::create("mall/defaultfruiticon.png");
        spriteFruit->ignoreAnchorPointForPosition(false);
        spriteFruit->setAnchorPoint(Vec2(0.5, 0.5));
		SetHttpSpriteURL(spriteFruit, fruit.m_strIconUrl);
		spriteFruit->setPosition(bgSz.width/2, bgSz.height/2);
		spriteBgFruit->addChild(spriteFruit);
		auto label = LineLabel::create();
		label->Appand(24, "fzltbd.ttf", "送", Color4B::WHITE);
		label->Appand(24, "fzltbd.ttf", FormatCash(fruit.m_dReturnCash), Color4B(0xff, 0xd2, 0, 0xff));
		label->Appand(24, "fzltbd.ttf", "德州币", Color4B::WHITE);
		label->setPosition(125, 224);
		spriteBgFruit->addChild(label);

		//FrameSpriteBGButton* bt = createButton("quickshop/btok.png", true);
        auto bt = FrameScale9SpriteButton::createWithScale9Sprite(0, Size(203, 70), false, true);
        bt->SetHitCB([this,index](Ref*)
        {
            popBuyAndPay();
            m_BuyGoods->UpdateGoodTypeInfo(0, index, false);
        });
		bt->setPosition(125, 60);
		auto labelBt = CreateLabelMSYHBD(30);
        labelBt->ignoreAnchorPointForPosition(false);
        labelBt->setAnchorPoint(Vec2(0, 0.5));
        labelBt->setPosition(-10, 0);
		auto sz = bt->getContentSize();
		char szPrice[64] = {0};
		sprintf(szPrice, "%s", FormatCash(fruit.m_dPrice).c_str());
		SetLabelString(labelBt, szPrice);
        CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", -35, 0, bt, 0);
		//labelBt->setPosition(sz.width / 2, sz.height / 2);
		bt->addChild(labelBt);
		spriteBgFruit->addChild(bt);

		fX += fXSpace;
        index += 1;
	}

    m_pLayerBuyDiamond = Layer::create();
    m_pLayerBuyDiamond->setContentSize(sz);
    m_pLayerBuyDiamond->setVisible(false);
    this->addChild(m_pLayerBuyDiamond);
    fX = 140;
    fAllW = g_globalMallInfo.m_vecDiamond.size()*fXSpace;
    auto scrollDiamond = CreateNode1AndPosAddTo<extension::ScrollView>(Size(578, fH), 24, 0, m_pLayerBuyDiamond, 0);
    scrollDiamond->setContentSize(Size(578, fH));
    scrollDiamond->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    for (auto diamondInfo : g_globalMallInfo.m_vecDiamond)
    {
        auto diamondBg =  CreateNode1AndPosAddTo<Sprite>("quickshop/bgfruit.png", fX, fY, scrollDiamond, 0);
        auto bgSize = diamondBg->getContentSize();
        auto shineBg = CreateNode1AndPosAddTo<Sprite>("mall/sc_shine.png", bgSize.width/2, bgSize.height/2, diamondBg, 0);
        auto textureNum = TextureNumber::create("mall/diamondnum.png", false);
        textureNum->setPosition(bgSize.width/2-16, 290);
        textureNum->SetHAlignment(TextureNumberHAlignment::Left);
        textureNum->ignoreAnchorPointForPosition(false);
        textureNum->setAnchorPoint(Vec2(0.5,0.5));
        diamondBg->addChild(textureNum);
        auto spriteFont = CreateNode1AndPosAddTo<Sprite>("mall/zuanshi.png", 0, 0, textureNum, 0);
        //钻石图片
        auto httpSprite = HttpSprite::create("mall/default_diamond.png", Size::ZERO);
        httpSprite->ignoreAnchorPointForPosition(false);
        httpSprite->setAnchorPoint(Vec2(0.5, 0.5));
        httpSprite->setPosition(bgSize.width/2, 170);
        diamondBg->addChild(httpSprite);
        //操作按钮
        auto menuBtn = FrameScale9SpriteButton::createWithScale9Sprite(0, Size(203, 70), false ,true);
        menuBtn->setPosition(125, 50);
        diamondBg->addChild(menuBtn);
        auto labelBtn = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"",Vec2(0.5,0.5), 0, 0, Color4B::WHITE, menuBtn, 0);

        char buff[32];
        textureNum->UpdateNumber(diamondInfo.m_uDiamondNum);
        auto textureSz = textureNum->getContentSize();
        spriteFont->setPosition(textureSz.width+20, -3);
        sprintf(buff, "%.0f元", diamondInfo.m_price);
        SetLabelString(labelBtn, buff);
        SetHttpSpriteURL(httpSprite, diamondInfo.m_iconUrl);

        menuBtn->SetHitCB(std::bind(&QuickShop::onMenuBuyDiamond, this, diamondInfo));

        fX += fXSpace;
    }
    scrollDiamond->setContentSize(Size(fAllW, fH));

	m_pSpriteBgBuyJetton = Sprite::create("quickshop/bgbuyjetton.png");
	m_pSpriteBgBuyJetton->setPosition(305, 274);
	m_pLayerBuyJetton->addChild(m_pSpriteBgBuyJetton);

	m_pLabelAllCash = CreateLabelMSYH(24, "", TextHAlignment::LEFT, TextVAlignment::BOTTOM);
	m_pLabelAllCash->setAnchorPoint(Vec2(0, 0));
	m_pLabelAllCash->setPosition(41, 335);
	m_pLabelAllCash->setTextColor(Color4B(0xff, 0xd2, 0x0, 255));
	m_pLayerBuyJetton->addChild(m_pLabelAllCash);

	m_pNumBuyJetton = TextureNumber::create("quickshop/numbuyjetton.png", false);
	m_pNumBuyJetton->setPosition(305, 273);
	m_pLayerBuyJetton->addChild(m_pNumBuyJetton);

	m_pSliderBuyJetton = extension::ControlSlider::create("quickshop/sliderbuyjettonbg.png", "quickshop/sliderbuyjettonprogress.png", "quickshop/sliderbuyjettonthumb.png");
	m_pSliderBuyJetton->setAnchorPoint(Vec2(0, 0));
	m_pSliderBuyJetton->setPosition(45, 179);
	m_pSliderBuyJetton->addTargetWithActionForControlEvents(this, cccontrol_selector(QuickShop::onSliderChanged), extension::Control::EventType::VALUE_CHANGED);
	m_pSliderBuyJetton->setVisible(true);
	m_pLayerBuyJetton->addChild(m_pSliderBuyJetton);

	m_pLabelBuyJettonMin = CreateLabelMSYH(20, "",  TextHAlignment::LEFT, TextVAlignment::BOTTOM);
	m_pLabelBuyJettonMin->setAnchorPoint(Vec2(0, 0));
	m_pLabelBuyJettonMin->setPosition(55, 155);
	m_pLabelBuyJettonMin->setTextColor(Color4B(0x5d, 0xd1, 0x83, 255));
	m_pLayerBuyJetton->addChild(m_pLabelBuyJettonMin);

	m_pLabelBuyJettonMax = CreateLabelMSYH(20, "", TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
	m_pLabelBuyJettonMax->setAnchorPoint(Vec2(1, 0));
	m_pLabelBuyJettonMax->setPosition(520, 155);
	m_pLabelBuyJettonMax->setTextColor(Color4B(0x5d, 0xd1, 0x83, 255));
	m_pLayerBuyJetton->addChild(m_pLabelBuyJettonMax);

	auto pMenuItem0 = MenuItemSpriteWithLabel::create("quickshop/btautobuyjettonunsel.png", 2, nullptr);
	auto pMenuItem1 = MenuItemSpriteWithLabel::create("quickshop/btautobuyjettonsel.png", 2, nullptr);
	auto pLabel0 = CreateLabelMSYH(24);
	auto pLabel1 = CreateLabelMSYH(24);
	pMenuItem0->SetEnabledColor(Color4B(0xff, 0xd2, 0x0, 0xff));
	pMenuItem1->SetEnabledColor(Color4B(0xff, 0xd2, 0x0, 0xff));
	SetLabelString(pLabel0, "筹码不足时自动补充到最大买入");
	SetLabelString(pLabel1, "筹码不足时自动补充到最大买入");
	pMenuItem0->SetLabel(pLabel0);
	pMenuItem1->SetLabel(pLabel1);
	//pLabel0->setPositionX(160+6);
	//pLabel1->setPositionX(160+6);
	m_pMenuItemAutoBuyJetton2Max = MenuItemToggle::create(pMenuItem0);
	m_pMenuItemAutoBuyJetton2Max->addSubItem(pMenuItem1);
	m_pMenuItemAutoBuyJetton2Max->setPosition(324, 111);

	m_pMenuItemBuyTools = MenuItemSpriteWithLabel::create("quickshop/btbuy_mid.png", 3, CC_CALLBACK_1(QuickShop::onMenuItemSelected, this));
	m_pMenuItemBuyJettons = MenuItemSpriteWithLabel::create("quickshop/btbuy_right.png", 3, CC_CALLBACK_1(QuickShop::onMenuItemSelected, this));
    m_pMenuItemBuyDiamond = MenuItemSpriteWithLabel::create("quickshop/btbuy_left.png", 3, CC_CALLBACK_1(QuickShop::onMenuItemSelected, this));
	m_pMenuItemBuyTools->setPosition(300, 420);
	m_pMenuItemBuyJettons->setPosition(464, 420); 
    m_pMenuItemBuyDiamond->setPosition(136, 420);
	auto pLabel = CreateLabelMSYHBD(26);
	SetLabelString(pLabel, "购买道具");
	m_pMenuItemBuyTools->SetLabel(pLabel);
	m_pMenuItemBuyTools->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
	m_pMenuItemBuyTools->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
	//m_pMenuItemBuyTools->setEnabled(false);

	pLabel = CreateLabelMSYHBD(26);
	SetLabelString(pLabel, "补充筹码");
	m_pMenuItemBuyJettons->SetLabel(pLabel);
	//pLabel->setPosition(pLabel->getPositionX(), pLabel->getPositionY() + 3);
	m_pMenuItemBuyJettons->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
	m_pMenuItemBuyJettons->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
	m_pMenuItemBuyJettons->setEnabled(false);

    pLabel = CreateLabelMSYHBD(26);
    SetLabelString(pLabel, "购买钻石");
    m_pMenuItemBuyDiamond->SetLabel(pLabel);
    //pLabel->setPosition(pLabel->getPositionX(), pLabel->getPositionY() + 3);
    m_pMenuItemBuyDiamond->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
    m_pMenuItemBuyDiamond->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));
    //m_pMenuItemBuyDiamond->setEnabled(false);

	//m_pMenuItemOk = MenuItemSpriteWithLabel::create("quickshop/btok.png", 3, CC_CALLBACK_1(QuickShop::onMenuItemSelected, this));
    m_pMenuItemOk = MenuItemScale9SpriteWithLabel::create(0, Size(204, 74), CC_CALLBACK_1(QuickShop::onMenuItemSelected, this));
	pLabel = CreateLabelMSYHBD(30);
	SetLabelString(pLabel, "确定");
	m_pMenuItemOk->SetLabel(pLabel);
	m_pMenuItemOk->setPosition(305, 54);
	m_pMenuItemOk->SetEnabledColor(Color4B(255, 255, 255, 255));

	//m_pMenuItemClose = MenuItemSpriteWithLabel::create("quickshop/btquickshopclose.png", 3, CC_CALLBACK_1(QuickShop::onMenuItemSelected, this));
	//m_pMenuItemClose->setPosition(543, 424);
	auto pMenu = Menu::create(m_pMenuItemOk, m_pMenuItemAutoBuyJetton2Max, /*m_pMenuItemClose, */nullptr);
	pMenu->setPosition(Vec2::ZERO);
	m_pLayerBuyJetton->addChild(pMenu);

	pMenu = Menu::create(m_pMenuItemBuyTools, m_pMenuItemBuyJettons, m_pMenuItemBuyDiamond, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu);

    g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskAddJetton);
    m_pPaymentMethod = nullptr;
	return true;
}

void QuickShop::UpdateJetton(double dAllCash, double dBuyJetton, double dBuyJettonMin, double dBuyJettonMax, bool bAutoBuy)
{
	std::string str("现有德州币: ");
	str += do_fraction(dAllCash);
	SetLabelString(m_pLabelAllCash, str);

	str = "最小买入: ";
	str += do_fraction(dBuyJettonMin);
	SetLabelString(m_pLabelBuyJettonMin, str);

	str = "最大买入: ";
	str += do_fraction(dBuyJettonMax);
	SetLabelString(m_pLabelBuyJettonMax, str);

	m_pSliderBuyJetton->setMinimumValue(dBuyJettonMin);
	m_pSliderBuyJetton->setMaximumValue(dBuyJettonMax);
	m_pSliderBuyJetton->setValue(dBuyJetton);

	m_pMenuItemAutoBuyJetton2Max->setSelectedIndex(bAutoBuy ? 0 : 1);

	m_dAutoSupply = dBuyJetton;
	m_bAutoSupply = bAutoBuy;
}

void QuickShop::onSliderChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent)
{
	double dValue = m_pSliderBuyJetton->getValue();
	m_pNumBuyJetton->UpdateNumber(dValue);
}

void QuickShop::onMenuItemSelected(cocos2d::Ref *pItem)
{
	if (pItem == m_pMenuItemOk)
	{
		if (pItem == m_pMenuItemOk)
		{
			m_dAutoSupply = m_pSliderBuyJetton->getValue();
			m_bAutoSupply = m_pMenuItemAutoBuyJetton2Max->getSelectedIndex() == 0;
			Json::Value param;
			param["ifAutoGetMax"] = m_bAutoSupply ? 1 : 0;
			param["money"] = m_dAutoSupply;
			SendClientRequest(ClientRequest::kSetAutoSupply, param);
			g_globalTableConfig.m_uAutoSupplyToMaxMoney = m_bAutoSupply ? 1 : 0;
			g_globalTableConfig.m_dAutoSupplyMoney = m_dAutoSupply;
		}

		setVisible(false);
	}
	else if (pItem == m_pMenuItemBuyTools)
	{
		m_pMenuItemBuyTools->setEnabled(false);
		m_pMenuItemBuyJettons->setEnabled(true);
        m_pMenuItemBuyDiamond->setEnabled(true);

		m_pLayerBuyJetton->setVisible(false);
		m_pLayerBuyTool->setVisible(true);
        m_pLayerBuyDiamond->setVisible(false);
	}
	else if (pItem == m_pMenuItemBuyJettons)
	{
		m_pSpriteBgBuyJetton->setVisible(true);
		m_pMenuItemBuyTools->setEnabled(true);
		m_pMenuItemBuyJettons->setEnabled(false);
        m_pMenuItemBuyDiamond->setEnabled(true);

		m_pLayerBuyJetton->setVisible(true);
		m_pLayerBuyTool->setVisible(false);
        m_pLayerBuyDiamond->setVisible(false);
	}
    else if (pItem == m_pMenuItemBuyDiamond)
    {
        m_pMenuItemBuyTools->setEnabled(true);
        m_pMenuItemBuyJettons->setEnabled(true);
        m_pMenuItemBuyDiamond->setEnabled(false);

        m_pLayerBuyJetton->setVisible(false);
        m_pLayerBuyTool->setVisible(false);
        m_pLayerBuyDiamond->setVisible(true);
    }
}

void QuickShop::onMenuBuyDiamond(const ItemInShopInfo &itemInfo)
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

















    //if (m_pPaymentMethod == nullptr)
    //{
    //    auto bgSz = this->getContentSize();
    //    m_pPaymentMethod = PaymentMethod::create();
    //    m_pPaymentMethod->ignoreAnchorPointForPosition(false);
    //    m_pPaymentMethod->setAnchorPoint(Vec2(0.5, 0.5));
    //    m_pPaymentMethod->setPosition(bgSz.width/2, bgSz.height/2);
    //    this->addChild(m_pPaymentMethod, 103);
    //}
    //m_pPaymentMethod->setVisible(true);

    //char buff[32] = {0};
    //GoodsInfo goodsInfo;
    //sprintf(buff, "%d钻石", itemInfo.m_uDiamondNum);
    //goodsInfo.goodsName = buff;
    //sprintf(buff, "￥%.0f", itemInfo.m_price);
    //goodsInfo.totalMoney = buff;
    //goodsInfo.Id = itemInfo.m_uID;
    //goodsInfo.num = itemInfo.m_uDiamondNum;
    //sprintf(buff, "￥%.0f / %d个", itemInfo.m_price, itemInfo.m_uDiamondNum);
    //goodsInfo.onePrice = buff;
    //goodsInfo.Price = itemInfo.m_price;
    //m_pPaymentMethod->UpdateGoodTypeInfo(goodsInfo);
}

void QuickShop::popBuyAndPay()
{
    if (m_BuyGoods == nullptr)
    {
        m_BuyGoods = BuyGoods::create();
        this->addChild(m_BuyGoods, 100);
        Size szBuy = this->getContentSize();
        m_BuyGoods->ignoreAnchorPointForPosition(false);
        m_BuyGoods->setAnchorPoint(Vec2(0.5, 0.5));
        m_BuyGoods->setPosition(szBuy.width/2, szBuy.height/2);
        m_BuyGoods->setQuickShopOpt(std::bind(&QuickShop::switchToDiamond, this));
    }
    m_BuyGoods->setVisible(true);
}

void QuickShop::switchToDiamond()
{
    onMenuItemSelected(m_pMenuItemBuyDiamond);
}

void QuickShop::onSetVisible(bool bVisible)
{
	m_pSliderBuyJetton->setValue(m_dAutoSupply);
	m_pMenuItemAutoBuyJetton2Max->setSelectedIndex(m_bAutoSupply ? 0 : 1);
}

void QuickShop::setVisible(bool bVisible)
{
	
	if (bVisible)
	{
		callShowAction() ;
		m_pSliderBuyJetton->setEnabled(true) ;
		//GameDialog::setVisible(bVisible);
		//Tools::callScaleAction(this) ;
		/*	auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0.618);GameDialog::setVisible(true);}), actScale, nullptr));*/
	}
	else
	{
		callHideAction() ;
		m_pSliderBuyJetton->setEnabled(false) ;
		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
	}
}

void QuickShop::OnHideUi(bool bVisible)
{
    if (m_pPaymentMethod != nullptr)
	    m_pPaymentMethod->setVisible(bVisible);
}