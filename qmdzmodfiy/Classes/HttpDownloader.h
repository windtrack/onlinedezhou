#ifndef __HTTPDOWNLOADER_H__
#define __HTTPDOWNLOADER_H__

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

class HttpDownloader
{
public:
	static HttpDownloader *getInstance();
	bool IsDownloading(const std::string& strUrl);
	void AddRequest(const std::string& strUrl, const std::string& strLocal, std::function<void(bool)> cbDownloaded);


protected:
	HttpDownloader();
	~HttpDownloader();

	void onHttpResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

private:
	struct DownloadRequest
	{
		std::string m_strUrl;
		std::string m_strLocal;
		std::function<void(bool)> m_cbDownloaded;
	};
	std::list<DownloadRequest> m_lstDownloadRequest;
};


#endif