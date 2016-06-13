#ifndef __HTTPUPLOADER_H__
#define __HTTPUPLOADER_H__

#include <string>
#include <functional>
#include <list>

namespace cocos2d
{
	namespace network
	{
		class HttpClient;
		class HttpResponse;
	}
}

class HttpUploader
{
public:
	static HttpUploader *getInstance();
	bool IsUploading(const std::string& strUrl);
	void AddRequest(const std::string& strUrl, const std::string& strLocal, const std::string& strTips, std::function<void(bool, std::string)> cbUploaded);

    HttpUploader();
    ~HttpUploader();
protected:


	void onHttpResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

private:
	struct UploadRequest
	{
		std::string m_strUrl;
		std::string m_strLocal;
		std::function<void(bool, std::string)> m_cbUploaded;
	};
	std::list<UploadRequest> m_lstUploadRequest;
};


#endif