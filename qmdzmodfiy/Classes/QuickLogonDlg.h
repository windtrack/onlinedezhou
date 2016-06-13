#ifndef __QUICKLOGONDLG_H__
#define __QUICKLOGONDLG_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MenuItemSpriteWithLabel;
class LogonLayerEx;
class QuickLogonDlg : public cocos2d::ui::Layout 
{
public:
    //static QuickLogonDlg* create(cocos2d::ui::Layout* quickLogin);
    bool initQuickDlg(LogonLayerEx* llEx);
private:
	//快速游戏
	void menuClickQuickGame(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type);
	//登陆回调
	void menuItemClick(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type);

private:
	cocos2d::ui::Button				*m_pMenuItemLoginByGuest;		//快速登陆
	cocos2d::ui::Button				*m_pMenuItemLoginByWeiXin;		//微信登陆
	cocos2d::ui::Button				*m_pMenuItemLoginByQQ;			//QQ登陆
	cocos2d::ui::Button				*m_pMenuItemLoginByAccount;		//账号登陆
	cocos2d::ui::Button				*m_pMenuItemLoginByWeiBo;		//微博登陆

    LogonLayerEx* m_parent;
};

#endif
