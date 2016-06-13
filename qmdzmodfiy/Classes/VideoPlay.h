#ifndef __VIDEOPLAY_H__
#define __VIDEOPLAY_H__

#include <string>
#include "cocos2d.h"
namespace cocos2d
{
	class Node;
	class MenuItem;
}

class VideoPlayer;
class VideoPlayMgr
{
public:
	VideoPlayMgr();
	~VideoPlayMgr();
	void InitTable(cocos2d::Node *pParent, int iZOrder);
	void PrePlay(unsigned uViewChair, const std::string& strVideoStreamUrl);
	void StartPlay(unsigned uViewChair, const std::string& strVideoStreamUrl);
	void StopPlay(unsigned uViewChair);
	// void StopPlay(const std::string& strVideoStreamUrl);
	void StopAll();
	void RemovePlay(unsigned uViewChair);

	void SetVisible(bool visible);
private:
	VideoPlayer * CreatePlayer(unsigned uViewChair);
	VideoPlayer * FindPlayer(unsigned uViewChair);
	VideoPlayer * FindPlayer(const std::string& strVideoStreamUrl);
private:
	cocos2d::Node *m_pParent;
	int m_iZOrder;
	VideoPlayer *m_pVideoPlayer[9];
	cocos2d::MenuItem *m_pBtOpenVideo[9];
	std::string m_strVideoStreamUrls[9];

	cocos2d::Vec2 s_ptOpen[9];

	cocos2d::Vec2 s_ptPlayer[9];
};

#endif