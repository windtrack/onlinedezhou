#include "GlobalUserData.h"
#include "cocos2d.h"
#include "JsonValueAdapter.h"

USING_NS_CC;

GlobalMyData::GlobalMyData()
{
	Reset();
}

std::string GlobalMyData::getFamilyName()
{
	return m_familyName;
}
unsigned GlobalMyData::getFamilyID()
{
	return m_familyId;
}
bool GlobalMyData::getHasCreatFamily()
{
	return m_JoinFamily;
}
unsigned GlobalMyData::getFamilyPower()
{
	return m_power;
}

void GlobalMyData::setFamilyName(std::string name)
{
	m_familyName = name ;
}

void GlobalMyData::setFamilyID(unsigned id)
{
	m_familyId= id ;
	setHasCreatFamliy(m_familyId!=0) ;
}
void GlobalMyData::setFamilyPower(unsigned power)
{
	m_power = power ;
}
void GlobalMyData::setHasCreatFamliy(bool flag)
{
	m_JoinFamily = flag ;
}

void GlobalMyData::setBindPhoneNumber(std::string code)
{
	m_bindPhoneCode = code ;
}
std::string GlobalMyData::getBindPhoneNumber()
{
	return m_bindPhoneCode ;
}

void GlobalMyData::setSngMasterScore(unsigned score)
{
	m_SngmastreScore = score ;
}
unsigned GlobalMyData::getSngMasterScore()
{
	return m_SngmastreScore ;
}

double GlobalMyData::GetCash()
{
	return m_dCash;
}

void GlobalMyData::SetCash(double dCash)
{
	if (fabs(dCash - m_dCash) >= 1.0f) 
	{
		m_dCash = dCash;
		for (auto cbItem : m_vecCBCashChanged)
		{
			if (cbItem)
			{
				cbItem(WAITCASHCHANGE);
			}
		}

		g_globalGameSerialization.AddAtomicAction(AtomicAction::UpdateCash);
	}
}

double GlobalMyData::GetDiamond()
{
    return m_dDiamond;
}
void GlobalMyData::SetDiamond(double dDiamond)
{
    /*if (fabs(dDiamond - m_dDiamond) >= 1.0f) 
    {*/
        m_dDiamond = dDiamond;
        for (auto cbItem : m_vecCBDiamondChanged)
        {
            if (cbItem)
            {
                cbItem(WAITDIAMONDCHANGE);
            }
        }

        g_globalGameSerialization.AddAtomicAction(AtomicAction::UpdateDiamond);
    /*}*/
}

bool GlobalMyData::IsBoy()
{
	return m_uGender == 1;
}

unsigned GlobalMyData::GetGender()
{
	return m_uGender;
}

void GlobalMyData::SetGender(unsigned uGender)
{
	if (m_uGender != uGender)
	{
		m_uGender = uGender;
		for (auto cbItem : m_vecCBGenderChanged)
		{
			if (cbItem)
			{
				cbItem(WAITGENDERCHANGE);
			}
		}
	}
}

unsigned GlobalMyData::GetSkinID()
{
	return m_uSkinId;
}

void GlobalMyData::SetSkinID(unsigned uSkin)
{
	if (m_uSkinId != uSkin)
	{
		m_uSkinId = uSkin;
		for (auto cbItem : m_vecCBSkinChanged)
		{
			if (cbItem)
			{
				cbItem(WAITSKINCHANGE);
			}
		}
	}
}


unsigned GlobalMyData::GetVipLevel()
{
	return m_uVipLevel;
}

void GlobalMyData::SetVipLevel(unsigned uVipLevel)
{
	if (m_uVipLevel != uVipLevel)
	{
		m_uVipLevel = uVipLevel;
		for (auto cbItem : m_vecCBVipChanged)
		{
			if (cbItem)
			{
				cbItem(WAITVIPCHANGE);
			}
		}
	}
}

void GlobalMyData::SetLadyLevel(unsigned uLadyLevel)
{
    m_uLadyLevel = uLadyLevel;
}

unsigned GlobalMyData::GetLadyLevel()
{
    return m_uLadyLevel;
}

void GlobalMyData::SetNickName(const std::string& strName)
{
	if (m_strNickName != strName)
	{
		m_strNickName = strName;
		for (auto cbItem : m_vecCBNickNameChanged)
		{
			if (cbItem)
			{
				cbItem(WAITNICKNAMECHANGE);
			}
		}
	}
}

void GlobalMyData::SetIconURL(const std::string& strName)
{
    m_strIconURL = strName;
    
    
    for (auto cbItem : m_vecCBSkinChanged)
    {
        if (cbItem)
        {
            cbItem(WAITSKINCHANGE);
        }
    }
}

const std::string GlobalMyData::GetNickName()
{
	return m_strNickName;
}

const std::string GlobalMyData::GetIconURL()
{
    return m_strIconURL;
}

void GlobalMyData::SetBindPhone(bool bBind)
{
	if (m_bHasBindPhone != bBind)
	{
		m_bHasBindPhone = bBind;
		for (auto cbItem : m_vecCBBindPhoneChanged)
		{
			if (cbItem)
			{
				cbItem(WAITBINDPHONECHANGE);
			}
		}
	}
}
bool GlobalMyData::IsBindPhone()
{
	return m_bHasBindPhone;
}



void GlobalMyData::WaitChange(unsigned uWaitChangeFlags, std::function<void(unsigned uChangeTypes)> cbChange)
{
	if (cbChange)
	{
		if (uWaitChangeFlags & WAITCASHCHANGE)
		{
			m_vecCBCashChanged.push_back(cbChange);
		}
		if (uWaitChangeFlags & WAITGENDERCHANGE)
		{
			m_vecCBGenderChanged.push_back(cbChange);
		}
		if (uWaitChangeFlags & WAITSKINCHANGE)
		{
			m_vecCBSkinChanged.push_back(cbChange);
		}
		if (uWaitChangeFlags & WAITVIPCHANGE)
		{
			m_vecCBVipChanged.push_back(cbChange);
		}
		if (uWaitChangeFlags & WAITBINDPHONECHANGE)
		{
			m_vecCBBindPhoneChanged.push_back(cbChange);
		}
		if (uWaitChangeFlags & WAITNICKNAMECHANGE)
		{
			m_vecCBNickNameChanged.push_back(cbChange);
		}	
        if (uWaitChangeFlags & WAITDIAMONDCHANGE)
        {
            m_vecCBDiamondChanged.push_back(cbChange);
        }
	}
}

void GlobalMyData::Reset()
{
	m_JoinFamily = false ;
	m_familyId=0;
	m_familyName="" ;
	m_power = 0 ;

	m_iUserId = 0;
	m_strNickName.clear();
	m_uSkinListCount = 0;
	m_FamilySkinListCount = 0 ;
	SetSkinID(0);
	//m_uSkinId = 0;
	//SetCash(0);
	// m_dCash = -1;			// 总乐币
	SetVipLevel(0); 
	// m_uVipLevel = -1; // VIP等级 0-6 一共6级，0代表当前不是vip
	m_strVipName.clear(); // 如果vip等级不为0，显示的vip级别的尊称
	m_uVipValidDay = 0; // 如果vip等级不为0，剩余有效的天数 
	m_dVipAward = 0; // vip登陆领取奖励
	m_bNeedShowVipAward = false; // 是否需要在进入大厅后显示登陆奖励领取界面 

	SetGender(1);
	// m_uGender = 1; // 性别  2代表女 1代表男
	m_strQQ.clear();
	m_strEmail.clear();
	m_bHasBindPhone = false; // 是否已经绑定手机
    //清理兑换商城的收货信息
	m_deqExpress.clear();
}

void GlobalMyData::AppandExpress(unsigned uType, unsigned uIdx)
{
	unsigned uIdxAll = uType * 16 + uIdx;
	for (auto iter = m_deqExpress.begin(); iter != m_deqExpress.end(); ++iter)
	{
		if (*iter == uIdxAll)
		{
			m_deqExpress.erase(iter);
			break;
		}
	}
	m_deqExpress.push_front(uIdxAll);
}

const UserSkin& GlobalMyData::GetSkin(unsigned uSkinID)
{
	for (unsigned i = 0; i < m_uSkinListCount; ++i)
	{
		UserSkin& skin = m_userSkinList.at(i) ;
		if (skin.m_uID == uSkinID)
		{
			return skin;
		}
	}
	return m_userDefaultSkin;

}

void GlobalTableConfig::SetTableMode(TableMode tableMode)
{
	m_tableMode = tableMode;
}

TableMode GlobalTableConfig::GetTableMode()
{
	return m_tableMode;
}

void GlobalTableConfig::SetMyChairID(unsigned uChairID)
{
	if (m_uChairID != uChairID)
	{
		m_uChairID = uChairID;
	}		
}
unsigned GlobalTableConfig::GetMyChairID()
{
	return m_uChairID;
}
unsigned GlobalTableConfig::GetMyViewChairID()
{
	return 4;
}
unsigned GlobalTableConfig::GetViewChairID(unsigned uChairID)
{



	unsigned uView = 8 - (((uChairID + 9) - m_uChairID + 4) % 9);

	if (IsTableHasVideo())
	{
		if (uView > GetMyViewChairID())
		{
			unsigned uOff = uView - GetMyViewChairID();
			static unsigned s_TargetV[] = {4, 5, 8, 0, 3};
			uView = s_TargetV[uOff];
		}
		else if (uView < GetMyViewChairID())
		{
			unsigned uOff = GetMyViewChairID() - uView;
			static unsigned s_TargetV[] = {4, 3, 0, 8, 5};
			uView = s_TargetV[uOff];
		}
	}




	return uView;
}
unsigned GlobalTableConfig::GetChairID(unsigned uViewChairID)
{


	if (IsTableHasVideo())
	{
		if (uViewChairID == 0)
		{
			uViewChairID = GetMyViewChairID() - 2;
		}
		else if (uViewChairID == 8)
		{
			uViewChairID = GetMyViewChairID() + 2;
		}

		//int id[9] ={4,3,2,1,0,8,7,6,5} ; 
		//int id[9] ={6,7,8,0,1,2,3,4,5} ; 
	/*		int id[9] ={5,6,7,8,0,1,2,3,4} ; 
		uViewChairID = id[uViewChairID] ;*/

	}
	return (m_uChairID + 9 - (uViewChairID + 9 - GetMyViewChairID()) % 9) % 9;
}

void GlobalTableConfig::AddPlayer(unsigned uChairID, unsigned uID, double dCash, 
								  const std::string& strNickName, const std::string& strIconUrl, 
								 unsigned uVoice, unsigned uSkin, unsigned uGender)
{
	PlayerInfo player;
	player.m_uChairID = uChairID;
	player.m_uID = uID;
	player.m_dCash = dCash;
	player.m_strNickName = strNickName;
	player.m_strIconUrl = strIconUrl;
	player.m_uSkin = uSkin;
	player.m_uVoice = uVoice;
	player.m_uGender = uGender;
	auto pUser = GetPlayerByID(uID);
	if (pUser)
	{
		*pUser = player;
	}
	else
	{
		m_vecPlayerInfo.push_back(player);
	}
}

unsigned GlobalTableConfig::GetRandChairID()
{
    if (m_vecPlayerInfo.size() == 0)
    {
        return 0;
    }
    else
    {
        unsigned vecSize = m_vecPlayerInfo.size();
        return m_vecPlayerInfo[std::rand()%vecSize].m_uChairID;
    }
}

PlayerInfo *GlobalTableConfig::GetPlayerByID(unsigned uID)
{
	for (auto iter = m_vecPlayerInfo.begin(); iter !=  m_vecPlayerInfo.end(); ++iter)
	{
		if (iter->m_uID == uID)
		{
			return &(*iter);
		}
	}
	return nullptr;
}
PlayerInfo *GlobalTableConfig::GetPlayerByChair(unsigned uChairID)
{
	for (auto iter = m_vecPlayerInfo.begin(); iter !=  m_vecPlayerInfo.end(); ++iter)
	{
		if (iter->m_uChairID == uChairID)
		{
			return &(*iter);
		}
	}
	return nullptr;
}
	
PlayerInfo *GlobalTableConfig::GetPlayerByViewChair(unsigned uViewChairID)
{
	unsigned uChair = GetChairID(uViewChairID);
	return GetPlayerByChair(uChair);
}
void GlobalTableConfig::RemovePlayerByID(unsigned uID)
{
	for (auto iter = m_vecPlayerInfo.begin(); iter !=  m_vecPlayerInfo.end(); ++iter)
	{
		if (iter->m_uID == uID)
		{
			m_vecPlayerInfo.erase(iter);
			break;
		}
	}
}

void GlobalTableConfig::RemovePlayerByChair(unsigned uChairID)
{
	for (auto iter = m_vecPlayerInfo.begin(); iter !=  m_vecPlayerInfo.end(); ++iter)
	{
		if (iter->m_uChairID == uChairID)
		{
			m_vecPlayerInfo.erase(iter);
			break;
		}
	}
}

void GlobalTableConfig::ClearPlayer()
{
	m_vecPlayerInfo.clear();
}

bool GlobalTableConfig::IsTableHasVideo()
{
	return m_uMaxPlayer == 5;
}

void GlobalTableConfig::Clear()
{
	m_uAreaID = 0;
	m_uRoomID = 0;
	m_uChairID = 0;
	m_uTableID = -1;
	m_uMaxPlayer = 0;
	m_uPotLimitTime = 0;
	m_dLittleBlind = 0;
	m_dBigBlind = 0;
	m_dRewardMoney = 0;
	m_uCardRangeLow = 0;
	m_uCardRangeHigh = 0;
	m_dMagicExpressPrice = 0;
	m_dAutoBringMoney = 0;
	m_dMinBringMoney = 0;
	m_dMaxBringMoney = 0;
	m_uAutoSupplyToMaxMoney = 0;
	m_dAutoSupplyMoney = 0;
	m_tableMode = TablePlaying;
	m_enterMoney = 0;
}

void LoadConfig(GlobalConfig& cfg)
{
	FileUtils *pFileUtils = FileUtils::getInstance();
	std::string strWritableDir = pFileUtils->getWritablePath();
	log("LoadConfig:%s", strWritableDir.c_str());
	std::string strCfgPath = strWritableDir + std::string("CattleRush");
	log("cfg path:%s", strCfgPath.c_str());
	if (pFileUtils->isFileExist(strCfgPath))
	{
		log("cfg path exist:%s", strCfgPath.c_str());
		FILE *file = fopen(strCfgPath.c_str(), "rb");
		if (file)
		{
			log("cfg path opened:%s", strCfgPath.c_str());
			fseek(file, 0L, SEEK_END);
			unsigned uSize = ftell(file);
			if (uSize == sizeof(GlobalConfig))
			{
				fseek(file, 0L, SEEK_SET);
				fread(&cfg, 1, sizeof(GlobalConfig), file);
			}
			fclose(file);
		}
	}
}

void FlushConfig(GlobalConfig& cfg)
{
	FileUtils *pFileUtils = FileUtils::getInstance();
	std::string strWritableDir = pFileUtils->getWritablePath();
	std::string strCfgPath = strWritableDir + std::string("CattleRush");
	FILE *file = fopen(strCfgPath.c_str(), "wb");
	if (file)
	{
		fwrite(&cfg, 1, sizeof(GlobalConfig), file);
		fclose(file);
	}
}

GlobalConfig::GlobalConfig()
	: m_iLastLogonSuccess(false)
	, m_iBgSoundValue(50)
	, m_iEffectSoundValue(50)
	, m_iReserved0(0)
	, m_bJinZhiPangGuan(false)
	, m_bJinZhiTongZhuo(false)
	, m_bJuJueYaoQing(false)
	, m_dMaxWinPerDay(0)
	, m_uFreezenMinWhenWin(0)
	, m_dMaxLosePerDay(0)
	, m_uFreezenMinWhenLose(0)
	, m_MaxOnlineHour(0)
	, m_uFreezenMinWhenTimeFull(0)
{
	memset(m_szLastLogonAccount, 0, sizeof(m_szLastLogonAccount));
	memset(m_szLastLogonPassword, 0, sizeof(m_szLastLogonPassword));	
}
GlobalConfig::~GlobalConfig()
{
	FlushConfig(*this);
}

void GlobalConfig::SetLastLogonInfo(const std::string& strAccount, const std::string& strPwd,  bool bRemPwd, bool bAutoLogon, bool bSuccess)
{
	memcpy(m_szLastLogonAccount, strAccount.c_str(), strAccount.size()+1);
	memcpy(m_szLastLogonPassword, strPwd.c_str(), strPwd.size()+1);
	m_iLastLogonSuccess = (bSuccess ? 1 : 0);
	m_bRemPwd = bRemPwd;
	m_bAutoLogon = bAutoLogon;
	FlushConfig(*this);
}

void GlobalConfig::SetSoundMusicValue(double dBgSoundValue)
{
	m_iBgSoundValue = dBgSoundValue * 100.0f;
	FlushConfig(*this);
}

void GlobalConfig::SetSoundEffectValue(double dEffectSoundValue)
{
	m_iEffectSoundValue = dEffectSoundValue * 100.0f;
	FlushConfig(*this);
}

void GlobalConfig::SetLastLogonResult(bool bSuccess, const std::string& strAutoLoginUser, const std::string& strAutoLoginPwd)
{
	memcpy(m_szAutoLoginName, strAutoLoginUser.c_str(), strAutoLoginUser.size()+1);
	memcpy(m_szAutoLoginPwd, strAutoLoginPwd.c_str(), strAutoLoginPwd.size()+1);
	m_iLastLogonSuccess = (bSuccess ? 1 : 0);
	FlushConfig(*this);
}

void GlobalConfig::SetSoundValue(double dBgSoundValue, double dEffectSoundValue)
{
	m_iBgSoundValue = dBgSoundValue * 100.0f;
	m_iEffectSoundValue = dEffectSoundValue * 100.0f;
	FlushConfig(*this);
}

void GlobalConfig::GetLastLogonInfo(std::string& strAccount, std::string& strPwd, bool& bRemPwd, bool& bAutoLogon, bool& bSuccess)
{
	if (m_iReserved0 == 0)
	{
		LoadConfig(*this);
		m_iReserved0 = 1;
	}
	strAccount = m_szLastLogonAccount;
	strPwd = m_szLastLogonPassword;
	bSuccess = m_iLastLogonSuccess == 1;
	bRemPwd = m_bRemPwd;
	bAutoLogon = m_bAutoLogon;
}

void GlobalConfig::GetLastLogonResult(std::string& strAutoAccount, std::string& strAutoPwd, bool& bAutoLogon, bool& bSuccess)
{
	if (m_iReserved0 == 0)
	{
		LoadConfig(*this);
		m_iReserved0 = 1;
	}
	strAutoAccount = m_szAutoLoginName;
	strAutoPwd = m_szAutoLoginPwd;
	bSuccess = m_iLastLogonSuccess == 1;
	bAutoLogon = m_bAutoLogon;
}

void GlobalConfig::GetSoundValue(double& dBgSoundValue, double& dEffectSoundValue)
{
	if (m_iReserved0 == 0)
	{
		LoadConfig(*this);
		m_iReserved0 = 1;
	}
	dBgSoundValue = m_iBgSoundValue / 100.0f;
	dEffectSoundValue = m_iEffectSoundValue / 100.0f;
}

static char *s_szMissionTags[] =
{
	"texas_guide", // 新手引导
	"texas_call", // 跟注
	"texas_fold", // 弃牌
	"texas_raise", // 加注	
	"texas_button", // 庄家位置玩牌
	"texas_card_intro", // 点开牌型介绍
	"texas_follow", // 关注玩家
	"texas_chat", // 聊天
	"texas_vchat", // 语音聊天
	"texas_magic", // 发送魔法表情
	"onaddcounter", // 打开补充筹码界面
	"invite", // 邀请好友

	"texascmp", // 指定场对局 800 2000 10000
	"texas_cg", // 常规场
	"texas_bsqj", // 白手起家
	"texas_zc", // 中产阶级
	"texas_dh", // 德州大亨
	"texas_sngcn", // SNG比赛菜鸟场

	"texas_cgwin", // 常规场胜利

	"texas_assets", // 资产达到 指定值 加奖励
};

GameSerialization::GameSerialization()
	: m_bGamePlaying(false)
{}

void GameSerialization::AddAtomicAction(AtomicAction atomicAction)
{
	switch (atomicAction)
	{
	case AtomicAction::UpdateCash:
		{
			for (int i = 0; i < g_globalMissionInfo.GetMissions(2).size(); i++)
			{
				auto& mission = g_globalMissionInfo.GetMissions(2)[i];
				// auto& tag = mission.missionTag;
				if (mission.uMissonStatus == 0 && mission.strMissionTagName == "texas_assets")
				{
					// double dTagValue = JsonValueAdapter(tag["texas_assets"]);
					if (g_globalMyData.GetCash() >= mission.uMissionTagValue)
					{
						Json::Value param;
						param["task_id"] = mission.uMissionID;
						SendClientRequest(ClientRequest::kUpdateMission, param);
						break;
					}
				}
			}
		}
		break;

    case AtomicAction::UpdateDiamond:
        {

        }break;

	case AtomicAction::GameBegin:
		if (!m_bGamePlaying)
		{
			m_bGamePlaying = true;
			OnGameStart();
		}
		break;

	case AtomicAction::GameEnd:
		if (m_bGamePlaying)
		{
			m_bGamePlaying = false;
			OnGameEnd();
		}
		break;

	case AtomicAction::WinGame:
		{
			OnWinGame();
		}
		break;
    case AtomicAction::TaskGuide:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_guide" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task800:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_800" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskCall:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_call" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskFold:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_fold" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskRaise:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_raise" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskButton:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_button" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskCardIntro:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_introduction" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskFllow:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_follow" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskChat:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_chat" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskVChat:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_vchat" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task2000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_2000" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskMagic:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_magic" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskAddJetton:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                if (g_globalMissionInfo.GetMissions(1)[0].strMissionTagName == "onaddcounter" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task4000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                auto& mission = g_globalMissionInfo.GetMissions(1)[0];
                if (mission.strMissionTagName == "texascmp" && mission.uMissionTagValue == 4000 && mission.uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task6000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                auto& mission = g_globalMissionInfo.GetMissions(1)[0];
                if (mission.strMissionTagName == "texascmp" && mission.uMissionTagValue == 6000 && mission.uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task10000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                auto& mission = g_globalMissionInfo.GetMissions(1)[0];
                if (mission.strMissionTagName == "texascmp" && mission.uMissionTagValue == 10000 && mission.uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task20000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                auto& mission = g_globalMissionInfo.GetMissions(1)[0];
                if (mission.strMissionTagName == "texascmp" && mission.uMissionTagValue == 20000 && mission.uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::Task30000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                auto& mission = g_globalMissionInfo.GetMissions(1)[0];
                if (mission.strMissionTagName == "texaszcdj" && mission.uMissionTagValue == 6 && mission.uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;
    case AtomicAction::TaskV30000:
        {
            if (g_globalMissionInfo.GetMissions(1).size() > 0)
            {
                auto& mission = g_globalMissionInfo.GetMissions(1)[0];
                if (mission.strMissionTagName == "texaszcdj" && mission.uMissionTagValue == 5 && mission.uMissonStatus == 0)
                {
                    Json::Value param;
                    param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
                    SendClientRequest(ClientRequest::kUpdateMission, param);
                }
            }
        }
        break;

	default:
		break;
	}
}

void GameSerialization::OnWinGame()
{
	bool bInChangGuiChang = false;
	if (g_globalTableConfig.m_uAreaID >= 0 && g_globalTableConfig.m_uAreaID <4) // 白手起家
	{
		bInChangGuiChang = true;
	}
	if (bInChangGuiChang)
	{
		// 常规游戏场对局胜利x次
		for (int i = 0; i < g_globalMissionInfo.GetMissions(2).size(); i++)
		{
			auto& mission = g_globalMissionInfo.GetMissions(2)[i];
			if (mission.uMissonStatus == 0 && mission.uMissionID >= 2027 && mission.uMissionID <= 2035)
			{
				Json::Value param;
				param["task_id"] = mission.uMissionID;
				SendClientRequest(ClientRequest::kUpdateMission, param);
			}
		}
        for (int i = 0; i < g_globalMissionInfo.GetMissions(3).size(); i++)
        {
            auto& mission = g_globalMissionInfo.GetMissions(3)[i];
            if (mission.uMissonStatus == 0 && mission.strMissionTagName == "texas_cgcweek")
            {
                Json::Value param;
                param["task_id"] = mission.uMissionID;
                SendClientRequest(ClientRequest::kUpdateMission, param);
            }
        }
	}
}

void GameSerialization::OnGameStart()
{

}

//游戏结束后更新任务
void GameSerialization::OnGameEnd()
{
    
    bool bIsMtt = g_globalTableConfig.m_bIsMTTScene;
    //MTT比赛更新
    if (bIsMtt)
    {
        //放在4015协议中请求更新

        //for (auto itemInfo : g_globalMissionInfo.GetMissions(0))//在每日任务中遍历更新MTT任务
        //{
        //    if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID == 2065 || itemInfo.uMissionID == 2065))
        //    {
        //        Json::Value param;
        //        param["task_id"] = itemInfo.uMissionID;
        //        SendClientRequest(ClientRequest::kUpdateMission, param);
        //    }
        //}
        //for (auto itemInfo : g_globalMissionInfo.GetMissions(3))//在每周任务中遍历更新MTT任务
        //{
        //    if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID == 2070 || itemInfo.uMissionID == 2071))
        //    {
        //        Json::Value param;
        //        param["task_id"] = itemInfo.uMissionID;
        //        SendClientRequest(ClientRequest::kUpdateMission, param);
        //    }
        //}
        ////MTT牌局结束后更新德州之王
        //for (auto itemInfo : g_globalMissionInfo.GetMissions(2))
        //{
        //    if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID >= 2081 && itemInfo.uMissionID <= 2089))
        //    {
        //        Json::Value param;
        //        param["task_id"] = itemInfo.uMissionID;
        //        SendClientRequest(ClientRequest::kUpdateMission, param);
        //    }
        //}
    }
    //m_uAreaID   0初出茅庐 1小有成就 2小有成就 3大显身手 4贵族开房
    else if (!bIsMtt && (g_globalTableConfig.m_uAreaID >= 0) && (g_globalTableConfig.m_uAreaID <= 3))
    {
        // 初出茅庐，小有成就，大显身手
        static char *s_szTagName[] ={
            "texas_ccml",
            "texas_xycj",
            "texas_dxss",
        };
        unsigned uAreaID = g_globalTableConfig.m_uAreaID;
        if (uAreaID == 1 || uAreaID == 2)
            uAreaID = 1;
        else if (uAreaID == 3)
            uAreaID = 2;
        for (int i = 0; i < g_globalMissionInfo.GetMissions(0).size(); i++)
        {
            auto& mission = g_globalMissionInfo.GetMissions(0)[i];
            if (mission.uMissonStatus == 0 && mission.strMissionTagName == s_szTagName[uAreaID])
            {
                Json::Value param;
                param["task_id"] = mission.uMissionID;
                SendClientRequest(ClientRequest::kUpdateMission, param);
            }
        }
        //更新富可敌国、勤劳致富
        for (auto itemInfo : g_globalMissionInfo.GetMissions(2))
        {
            if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID >= 2009 && itemInfo.uMissionID <= 2026))
            {
                Json::Value param;
                param["task_id"] = itemInfo.uMissionID;
                SendClientRequest(ClientRequest::kUpdateMission, param);
            }
        }
    }
    //m_uAreaID 100:SNG菜鸟场 101:SNG普通场 102:SNG精英场 103:SNG卓越场 104:SNG大师场 105:SNG宗师场
    else if (!bIsMtt && (g_globalTableConfig.m_uAreaID >= 100) && (g_globalTableConfig.m_uAreaID <= 105))
    {
        //放在3013协议中请求更新

        //unsigned uAreaID = g_globalTableConfig.m_uAreaID + 1959;
        //for (auto itemInfo : g_globalMissionInfo.GetMissions(0))//在每日任务中遍历更新SNG任务
        //{
        //    if (itemInfo.uMissonStatus == 0 && itemInfo.uMissionID == uAreaID)
        //    {
        //        Json::Value param;
        //        param["task_id"] = itemInfo.uMissionID;
        //        SendClientRequest(ClientRequest::kUpdateMission, param);
        //    }
        //}
        //for (auto itemInfo : g_globalMissionInfo.GetMissions(3))//在每周任务中遍历更新SNG任务
        //{
        //    if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID == 2068 || itemInfo.uMissionID == 2069))
        //    {
        //        Json::Value param;
        //        param["task_id"] = itemInfo.uMissionID;
        //        SendClientRequest(ClientRequest::kUpdateMission, param);
        //    }
        //}
        ////更新竞技之神
        //for (auto itemInfo : g_globalMissionInfo.GetMissions(2))
        //{
        //    if (itemInfo.uMissonStatus == 0 && (itemInfo.uMissionID >= 2072 && itemInfo.uMissionID <= 2080))
        //    {
        //        Json::Value param;
        //        param["task_id"] = itemInfo.uMissionID;
        //        SendClientRequest(ClientRequest::kUpdateMission, param);
        //    }
        //}
    }

    //m_uAreaID   0初出茅庐 1小有成就 2小有成就 3大显身手 4贵族开房
	//bool bInChangGuiChang = (g_globalTableConfig.m_uAreaID >= 0) && (g_globalTableConfig.m_uAreaID <= 4);
 //   unsigned uAreaID = g_globalTableConfig.m_uAreaID;
 //   if (uAreaID == 1 || uAreaID == 2)
 //       uAreaID = 1;
 //   else if (uAreaID == 3)
 //       uAreaID = 2;
	//if (!bInChangGuiChang)
	//{

	//}
	//else
	//{
	//	// 初出茅庐，小有成就，大显身手
	//	static char *s_szTagName[] ={
	//		"texas_ccml",
	//		"texas_xycj",
	//		"texas_dxss",
	//	};
	//	for (int i = 0; i < g_globalMissionInfo.GetMissions(0).size(); i++)
	//	{
	//		auto& mission = g_globalMissionInfo.GetMissions(0)[i];
	//		// auto& tag = mission.missionTag;
	//		if (mission.uMissonStatus == 0 && mission.strMissionTagName == s_szTagName[uAreaID])
	//		{
	//			Json::Value param;
	//			param["task_id"] = mission.uMissionID;
	//			SendClientRequest(ClientRequest::kUpdateMission, param);
	//		}
	//	}

	//	// 小有成就对局（6、9人桌）
	//	//if (g_globalTableConfig.m_uAreaID == 1 || g_globalTableConfig.m_uAreaID == 2) // 小有成就
	//	//{
	//	//	for (int i = 0; i < g_globalMissionInfo.GetMissions(0).size(); i++)
	//	//	{
	//	//		auto& mission = g_globalMissionInfo.GetMissions(0)[i];
	//	//		// auto& tag = mission.missionTag;
	//	//		if (mission.uMissonStatus == 0 && mission.strMissionTagName == "texaszcdj")
	//	//		{
	//	//			if ((g_globalTableConfig.m_uAreaID == 1 && mission.uMissionTagValue == 6) ||
	//	//				(g_globalTableConfig.m_uAreaID == 2 && mission.uMissionTagValue == 9))
	//	//			{
	//	//				Json::Value param;
	//	//				param["task_id"] = mission.uMissionID;
	//	//				SendClientRequest(ClientRequest::kUpdateMission, param);
	//	//			}
	//	//		}
	//	//	}
	//	//}

		// 成就更新，仅更新对局xx次，胜利多少次不更新
		//for (int i = 0; i < g_globalMissionInfo.GetMissions(2).size(); i++)
		//{
		//	auto& mission = g_globalMissionInfo.GetMissions(2)[i];
		//	// auto& tag = mission.missionTag;
		//	if (mission.uMissonStatus == 0 && mission.strMissionTagName == "texas_cg")
		//	{
		//		Json::Value param;
		//		param["task_id"] = mission.uMissionID;
		//		SendClientRequest(ClientRequest::kUpdateMission, param);
		//	}
		//}

}

bool GameSerialization::IsInGameNow()
{
	return m_bGamePlaying;
}






void GlobalMyFamilyInfo::WaitChange(std::function<void()> cbChange)
{
	if (cbChange)
	{
		m_vecCBSkinChanged.push_back(cbChange);	
	}
}


void GlobalMyFamilyInfo::setFamilyIconID(unsigned uSkin)
{
	if (m_familyIconID != uSkin)
	{
		m_familyIconID = uSkin;
		for (auto cbItem : m_vecCBSkinChanged)
		{
			if (cbItem)
			{
				cbItem();
			}
		}
	}
}

void GlobalMyFamilyInfo::setFamilyIconUrl(std::string url)
{
	if (m_famliyIconUrl != url)
	{
		m_famliyIconUrl = url;

		for (int i=0; i< g_globalMyData.m_FamilySkinListCount;i++)
		{
			if (m_famliyIconUrl == g_globalMyData.m_familySkinList[i].m_strIconUrl)
			{
				setFamilyIconID(i+1) ;
			}

		}
	}
}


void GlobalMyFamilyInfo::cleanMyFamliyInfo()
{
	m_famliyIconUrl = "" ;
	m_familyIconID = -1 ;
	m_familyName = "" ;

}
//void GlobalMyFamilyInfo::setFamilySkinID(unsigned uSkin)
//{
//	//m_familySkinId = id ;
//
//	if (m_familySkinId != uSkin)
//	{
//		m_familySkinId = uSkin;
//		for (auto cbItem : m_vecCBSkinChanged)
//		{
//			if (cbItem)
//			{
//				cbItem();
//			}
//		}
//	}
//
//
//}


const FamilySkin& GlobalMyData::GetFamilySkin(unsigned uSkinID)
{
	for (unsigned i = 0; i < m_FamilySkinListCount; ++i)
	{
		if (m_familySkinList[i].m_fuid == uSkinID)
		{
			return m_familySkinList[i];
		}
	}
	return m_familyDefaultSkin;
}


//unsigned GlobalMyFamilyInfo::getFamilySkinID()
//{
//	return m_familySkinId ;
//}



GlobalTempData g_globalTempData;
GlobalMyData g_globalMyData;
GlobalSession g_globalSession;
GlobalServerInfo g_globalServerInfo;
GlobalConfig g_globalConfig;
GlobalMyAddr g_globalMyAddr;
GlobalTableConfig g_globalTableConfig;
GlobalMissionInfo g_globalMissionInfo;
GlobalTableAreaInfo g_globalTableAreaInfo;
GlobalFollowUserInfo g_globalFollowUserInfo;
GlobalRankUserInfo g_globalRankUserInfo;
GlobalQuickFollowInfo g_globalQuickFollowInfo;
GlobalActivityInfo g_globalActivityInfo;
GlobalActRewardInfo g_globalActRewardInfo;
GlobalMessageInfo g_globalMessageInfo;
GlobalAchievementInfo g_globalAchievementInfo;
GlobalMallInfo g_globalMallInfo;
GlobalVipOpenRoom g_globalVipOpenRoom;
GlobalGameRecordInfo g_globalGameRecord;
GameSerialization g_globalGameSerialization;
GlobalContinueLogin g_globalContinueLogin;

GlobalFamilyInfo g_globalFamilyInfo;
GlobalMyFamilyInfo g_globalMyFamilyInfo;
//GlobalTargetFamilyInfo g_globalTargetFamilyInfo;
GlobalFamilySKin g_globalFamilySKin;

GlobaRatio g_globalRatio;

GlobalSngHallInfo g_globalSngHallInfo ;
GlobalSngGameInfo g_globalSngTableInfo ;
GlobalSngRankInfo g_globalSngRankInfo ;

GlobalUserItemList g_globalUserItemList ;
GlobalUserSigin g_globalUserSigin ;
GlobalUpdateInfo g_globaUpdateInfo ;

GlobalMttHallInfo g_globalMttHallInfo ;
GlobalMttGameInfo g_globaMttGameInfo ;

UpGradeUserAccount g_globalUpgradeAccount;

GlobaMttChangedTicket m_MttChangedTicket;
MttFloatReward m_mttFloatMatchRewardInfo ;

GlobalAllFundData m_globalAllFundData ;

GlobalFunData m_globalFundData ;
GlobalPrivateRoomInfo m_globaPrivateRoomInfo ;

GlobalSelfJoinClub g_globaSelfJoinClub ;
GlobalFriendsSpread g_globalFriendsSpread;