#include "SoundManager.h"
#include "SimpleAudioEngine.h"
USING_NS_CC ;


SoundManager* SoundManager::m_instance = nullptr; 
SoundManager* SoundManager::getInstance()
{
	if (m_instance == 0 )
	{
		m_instance = new SoundManager() ;
	}

	return m_instance ;
}

std::string SoundManager::getSoundPath(int uIndex,int type)
{
	unsigned uCounty = 0;
	unsigned uGender = 1;
	std::string strBase = "sound/skin/";

	if (uIndex == 0)
	{
		uCounty = 0 ;
		uGender = 0 ;
	}

	static char *szCounty[] = {"china/", "foreign/","christmas/"};
	strBase += szCounty[uCounty];
	static char *szGender[] = {"woman/", "man/"};
	strBase += szGender[uGender];

	return strBase ;
}

void SoundManager::playEffect(int uIndex,int type,bool loop)
{
	std::string strBase = getSoundPath(uIndex,type) ;

	playEffect(strBase,loop) ;

}

void SoundManager::playEffect(std::string strBase,bool loop)
{
	int id = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(strBase.c_str(),false) ;

	if (m_mapSoundID.find(strBase)==m_mapSoundID.end())
	{
		m_mapSoundID.insert(std::pair<std::string,int>(strBase,id)) ;
	}

}
void SoundManager::pauseEffect(int uIndex,int type)
{
	std::string strBase = getSoundPath(uIndex,type) ;
	pauseEffect(strBase) ;

}
void SoundManager::pauseEffect(std::string strBase)
{

	std::map<std::string,int>::iterator It;
	It = m_mapSoundID.find(strBase);
	if (It!=m_mapSoundID.end())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(It->second) ;
	}

}


//背景音乐只有一个
void SoundManager::playBackGroundMusic()
{
	if (m_isMusicOn)
	{
		auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
		if (!pAudioEngine->isBackgroundMusicPlaying())
		{
			//pAudioEngine->playBackgroundMusic(strSound.c_str(), true);
		}
	}
}

void SoundManager::pauseBackGroundMusic()
{

}

void SoundManager::stopBackGroundMusic()
{

}