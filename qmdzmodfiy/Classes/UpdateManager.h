#ifndef _UPDATEMANAGER_H_
#define _UPDATEMANAGER_H_

#include "cocos2d.h"
#include "UICommon.h"

class UpdateInfo ;
class UpdateTips ;


struct updateProgressBar
{
	cocos2d::Sprite* m_bgpro ;
	cocos2d::Node* m_node ;
	cocos2d::Sprite* m_frontPro ;

	cocos2d::Sprite* m_left ;
	cocos2d::Sprite* m_right ;
	cocos2d::Sprite* m_light ;

	//cocos2d::Sprite* m_moveLight ;
	cocos2d::Label* mLabelUpdatePro ;

	cocos2d::Sprite *mmoban;   
	cocos2d::ClippingNode *mclippingNode;    
};


class UpdateManager :public cocos2d::Layer

{
public:
	static cocos2d::Scene* createScene();

	CREATE_FUNC(UpdateManager) ;
	bool init() ;


	void showUpdateTipsLayer(bool flag) ;
	void showUpdateInfoLayer(bool flag) ;

	void updatePro(int pro) ;
    void newUpdatePro(int pro);
	void updateVersionCode(std::string versionCode) ;
	void showUpdateInfo(std::string info) ;
private:
	void creatUpdateProgressBar() ;
	void showUpdateProgressBar(bool flag) ;

	void update(float delta) ;
	void updateMoveLight(float delta) ;
	void creatMoveLight(float delta) ;
private:

	UpdateTips* m_tipsLayer ;
	UpdateInfo* m_infoLayer ;

    //改变进度条样式后新增控件
    cocos2d::ui::LoadingBar* _progressBar;
    cocos2d::Sprite* _spriteLoading;
    cocos2d::Label  *m_labelProgress;
	
	cocos2d::Label* mLabelVersionCode ;

	updateProgressBar m_ProgressBar ;

	int m_curPro ;

	float m_curProPx ;
	float m_preProPx ;
};



extern UpdateManager *g_pUpdateManager;
#endif