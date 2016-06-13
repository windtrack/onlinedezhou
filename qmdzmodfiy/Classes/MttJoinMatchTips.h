#ifndef _MttJoinMatchTips_h_
#define _MttJoinMatchTips_h_
#include "GameDialog.h"
#include "cocos2d.h"
#include "GameCommon.h"
class MenuItemScale9Sprite;
class LineLabel ;
class LayerButton;
class MttJoinMatchTips :public GameDialog
{
public:
	CREATE_FUNC(MttJoinMatchTips) ;
	bool init() ;

	void setVisible(bool bVisible);
	void onGrabTouchBegan(bool bHitDialog) ;
	void updateView(std::string name ,unsigned time,unsigned tableid,bool delayRegist) ;

	void doAddMissTime(float time) ;

private:
	void onMenuItemClick(cocos2d::Ref * ref) ;

private:
	MenuItemScale9Sprite* m_btSure ;
	int m_time ;
	std::string m_matchName ;
	int m_tableID ;
	bool m_isdelayRegist;//是否延迟报名
	cocos2d::Label* m_labelTime ;

	LineLabel* m_lineUp ;
	LineLabel* m_lineDown ;


};


class MttMatchDisMissTips:public GameDialog
{
public:
	CREATE_FUNC(MttMatchDisMissTips) ;
	bool init() ;

	void setVisible(bool bVisible);
	void onGrabTouchBegan(bool bHitDialog) ;
	void updateView(std::string name ,std::string reason ,unsigned tableid) ;
private:
	void onMenuItemClick(cocos2d::Ref * ref) ;

private:
	MenuItemScale9Sprite* m_btSure ;
	int m_time ;
	std::string m_matchName ;
	int m_tableID ;

	LineLabel* m_lineUp ;
	LineLabel* m_lineDown ;
};

#endif
