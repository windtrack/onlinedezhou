#ifndef _EDITFAMILYROLE_H_

#define _EDITFAMILYROLE_H_

#include "GameDialog.h"
#include "cocos2d.h"
#include "FrameSprite.h"
class LayerButton;
class TouchGrabber;
class MenuItemFrameSprite;
enum EditRole
{
	Edit_LookInf0=1,//查看资料
	Edit_To_EditFund=2,//任命副族长
	Edit_ChangeShaikh=4,//转让俱乐部
	Edit_Push=8,//逐出俱乐部
};

class EditFamilyRole :public GameDialog
{

public:
	CREATE_FUNC(EditFamilyRole);

	bool init() ;

	void updateView(unsigned value) ;

	inline void setUserID(unsigned id){m_Uid = id;} ;
	inline unsigned getUserID(){return m_Uid ;} ;

	inline void setUserVipLv(unsigned lv){m_Uviplv = lv;} ;
	inline unsigned getUserVipLv(){return m_Uviplv ;} ;

	inline void setUName(std::string name){m_uNmae = name ;} ;
private:

	void onMenuItemSelected(cocos2d::Ref* pItem) ;
	void resetAllButton() ;

private:
	unsigned m_Uid ;
	unsigned m_Uviplv ;
	std::string m_uNmae ;
	std::vector<MenuItemFrameSprite*> m_editBox;

	std::vector<std::string> m_editBoxName;

	MenuItemFrameSprite *m_btInfo;
	//MenuItemFrameSprite *m_bt_to_viceShaikh;
	MenuItemFrameSprite *m_bt_conFund;
	MenuItemFrameSprite *m_bt_changeShaikh;
	MenuItemFrameSprite *m_bt_pushRole;

	cocos2d::Sprite* m_bigBlock ;
	cocos2d::Sprite* m_smallBlock ;

};
#endif
