#ifndef  __RESETPASSWORD_H__
#define  __RESETPASSWORD_H__

#include "cocos2d.h"
#include "GameDialog.h"
//#include "ui/UIEditBox/UIEditBox.h"
#include "BaseButton.h"
#include "ui/CocosGUI.h"

class LogonLayerEx;


class ResetPassword : public cocos2d::ui::Layout// , public cocos2d::ui::EditBoxDelegate
{
public:
	//CREATE_FUNC(ResetPassword);
    //static ResetPassword* create(cocos2d::ui::Layout* resetView);
	//virtual bool init();
    bool initResetPassword(LogonLayerEx*);
protected:
	//virtual void onGrabTouchBegan(bool bHitDialog) override;
	//virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	//virtual void onKeyEscPressed() override;
	
    void GetSecurityCode();
    void menuWrongOrRight(cocos2d::Ref* pSender);
	// 确认修改的回调事件
	void onMenuItemClicked(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type);
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
private:
	bool m_bPass;	// 是否通过
	cocos2d::ui::Button						*m_pConfirmModify;		// 确认修改
	cocos2d::ui::Button						*m_pBackmenu;			// 返回键
	cocos2d::ui::TextField					*m_pTxtFiledUser;		// 用户名
	cocos2d::ui::Button						*m_pUserSpriteImage[2];	// 用户输入框的按钮
	cocos2d::ui::TextField					*m_pTxtGetCode;			// 获取验证码
	cocos2d::ui::TextField					*m_pTxtFiledPass;		// 输入密码
	AutoDelayEnabledFrameSpriteButton		*m_btGetSecurityCode;
    LogonLayerEx                            *m_LogoLayerEx;
};

#endif //__RESETPASSWORD_H__