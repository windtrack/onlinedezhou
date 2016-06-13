#ifndef _REGISTERVIEW_H__
#define _REGISTERVIEW_H__

#include "cocos2d.h"
#include "GameDialog.h"
//#include "ui/UIEditBox/UIEditBox.h"
#include "BaseButton.h"
#include "CCScrollView.h"
#include "ui/CocosGUI.h"

class LogonLayerEx;
class FrameSpriteButton;

class RegisterView  : public cocos2d::ui::Layout//, public cocos2d::ui::EditBoxDelegate
{
public:
    //CREATE_FUNC(RegisterView);
    //static RegisterView* create(cocos2d::ui::Layout* registerView);
    bool initRegisterView(LogonLayerEx*);
    void GetSecurityCode();
    void AuthorityAndRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
    void ReadUserProtocol(cocos2d::Ref* pSender);
    void ExitRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
protected:
    //virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    //virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text){};
    //virtual bool init();
	//virtual void onGrabTouchBegan(bool bHitDialog) override;
	//virtual void onKeyEscPressed() override;
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    void menuWrongOrRight(Ref* pSender);
private:
    AutoDelayEnabledFrameSpriteButton       *m_btGetSecurityCode;             //获取验证码
    cocos2d::ui::Button                     *m_btAuthorityAndRegster;         //授权并注册
    cocos2d::ui::Button                     *m_btExitRegister;
    cocos2d::ui::TextField                  *m_editPhoneNum;
    cocos2d::ui::TextField                  *m_editSecurityCode;
    cocos2d::ui::TextField                  *m_editPassword;
	cocos2d::ui::Button						*m_pUserSpriteImage[2];	// 用户输入框的按钮
    LogonLayerEx                            *m_LogoLayerEx;
};


class UserProtocol : public GameDialog
{
public:
    CREATE_FUNC(UserProtocol);
    static UserProtocol* create(cocos2d::ui::Layout* userPro);
protected:
    void ExitProtocol();
    virtual bool init();
    bool init(cocos2d::ui::Layout* userPro);
	virtual void onKeyEscPressed() override;
private:
    cocos2d::extension::ScrollView             *m_protocolScrollView;
    cocos2d::Label                             *m_labelProtocol;
    FrameSpriteButton                          *m_btExitProtocol;
};
#endif