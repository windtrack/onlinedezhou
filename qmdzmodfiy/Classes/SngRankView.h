#ifndef _SNGRANKVIEW_H_

#define _SNGRANKVIEW_H_
#include "cocos2d.h"
#include "GameDialog.h"

struct MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class RankTexture;

class SngRankView :public GameDialog
{
public:

	CREATE_FUNC(SngRankView) ;
	bool init() ;

	void updateSngRankView(unsigned uMaxPlayer, unsigned uRankId,unsigned cash,unsigned score) ;
    virtual void setVisible(bool Visible) override;
	void onMenuItemCloseSelected(cocos2d::Ref *);
protected:
    void onMenuItemBottomSelect(cocos2d::Ref*pMenuItem);
	
    void GetReword();
    void SetAllSpriteVisible(bool bVisible);

	void onGrabTouchBegan(bool bHitDialog) ;

	void doLookUp() ;

private:
    unsigned m_uMaster;
    double m_dReword;
    cocos2d::Sprite          *m_pWinCup[3];
    RankTexture              *m_pRankTexture;
    cocos2d::Sprite          *m_pFailedExpress;
    cocos2d::Label           *m_pLabelFailed;
    cocos2d::Label           *m_pLabelWin[2];
    cocos2d::Label           *m_pLabelReward[2];
    MenuItemScale9SpriteWithLabel  *m_pMenuItem[3];//返回大厅, 留下旁观, 再战一场

	int m_curRank ;
	int m_maxRole ;
};


#endif
