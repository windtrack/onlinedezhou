#ifndef __LOGONLAYEREX_H__
#define __LOGONLAYEREX_H__

#include "GameDialog.h"
#include "BaseButton.h"
#include "ui/CocosGUI.h"

//#include "AnimationManager.h"
//class QuickLogonDlg;
//class AccountLogin;
//class RegisterView;
//class ResetPassword;
class UserProtocol;
class LogonLoading;
class AnimateViewer;
class AnimationManager ;

#define NETWORK_DISCONNECT_STRING "您的网络连接已中断，请您检查网络是否连接正常"
#define NETWORK_RECONNECTING_STRING "正在努力重新连接服务器..."

class LogonLayerEx : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(LogonLayerEx);

	void ShowQuickLogon();
	void ShowAccountLogon();
	void ShowRegisit();
	void ShowResetPwd();
	void ShowLicence();
	void ShowLoading();

	bool IsLoading();
	void CancelLoading();

	bool ifLoadingVisible();

	void onScheduleIfLogin(float fDelta);
    /*void onExit();*/
	void show3rdLoginButton(bool flag) ;

	void callLogin() ;
protected:
	bool init();
	virtual void onSetVisible(bool bVisible) override;
	virtual void onKeyEscPressed() override;
	virtual void onGrabTouchBegan(bool bHitDialog) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
private:
	template <typename NewViewTypeName>
	void ReplaceVisibleView(NewViewTypeName*& pNewView)
	{
		if (m_pNodeVisible)
		{
			m_pNodeVisible->setVisible(false);
		}

		pNewView->setVisible(true);
		m_pNodeVisible = pNewView;
	}

	cocos2d::ui::Layout *m_pQuickLogonDlg;
	cocos2d::ui::Layout *m_pAccountLogonDlg;
	cocos2d::ui::Layout *m_pRegisitDlg;
	cocos2d::ui::Layout *m_pResetPwdDlg;
	UserProtocol *m_pLicenceViewDlg;
	LogonLoading *m_pLogonLoading;

	cocos2d::Node *m_pNodeVisible;
	cocos2d::Node *m_pPrevLoading;
    unsigned m_uCurLoginUI;//当前的登陆界面 0:登陆动画  1:快速登陆  2:账号登陆  3:注册界面  4:重置密码  5:用户协议

	MenuItemSpriteWithLabel* spriteLogin ;
///////////////////////////////////////////////////////////////////////////////////////////////
public:
    bool initQuickDlg();

    bool initRegisterView();

    bool initAccountLogin();

    bool initResetPassword();

    void GetSecurityCodeReg();
    void AuthorityAndRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
    void ReadUserProtocol(cocos2d::Ref* pSender);
    void ExitRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);

    void GetSecurityCodeReset();
    void menuWrongOrRightReg(cocos2d::Ref* pSender);
    // 确认修改的回调事件
    void onMenuItemClicked(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type);

    void setNewPwd();//重置密码后返回账号登录并设置新密码
private:
    //快速游戏
    void menuClickQuickGame(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
    //登陆回调
    void menuItemClick(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type);

    void menuWrongOrRightReset(Ref* pSender);
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
    cocos2d::ui::Button				*m_pMenuItemLoginByGuest;		//快速登陆
    cocos2d::ui::Button				*m_pMenuItemLoginByWeiXin;		//微信登陆
    cocos2d::ui::Button				*m_pMenuItemLoginByQQ;			//QQ登陆
    cocos2d::ui::Button				*m_pMenuItemLoginByAccount;		//账号登陆
    cocos2d::ui::Button				*m_pMenuItemLoginByWeiBo;		//微博登陆

    AutoDelayEnabledFrameSpriteButton       *m_btGetSecurityCodeReg;             //获取验证码
    cocos2d::ui::Button                     *m_btAuthorityAndRegster;         //授权并注册
    cocos2d::ui::Button                     *m_btExitRegister;
    cocos2d::ui::EditBox                  *m_editPhoneNum;
    cocos2d::ui::EditBox                  *m_editSecurityCode;
    cocos2d::ui::EditBox                  *m_editPassword;
    cocos2d::ui::Button						*m_pUserSpriteImageReg[2];	// 用户输入框的按钮
   
    cocos2d::ui::Button				*m_pMenuItemLoginNow;			//立即登陆
    cocos2d::ui::Button				*m_pMenuItemLoginByOther;		//其它方式登陆
    cocos2d::ui::EditBox			*m_pTxtFiledUser;				//账号
    cocos2d::ui::EditBox			*m_pTxtFiledPwd;				//密码
    cocos2d::MenuItemToggle			*m_pRememberPwd;				//记住密码
    cocos2d::MenuItemToggle			*m_pSelfLogin;					//自动登录

    bool m_bPass;	// 是否通过
    cocos2d::ui::Button						*m_pConfirmModify;		// 确认修改
    cocos2d::ui::Button						*m_pBackmenu;			// 返回键
    cocos2d::ui::EditBox					*m_pTxtFiledUserReset;		// 用户名
    cocos2d::ui::Button						*m_pUserSpriteImageReset[2];	// 用户输入框的按钮
    cocos2d::ui::EditBox					*m_pTxtGetCode;			// 获取验证码
    cocos2d::ui::EditBox					*m_pTxtFiledPass;		// 输入密码
    AutoDelayEnabledFrameSpriteButton		*m_btGetSecurityCodeReset;
///////////////////////////////////////////////////////////////////////////////////////////////
};


class LogonLoading : public GameLayer
{
public:
    CREATE_FUNC(LogonLoading);
    static LogonLoading* create(cocos2d::ui::Layout* loading);
    virtual void setVisible(bool bVisible);
    virtual bool init(/*cocos2d::ui::Layout* loading*/);
protected:
    AnimateViewer *m_pAnimateViewer;
};





class WaitBeginGame : public GameLayer
{
public:
	CREATE_FUNC(WaitBeginGame);
	virtual void setVisible(bool bVisible);
protected:
	bool init();
	AnimateViewer *m_pAnimateViewer;
};



class DataLoading : public GameLayer
{
public:
	CREATE_FUNC(DataLoading);
	virtual void setVisible(bool bVisible);
	virtual bool init(/*cocos2d::ui::Layout* loading*/);
protected:
	//AnimateViewer *m_pAnimateViewer;
	AnimationManager* m_pAnimateViewer ;
};


#endif