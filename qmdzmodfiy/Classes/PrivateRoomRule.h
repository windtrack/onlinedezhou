#ifndef _PRAVITEROOM_H_
#define _PRAVITEROOM_H_

#include "cocos2d.h"
#include "GameDialog.h"

#include "UICommon.h"
class PrivateRoomRule :public GameDialog
{
public:
	CREATE_FUNC(PrivateRoomRule) ;
	bool init() ;
	void setVisible(bool bVisible) ;
};

#endif