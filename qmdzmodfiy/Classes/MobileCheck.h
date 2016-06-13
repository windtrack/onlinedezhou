
#ifndef __MobileCheck_H__
#define __MobileCheck_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "GameDialog.h"
#include "BaseButton.h"

class MobileCheck : public GameDialog ,public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(MobileCheck);

	inline void setNewSkaikhID(unsigned newid){m_newID = newid;} ;	
	void setBindPhone(std::string  iphone) ;
protected:
	virtual bool init();
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

private:

	unsigned m_newID ;
	cocos2d::ui::EditBox             *m_editPhoneNum;
	cocos2d::ui::EditBox             *m_editSecurityCode;
	AutoDelayEnabledFrameSpriteButton       *m_btGetSeccode;

	//获取验证码
	void menuClickGetSeccode(Ref* pSender);
	//确认绑定按钮
	void menuClickConfirmBind(Ref* pSender);
};

#endif