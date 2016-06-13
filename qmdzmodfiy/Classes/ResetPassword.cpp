#include "ResetPassword.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "GameCommon.h"
#include "FrameSprite.h"
#include "MenuItemSpriteWithLabel.h"
#include "BaseButton.h"
#include "LogonLayerEx.h"
#include "Tools.h"

USING_NS_CC;
using namespace cocos2d::ui;

//---------------------------------------------------------------------------------------------------------------
//ResetPassword* ResetPassword::create(Layout* resetView)
//{
//    auto pRet = new ResetPassword;
//    if (pRet->init(resetView))
//    {
//        pRet->autorelease();
//    }
//    else
//    {
//        delete pRet;
//        pRet = nullptr;
//    }
//    return pRet;
//}

bool ResetPassword::initResetPassword(LogonLayerEx* logoLayer)
{
    m_bPass = false;
    // 确认修改按钮
    m_pConfirmModify = dynamic_cast<Button*>(Tools::getWidgetByName(this,std::string("bt_resetPassword")));
    m_pConfirmModify->addTouchEventListener(CC_CALLBACK_2(ResetPassword::onMenuItemClicked, this));

    // 返回键
    m_pBackmenu = dynamic_cast<Button*>(Tools::getWidgetByName(this,std::string("bt_backReset")));
    m_pBackmenu->addTouchEventListener(CC_CALLBACK_2(ResetPassword::onMenuItemClicked, this));

    // 用户输入手机号
    m_pTxtFiledUser = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_phoneReset")));
    m_pTxtFiledUser->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_pTxtFiledUser->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_pTxtFiledUser->setFontSize(24);
    m_pTxtFiledUser->addEventListener(CC_CALLBACK_2(ResetPassword::textFieldEvent, this));
    m_pTxtFiledUser->setPlaceHolderColor(Color3B(0x7c, 0x7c, 0x7c));
    m_pTxtFiledUser->setPlaceHolder("请输入手机号");

    // 获取验证码
    m_pTxtGetCode = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_codeReset")));
    m_pTxtGetCode->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_pTxtGetCode->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_pTxtGetCode->setFontSize(24);
    m_pTxtGetCode->addEventListener(CC_CALLBACK_2(ResetPassword::textFieldEvent, this));
    m_pTxtGetCode->setPlaceHolderColor(Color3B(0x7c, 0x7c, 0x7c));
    m_pTxtGetCode->setPlaceHolder("请输入验证码");

    // 输入密码
    m_pTxtFiledPass = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_passwordReset")));
    m_pTxtFiledPass->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_pTxtFiledPass->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_pTxtFiledPass->setFontSize(24);
    m_pTxtFiledPass->addEventListener(CC_CALLBACK_2(ResetPassword::textFieldEvent, this));
    m_pTxtFiledPass->setPlaceHolderColor(Color3B(0x7c, 0x7c, 0x7c));
    m_pTxtFiledPass->setPlaceHolder("支持6-24位字符");

    // 获取验证码按钮
    auto SecurityCodeBg = dynamic_cast<Layout*>(Tools::getWidgetByName(this, std::string("Panel_codeReset")));
    m_btGetSecurityCode = createButton("res/ui/securitycode.png", true, 60, Color4B(0, 0, 0, 0), Color4B(0, 0, 0, 40));
    Size sz = SecurityCodeBg->getContentSize();
    Size szSelf = m_btGetSecurityCode->getContentSize();
    m_btGetSecurityCode->setPosition(sz.width - szSelf.width/2, sz.height - szSelf.height/2 + 1);
    m_btGetSecurityCode->SetHitCB(std::bind(&ResetPassword::GetSecurityCode, this));
    SecurityCodeBg->addChild(m_btGetSecurityCode);

    // 动态显示验证码
    m_pUserSpriteImage[0] = dynamic_cast<Button*>(Tools::getWidgetByName(this,std::string("bt_judgePhoneReset")));
    m_pUserSpriteImage[0]->addTouchEventListener(CC_CALLBACK_1(ResetPassword::menuWrongOrRight, this));
    m_pUserSpriteImage[0]->setVisible(false);

    m_pUserSpriteImage[1] = dynamic_cast<Button*>(Tools::getWidgetByName(this,std::string("bt_judgeCodeReset")));
    m_pUserSpriteImage[1]->addTouchEventListener(CC_CALLBACK_1(ResetPassword::menuWrongOrRight, this));
    m_pUserSpriteImage[1]->setVisible(false);

    m_LogoLayerEx = logoLayer;

    return true;
}

void ResetPassword::textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    if (type == TextField::EventType::DETACH_WITH_IME)
    {
        if(sender == m_pTxtFiledUser)	// 判断用户名对错显示的图片标志
        {
            std::string strPhone = m_pTxtFiledUser->getString();
            if (strPhone.length() == 11)
            {
                m_pUserSpriteImage[0]->setVisible(true);
                m_pUserSpriteImage[0]->setBright(false);
            }
            else
            {
                m_pUserSpriteImage[0]->setVisible(true);
                m_pUserSpriteImage[0]->setBright(true);
            }
        }
        if(sender == m_pTxtGetCode)	// 判断验证码对错显示的图片标志
        {
            std::string strPasswd = m_pTxtGetCode->getString();
            if(strPasswd.length() > 0)
            {
                m_pUserSpriteImage[0]->setVisible(true);
                m_pUserSpriteImage[1]->setVisible(true);
                m_pUserSpriteImage[1]->setBright(false);
            }
            else
            {
                m_pUserSpriteImage[1]->setVisible(true);
                m_pUserSpriteImage[1]->setBright(true);
            }
        }
    }
}

//void ResetPassword::editBoxReturn(cocos2d::ui::EditBox* editBox)
//{
//	if(editBox == m_pTxtFiledUser)	// 判断用户名对错显示的图片标志
//	{
//		std::string strPhone = m_pTxtFiledUser->getText();
//		if (strPhone.length() == 11)
//		{
//			m_pUserSpriteImage[0]->SetFrameIdx(1);
//			m_pUserSpriteImage[0]->setVisible(true);
//		}
//		else
//		{
//			m_pUserSpriteImage[0]->SetFrameIdx(0);
//			m_pUserSpriteImage[0]->setVisible(true);
//			m_pUserSpriteImage[0]->setPosition(425, 293);
//		}
//	}
//	if(editBox == m_pTxtGetCode)	// 判断验证码对错显示的图片标志
//	{
//		std::string strPasswd = m_pTxtGetCode->getText();
//		if(strPasswd.length() > 0)
//		{
//			m_pUserSpriteImage[1]->SetFrameIdx(1);
//			m_pUserSpriteImage[0]->setVisible(true);
//			m_pUserSpriteImage[1]->setVisible(true);
//			m_pUserSpriteImage[1]->setPosition(425, 215);
//		}
//		else
//		{
//			m_pUserSpriteImage[1]->SetFrameIdx(0);
//			m_pUserSpriteImage[1]->setVisible(true);
//			m_pUserSpriteImage[1]->setPosition(425, 215);
//		}
//	}
//	return;
//}

void ResetPassword::menuWrongOrRight(cocos2d::Ref* pSender){}

//void ResetPassword::onGrabTouchBegan(bool bHitDialog)
//{
//	if (!bHitDialog)
//	{
//		auto pParent = getParent();
//		LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
//		pLogon->ShowAccountLogon();
//	}
//}
//void ResetPassword::onKeyEscPressed()
//{
//	auto pParent = getParent();
//	LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
//	pLogon->ShowAccountLogon();
//}

void ResetPassword::GetSecurityCode()
{
	std::string strPhone = m_pTxtFiledUser->getString();
	if (strPhone.length() == 11)
	{
		Json::Value param;
		param["phone"] = strPhone;
		SendClientRequest(ClientRequest::kGetRegisterCode, param);
	}
	else
	{
		m_btGetSecurityCode->SetEnable(true);
	}
}

void ResetPassword::onMenuItemClicked(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        if (pMenuItem == m_pConfirmModify)
        {
            std::string strPhone = m_pTxtFiledUser->getString();
            std::string strCode = m_pTxtGetCode->getString();
            std::string strPwd = m_pTxtFiledPass->getString();
            if (strPhone.length() == 11 && strCode.length() > 0 && strPwd.length() > 0)
            {
                Json::Value param;
                param["phone"] = strPhone;
                param["code"] = strCode;
                param["pwd"] = strPwd;
                SendClientRequest(ClientRequest::kResetPassword, param);

                auto pParent = getParent();
                LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
                pLogon->ShowLoading();
                m_bPass = true;
            }
            if(m_bPass)
            {
                m_LogoLayerEx->ShowLoading();
            }
        }
        else if (pMenuItem == m_pBackmenu)
        {
            m_LogoLayerEx->ShowAccountLogon();
        }
    }
}

