#ifndef _GAMETIPSMANAGER_H_
#define  _GAMETIPSMANAGER_H_
#include "cocos2d.h"

class GameTipsManager
{
public:
	static GameTipsManager* getInstance() ;


	void addTips(cocos2d::Node* tips) ;
	void callNextTips() ;
	void hideTips() ;
	void clean() ;
private:
	void init() ;
public:

	cocos2d::Vector<cocos2d::Node*> m_verTips ;//¹ÜÀíµ¯³ö¿ò

private:
	static GameTipsManager* _instance ;
	bool m_isShowTips ;
};

#endif