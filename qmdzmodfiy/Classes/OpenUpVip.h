/********************************************************************
	created:	2015/04/10
	created:	2015:04:10  18:30
	filename: 	OpenUpVip.h
	file path:	D:\WORK\texaspoker\Classes\GameDialog.h
	file base:	hall
	file ext:	
	author:		卢康康
	
	purpose:	开通会员提示框
*********************************************************************/

#ifndef _GROWUPTOVIP_H__
#define _GROWUPTOVIP_H__

#include "cocos2d.h"
#include "GameDialog.h"

class OpenUpVip : public GameLayer
{
public:
    CREATE_FUNC(OpenUpVip);
    virtual bool init();
    virtual void onKeyEscPressed();
	void Update();
protected:
    void OnClose(cocos2d::Ref* pSender);
private:
    cocos2d::Label              *m_labelItemName[8];
    cocos2d::Label              *m_labelItemContent[8];
};

#endif