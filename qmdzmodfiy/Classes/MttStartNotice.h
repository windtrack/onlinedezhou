#ifndef _MttStartNotice_H_

#define _MttStartNotice_H_

#include "cocos2d.h"

class LineLabel ;
class MenuItemScale9Sprite ;
class MttStartNotice :public cocos2d::Layer
{
public:
	CREATE_FUNC(MttStartNotice) ;
	bool init() ;

	void addTips(unsigned tableid,std::string matchName, unsigned time,bool delayRegist) ;
	void callMatchTips(std::string matchName) ;
    unsigned getTableID(){return m_tableID;};

	void addMissTime(float time) ;

private:
	void updateTime() ;

	void onMenuItemSelected(cocos2d::Ref * ref) ;
private:
	MenuItemScale9Sprite* m_bt_quiteMatch;
	MenuItemScale9Sprite* m_bt_joinMatch;
	cocos2d::MenuItemImage	*m_bt_close ;

	LineLabel	*m_lineLabelTips ;

private:
	std::string m_matchName;
	int	m_time ;
	unsigned m_tableID ;
	bool m_isdelayRegist;

	bool showOneMins ;
};

#endif