#ifndef _MttSignUpSuccess_H_
#define _MttSignUpSuccess_H_
#include "cocos2d.h"
#include "GameDialog.h"
class MenuItemScale9Sprite;
class MttSignUpSuccess :public GameDialog
{
public:
	CREATE_FUNC(MttSignUpSuccess) ;
	bool init() ;

	void setVisible(bool bVisible);
	void onGrabTouchBegan(bool bHitDialog) ;

	void updateView(std::string name,std::string money,std::string time) ;
private:
	void onMenuItemClick(cocos2d::Ref * ref) ;

private:
	cocos2d::Label* m_labelName;
	cocos2d::Label* m_labelMoney;
	cocos2d::Label* m_labelTime;

	MenuItemScale9Sprite* m_btSure ;
};

#endif