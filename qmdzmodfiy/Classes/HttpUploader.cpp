#include "HttpUploader.h"

#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "cocos2d.h"
#include "GameFrame.h"

USING_NS_CC;

HttpUploader* s_httpUploader = nullptr;

HttpUploader *HttpUploader::getInstance()
{
    if (s_httpUploader == nullptr) {
        s_httpUploader = new HttpUploader();
    }
    
	return s_httpUploader;
}

bool HttpUploader::IsUploading(const std::string& strUrl)
{
	for(auto item : m_lstUploadRequest)
	{
		if (item.m_strUrl == strUrl)
		{
			return true;
		}
	}
	return false;
}

void HttpUploader::AddRequest(const std::string& strUrl, const std::string& strLocal, const std::string& strTips, std::function<void(bool, std::string)> cbUploaded)
{
    g_pGameFrame->PopLoading(true, strTips.c_str());
    Data data = FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->getWritablePath() + strLocal);
    
	network::HttpRequest *request = new network::HttpRequest();
    request->setRequestType(network::HttpRequest::Type::POST);
	request->setUrl(strUrl.c_str());
    
    UploadRequest uploadRequest;
    uploadRequest.m_strUrl = strUrl;
    uploadRequest.m_strLocal = strLocal;
    uploadRequest.m_cbUploaded = cbUploaded;
    m_lstUploadRequest.push_back(uploadRequest);
    request->setRequestData((const char*)data.getBytes(), data.getSize());
    
	request->setResponseCallback(CC_CALLBACK_2(HttpUploader::onHttpResponse, this)); //ªÿµ˜
	network::HttpClient::getInstance()->sendImmediate(request);
    request->release();
}

HttpUploader::HttpUploader()
{
}

HttpUploader::~HttpUploader()
{
}

void HttpUploader::onHttpResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
{
    bool bSucceed = false;
    g_pGameFrame->PopLoading(false);
	if (response)
	{
		network::HttpRequest *pRequest = response->getHttpRequest();
		if (pRequest)
		{
			std::string strUrl(pRequest->getUrl());
			for (auto iter = m_lstUploadRequest.begin(); iter != m_lstUploadRequest.end(); ++iter)
			{
				if (iter->m_strUrl == strUrl)
				{
					if (response->isSucceed())
					{
                        bSucceed = true;
					}
                    
                    //log("reponose: %s", response->getResponseData());
                    auto respData = response->getResponseData();
                    std::vector<char>& vecResponseData = *respData;
                    std::string picUrl(vecResponseData.begin(), vecResponseData.end());
                    
					auto cb = iter->m_cbUploaded;
					m_lstUploadRequest.erase(iter);
					if (cb)
					{
						cb(bSucceed, picUrl);
					}
					break;
				}
			}
		}
	}
}
