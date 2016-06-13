#ifndef __GAMECOMMON_H__
#define __GAMECOMMON_H__

#include "jsonrpc/json/value.h"

#define GAME_ID  (6)
#define GAME_NAME ("德州扑克")

static int GAME_WIDTH =1136;
static int GAME_HEIGHT =640  ;

//#define GAME_WIDTH (1136)
//#define GAME_HEIGHT (640)

#define GAME_PLAYER (9)

enum ClientRequest : unsigned
{
	kSendTip = 1,
	kSendTableTip,

	kGetRegisterCode,
	kRegisit,
	kResetPassword,

	kAutoLogon,
	kUserLogon,
	kQQLogon,
	kQQLogon2,
	kGuestLogon,
	kLogonBy3rd, // 0-qq 1-weixin 2-weibo
    kContinueLogOn, // 连续登录

	kGetBindSafeCode,
	kBindPhone,

    kNewbieGuidLayer,   //新手引导
    kShowNewPlayerTask, //新手任务
	kDrawVipLoginAward, // 领取vip登陆奖励
	kDrawWaveGrilLoginAward,	// 领取德女郎登录奖励

	kGetHallInfo,


    kGetMagnateInfo,
	kGetVipAllOpenInfo,
	kStartPlayVideoInfo,
	kGetVipOpenRoomInfo,
	kSendVipOpenRoomInfo,
	kQueryUserInfo,
	kOperateOtherPlayer,
    kClickChatBox,
	kTouSu,
	kSendTool,
	kSendVipMessage,

	kInvitePlayer,
	kEnterTable,
	kEnterSngTable,
	kEnterMttTable,
	kEnterPrivateTable,

	kJoinTable,
	kLeaveTable,
	kChangeTable,
	kPopGameSet, // 弹出设置界面
	kStandUp, // 从桌子上站起

	kStandUpByMttMatch, // mtt比赛旁观桌子上站起
	kRequestSitDown, // 申请左下
	kRequestAutoSitDown, // 申请自动左下
	kEnterHall2,
	kLeaveHall2,

	kEnterGameSngHall,
	kLeaveGameSngHall,

	kEnterGameMttHall,
	kLeaveGameMttHall,


	kLeaveVipOpenRoom,
	
	kCaoDanDeSendGameMsg,

	kAddScore,
	kGiveUp,
	kShowBrightBrand,

	kReward,
	kSetAutoSupply,
	kDrawAward, // 领取任务奖励
	kUpdateMission,
    kJumpOverNewerGuide,
    kEscNewbieGuide,

	kGetFollowList, // 获取关注列表
	kGetRankList,	// 获取排行榜
	kGetSngRankList,	// 获取排行榜

	kFollowUser, // 关注玩家
	kPopUserDetailInfo, // 弹出并获取玩家信息
	kGetUserDetailInfo, // 大厅获取玩家信息
	kGetMyDetailInfo,
	kPopUserAchievementInfo, // 获取玩家成就信息
	kGetActivityInfo,	//  获取活动信息
	kGetMessageInfo,	// 获取系统消息
	kGetAchievements, // 获取成就详细信息

	kZhuXiao = 101,
	kSetGameSet,
	kPopMall,
	kShowHallBgLayer,
    kOptHallBgLayer,
	kOptSNGHallBgLayer,
	kOptMTTHallBgLayer,
	kEditMyInfo,

	kPublicChat,
	kPublicExpress,
	kChangeNickName,
	kChangeAccount,
	kChangePassWord,

	kGetGamePlayBackList, // 获取游戏回放列表
	kGetGamePlayBack, // 获取游戏回放数据
	kMobilePay, // 支付请求
	kExit = 201,

	kJoinGame,
	kCheckSitdown,

	kGameCheck,//心跳
	kCreatFamily,
	kJoinFamily,
	kLevelFamily,
	kPushOutFamily,
										
										
	kGetAllFamilyList,
	kTargetFamilyInfo,
	kShowTargetFamilyInfo,//

	kGetMyFamilyInfo,

	kEditFamilyIcon,
	kEditFamliyNotice,

	kGetFamilyRankRole,
	kGetFamilyNoSKRole,
	kGetAllFriends,
																																										
																							
	kTo_ViceShaikh,//设置副族长
	kMove_ViceShaikh,//取消副族长

	kGetChangeShaiKhCode,//获取转让俱乐部验证码

	kChangeFamilyShaikh,//转让俱乐部
	kGetFamiliyReward,//领取俱乐部奖励
	kInviteFreinds,//邀请好友
	kSearchFamily,//搜索俱乐部

	kDealApplyFamily,//是否w同意玩家的申请加入俱乐部
	kDealInvateJoinFamily,//是否同意邀请


	kCheckNewMessage,//检查是否有新消息

    kPopCustomerServices,

	kCallRoleUpdateFamilyInfo,
	CheckVipLv,
	kGetDailyLogin,
	kGetTableUserDetailInfo,//牌桌上玩家基础信息
	kCallPauseHeart ,//暂停心跳检测
	CallGameFinished ,		//退出游戏
	callGameCancleFriutUI, //关闭购买金币ui
	kCallUpdateCashAndVip, //更新金币和vip等级  购买成功后  和从后台返回后 请求

	kGetSngHallInfo,//获取sng大厅信息
	kCallSngTableMangzhu,//请求盲注数据
	kCallSngTableRankList,//请求牌桌排行

	kCallLevelTable,//离开桌子
	kCallLookUp,//确认旁观
	kCallPlayAgain,//再玩一场

	CallReturnToLogin,//返回登录界面

	kCallUploadPhoto,//上传照片
	kCallUserItemList,//获取我的道具
	kCallExpressPower,//获取是否使用魔法表情的权限

	kCallLevelTableWithOutLevel,//退出桌子

	kCallSngHallInfo,//sng大厅面板数据
	kCallMttHallInfo,//mtt大厅面板数据
	kCallGetContinuesSiginReward,//累计登录
	kCallGetSiginReward ,//每日签到
    kMttMatchState,
	kMttHallInfo,//mtt大厅
	kEnterMttMatch,//进入mTT比赛

	kMttMatchSiginUp,//mTT报名
	kMttMatchQuit,//mTT退赛
	kCallMttMatchRankList,//mtt排行列表

	kCallMttMatchRebuy , //mtt比赛重购
	kCallMttMatchAddon ,//mtt比赛增购
	kCallMttSingleMatchInfo,//mtt单个比赛消息
	kCallMttRebuyCannel,//取消增购
	kCallGetMttFloatMatchInfo,//mtt浮动赛奖励模板
	kCallRequestHasMatch,///请求当前是否有比赛正在进行
    kCallExchangeInfo,//获取兑换券信息
    kExchangeObject,//兑换实物
    kCallGuideTipsInfo,//获取功能引导信息
    kModifyGuideTipsInfo,//修改功能引导信息	
	kCallClientCancelAutoTrust,//牌局自动托管
	kRequsetIsAutoTrust,//是否自动托管
    kPopDiamondBuy,     //弹出购买钻石界面
    kBuyGoodsByDiamond, //通过钻石购买道具
    kPopActReward,      //弹出活动奖励界面
	kSerachPriavteRoom, //搜索私密房
	kRequsetPrivateRoomInfo,//请求私密房数据

	kCallPrivateGameController,//私密房牌局开始和关闭

	kCallCreatPrivateRoomName,//生成私密房间名称

	kCallPrivateRoomPushPlayer,//私密房踢人

	kCallPrivateFundData,//私密房基金

	kNoticeCreatClub,//创建俱乐部
	kNoticeSellingClub ,//转让俱乐部
	kNoticeQuiteClub ,//退出俱乐部
	kNoticeBeKickedClub ,//被提出俱乐部
	kNoticeJoinClub ,//加入俱乐部

	kCallSelfJoinClub ,//所有加入的俱乐部
	kCallAllotFunToRole,// 分配基金
	kCallUpdatePlayerCash,
    kCallHideHandCard,
    kBindSpread, //绑定推广人
    kGetMySpread, //获取我的推广列表
};

#include <functional>

void SendClientRequest(ClientRequest clientRequest, const Json::Value& param = Json::Value());
void SendGameTip(const std::string& strTip); // 整个游戏的提示
void SendTableTip(const std::string& strTip); // 桌子里面的提示

// 弹框
void PopTipWithBt2(const std::string& strContent, const std::string& strBt1, const std::string& strBt2, std::function<void(unsigned)> cbBt = nullptr);
void PopTipWithBt1(const std::string& strContent, const std::string& strBt, std::function<void(unsigned)> cbBt = nullptr);

// 注册返回按键回调函数
namespace cocos2d
{
	class Ref;
}
void RegistEscPressCb(cocos2d::Ref *target, std::function<void(cocos2d::Ref *pTarget)>);
void UnRegistEscPressCb(cocos2d::Ref *target);

enum NonUICmd
{
	NonUI_QQLoginResult,
	NonUI_WXLoginResult,

	NonUI_SinaLoginResult,
	NonUI_3rdLoginResult,
	NonUI_MobilePayResult,
    NonUI_AppStoreCheck,
	NonUI_NetworkChanged,
};
void SendNonUIMsg(const Json::Value& param);

void SendNetworkChangedMsg(int status);

void setUpdateLoadPro(int pro) ;

void setNewVersion(std::string version) ;

void setUpdateContent(std::string content) ;
#endif