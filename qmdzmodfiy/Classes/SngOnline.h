#ifndef _SNGONLINE_H_
#define _SNGONLINE_H_

#include "GameDialog.h"
#include "cocos2d.h"
class SngOnline :public cocos2d::Sprite
{
public:
	CREATE_FUNC(SngOnline);
	bool init() ;

	void setVisible(bool bVisible);


};

#endif