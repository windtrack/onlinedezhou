#ifndef _SNGRULE_H_
#define  _SNGRULE_H_
#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
class SngRule:public GameDialog
{
public:
	CREATE_FUNC(SngRule) ;
	bool init() ;

	void setVisible(bool bVisible) ;
private:



};

#endif