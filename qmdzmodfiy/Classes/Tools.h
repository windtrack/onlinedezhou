#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "cocos2d.h"
#include "LogonLayerEx.h"
class Tools
{
public:

	
	static cocos2d::Node* getWidgetByName(cocos2d::Node* root, const std::string& name) ;

	static void callScaleBig(cocos2d::Node* node) ;
	static void callScaleSmall(cocos2d::Node* node) ;


	static DataLoading* creatDataLoading(cocos2d::Node* node,float x,float y,int zOrder) ;

	static void callScaleAction(cocos2d::Node* node) ;

	static void callHideScaleAction(cocos2d::Node* node,std::function<void()> &funcation) ;

	static void callScaleAction(cocos2d::Node* node,bool useBig,float bigtime,float smalltime,float scaleBig,float scalesmall) ;

	static void creatParticleAddTo(std::string path ,cocos2d::Node* nodep,cocos2d::Point pos,int Zorder) ;

	static bool IsValidChar(char ch) ;
	static bool IsValidEmail(std::string strEmail) ; //≈–∂œ «∑Ò «” œ‰
	static bool is_number(std::string str) ;
private:

																							

};


#endif
