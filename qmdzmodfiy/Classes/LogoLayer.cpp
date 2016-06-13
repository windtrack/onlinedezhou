#include "LogoLayer.h"
#include "GameScene.h"
#include "UICommon.h"
#include "LoadManager.h"
#include "GameData.h"
#include "GameCommon.h"
#include "AnimateViewer.h"
#include "Util.h"
#include "TrackManager.h"
#include "UpdateManager.h"
USING_NS_CC;

int g_updateFlag = 0;




bool LogoLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	//m_spriteBg = Sprite::create("logobg.jpg");
    m_layerSpriteBg = LayerColor::create(cocos2d::Color4B::WHITE);
    m_layerSpriteBg->setContentSize(winSize);
	m_layerSpriteBg->setPosition(0, 0);
	//m_spriteBg->setScale(GameData::getInstance()->getGameScale());
    auto bgSz = m_layerSpriteBg->getContentSize();
	this->addChild(m_layerSpriteBg, 5);
    m_logoSprite = CreateNode1AndPosAddTo<Sprite>("logo@2208.png", bgSz.width/2, bgSz.height/2, m_layerSpriteBg, 0);
    m_logoSprite->setScale(0.5);

    m_spriteBg1 = Sprite::create("logobg.png");
    m_spriteBg1->setPosition(Vec2(winSize.width/2, winSize.height/2));
    m_spriteBg1->setScale(GameData::getInstance()->getGameScale());
    this->addChild(m_spriteBg1, 3);

    CreateNode1AndPosAddTo<Sprite>("loadinglogo.png", 85, winSize.height-40, this, 3);

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
	g_updateFlag = 1;
#endif
	curPrecent =0 ;

	CCSize size=CCDirector::sharedDirector()->getWinSize();  


    ttf = CreateLabelMSYH(30, "游戏加载中%0", cocos2d::TextHAlignment::CENTER);
    ttf->ignoreAnchorPointForPosition(false);
    ttf->setAnchorPoint(Vec2(0.5, 0.5));
    ttf->setPosition(40, -20);   
	ttf->setColor(Color3B::WHITE);


    m_animate = AnimateViewer::create("login/loading.png", true, 0.2, 3, 3, 8);
    m_animate->setPosition(910, 100);
    m_animate->setVisible(false);
    m_animate->addChild(ttf);
    m_spriteBg1->addChild(m_animate);


	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	ttf->setVisible(false);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	   this->scheduleUpdate();
    /*StartUpLogo(1.0f);*/ // scheduleOnce(schedule_selector(LogoLayer::StartUpLogo), 1.0f);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        this->scheduleUpdate();
    }
    
#endif

	return true;
}

LogoLayer::LogoLayer() :
	m_updateListener(nullptr)
{
	log("LogoLayer()");
}

LogoLayer::~LogoLayer()
{
	log("~LogoLayer()");
	if (m_updateListener != nullptr) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_updateListener);
	}
}

Scene* LogoLayer::createScene()
{
	auto logo = LogoLayer::create();
	auto scene = Scene::create();
	scene->addChild(logo);

	return scene;
}

void LogoLayer::StartUpLogo(float fDelat)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_layerSpriteBg->setCascadeOpacityEnabled(true);
	m_layerSpriteBg->runAction(Sequence::create(DelayTime::create(1.5), FadeOut::create(0.0), nullptr));

	m_spriteBg1->runAction(Sequence::create(DelayTime::create(1.5), FadeIn::create(0.0), nullptr));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	m_layerSpriteBg->setVisible(false) ;
	m_spriteBg1->setVisible(true) ;
#endif


}

void LogoLayer::update(float dt)
{


	if (g_updateFlag == 0)
		return;
   if (g_updateFlag == 3)
	{
		Director::getInstance()->runWithScene(UpdateManager::createScene());
		g_updateFlag = 4 ;
		return ;
	}
	 if (g_updateFlag == 1) {
		 StartUpLogo(1.0f);
		ttf->setVisible(true);
		/*_spriteLoading->setVisible(true);
		_progressBar->setVisible(true);*/
		LoadManager::getInstance()->callLoadRes();
		g_updateFlag = 2;
		m_animate->setVisible(true);
	}

	int haveLoadedNum = LoadManager::getInstance()->getCurLoadIndex() ;

	int totalNum = LoadManager::getInstance()->getMaxLoadCount() ;

	char tmp[64];  

	int destPrecent = (int)((float)haveLoadedNum/totalNum*100);

	int sp = destPrecent-curPrecent;
	if (sp>0)
	{
		//curPrecent++ ;
		curPrecent+=(sp>2?2:1);
	}

	//_progressBar->setPercent(curPrecent);
	sprintf(tmp, "游戏加载中 %d%%",curPrecent);  

	SetLabelString(ttf,tmp) ;

	//float loadPro = curPrecent*0.01f ;
	//if (loadPro>0)
	//{
	//	m_spriteLoadingPro->setScaleX(loadPro) ;
	//	m_spriteLoadingPro->setVisible(true) ;
	//}


	if (haveLoadedNum==totalNum&&curPrecent==destPrecent)  
	{  
		//GameData::getInstance()->initGameTablePos();

		//Director::getInstance()->replaceScene(TransitionFade::create(1.0f,GameScene::createScene()));
		//Director::getInstance()->replaceScene(TransitionFade::create(1.0f,GameScene::createScene()));
		Director::getInstance()->replaceScene(GameScene::createScene());
        this->runAction(Sequence::create(DelayTime::create(2.0), CallFunc::create([this](){
            m_spriteBg1->stopAllActions();
            //m_spriteJetton->removeAllChildrenWithCleanup(true);
            m_layerSpriteBg->removeFromParentAndCleanup(true);
            m_spriteBg1->removeFromParentAndCleanup(true);
            m_spriteBg1 = nullptr;
            m_layerSpriteBg = nullptr;
            //m_spriteJetton = nullptr;
        }), nullptr));

		this->unscheduleUpdate();
        int timeInterval = getSystemTime() - TrackMgr->GetPreTime();
        if (timeInterval <=10)
            TrackMgr->event("LoadingTime", "", timeInterval, "秒");
        else if (timeInterval >10 && timeInterval <=15)
            TrackMgr->event("LoadingTime", "11-15", 0, "秒");
        else if (timeInterval >15 && timeInterval <=20)
            TrackMgr->event("LoadingTime", "16-20", 0, "秒");
        TrackMgr->SetPreTime(getSystemTime());

		return ;
	}
}




