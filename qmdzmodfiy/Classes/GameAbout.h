#ifndef  __GAMEABOUT_H__
#define  __GAMEABOUT_H__

#include "cocos2d.h"
#include "GameDialog.h"

class GameAbout : public GameDialog
{
public:
	CREATE_FUNC(GameAbout);

protected:
	virtual bool init();
private:

};

#endif //__GAMEABOUT_H__