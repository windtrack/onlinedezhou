#ifndef _MOVELABEL_H_
#define _MOVELABEL_H_
#include "LineLabel.h"

class MoveLabel :public LineLabel
{
public:

	CREATE_FUNC(MoveLabel) ;

	void doMove(std::function<void()> callBack,std::function<void()> callBack2) ;


private:
	std::function<void()> moveCallBack ;
};

#endif

