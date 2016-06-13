#ifndef _UPDATETIPS_H_
#define _UPDATETIPS_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"

class UpdateTips :public GameLayer
{
public:
	CREATE_FUNC(UpdateTips) ;

protected:
	bool init() ;

private:
	void callUpdate(cocos2d::Ref* pSend) ;
	void callCancel(cocos2d::Ref* pSend) ;

};

#endif