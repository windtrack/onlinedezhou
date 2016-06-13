#include "LoadManager.h"
#include "UICommon.h"
#include "GameData.h"
USING_NS_CC;

LoadManager* LoadManager::s_instance = nullptr; 
LoadManager* LoadManager::getInstance()
{
	if (s_instance == 0 )
	{
		s_instance = new LoadManager() ;
	}

	return s_instance ;
}


void LoadManager::callLoadRes()
{

	 s_curAniIndex =0;
	 s_curPlistIndex =0;
	 s_curPngIndex =0;

	s_curLoadIndex = 0 ;
	//s_maxLoadCount = ResAniCount + ResPlistCount + (ResPngCount);
	s_maxLoadCount = ResAniCount + ResPlistCount + (ResPngCount);
	//jsonÎÄ¼þÔ¤¼ÓÔØ
	loadingResources(kResAnim);


}

void LoadManager::loadingResources(ResType type)
{
	switch (type)
	{
	case kResAnim:
		for (int i=0 ; i<ResAniCount; i++ )
		{
			ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(aniPath[i],  this, schedule_selector(LoadManager::jsonAsyncCallback)); 
		}
		break;
	case kResPlist:
		for (int i=0 ; i<ResPlistCount; i++ )
		{
			std::string  path = plistPath[i]+".png" ;
			TextureCache::sharedTextureCache()->addImageAsync(path, CC_CALLBACK_1(LoadManager::plistCallBack, this));
		}
		break;
	case kResPng:
		for (int i=0 ; i<ResPngCount; i++ )
		{

			TextureCache::sharedTextureCache()->addImageAsync(pngPath[i],  CC_CALLBACK_0(LoadManager::pngCallBack, this));
		}
		break;

	default:
		break;
	}
}

void LoadManager::jsonAsyncCallback(float dt)
{
	s_curAniIndex++ ;
	if (s_curAniIndex >= ResAniCount) {
		loadingResources(kResPlist);
	}
	callLoadback();
}
void LoadManager::pngCallBack()
{
	s_curPngIndex++ ;
	callLoadback();
}
void LoadManager::plistCallBack(cocos2d::Texture2D* texture)
{
	std::string  path = plistPath[s_curPlistIndex]+".plist" ;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(path, texture);
	s_curPlistIndex++ ;
	if (s_curPlistIndex >= ResPlistCount)
		loadingResources(kResPng);
	callLoadback();
}

void LoadManager::callLoadback()
{
	s_curLoadIndex++ ;
}

void LoadManager::delayLoadingAnims()
{
	for (int i = 0; i < DelayLoadingAnimCount; i++) {
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(delayLoadingAnimPath[i],  this, schedule_selector(LoadManager::jsonAsyncCallback2)); 
	}
}

void LoadManager::jsonAsyncCallback2(float dt)
{
	// TODO: nothing
}