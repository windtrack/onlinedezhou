#ifndef _LOADMANAGER_H_
#define  _LOADMANAGER_H_

#include "cocos2d.h"

#define  ResAniCount 21 //28 
#define  ResPlistCount 13
#define  ResPngCount 8

#define  PlistPngCount 0

#define DelayLoadingAnimCount 12


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define SOUND_EXT ".ogg"
#else
#define SOUND_EXT ".mp3"
#endif

const  std::string aniPath[ResAniCount]=
{
	"res/anims/heguan/heguan.csb" ,
	"res/anims/huangjiatonghuashun/huangjiatonghuashun.csb" ,
	"res/anims/hulu/hulu.csb" ,
	"res/anims/jingang/jingang.csb" ,
	"res/anims/tonghuashun/tonghuashun.csb" ,
	"res/anims/niyingle/niyingle.csb" ,
	"res/anims/btpopmissionreward1/renwu_baoxiang_dakai.csb" ,
	"res/anims/vipopenroom/vipopenroom.csb" ,

	"res/anims/aniallin/aniallin.csb" ,
	"res/anims/anitipshowcard/anitipshowcard.csb" ,
	
	"res/anims/allin/allin.csb" ,
	"res/anims/flower/flower.csb" ,
	"res/anims/jetton/jetton.csb" ,

	"res/anims/showwin/showwin.csb",

	//±Ì«È
	"res/anims/guzubiaoqing/guzubiaoqing.csb" ,
	"res/anims/putongbiaoqing/putongbiaoqing.csb" ,
	"res/anims/mtt_entershine/mtt_entershine.csb" ,
	"res/anims/xinchun_yanwu/xinchun_yanwu.csb" ,

	"res/anims/matchmtt/matchmtt.csb" ,
	"res/anims/matchsng/matchsng.csb" ,
	"res/anims/simichang/simichang.csb" ,
};


 static  std::string plistPath[ResPlistCount] = 
{
	
	"res/ui/dezhou1",
	"res/ui/gamehall",
	"res/ui/gamemall",
	"res/ui/gametable",
	"res/ui/login_ui",
	"res/ui/CustomerServices",
	"res/ui/gamefamily",
	"res/ui/gamesng",
	"res/ui/gamemtt",
	"plist/useritem",
    "res/ui/upgradeaccount",
	"res/ui/PrivateRoom",
	"res/ui/gameclub",
	
} ;

 static std::string delayLoadingAnimPath[DelayLoadingAnimCount] =
 {
	 "res/anims/toolani0/toolani0.csb" ,
	 "res/anims/toolani1/toolani1.csb" ,
	 "res/anims/toolani2/toolani2.csb" ,
	 "res/anims/toolani3/toolani3.csb" ,
	 "res/anims/toolani4/toolani4.csb" ,
	 "res/anims/toolani5/toolani5.csb" ,
	 "res/anims/toolani6/toolani6.csb" ,
	 "res/anims/toolani7/toolani7.csb" ,
	 "res/anims/toolani8/toolani8.csb" ,
	 "res/anims/toolani9/toolani9.csb" ,
	 "res/anims/toolani10/toolani10.csb" ,
	 "res/anims/toolani11/toolani11.csb" ,
	
 };

 //
 //static  std::string plistpngPath[PlistPngCount] = 
 //{
	// "plist/useritem",
 //} ;


 //µ•‘ÿÕº∆¨ 
 static const char* pngPath[ResPngCount]=
 {
	/* "ui/hall/dezhou1.png",*/
	 "anishowcard.png",
	 "showmycard1.png",
	 "showmycard2.png",
	 "handcard0.png",
	 "handcard1.png",
	 "bgtable.png",
	 "hall/img_bg.png",
	 "publiccard.webp",

 };

 enum ResType {
	 kResAnim = 1,
	 kResPlist,
	 kResPng,
	 kPlistPng,
 };

#define LoadingMgr LoadManager::getInstance()

class LoadManager :public cocos2d::Node
{
public:
	static LoadManager* getInstance() ;
	inline int getCurLoadIndex(){return s_curLoadIndex ; } ;
	inline int getMaxLoadCount(){return s_maxLoadCount ;} ;
	void callLoadRes() ;
	void updateByTime() ;

	void delayLoadingAnims();
private:
	void callLoadback() ;

	void loadingResources(ResType type);
	void jsonAsyncCallback(float dt) ;
	void pngCallBack() ;
	void plistCallBack(cocos2d::Texture2D* texture) ;

	void jsonAsyncCallback2(float dt) ;

	
private:
	static LoadManager* s_instance ;

	int s_curLoadIndex ;
	int s_curAniIndex ;
	int s_curPlistIndex ;
	int s_curPngIndex ;

	int s_maxLoadCount ;


	int s_curTime ;
	int s_preTime ;
};

#endif