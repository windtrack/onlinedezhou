#include "UpdateManager.h"
#include "UpdateInfo.h"
#include "UpdateTips.h"
#include "GameData.h"
#include "Util.h"
#include "PlatformSpec.h"
USING_NS_CC ;

Scene* UpdateManager::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	auto gameFrame = UpdateManager::create();
	scene->addChild(gameFrame);

	return scene;
}


UpdateManager *g_pUpdateManager = nullptr;
bool UpdateManager::init()
{

	if (!Layer::init())
	{
		return false ;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	auto m_spriteBg = LayerColor::create(cocos2d::Color4B::WHITE);
    m_spriteBg->setPosition(0, 0);
    m_spriteBg->setContentSize(winSize);
	//m_spriteBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
	//m_spriteBg->setScale(GameData::getInstance()->getGameScale());
	this->addChild(m_spriteBg, 0);


	g_pUpdateManager = this ;
	m_tipsLayer = nullptr ;
	m_infoLayer = nullptr ;


    _spriteLoading = Sprite::create("update/loading_bg.png");

    this->addChild(_spriteLoading, 3);
    _spriteLoading->setPosition(GAME_WIDTH/2,80);
    _progressBar = ui::LoadingBar::create("update/loading_prograss.png",0);
    _progressBar->setPosition(Vec2(GAME_WIDTH/2,80));	
    _progressBar->setPercent(0);
    this->addChild(_progressBar, 3);
    m_labelProgress = CreateLabelMSYH(24, "更新进度 : 0%");
    m_labelProgress->setTextColor(cocos2d::Color4B(0x33,0x9e,0x57,0xff));
    m_labelProgress->setPosition(GAME_WIDTH/2, 40);
    this->addChild(m_labelProgress);

	mLabelVersionCode =  CreateLabelMSYH(20, "当前版本号：");
	mLabelVersionCode->setPosition(1117,27) ;
	mLabelVersionCode->setAnchorPoint(Point(1,0.5)) ;
	//mLabelVersionCode->setTextColor(Color4B(0x50,0xbd,0x2e,255)) ;
    mLabelVersionCode->setTextColor(Color4B(0x81,0x81,0x81,255)) ;
	this->addChild(mLabelVersionCode);
	mLabelVersionCode->setVisible(true) ;

	//creatUpdateProgressBar() ;
	showUpdateTipsLayer(true) ;
	showUpdateInfoLayer(false) ;

	m_curPro = 0 ;

	char buf[64] ;
	sprintf(buf,"当前版本号：%s",getVersionName().c_str()) ;
	SetLabelString(mLabelVersionCode,buf) ;

	updateVersionCode(GameData::getInstance()->getNewVersionCode()) ;

	//this->schedule(schedule_selector(UpdateManager::update), 0.1f);  
	//this->schedule(schedule_selector(UpdateManager::update), 0.5f);  
	m_curProPx = 0 ;
	m_preProPx = 0 ;
	return true ;
}
void UpdateManager::update(float delta)
{
	m_curPro+=1 ;
	if (m_curPro>=100)
	{
		m_curPro = 0 ;
	}
	//updatePro(m_curPro) ;
    newUpdatePro(m_curPro);
}

void UpdateManager::showUpdateInfoLayer(bool flag)
{
	if (m_infoLayer == nullptr)
	{
		m_infoLayer = UpdateInfo::create() ;
		m_infoLayer->ignoreAnchorPointForPosition(false) ;
		m_infoLayer->setAnchorPoint(Point(0.5,0.5)) ;
		m_infoLayer->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2+10) ;
		this->addChild(m_infoLayer);
	}
	m_infoLayer->setVisible(flag) ;
	//m_ProgressBar.m_bgpro->setVisible(flag) ;
	/*if (flag)
	{
		this->schedule(schedule_selector(UpdateManager::creatMoveLight), 0.8f);  
	}*/
}

void UpdateManager::showUpdateTipsLayer(bool flag)
{
	if (m_tipsLayer == nullptr)
	{
		m_tipsLayer = UpdateTips::create() ;
		m_tipsLayer->ignoreAnchorPointForPosition(false) ;
		m_tipsLayer->setAnchorPoint(Point(0.5,0.5)) ;
		m_tipsLayer->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2+10) ;
		this->addChild(m_tipsLayer);
	}
	m_tipsLayer->setVisible(flag) ;

}

void UpdateManager::updatePro(int pro)
{
	char buf[64] ;
	sprintf(buf,"更新进度 : %d%%",pro) ;
	SetLabelString(m_ProgressBar.mLabelUpdatePro,buf) ;
	float cPro = pro*0.01 ;
	if (cPro>0)
	{
		m_ProgressBar.m_frontPro->setVisible(true) ;
	}
	m_ProgressBar.m_frontPro->setScaleX(cPro);
	m_curProPx =m_ProgressBar.m_frontPro->getContentSize().width*cPro+23;
	if (m_curProPx!=m_preProPx)
	{
		m_preProPx = m_curProPx ;
		m_ProgressBar.m_right->setPositionX(m_ProgressBar.m_frontPro->getContentSize().width*cPro+15) ;
		//m_ProgressBar.m_right->stopAllActions() ;
		//auto moveTo = MoveTo::create(0.5f,Point(m_curProPx,m_ProgressBar.m_right->getPositionY())) ;
		//m_ProgressBar.m_right->runAction(moveTo);
	}




}

void UpdateManager::newUpdatePro(int pro)
{
    char buf[64] ;
    sprintf(buf,"更新进度 : %d%%",pro) ;
    SetLabelString(m_labelProgress, buf);
    _progressBar->setPercent((float)pro);
}

void UpdateManager::updateVersionCode(std::string versionCode)
{

	m_infoLayer->updateTileVersion(versionCode) ;
}

void UpdateManager::showUpdateInfo(std::string info)
{
	m_infoLayer->updateInfo() ;
}

void UpdateManager::creatUpdateProgressBar()
{

	updateProgressBar proBar ;


	auto m_bgpro= Sprite::create("update/bgupdatepro.png") ;
	m_bgpro->setPosition(GameData::getInstance()->getGameWidth()/2,80) ;
	this->addChild(m_bgpro);

	auto m_frontPro = Sprite::create("update/frontpro.png") ;
	m_frontPro->setVisible(false) ;
	m_frontPro->setAnchorPoint(Point(0,0.5)) ;
	m_frontPro->setPosition(m_bgpro->getContentSize().width/2-m_frontPro->getContentSize().width/2,m_bgpro->getContentSize().height/2) ;
	m_bgpro->addChild(m_frontPro);


	//遮罩
	auto mmoban = Sprite::create("update/moveclilp.png") ;
	auto sz = mmoban->getContentSize();
	mmoban->setPosition(sz.width / 2+1, sz.height / 2);
	mmoban->setAnchorPoint(Point(0,0.5)) ;
	mmoban->setPosition(-m_frontPro->getContentSize().width/2-10,0) ;
	mmoban->retain();

	auto clippingNode = ClippingNode::create();
	clippingNode->setStencil(mmoban);     //设置模板
	//clippingNode->setInverted(false);        //设置底板可见
	clippingNode->setAlphaThreshold(0.5);         //设置绘制底板的Alpha值为0
	clippingNode->setPosition(GameData::getInstance()->getGameWidth()/2,80) ;
	clippingNode->setContentSize(sz);
	this->addChild(clippingNode);





	auto m_left = Sprite::create("update/left.png") ;
	m_left->setAnchorPoint(Point(0,0.5)) ;
	m_left->setPosition(7,m_bgpro->getContentSize().height/2);
	m_bgpro->addChild(m_left) ;

	auto m_right = Sprite::create("update/right.png") ;
	m_right->setAnchorPoint(Point(0,0.5)) ;
	m_right->setPosition(m_left->getContentSize().width+7,m_bgpro->getContentSize().height/2);
	m_bgpro->addChild(m_right) ;

	auto m_light = Sprite::create("update/updatelight.png") ;
	m_light->setAnchorPoint(Point(1,0.5)) ;
	m_light->setPosition(m_right->getContentSize().width+24,m_right->getContentSize().height/2);
	m_right->addChild(m_light) ;

	auto mLabelUpdatePro =  CreateLabelMSYH(22, "更新进度");
	mLabelUpdatePro->setPosition(m_bgpro->getContentSize().width/2,-35) ;
	mLabelUpdatePro->setAnchorPoint(Point(0.5,0.5)) ;
	mLabelUpdatePro->setTextColor(Color4B::WHITE) ;
	m_bgpro->addChild(mLabelUpdatePro);




	proBar.m_bgpro = m_bgpro ;
	proBar.m_frontPro = m_frontPro ;
	proBar.m_left = m_left ;
	proBar.m_right = m_right ;
	proBar.m_light = m_light ;
	proBar.mLabelUpdatePro = mLabelUpdatePro ;
	proBar.mmoban = mmoban ;
	proBar.mclippingNode = clippingNode ;
	m_ProgressBar = proBar ;

	m_ProgressBar.m_bgpro->setVisible(false) ;
}

void UpdateManager::showUpdateProgressBar(bool flag)
{

}

void UpdateManager::creatMoveLight(float delta)
{
	auto m_moveLight = Sprite::create("update/movelight.png") ;
	m_moveLight->setAnchorPoint(Point(0.5,0.5)) ;
	m_moveLight->setPosition(-m_ProgressBar.m_bgpro->getContentSize().width/2-m_moveLight->getContentSize().width,0);
	m_ProgressBar.mclippingNode->addChild(m_moveLight);


	auto light = m_moveLight ;
	auto move = [this,light](){
		float dis =m_ProgressBar.m_right->getPositionX() -light->getPositionX()- m_ProgressBar.m_bgpro->getContentSize().width/2;
		auto moveEnd = [light](){
			light->removeFromParentAndCleanup(true) ;
		} ;
		auto moveAction = MoveTo::create(dis/30*0.1f,Point(m_ProgressBar.m_right->getPositionX()-m_ProgressBar.m_bgpro->getContentSize().width/2-light->getContentSize().width/4,light->getPositionY())) ;
		light->runAction(Sequence::create(moveAction,CallFunc::create(moveEnd),NULL)) ;
	} ;
	m_moveLight->runAction(CallFunc::create(move)) ;




	//auto m_moveLight = Sprite::create("update/movelight.png") ;
	//m_moveLight->setAnchorPoint(Point(0.5,0.5)) ;
	//m_moveLight->setPosition(0,m_ProgressBar.m_left->getPositionY());
	//m_ProgressBar.m_bgpro->addChild(m_moveLight) ;
	//auto light = m_moveLight ;
	//auto move = [this,light](){
	//	float dis =m_ProgressBar.m_right->getPositionX() -light->getPositionX() ;
	//	auto moveEnd = [light](){
	//		light->removeFromParentAndCleanup(true) ;
	//	} ;
	//	auto moveAction = MoveTo::create(dis/30*0.1f,Point(m_ProgressBar.m_right->getPositionX(),m_ProgressBar.m_left->getPositionY())) ;
	//	light->runAction(Sequence::create(moveAction,CallFunc::create(moveEnd),NULL)) ;
	//} ;
	//m_moveLight->runAction(CallFunc::create(move)) ;
}

void UpdateManager::updateMoveLight(float delta)
{
	//float px = m_ProgressBar.m_moveLight->getPositionX() ;
	//if (px+1>=m_ProgressBar.m_right->getPositionX())
	//{
	//	px = 0 ;
	//}
	//else
	//{
	//	float dis =m_ProgressBar.m_right->getPositionX() -px ;
	//	px+=dis/10 ;
	//}
	//m_ProgressBar.m_moveLight->setPositionX(px) ;
}