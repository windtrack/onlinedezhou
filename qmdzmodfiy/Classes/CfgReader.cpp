#include "CfgReader.h"

CfgReader g_cfgReader;

CfgReader::CfgReader()
{
	#ifdef WIN32
	unsigned uEleCnt = sizeof(m_szBaseDir) / sizeof(m_szBaseDir[0]);
	::GetModuleFileNameW(nullptr, m_szBaseDir, uEleCnt);
	(wcsrchr(m_szBaseDir, L'\\'))[0] = 0;
	wcsncat(m_szBaseDir, L"\\", uEleCnt);

	uEleCnt = sizeof(m_szCfgFilePath) / sizeof(m_szCfgFilePath[0]);
	m_szCfgFilePath[0] = 0;
	wcsncat(m_szCfgFilePath, m_szBaseDir, uEleCnt);
	wcsncat(m_szCfgFilePath, L"cfg.ini", uEleCnt);
	#endif
}

float CfgReader::GetPlayerComeIn()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"playercomein", 240, m_szCfgFilePath) / 1000.0f;
#else
	return 0.24;
#endif
}
float CfgReader::GetPlayerLeave()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"playerleave", 240, m_szCfgFilePath) / 1000.0f;
#else
	return 0.24;
#endif
}
float CfgReader::GetDMove()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"dmove", 600, m_szCfgFilePath) / 1000.0f;
#else
	return 0.6;
#endif
}
float CfgReader::GetDispatchHandCardInterval()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"dispatchhandcardinterval", 100, m_szCfgFilePath) / 1000.0f;
#else
	return 0.1;
#endif
}

float CfgReader::GetDispatchHandCard(unsigned uRound)
{
	float s_fDefault[2] = {0.28, 0.32};
#ifdef WIN32
	static WCHAR *s_szSeg[2] = {L"dispatchhandCard1", L"dispatchhandCard2"};
	return (float)::GetPrivateProfileIntW(L"time", s_szSeg[uRound], s_fDefault[uRound]*1000.0f, m_szCfgFilePath) / 1000.0f;
#else
	return s_fDefault[uRound];
#endif
}
float CfgReader::GetPublicCardDelay()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"publiccarddelay", 800, m_szCfgFilePath) / 1000.0f;
#else
	return 0.8;
#endif
}

float CfgReader::GetActionBarUp()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"actionbarup", 200, m_szCfgFilePath) / 1000.0f;
#else
	return 0.2;
#endif
}

float CfgReader::GetFoldCard()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"foldcard", 1280, m_szCfgFilePath) / 1000.0f;
#else
	return 1.28;
#endif
}
float CfgReader::GetBetJetton()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"betjetton", 320, m_szCfgFilePath) / 1000.0f;
#else
	return 0.32;
#endif
}
float CfgReader::GetCollectJetton()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"collectjetton", 300, m_szCfgFilePath) / 1000.0f;
#else
	return 0.3f;
#endif
}
float CfgReader::GetDelayBeforeDispatchWinJetton()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"delaybeforedispatchwinjetton", 480, m_szCfgFilePath) / 1000.0f;
#else
	return 0.48f;
#endif
}
float CfgReader::GetDispatchWinJetton()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"dispatchwinjetton", 580, m_szCfgFilePath) / 1000.0f;
#else
	return 0.58;
#endif
}
float CfgReader::GetDelayBeforeShowWinNum()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"delaybeforeshowwinnum", 0, m_szCfgFilePath) / 1000.0f;
#else
	return 0.0f;
#endif
}
float CfgReader::GetShowWinNum()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"showwinnum", 720, m_szCfgFilePath) / 1000.0f;
#else
	return 0.72;
#endif
}
float CfgReader::GetDelayBeforeCleanTable()
{
#ifdef WIN32
	return (float)::GetPrivateProfileIntW(L"time", L"delaybeforecleantable", 1320, m_szCfgFilePath) / 1000.0f;
#else
	return 1.32;
#endif
}

