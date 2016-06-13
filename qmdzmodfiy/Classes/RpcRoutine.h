#ifndef __RPC_ROUTINE_H__
#define __RPC_ROUTINE_H__

#include <functional>
#include "JsonValueAdapter.h"
#include "SyncDeque.h"
#include <thread>
#include <mutex>
#include <condition_variable>



template <typename T1>
void MakeParam(Json::Value& param, T1 t1)
{
	param[0u] = Json::Value(t1);
}

template <typename T1, typename T2>
void MakeParam(Json::Value& param, T1 t1, T2 t2)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
}

template <typename T1, typename T2, typename T3>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
}

template <typename T1, typename T2, typename T3, typename T4>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
	param[4] = Json::Value(t5);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
	param[4] = Json::Value(t5);
	param[5] = Json::Value(t6);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
	param[4] = Json::Value(t5);
	param[5] = Json::Value(t6);
	param[6] = Json::Value(t7);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
	param[4] = Json::Value(t5);
	param[5] = Json::Value(t6);
	param[6] = Json::Value(t7);
	param[7] = Json::Value(t8);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
	param[4] = Json::Value(t5);
	param[5] = Json::Value(t6);
	param[6] = Json::Value(t7);
	param[7] = Json::Value(t8);
	param[8] = Json::Value(t9);
}
template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
void MakeParam(Json::Value& param, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10)
{
	param[0u] = Json::Value(t1);
	param[1] = Json::Value(t2);
	param[2] = Json::Value(t3);
	param[3] = Json::Value(t4);
	param[4] = Json::Value(t5);
	param[5] = Json::Value(t6);
	param[6] = Json::Value(t7);
	param[7] = Json::Value(t8);
	param[8] = Json::Value(t9);
	param[9] = Json::Value(t10);
}
enum class RPCCmd
{
	UnKnown,
	GetRegisterCode,//注册重置密码获取验证码 m_get_registercode
	Regisit,//注册  m_reg
	ResetPassword,//重置密码  m_reset_pwd
	Logon,//登录  texas_m_user_login
	AutoLogon,//自动登录  texas_m_user_login
	GuestLogon,//游客登录  texas_reg_or_login
	QQLogon,//qq登录反馈  texas_m_qq_login
	WXLogon,//微信登录反馈 texas_wechat_login
	SinaLogon,//新浪登录反馈 oauth_login
	The3rdLogon,//第三方登录反馈  oauth_login
	GetBindSafeCode,///绑定手机获取验证码  get_vmobile_code
	BindPhone,//绑定手机  vmobile
	GetVIPPrice,//获取vip相关信息  texas_m_vip_rchglist
	GetNewShopPrice,//商城整合水果和道具   老版本 只获取水果  新版本将水果道具整合在一起  texas_m_new_gifts_conf
	GetNewTaskInfo,//获取所有任务  texas_new_tasklist
	UpdateTask,//更新任务  texas_updatetask
	DrawMissionAward,// 领取任务奖励 texas_get_taskreward
	GetFollowList,//大厅关注列表  texas_follow_list
	GetWeekWinPotRank, // 周战绩   	texas_feat_rank
	GetWeekWinCashRank, // 周盈利	texas_week_win_rank
	GetDailyWinCashRank, // 周日单局	texas_day_win_rank
	GetCashRank, // 财富榜	texas_cost_rank


	GetSngScoreRank, // sng大师分战绩	texas_master_rank
	GetMttScoreRank, // mtt大师分战绩  texas_master_mtt_rank
	GetMasterScoreRank,//大师分   	texas_master_zh_rank

	
	FollowUser, // 关注玩家  texas_deal_setting
	GetUserDetailInfo, // 大厅获取玩家信息  texas_hall_basic_info
	GetActivityInfo,	//  获取活动信息  texas_activity_infos
	GetMessageInfo,	// 获取系统消息   texas_sys_msg
	GetAchievements, // 获取成就详细信息  texas_get_achievements
	GetGamePlayBackList, // 获取回放数据列表  texas_get_gamelog_list
	GetGamePlayBack, // 获取回放数据  texas_get_onegamelog

	GetYinLianTradeID, // 获取银联交易单号 mupacp_get_trade_no
	GetAliPayTradeID, // 获取支付宝交易单号 m_get_order_no
	GetWeixinPayTradeID, // 获取微信交易单号 m_wechat_order_dz
	Get3rdTradeID,//获取第三方支付的交易订单号  m_get_order_no
    GetAppstoreTradeID,//获取第三方支付的交易订单号  m_get_order_no
	CreatFamily, //创建俱乐部  texas_create_group
	JoinFamily,//加入俱乐部  texas_join_group
	LevelFamily,//离开俱乐部  texas_group_info
	PushOutFamily,//踢出俱乐部  texas_group_rm
	EditFamilyInfo,//修改俱乐部信息  icon 公告  texas_group_update

	GetMyFamilyInfo,//我的俱乐部信息  texas_group_info
	GetTargetFamilyInfo,//选中俱乐部信息  texas_group_info
	ShowGetTargetFamilyInfo,//消息列表中显示头像上的俱乐部信息  texas_group_info
	GetFamilyList, //俱乐部列表  texas_group_list

	GetFamilyAllRole, //获取俱乐部所有成员   texas_group_members_rank
	GetFamilyNoSKRole,//获取没有权限的俱乐部人员列表  texas_group_members
	GetAllFriends,//获取所有可邀请的好友  texas_group_invite_list


	To_ViceShaikh,//设置副族长 texas_group_set_helper
	Move_ViceShaikh,//取消副族长  texas_group_set_helper
	GetChangeShaiKhCode,////获取转让俱乐部验证码  get_smscode
	ChangeFamilyShaikh,//转让俱乐部  texas_group_transfer

	GetFamiliyReward,//领取俱乐部奖励  texas_group_reward
	InviteFreinds,//邀请好友  texas_group_invite
	SearchFamily,//搜索俱乐部  texas_group_search
	DealApplyFamily, //处理俱乐部申请  texas_group_addmembers
	DealInvateJoinFamily,//处理俱乐部邀请 texas_group_invite_deal


	CheckNewMessage,//检查最新的 俱乐部消息时间  有问题  texas_group_last_check_msg_time
	CheckVipLv,//检测vip等级 转让俱乐部  5级vip会员   texas_vipinfo
	GetTicketLogin,//新版登录消息收取  texas_get_new_ticketlogin

	GetTableUserDetailInfo,//牌桌上玩家信息  texas_table_basic_info
    GetNewOpenIDKey, //获取新的OPEN ID和KEY 来上传照片   texas_m_auth
	GetUserItemBagPack,//获取我的道具  texas_m_get_backpack
	GetExpressPower,//能否使用魔法表情 texas_m_expressbag

	GetSngHallInfo,//sng面板数据   texas_sng_basic_info
	GetMTTHallInfo,//mtt面板数据   texas_mtt_basic_info
    CallExchangeInfo,//获取兑换券信息 texas_get_exchange_info
    ExchangeObject, //兑换实物  texas_add_exchange
    GetGuideTipsInfo,//功能引导信息 texas_get_guidance
    ModifyGuideTipsInfo,//修改功能引导信息  texas_do_guidance
    BuyGoodsByDiamond,//钻石购买 texas_diamond_exchange
    CheckAppStoreProduce,//appstorez支付验证
    PopActReward,     //弹出活动领奖界面texas_updataaction

	CheckVersion,//ios版本检测

	GetPrivateFundData,//私密房 基金texas_private_fund

	GetAllSelfJoinClub,//所有加入的俱乐部 texas_selfJoinClub 

	CallAllotFund,//分配基金 texas_save_club_fund_log
    BindSpread,
    GetMySpread,
};

struct RPC
{
	template <typename T1>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1);
	}
	template <typename T1, typename T2>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2);
	}
	template <typename T1, typename T2, typename T3>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3);
	}
	template <typename T1, typename T2, typename T3, typename T4>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4, t5);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4, t5, t6);
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4, t5, t6, t7);
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4, t5, t6, t7, t8);
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4, t5, t6, t7, t8, t9);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8,typename T9, typename T10>
	RPC(RPCCmd rpcCmd, const std::string& name, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9,T10 t10)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
	{
		MakeParam(m_rpcParam, t1, t2, t3, t4, t5, t6, t7, t8, t9,t10);
	}

	RPC()
		: m_rpcCmd(RPCCmd::UnKnown)
	{
	}

	RPC(RPCCmd rpcCmd, const std::string& name, const Json::Value& param)
		: m_rpcCmd(rpcCmd)
		, m_rpcName(name)
		, m_rpcParam(param)
	{
	}

	RPCCmd m_rpcCmd;
	std::string m_rpcName;
	Json::Value m_rpcParam;
	std::string m_rpcException;
	Json::Value m_rpcResult;
};

typedef std::function<void(const RPC&)> FuncRpcResultProc;

class RpcRoutine
{
public:
	RpcRoutine();
	~RpcRoutine();

	void SetRpcResultProc(FuncRpcResultProc funcRpcResultProc);

	void Start(const std::string& strRpcUrl, const std::string& strHttpUserAgent);
	void Stop();

	void AddRequest(const RPC& rpc);
	void ProcResult();

	void ThreadRoutine(const std::string& strRpcUrl, const std::string& strHttpUserAgent);

	void clear() ;
private:
	FuncRpcResultProc m_funcRpcResultProc;
	std::string m_strRpcUrl, m_strHttpUserAgent;
	SyncDeque<RPC> m_deqRequest;
	SyncDeque<RPC> m_deqResult;
	bool m_bContinue;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable m_conditionVar;
};

#endif