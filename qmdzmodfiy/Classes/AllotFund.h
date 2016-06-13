#ifndef _AllotFund_H_
#define _AllotFund_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"

class AllotFund :public GameDialog,public  cocos2d::ui::EditBoxDelegate
{

public:
	CREATE_FUNC(AllotFund) ;
	bool init() ;
	void setVisible(bool bVisible) ;
	void setInfo(unsigned fid,unsigned uid,unsigned mid,std::string uname) ;

	void setCB( std::function<void(unsigned)> cbBt) ;

	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
private:
	void onMenuItemSelect(cocos2d::Ref* ref,int index) ;
public:

private:

	cocos2d::Label* lableName ;
	cocos2d::Label *m_pLabelContent;
	unsigned m_fid;
	unsigned m_uid;
	unsigned m_con;
	unsigned m_mid ;
	std::string m_uname ;
	std::string m_uicon;

	cocos2d::ui::EditBox					*m_editBox ;

	double			m_edit_fund ;
};
#endif