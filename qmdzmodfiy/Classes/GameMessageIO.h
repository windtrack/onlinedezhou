#ifndef __GAMEMESSAGEIO_H__
#define __GAMEMESSAGEIO_H__

#include "jsonrpc/json/value.h"
#include <functional>
#include <vector>
#include <utility>
#include "GameSocket.h"


enum GameMsgCmdS : unsigned
{
	LogonResult = 204,		// 登陆反馈  103
	GameScence = 205,		// 游戏场景消息？  801  802 3004
	GameRewardPool = 609,	// 游戏奖池  610
	GameScenceRestore = 1022, // 场景恢复消息
	GameSetInfo = 606,     // 游戏设置信息
	PlayerComeIn = 331,
	PlayerLeave = 332,
	UpdateWaitPlayerNum = 1013, // 等待队列人数
	UpdateMoneyFaile = 526,		// 更新钱币失败
	UpdateMoney = 510,  // 更新乐币和带入现金
	AutoSupplyMoneyTip = 555, // 自动带入提示
	GameStart = 501,
	BetResult = 513, // 下注结果
	FoldCardResult = 514, // 弃牌
	BrightBrandResult = 523,	  // 亮牌
	GameEnd = 507,
	RewardResult = 511,

	GetHallInfoResult = 1005,//1004
    GetMagnateInfoResult = 1021,//1020
	GetVipOpenRoomResult = 603,//804
	GetDefineVipOpenResult = 607,//805
    ContinueLoginResult = 608,//806
	GetContinueTicketLogin = 808,//新门票签到回调809
	QueryUserInfoResult = 1003,//1002
	OperateOtherPlayerResult = 1001,//1000
	FollowPlayerResult = 1024,//1009
	SendToolResult = 515,//1006

	GetFollowUserInfoResult = 1008,//1007
	UpdateFollowUserInfo = 1009, //  大厅关注用户信息刷新

	DrawVipLoginAwardResut = 1015, // vip领取奖励结果 sendResult 0：失败  1：成功  beans_after 领取奖励之后的德州币  1014
	DrawWaveGrilLoginAwardResult = 612, // 德女郎领奖反馈  //807
	UserInviteRequest = 1017,//1016
	EditMyInfoResult = 1019,//1018
	KickedByLogonTwice = 1026, // 被重复登陆提示
	ServerStopNotify = 1027, //  通知客户端，停服公告  
	ServerMessage = 1028, //  后台消息通知

	EnterTableFailed = 2000,
	SitDownFailed = 2001,


	NoticeRoleUpdateFamilyInfo = 2003,//2002
	RechargeUpdateMoney = 2025,	// 充值后服务器通知更新钱币  1025
	//UpdateFamilyData = 2002,
	BroadcastTips = 610,

	GetSngHallInfo = 3003,//3002
	GetSngMatchRankList = 3007,//3006
	GetSngTableMangzhu = 3009,//3008
	GetShowSngRankView = 3010,//玩家胜利或失败

	GetSngPlayerFailedRankInTable = 3011,//玩家失败，桌面显示的名次

	GetSngMyRank = 3012,//更新我的排行

	GetSngGameOver = 3013,//通知sng结束


	GetServerTableTips = 3014,//服务器发来的消息

	GetServerTableScorllTips = 3015,//服务器发来的滚动消息

	GetMasterScore = 3016,//大师分
	CheckOnline = 3018,//30017

	GetRobotRoomMessage = 3019,//机器人互动  机器人发送的消息

	GetContinueSiginReward = 9015 ,//领取累计登录奖励 9014

	GetSiginReward = 9013,//领取签到奖励 9012

	GetMttHallInfo = 4002,//获取mtt大厅消息 4001

	GetUpdateMttGameState = 4003,//获取mtt大厅消息 更改当前状态

	GetUpdateMttGameSiginPlayerNum = 4004,//获取mtt 赛事单个列表人数

	GetUpdateMttMatchCountDownNotify = 4012,//获取mtt比赛即将开始的提示

	GetUpdateMttMatchDissmissNotify = 4013,//获取mtt比赛解散的消息

	

	GetEnterMttGame = 4021,//进入mtt比赛 4020

	GetUpdateMttMatchRank = 4023,////牌桌上显示mtt排行xx/xx 

	GetMttMatchSiginUp = 4011,//mtt报名结果 4011

	GetMttMatchQuit = 4039,//mtt退赛结果 4038

	GetMttMatchRankList = 4033,//获取mtt排行列表 4032

	GetMttRankByGameOver = 4015,//mtt 个人钱没有后的排名提示


	GetMttRebuyResult = 4017,//重购回调 4016

	GetMttAddonResult = 4019,//加购回调 4018

	GetMttSingleMatchInfo = 4027,//4026 特殊情况下 获取本厂赛况消息

	GetMttRebuyTipsByServer = 4045,//mtt重购

	GetMttGameOver = 4046,//通知mtt结束结束

	GetMttUpdateTicket = 4041,//更新门票数量

	GetMttMatchDissolve = 4047,//mtt比赛解散消息

	GetMttMatchTableDissolve = 4040,//mtt当前桌子合桌 旁观人收到解散消息

	GetUpdateMttRankAndAcore = 4048,//更新大师分和排行

	GetUpdateMttButtonShow = 4049,//更新mtt比赛时重购加购的显示

	GetMttFloatMatchInfo = 4051,//4050 mtt浮动赛奖励模板

	GetMttFloatMatchPoolAndRoleCount = 4052,//更新浮动赛奖池与玩家数量

	GetHasMatchNow = 5002,//请求当前是否有比赛正在进行 5001

	GetRefshRebuyAndAddBuyCount = 4054,//请求 4053

	GetServerTipsToSinglePlayer = 9016,//服务器的单条给玩家的提示

	GetSetServerAutoTrust = 4056,//进入托管  服务器主动发送

	GetServerIsAutoTrust = 4058,//是否托管中个

	GetAutoJoinMttMatch = 4059,//mtt自动入场

	GetPrivateRoomPasswordReslut = 6016,//房间是否存在 6015

	GetCurPrivateRoomInfo = 6018,//6017

	GetPrivateRoomControll = 6020,//私密房开关 6019

	GetPrivateRoomName = 6022,//获取生成的房间名 6021

	GetJoinToPrivateRoomReslut = 6024 ,//进入私密房回调 6023


	GetPrivateRoomPushPlayer = 6026,//房主踢人消息回调

	GetPrivateRoomGameOver = 6028,//私密房解散

	GetPrivateRoomLeavelRoomResult = 6030,//收到被踢消息

	GetUpdatePlayerCash = 6042,//通知玩家刷新金币6041
};

enum GameMsgCmdC : unsigned
{
	Logon = 103, //204
	SetGameSet = 803,
	EnterGame = 801, //205
	JoinGame = 802,//205
	InvitePlayer = 1016,//1017
	Ready = 101,
	Bet = 713,
	FoldCard = 714,//514
	BrightBrand = 716,	  // 亮牌  523
	RewardDelaer = 711,	//511
	SetAutoSupply = 703,
	LeaveTable = 704,
	StandUp = 1010,
	RequestSitDown = 1011,
	RequestAutoSitDown = 1012,

	GetHallInfo = 1004,//1005
	QueryUserInfo = 1002,//1003
	OperateOtherPlayer = 1000,//1001
	TouSu = 1001, // TODO: 类型为5 
	SendTool = 1006, //515
	GetFollowUserInfo = 1007, //获取大厅关注用户信息  1008
	DrawVipLoginAward = 1014, // vip领取登陆奖励  1015
	DrawWaveGrilLoginAward = 807,	// 领取德女郎特殊奖励  612
	EditMyInfo = 1018,//1019
    GetMagnateInfo = 1020,//1021
	GetVipOpenRoomInfo = 804,//603
	SendVipOpenRoomInfo = 805,//607
    ContinueLogin = 806,//608

	Check_In_Game =999,

	NoticeRoleAboutFamilyInfo= 2002,//通知玩家更新俱乐部 2003

	CallPauseHeart= 3005,//

	CallUpdateCashAndVip = 1025,//更新德州币和vip  2025
	QueryCheckOnline = 3017,//3018
	CallSngHallInfo = 3002,//更新德州币和vip = 1025,//更新德州币和vip  3003
	EnterSngGame = 3004,//205


	CallSngMatchRankList =  3006 ,//请求sng牌桌玩家排行//3007
	CallSngTableMangzhu = 3008,//请求盲注数据 //3009
	
	CallContinueTicketLogin = 809,//新门票签到808

	CallGetContinueSiginReward = 9014 ,//领取累计登录奖励 9015

	CallGetSiginReward = 9012,//领取签到奖励 9013




	CallEnterMttHall=4001,//进入mtt大厅  4002

	CallEnterMttSiginUp=4010,//报名mtt  4011

	CallEnterMttMatch=4020,//进入mtt比赛  4021

	CallMttMatchQuit = 4038,//mtt退赛 4039

	CallMttMatchRankList = 4032,///获取mtt排行列表 4033

	CallMttMatchRebuy = 4016, //mtt比赛重购 4017
	CallMttMatchAddon = 4018,//mtt比赛增购 4019

	CallMttSingleMatchInfo = 4026,//4027 特殊情况下 请求本厂赛况

	CallMttRebuyCannel = 4044,//取消重购

	CallMttFloatMatchInfo = 4050,//mtt浮动赛奖励模板 4051

	CallRequestHasMatch = 5001,//请求当前是否有比赛正在进行 5002

	CallRefshRebuyAndAddBuyCount = 4053,//请求4054

	CallCancelAutoTrust = 4055 ,//取消托管  客户端发送

	CallIsAutoTrust = 4057 ,//询问是否托管

	CallSendPrivateRoomPassword = 6015,//通过口令请求房间6016

	CallCUrPrivateRoomInfo = 6017,//获取当前进入的私密房数据

	CallPrivateRoomControll = 6019,//私密房牌局开关 6020  isopen

	CallCrearPrivateRoomName = 6021,//生成私密房间名称 6022

	CallEnterRrivateRoom = 6023,//进入私密房

	CallPrivateRoomPushPlayer = 6025,//房主踢人

	CallNoticeCreatClub = 6031,//创建俱乐部
	CallNoticeSellingClub = 6033,//转让俱乐部
	CallNoticeQuiteClub = 6035,//退出俱乐部
	CallNoticeBeKickedClub = 6037,//被提出俱乐部
	CallNoticeJoinClub = 6039,//加入俱乐部

	CallUpdatePlayerCash = 6041,//通知玩家刷新金币6042
};

#pragma pack (1)
struct GameMsgHead
{
	GameMsgHead(GameMsgCmdC cmdC, unsigned uBodySize);
	unsigned m_uPktSize; // 12 + m_uBodySize
	unsigned m_uCmd;
	unsigned m_uBodySize;

};
#pragma pack ()


typedef std::function<void()> GameIOAbordProc;
typedef std::function<void(const GameMsgHead&, const char*, unsigned)> GameMsgProc;


class GameMessageIO
{
public:
	GameMessageIO();
	~GameMessageIO();

	bool StartIO(const std::string& strIp, unsigned uPort);
	void ProcIO();
	void StopIO();

	void SetGameIOAbordProc(GameIOAbordProc gameIOAbordProc);
	void SetGameMsgProc(GameMsgProc gameMsgProc);

	void SendGameMessage(GameMsgCmdC cmdC);
	void SendGameMessage(GameMsgCmdC cmdC, const Json::Value& body);

	bool check();
	void clear() ;

	void Flush();
protected:
	void ProcInGameMsg(const GameMsgHead& msgHead, const char*pBody, unsigned uBodySize);
private:	
	CGameSocket *m_gameSocket;
	GameIOAbordProc m_gameIOAbordProc;
	GameMsgProc m_gameMsgProc;
};

#endif