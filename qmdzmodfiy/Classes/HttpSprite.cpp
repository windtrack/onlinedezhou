#include "HttpSprite.h"
#include "cocos2d.h"

#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "Util.h"

USING_NS_CC;

//namespace
//{
//std::string GetSourceLocalShortPath(const std::string& strSourceUrl)
//{
//	
//	char szBuf[1024];
//	strncpy(szBuf, strSourceUrl.c_str(), 1024);
//	unsigned uIdx = 0;
//	while (szBuf[uIdx] != 0)
//	{
//		if ((szBuf[uIdx] >= 'a' && szBuf[uIdx] <= 'z') ||
//			(szBuf[uIdx] >= 'A' && szBuf[uIdx] <= 'Z') ||
//			(szBuf[uIdx] >= '0' && szBuf[uIdx] <= '9'))
//		{
//		}
//		else
//		{
//			szBuf[uIdx] = '_';
//		}
//		++uIdx;
//	}
//	std::string strSrcUrl(szBuf);
//	return strSrcUrl;
//}
//
//std::string GetSourceLocalFullPath(const std::string& strSourceUrl)
//{
//	FileUtils *pFileUtils = FileUtils::getInstance();
//	std::string strWritableDir = pFileUtils->getWritablePath();
//	std::string strFullPath = strWritableDir + GetSourceLocalShortPath(strSourceUrl);
//	return strFullPath;
//}
//
//bool CheckSpriteExist(const std::string& strSpriteLocalPath)
//{
//	FileUtils *pFileUtils = FileUtils::getInstance();
//	return pFileUtils->isFileExist(strSpriteLocalPath);
//}
//}


HttpSprite *HttpSprite::create(const std::string& strDefaultLocalSprite, Size sz)
{
    HttpSprite *pRet = new HttpSprite();
    if (pRet && pRet->init(strDefaultLocalSprite, sz)) 
    { 
        pRet->autorelease(); 
        return pRet;
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
    } 
    return pRet; 
}

bool HttpSprite::init(const std::string& strDefaultLocalSprite, Size sz)
{
	if (!Layer::init())
	{
		return false;
	}
	m_useDownLoadedSpriteSize = false;
	m_bDefaultSprite = true;
	m_strDefaultLocalSprite = strDefaultLocalSprite;
	m_pSprite = Sprite::create(m_strDefaultLocalSprite);
	if (sz.width < 0.1 && sz.height < 0.1)
	{
		sz = m_pSprite->getContentSize();
	}
	setContentSize(sz);
	m_pSprite->setPosition(sz.width / 2, sz.height / 2);
	auto szImg = m_pSprite->getContentSize();
	m_pSprite->setScaleX(sz.width / szImg.width);
	m_pSprite->setScaleY(sz.height / szImg.height);
	addChild(m_pSprite);
	return true;
}

void HttpSprite::ResetToDefaultSprite()
{
	m_pSprite->setScale(1.0f);
	m_pSprite->setTexture(m_strDefaultLocalSprite); 
	Size sz = getContentSize();
	Size szSprite = m_pSprite->getContentSize();
	m_pSprite->setScale(sz.width / szSprite.width, sz.height / szSprite.height);
	m_bDefaultSprite = true;
}

void HttpSprite::ResetToLocalSprite(std::string path)
{
	m_pSprite->setScale(1.0f);
	m_pSprite->setTexture(path); 
	Size sz = getContentSize();
	Size szSprite = m_pSprite->getContentSize();
	m_pSprite->setScale(sz.width / szSprite.width, sz.height / szSprite.height);
	m_strSourceUrl = path;
	//m_bDefaultSprite = true;
}

void HttpSprite::ResetSourceUrl(const std::string& strSourceUrl)
{
	if (strSourceUrl.length() > 7 && m_strSourceUrl != strSourceUrl)
	{
		m_strSourceUrl = strSourceUrl;
		m_strLocalFullPath = ConvertUrl2LocalFullPath(strSourceUrl);

	/*	if (m_strLocalFullPath.compare("C:/Users/Administrator/AppData/Local/QMTexasPoker_new/defaulticon.png"))
		{
			int aaa = 0 ;
		}*/

		if (IsFileExist(m_strLocalFullPath))
		{
			StopScheduleCheckDownload();
			UpdateSprite();
		}
		else
		{
				
				


			if (!m_bDefaultSprite)
			{
				m_pSprite->setScale(1.0f);
				m_pSprite->setTexture(m_strDefaultLocalSprite); 
				Size sz = getContentSize();
				Size szSprite = m_pSprite->getContentSize();
				m_pSprite->setScale(sz.width / szSprite.width, sz.height / szSprite.height);
				m_bDefaultSprite = true;
			}

			StartScheduleCheckDownload();

			network::HttpRequest *request = new network::HttpRequest();

			request->setRequestType(network::HttpRequest::Type::GET);

			char buff[128];
			sprintf(buff,"%s?%d",m_strSourceUrl.c_str(),getCurrentTime()) ;
			m_strSourceUrl_requst = buff ;

			request->setUrl(m_strSourceUrl_requst.c_str());
			//request->setUrl(strSourceUrl.c_str());
			request->setResponseCallback(CC_CALLBACK_2(HttpSprite::onHttpResponseGetSprite, this));//回调
			network::HttpClient::getInstance()->send(request);
//			request->release();
		}
	}
}

void HttpSprite::onHttpResponseGetSprite(network::HttpClient* client, network::HttpResponse* response)
{
	if (response && response->isSucceed())
	{
		network::HttpRequest *pRequest = response->getHttpRequest();
		if (pRequest)
		{
			std::string strUrl(pRequest->getUrl());


			if (strUrl == m_strSourceUrl_requst)
			//if (strUrl == m_strSourceUrl)
			{
				std::vector<char> *pVecResponseData = response->getResponseData();
				if (pVecResponseData && !pVecResponseData->empty())
				{
					FILE *file = fopen(m_strLocalFullPath.c_str(), "wb");
					if (file)
					{
						std::vector<char>& vecResponseData = *pVecResponseData;
						fwrite(&vecResponseData[0], sizeof(char), vecResponseData.size(), file);
						fclose(file);
						NotifyScheduleCheckDownload();
					}
				}
			}
		}		
	}
	//response->release();
}

void HttpSprite::onScheduleCheckDownload(float fDelta)
{
	if (m_bDownloaded)
	{
		UpdateSprite();
		StopScheduleCheckDownload();
	}
}

void HttpSprite::StartScheduleCheckDownload()
{
	m_bDownloaded = false;
	schedule(schedule_selector(HttpSprite::onScheduleCheckDownload), 1.0f, 100, 1.0f);
}

void HttpSprite::NotifyScheduleCheckDownload()
{
	m_bDownloaded = true;
}

void HttpSprite::StopScheduleCheckDownload()
{
	unschedule(schedule_selector(HttpSprite::onScheduleCheckDownload));
	m_bDownloaded = false;
}

void HttpSprite::UpdateSprite()
{
	auto sprite = Sprite::create(m_strLocalFullPath);
	if (sprite)
	{
		m_pSprite->removeFromParentAndCleanup(true);
		m_pSprite =	sprite;
		Size sz ;
		if (m_useDownLoadedSpriteSize)
		{
			 sz = m_pSprite->getContentSize();
			 setContentSize(sz);
			 m_pSprite->setPosition(sz.width / 2, sz.height / 2);
			 addChild(m_pSprite);
			 Size szSprite = m_pSprite->getContentSize();
			 m_bDefaultSprite = false;
			 setContentSize(sz);
		}
		else
		{
			 sz = getContentSize();
			 m_pSprite->setPosition(sz.width / 2, sz.height / 2);
			 addChild(m_pSprite);
			 Size szSprite = m_pSprite->getContentSize();
			 m_pSprite->setScale(sz.width / szSprite.width, sz.height / szSprite.height);
			 m_bDefaultSprite = false;
		}
		

	}

	//if (m_pSprite != nullptr)
	//{
	//	m_pSprite->setScale(1.0f);
	//	m_pSprite->setTexture(m_strLocalFullPath);
	//}
	//else
	//{
	//	auto sprite = Sprite::create(m_strLocalFullPath);
	//	if (sprite)
	//	{
	//		m_pSprite =	sprite;
	//	}
	//}
	//if (m_pSprite)
	//{
	//	Size sz = getContentSize();
	//	Size szSprite = m_pSprite->getContentSize();
	//	m_pSprite->setScale(sz.width / szSprite.width, sz.height / szSprite.height);
	//	m_bDefaultSprite = false;
	//}
}


HttpSprite::HttpSprite()
	: m_pSprite(nullptr)
	, m_bDownloaded(false)
	, m_bDefaultSprite(true)
{
}

HttpSprite::~HttpSprite()
{
}

