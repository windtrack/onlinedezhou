#ifndef  _DEALAPPLYFAMILY_H_
#define  _DEALAPPLYFAMILY_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "HttpSprite.h"

class GameTipLayer;

class DealApplyFamily :public GameDialog
{
public:
	
	CREATE_FUNC(DealApplyFamily) ;
	bool init() ;

	void setInfo(unsigned fid,unsigned uid,unsigned mid,std::string uname,std::string uicon) ;

	void setCB( std::function<void(unsigned)> cbBt) ;

	void setVisible(bool bVisible) ;
private:
	void onMenuItemSelect(cocos2d::Ref* ref,int index) ;

private:

	GameTipLayer *m_pTipLayerWithBt2;

	cocos2d::MenuItemToggle *m_pMenuItemAutoBuyJetton2Max;

	HttpSprite* httpIcon;
	cocos2d::Label* lableName ;
	cocos2d::Label *m_pLabelContent;
	unsigned m_fid;
	unsigned m_uid;
	unsigned m_con;
	unsigned m_mid ;
	std::string m_uname ;
	std::string m_uicon;

};




#endif
