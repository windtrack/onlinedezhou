#ifndef _TABLETIPS_H_
#define _TABLETIPS_H_

#include "cocos2d.h"
#include "GameDialog.h"


class TableTips
{
public:

};



/************************************************************************/
/*重购                                                                 */
/************************************************************************/
class MttRebuyTips :public GameDialog
{
public:
	CREATE_FUNC(MttRebuyTips) ;
	bool init() ;
	void setVisible(bool bVisible) ;
	void onGrabTouchBegan(bool bHitDialog) ;

	void updateMttRebuyTips() ;
protected:
	
	void onMenuRebuyItemSelect(cocos2d::Ref* ref,int index) ;
private:
	cocos2d::Label *m_pLabelContent ;
	cocos2d::Label *m_pLabelCount ;
};

/************************************************************************/
/* 增购                                                                     */
/************************************************************************/
class MttAddonTips :public GameDialog
{
public:
	CREATE_FUNC(MttAddonTips) ;
	bool init() ;
	void setVisible(bool bVisible) ;
	void onGrabTouchBegan(bool bHitDialog) ;
	void updateMttAddonTips() ;
protected:
	
	void onMenuAddonItemSelect(cocos2d::Ref* ref,int index) ;
private:
	cocos2d::Label *m_pLabelContent ;
	cocos2d::Label *m_pLabelCount ;
};




/************************************************************************/
/*重购  服务器发来                                                               */
/************************************************************************/
class MttRebuyTipsByServer :public GameDialog
{
public:
	CREATE_FUNC(MttRebuyTipsByServer) ;
	bool init() ;
	void setVisible(bool bVisible) ;
	void onGrabTouchBegan(bool bHitDialog) ;

	void updateMttRebuyTips(int time) ;
protected:

	void onMenuRebuyItemSelect(cocos2d::Ref* ref,int index) ;
private:
	cocos2d::Label *m_pLabelContent ;
	cocos2d::Label *m_pLabelCan ;
	cocos2d::Label *m_pLabelCount ;
	cocos2d::Label *m_pLabelTime ;
	int			m_time ;
};


#endif