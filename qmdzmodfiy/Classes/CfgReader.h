#ifndef __UPDATECFGREADER_H__
#define __UPDATECFGREADER_H__

#ifdef WIN32
#include <Windows.h>
#endif

class CfgReader
{
public:
	CfgReader();

	float GetPlayerComeIn();
	float GetPlayerLeave();
	float GetDMove();
	float GetDispatchHandCardInterval();
	float GetDispatchHandCard(unsigned uRound);
	float GetPublicCardDelay();
	float GetActionBarUp();
	float GetFoldCard();
	float GetBetJetton();
	float GetCollectJetton();
	float GetDelayBeforeDispatchWinJetton();
	float GetDispatchWinJetton();
	float GetDelayBeforeShowWinNum();
	float GetShowWinNum();
	float GetDelayBeforeCleanTable();
private:
#ifdef WIN32
	WCHAR m_szBaseDir[1024];
	WCHAR m_szCfgFilePath[1024];
#endif
};

extern CfgReader g_cfgReader;

#endif