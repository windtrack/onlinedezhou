
#ifndef _MttQuitMatch_h_
#define _MttQuitMatch_h_
#include "GameDialog.h"
#include "cocos2d.h"
#include "GameCommon.h"
class MenuItemScale9Sprite;
class LineLabel ;
class LayerButton;
class MttQuitMatch :public GameDialog
{
public:
	CREATE_FUNC(MttQuitMatch) ;
	bool init() ;

	void setVisible(bool bVisible);
	void onGrabTouchBegan(bool bHitDialog) ;
	void updateView(int type) ;
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
