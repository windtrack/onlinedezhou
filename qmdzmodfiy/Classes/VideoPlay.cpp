#include "VideoPlay.h"
#include "cocos2d.h"
#include "PlatformSpec.h"
#include "GrabTouch.h"
#include "GlobalUserData.h"
#include "Util.h"
#include "GameData.h"
USING_NS_CC;

class VideoPlayer : public Layer
{
public:
	CREATE_FUNC(VideoPlayer);
	virtual bool init()
	{
		if (!Layer::init())
		{
			return false;
		}
		this->ignoreAnchorPointForPosition(false);
		this->setAnchorPoint(Point(0.5,0.5));
		auto bgbound = Sprite::create("video/bgbound.png");
		m_pBgVideo = Sprite::create("video/bgvideo.png");
		auto sz = bgbound->getContentSize();
		#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			m_pBgVideo->setVisible(false);
		#endif
		setContentSize(sz);
		bgbound->setPosition(sz.width / 2, sz.height / 2);
		addChild(bgbound);
		m_pBgVideo->setPosition(sz.width / 2, sz.height / 2);
		addChild(m_pBgVideo);

		auto label = CreateLabelMSYH(20, "视频连接中...");
		label->setPosition(sz.width / 2, 22);
		addChild(label);

		auto btCloseVideo = MenuItemImage::create("video/btclose.png", "video/btclose.png");
		auto btCloseSound = MenuItemImage::create("video/vediopng.png", "video/vediopng.png");
		auto btOpenSound = MenuItemImage::create("video/vediopng.png", "video/vediopng.png");

		//auto btCloseVideo = MenuItemImage::create("video/btclose.png", "video/btclose.png");
		//auto btCloseSound = MenuItemImage::create("video/btclosedound.png", "video/btclosedound.png");
		//auto btOpenSound = MenuItemImage::create("video/btopensound.png", "video/btopensound.png");

		btCloseVideo->setPosition(175, 127);
		btCloseVideo->setCallback([this](Ref*){Stop();});
		btCloseSound->setPosition(98, 34);
		btCloseSound->setCallback([this, btCloseSound, btOpenSound](Ref*){
			setVideoSoundEnable(false);
			btCloseSound->setVisible(false);
			btOpenSound->setVisible(true);});
		btOpenSound->setPosition(98, 34);
		btOpenSound->setVisible(false);
		btOpenSound->setCallback([this, btCloseSound, btOpenSound](Ref*){
			setVideoSoundEnable(true);
			btCloseSound->setVisible(true);
			btOpenSound->setVisible(false);
		});
		auto menu = Menu::create(btCloseVideo, btOpenSound, btCloseSound, nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu);
		menu->setVisible(false);
		m_pMenu = menu;
		m_pBtOpenSound = btOpenSound;
		m_pBtCloseSound = btCloseSound;
		m_pBindBt = nullptr;
		m_strVideoStreamUrl = "";

		QuickGrabTouch(this, true, [menu, this](Touch*touch, Event*event) -> bool {
			if (this->isVisible() && TouchHitNode(touch, this))
			{ 
				menu->stopAllActions();
				menu->runAction(Sequence::create(Show::create(), DelayTime::create(5.0f), CallFunc::create([](){setVideoToolsVisible(false);}), Hide::create(), nullptr));
				setVideoToolsVisible(true);
				return false;
			}
			return false;
		});
		return true;
	}

	void Stop2(bool checkBind = true)
	{
		setVideoVisible(false);

		m_pMenu->stopAllActions();
		m_pMenu->setVisible(false);
		m_pBtCloseSound->setVisible(true);
		m_pBtOpenSound->setVisible(false);

		m_strVideoStreamUrl = "";
		setVisible(false);

		if (nullptr != m_pBindBt) {
			if (checkBind) {
				m_pBindBt->setVisible(true);
			} else {
				m_pBindBt->setVisible(false);
			}
		}
	}

	void StartPlay(const std::string& strVideoStreamUrl)
	{
		if (strVideoStreamUrl.empty() || m_strVideoStreamUrl != strVideoStreamUrl)
		{
			m_pMenu->stopAllActions();
			m_pMenu->setVisible(false);
			m_pBtCloseSound->setVisible(true);
			m_pBtOpenSound->setVisible(false);
			

			m_strVideoStreamUrl = strVideoStreamUrl;
			bool bHasVideo = !m_strVideoStreamUrl.empty();
			setVisible(bHasVideo);

			if (m_pBindBt)
			{
				m_pBindBt->setVisible(!bHasVideo);
			}
			if (bHasVideo)
			{
				stopVideo();
				auto director = Director::getInstance();
				auto szV = director->getVisibleSize();
				auto glView = director->getOpenGLView();
				auto szGlView = glView->getFrameSize();
				float fScaleX = szGlView.width / szV.width;
				float fScaleY = szGlView.height / szV.height; 

				auto sz = m_pBgVideo->getContentSize();
				auto szA = getContentSize();
				auto ptWld = convertToWorldSpace(Vec2((szA.width - sz.width) / 2, szA.height - (szA.height - sz.height) / 2));
				auto ptUi = Director::getInstance()->convertToUI(ptWld);
				ptUi.x *= fScaleX;
				ptUi.y *= fScaleY;
				std::string strFullUrl  = g_globalServerInfo.m_strVideoServerApp + m_strVideoStreamUrl;
				setVideoVisible(true);

				float curx =this->getPosition().x;
				float cury =this->getPosition().y;

				playVideo(strFullUrl.c_str(), ptUi.x, ptUi.y, sz.width * fScaleX, sz.height * fScaleY);
				//playVideo(strFullUrl.c_str(), curx, cury, sz.width * fScaleX, sz.height * fScaleY);
			} else {
				setVideoVisible(false);
				stopVideo();
			}
		}
		setVideoToolsVisible(false);
	}

	void Stop()
	{
		StartPlay("");
	}

	std::string GetVideoStreamUrl()
	{
		return m_strVideoStreamUrl;
	}

	void BindButton(Node*pBt)
	{
		m_pBindBt = pBt;
	}

private:
	std::string m_strVideoStreamUrl;
	Sprite *m_pBgVideo;
	Menu *m_pMenu;
	MenuItem *m_pBtOpenSound, *m_pBtCloseSound;
	Node *m_pBindBt;
};
VideoPlayMgr::VideoPlayMgr()
{
	for (auto&player : m_pVideoPlayer)
	{
		player = nullptr;
	}
}
VideoPlayMgr::~VideoPlayMgr()
{
	StopAll();
}

void VideoPlayMgr::InitTable(cocos2d::Node *pParent, int iZOrder)
{
	m_pParent = pParent;
	m_iZOrder = iZOrder;

	//static Vec2 s_ptOpen[] = {
	//	Vec2(898, 477), Vec2(904, 397), Vec2(904, 397),
	//	Vec2(962, 183), Vec2(488, 180), Vec2(172, 180),
	//	Vec2(34, 206), Vec2(34, 206), Vec2(242, 477),
	//};

	for (int i=0;i<9;i++)
	{
		s_ptOpen[i] = GameData::getInstance()->s_ptOpen[i];
		s_ptPlayer[i] =GameData::getInstance()->s_ptPlayer[i];
	}


	auto menu = Menu::create();
	for (unsigned i = 0; i < 9; ++i)
	{
		auto& bt = m_pBtOpenVideo[i];
		auto cbOpenVideo = [this, i](Ref*){
				StartPlay(i, m_strVideoStreamUrls[i]);
		};
		bt = MenuItemImage::create("video/btopenvideo.png", "video/btopenvideo.png", cbOpenVideo);
		bt->setVisible(false);
		bt->setPosition(s_ptOpen[i]);
		menu->addChild(bt);
	}
	menu->setPosition(Vec2::ZERO);
	pParent->addChild(menu, iZOrder);

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	
//#endif

}

void VideoPlayMgr::PrePlay(unsigned uViewChair, const std::string& strVideoStreamUrl)
{
	m_strVideoStreamUrls[uViewChair] = strVideoStreamUrl;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_pBtOpenVideo[uViewChair]->setVisible(true);
#endif
	
}

void VideoPlayMgr::StartPlay(unsigned uViewChair, const std::string& strVideoStreamUrl)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return;
#endif

    int enterFlag = g_globalMyData.GetVipLevel() + g_globalMyData.GetLadyLevel();
    if (uViewChair == g_globalTableConfig.GetMyViewChairID() && enterFlag < 1 && g_globalTableConfig.GetTableMode() == TablePlaying)
    {
        SendClientRequest(ClientRequest::kStartPlayVideoInfo);
        return;
    }
	StopAll();
	//m_pBtOpenVideo[uViewChair]->setVisible(false);
	auto player = CreatePlayer(uViewChair);
	player->StartPlay(strVideoStreamUrl);
}

void VideoPlayMgr::StopPlay(unsigned uViewChair)
{
	auto player = FindPlayer(uViewChair);
	if (player)
	{
		//m_pBtOpenVideo[uViewChair]->setVisible(true);
		player->Stop2();
	}
}

//void VideoPlayMgr::StopPlay(const std::string& strVideoStreamUrl)
//{
//	auto player = FindPlayer(strVideoStreamUrl);
//	if (player)
//	{
//		player->Stop();
//	}
//}

void VideoPlayMgr::StopAll()
{
	for (unsigned i  = 0; i < 9; ++i)
	{
		auto player = m_pVideoPlayer[i];
		if (player != nullptr)
		{
			PlayerInfo* pInfo = g_globalTableConfig.GetPlayerByViewChair(i);
			player->Stop2(nullptr != pInfo);
		}
	}

	//stopVideo();
	setVideoToolsVisible(false);
}

void VideoPlayMgr::RemovePlay(unsigned uViewChair)
{
	StopPlay(uViewChair);
	m_pBtOpenVideo[uViewChair]->setVisible(false);
}

VideoPlayer * VideoPlayMgr::CreatePlayer(unsigned uViewChair)
{
	//static Vec2 s_ptPlayer[] = {
	//	Vec2(904, 397), Vec2(904, 397), Vec2(904, 397),
	//	Vec2(941, 206), Vec2(310, 88), Vec2(34, 206),
	//	Vec2(310, 88), Vec2(310, 88), Vec2(34, 397),
	//};
	auto& player = m_pVideoPlayer[uViewChair];
	if (nullptr == player)
	{
		player = VideoPlayer::create();
		player->setPosition(s_ptPlayer[uViewChair]);
		player->BindButton(m_pBtOpenVideo[uViewChair]);
		m_pParent->addChild(player, m_iZOrder);
	}
	return player;
}

VideoPlayer * VideoPlayMgr::FindPlayer(unsigned uViewChair)
{
	return m_pVideoPlayer[uViewChair];
}

VideoPlayer * VideoPlayMgr::FindPlayer(const std::string& strVideoStreamUrl)
{
	for (auto player : m_pVideoPlayer)
	{
		if (player && player->GetVideoStreamUrl() == strVideoStreamUrl)
		{
			return player;
		}
	}
	return nullptr;
}

void VideoPlayMgr::SetVisible(bool visible)
{
	for (auto item : m_pBtOpenVideo) {
		if (nullptr != item) {
			item->setVisible(false);
		}
	}

	for (auto player : m_pVideoPlayer)
	{
		if (nullptr != player)
			player->setVisible(false);
	}
}