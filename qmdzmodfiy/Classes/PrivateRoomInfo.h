#ifndef _PRIVATEROOMINFO_H_
#define _PRIVATEROOMINFO_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"

class PrivateRoomInfo :public GameDialog
{
public:
	CREATE_FUNC(PrivateRoomInfo) ;
	bool init() ;
	void setVisible(bool bVisible) ;

	//void onGrabTouchBegan(bool bHitDialog) ;

	void updateCurRoomInfo(bool flag) ;
private:
	cocos2d::ui::Text					*m_personalRoom ;
	cocos2d::ui::Text					*m_clubRoom ;
	cocos2d::ui::Text					*m_playerNum ;
	cocos2d::ui::Text					*m_ownerName ;
	cocos2d::ui::Text					*m_password ;
	cocos2d::ui::Text					*m_blind ;
	cocos2d::ui::Text					*m_ante ;
	cocos2d::ui::Text					*m_minCarry ;
	cocos2d::ui::Text					*m_maxCarry ;
	cocos2d::ui::Text					*m_curFund ;
	cocos2d::ui::Text					*m_fundP ;
	cocos2d::ui::Text					*m_minCollect ;
	cocos2d::ui::Text					*m_maxCollect ;


	cocos2d::ui::Text					*m_str_password ;

	cocos2d::ui::Text					*m_curFundTitle ;
private:


};
#endif
