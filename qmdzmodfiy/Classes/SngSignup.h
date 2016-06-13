#ifndef _SNGSIGNUP_H__
#define _SNGSIGNUP_H__

#include "GameDialog.h"
#include "UICommon.h"
#include "GlobalUserData.h"
struct  GlobalSngTableInfo ;

class SngSignUp : public GameDialog
{
public:
    CREATE_FUNC(SngSignUp);
    virtual bool init();
    void UpdateView(GlobalSngTableInfo date);
    virtual void setVisible(bool Visible) override;
protected:
    virtual void onSetVisible(bool bVisible) override;
    void OnMenuClick(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
	
	void onGrabTouchBegan(bool bHitDialog) ;

	void onMenuItemCloseSelected(cocos2d::Ref *) ;

private:
    cocos2d::ui::Layout         *m_pLayoutBg;

    cocos2d::ui::Layout         *m_pSignup6p;
    cocos2d::ui::Layout         *m_pSignup9p;

    cocos2d::ui::Layout         *m_pNoTicket;
    cocos2d::ui::Layout         *m_pHasTicket;

    cocos2d::ui::Button         *m_pBtnSignup;
    cocos2d::ui::Button         *m_pBtnCancel;

    cocos2d::ui::Text           *m_pTextAward6p[2];         //六人桌奖励
    cocos2d::ui::Text           *m_pTextMaster6p[2];        //六人桌大师分

    cocos2d::ui::Text           *m_pTextAward9p[3];         //九人桌奖励
    cocos2d::ui::Text           *m_pTextMaster9p[3];        //九人桌大师分

    cocos2d::ui::Text           *m_pTextEntryFee[3];           //0：参赛费  1：服务费  2：总资产   没有门票
    cocos2d::ui::Text           *m_pTextEntryTicket[3];        //0：场类型  1：参赛费  2：总资产   有门票

	unsigned					m_tableId ;

	GlobalSngTableInfo           curTableInfo ;
};

#endif