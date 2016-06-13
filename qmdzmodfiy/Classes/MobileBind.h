#ifndef __MOBILEBIND_H__
#define __MOBILEBIND_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "GameDialog.h"
#include "BaseButton.h"

class MobileBind : public GameDialog ,public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(MobileBind);
protected:
	virtual bool init();
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

private:
	cocos2d::ui::EditBox             *m_editPhoneNum;
    cocos2d::ui::EditBox             *m_editSecurityCode;
    AutoDelayEnabledFrameSpriteButton       *m_btGetSeccode;
    cocos2d::Label                          *m_labelPhoneTips;

	//获取验证码
	void menuClickGetSeccode(Ref* pSender);
	//确认绑定按钮
	void menuClickConfirmBind(Ref* pSender);
};

#endif