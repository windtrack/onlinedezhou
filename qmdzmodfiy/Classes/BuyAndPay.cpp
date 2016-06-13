#include "BuyAndPay.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "GameCommon.h"
#include <stdlib.h>
#include "GameMall.h"
#include "FrameSprite.h"
#include "GameLoading.h"
#include "Util.h"
#include "GameData.h"
#include "GameConfig.h"
#include "Tools.h"


USING_NS_CC;

bool BuyGoods::init()
{
    if (!GameDialog::init("userinfo/bgdialog0.png", false))
    {
        return false;
    }
    //if (!GameDialog::init(640, 261, false))
    //{
    //    return false;
    //}
    ////灰色背景
    //auto layerBg = LayerColor::create(cocos2d::Color4B(0xf0,0xf0,0xf0,0xff), 640, 261);
    //this->addChild(layerBg);
    ////确定按钮
    //auto buyBt = MenuItemSpriteWithLabel::create("payment/btn1.png", 2, CC_CALLBACK_1(BuyGoods::onMenuSelect, this));
    //buyBt->setPosition(320, 31);
    //CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "确定", Vec2(0.5,0.5), 320, 33.5, cocos2d::Color4B(0xff,0xff,0xff,0xff), buyBt, 0);
    //auto menuBuy = Menu::create(buyBt, nullptr);
    //menuBuy->setPosition(Vec2::ZERO);
    //this->addChild(menuBuy);
    ////编辑框
    //m_editFruitNum = ui::EditBox::create(Size(330, 65), cocos2d::ui::Scale9Sprite::create("payment/bgedit.png"));
    //m_editFruitNum->setInputMode(ui::EditBox::InputMode::NUMERIC);
    //m_editFruitNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    //m_editFruitNum->setFont("fzltbd.ttf", 30);
    //m_editFruitNum->setFontColor(Color3B(0xff,0x5f,0x00));
    //m_editFruitNum->setDelegate(this);
    //m_editFruitNum->setPosition(Vec2(377, 138));
    //this->addChild(m_editFruitNum);
    ////"-" & "+"
    //auto minusBt = MenuItemSpriteWithLabel::create("payment/minus.png", 3, CC_CALLBACK_1(BuyGoods::MinusBuy, this));
    //minusBt->setPosition(180, 138);
    //auto minusBuy = Menu::create(minusBt, nullptr);
    //minusBuy->setPosition(Vec2::ZERO);
    //this->addChild(minusBuy);

    //auto addBt = MenuItemSpriteWithLabel::create("payment/add.png", 3, CC_CALLBACK_1(BuyGoods::AddBuy, this));
    //addBt->setPosition(574, 138);
    //auto addBuy = Menu::create(addBt, nullptr);
    //addBuy->setPosition(Vec2::ZERO);
    //this->addChild(addBuy);
    ////水果名称/单价//g_globalMallInfo.m_fruitInfo[i]
    //m_labelGoodsName = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0,0), 150, 201, cocos2d::Color4B(0x34,0x34,0x34,0xff), this, 0);
    //m_labelGoodsPrice = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0,0), 340, 201, cocos2d::Color4B(0x8b,0x8b,0x8b,0xff), this, 0);
    //m_labelTotalMoney = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(1,0), 609, 198, cocos2d::Color4B(0xff,0x5f,0x00,0xff), this, 0);
    ////购买水果图片
    //m_httpFruitSprite = CreateNode2AndPosAddTo<HttpSprite>("mall/defaultfruiticon.png", cocos2d::Size::ZERO, 82, 153, this, 0);
    ////购买会员图片
    //m_spriteVip = FrameSprite::create("vip.png", 6, 1, 0);
    //m_spriteVip->setPosition(82, 143);
    //this->addChild(m_spriteVip);
    //m_spriteVip->setVisible(false);

    /*m_pPaymentMethod = PaymentMethod::create();
    Size szBuy = this->getContentSize();
    Size szPay = m_pPaymentMethod->getContentSize();
    m_pPaymentMethod->setPosition(313 - szPay.width/2, 136 - szPay.height/2);
    this->addChild(m_pPaymentMethod);
    m_pPaymentMethod->setVisible(false);*/
    auto bgSz = this->getContentSize();
    //操作按钮
    m_pMenuBuy = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(BuyGoods::onMenuSelect, this));
    m_pMenuBuy->SetLabel(CreateLabelMSYHBD(30, "确认"));
    m_pMenuBuy->setPosition(222, 56);
    m_pMenuCancel = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(BuyGoods::onMenuSelect, this));
    m_pMenuCancel->SetLabel(CreateLabelMSYHBD(30, "取消"));
    m_pMenuCancel->setPosition(456, 56);
    auto pMenu = Menu::create(m_pMenuBuy, m_pMenuCancel, nullptr);
    pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu);
    //描述
    m_pLabelDis = CreateLabelMSYHAndAnchorPosClrAddTo(28, "", Vec2(0.5,0.5), bgSz.width/2, 200, Color4B::WHITE, this, 0);
    m_pLineLabel = LineLabel::create();
    m_pLineLabel->setPosition(bgSz.width/2, 250);
    this->addChild(m_pLineLabel);
    m_bDiamondPlenty = false;

    //m_dOneGoodsPrice = 0.0;
    return true;
}

//void BuyGoods::OnCancleGameLayer(bool bVisible)
//{
//	m_pPaymentMethod->onKeyEscPressedVisible(bVisible);
//}

void BuyGoods::UpdateGoodTypeInfo(unsigned type, unsigned index, bool bGameMallCall)
{
    char buff[64] = {0};
    m_bGameMallCall = bGameMallCall;
    switch(type)
    {
    case 0:
        { 
            if (g_globalMyData.GetDiamond() >= g_globalMallInfo.m_fruitInfo[index].m_dPrice)
            {
                m_pLineLabel->resetLable();
                m_pLabelDis->setPositionY(200);
                m_pLineLabel->setVisible(true);
                SetLabelString(m_pLabelDis, "是否继续？");
                m_pLineLabel->Appand(28, "fzlt.ttf", "您将花费", Color4B::WHITE);
                sprintf(buff, "%.0f", g_globalMallInfo.m_fruitInfo[index].m_dPrice);
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_pLineLabel->Appand(28, "fzlt.ttf", "钻购买", Color4B::WHITE);
                sprintf(buff, "%s", g_globalMallInfo.m_fruitInfo[index].m_strName.c_str());
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_strGoodsName = g_globalMallInfo.m_fruitInfo[index].m_strName;
                m_dGoodsPrice = g_globalMallInfo.m_fruitInfo[index].m_dPrice;
                m_uGoodsID = g_globalMallInfo.m_fruitInfo[index].m_uID;
                m_bDiamondPlenty = true;
            }
            else
            {
                m_bDiamondPlenty = false;
                m_pLineLabel->setVisible(false);
                m_pLabelDis->setPositionY(250);
                SetLabelString(m_pLabelDis, "您的钻石不足，是否进行购买?");
            }
        }break;
    case 1:
        {
            if (g_globalMyData.GetDiamond() >= g_globalMallInfo.m_vipInfo[index].m_dPrice)
            {
                m_pLineLabel->resetLable();
                m_pLabelDis->setPositionY(200);
                m_pLineLabel->setVisible(true);
                SetLabelString(m_pLabelDis, "是否继续？");
                m_pLineLabel->Appand(28, "fzlt.ttf", "您将花费", Color4B::WHITE);
                sprintf(buff, "%.0f", g_globalMallInfo.m_vipInfo[index].m_dPrice);
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_pLineLabel->Appand(28, "fzlt.ttf", "钻购买", Color4B::WHITE);
                sprintf(buff, "%s", g_globalMallInfo.m_vipInfo[index].m_strVipName.c_str());
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_strGoodsName = g_globalMallInfo.m_vipInfo[index].m_strVipName;
                m_dGoodsPrice = g_globalMallInfo.m_vipInfo[index].m_dPrice;
                m_uGoodsID = index + 2001;
                m_bDiamondPlenty = true;
            }
            else
            {
                m_bDiamondPlenty = false;
                m_pLineLabel->setVisible(false);
                m_pLabelDis->setPositionY(250);
                SetLabelString(m_pLabelDis, "您的钻石不足，是否进行购买?");
            }
        }break;
    case 2:{}break;
    case 3:
        {
            ItemInShopInfo itemInfo = g_globalMallInfo.m_vecShopItemList.at(index);
            if (g_globalMyData.GetDiamond() >= itemInfo.m_price)
            {
                m_pLineLabel->resetLable();
                m_pLabelDis->setPositionY(200);
                m_pLineLabel->setVisible(true);
                SetLabelString(m_pLabelDis, "是否继续？");
                m_pLineLabel->Appand(28, "fzlt.ttf", "您将花费", Color4B::WHITE);
                sprintf(buff, "%.0f", itemInfo.m_price);
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_pLineLabel->Appand(28, "fzlt.ttf", "钻购买", Color4B::WHITE);
                sprintf(buff, "%s", itemInfo.m_name.c_str());
                m_pLineLabel->Appand(24, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_strGoodsName = itemInfo.m_name.c_str();
                m_dGoodsPrice = itemInfo.m_price;
                m_uGoodsID = itemInfo.m_uID;
                m_bDiamondPlenty = true;
            }
            else
            {
                m_bDiamondPlenty = false;
                m_pLineLabel->setVisible(false);
                m_pLabelDis->setPositionY(250);
                SetLabelString(m_pLabelDis, "您的钻石不足，是否进行购买?");
            }
        }break;
    case 4:
        {
            ItemInShopInfo itemInfo = g_globalMallInfo.m_vecHeadShop.at(index);
            if (g_globalMyData.GetDiamond() >= itemInfo.m_price)
            {
                m_pLineLabel->resetLable();
                m_pLabelDis->setPositionY(200);
                m_pLineLabel->setVisible(true);
                SetLabelString(m_pLabelDis, "是否继续？");
                m_pLineLabel->Appand(28, "fzlt.ttf", "您将花费", Color4B::WHITE);
                sprintf(buff, "%.0f", itemInfo.m_price);
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_pLineLabel->Appand(28, "fzlt.ttf", "钻购买", Color4B::WHITE);
                sprintf(buff, "%s", itemInfo.m_name.c_str());
                m_pLineLabel->Appand(28, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
                m_strGoodsName = itemInfo.m_name.c_str();
                m_dGoodsPrice = itemInfo.m_price;
                m_uGoodsID = itemInfo.m_uID;
                m_bDiamondPlenty = true;
            }
            else
            {
                m_bDiamondPlenty = false;
                m_pLineLabel->setVisible(false);
                m_pLabelDis->setPositionY(250);
                SetLabelString(m_pLabelDis, "您的钻石不足，是否进行购买?");
            }
        }break;
    }

 //   int num = 1;
 //   this->setVisible(true);
 //   //m_pPaymentMethod->setVisible(false);
 //   if (type == 0)//购买水果
 //   {
 //       m_spriteVip->setVisible(false);
 //       m_httpFruitSprite->setVisible(true);
 //       m_dOneGoodsPrice = g_globalMallInfo.m_fruitInfo[index].m_dPrice;
 //       m_strGoodsName = g_globalMallInfo.m_fruitInfo[index].m_strName;
 //       SetLabelString(m_labelGoodsName, g_globalMallInfo.m_fruitInfo[index].m_strName);
 //       m_strOnePrice = buff;
 //       SetLabelString(m_labelGoodsPrice, buff);
 //       SetHttpSpriteURL(m_httpFruitSprite, g_globalMallInfo.m_fruitInfo[index].m_strIconUrl);
 //       Size sz = m_httpFruitSprite->getContentSize();
 //       m_httpFruitSprite->setPosition(82 - sz.width/2, 153 - sz.height/2);

 //       m_editFruitNum->setText("1");
 //       sprintf(buff, "￥%.0f", num*m_dOneGoodsPrice);
 //       m_strTotalMoney = buff;
 //       SetLabelString(m_labelTotalMoney, buff);
	//	m_uGoodsId = g_globalMallInfo.m_fruitInfo[index].m_uID;
 //   }
 //   else if (type == 1)//开通会员
 //   {
 //       std::string strVipName[6] = {"新秀","侠士","骑士","精灵","天使","天神"};
 //       m_spriteVip->setVisible(true);
 //       m_httpFruitSprite->setVisible(false);
 //       m_dOneGoodsPrice = g_globalMallInfo.m_vipInfo[index].m_dPrice;
	//	m_strGoodsName = g_globalMallInfo.m_vipInfo[index].m_strVipName;
 //       SetLabelString(m_labelGoodsName, strVipName[index]);
 //       sprintf(buff, "￥%.0f / 月", g_globalMallInfo.m_vipInfo[index].m_dPrice);
 //       m_strOnePrice = buff;
 //       SetLabelString(m_labelGoodsPrice, buff);
 //       m_spriteVip->SetFrameIdx(index);

 //       m_editFruitNum->setText("1");
 //       sprintf(buff, "￥%.0f", num*m_dOneGoodsPrice);
 //       m_strTotalMoney = buff;
 //       SetLabelString(m_labelTotalMoney, buff);
	//	m_uGoodsId = index + 2001; // CAODANDE: vip编号 这里应该是从服务端发过来的
 //   }
	//else if (type == 2)//兑换
	//{

	//}
	//else if (type == 3)//道具商城
	//{
	//	m_spriteVip->setVisible(false);
	//	m_httpFruitSprite->setVisible(true);

	//	ItemInShopInfo itemInfo = g_globalMallInfo.m_vecShopItemList.at(index) ;
	//	m_dOneGoodsPrice = itemInfo.m_price;
	//	m_strGoodsName = itemInfo.m_name;
	//	SetLabelString(m_labelGoodsName, itemInfo.m_name);
	//	m_strOnePrice = buff;
	//	SetLabelString(m_labelGoodsPrice, buff);
	//	SetHttpSpriteURL(m_httpFruitSprite, itemInfo.m_iconUrl);
	//	Size sz = m_httpFruitSprite->getContentSize();
	//	m_httpFruitSprite->setPosition(82 - sz.width/2, 153 - sz.height/2);

	//	m_editFruitNum->setText("1");
	//	sprintf(buff, "￥%.2f", num*m_dOneGoodsPrice);
	//	m_strTotalMoney = buff;
	//	SetLabelString(m_labelTotalMoney, buff);
	//	m_uGoodsId = itemInfo.m_uID;
	//}
 //   else if (type == 4)
 //   {
 //       for (auto headItem : g_globalMallInfo.m_vecHeadShop)
 //       {
 //           if (headItem.m_uID == index)
 //           {
 //               m_dOneGoodsPrice = headItem.m_price;
 //               m_strGoodsName = headItem.m_name;
 //               sprintf(buff, "￥%.0f / 月", headItem.m_price);
 //               m_strOnePrice = buff;
 //               SetLabelString(m_labelGoodsName, buff);
 //               SetLabelString(m_labelGoodsName, m_strGoodsName);
 //               SetHttpSpriteURL(m_httpFruitSprite, headItem.m_iconUrl2);
 //               Size sz = m_httpFruitSprite->getContentSize();
 //               m_httpFruitSprite->setPosition(82 - sz.width/2, 153 - sz.height/2);
 //               m_editFruitNum->setText("1");
 //               sprintf(buff, "￥%.0f", num*m_dOneGoodsPrice);
 //               m_strTotalMoney = buff;
 //               SetLabelString(m_labelTotalMoney, buff);
 //               m_uGoodsId = headItem.m_uID;
 //               break;
 //           }
 //       }
 //   }
}

//void BuyGoods::editBoxReturn(cocos2d::ui::EditBox* editBox)
//{
//    char buff[64] = {0};
//    int num = atoi(editBox->getText());
//    sprintf(buff, "￥%.0f", num*m_dOneGoodsPrice);
//    m_strTotalMoney = buff;
//    SetLabelString(m_labelTotalMoney, buff);
//}

void BuyGoods::onMenuSelect(cocos2d::Ref* pSend)
{
 //   m_pPaymentMethod->setVisible(true);
 //   int goodsNum = atoi(m_editFruitNum->getText());
	//m_goodsInfo.goodsName = m_strGoodsName;
	//m_goodsInfo.onePrice = m_strOnePrice;
	//m_goodsInfo.totalMoney = m_strTotalMoney;
	//m_goodsInfo.num = goodsNum;
	//m_goodsInfo.Id = m_uGoodsId;
	//m_goodsInfo.Price = m_dOneGoodsPrice;
 //   m_pPaymentMethod->UpdateGoodTypeInfo(m_goodsInfo);
    if (m_bDiamondPlenty)//钻石充足->购买商品
    {
        if (pSend == m_pMenuBuy)
        {
            Json::Value param;
            param["consume"] = m_dGoodsPrice;
            param["goodsID"] = m_uGoodsID;
            SendClientRequest(ClientRequest::kBuyGoodsByDiamond, param);
            this->setVisible(false);
        }
        else if (pSend == m_pMenuCancel)
        {
            this->setVisible(false);
        }
    }
    else//钻石不足->购买钻石
    {
        if (pSend == m_pMenuBuy)
        {
            this->setVisible(false);
            if (m_bGameMallCall)
            {
                SendClientRequest(ClientRequest::kPopDiamondBuy);
            }
            else
            {
                m_func() ;
            }
        }
        else if (pSend == m_pMenuCancel)
        {
            this->setVisible(false);
        }
    }

//#if Use_3rd_Pay
//
//	m_pPaymentMethod->setVisible(false);
//
//	Json::Value param;
//	//param["price"] = 0.01*goodsNum;
//	param["price"] = m_dOneGoodsPrice*goodsNum;
//	param["paytype"] = 3;
//	param["id"] = m_uGoodsId; // 商品id，如果为水果，就是水果id，如果是vip就是2001-2006
//	param["name"] = m_strGoodsName ;
//	SendClientRequest(ClientRequest::kMobilePay, param);
//
//	GameDialog::setVisible(false) ;
//#endif

}

void BuyGoods::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
        GameDialog::setVisible(true);
        Tools::callScaleAction(this) ;
    }
    else
    {
        callHideAction() ;
    }
}

void BuyGoods::setQuickShopOpt(std::function<void()> func)
{
    m_func = func;
}

//void BuyGoods::MinusBuy(cocos2d::Ref* pSend)
//{
//    int num = atoi(m_editFruitNum->getText());
//    if (num > 1)
//    {
//       num -=1;
//    }
//    char buff[64] = {0};
//    sprintf(buff, "%d", num);
//    m_editFruitNum->setText(buff);
//    sprintf(buff, "￥%.0f", num*m_dOneGoodsPrice);
//    m_strTotalMoney = buff;
//    SetLabelString(m_labelTotalMoney, buff);
//}
//void BuyGoods::AddBuy(cocos2d::Ref* pSend)
//{
//    int num = atoi(m_editFruitNum->getText());
//    num +=1;
//    char buff[64] = {0};
//    sprintf(buff, "%d", num);
//    m_editFruitNum->setText(buff);
//    sprintf(buff, "￥%.0f", num*m_dOneGoodsPrice);
//    m_strTotalMoney = buff;
//    SetLabelString(m_labelTotalMoney, buff);
//}


bool PaymentMethod::init()
{
    if (!GameLayer::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), cocos2d::Color4B(0xff,0xff,0xff,0xff), true, true))
    {
        return false;
    }
    //返回按钮
    auto btBack = MenuItemSpriteWithLabel::create("payment/btback.png", 2, CC_CALLBACK_1(PaymentMethod::BtnExit, this));
    btBack->setPosition(54, GameData::getInstance()->getGameHeight() - 60);
    auto menuBack = Menu::create(btBack, nullptr);
    menuBack->setPosition(Vec2::ZERO);
    this->addChild(menuBack);
    //label
    m_labelGoodsName = CreateLabelMSYHAndAnchorPosClrAddTo(28, "", Vec2(0,0), 30, 393, cocos2d::Color4B(0x17,0x17,0x17,0xff), this, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(10, "-------------------------------------", Vec2(0,0), 30, 380, cocos2d::Color4B(0x8b,0x8b,0x8b,0xff), this, 0);
    m_laelOnePrice = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0,0), 30, 352, cocos2d::Color4B(0x8b,0x8b,0x8b,0xff), this, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(10, "-------------------------------------", Vec2(0,0), 30, 338, cocos2d::Color4B(0x8b,0x8b,0x8b,0xff), this, 0);
    m_labelGoodsNum = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0,0), 30, 300, cocos2d::Color4B(0x17,0x17,0x17,0xff), this, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(20, "总价 : ", Vec2(0,0), 30, 248, cocos2d::Color4B(0x17,0x17,0x17,0xff), this, 0);
    m_labelTotalMoney = CreateLabelMSYHAndAnchorPosClrAddTo(28, "", Vec2(0,0), 95, 248, cocos2d::Color4B(254,93,3,255), this, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(10, "-------------------------------------", Vec2(0,0), 30, 228, cocos2d::Color4B(0x8b,0x8b,0x8b,0xff), this, 0);
    //右侧背景层
    auto layerBg = CreateNode3AndPosAddTo<LayerColor>(Color4B(0xf0,0xf0,0xf0,0xff), 845, GameData::getInstance()->getGameHeight(), 300, 0, this, 0);
    //波浪线
    CreateNode1AndPosAddTo<Sprite>("payment/wave.png", 3, (GameData::getInstance()->getGameHeight())/2-1, layerBg, 0);
    CreateLabelMSYHAndAnchorPosClrAddTo(23, "支付方式", Vec2(0,0), 145, 489, cocos2d::Color4B(0x17,0x17,0x17,0xff), layerBg, 0);
    //支付方式按钮
    m_weixinPayBtn = MenuItemSpriteWithLabel::create("payment/paybt.png", 3, [this](cocos2d::Ref*){SelectPayMethod(2);});
    Size sz = m_weixinPayBtn->getContentSize();
    CreateNode1AndPosAddTo<Sprite>("payment/weixin.png", sz.width/2, sz.height/2, m_weixinPayBtn, 0);
    m_weixinPayBtn->setPosition(430, 342);
    auto menuWeiXin = Menu::create(m_weixinPayBtn, nullptr);
    menuWeiXin->setPosition(Vec2::ZERO);
    layerBg->addChild(menuWeiXin);

    m_zhifubaoPayBtn = MenuItemSpriteWithLabel::create("payment/paybt.png", 3, [this](cocos2d::Ref*){SelectPayMethod(0);});
    sz = m_zhifubaoPayBtn->getContentSize();
    CreateNode1AndPosAddTo<Sprite>("payment/zhifubao.png", sz.width/2, sz.height/2, m_zhifubaoPayBtn, 0);
    m_zhifubaoPayBtn->setPosition(430, 432);
    auto menuZhiFuBao = Menu::create(m_zhifubaoPayBtn, nullptr);
    menuZhiFuBao->setPosition(Vec2::ZERO);
    layerBg->addChild(menuZhiFuBao);

    m_shoujiPayBtn = MenuItemSpriteWithLabel::create("payment/paybt.png", 3, [this](cocos2d::Ref*){SelectPayMethod(1);});
    CreateNode1AndPosAddTo<Sprite>("payment/shoujizhifu.png", sz.width/2, sz.height/2, m_shoujiPayBtn, 0);
    m_shoujiPayBtn->setPosition(430, 250);
    auto menuShouJi = Menu::create(m_shoujiPayBtn, nullptr);
    menuShouJi->setPosition(Vec2::ZERO);
    layerBg->addChild(menuShouJi);
    //确认按钮
    auto buyBt = MenuItemSpriteWithLabel::create("payment/btn2.png", 2, CC_CALLBACK_1(PaymentMethod::DeterminePay, this));
    buyBt->setPosition(430, 162);
    CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "确定", Vec2(0.5,0.5), 320, 32.5, cocos2d::Color4B(0xff,0xff,0xff,0xff), buyBt, 0);
    auto menuBuy = Menu::create(buyBt, nullptr);
    menuBuy->setPosition(Vec2::ZERO);
    layerBg->addChild(menuBuy);

    m_zhifubaoPayBtn->setEnabled(false);
    m_uPayMethod = 1;


#if !OpenWeixinAndYL
	//m_weixinPayBtn->setVisible(false) ;
	m_shoujiPayBtn->setVisible(false) ;

#endif
    return true;
}

void PaymentMethod::BtnExit(cocos2d::Ref* pSend)
{
    this->setVisible(false);
}

void PaymentMethod::UpdateGoodTypeInfo(GoodsInfo goodsInfo)
{
    char buff[64] = {0};
	SetLabelString(m_labelGoodsName, goodsInfo.goodsName);
	SetLabelString(m_laelOnePrice, goodsInfo.onePrice);
	sprintf(buff, "数量 : %d", goodsInfo.num);
    SetLabelString(m_labelGoodsNum, buff);
	SetLabelString(m_labelTotalMoney, goodsInfo.totalMoney);
	m_dTotalMoney = goodsInfo.Price;
	m_uGoodsId = goodsInfo.Id;
}

void PaymentMethod::SelectPayMethod(unsigned idx)
{
#if OpenWeixinAndYL
	if (idx == 0)//支付宝支付
	{
		m_uPayMethod = 1;
		m_weixinPayBtn->setEnabled(true);
		m_zhifubaoPayBtn->setEnabled(false);
		m_shoujiPayBtn->setEnabled(true);
	}
	else if(idx == 1)//手机支付
	{
		m_uPayMethod = 0;
		m_weixinPayBtn->setEnabled(true);
		m_zhifubaoPayBtn->setEnabled(true);
		m_shoujiPayBtn->setEnabled(false);
	}
	else if (idx == 2) { // 微信
		m_uPayMethod = 2;
		m_weixinPayBtn->setEnabled(false);
		m_zhifubaoPayBtn->setEnabled(true);
		m_shoujiPayBtn->setEnabled(true);
	}
#else
	//m_weixinPayBtn->setVisible(false) ;
	m_shoujiPayBtn->setVisible(false) ;
	if (idx == 0)//支付宝支付
	{
		m_uPayMethod = 1;
		m_weixinPayBtn->setEnabled(true);
		m_zhifubaoPayBtn->setEnabled(false);
		m_shoujiPayBtn->setEnabled(true);
	}
	else if(idx == 1)//手机支付
	{
		m_uPayMethod = 0;
		m_weixinPayBtn->setEnabled(false);
		m_zhifubaoPayBtn->setEnabled(false);
		m_shoujiPayBtn->setEnabled(false);
	}
	else if (idx == 2) { // 微信
		m_uPayMethod = 2;
		m_weixinPayBtn->setEnabled(false);
		m_zhifubaoPayBtn->setEnabled(true);
		m_shoujiPayBtn->setEnabled(true);
	}

#endif

}

void PaymentMethod::DeterminePay(cocos2d::Ref* pSend)
{

	Json::Value param;

	param["price"] = m_dTotalMoney;
	//param["price"] = 0.01;
	param["paytype"] = m_uPayMethod;
	param["id"] = m_uGoodsId; // 商品id，如果为水果，就是水果id，如果是vip就是2001-2006
	param["name"] = m_labelGoodsName->getString();
	SendClientRequest(ClientRequest::kMobilePay, param);

}

void PaymentMethod::onKeyEscPressed()
{
    this->setVisible(false);
}

void PaymentMethod::onKeyEscPressedVisible(bool bVisible)
{
	this->setVisible(false);
}