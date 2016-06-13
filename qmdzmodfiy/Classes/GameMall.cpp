#include "GameMall.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "Util.h"
#include "LineLabel.h"
#include "HttpSprite.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"
#include "FrameSpriteButton.h"
#include "FrameScale9SpriteButton.h"
#include "GameCommon.h"
#include "BuyAndPay.h"
#include "Tools.h"
#include "GameConfig.h"
#include "GameData.h"
#include "PlatformSpec.h"
#include "GameBuyDiamond.h"

USING_NS_CC;


enum 
{
	ItemShopScrollW = 1044,
	ItemShopScrollH = 460,
    ExchangeScrollW = 900,
    ExchangeScrollH= 465,
    HeadShopScrollW = 1044,
    HeadShopScrollH = 460,
    ScrollViewW = 900,
    ScrollViewH = 550
};

enum BgItem
{
	ListBgItemW = 900,
	ListBgItemH = 162
};


bool GameMall::init()
{
	if(!GameDialog::init())
	{
		return false;
	}

	//载入布局
	auto rootNode = CSLoader::createNode("res/ui/Gamemall.csb");
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	rootNode->setContentSize(frameSize); 
	ui::Helper::doLayout(rootNode);  
	addChild(rootNode);
	char szButton[64] = {0};
	m_uCurSelArea = 0;

	// 按钮
	m_pBtclose = static_cast<Button*>(Tools::getWidgetByName(rootNode,"Button_close"));
	m_pBtclose->addTouchEventListener(this,toucheventselector(GameMall::onMenuItemClose)) ;
    //左右分割线
    m_pImageDivision = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_division"));
    m_posDivision = m_pImageDivision->getPosition();

	for (int i = 0; i < 5; i++)
	{
		sprintf(szButton, "Button_mall%d",i);
		m_pMenuItemSwitchArea[i]= static_cast<Button*>(Tools::getWidgetByName(rootNode,szButton));
		sprintf(szButton, "Textmall_%d",i);
		m_pTextLabel[i] = static_cast<Text*>(Tools::getWidgetByName(rootNode,szButton));
		m_pTextLabel[i]->setTextColor(Color4B(0x37,0xa4,0x5c,0xff));
		m_pMenuItemSwitchArea[i]->addTouchEventListener(this,toucheventselector(GameMall::onMenuItemSwitchArea)) ;
		m_pMenuItemSwitchArea[i]->setBright(m_uCurSelArea != i);
	}
    m_pTextLabel[0]->setTextColor(Color4B(0xff,0xff,0xff, 0xff));

#if OpenItemShop
	const char * bgview[5] = {
		"Panel_Shop", "Panel_OpenVip", "Panel_itemshop", "Panel_headshop", "Panel_Change"
	};
#else
	const char * bgview[5] = {
		"Panel_Shop", "Panel_OpenVip", "Panel_Change", "Panel_Change", "Panel_Change"
	};
#endif
	// 按钮
	for (int i = 0; i < 5; i++)
	{
		m_pSpriteBg[i]= static_cast<ImageView*>(Tools::getWidgetByName(rootNode,bgview[i]));
		m_pSpriteBg[i]->addTouchEventListener(this,toucheventselector(GameMall::onMenuItemSwitchArea));
		m_pSpriteBg[i]->setVisible(m_uCurSelArea == i);
	}

    //购买钻石
    auto imageTitleNode = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "Image_title_bg"));
    m_btnBuyDiamond = static_cast<ui::Button*>(Tools::getWidgetByName(imageTitleNode, "Button_add_diamond"));
    m_btnBuyDiamond->addTouchEventListener(this,toucheventselector(GameMall::onMenuItemBuyDiamond));
    m_textCurDiamond = static_cast<ui::Text*>(Tools::getWidgetByName(imageTitleNode, "Text_cur_diamond"));
    char szTxtBuff[64] = {0};
    sprintf(szTxtBuff, "%s", FormatCash4(g_globalMyData.GetDiamond()).c_str());
    setUITextString(m_textCurDiamond, szTxtBuff);

    //m_imageFrameExchange = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_exchange_frame1"));
    //m_imageFrameHead = static_cast<ImageView*>(Tools::getWidgetByName(rootNode,"Image_headshop_frame1"));

    m_pBuyGoods = nullptr;
    m_pGameBuyDiamond = nullptr;
	m_pScrollViewVIPInfo = nullptr;
	m_pDlgExchangeTips = nullptr;
	m_pDlgAddrSet = nullptr;
    m_pDlgExchangeTips = nullptr;
    m_layerColor = nullptr;
    m_scale9SpriteShadow = nullptr;
    m_spriteFrame = nullptr;
    m_spriteFrameHead = nullptr;
    m_uTipsType = 0;
    m_bModifyInfo = false;
    m_iCurExchangeId = 0;
    m_iCurConsume = 0;
    m_iCurExchangeType = 1;

	/**********************************************宝石购买修改******************************************************/
    auto diamondMenu = Menu::create();
    diamondMenu->setPosition(Vec2::ZERO);
    m_pSpriteBg[0]->addChild(diamondMenu);
	for(int i=0; i<6; i++)
    {
		unsigned uX = i % 3;
		unsigned uY = i / 3;
		auto icon = HttpSprite::create("mall/defaultfruiticon.png", Size(198, 219));
		icon->ignoreAnchorPointForPosition(false);
		icon->setAnchorPoint(Vec2(0.5, 0.5));
		icon->setPosition(140+uX*300, 427-uY*245);
		SetHttpSpriteURL(icon, g_globalMallInfo.m_fruitInfo[i].m_strIconUrl);
		m_pSpriteBg[0]->addChild(icon);
		//点击宝石图片
		auto btDetail = LayerButton::create(true, true);
		btDetail->SetHitCB(CC_CALLBACK_1(GameMall::onMenuItemBuyFruit, this, i)); 
		btDetail->setContentSize(icon->getContentSize());
		icon->addChild(btDetail);

		auto label = LineLabel::create();
		label->Appand(24, "fzltbd.ttf", "送", Color4B::WHITE);
		label->Appand(24, "fzltbd.ttf", FormatCash(g_globalMallInfo.m_fruitInfo[i].m_dReturnCash), Color4B(0xff, 0xd2, 0, 0xff));
		label->Appand(24, "fzltbd.ttf", "德州币", Color4B::WHITE);
		label->setPosition(99, 175);
		icon->addChild(label);

        auto bmenuBtn= FrameScale9SpriteButton::createWithScale9Sprite(0, Size(174, 54), false, true);
        bmenuBtn->SetHitCB(CC_CALLBACK_1(GameMall::onMenuItemBuyFruit, this, i));
		bmenuBtn->setPosition(99, 20);
        icon->addChild(bmenuBtn); 
		char szPrice[64] = {0};
		sprintf(szPrice, "%s",  FormatCash(g_globalMallInfo.m_fruitInfo[i].m_dPrice).c_str());
		auto labelPrice = CreateLabelMSYHBD(24, szPrice);
		bmenuBtn->addChild(labelPrice);
        labelPrice->ignoreAnchorPointForPosition(false);
        labelPrice->setAnchorPoint(Vec2(0, 0.5));
        labelPrice->setPosition(-6, 0);
        auto spriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", -30, 0, bmenuBtn, 0);
        spriteDiamond->setScale(0.8);

	}
	/**********************************************VIP数据******************************************************/
	unsigned uX = 0;
	unsigned uXDate = 232;
	unsigned uY = 0;
	int iLineH = -46;
	unsigned uRowW = 141;
	int iFontSize = 20;
	//char szTxtBuff[64] = {0};
	for(int i=0; i<5; i++)
    {
		uY = 380;	
		if (m_pScrollViewVIPInfo == nullptr)
		{
			m_pScrollViewVIPInfo = extension::ScrollView::create(Size(880, 368));
			m_pScrollViewVIPInfo->setContentSize(Size(880, 368));
			//m_pScrollViewVIPInfo->setContentOffset(Vec2(0, 0));
            m_pScrollViewVIPInfo->setBounceable(false);
			m_pScrollViewVIPInfo->setPosition(0, 87);
			m_pScrollViewVIPInfo->setDirection(extension::ScrollView::Direction::VERTICAL);
			m_pSpriteBg[1]->addChild(m_pScrollViewVIPInfo);

			const char *szTxt[8] = {
				"立即赠送德州币", "立即赠送经验", "每日登录返还", "幸运值", 
				"贵族表情包", "贵族魔法表情", "牌局记录", "私密开房"
			};	
			uX = 12;
			uY = 368+iLineH/2;
			for (int i = 0; i < 8; i++)
			{
                //阴影背景
                if ((i+1)%2 == 0)
                {
                    auto vipBgSprite = ui::Scale9Sprite::create("mall/shop_vipbg2.png", Rect(0,0,141,46),Rect(10,10,121,26));
                    vipBgSprite->setPreferredSize(Size(880,46));
                    vipBgSprite->setAnchorPoint(Vec2(0, 0.5));
                    vipBgSprite->setPosition(0, uY);
                    m_pScrollViewVIPInfo->addChild(vipBgSprite);
                }

				auto label = CreateLabelMSYH(22, szTxt[i]);
				label->setAnchorPoint(Vec2(0, 0.5));
				label->setPosition(uX, uY);
				m_pScrollViewVIPInfo->addChild(label);
				uY += iLineH;
			}
            float divisionfX = 168;
            for (int i= 0; i<5; i++)
            {
                CreateNode1AndPosAddTo<Sprite>("mall/vip_division.png", divisionfX, 368/2, m_pScrollViewVIPInfo, 4);
                divisionfX += 141;
            }
		}
		iFontSize = 22;
		uY = 368-iFontSize;
		auto label = CreateLabelMSYH(iFontSize, FormatCash(g_globalMallInfo.m_vipInfo[i].m_dAddLeBi));
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		sprintf(szTxtBuff, "%d点", g_globalMallInfo.m_vipInfo[i].m_uAddJingYan);
		label = CreateLabelMSYH(iFontSize, szTxtBuff);
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		label = CreateLabelMSYH(iFontSize, FormatCash(g_globalMallInfo.m_vipInfo[i].m_dDengLuFanHuan));
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		sprintf(szTxtBuff, "+%d点", g_globalMallInfo.m_vipInfo[i].m_uAddMeiLi);
		label = CreateLabelMSYH(iFontSize, szTxtBuff);
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		sprintf(szTxtBuff, "%s", g_globalMallInfo.m_vipInfo[i].m_bOpenGuiZuBiaoQing ? "开启" : "关闭");
		label = CreateLabelMSYH(iFontSize, szTxtBuff);
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		sprintf(szTxtBuff, "%s", g_globalMallInfo.m_vipInfo[i].m_bOpenGuiZuMoFaBiaoQing ? "开启" : "关闭");
		label = CreateLabelMSYH(iFontSize, szTxtBuff);
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		sprintf(szTxtBuff, "%d手", g_globalMallInfo.m_vipInfo[i].m_uJiLuPaiJu);
		label = CreateLabelMSYH(iFontSize, szTxtBuff);
		label->setPosition(uXDate, uY);
		m_pScrollViewVIPInfo->addChild(label);
		uY += iLineH;
		if(i < 4)
		{
			sprintf(szTxtBuff, "%s", g_globalMallInfo.m_vipInfo[i].m_bOpenSiMiKaiFang ? "开启" : "关闭");
			label = CreateLabelMSYH(iFontSize, szTxtBuff);
			label->setPosition(uXDate, uY);
			m_pScrollViewVIPInfo->addChild(label);
		}else {
			if(g_globalMallInfo.m_vipInfo[i].m_bOpenSiMiKaiFang)
			{
// 				if(g_globalMallInfo.m_vipInfo[i].m_bOpenZiDingYiKaiFang){
// 					sprintf(szTxtBuff, "%s", "开启");
// 				}else{
// 					sprintf(szTxtBuff, "%s", "开启");
// 				}
                sprintf(szTxtBuff, "%s", "开启");
			}else{
				sprintf(szTxtBuff, "%s", "关闭");
			}
			label = CreateLabelMSYH(iFontSize, szTxtBuff);
			label->setPosition(uXDate, uY);
			m_pScrollViewVIPInfo->addChild(label);
		}
		uY += iLineH;
		uXDate += uRowW;	
	}
	/**********************************************VIP图标******************************************************/
	uX = 232;
	uY = 477;
	for (int i = 0; i < 5; i++)
	{
		auto vipflag = FrameSprite::create("vip.png", 6, 1, i);
		vipflag->setPosition(uX, uY);
		m_pSpriteBg[1]->addChild(vipflag);
		uX += uRowW;
	}
	/**********************************************VIP购买******************************************************/
	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	m_pSpriteBg[1]->addChild(menu);
	uX = 232;
	uY = 58;
	iFontSize = 20;
	for (int i = 0; i < 5; i++)
	{
		//m_pMenuitem[i] = MenuItemSpriteWithLabel::create("mall/btbuyvip.png", 3, CC_CALLBACK_1(GameMall::onMenuItemBuyVIP, this, i));
        m_pMenuitem[i] = MenuItemScale9SpriteWithLabel::create(1, Size(132, 56), CC_CALLBACK_1(GameMall::onMenuItemBuyVIP, this, i));
		char szTxt2[64] = {0};
		sprintf(szTxt2, "%s", FormatCash(g_globalMallInfo.m_vipInfo[i].m_dPrice).c_str());
		auto label = CreateLabelMSYHBD(26, szTxt2);
		m_pMenuitem[i]->SetLabel(label);
        label->ignoreAnchorPointForPosition(false);
        label->setAnchorPoint(Vec2(0,0.5));
        label->setPosition(50, 56/2);
		m_pMenuitem[i]->setPosition(uX, uY);
        auto spriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", 30, 56/2, m_pMenuitem[i], 0);
        spriteDiamond->setScale(0.8);
		menu->addChild(m_pMenuitem[i]);
		m_pMenuItemBuyVIP.push_back(m_pMenuitem[i]);
		uX += uRowW;
	}
/**********************************************VIP滚动之外的选项******************************************************/
	m_pLabelVIPStatus = CreateLabelMSYH(24, "");
	m_pLabelVIPStatus->setAnchorPoint(Vec2(0, 0.5));
	if (g_globalMyData.GetVipLevel() == 0)
	{
		SetLabelString(m_pLabelVIPStatus, "您还没有开通VIP");
	}
	else
	{
		sprintf(szTxtBuff, "您当前为%d级VIP", g_globalMyData.GetVipLevel());
		SetLabelString(m_pLabelVIPStatus, szTxtBuff);
	}
	m_pLabelVIPStatus->setPosition(10, 523);
	m_pLabelVIPStatus->setTextColor(Color4B(0x64, 0xcd, 0x0a, 255));
	m_pSpriteBg[1]->addChild(m_pLabelVIPStatus);

	auto label = CreateLabelMSYH(22, "VIP等级");
    label->setAnchorPoint(Vec2(0, 0.5));
	label->setPosition(10, 477);
	label->setTextColor(Color4B(0xad,0xf0,0xbd,0xff));
	m_pSpriteBg[1]->addChild(label);

	label = CreateLabelMSYHBD(26, "立即开通");
	label->setPosition(75, 57);
	label->setTextColor(Color4B(0x64, 0xcd, 0x0a, 0xff));
	m_pSpriteBg[1]->addChild(label);

    label = CreateLabelMSYH(21, "会员有效期31天");
    label->setPosition(80, 22);
    label->setTextColor(Color4B(0x2c, 0xa2, 0x5c, 0xff));
    m_pSpriteBg[1]->addChild(label);

	/************************************************************************/
	/* 道具商城                                                                     */
	/************************************************************************/
    //auto frameBgSz = m_pSpriteBg[4]->getContentSize();
    m_scrollExchange = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollViewW, ScrollViewH), 0, 0, m_pSpriteBg[4], 0);
    m_scrollExchange->setContentSize(Size(ScrollViewW, ScrollViewH));
    m_scrollExchange->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_scrollExchange->ignoreAnchorPointForPosition(false);
    m_scrollExchange->setAnchorPoint(Vec2(0, 0));
    m_scrollExchange->setPosition(-20, 0);

    m_pMenuItemAddr = MenuItemSpriteWithLabel::create("mall/adressbtn.png", 2, CC_CALLBACK_1(GameMall::onMenuItemSaveAddr, this));
    m_pMenuItemAddr->setAnchorPoint(Vec2(0,1));
    m_pMenuItemAddr->SetLabel(CreateLabelMSYHBD(22, "收货地址"));
    m_pMenuItemAddr->setPosition(-4, 552);
    auto pMenu = Menu::create(m_pMenuItemAddr, nullptr);
    pMenu->setPosition(Vec2::ZERO);
    m_pSpriteBg[4]->addChild(pMenu, 10);
    
    //头像商城
    m_scrollHeadShop = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollViewW, ScrollViewH), 0, 0, m_pSpriteBg[3], 0);
    m_scrollHeadShop->setContentSize(Size(ScrollViewW, ScrollViewH));
    m_scrollHeadShop->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_scrollHeadShop->ignoreAnchorPointForPosition(false);
    m_scrollHeadShop->setAnchorPoint(Vec2(0, 0));
    m_scrollHeadShop->setPosition(-20, 0);

	initItemShopLayer() ;
    initHeadShopLayer(); 
    initExChangeLayer();

    g_globalMyData.WaitChange(WAITCASHCHANGE | WAITDIAMONDCHANGE, std::bind(&GameMall::onUpdateMyData, this, std::placeholders::_1));
	return true;
}

void GameMall::onUpdateMyData(unsigned uType)
{
    if (uType == WAITDIAMONDCHANGE)
    {
        char szTxtBuff[32] = {0};
        sprintf(szTxtBuff, "%s", FormatCash4(g_globalMyData.GetDiamond()).c_str());
        setUITextString(m_textCurDiamond, szTxtBuff);
    }
}

void GameMall::SwitchToArea(unsigned uArea)
{
    m_pMenuItemAddr->setVisible(false);
	if (m_uCurSelArea != uArea)
	{
		m_pMenuItemSwitchArea[m_uCurSelArea]->setBright(true);
		m_pSpriteBg[m_uCurSelArea]->setVisible(false);
        m_pTextLabel[m_uCurSelArea]->setTextColor(Color4B(0x37,0xa4,0x5c,0xff));
		m_uCurSelArea = uArea;
		m_pMenuItemSwitchArea[m_uCurSelArea]->setBright(false);
		m_pSpriteBg[m_uCurSelArea]->setVisible(true);
		m_pTextLabel[m_uCurSelArea]->setTextColor(Color4B(0xff,0xff,0xff, 0xff));
	}
    else
    {
        //修改：牌桌中通过 VIP数据 连续跳转到商城界面时字体显示不正确
        m_pTextLabel[m_uCurSelArea]->setTextColor(Color4B(0xff,0xff,0xff, 0xff));
    }
    if (uArea == 4)//兑换商城
    {
		
        m_pMenuItemAddr->setVisible(true);
    }
}

void GameMall::popDiamondBuy()
{
    if (m_pGameBuyDiamond == nullptr)
    {
        m_pGameBuyDiamond = GameBuyDiamond::create();
        m_pGameBuyDiamond->ignoreAnchorPointForPosition(false);
        m_pGameBuyDiamond->setAnchorPoint(Vec2(0.5, 0.5));
        auto bgSz = this->getContentSize();
        m_pGameBuyDiamond->setPosition(bgSz.width/2, bgSz.height/2);
        this->addChild(m_pGameBuyDiamond, 2);
    }
    m_pGameBuyDiamond->setVisible(true);
}

void GameMall::popBuyAndPay()
{
    //购买&支付
    if (m_pBuyGoods == nullptr)
    {
        m_pBuyGoods = BuyGoods::create();
        Size mallSz = this->getContentSize();
        m_pBuyGoods->ignoreAnchorPointForPosition(false);
        m_pBuyGoods->setAnchorPoint(Vec2(0.5, 0.5));
        m_pBuyGoods->setPosition(mallSz.width/2, mallSz.height/2);
        this->addChild(m_pBuyGoods, 100);
    }
    m_pBuyGoods->setVisible(true);
}

void GameMall::onMenuItemBuyDiamond(cocos2d::Ref* pMenuItem, cocos2d::ui::TouchEventType type)
{
    if (type == TouchEventType::TOUCH_EVENT_ENDED)
    {
        if (pMenuItem == m_btnBuyDiamond)
        {
            popDiamondBuy();
        }
    }
}

void GameMall::onMenuItemSwitchArea(cocos2d::Ref* pMenuItem, cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		for (unsigned i = 0; i < 5; i++)
		{
			if (pMenuItem == m_pMenuItemSwitchArea[i])
			{
				SwitchToArea(i);
//  				if(i == 2)
//  				{
//  					m_pTextBg[0]->setVisible(false);
//  					m_pTextBg[1]->setVisible(false);
//  					m_pTextBg[2]->setVisible(false);
//  				}
//  #if !OpenItemShop
//  				if(i == 3)
//  				{
//  					m_pTextBg[0]->setVisible(false);
//  					m_pTextBg[1]->setVisible(false);
//  					m_pTextBg[2]->setVisible(false);
//  				}
//  #endif
//                 if(i == 4)
//                 {
//                     m_pTextBg[0]->setVisible(false);
//                     m_pTextBg[1]->setVisible(false);
//                     m_pTextBg[2]->setVisible(true);
//                 }
			}
            else
            {
                m_pTextLabel[i]->setTextColor(Color4B(0x37,0xa4,0x5c,0xff));
            }
		}
	}
}

void GameMall::onMenuItemClose(cocos2d::Ref* pMenuItem, cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
        if (m_uCurSelArea)
            m_pTextLabel[m_uCurSelArea]->setTextColor(Color4B(0x37,0xa4,0x5c,0xff));
		setVisible(false);
	}
}

void GameMall::onMenuItemBuyFruit(cocos2d::Ref* pSend, unsigned idx)
{
    popBuyAndPay();
    m_pBuyGoods->UpdateGoodTypeInfo(0, idx, true);
}

void GameMall::onMenuItemBuyVIP(cocos2d::Ref* pSend, unsigned idx)
{
    popBuyAndPay();
    m_pBuyGoods->UpdateGoodTypeInfo(1, idx, true);
}

void GameMall::PopAddrSet()
{
	if (m_pDlgAddrSet == nullptr)
	{
        auto bgSz = this->getContentSize();
		m_pDlgAddrSet = GameDialog::create("vipprivatenormal/bdefinebg.webp", false);
        auto addrSetSz = m_pDlgAddrSet->getContentSize();
		m_pDlgAddrSet->setPosition((bgSz.width-addrSetSz.width)/2, (bgSz.height-addrSetSz.height)/2);
		m_pDlgAddrSet->setVisible(false);
		addChild(m_pDlgAddrSet, 1);

		m_pLabelAddrSetTitle = CreateLabelMSYH(30, g_globalMyAddr.m_bHasAddr ? "查看并修改您的信息" : "请完善您的信息");
		m_pLabelAddrSetTitle->setPosition(addrSetSz.width/2, addrSetSz.height-65);
		m_pLabelAddrSetTitle->setTextColor(Color4B(0xff, 0xd2, 0x00, 0xff));
		m_pDlgAddrSet->addChild(m_pLabelAddrSetTitle);

		Vec2 anchorTxt(1, 0);
		Color4B clrTxt(255, 255, 255, 255);
		unsigned uX = 200, uY = 450, uLineH = 70;
        const char *szFlag[] = {"*","*","","*",""};
		const char *szPre[] = {
			"收货人：",
			"手机号码：",
			"QQ号码：",
			"收货地址：",
			"电子邮箱："
		};
		const char *szPlaceHold[] = {
			"姓名",
			"手机号码",
			"QQ号码",
			"请填写省份、城市、区（县）及街道门牌信息",
			"邮箱地址"
		};
		Size szEdt = Size(480, 28);
		for (int i = 0; i < 5; i++)
		{
            if (i%2 == 0)
            {
                auto addrInfoBg = LayerColor::create(Color4B(0x03,0x3f,0x1e,0xff));
                addrInfoBg->setContentSize(Size(addrSetSz.width-40, 70));
                addrInfoBg->ignoreAnchorPointForPosition(false);
                addrInfoBg->setAnchorPoint(Vec2(0.5, 0.5));
                addrInfoBg->setPosition(addrSetSz.width/2, uY+12);
                m_pDlgAddrSet->addChild(addrInfoBg);
            }
            auto labelTitle = LineLabel::create();
            labelTitle->ignoreAnchorPointForPosition(false);
            labelTitle->setAnchorPoint(Vec2(1,0));
            labelTitle->setPosition(uX, uY);
            labelTitle->Appand(24,"fzlt.ttf",szFlag[i],Color4B(0xff, 0xd2, 0x00, 0xff));
            labelTitle->Appand(24,"fzlt.ttf",szPre[i],Color4B::WHITE);
            m_pDlgAddrSet->addChild(labelTitle);

			//CreateLabelMSYHAndAnchorPosClrAddTo(24, szPre[i], Vec2(1, 0), uX, uY, clrTxt, m_pDlgAddrSet, 0);
            m_labelAddrInfo[i] = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0,0), uX+15, uY, clrTxt, m_pDlgAddrSet, 0);
            m_labelEditbox[i] = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0,0), uX+15, uY, Color4B(0x34,0x88,0x50,0xff), m_pDlgAddrSet, 0);
			//m_pEdtAddrItem[i] = ui::EditBox::create(szEdt, ui::Scale9Sprite::create("mall/bgedit.png"));
			///*	m_pEdtAddrItem[i]->setFont("fzlt.ttf", 24);
			//m_pEdtAddrItem[i]->setPlaceHolder(szPlaceHold[i]);
			//m_pEdtAddrItem[i]->setFontColor(Color3B(0x34, 0x88, 0x50));
			//m_pEdtAddrItem[i]->setPlaceholderFontColor(Color3B(0x37, 0x85, 0x51));
			//m_pEdtAddrItem[i]->setPosition(Vec2(178 + 260, uY + 14));
			//m_pEdtAddrItem[i]->setReturnType(ui::EditBox::KeyboardReturnType::DONE);*/
   //         m_pEdtAddrItem[i]->setDelegate(this);



			m_pEdtAddrItem[i] = CreateNode2AndPosAddTo<ui::EditBox>(Size(480, 70), ui::Scale9Sprite::create("mall/bgedit.png") , 178 + 260, uY+14, m_pDlgAddrSet, 0);
			m_pEdtAddrItem[i]->setFont("fzlt.ttf", 24);
			m_pEdtAddrItem[i]->setFontColor(Color3B(0x34, 0x88, 0x50));
			//m_pEdtAddrItem[i]->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
			m_pEdtAddrItem[i]->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
			m_pEdtAddrItem[i]->setDelegate(this);	

	



			if (i == 1)
			{				
				m_pEdtAddrItem[i]->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
			}
            else if (i == 2)
            {
                m_pEdtAddrItem[i]->setInputMode(ui::EditBox::InputMode::NUMERIC);
            }
            else if (i == 4)
            {
                m_pEdtAddrItem[i]->setInputMode(ui::EditBox::InputMode::EMAIL_ADDRESS);
            }
			else
			{
				m_pEdtAddrItem[i]->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);				
			}

			//m_pDlgAddrSet->addChild(m_pEdtAddrItem[i]);
			uY -= uLineH;
		}
		CreateLabelMSYHAndAnchorPosClrAddTo(22, 
			"带 * 项为必填选项,请务必正确填写姓名和联系方式，\n以便获奖后我们能及时联系你。",
			Vec2(0.5, 0.5), addrSetSz.width/2, 120, Color4B(0x64, 0xcd, 0x0a, 0xff), m_pDlgAddrSet, 0);

		//m_pMenuItemSaveAddr = MenuItemSpriteWithLabel::create("mall/btsaveaddr.png", 3, CC_CALLBACK_1(GameMall::onMenuItemSaveAddr, this));
        m_pMenuItemSaveAddr = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(GameMall::onMenuItemSaveAddr, this));
		m_pMenuItemSaveAddr->SetLabel(CreateLabelMSYHBD(30, ""));
		m_pMenuItemSaveAddr->setPosition(addrSetSz.width/2, 60);
        auto pMenu = Menu::create(m_pMenuItemSaveAddr, nullptr);
        pMenu->setPosition(Vec2::ZERO);
        m_pDlgAddrSet->addChild(pMenu);
		
        m_btnBackAddr = FrameSpriteButton::createWithVertFrameSprite("res/ui/btback.png", false, true);
        m_btnBackAddr->setPosition(55, addrSetSz.height-55);
        m_btnBackAddr->SetHitCB(CC_CALLBACK_1(GameMall::onMenuItemSaveAddr, this));
        m_pDlgAddrSet->addChild(m_btnBackAddr);
	}

    m_pDlgAddrSet->setVisible(true);

    if (g_globalMyAddr.m_strXingMing != "" && g_globalMyAddr.m_strShouJi != "" && g_globalMyAddr.m_strDiZhi != "")
        g_globalMyAddr.m_bHasAddr = true;
    else 
        g_globalMyAddr.m_bHasAddr = false;

	if (g_globalMyAddr.m_bHasAddr)
	{
		SetLabelString(m_pLabelAddrSetTitle, "查看并修改您的信息");
		SetLabelString(m_pMenuItemSaveAddr->GetLabel(), "修改");
        m_bModifyInfo = true;
        m_pDlgAddrSet->setVisible(false);
	}
	else
	{
		SetLabelString(m_pLabelAddrSetTitle, "请完善您的信息");
		SetLabelString(m_pMenuItemSaveAddr->GetLabel(), "保存");
        m_bModifyInfo = false;
	}
    for (int i=0; i<5; i++)
    {
        m_labelAddrInfo[i]->setVisible(m_bModifyInfo);
        m_pEdtAddrItem[i]->setVisible(!m_bModifyInfo);
        m_labelEditbox[i]->setVisible(!m_bModifyInfo);
        SetLabelString(m_labelEditbox[i], "");
    }
    if (m_bModifyInfo)//修改个人信息
    {
        SetLabelString(m_labelAddrInfo[0], g_globalMyAddr.m_strXingMing);
        SetLabelString(m_labelAddrInfo[1], g_globalMyAddr.m_strShouJi);
        SetLabelString(m_labelAddrInfo[2], g_globalMyAddr.m_strQQ);
        SetLabelString(m_labelAddrInfo[3], g_globalMyAddr.m_strDiZhi);
        SetLabelString(m_labelAddrInfo[4], g_globalMyAddr.m_strEmail);
    }
	
}

void GameMall::popExchangeTips()
{
    if (m_pDlgExchangeTips == nullptr)
    {
        m_pDlgExchangeTips = GameLayer::create(GameData::getInstance()->getGameWidth(),GameData::getInstance()->getGameHeight(), Color4B(0x00,0x00,0x00,0xaf), true, true);
        auto layerSz = m_pDlgExchangeTips->getContentSize();
        m_pDlgExchangeTips->setPosition(0,0);
        this->addChild(m_pDlgExchangeTips, 2);
        auto spriteBg = Sprite::create("userinfo/bgdialog0.png");
        auto bgSz = spriteBg->getContentSize();
        spriteBg->setPosition(layerSz.width/2, layerSz.height/2);
        m_pDlgExchangeTips->addChild(spriteBg);
        //标题
        m_rewardTitle = Sprite::create("vipdredge/title.png");
        m_rewardTitle->setPosition(bgSz.width/2, bgSz.height+10);
        spriteBg->addChild(m_rewardTitle);
        //操作按钮
        m_pMenuExchange[0] = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(GameMall::onMenuItemTips, this));
        m_pMenuExchange[0]->SetLabel(CreateLabelMSYHBD(30, "取消"));
        m_pMenuExchange[0]->setPosition(222, 56);
        m_pMenuExchange[1] = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(GameMall::onMenuItemTips, this));
        m_pMenuExchange[1]->SetLabel(CreateLabelMSYHBD(30, "确认"));
        m_pMenuExchange[1]->setPosition(456, 56);
        auto pMenu = Menu::create(m_pMenuExchange[0], m_pMenuExchange[1], nullptr);
        pMenu->setPosition(Vec2::ZERO);
        spriteBg->addChild(pMenu);
        //友情提示
        m_labelFriendlyTips = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5, 0.5), bgSz.width/2, 120, Color4B(0x64,0xcd,0x0a,0xff), spriteBg, 0);
        m_labelContent = LineLabel::create();
        m_labelContent->setPosition(bgSz.width/2, 250);
        spriteBg->addChild(m_labelContent);
    }
    m_pDlgExchangeTips->setVisible(true);
}

void GameMall::updateExchangeTips(unsigned uType, std::string str1, std::string str2)
{
    //uType 1:兑换券过期 2:兑换成功 3:确认兑换 4:保存收货信息提示
    const char* tips[] = {
        "友情提示：过期的兑换券不能兑换奖品。",
        "友情提示：请保持手机畅通，工作人员会尽快与您联系。",
        "友情提示：兑换前要确保收货地址正确哦！\n兑换成功后工作人员将会在1-3个工作日与您联系！",
        ""
    };
    m_uTipsType = uType;
    m_rewardTitle->setVisible(false);
    m_labelContent->resetLable();
    if (str1 != "")
        m_labelContent->Appand(28, "fzlt.ttf", str1, Color4B::WHITE);
    if (str2 != "")
        m_labelContent->Appand(28, "fzlt.ttf", str2, Color4B(0xff,0xd2,0x00,0xff));
    if (uType == 1)//兑换券过期
    {
        m_pMenuExchange[0]->setVisible(false);
        m_pMenuExchange[1]->setPosition(339, 56);
        SetLabelString(m_labelFriendlyTips, tips[0]);
    }
    else if (uType == 2)//兑换成功
    {
        initExChangeLayer();//兑换成功刷兑换券
        m_pMenuExchange[0]->setVisible(false);
        m_rewardTitle->setVisible(true);
        m_pMenuExchange[1]->setPosition(339, 56);
        SetLabelString(m_labelFriendlyTips, tips[1]);
    }
    else if (uType == 3)//确认兑换
    {
        m_pMenuExchange[0]->setVisible(true);
        m_pMenuExchange[1]->setPosition(456, 56);
        SetLabelString(m_labelFriendlyTips, tips[2]);
    }
    else if (uType == 4)//填写收货信息提示
    {
        m_pMenuExchange[0]->setVisible(false);
        m_pMenuExchange[1]->setPosition(339, 56);
        SetLabelString(m_labelFriendlyTips, tips[3]);
    }
}

void GameMall::onMenuItemSaveAddr(cocos2d::Ref* pSend)
{
    if (pSend == m_btnBackAddr)//收货地址左上角返回按钮回调
    {
        m_pDlgAddrSet->setVisible(false);
    }
    else if (pSend == m_pMenuItemSaveAddr)//保存，修改用户信息
    {
        for (int i=0; i<5; i++)
        {
            m_labelAddrInfo[i]->setVisible(m_bModifyInfo);
            m_pEdtAddrItem[i]->setVisible(!m_bModifyInfo);
            m_labelEditbox[i]->setVisible(!m_bModifyInfo);
        }
        if (m_bModifyInfo)//修改个人信息
        {
            for (int i=0; i<5; i++)
            {
                m_labelAddrInfo[i]->setVisible(false);
                m_pEdtAddrItem[i]->setVisible(true);
                m_labelEditbox[i]->setVisible(true);
            }
            SetLabelString(m_labelEditbox[0], g_globalMyAddr.m_strXingMing);
            SetLabelString(m_labelEditbox[1], g_globalMyAddr.m_strShouJi);
            SetLabelString(m_labelEditbox[2], g_globalMyAddr.m_strQQ);
            SetLabelString(m_labelEditbox[3], g_globalMyAddr.m_strDiZhi);
            SetLabelString(m_labelEditbox[4], g_globalMyAddr.m_strEmail);
            SetLabelString(m_pMenuItemSaveAddr->GetLabel(), "保存");
            SetLabelString(m_pLabelAddrSetTitle, "请完善您的信息");
            m_bModifyInfo = false;
        }
        else//保存用户信息
        {
            if (!judgeEditboxInfo())
            {
                popExchangeTips();
                updateExchangeTips(4, "", "请填写正确的收货信息");
                return;
            }
            for (int i=0; i<5; i++)
            {
                m_labelAddrInfo[i]->setVisible(true);
                m_pEdtAddrItem[i]->setVisible(false);
                m_labelEditbox[i]->setVisible(false);
            }
            SetLabelString(m_labelAddrInfo[0], g_globalMyAddr.m_strXingMing);
            SetLabelString(m_labelAddrInfo[1], g_globalMyAddr.m_strShouJi);
            SetLabelString(m_labelAddrInfo[2], g_globalMyAddr.m_strQQ);
            SetLabelString(m_labelAddrInfo[3], g_globalMyAddr.m_strDiZhi);
            SetLabelString(m_labelAddrInfo[4], g_globalMyAddr.m_strEmail);
            SetLabelString(m_pLabelAddrSetTitle, "查看并修改您的信息");
            SetLabelString(m_pMenuItemSaveAddr->GetLabel(), "修改");
            m_bModifyInfo = true;
        }
    }
    else if (pSend == m_pMenuItemAddr)//收货地址按钮
    {
        PopAddrSet();
        m_pDlgAddrSet->setVisible(true);
    }
}

void GameMall::onMenuItemTips(cocos2d::Ref* pSend)
{
    if (pSend == m_pMenuExchange[0])//取消
    {
        m_pDlgExchangeTips->setVisible(false);
    }
    else if (pSend == m_pMenuExchange[1])//确认、确定
    {
        if (m_uTipsType == 1 || m_uTipsType == 3)
        {//1：过期的兑换券  2：没有过期的兑换券
            Json::Value param;
            param["consume"] = m_iCurConsume;
            param["id"] = m_iCurExchangeId;
            param["type"] = m_iCurExchangeType;
            SendClientRequest(ClientRequest::kExchangeObject, param);
            if (m_uTipsType == 1)//过期的兑换券直接隐藏提示
                m_pDlgExchangeTips->setVisible(false);
        }
        else
        {
            m_pDlgExchangeTips->setVisible(false);
        }

    }
}

void GameMall::onSetVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
	//if (!bVisible)
	//{
	//	PlayVisibleSound(bVisible);
	//}
    char szTxtBuff[64] = {0};
	if (bVisible)
	{
        for (int i=0; i<5; i++)
            m_pMenuitem[i]->setEnabled(true);
        if (g_globalMyData.GetVipLevel() == 0)
        {
            SetLabelString(m_pLabelVIPStatus, "您还没有开通VIP");
        }
        else
        {
            sprintf(szTxtBuff, "您当前为%d级VIP", g_globalMyData.GetVipLevel());
            SetLabelString(m_pLabelVIPStatus, szTxtBuff);
        }
        onCancelMenuEnable(g_globalMyData.GetVipLevel());
        sprintf(szTxtBuff, "%.0f", g_globalMyData.GetDiamond());
        setUITextString(m_textCurDiamond, szTxtBuff);
	}

}

void GameMall::onCancelMenuEnable(unsigned uVipLevel)
{
    if(uVipLevel >=5 )
        uVipLevel = 5;
	for(int i = 0;i < uVipLevel; i++)
	{
		m_pMenuitem[i]->setEnabled(false);
	}

}

void GameMall::onCancleBuyGoodsUI(bool bVisible)
{
	m_pGameBuyDiamond->OnCancleGameLayer(bVisible);
}

/************************************************************************/
/* 道具商城                                                                     */
/************************************************************************/

void GameMall::initItemShopLayer()
{

#if OpenItemShop
	//滚动视图
	Size sz = m_pSpriteBg[2]->getContentSize() ;

	m_ScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollViewW, ScrollViewH), 0, 0, m_pSpriteBg[2], 0);
	m_ScrollView->ignoreAnchorPointForPosition(false);
	m_ScrollView->setAnchorPoint(Vec2(0, 0)) ;
    m_ScrollView->setPosition(-20, 0);
	m_ScrollView->setContentSize(Size(ScrollViewW,ScrollViewH));
	m_ScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	//m_ScrollView->setSwallowsTouches(false) ;
	m_ScrollView->setVisible(true);


	if (g_globalMallInfo.m_vecShopItemList.size()<4)
	{
		//m_ScrollView->setTouchEnabled(false) ;
		int count = 4-g_globalMallInfo.m_vecShopItemList.size() ;
		for (int i=0;i<count;i++)
		{
			ItemInShopInfo item ;
			item.isNull = true ;
			g_globalMallInfo.m_vecShopItemList.push_back(item) ;
		}
	}

	updateItemSopLayer() ;
#endif

}
void GameMall::updateItemSopLayer()
{
	unsigned uCount = g_globalMallInfo.m_vecShopItemList.size();

	unsigned uItemH = ListBgItemH;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < ScrollViewH)
	{
		fRealH = ScrollViewH;
        m_ScrollView->setBounceable(false);
	}
    else
    {
        m_ScrollView->setBounceable(true);
    }
    int offset = ScrollViewH-fRealH;
    m_ScrollView->setContentOffset(Vec2(0, offset), false);

    //最上面的阴影
    auto frameSprite = CreateNode1AndPosAddTo<Sprite>("mall/shopitembg.png", 0, 0, m_ScrollView, 0);
    frameSprite->setAnchorPoint(Vec2(0, 0));
    frameSprite->setPosition(0, fRealH);
    auto scale9Sprite = ui::Scale9Sprite::create("mall/shop_shodow.png", Rect(0,0,40,56), Rect(13,15,14,26));
    scale9Sprite->setPreferredSize(Size(900, 56));
    scale9Sprite->ignoreAnchorPointForPosition(false);
    scale9Sprite->setAnchorPoint(Vec2(0, 1));
    scale9Sprite->setPosition(0, fRealH);
    m_ScrollView->addChild(scale9Sprite);

    float fY = fRealH - ListBgItemH;
	for(auto& itemLabel:g_globalMallInfo.m_vecShopItemList)
	{
		auto item = GetListItem(itemLabel);		//auto item = GetFamilyListItem(followList.m_uid,followList.m_power);
		UpdateItem(item, itemLabel);

		item->m_pSpriteBGItem->setPosition(0, fY);
		item->m_pSpriteBGItem->setVisible(true); 
		m_vecItemListOld.push_back(item);
		fY -= uItemH;     
	}
    
	//每次都显示滚动框的最上面
	m_ScrollView->setContentSize(Size(ScrollViewW, fRealH));


	//setShowDataLoading(false) ;
}

GameMall::ShopItemLabel* GameMall::GetListItem(const ItemInShopInfo& itemInfo)
{
	ShopItemLabel* item = nullptr ;

	//背景
	auto bgItem = Sprite::create("mall/shopitembg.png");
    bgItem->ignoreAnchorPointForPosition(false);
    bgItem->setAnchorPoint(Vec2(0, 0));
	bgItem->setPosition(0, 0);
	m_ScrollView->addChild(bgItem) ;

    auto scale9Sprite = ui::Scale9Sprite::create("mall/shop_shodow.png", Rect(0,0,40,56), Rect(13,15,14,26));
    scale9Sprite->setPreferredSize(Size(900, 56));
    scale9Sprite->ignoreAnchorPointForPosition(false);
    scale9Sprite->setAnchorPoint(Vec2(0, 1));
    scale9Sprite->setPosition(0,0);
    bgItem->addChild(scale9Sprite);

    auto spriteLight = Sprite::create("mall/shop_light.png");
    spriteLight->ignoreAnchorPointForPosition(false);
    spriteLight->setAnchorPoint(Vec2(0.5, 1));
    spriteLight->setPosition(100, ListBgItemH);
    bgItem->addChild(spriteLight);
	
	if (itemInfo.isNull)
	{
		auto pItem = new ShopItemLabel;
		pItem->m_pSpriteBGItem = bgItem;
		pItem->m_pSpriteIcon = CreateNode2AndPosAddTo<HttpSprite>("mall/shopwaitting.png", cocos2d::Size::ZERO, 26, 60, bgItem, 0);

		item = pItem;
	}
	else
	{
		//图标
		auto  httpIconSprite = CreateNode2AndPosAddTo<HttpSprite>("sigin_default.png", cocos2d::Size::ZERO, 0, -5, bgItem, 0);

		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0,0.5),180,135,Color4B(0xff,0xd2,0x00,0xff),bgItem,0);
		auto labelDes = CreateLabelMSYHAndAnchorPosClrAddTo(20,"",Vec2(0,0.5),180,100,Color4B::WHITE,bgItem,0);

		//按钮
        //auto buttonDeal = FrameSpriteButton::createWithVertFrameSprite("quickshop/btok.png", false, true);
		auto buttonDeal = FrameScale9SpriteButton::createWithScale9Sprite(0, Size(172, 68), false, true);
		auto buttonDes = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"5元",Vec2(0,0.5),-6,0,Color4B::WHITE,buttonDeal,0);
		buttonDeal->setPosition(800,55);
		bgItem->addChild(buttonDeal);
		buttonDeal->SetEnable(true) ;

        auto spriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", -30, 0, buttonDeal, 0);
        spriteDiamond->setScale(0.8);

		auto pItem = new ShopItemLabel;
		pItem->m_pSpriteBGItem = bgItem;
		pItem->m_pSpriteIcon = httpIconSprite;
		pItem->m_pLabelName = labelName;
		pItem->m_pLabelDes = labelDes;
		pItem->m_pBt = buttonDeal ;
		pItem->m_pLabelBtDes = buttonDes ;
        pItem->m_pSpriteDiamond = spriteDiamond;
		item = pItem;
	}

	return item ;
}

void GameMall::UpdateItem(GameMall::ShopItemLabel *item, const ItemInShopInfo& itemInfo)
{
	if (!itemInfo.isNull)
	{
		char buff[64] ;
		item->m_pSpriteIcon->useDownLoadSize() ;
		SetHttpSpriteURL(item->m_pSpriteIcon, itemInfo.m_iconUrl);
		item->m_pSpriteIcon->setScale(0.87) ;
		SetLabelString(item->m_pLabelName,itemInfo.m_name.c_str()) ;

		SetLabelString(item->m_pLabelDes,itemInfo.m_des.c_str()) ;

		item->m_pBt->SetEnable(true) ;

		sprintf(buff, "%.0f", itemInfo.m_price);
		SetLabelString(item->m_pLabelBtDes,buff) ;

		item->m_pBt->SetHitCB(std::bind(&GameMall::menuClickItem, this,itemInfo));
	}


}

void GameMall::initHeadShopLayer()
{
    //添加一个默认空头像
    ItemInShopInfo info;
    info.m_iconUrl = "";
    info.m_price = 0;
    info.isNull = true;
    g_globalMallInfo.m_vecHeadShop.push_back(info);

    for (auto item : m_vecHeadNew)
    {
        item->m_spriteShineBg->setVisible(false);
        m_vecHeadOld.push_back(item);
    }
    for (auto item : m_vecSpriteFrameBgNew[1])
    {
        item->setVisible(false);
        m_vecSpriteFrameBgOld[1].push_back(item);
    }
    m_vecHeadNew.clear();
    m_vecSpriteFrameBgNew[1].clear();

    float oneItemH = 245;
    float totalLine = g_globalMallInfo.m_vecHeadShop.size()%3==0?(g_globalMallInfo.m_vecHeadShop.size()/3):(g_globalMallInfo.m_vecHeadShop.size()/3+1);
    float totalItemH = totalLine*oneItemH;
    float fX[3] = {3*ScrollViewW/4+70, ScrollViewW/4-70, 2*ScrollViewW/4};
    float offSet = 0;
    if (totalItemH < ScrollViewH)
    {
        totalItemH = ScrollViewH;
        m_scrollHeadShop->setBounceable(false);
    }
    else
    {
        m_scrollHeadShop->setBounceable(true);
    }
    float fY = totalItemH;
    int index = 1;
    m_scrollHeadShop->setContentOffset(Vec2(0, ScrollViewH-totalItemH), false);

    //最上面的阴影
    if (m_spriteFrameHead == nullptr)
    {
        m_spriteFrameHead = CreateNode1AndPosAddTo<Sprite>("mall/shopitembg.png", 0, 0, m_scrollHeadShop, 0);
        m_spriteFrameHead->setAnchorPoint(Vec2(0, 0));
        m_spriteFrameHead->setPosition(0, totalItemH);
        auto scale9Sprite = ui::Scale9Sprite::create("mall/shop_shodow.png", Rect(0,0,40,56), Rect(13,15,14,26));
        scale9Sprite->setPreferredSize(Size(900, 56));
        scale9Sprite->ignoreAnchorPointForPosition(false);
        scale9Sprite->setAnchorPoint(Vec2(0, 1));
        scale9Sprite->setPosition(0,totalItemH);
        m_scrollHeadShop->addChild(scale9Sprite);
        auto lastTimeSprite = ui::Scale9Sprite::create("mall/shop_txyxq_bg.png", Rect(0,0,58,40), Rect(25,18,8,4));
        lastTimeSprite->setPreferredSize(Size(220, 40));
        lastTimeSprite->setPosition(760,30);
        m_pSpriteBg[3]->addChild(lastTimeSprite,2);
        CreateLabelMSYHAndAnchorPosClrAddTo(22, "头像有效期31天", Vec2(0.5,0.5), 110, 20, Color4B(0x2c,0xa2,0x5c,0xff), lastTimeSprite, 0);
    }

    for (auto item : g_globalMallInfo.m_vecHeadShop)
    {
        auto headItem = GetHeadShopItem(item);
        updateHeadShopItem(headItem, item);
        headItem->m_spriteShineBg->setPosition(fX[index%3], fY);
        headItem->m_spriteShineBg->setVisible(true);
        m_vecHeadNew.push_back(headItem);
        if (index%3 == 0)
            fY -=oneItemH;
        else if (index%3 == 1)
        {
            auto frameBg = GetFrameBgSprite(1);
            frameBg->setVisible(true);
            frameBg->setPosition(0, fY-215);
            m_vecSpriteFrameBgNew[1].push_back(frameBg);
        }
        index++;
    }
    //头像少于3个的时候，也要有两个商品底座
    if (g_globalMallInfo.m_vecHeadShop.size() <= 3)
    {
        auto frameBg = GetFrameBgSprite(1);
        frameBg->setVisible(true);
        frameBg->setPosition(0, fY-oneItemH-215);
        m_vecSpriteFrameBgNew[1].push_back(frameBg);
    }

    m_scrollHeadShop->setContentSize(Size(ScrollViewW, totalItemH));
}

GameMall::HeadShopItem* GameMall::GetHeadShopItem(const ItemInShopInfo& itemInfo)
{
    HeadShopItem *headItem = nullptr;
    if (m_vecHeadOld.empty())
    {
        //光效
        auto spriteShine = CreateNode1AndPosAddTo<Sprite>("mall/shop_light.png", 0, 0, m_scrollHeadShop, 3);
        spriteShine->ignoreAnchorPointForPosition(false);
        spriteShine->setAnchorPoint(Vec2(0.5, 1));
        auto shineBgSz = spriteShine->getContentSize();

        auto layerBg = LayerColor::create(Color4B(0xc3,0xc3,0xc3,0xff));
        layerBg->setContentSize(Size(142,172));
        layerBg->ignoreAnchorPointForPosition(false);
        layerBg->setAnchorPoint(Vec2(0.5, 1));
        layerBg->setPosition(shineBgSz.width/2, shineBgSz.height-7);
        spriteShine->addChild(layerBg);
        auto bgSz = layerBg->getContentSize();

        //边框
        auto spriteFrame = CreateNode1AndPosAddTo<Sprite>("mall/headframe.png", bgSz.width/2, bgSz.height/2, layerBg, 2);
        auto spriteShadow = Sprite::create("mall/shop_txboxyy.png");
        spriteShadow->ignoreAnchorPointForPosition(false);
        spriteShadow->setAnchorPoint(Vec2(0,0.5));
        auto spriteFrameSz = spriteFrame->getContentSize();
        spriteShadow->setPosition(spriteFrameSz.width, spriteFrameSz.height/2);
        spriteFrame->addChild(spriteShadow);
        //遮罩
        auto spriteMask = ui::Scale9Sprite::create("mall/headmask.png", Rect(0,0,24,24), Rect(8,8,8,8));
        spriteMask->setPreferredSize(Size(142, 172));
        spriteMask->setPosition(75, 86);

        auto clipNode = ClippingNode::create();
        clipNode->setStencil(spriteMask);
        clipNode->setInverted(false);
        clipNode->setAlphaThreshold(0.5);
        clipNode->setPosition(0, 0);
        layerBg->addChild(clipNode, 1);
        auto  httpIconSprite = CreateNode2AndPosAddTo<HttpSprite>("wepoker_normalperson.png", Size::ZERO, 0, 0, clipNode, 0);
        httpIconSprite->ignoreAnchorPointForPosition(false);
        httpIconSprite->setAnchorPoint(Vec2(0.5, 1));
        httpIconSprite->setScale(0.56);
        httpIconSprite->setPosition(bgSz.width/2, bgSz.height);
        //头像上的点击事件
        auto layerBtn = LayerButton::create(false, true);
        layerBtn->setContentSize(Size(150,180));
        layerBtn->setPosition(0, 0);
        //layerBtn->SetHitCB(std::bind(&GameMall::menuHeadShopcallBack, this, itemInfo));
        layerBg->addChild(layerBtn);
        //按钮
        auto buttonDeal = FrameScale9SpriteButton::createWithScale9Sprite(5, Size(172, 52), false, true);
        auto btnName = CreateLabelMSYHBDAndAnchorPosClrAddTo(24,"5",Vec2(0,0.5), -23, 0, Color4B::WHITE, buttonDeal, 0);
        buttonDeal->setPosition(bgSz.width/2, -28);
        layerBg->addChild(buttonDeal);

        auto spriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", -35, 0, buttonDeal, 0);
        spriteDiamond->setScale(0.8);
        //buttonDeal->SetHitCB(std::bind(&GameMall::menuHeadShopcallBack, this, itemInfo));

        auto pItem = new HeadShopItem;
        pItem->m_spriteFrame = spriteFrame;
        pItem->m_httpSpriteHead = httpIconSprite;
        pItem->m_spriteMask = spriteMask;
        pItem->m_clipNode = clipNode;
        pItem->m_btnBuy = buttonDeal;
        pItem->m_labelBtnName = btnName;
        pItem->m_spriteShineBg = spriteShine;
        pItem->m_layerColor = layerBg;
        pItem->m_btnFigure = layerBtn;
        pItem->m_spriteShadow = spriteShadow;
        pItem->m_pSpriteDiamond = spriteDiamond;
        headItem = pItem;
    }
    else
    {
        headItem = m_vecHeadOld.back();
        m_vecHeadOld.pop_back();
    }
    headItem->m_btnFigure->SetHitCB(std::bind(&GameMall::menuHeadShopcallBack, this, itemInfo));
    headItem->m_btnBuy->SetHitCB(std::bind(&GameMall::menuHeadShopcallBack, this, itemInfo));

    return headItem;
}

void GameMall::updateHeadShopItem(GameMall::HeadShopItem* headShop, const ItemInShopInfo& itemInfo)
{
    headShop->m_btnBuy->SetEnable(itemInfo.isNull==true?false:true);
    char buff[32] = {0};
    if (!itemInfo.isNull)
    {
        headShop->m_pSpriteDiamond->setVisible(true);
        sprintf(buff, "%.0f", itemInfo.m_price);
        headShop->m_labelBtnName->setColor(Color3B::WHITE);
        SetLabelString(headShop->m_labelBtnName, buff);
        headShop->m_labelBtnName->setPositionX(-12);
        SetHttpSpriteURL(headShop->m_httpSpriteHead, itemInfo.m_iconUrl2);
    } 
    else
    {
        headShop->m_pSpriteDiamond->setVisible(false);
        sprintf(buff, "敬请期待", itemInfo.m_price);
        headShop->m_labelBtnName->setColor(Color3B(0xd3,0xc0,0xa7));
        SetLabelString(headShop->m_labelBtnName, buff);
        headShop->m_labelBtnName->setPositionX(-48);
        SetHttpSpriteURL(headShop->m_httpSpriteHead, "wepoker_normalperson.png");
    }
}

void GameMall::initExChangeLayer()
{	
    for (auto item : m_vecExchangeNew)
    {
        item->m_spriteShineBg->setVisible(false);
        m_vecExchangeOld.push_back(item);
    }
    for (auto item : m_vecSpriteFrameBgNew[0])
    {
        item->setVisible(false);
        m_vecSpriteFrameBgOld[0].push_back(item);
    }
    m_vecExchangeNew.clear();
    m_vecSpriteFrameBgNew[0].clear();
    
    float oneItemH = 245;
    float totalLine = g_globalMallInfo.m_vecExchange.size()%3==0?(g_globalMallInfo.m_vecExchange.size()/3):(g_globalMallInfo.m_vecExchange.size()/3+1);
    float totalItemH = totalLine*oneItemH;
    float fX[3] = {3*ScrollViewW/4+70, ScrollViewW/4-70, 2*ScrollViewW/4};
    float offSet = 0;
    if (totalItemH < ScrollViewH)
    {
        totalItemH = ScrollViewH;
        m_scrollExchange->setBounceable(false);
    }
    else
    {
        m_scrollExchange->setBounceable(true);
    }
    float fY = totalItemH;
    int index = 1;
    m_scrollExchange->setContentOffset(Vec2(0, ScrollViewH-totalItemH), false);

    //最上面的阴影
    if (m_scale9SpriteShadow == nullptr)
    {
        m_spriteFrame = CreateNode1AndPosAddTo<Sprite>("mall/shopitembg.png", 0, 0, m_scrollExchange, 0);
        m_spriteFrame->setAnchorPoint(Vec2(0, 0));
        m_scale9SpriteShadow = ui::Scale9Sprite::create("mall/shop_shodow.png", Rect(0,0,40,56), Rect(13,15,14,26));
        m_scale9SpriteShadow->setPreferredSize(Size(900, 56));
        m_scale9SpriteShadow->ignoreAnchorPointForPosition(false);
        m_scale9SpriteShadow->setAnchorPoint(Vec2(0, 1));
        m_scale9SpriteShadow->setPosition(0, totalItemH);
        m_scrollExchange->addChild(m_scale9SpriteShadow);
    }
    m_spriteFrame->setPosition(0, totalItemH);
    m_scale9SpriteShadow->setPosition(0, totalItemH);

    for (auto item : g_globalMallInfo.m_vecExchange)
    {
        auto exchangeItem = GetExchangeShopItem(item);
        updateExchangeItem(exchangeItem, item);
        exchangeItem->m_spriteShineBg->setPosition(fX[index%3], fY);
        exchangeItem->m_spriteShineBg->setVisible(true);
        exchangeItem->m_layerBtn->setVisible(true);
        m_vecExchangeNew.push_back(exchangeItem);
        if (index%3 == 0)
            fY -=oneItemH;
        else if (index%3 == 1)
        {
            auto frameBg = GetFrameBgSprite(0);
            frameBg->setVisible(true);
            frameBg->setPosition(0, fY-215);
            m_vecSpriteFrameBgNew[0].push_back(frameBg);
        }
        index++;
    }
    m_scrollExchange->setContentSize(Size(ScrollViewW, totalItemH));
}

GameMall::ExChangeItem* GameMall::GetExchangeShopItem(const ItemInShopInfo& itemInfo)
{
    ExChangeItem *exchangeItem = nullptr;
    if (m_vecExchangeOld.empty())
    {
        auto spriteShineBg = CreateNode1AndPosAddTo<Sprite>("mall/shop_light.png", 0, 0, m_scrollExchange, 2);
        spriteShineBg->ignoreAnchorPointForPosition(false);
        spriteShineBg->setAnchorPoint(Vec2(0.5, 1));
        auto bgSz = spriteShineBg->getContentSize();
        auto labelDes = CreateLabelMSYHAndAnchorPosClrAddTo(20, "兑换券描述", Vec2(0.5, 0.5), bgSz.width/2, 121, Color4B(0x9e,0xe9,0xb6,0xff), spriteShineBg, 0);
        auto labelIndate = CreateLabelMSYHAndAnchorPosClrAddTo(20, "有效期", Vec2(0.5, 0.5), bgSz.width/2, -80, Color4B(0x73,0xe8,0x0f,0xff), spriteShineBg, 0);
        auto  httpIconSprite = CreateNode2AndPosAddTo<HttpSprite>("sigin_default.png", Size::ZERO, 0, 80, spriteShineBg, 0);
        auto httpSz = httpIconSprite->getContentSize();
        httpIconSprite->setPosition((bgSz.width-httpSz.width)/2, -10);
        labelIndate->setVisible(false);
        //点击实物
		auto layerBtn = LayerButton::create(false, true);
		layerBtn->setContentSize(Size(180,120));
		layerBtn->ignoreAnchorPointForPosition(false);
		layerBtn->setAnchorPoint(Vec2(0.5,0.5));
		layerBtn->setPosition(bgSz.width/2, 40);
		spriteShineBg->addChild(layerBtn);

        //按钮
		auto buttonDeal = FrameScale9SpriteButton::createWithScale9Sprite(5, Size(172, 52), false, true);
		buttonDeal->setPosition(bgSz.width/2, -36);
		spriteShineBg->addChild(buttonDeal);
        auto btnName = CreateLabelMSYHBDAndAnchorPosClrAddTo(22,"兑换券",Vec2(0.5,0.5), 0, 0, Color4B::WHITE, buttonDeal, 0);
			

        ExChangeItem* pItem = new ExChangeItem;
        pItem->m_spriteShineBg = spriteShineBg;
        pItem->m_labelDes = labelDes;
        pItem->m_labelInDate = labelIndate;
        pItem->m_labelBtnName = btnName;
        pItem->m_httpSpriteChange = httpIconSprite;
        pItem->m_btnExchange = buttonDeal;
        pItem->m_layerBtn = layerBtn;
        exchangeItem = pItem;
    }
    else 
    {
        exchangeItem = m_vecExchangeOld.back();
        m_vecExchangeOld.pop_back();
    }
    exchangeItem->m_layerBtn->SetHitCB(std::bind(&GameMall::onExchangeBtnClick, this, itemInfo));
    exchangeItem->m_btnExchange->SetHitCB(std::bind(&GameMall::onExchangeBtnClick, this, itemInfo));

    exchangeItem->m_layerBtn->setVisible(false);
    return exchangeItem;
}

void GameMall::updateExchangeItem(GameMall::ExChangeItem* exchangeItem, const ItemInShopInfo& itemInfo)
{
    char buff[32] = {0};
    SetLabelString(exchangeItem->m_labelDes, itemInfo.m_des);
    SetLabelString(exchangeItem->m_labelInDate, itemInfo.m_strIndate);
    sprintf(buff, "%d/%d兑换券", itemInfo.m_uTicketNum, itemInfo.m_uTicketConsume);
    SetLabelString(exchangeItem->m_labelBtnName, buff);
    SetHttpSpriteURL(exchangeItem->m_httpSpriteChange, itemInfo.m_iconUrl);
    if (itemInfo.m_uTicketNum == 0)
    {
        exchangeItem->m_btnExchange->SetEnable(false);
    }
    else
    {
        exchangeItem->m_btnExchange->SetEnable(true);
    }
}

void GameMall::onExchangeBtnClick(const ItemInShopInfo& itemInfo)//兑换商城回调
{
    if (itemInfo.m_uTicketNum == 0)
        return;
    if (itemInfo.m_isPastDue)
    {
        if (g_globalMyAddr.m_strXingMing == "" || g_globalMyAddr.m_strShouJi == "" || g_globalMyAddr.m_strDiZhi == "")
        {
            PopAddrSet();
            return;
        }
        popExchangeTips();
        char buff[32] = {0};
        sprintf(buff, "%d张 %s", itemInfo.m_uTicketConsume, itemInfo.m_des.c_str());
        updateExchangeTips(3, "您将消耗 ", buff);
        m_iCurExchangeId = itemInfo.m_uID;
        m_iCurConsume = -itemInfo.m_uTicketConsume;
        m_iCurExchangeType = 1;
    }
    else
    {
        popExchangeTips();
        updateExchangeTips(1, "哎呀，兑换券不小心过期了！", "");
        m_iCurExchangeId = itemInfo.m_uID;
        m_iCurConsume = -itemInfo.m_uTicketConsume;
        m_iCurExchangeType = 2;
    }
}

void GameMall::menuClickItem(ItemInShopInfo baseInfo)
{
	popBuyAndPay();
	m_pBuyGoods->UpdateGoodTypeInfo(3, baseInfo.m_index, true);
}

void GameMall::PopFigure(unsigned uIndex)//头像商城购买
{
    if (m_layerColor == nullptr)
    {
        m_layerColor = GameLayer::create(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(),Color4B(0x00,0x00,0x00,0xaf));
        this->addChild(m_layerColor);

        // 关闭按钮
        auto btnClose = MenuItemImage::create("mall/close_white.png","mall/close_white.png",[this](Ref*){m_layerColor->setVisible(false);});
        btnClose->setPosition(750, 440);
        auto menu = Menu::create(btnClose, nullptr);
        menu->setPosition(Vec2::ZERO);
        m_layerColor->addChild(menu);
        //形象
        m_httpSpriteFigure = CreateNode2AndPosAddTo<HttpSprite>("wepoker_normalperson.png", Size::ZERO, 0, 0, m_layerColor, 0);
        m_httpSpriteFigure->ignoreAnchorPointForPosition(false);
        m_httpSpriteFigure->setAnchorPoint(Vec2(0.5, 0));
        m_httpSpriteFigure->setPosition(GameData::getInstance()->getGameWidth()/2, 0);

        m_btnBuyHead = FrameScale9SpriteButton::createWithScale9Sprite(0, Size(234, 74), false, true);
        m_btnBuyHead->setPosition(920, 90);
        m_labelBtnName = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"",Vec2(0, 0.5), 0, 0, Color4B::WHITE, m_btnBuyHead, 0);
        m_spriteDiamond = CreateNode1AndPosAddTo<Sprite>("mall/diamond.png", -35, 0, m_btnBuyHead, 0);
        m_layerColor->addChild(m_btnBuyHead);
    }
    m_layerColor->setVisible(true);
    m_btnBuyHead->SetHitCB([this, uIndex](Ref*){
        popBuyAndPay();
        m_pBuyGoods->UpdateGoodTypeInfo(4, uIndex, true);
        m_layerColor->setVisible(false);
    });
}

void GameMall::menuHeadShopcallBack(ItemInShopInfo baseInfo)//头像商城点击回调
{
    if (baseInfo.isNull)
        return;
    char buff[64] = {0};
    PopFigure(baseInfo.m_index);
    sprintf(buff, "sound/shop/%d.ogg", baseInfo.m_uID);
    PlayEffectSound(buff);
    if (baseInfo.m_uLastTime > 0)
    {
        sprintf(buff, "剩余%d天", baseInfo.m_uLastTime);
        m_spriteDiamond->setVisible(false);
        m_labelBtnName->setPositionX(-55);
    }
    else
    {
        sprintf(buff, "%.0f", baseInfo.m_price);
        m_spriteDiamond->setVisible(true);
        m_labelBtnName->setPositionX(0);
    }
    SetLabelString(m_labelBtnName, buff);
    SetHttpSpriteURL(m_httpSpriteFigure, baseInfo.m_iconUrl2);
}

//判断字符串是不是数字串
bool judgeStringIsNum(char *buff, int len)
{
    for (int i=0; i<len; i++)
    {
        if (buff[i] < 48 || buff [i] > 57)
        {
            return false;
            break;
        }
    }
    return true;
}
//
bool judgeStringIsEmail(char *buff, int len)
{
    if (len < 6)
        return false;
    bool bTemp = false;
    for (int i=0; i<len ;i++)
    {
        if (buff[i] == '@')
        {
            bTemp = true;
            break;
        }
    }
    if (!bTemp)
        return false;
    return true;
}
bool GameMall::judgeEditboxInfo()
{
    char buff[128] = {0};
    ///姓名
    if (m_labelEditbox[0]->getString() == "")
    {
        return false;
    }
    ///手机
    if (m_labelEditbox[1]->getString() == "")
    {
        return false;
    }
    //地址
    if (m_labelEditbox[3]->getString() == "")
    {
        return false;
    }
    //手机号判断
    strcpy(buff, m_labelEditbox[1]->getString().c_str());
    unsigned len = strlen(buff);
    if (len < 11 || !judgeStringIsNum(buff, len))
    {
        return false;
    }
    //QQ号码号码 
    /*strData = m_pEdtAddrItem[2]->getText();
    strcpy(buff, strData.c_str());
    len = strlen(buff);
    if (len < 6 || !judgeStringIsNum(buff, len))
    {
    return false;
    }*/
    //邮箱
    //strData = m_pEdtAddrItem[4]->getText();
    //strcpy(buff, strData.c_str());
    //len = strlen(buff);
    //if (!judgeStringIsEmail(buff, len))
    //    return false;
    return true;
}

void GameMall::editBoxEditingDidBegin(EditBox* editBox)
{
    if (editBox == m_pEdtAddrItem[0])//姓名
    {
        m_pEdtAddrItem[0]->setText(m_labelEditbox[0]->getString().c_str());
        SetLabelString(m_labelEditbox[0], "");
    }
    else if (editBox == m_pEdtAddrItem[1])//手机号
    {
        m_pEdtAddrItem[1]->setText(m_labelEditbox[1]->getString().c_str());
        SetLabelString(m_labelEditbox[1], "");
    }
    else if (editBox == m_pEdtAddrItem[2])//qq号
    {
        m_pEdtAddrItem[2]->setText(m_labelEditbox[2]->getString().c_str());
        SetLabelString(m_labelEditbox[2], "");
    }
    else if (editBox == m_pEdtAddrItem[3])//地址
    {
        m_pEdtAddrItem[3]->setText(m_labelEditbox[3]->getString().c_str());
        SetLabelString(m_labelEditbox[3], "");
    }
    else if (editBox == m_pEdtAddrItem[4])//邮箱
    {
        m_pEdtAddrItem[4]->setText(m_labelEditbox[4]->getString().c_str());
        SetLabelString(m_labelEditbox[4], "");
    }
}

void GameMall::editBoxReturn(cocos2d::ui::EditBox* editBox)
{		
    /*当type为4时，修改用户的【QQ号码】
	当type为5时，修改用户的【邮箱】
    当type为6时，修改用户的【收货人】
    当type为7时，修改用户的【手机号码】
    当type为8时，修改用户的【所在地区】*/
    char buff[128] = {0};
    std::string strData = editBox->getText();
    unsigned uOpType = 0;
    unsigned len = 0;
    if (editBox == m_pEdtAddrItem[0])//姓名
    {
        uOpType = 6;
        if (strData == "")
            return;
        SetLabelString(m_labelEditbox[0], strData);
        m_pEdtAddrItem[0]->setText("");
    }
    if (editBox == m_pEdtAddrItem[1])//手机号
    {
        uOpType = 7;
        strcpy(buff, strData.c_str());
        unsigned len = strlen(buff);
        if (len < 11 || !judgeStringIsNum(buff, len))
        {
            return;
        }
        SetLabelString(m_labelEditbox[1], strData);
        m_pEdtAddrItem[1]->setText("");
    }
    if (editBox == m_pEdtAddrItem[2])//qq号
    {
        uOpType = 4;
        if (strData == "")
            SetLabelString(m_labelEditbox[2], "QQ号码");
        SetLabelString(m_labelEditbox[2], strData);
        m_pEdtAddrItem[2]->setText("");
        /*strcpy(buff, strData.c_str());
        len = strlen(buff);
        if (len < 6 || !judgeStringIsNum(buff, len))
        {
        return;
        }*/
    }
    if (editBox == m_pEdtAddrItem[3])//地址
    {
        uOpType = 8;
        if (strData == "")
            return;
        SetLabelString(m_labelEditbox[3], strData);
        m_pEdtAddrItem[3]->setText("");
    }
    if (editBox == m_pEdtAddrItem[4])//邮箱
    {
        uOpType = 5;
        SetLabelString(m_labelEditbox[4], strData);
        m_pEdtAddrItem[4]->setText("");
        /*strcpy(buff, strData.c_str());
        len = strlen(buff);
        if (!judgeStringIsEmail(buff, len))
            return ;*/
    }
    Json::Value param;
    param["data"] = strData;
    param["type"] = uOpType;
    SendClientRequest(ClientRequest::kEditMyInfo, param);
}

Sprite* GameMall::GetFrameBgSprite(unsigned uIdx)
{
    Sprite *spriteFrameBg = nullptr;
    if (m_vecSpriteFrameBgOld[uIdx].empty())
    {
        if (uIdx == 0)
            spriteFrameBg = CreateNode1AndPosAddTo<Sprite>("mall/shopitembg.png", 0, 0 , m_scrollExchange, 0);
        else
            spriteFrameBg = CreateNode1AndPosAddTo<Sprite>("mall/shopitembg.png", 0, 0 , m_scrollHeadShop, 0);
        spriteFrameBg->ignoreAnchorPointForPosition(false);
        spriteFrameBg->setAnchorPoint(Vec2(0, 1));

        auto scale9Sprite = ui::Scale9Sprite::create("mall/shop_shodow.png", Rect(0,0,40,56), Rect(13,15,14,26));
        scale9Sprite->setPreferredSize(Size(900, 56));
        scale9Sprite->ignoreAnchorPointForPosition(false);
        scale9Sprite->setAnchorPoint(Vec2(0, 1));
        scale9Sprite->setPosition(0,0);
        spriteFrameBg->addChild(scale9Sprite);
    }
    else
    {
        spriteFrameBg = m_vecSpriteFrameBgOld[uIdx].back();
        m_vecSpriteFrameBgOld[uIdx].pop_back();
    }
    return spriteFrameBg;
}