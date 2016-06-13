#include "AccountLogin.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "LogonLayerEx.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "Tools.h"

USING_NS_CC;
using namespace cocos2d::ui;


bool AccountLogin::initAccountLogin(LogonLayerEx* accountLogin)
{
    ////账号
    m_pTxtFiledUser = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_Count")));
    m_pTxtFiledUser->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_pTxtFiledUser->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_pTxtFiledUser->setFontSize(24);
    m_pTxtFiledUser->addEventListener(CC_CALLBACK_2(AccountLogin::textFieldEvent, this));

    ////密码
    m_pTxtFiledPwd = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_Password")));
    m_pTxtFiledPwd->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_pTxtFiledPwd->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_pTxtFiledPwd->setFontSize(24);
    m_pTxtFiledPwd->addEventListener(CC_CALLBACK_2(AccountLogin::textFieldEvent, this));

    //账号
    auto panel_Count = dynamic_cast<Layout*>(Tools::getWidgetByName(this, std::string("Panel_Count")));
    //m_pTxtFiledUser = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPanel.width, szPanel.height), ui::Scale9Sprite::create("login/bgedit.png") , 0, 0, this, 1);
    /*m_pTxtFiledUser = ui::EditBox::create(Size(szPanel.width, szPanel.height), ui::Scale9Sprite::create("login/bgedit.png"));
    m_pTxtFiledUser->setFont("fzlt.ttf", 24);
    m_pTxtFiledUser->setFontColor(Color3B::BLACK);
    m_pTxtFiledUser->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pTxtFiledUser->setPosition(panel_Count->getPosition());
    m_pTxtFiledUser->setDelegate(this);*/

    //密码
    auto panel_Password = dynamic_cast<Layout*>(Tools::getWidgetByName(this, std::string("Panel_Password")));
    /*m_pTxtFiledPwd = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPanel.width, szPanel.height), ui::Scale9Sprite::create("login/bgedit.png") , 0, 0, this, 1);
    m_pTxtFiledPwd->setFont("fzlt.ttf", 24);
    m_pTxtFiledPwd->setFontColor(Color3B::BLACK);
    m_pTxtFiledPwd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    m_pTxtFiledPwd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pTxtFiledPwd->setPosition(panel_Password->getPosition());
    m_pTxtFiledPwd->setDelegate(this);*/
    //立即登陆
    m_pMenuItemLoginNow = dynamic_cast<Button*>(Tools::getWidgetByName(this,std::string("bt_loginNow")));
    m_pMenuItemLoginNow->addTouchEventListener(CC_CALLBACK_2(AccountLogin::menuClickLoginNow, this));

    //其它方式登陆
    m_pMenuItemLoginByOther = dynamic_cast<Button*>(Tools::getWidgetByName(this,std::string("bt_loginOther")));
    m_pMenuItemLoginByOther->addTouchEventListener(CC_CALLBACK_2(AccountLogin::menuClickLoginOther, this));

    //注册账号
    auto labelRegister = Label::createWithTTF("","ui/fzlt.ttf",18);
    SetLabelString(labelRegister,"注册账号");
    labelRegister->setTextColor(Color4B(0x39,0x9f,0x56,0xff));
    auto registerAccount = MenuItemLabel::create(labelRegister,CC_CALLBACK_1(AccountLogin::menuClickAccountRegister,this));
    registerAccount->setAnchorPoint(Vec2(0, 0.5));
    /*auto pos = m_pTxtFiledUser->getWorldPosition();*/
    auto sz = panel_Count->getContentSize();
    registerAccount->setPosition(sz.width + 15, sz.height/2);
    auto menu = Menu::create(registerAccount,nullptr);
    menu->setPosition(Vec2::ZERO);
    panel_Count->addChild(menu);

    //忘记密码
    auto labelForget = Label::createWithTTF("","ui/fzlt.ttf",18);
    SetLabelString(labelForget,"忘记密码");
    labelForget->setTextColor(Color4B(0x39,0x9f,0x56,0xff));
    auto forgetPwd = MenuItemLabel::create(labelForget,CC_CALLBACK_1(AccountLogin::menuClickForgetPwd,this));
    forgetPwd->setAnchorPoint(Vec2(0, 0.5));
    //pos = m_pTxtFiledPwd->getWorldPosition();
    sz = panel_Password->getContentSize();
    forgetPwd->setPosition(sz.width + 15, sz.height/2);
    menu = Menu::create(forgetPwd,nullptr);
    menu->setPosition(Vec2::ZERO);
    panel_Password->addChild(menu);

    //记住密码
    auto pMenuItemRemember0 = MenuItemSpriteWithLabel::create("res/ui/right0.png", 2, nullptr);
    auto pMenuItemRemember1 = MenuItemSpriteWithLabel::create("res/ui/right1.png", 2, nullptr);
    auto pLabelRemember0 = CreateLabelMSYH(20);
    auto pLabelRemember1 = CreateLabelMSYH(20);
    pMenuItemRemember0->SetEnabledColor(Color4B::WHITE);
    pMenuItemRemember1->SetEnabledColor(Color4B::WHITE);
    SetLabelString(pLabelRemember0, "记住密码");
    SetLabelString(pLabelRemember1, "记住密码");
    pMenuItemRemember0->SetLabel(pLabelRemember0);
    pMenuItemRemember1->SetLabel(pLabelRemember1);
    pLabelRemember0->setPosition(80,18);
    pLabelRemember1->setPosition(80,18);
    m_pRememberPwd = MenuItemToggle::create(pMenuItemRemember0);
    m_pRememberPwd->addSubItem(pMenuItemRemember1);
    m_pRememberPwd->setPosition(162,176);

    //自动登录
    auto pMenuItemSelfLogin0 = MenuItemSpriteWithLabel::create("res/ui/right0.png", 2, nullptr);
    auto pMenuItemSelfLogin1 = MenuItemSpriteWithLabel::create("res/ui/right1.png", 2, nullptr);
    auto pLabelSelfLogin0 = CreateLabelMSYH(20);
    auto pLabelSelfLogin1 = CreateLabelMSYH(20);
    pMenuItemSelfLogin0->SetEnabledColor(Color4B::WHITE);
    pMenuItemSelfLogin1->SetEnabledColor(Color4B::WHITE);
    SetLabelString(pLabelSelfLogin0, "自动登录");
    SetLabelString(pLabelSelfLogin1, "自动登录");
    pMenuItemSelfLogin0->SetLabel(pLabelSelfLogin0);
    pMenuItemSelfLogin1->SetLabel(pLabelSelfLogin1);
    pLabelSelfLogin0->setPosition(80,18);
    pLabelSelfLogin1->setPosition(80,18);
    m_pSelfLogin = MenuItemToggle::create(pMenuItemSelfLogin0);
    m_pSelfLogin->addSubItem(pMenuItemSelfLogin1);
    m_pSelfLogin->setPosition(316,176);

    menu = Menu::create(m_pRememberPwd,m_pSelfLogin,nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    std::string strAccount, strPwd;
    bool bLastLogonSuccess = false;
    bool bRemPwd = false, bAutoLogin = false;
    g_globalConfig.GetLastLogonInfo(strAccount, strPwd, bRemPwd, bAutoLogin, bLastLogonSuccess);
    m_pTxtFiledUser->setText(strAccount.c_str());
    if (bLastLogonSuccess && bRemPwd)
    {
        m_pTxtFiledPwd->setText(strPwd.c_str());
    }
    else
    {
        m_pTxtFiledPwd->setText("");
    }

    m_pRememberPwd->setSelectedIndex(bRemPwd ? 1 : 0);
    m_pSelfLogin->setSelectedIndex(bAutoLogin ? 1 : 0);

    m_accountLogin = accountLogin;

    return true;
}

void AccountLogin::textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    if (type == TextField::EventType::DETACH_WITH_IME)
    {
        if(sender == m_pTxtFiledUser)
        {

        }
        else if (sender == m_pTxtFiledPwd)
        {

        }
    }
}

//注册账号
void AccountLogin::menuClickAccountRegister(cocos2d::Ref* pSender)
{
	m_accountLogin->ShowRegisit();
}

//忘记密码
void AccountLogin::menuClickForgetPwd(cocos2d::Ref* pSender)
{
	m_accountLogin->ShowResetPwd();
}

//立即登录
void AccountLogin::menuClickLoginNow(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        Json::Value param;
        param["account"] = m_pTxtFiledUser->getString();
        param["password"] = m_pTxtFiledPwd->getString();
        SendClientRequest(ClientRequest::kUserLogon, param);
        bool bRemPwd = m_pRememberPwd->getSelectedIndex() == 1;
        bool bAutoLogin = m_pSelfLogin->getSelectedIndex() == 1;
        g_globalConfig.SetLastLogonInfo(m_pTxtFiledUser->getString(), m_pTxtFiledPwd->getString(), bRemPwd, bAutoLogin, false);

        m_accountLogin->ShowLoading();
    }
	
}

//其他方式登陆
void AccountLogin::menuClickLoginOther(cocos2d::Ref* pSender,  cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        m_accountLogin->ShowQuickLogon();
    }
	
}

void AccountLogin::menuDefaultSelect(cocos2d::Ref* pSender)
{

}

//void AccountLogin::editBoxReturn(cocos2d::ui::EditBox* editBox)
//{
//
//}