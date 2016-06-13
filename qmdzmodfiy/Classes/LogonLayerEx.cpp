#include "LogonLayerEx.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "AccountLogin.h"
#include "RegisterView.h"
#include "ResetPassword.h"
#include "QuickLogonDlg.h"
#include "PlatformSpec.h"
#include "AnimateViewer.h"
#include "MenuItemSpriteWithLabel.h"
#include "Tools.h"
#include "GameData.h"
#include "AnimationManager.h"
#include "GameFrame.h"
#include "GameConfig.h"
#include "FrameSpriteButton.h"
#include "TrackManager.h"
#include "LoadManager.h"
USING_NS_CC;


using namespace cocos2d::ui;

void LogonLoading::setVisible(bool bVisible)
{
    if (isVisible() != bVisible)
    {
        GameLayer::setVisible(bVisible);
        m_pAnimateViewer->setVisible(bVisible);
    }
}

bool LogonLoading::init(/*cocos2d::ui::Layout* loading*/)
{
	if (!GameLayer::init(460, 380, true, false))
	{
		return false;
	} 
	m_pAnimateViewer = AnimateViewer::create("login/loading.png", true, 0.1, 3, 3, 8);
	m_pAnimateViewer->setPosition(200, 190);
	addChild(m_pAnimateViewer);
	m_pAnimateViewer->setVisible(true);
	auto label = CreateLabelMSYH(24, "正在登陆...");
	label->setPosition(295, 190);
	addChild(label);

	return true;
}








////////////////////////////////////////////////////////////
void LogonLayerEx::ShowQuickLogon()
{
    m_uCurLoginUI = 1;
	ReplaceVisibleView(m_pQuickLogonDlg);
}

void LogonLayerEx::ShowAccountLogon()
{
    m_uCurLoginUI = 2;
	ReplaceVisibleView(m_pAccountLogonDlg);
}

void LogonLayerEx::ShowRegisit()
{
    m_uCurLoginUI = 3;
	ReplaceVisibleView(m_pRegisitDlg);
}

void LogonLayerEx::ShowResetPwd()
{
    m_uCurLoginUI = 4;
	ReplaceVisibleView(m_pResetPwdDlg);
}

void LogonLayerEx::ShowLicence()
{
    if (m_pLicenceViewDlg == nullptr)
    {
        m_pLicenceViewDlg = UserProtocol::create();
        m_pLicenceViewDlg->setPosition(0, 0);
        m_pLicenceViewDlg->setVisible(false);
        addChild(m_pLicenceViewDlg);
    }
    ReplaceVisibleView(m_pLicenceViewDlg);
    /*m_pLicenceViewDlg->setVisible(true);
    m_pNodeVisible = m_pLicenceViewDlg;*/

}
void LogonLayerEx::ShowLoading()
{
    if (m_pNodeVisible)
    {
        m_pNodeVisible->setVisible(false);
    }
	if (m_pLogonLoading == nullptr)
	{
		m_pLogonLoading = LogonLoading::create();
		Size sz = m_pLogonLoading->getContentSize();
		m_pLogonLoading->setPosition(Director::getInstance()->getVisibleSize().width - sz.width, 80);
		m_pLogonLoading->setVisible(false);
		addChild(m_pLogonLoading);
	}
    m_pLogonLoading->setVisible(true);
    m_pNodeVisible = m_pLogonLoading;

    m_pPrevLoading = m_pLogonLoading;


	show3rdLoginButton(false) ;


}

bool LogonLayerEx::IsLoading()
{
	return m_pPrevLoading != nullptr;
}

void LogonLayerEx::CancelLoading()
{
	if (m_pNodeVisible == m_pLogonLoading && m_pLogonLoading != nullptr)
	{
		m_pLogonLoading->setVisible(false);
		m_pNodeVisible = m_pPrevLoading;
		m_pPrevLoading = nullptr;
		if (m_pNodeVisible) {
			m_pNodeVisible->setVisible(true);
		}
	}


	show3rdLoginButton(true) ;


}

bool LogonLayerEx::init()
{
	if (!GameDialog::init())
	{
		return false;
	}

    auto LoginBg = Sprite::create("res/ui/loginbg.png");
    Size sz = this->getContentSize();
    LoginBg->setPosition(sz.width/2, sz.height/2);
    this->addChild(LoginBg);

	LoginBg->setScale(GameData::getInstance()->getGameScale());
    auto rootNode = CSLoader::createNode("res/ui/UILogin.csb");
    Size frameSize = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(frameSize);
    ui::Helper::doLayout(rootNode);
    addChild(rootNode);





    m_pQuickLogonDlg = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode,"panel_quickLogin"));
    initQuickDlg();
    //m_pQuickLogonDlg->initQuickDlg(this);

    m_pAccountLogonDlg = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode,"panel_accountLogin"));
    initAccountLogin();
    //m_pAccountLogonDlg->initAccountLogin(this);

    m_pRegisitDlg = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode,"panel_regisertView"));
    initRegisterView();

    m_pResetPwdDlg = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode,"panel_resetPassword"));
    initResetPassword();
    //m_pResetPwdDlg->initResetPassword(this);

    m_pQuickLogonDlg->setVisible(false);
    m_pAccountLogonDlg->setVisible(false);
    m_pRegisitDlg->setVisible(false);
    m_pResetPwdDlg->setVisible(false);

    m_pLicenceViewDlg = nullptr;
    m_pLogonLoading = nullptr;
    m_pNodeVisible = nullptr;
    m_uCurLoginUI = 0;


	if (m_pLogonLoading == nullptr)
	{
		m_pLogonLoading = LogonLoading::create();
		Size sz = m_pLogonLoading->getContentSize();
		m_pLogonLoading->setPosition(Director::getInstance()->getVisibleSize().width - sz.width, 80);
		m_pLogonLoading->setVisible(false);
		addChild(m_pLogonLoading);
	}



#if Use_3rd_Login
	rootNode->setVisible(false) ;

	spriteLogin = MenuItemSpriteWithLabel::create("login/loginbutton.png", 2, CC_CALLBACK_0(LogonLayerEx::callLogin, this));
	spriteLogin->setPosition(850,300) ;

	auto AchievementRecord = Menu::create(spriteLogin,nullptr);
	AchievementRecord->setPosition(Vec2::ZERO);
	addChild(AchievementRecord);
#endif

	LoadingMgr->delayLoadingAnims();

	return true;
}



void LogonLayerEx::callLogin()
{

	call3rdLogin() ;
}

void LogonLayerEx::show3rdLoginButton(bool flag)
{
#if Use_3rd_Login
	spriteLogin->setVisible(flag) ;
#endif
	
}

bool LogonLayerEx::ifLoadingVisible()
{
	return m_pLogonLoading->isVisible();
}

void LogonLayerEx::onScheduleIfLogin(float fDelta)
{
	if(ifLoadingVisible() && false == g_pGameFrame->getLoginFlag())
	{
		CancelLoading();
		ShowQuickLogon();
		PopTipWithBt1("请检查网络状况或者稍后重试！","确定");

		show3rdLoginButton(true) ;
		g_pGameFrame->cleanLogin() ;
	}
}

void LogonLayerEx::onGrabTouchBegan(bool bHitDialog)
{

}

void LogonLayerEx::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		ShowQuickLogon();

		show3rdLoginButton(true) ;

	}
	else
	{
		if (m_pNodeVisible)
		{
			m_pLogonLoading->setVisible(false);
			m_pNodeVisible = nullptr;
			m_pPrevLoading = nullptr;
		}

		unschedule(schedule_selector(LogonLayerEx::onScheduleIfLogin));
	}
}
void LogonLayerEx::onKeyEscPressed()
{
    //0:登陆动画  1:快速登陆  2:账号登陆  3:注册界面  4:重置密码  5:用户协议
    switch(m_uCurLoginUI)
    {
    case 0:{SendClientRequest(ClientRequest::kExit);}break;
    case 1:{SendClientRequest(ClientRequest::kExit);}break;
    case 2:{ShowQuickLogon();}break;
    case 3:{ShowAccountLogon();}break;
    case 4:{ShowAccountLogon();}break;
    }
}

bool WaitBeginGame::init()
{
	if (!GameLayer::init(200, 50, false, false))
	{
		return false;
	} 

	m_pAnimateViewer = AnimateViewer::create("login/waitbegin.png", true, 0.1, 3, 3, 8);
	m_pAnimateViewer->setPosition(20, 25);
	addChild(m_pAnimateViewer);
	m_pAnimateViewer->setVisible(true);
	auto label = CreateLabelMSYH(24, "等待开局中...");
	label->setAnchorPoint(Vec2(0, 0.5));
	label->setPosition(53, 25);
	addChild(label);
	return true;
}

void WaitBeginGame::setVisible(bool bVisible)
{
	GameLayer::setVisible(bVisible);
	m_pAnimateViewer->setVisible(bVisible);
}


bool DataLoading::init()
{
	if (!GameLayer::init(200, 50, false, false))
	{
		return false;
	} 

	//m_pAnimateViewer = AnimateViewer::create("login/waitbegin.png", true, 0.1, 4, 1);


	m_pAnimateViewer = AnimationManager::creatArmature("flower") ;


	m_pAnimateViewer->setPosition(100, 24+25);
	addChild(m_pAnimateViewer);
	m_pAnimateViewer->playByIndexAndRepatForever(0) ;




	m_pAnimateViewer->setVisible(true);
	auto label = CreateLabelMSYH(24, "正在载入中...");
	label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setPosition(100, -24+25);
	addChild(label);
	return true;
}

void DataLoading::setVisible(bool bVisible)
{
	GameLayer::setVisible(bVisible);
	m_pAnimateViewer->setVisible(bVisible);
}

//void LogonLayerEx::onExit()
//{
//    Layer::onExit() ;
//   CSLoader::getInstance()->destroyInstance();
//
//}
//////////////////////////////////////////////////////////////////////////
bool LogonLayerEx::initQuickDlg()
{
    m_pMenuItemLoginByGuest = static_cast<Button*>(Tools::getWidgetByName(m_pQuickLogonDlg,"bt_youkeLogin"));
    m_pMenuItemLoginByGuest->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuClickQuickGame,this));
    m_pMenuItemLoginByGuest->setTag(0);

    m_pMenuItemLoginByWeiXin = static_cast<Button*>(Tools::getWidgetByName(m_pQuickLogonDlg,"bt_weixinLogin"));
    m_pMenuItemLoginByWeiXin->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuItemClick,this));
    m_pMenuItemLoginByWeiXin->setTag(1);

	

    m_pMenuItemLoginByQQ =  static_cast<Button*>(Tools::getWidgetByName(m_pQuickLogonDlg,"bt_qqLogin"));
    m_pMenuItemLoginByQQ->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuItemClick,this));
    m_pMenuItemLoginByQQ->setTag(2);

    m_pMenuItemLoginByAccount = static_cast<Button*>(Tools::getWidgetByName(m_pQuickLogonDlg,"bt_zhanghaoLogin"));
    m_pMenuItemLoginByAccount->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuItemClick,this));
    m_pMenuItemLoginByAccount->setTag(3);

    m_pMenuItemLoginByWeiBo = static_cast<Button*>(Tools::getWidgetByName(m_pQuickLogonDlg,"bt_weiboLogin"));
    m_pMenuItemLoginByWeiBo->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuItemClick,this));
    m_pMenuItemLoginByWeiBo->setTag(4);


	//m_pMenuItemLoginByWeiXin->setVisible(false) ;
	//m_pMenuItemLoginByQQ->setVisible(false) ;
	//m_pMenuItemLoginByWeiBo->setVisible(false) ;

	//m_pMenuItemLoginByAccount->setPosition(Point(m_pMenuItemLoginByGuest->getPositionX(),m_pMenuItemLoginByGuest->getPositionY()-150)) ;

    return true;
}

//快速游戏
void LogonLayerEx::menuClickQuickGame(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        if (checkNetworkState())
        {
            SendClientRequest(ClientRequest::kGuestLogon);
            ShowLoading();
            scheduleOnce(schedule_selector(LogonLayerEx::onScheduleIfLogin), 20.0f);
        }
        else
        {
            PopTipWithBt1(NETWORK_DISCONNECT_STRING, "确定");
        }
    }
}

//登陆回调
void LogonLayerEx::menuItemClick(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        TrackMgr->SetPreTime(getSystemTime());
        auto item = (Button*)pMenuItem;
        if (checkNetworkState())
        {
            if(item->getTag() == 3)
            {
                ShowAccountLogon();
            }
            else if(item->getTag() == 1)
            {
                Json::Value param;
                param["3rd"] = 1;
                SendClientRequest(ClientRequest::kLogonBy3rd, param);
                scheduleOnce(schedule_selector(LogonLayerEx::onScheduleIfLogin), 20.0f);
            }
            else if(item->getTag() == 2)
            {
                Json::Value param;
                param["3rd"] = 0;
                SendClientRequest(ClientRequest::kLogonBy3rd, param);
                scheduleOnce(schedule_selector(LogonLayerEx::onScheduleIfLogin), 20.0f);
            }
            else if(item->getTag() == 4)
            {
				Json::Value param;
				param["3rd"] = 2;
				SendClientRequest(ClientRequest::kLogonBy3rd, param);
                scheduleOnce(schedule_selector(LogonLayerEx::onScheduleIfLogin), 20.0f);
				//PopTipWithBt1("微博登录功能暂未开放，敬请期待中~","确定");
            }			
        }
        else
        {
            PopTipWithBt1(NETWORK_DISCONNECT_STRING, "确定");
        }
    }
}

bool LogonLayerEx::initRegisterView()
{
    //手机号编辑框
    auto panelPhoneNum = dynamic_cast<cocos2d::ui::Layout*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("Panel_phoneReg")));
    Size szPhoneNum = panelPhoneNum->getContentSize();
    m_editPhoneNum = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPhoneNum.width, szPhoneNum.height), cocos2d::ui::Scale9Sprite::create("res/ui/bgedit.png"), szPhoneNum.width/2, szPhoneNum.height/2, panelPhoneNum, 0);
    m_editPhoneNum->setFont("fzlt.ttf", 22);
    m_editPhoneNum->setFontColor(Color3B::BLACK);
    m_editPhoneNum->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
    m_editPhoneNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editPhoneNum->setDelegate(this);
    m_editPhoneNum->setPlaceholderFontColor(Color3B(0x7c, 0x7c, 0x7c));
    m_editPhoneNum->setPlaceHolder("请输入手机号");

    //验证码编辑框
    auto panelSecurityCode = dynamic_cast<cocos2d::ui::Layout*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("Panel_codeReg")));
    Size szSecurityCode = panelPhoneNum->getContentSize();
    m_editSecurityCode = CreateNode2AndPosAddTo<ui::EditBox>(Size(szSecurityCode.width/2, szSecurityCode.height), cocos2d::ui::Scale9Sprite::create("res/ui/bgedit.png"), szSecurityCode.width/4, szSecurityCode.height/2, panelSecurityCode, 0);
    m_editSecurityCode->setFont("fzlt.ttf", 22);
    m_editSecurityCode->setFontColor(Color3B::BLACK);
    m_editSecurityCode->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editSecurityCode->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editSecurityCode->setDelegate(this);
    m_editSecurityCode->setPlaceholderFontColor(Color3B(0x7c, 0x7c, 0x7c));
    m_editSecurityCode->setPlaceHolder("请输入验证码");

    //密码编辑框
    auto panelPassword = dynamic_cast<cocos2d::ui::Layout*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("Panel_passwordReg")));
    Size szPassword = panelPhoneNum->getContentSize();
    m_editPassword = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPassword.width, szPassword.height), cocos2d::ui::Scale9Sprite::create("res/ui/bgedit.png"), szPassword.width/2, szPassword.height/2, panelPassword, 0);
    m_editPassword->setFont("fzlt.ttf", 22);
    m_editPassword->setFontColor(Color3B::BLACK);
    m_editPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    m_editPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editPassword->setDelegate(this);
    m_editPassword->setPlaceholderFontColor(Color3B(0x7c, 0x7c, 0x7c));
    m_editPassword->setPlaceHolder("支持6-24位字符");

    //获取验证码按钮
    auto SecurityCodeBg = dynamic_cast<Layout*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("Panel_codeReg")));
    m_btGetSecurityCodeReg = createButton("res/ui/securitycode.png", true, 60, Color4B(0, 0, 0, 0), Color4B(0, 0, 0, 40));
    Size sz = SecurityCodeBg->getContentSize();
    Size szSelf = m_btGetSecurityCodeReg->getContentSize();
    m_btGetSecurityCodeReg->setPosition(sz.width - szSelf.width/2, sz.height - szSelf.height/2 + 1);
    m_btGetSecurityCodeReg->SetHitCB(std::bind(&LogonLayerEx::GetSecurityCodeReg, this));
    SecurityCodeBg->addChild(m_btGetSecurityCodeReg);

    //授权并注册按钮
    m_btAuthorityAndRegster = dynamic_cast<Button*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("bt_receiveAndReg")));
    m_btAuthorityAndRegster->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::AuthorityAndRegister, this));

    //返回按钮
    m_btExitRegister = dynamic_cast<Button*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("bt_backReg")));
    m_btExitRegister->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::ExitRegister, this));

    //输入是否正确
    m_pUserSpriteImageReg[0] = dynamic_cast<Button*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("bt_judgePhoneReg")));
    m_pUserSpriteImageReg[0]->addTouchEventListener(CC_CALLBACK_1(LogonLayerEx::menuWrongOrRightReg, this));
    m_pUserSpriteImageReg[0]->setVisible(false);

    m_pUserSpriteImageReg[1] = dynamic_cast<Button*>(Tools::getWidgetByName(m_pRegisitDlg, std::string("bt_judgeCodeReg")));
    m_pUserSpriteImageReg[1]->addTouchEventListener(CC_CALLBACK_1(LogonLayerEx::menuWrongOrRightReg, this));
    m_pUserSpriteImageReg[1]->setVisible(false);

    //用户协议
    auto labelRead = CreateLabelMSYH(20, "查看用户协议");// Label::createWithTTF(, "fzlt.ttf", 20);//查看用户协议
    labelRead->setTextColor(Color4B(0xa5, 0xa5, 0xa5, 0xff));
    auto btRead = MenuItemLabel::create(labelRead, CC_CALLBACK_1(LogonLayerEx::ReadUserProtocol, this));
    auto posBt = m_btAuthorityAndRegster->getPosition();
    //auto szBt = m_btAuthorityAndRegster->getContentSize();
    auto posPanel = panelPassword->getPosition();
    btRead->setPosition(posBt.x, posBt.y/2 + posPanel.y/2);
    btRead->setAnchorPoint(Vec2(0.5, 0.5));
    auto menu = Menu::create(btRead, nullptr);
    menu->setPosition(Vec2::ZERO);
    m_pRegisitDlg->addChild(menu);

    return true;
}

void LogonLayerEx::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    if(editBox == m_editPhoneNum)	// 判断用户名对错显示的图片标志
    {
        std::string strPhone = m_editPhoneNum->getText();
        if (strPhone.length() == 11)
        {
            m_pUserSpriteImageReg[0]->setVisible(true);
            m_pUserSpriteImageReg[0]->setBright(false);
        }
        else
        {
            m_pUserSpriteImageReg[0]->setVisible(true);
            m_pUserSpriteImageReg[0]->setBright(true);
        }
    }
    else if(editBox == m_editSecurityCode)	// 判断验证码对错显示的图片标志
    {
        std::string strPasswd = m_editSecurityCode->getText();
        if(strPasswd.length() > 0)
        {
            m_pUserSpriteImageReg[0]->setVisible(true);
            m_pUserSpriteImageReg[1]->setVisible(true);
            m_pUserSpriteImageReg[1]->setBright(false);
        }
        else
        {
            m_pUserSpriteImageReg[1]->setVisible(true);
            m_pUserSpriteImageReg[1]->setBright(true);
        }
    }
    else if(editBox == m_pTxtFiledUserReset)	// 判断用户名对错显示的图片标志
    {
        std::string strPhone = m_pTxtFiledUserReset->getText();
        if (strPhone.length() == 11)
        {
            m_pUserSpriteImageReset[0]->setVisible(true);
            m_pUserSpriteImageReset[0]->setBright(false);
        }
        else
        {
            m_pUserSpriteImageReset[0]->setVisible(true);
            m_pUserSpriteImageReset[0]->setBright(true);
        }
    }
    else if(editBox == m_pTxtGetCode)	// 判断验证码对错显示的图片标志
    {
        std::string strPasswd = m_pTxtGetCode->getText();
        if(strPasswd.length() > 0)
        {
            m_pUserSpriteImageReset[0]->setVisible(true);
            m_pUserSpriteImageReset[1]->setVisible(true);
            m_pUserSpriteImageReset[1]->setBright(false);
        }
        else
        {
            m_pUserSpriteImageReset[1]->setVisible(true);
            m_pUserSpriteImageReset[1]->setBright(true);
        }
    }
}

void LogonLayerEx::GetSecurityCodeReg()
{
    std::string strPhone = m_editPhoneNum->getText();
    if (strPhone.length() == 11)
    {
        Json::Value param;
        param["phone"] = strPhone;
        param["flag"] = "reg";
        SendClientRequest(ClientRequest::kGetRegisterCode, param);
    }
    else
    {
        m_btGetSecurityCodeReg->SetEnable(true);
    }
}

void LogonLayerEx::AuthorityAndRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        std::string strPhone = m_editPhoneNum->getText();
        std::string strCode = m_editSecurityCode->getText();
        std::string strPwd = m_editPassword->getText();
        if (strPhone.length() == 11 && strCode.length() > 0 && strPwd.length() > 0)
        {
            Json::Value param;
            param["phone"] = strPhone;
            param["code"] = strCode;
            param["pwd"] = strPwd;
            SendClientRequest(ClientRequest::kRegisit, param);

            ShowLoading();
			scheduleOnce(schedule_selector(LogonLayerEx::onScheduleIfLogin), 20.0f);
        }
		else
		{
			PopTipWithBt1("对不起您输入的有误,请重新输入手机号码和密码！","确定");
		}
    }
}
void LogonLayerEx::menuWrongOrRightReg(Ref* pSender){}

void LogonLayerEx::ReadUserProtocol(cocos2d::Ref* pSender)
{
    ShowLicence();  
}

void LogonLayerEx::ExitRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        ShowAccountLogon();
    }
}

bool LogonLayerEx::initAccountLogin()
{
    //账号
    auto panel_Count = dynamic_cast<Layout*>(Tools::getWidgetByName(m_pAccountLogonDlg, std::string("Panel_Count")));
    Size szCount = panel_Count->getContentSize();
    m_pTxtFiledUser = CreateNode2AndPosAddTo<ui::EditBox>(Size(szCount.width, szCount.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szCount.width/2, szCount.height/2, panel_Count, 0);
    m_pTxtFiledUser->setFont("fzlt.ttf", 24);
    m_pTxtFiledUser->setFontColor(Color3B::BLACK);
    m_pTxtFiledUser->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pTxtFiledUser->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_pTxtFiledUser->setDelegate(this);

    //密码
    auto panel_Password = dynamic_cast<Layout*>(Tools::getWidgetByName(m_pAccountLogonDlg, std::string("Panel_Password")));
    Size szPassword = panel_Count->getContentSize();
    m_pTxtFiledPwd = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPassword.width, szPassword.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szPassword.width/2, szPassword.height/2, panel_Password, 0);
    m_pTxtFiledPwd->setFont("fzlt.ttf", 24);
    m_pTxtFiledPwd->setFontColor(Color3B::BLACK);
    m_pTxtFiledPwd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    m_pTxtFiledPwd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pTxtFiledPwd->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_pTxtFiledPwd->setDelegate(this);

    //立即登陆
    m_pMenuItemLoginNow = dynamic_cast<Button*>(Tools::getWidgetByName(m_pAccountLogonDlg, std::string("bt_loginNow")));
    m_pMenuItemLoginNow->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuClickLoginNow, this));

    //其它方式登陆
    m_pMenuItemLoginByOther = dynamic_cast<Button*>(Tools::getWidgetByName(m_pAccountLogonDlg, std::string("bt_loginOther")));
    m_pMenuItemLoginByOther->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::menuClickLoginOther, this));

    //注册账号
    auto labelRegister = Label::createWithTTF("","fzlt.ttf",22);
    SetLabelString(labelRegister,"注册账号");
    labelRegister->setTextColor(Color4B(0x39,0x9f,0x56,0xff));
    auto registerAccount = MenuItemLabel::create(labelRegister,CC_CALLBACK_1(LogonLayerEx::menuClickAccountRegister,this));
    //registerAccount->setAnchorPoint(Vec2(0, 0.5));
    /*auto pos = m_pTxtFiledUser->getWorldPosition();*/
    auto sz = panel_Count->getContentSize();
    registerAccount->setPosition(sz.width + registerAccount->getContentSize().width/2 + 10, sz.height/2);
    auto menu = Menu::create(registerAccount,nullptr);
    menu->setPosition(Vec2::ZERO);
    panel_Count->addChild(menu);

    auto layerBtnReg = LayerButton::create(false, true);
    panel_Count->addChild(layerBtnReg);
    layerBtnReg->setAnchorPoint(Vec2(0.5, 0.5));
    layerBtnReg->setContentSize(Size(labelRegister->getContentSize().width, sz.height));
    layerBtnReg->setPosition(sz.width + 10, 0);
    layerBtnReg->SetHitCB([this](Ref*)
    {
        ShowRegisit();
    });

    //忘记密码
    auto labelForget = Label::createWithTTF("","fzlt.ttf",22);
    SetLabelString(labelForget,"忘记密码");
    labelForget->setTextColor(Color4B(0x39,0x9f,0x56,0xff));
    auto forgetPwd = MenuItemLabel::create(labelForget,CC_CALLBACK_1(LogonLayerEx::menuClickForgetPwd,this));
    sz = panel_Password->getContentSize();
    forgetPwd->setPosition(sz.width + forgetPwd->getContentSize().width/2 + 10, sz.height/2);
    menu = Menu::create(forgetPwd,nullptr);
    menu->setPosition(Vec2::ZERO);
    panel_Password->addChild(menu);

    auto layerBtnPwd = LayerButton::create(false, true);
    panel_Password->addChild(layerBtnPwd);
    layerBtnPwd->setAnchorPoint(Vec2(0.5, 0.5));
    layerBtnPwd->setContentSize(Size(labelForget->getContentSize().width, sz.height));
    layerBtnPwd->setPosition(sz.width + 10, 0);
    layerBtnPwd->SetHitCB([this](Ref*)
    {
        ShowResetPwd();
    });

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
    auto posLoginNow = m_pMenuItemLoginNow->getPosition();
    auto posPanel = panel_Password->getPosition();
    auto szLoginNow = m_pMenuItemLoginNow->getContentSize();
    m_pRememberPwd->setAnchorPoint(Vec2(0, 0.5));
    m_pRememberPwd->setPosition(posLoginNow.x - szLoginNow.width/2 + 10, posLoginNow.y/2 + posPanel.y/2);

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
    m_pSelfLogin->setAnchorPoint(Vec2(1, 0.5));
    m_pSelfLogin->setPosition(posLoginNow.x + szLoginNow.width/2 - 10, posLoginNow.y/2 + posPanel.y/2);

    menu = Menu::create(m_pRememberPwd,m_pSelfLogin,nullptr);
    menu->setPosition(Vec2::ZERO);
    m_pAccountLogonDlg->addChild(menu);

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

    return true;
}

//注册账号
void LogonLayerEx::menuClickAccountRegister(cocos2d::Ref* pSender)
{
	//ShowRegisit();
}

//忘记密码
void LogonLayerEx::menuClickForgetPwd(cocos2d::Ref* pSender)
{
	//ShowResetPwd();
}

//立即登录
void LogonLayerEx::menuClickLoginNow(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        Json::Value param;
        param["account"] = m_pTxtFiledUser->getText();
        param["password"] = m_pTxtFiledPwd->getText();

		if (param["account"]==""||param["password"]==""||param["password"]==" ")
		{
			SendGameTip("用户名或密码不能为空");
		}
		else
		{
			SendClientRequest(ClientRequest::kUserLogon, param);
			bool bRemPwd = m_pRememberPwd->getSelectedIndex() == 1;
			bool bAutoLogin = m_pSelfLogin->getSelectedIndex() == 1;
			g_globalConfig.SetLastLogonInfo(m_pTxtFiledUser->getText(), m_pTxtFiledPwd->getText(), bRemPwd, bAutoLogin, false);
		    ShowLoading();
			scheduleOnce(schedule_selector(LogonLayerEx::onScheduleIfLogin), 20.0f);
		} 
    }
}

//其他方式登陆
void LogonLayerEx::menuClickLoginOther(cocos2d::Ref* pSender,  cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        ShowQuickLogon();
    }
	
}

void LogonLayerEx::menuDefaultSelect(cocos2d::Ref* pSender)
{

}

bool LogonLayerEx::initResetPassword()
{
    m_bPass = false;
    // 确认修改按钮
    m_pConfirmModify = dynamic_cast<Button*>(Tools::getWidgetByName(m_pResetPwdDlg,std::string("bt_resetPassword")));
    m_pConfirmModify->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::onMenuItemClicked, this));

    // 返回键
    m_pBackmenu = dynamic_cast<Button*>(Tools::getWidgetByName(m_pResetPwdDlg,std::string("bt_backReset")));
    m_pBackmenu->addTouchEventListener(CC_CALLBACK_2(LogonLayerEx::onMenuItemClicked, this));

    // 用户输入用户名
    auto panelPhoneReset = dynamic_cast<cocos2d::ui::Layout*>(Tools::getWidgetByName(m_pResetPwdDlg, std::string("Panel_phoneReset")));
    Size szPhoneReset = panelPhoneReset->getContentSize();
    m_pTxtFiledUserReset = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPhoneReset.width, szPhoneReset.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szPhoneReset.width/2, szPhoneReset.height/2, panelPhoneReset, 0);
    m_pTxtFiledUserReset->setFont("fzlt.ttf", 22);
    m_pTxtFiledUserReset->setFontColor(Color3B::BLACK);
    m_pTxtFiledUserReset->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
    m_pTxtFiledUserReset->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_pTxtFiledUserReset->setDelegate(this);
    m_pTxtFiledUserReset->setPlaceholderFontColor(Color3B(0x7c, 0x7c, 0x7c));
    m_pTxtFiledUserReset->setPlaceHolder("请输入手机号码");

    // 获取验证码
    auto panelCodeReset = dynamic_cast<cocos2d::ui::Layout*>(Tools::getWidgetByName(m_pResetPwdDlg, std::string("Panel_codeReset")));
    Size szCodeReset = panelPhoneReset->getContentSize();
    m_pTxtGetCode = CreateNode2AndPosAddTo<ui::EditBox>(Size(szCodeReset.width/2, szCodeReset.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szCodeReset.width/4, szCodeReset.height/2, panelCodeReset, 0);
    m_pTxtGetCode->setFont("fzlt.ttf", 22);
    m_pTxtGetCode->setFontColor(Color3B::BLACK);
    m_pTxtGetCode->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pTxtGetCode->setDelegate(this);
    m_pTxtGetCode->setPlaceholderFontColor(Color3B(0x7c, 0x7c, 0x7c));
    m_pTxtGetCode->setPlaceHolder("请输入验证码");


    // 输入密码
    auto panelPasswordReset = dynamic_cast<cocos2d::ui::Layout*>(Tools::getWidgetByName(m_pResetPwdDlg, std::string("Panel_passwordReset")));
    Size szPasswordReset = panelPhoneReset->getContentSize();
    m_pTxtFiledPass = CreateNode2AndPosAddTo<ui::EditBox>(Size(szPasswordReset.width, szPasswordReset.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szPasswordReset.width/2, szPasswordReset.height/2, panelPasswordReset, 0);
    m_pTxtFiledPass->setFont("fzlt.ttf", 22);
    m_pTxtFiledPass->setFontColor(Color3B::BLACK);
    m_pTxtFiledPass->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    m_pTxtFiledPass->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pTxtFiledPass->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_pTxtFiledPass->setDelegate(this);
    m_pTxtFiledPass->setPlaceholderFontColor(Color3B(0x7c, 0x7c, 0x7c));
    m_pTxtFiledPass->setPlaceHolder("支持6-24位字符");

    // 获取验证码按钮
    auto SecurityCodeBg = dynamic_cast<Layout*>(Tools::getWidgetByName(m_pResetPwdDlg, std::string("Panel_codeReset")));
    m_btGetSecurityCodeReset = createButton("res/ui/securitycode.png", true, 60, Color4B(0, 0, 0, 0), Color4B(0, 0, 0, 40));
    Size sz = SecurityCodeBg->getContentSize();
    Size szSelf = m_btGetSecurityCodeReset->getContentSize();
    m_btGetSecurityCodeReset->setPosition(sz.width - szSelf.width/2, sz.height - szSelf.height/2 + 1);
    m_btGetSecurityCodeReset->SetHitCB(std::bind(&LogonLayerEx::GetSecurityCodeReset, this));
    SecurityCodeBg->addChild(m_btGetSecurityCodeReset);

    // 动态显示验证码
    m_pUserSpriteImageReset[0] = dynamic_cast<Button*>(Tools::getWidgetByName(m_pResetPwdDlg,std::string("bt_judgePhoneReset")));
    m_pUserSpriteImageReset[0]->addTouchEventListener(CC_CALLBACK_1(LogonLayerEx::menuWrongOrRightReset, this));
    m_pUserSpriteImageReset[0]->setVisible(false);

    m_pUserSpriteImageReset[1] = dynamic_cast<Button*>(Tools::getWidgetByName(m_pResetPwdDlg,std::string("bt_judgeCodeReset")));
    m_pUserSpriteImageReset[1]->addTouchEventListener(CC_CALLBACK_1(LogonLayerEx::menuWrongOrRightReset, this));
    m_pUserSpriteImageReset[1]->setVisible(false);

    return true;
}

void LogonLayerEx::menuWrongOrRightReset(cocos2d::Ref* pSender){}

void LogonLayerEx::GetSecurityCodeReset()
{
    std::string strPhone = m_pTxtFiledUserReset->getText();
    if (strPhone.length() == 11)
    {
        Json::Value param;
        param["phone"] = strPhone;
        param["flag"] = "reset";
        SendClientRequest(ClientRequest::kGetRegisterCode, param);
    }
    else
    {
        m_btGetSecurityCodeReset->SetEnable(true);
    }
}

void LogonLayerEx::onMenuItemClicked(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        if (pMenuItem == m_pConfirmModify)
        {
            std::string strPhone = m_pTxtFiledUserReset->getText();
            std::string strCode = m_pTxtGetCode->getText();
            std::string strPwd = m_pTxtFiledPass->getText();
            if (strPhone.length() == 11 && strCode.length() > 0 && strPwd.length() > 0)
            {
                Json::Value param;
                param["phone"] = strPhone;
                param["code"] = strCode;
                param["pwd"] = strPwd;
                SendClientRequest(ClientRequest::kResetPassword, param);

                ShowLoading();
                m_bPass = true;
            }
            if(m_bPass)
            {
                ShowLoading();
            }
        }
        else if (pMenuItem == m_pBackmenu)
        {
            ShowAccountLogon();
        }
    }
}

void LogonLayerEx::setNewPwd()
{
    m_pTxtFiledPwd->setText(m_pTxtFiledPass->getText());
}
//////////////////////////////////////////////////////////////////////////////////////////
