#ifndef __HTTPSPRITE_H__
#define __HTTPSPRITE_H__

#include "cocos2d.h"
#include <string>

namespace cocos2d
{
	namespace network
	{
		class HttpClient;
		class HttpResponse;
	}
}

class HttpSprite : public cocos2d::Layer
{
public:
	static HttpSprite *create(const std::string& strDefaultLocalSprite, cocos2d::Size sz = cocos2d::Size::ZERO);
	virtual bool init(const std::string& strDefaultLocalSprite, cocos2d::Size sz);

	void ResetSourceUrl(const std::string& strSourceUrl);

	void ResetToDefaultSprite();

	void ResetToLocalSprite(std::string path);
	inline void useDownLoadSize() {m_useDownLoadedSpriteSize = true ;};
	inline cocos2d::Sprite* getSprite(){ return m_pSprite ;} ;

	void StopScheduleCheckDownload();
protected:
		// http callback
	void onHttpResponseGetSprite(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
	void onScheduleCheckDownload(float fDelta);

private:
	HttpSprite();
	~HttpSprite();

	void StartScheduleCheckDownload();
	void NotifyScheduleCheckDownload();
	
	void UpdateSprite();

	bool m_bDefaultSprite;
	std::string m_strDefaultLocalSprite;
	cocos2d::Sprite *m_pSprite;
	std::string m_strSourceUrl, m_strLocalFullPath,m_strSourceUrl_requst;
	bool m_bDownloaded;
	bool m_useDownLoadedSpriteSize ;//使用下载下来的图片尺寸
};




#endif