#include "HttpDownloader.h"

#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

USING_NS_CC;

HttpDownloader *HttpDownloader::getInstance()
{
	static HttpDownloader s_httpDownloader;
	return &s_httpDownloader;
}

bool HttpDownloader::IsDownloading(const std::string& strUrl)
{
	for(auto item : m_lstDownloadRequest)
	{
		if (item.m_strUrl == strUrl)
		{
			return true;
		}
	}
	return false;
}

void HttpDownloader::AddRequest(const std::string& strUrl, const std::string& strLocal, std::function<void(bool)> cbDownloaded)
{
	network::HttpRequest *request = new network::HttpRequest();
	request->setRequestType(network::HttpRequest::Type::GET);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(CC_CALLBACK_2(HttpDownloader::onHttpResponse, this)); //»Øµ÷
	network::HttpClient::getInstance()->send(request);

	DownloadRequest downloadRequest;
	downloadRequest.m_strUrl = strUrl;
	downloadRequest.m_strLocal = strLocal;
	downloadRequest.m_cbDownloaded = cbDownloaded;
	m_lstDownloadRequest.push_back(downloadRequest);
}

HttpDownloader::HttpDownloader()
{
}

HttpDownloader::~HttpDownloader()
{
}

void HttpDownloader::onHttpResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
{
	if (response)
	{
		network::HttpRequest *pRequest = response->getHttpRequest();
		if (pRequest)
		{
			std::string strUrl(pRequest->getUrl());
			for (auto iter = m_lstDownloadRequest.begin(); iter != m_lstDownloadRequest.end(); ++iter)
			{
				if (iter->m_strUrl == strUrl)
				{
					bool bSucceed = false;
					if (response->isSucceed())
					{
						std::vector<char> *pVecResponseData = response->getResponseData();
						if (pVecResponseData && !pVecResponseData->empty())
						{
							FILE *file = fopen(iter->m_strLocal.c_str(), "wb");
							if (file)
							{
								std::vector<char>& vecResponseData = *pVecResponseData;
								fwrite(&vecResponseData[0], sizeof(char), vecResponseData.size(), file);
								fclose(file);
								bSucceed = true;
							}
						}
					}
					auto cb = iter->m_cbDownloaded;
					m_lstDownloadRequest.erase(iter);
					if (cb)
					{
						cb(bSucceed);
					}
					break;
				}
			}
		}
	}
}
