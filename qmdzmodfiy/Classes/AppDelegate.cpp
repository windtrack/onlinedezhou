#include "AppDelegate.h"
#include "GameScene.h"
#include "GameCommon.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "GameFrame.h"
#include "LogoLayer.h"
#include "SimpleAudioEngine.h"
#include "PlatformSpec.h"
#include "GameData.h"
#include "TrackManager.h"
#include "UpdateManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "IosAutoUpdate.h"
#endif

USING_NS_CC;

AppDelegate::AppDelegate() :
lastTime(0)
{
}

AppDelegate::~AppDelegate()
{
																																																																																																																																																																																																																														
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

// TODO 150504: should not set search path
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	FileUtils::getInstance()->addSearchPath("../../Resources");
	//FileUtils::getInstance()->addSearchPath("../../Resources/res");	
	FileUtils::getInstance()->addSearchPath("../../Resources/res/ui");	
#endif

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create(GAME_NAME);
        director->setOpenGLView(glview);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    glview->setFrameSize(1136,640) ;
#endif
    TrackMgr->setEnabled(true);
    TrackMgr->MobclickcppStartWithAppKeyAndChannel();
	Size screenSize = Director::getInstance()->getOpenGLView()->getFrameSize(); 
	
	float scale =0 ;
	float dis ;
	if((screenSize.width / screenSize.height)  < (16.0f / 9.0f)) 
	{ 
		Director::getInstance()->getOpenGLView()->setDesignResolutionSize(GAME_WIDTH, GAME_HEIGHT, ResolutionPolicy::FIXED_WIDTH); 	

		dis = abs(GAME_HEIGHT-Director::getInstance()->getVisibleSize().height);
		scale = dis/GAME_HEIGHT;
	} 
	else 
	{ 		
		Director::getInstance()->getOpenGLView()->setDesignResolutionSize(GAME_WIDTH, GAME_HEIGHT, ResolutionPolicy::FIXED_HEIGHT); 
		dis = abs(GAME_WIDTH-Director::getInstance()->getVisibleSize().width);
		scale = dis/GAME_WIDTH;
	} 
	
	GameData::getInstance()->setGameScale(1+scale);

	GameData::getInstance()->setGameWidth( Director::getInstance()->getVisibleSize().width);
	GameData::getInstance()->setGameHeight(Director::getInstance()->getVisibleSize().height);
	
	//glview->setContentScaleFactor(0.5) ;

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	
	/*
	#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (false == Application::getInstance()->letsGo()) {
			director->end();
		}
	#endif
	*/
    TrackMgr->SetPreTime(getSystemTime());
	GameData::getInstance() ;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    director->runWithScene(IosAutoUpdateLayer::createScene());
#else
	//GameData::getInstance()->setUpdateVersion(true) ;
	//Director::getInstance()->runWithScene(UpdateManager::createScene());
	director->runWithScene(LogoLayer::createScene());
	TrackMgr->event("EnterGame", "进入加载界面");

    //director->runWithScene(GameScene::createScene());
#endif

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	curPauseTime  =getCurrentTime() ;
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (g_pGameFrame != nullptr && false == g_pGameFrame->isLoginVisible()) {
		g_pGameFrame->PauseHeartBeatCheck(1);
	}
#endif

    TrackMgr->ApplicationDidEnterBackground();
	
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	
	lastTime = getSystemTime();
	//bringVideoToFront();

	GameData::getInstance()->setPauseNetWork(true) ;
	GameData::getInstance()->setPauseNetWorkClear(false) ;
														  		
	GameData::getInstance()->setSoundOn(false) ;
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (g_pGameFrame != nullptr && false == g_pGameFrame->isLoginVisible()) {
		g_pGameFrame->PauseHeartBeatCheck(0);
		g_pGameFrame->startCheckNetwork();
	}
	endTime  =getCurrentTime() ;
	if (g_pGameFrame != nullptr ) {
		g_pGameFrame->updateMttHallFixTime((endTime-curPauseTime)/1000.00) ;
	}
	SendClientRequest(ClientRequest::kCallUpdateCashAndVip);
	SendClientRequest(ClientRequest::kRequsetIsAutoTrust);
#endif

    TrackMgr->ApplicationWillEnterForeground();

    Director::getInstance()->startAnimation();

	GameData::getInstance()->setPauseNetWork(false) ;
	
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();




}
