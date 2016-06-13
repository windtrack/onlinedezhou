#ifndef __ACCOUNTLOGIN_H__
#define __ACCOUNTLOGIN_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "ui/CocosGUI.h"
#include "ui/UIEditBox/UIEditBox.h"

class LogonLayerEx;

class AccountLogin : public cocos2d::ui::Layout/* , public cocos2d::ui::EditBoxDelegate*/
{
public:
    bool initAccountLogin(LogonLayerEx* accountLogin);
protected:
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
private:
	//注册账号
	void menuClickAccountRegister(cocos2d::Ref* pSender);
	//忘记密码
	void menuClickForgetPwd(cocos2d::Ref* pSender);
	//记住密码
	void menuClickRememberPwd(cocos2d::Ref* pSender);
	//自动登陆
	void menuClickSelfLogin(cocos2d::Ref* pSender);
	//立即登录
	void menuClickLoginNow(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
	//其他方式登陆
	void menuClickLoginOther(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
    //记住密码，自动登录
    void menuDefaultSelect(cocos2d::Ref* pSender);

private:
	cocos2d::ui::Button				*m_pMenuItemLoginNow;			//立即登陆
	cocos2d::ui::Button				*m_pMenuItemLoginByOther;		//其它方式登陆
	cocos2d::ui::TextField			*m_pTxtFiledUser;				//账号
	cocos2d::ui::TextField			*m_pTxtFiledPwd;				//密码
	cocos2d::MenuItemToggle			*m_pRememberPwd;				//记住密码
	cocos2d::MenuItemToggle			*m_pSelfLogin;					//自动登录
    LogonLayerEx*                    m_accountLogin;
};

#endif