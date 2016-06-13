#ifndef _CUSTOMERSERVICES_H__
#define _CUSTOMERSERVICES_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "ui/CocosGUI.h"

class CustomerServices : public GameDialog
{
public:
    CREATE_FUNC(CustomerServices);
    virtual void setVisible(bool bVisible) override;

protected:
    virtual bool init();
    void OnMenuItemSelect(cocos2d::Ref* pSend, cocos2d::ui::Button::TouchEventType type);

private:
    cocos2d::ui::Button         *m_btOnlineServices;
    cocos2d::ui::Button         *m_btPhoneServices;
    cocos2d::ui::Button         *m_btVipChannel;
    cocos2d::ui::Button         *m_btBack;
};

#endif