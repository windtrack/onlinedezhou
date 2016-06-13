#ifndef __GLOBALUSERDATA_H__
#define __GLOBALUSERDATA_H__

#include "GameCommon.h"
#include <string.h>
#include <assert.h>
#include <string>
#include "jsonrpc/json/value.h"
#include <vector>
#include <deque>

struct GlobalTempData
{
	std::string			m_strLastRegisitPhone;
	std::string			m_strLastRegisitPwd;
};

struct UserSkin
{
	unsigned m_uID;
	std::string			m_strIconUrl;	// 头像
	std::string			m_strVoiceDir;	// 声音
	std::string			m_strAvatorUrl; // 形象
	std::string			m_strAvatorBgUrl;
	bool				m_islocked; //是否购买
};

//元旦   new_year       //春节   spring_festival
//情人节 valentine_day  //端午节 dragon_boat
//七夕节 star_festival  //中秋节 mid_autumn
//国庆节 national_day   //圣诞节 Christmass
struct ActivityState
{
    std::string m_activityName;
    std::string m_strURL;//活动大厅背景或其他资源
    unsigned    m_uIndex;//活动索引
    bool m_state;
};

struct DataRatio
{
	int					m_personRatio;
	int					m_fatherRatio;
	int					maxRole ;
};

struct GlobaRatio
{
	DataRatio*			m_dataRatio[10] ;
};


struct FamilySkin
{
	unsigned			m_fuid;
	std::string			m_strIconUrl;	// 俱乐部头像
};

struct GlobalFamilySKin
{
	std::vector<FamilySkin> m_vecAllFamiluSkin; // 0-我关注的 1-德女郎 2-邀请我的
	unsigned familySkinSize ;
};


#define WAITCASHCHANGE 1
#define WAITGENDERCHANGE 2
#define WAITSKINCHANGE 4
#define WAITVIPCHANGE 8
#define WAITNICKNAMECHANGE 16
#define WAITBINDPHONECHANGE 32
#define WAITDIAMONDCHANGE 64

#define BUFFER_SIZE 128

struct GlobalMyData
{
	GlobalMyData();
	double GetCash();
	void SetCash(double dCash);
    double GetDiamond();
    void SetDiamond(double dDiamond);
	bool IsBoy();
	void SetGender(unsigned uGender);
	unsigned GetGender();
	unsigned GetSkinID();
	void SetSkinID(unsigned uSkin);

	inline unsigned GetVoiceID(){ return m_uVoiceId;} ;
	void SetVoiceID(unsigned id){m_uVoiceId = id ;};

	unsigned GetVipLevel();
	void SetVipLevel(unsigned uVipLevel);
	void SetNickName(const std::string& strName);
	const std::string GetNickName();
    void SetIconURL(const std::string& strName);
    const std::string GetIconURL();
	void SetBindPhone(bool bBind);
	bool IsBindPhone();
    unsigned GetLadyLevel();
    void SetLadyLevel(unsigned uLadyLevel);

	void WaitChange(unsigned uWaitChangeFlags, std::function<void(unsigned uChangeType)> cbChange);

	void setFamilyName(std::string name) ;
	std::string getFamilyName() ;

	void setFamilyID(unsigned id) ;
	unsigned getFamilyID() ;

	void setHasCreatFamliy(bool flag) ;
	bool getHasCreatFamily() ;

	void setFamilyPower(unsigned power) ;
	unsigned getFamilyPower() ;

	void setBindPhoneNumber(std::string code) ;
	std::string getBindPhoneNumber() ;

	//sng大师分

	void setSngMasterScore(unsigned score) ;
	unsigned getSngMasterScore() ;
	inline void setSngMasterRank(unsigned rank){m_SngmastreRank = rank ;} ;
	inline unsigned getSngMasterRank(){return m_SngmastreRank ;} ;


	inline void setMttMasterScore(unsigned score) {m_MttmastreScore = score ;};
	inline unsigned getMttMasterScore() {return m_MttmastreScore;};
	inline void setMttMasterRank(unsigned rank){m_MttmastreRank = rank ;} ;
	inline unsigned getMttMasterRank(){return m_MttmastreRank ;} ;


	inline void setUseExpress(bool flag) { m_useExpress = flag ; } ;
	inline void setuseMagicExpress(bool flag) { m_useMagicExpress = flag ; } ;


	void Reset();
	void AppandExpress(unsigned uType, unsigned uIdx);
	const UserSkin& GetSkin(unsigned uSkinID);
	const FamilySkin& GetFamilySkin(unsigned uSkinID);
private:
	double			m_dCash;			// 总乐币
    double          m_dDiamond;         // 总钻石数量
	unsigned		m_uGender; // 性别 0代表未知 1代表男 2代表女
	std::string		m_strNickName;	
    std::string		m_strIconURL;
	unsigned		m_uSkinId;
	unsigned		m_uVoiceId;
	unsigned		m_uVipLevel; // VIP等级 0-6 一共6级，0代表当前不是vip
	bool			m_bHasBindPhone; // 是否已经绑定手机
	std::string		m_bindPhoneCode ;//绑定的号码
	std::vector<std::function<void(unsigned)>> m_vecCBCashChanged;
	std::vector<std::function<void(unsigned)>> m_vecCBGenderChanged;
	std::vector<std::function<void(unsigned)>> m_vecCBSkinChanged;
	std::vector<std::function<void(unsigned)>> m_vecCBVipChanged;
	std::vector<std::function<void(unsigned)>> m_vecCBNickNameChanged;
	std::vector<std::function<void(unsigned)>> m_vecCBBindPhoneChanged;
    std::vector<std::function<void(unsigned)>> m_vecCBDiamondChanged;
public:
	int m_iUserId;
	UserSkin m_userDefaultSkin;
	std::vector<UserSkin>  m_userSkinList;

	unsigned m_uSkinListCount;
	
	std::string		m_strVipName; // 如果vip等级不为0，显示的vip级别的尊称
	unsigned		m_uVipValidDay; // 如果vip等级不为0，剩余有效的天数 
	double			m_dVipAward; // vip登陆领取奖励
	double			m_WaveGrilAward;	// 德女郎登录奖励
	bool			m_bNeedShowVipAward; // 是否需要在进入大厅后显示登陆奖励领取界面 
    unsigned		m_uLadyLevel;//1-5，五个等级，0代表非德女郎
    unsigned		m_continueLoginType;//0:领取德州币， 1：领取门票
	
	std::string		m_strQQ;
	std::string		m_strEmail;
	std::deque<unsigned> m_deqExpress;

	std::string		 m_strVideoStreamUrl; // 我自己的视频流地址


	unsigned		 m_familyId;
	unsigned		 m_applyFamilyId ;//已经申请的id

	
	std::string		 m_familyName;
	bool			 m_JoinFamily ;
	unsigned		 m_power;

	FamilySkin		 m_familyDefaultSkin;
	FamilySkin		 m_familySkinList[32];
	unsigned		 m_FamilySkinListCount;


	unsigned		m_SngmastreScore;//大师分
	unsigned		m_SngmastreRank;//大师分排名

	unsigned		m_MttmastreScore;//大师分
	unsigned		m_MttmastreRank;//大师分排名
	//道具
	bool			 m_useExpress ;//是否可以使用表情
	bool			 m_useMagicExpress ;//是否可以使用魔法表情
    std::vector<ActivityState>  m_vecActivityState;
};

/************************************************************************/
/* 我的基金                                                                    */
/************************************************************************/

struct SingleFundData
{
	std::string  m_date ;//日期
	std::string  m_roomName ;//房间名
	unsigned	 m_cash;//基金
	bool		 m_isPersonalRoom;//个人房
	unsigned	 m_index ;
};

struct GlobalFunData
{
	unsigned m_allFund ;//总基金
	unsigned m_personalRoomFund ; //个人房基金
	unsigned m_clubFund ;//俱乐部基金

	std::vector<SingleFundData> m_vecOneFundData; 

};

struct GlobalAllFundData
{
	GlobalFunData m_vecAllFundData[2][4]; 
};

/************************************************************************/
/* 私密房信息                                                               */
/************************************************************************/
struct GlobalPrivateRoomInfo
{
	unsigned		m_Type;//0 个人 1俱乐部
	unsigned		m_playerNum;
	std::string		m_ownerName;
	std::string		m_roomName;
	std::string		m_password;
	unsigned		m_minBlind;
	unsigned		m_maxBlind;
	unsigned		m_ante;
	unsigned		m_minCarry;
	unsigned		m_maxCarry;
	double			m_curCollectFund;
	unsigned		m_fundP;
	unsigned		m_minCollect;		
	unsigned		m_maxCollect ;

	unsigned		m_roomid ;
	unsigned		m_isGameState ; //0等待  1开始 2结束
	bool			m_hasGetData ;
	bool			m_isOwner ;//是否是房主
	void clean(){
		m_isOwner = false ;
		m_isGameState = 0 ;
		m_hasGetData = false ;
	} ;
};




/************************************************************************/
/* 更新提示                                                                     */
/************************************************************************/
struct LineUpdateInfo
{
	std::string m_info ;
};

struct GlobalUpdateInfo
{
	std::vector<LineUpdateInfo> m_vecAllUpdateInfo; 
};

/************************************************************************/
/* 签到开始                                                                     */
/************************************************************************/


struct SiginDay
{
	unsigned		m_state;//丢失  已领取  当前领取  未领取  0，1，2，3
	std::string		m_itemName;//道具名称
	unsigned		m_count;//道具数量
	std::string		m_iconUrl;//道具图片地址
};

struct ContinuousSingin 
{
	bool			m_hasItem; //是否有领取物品  无物品  已领取  当前领取  未领取  0，1，2，3
	std::string		m_itemName;//道具名称
	unsigned		m_count;//道具数量
	std::string		m_iconUrl;//道具图片地址
};

struct GlobalUserSigin
{
	std::vector<SiginDay> m_vecUserSigin; 
	std::vector<ContinuousSingin> m_vecUserContinuousSingin; 
	unsigned m_continusdays ;//连续登录天数
	unsigned m_today ;//星期几
	bool m_hasGetContinuonsReward ;//是否领取 累计登录
	bool m_hasGetSiginReward ;//是否领取 签到奖励
	bool m_hasShowSiginLayer ;//是否显示过该界面
};

struct AccountBindReward
{
    std::string     m_strIconURL;         //奖励URL
    std::string     m_strDes;             //奖励说明
};

struct UpGradeUserAccount
{
    bool            m_bBindAccount;       //是否绑定账号
    std::string     m_strChannelName;     //绑定的渠道名
    std::vector<AccountBindReward> m_vecBindReward;//绑定奖励
};

/************************************************************************/
/* 我的道具开始                                                                     */
/************************************************************************/
enum UserItemDealType
{
	UItemDealType_siginup =0 ,//报名
	UItemDealType_exchange,//兑换1
	UItemDealType_NoUse,//未使用2
	UItemDealType_HasUse,//已使用3
	UItemDealType_ToUse,//使用4
	UItemDealType_Mttsiginup,//mtt报名5
};
//模拟数据
enum UserItemType
{
	UItemType_Ticket_cainiao =0 ,
	UItemType_Ticket_putong ,
	UItemType_Ticket_jingyin  ,
	UItemType_Ticket_zhuoyue  ,
	UItemType_Ticket_dashi  ,
	UItemType_Ticket_zongshi  ,
	UItemType_Guizu_expression,//贵族表情
	UItemType_Guizu_MagicExpression,//贵族魔法表情
	UItemType_3DMoviesTicket,//电影票
};

struct UserItemInfo
{
	std::string		m_iconUrl ; //图片地址
	std::string	    m_name ;    //图片名字
	std::string		m_expDate ; //有效期
	std::string		m_des;		//描述
	unsigned		m_count;	//数量  0 不显示数量标志  
	std::string		m_btdes;	//按钮描述
	unsigned		m_state ;	//按钮操作类型  //报名 0   兑换1 未使用2  已生效3 使用4
	unsigned		m_type ;	//物品类型  菜鸟0 普通1 精英2 卓越3 大师4 宗师5 贵族表情6 贵族魔法表情7 电影票 8
};

struct GlobalUserItemList
{
	std::vector<UserItemInfo> m_vecUserItemList; 
};

/************************************************************************/
/* 我的道具结束                                                                    */
/************************************************************************/


/************************************************************************/
/* sng                                                                    */
/************************************************************************/
struct GlobalSngTableInfo
{
	std::string		m_tableName ;//桌子名称
	unsigned	    m_tableID ;//桌子id
	unsigned		m_ImgID ;//图片id
	unsigned		m_maxPlayRoleCount ;//最大人数
	unsigned		m_roleCountInGame ;//当前人数
	unsigned		m_rewardCount ;//前几名有奖励
	unsigned		m_reward[3] ;//奖励金额
	unsigned		m_score[3] ;//大师分
	unsigned		m_entryFee ;//报名费
	unsigned		m_fee ;//服务费
	unsigned		m_freeTicket ;//比赛需要的门票
	unsigned		m_myTicket;//当前拥有的门票

	unsigned		m_index;
};

struct GlobalSngHallInfo
{
	unsigned		m_rank ;//当前排名
	unsigned		m_masterRank ;//大师排名
	std::vector<GlobalSngTableInfo> m_sngTableInfo;
	GlobalSngTableInfo m_curJoinTable; //当前加入的牌桌
};

//排行
struct SngRankInfo
{
	unsigned		m_uID;
	std::string		m_strNickName;
	std::string		m_strIconUrl;
	double			m_dWinValue; 
	unsigned		m_uRank;
};


//sng所有排行
struct GlobalSngRankInfo
{

	std::vector<SngRankInfo>& GetRankList(unsigned uRankType, unsigned uWinType)
	{
		return m_vecSngRankList[uWinType*2+uRankType];
	}
	void GetPageInfo(unsigned uRankType, unsigned uWinType,
		unsigned& uCurPage, unsigned& uMaxPage)
	{
		uCurPage = m_uPageInfo[uWinType*2+uRankType][0];
		uMaxPage = m_uPageInfo[uWinType*2+uRankType][1];
	}

	std::vector<SngRankInfo> m_vecSngRankList[6]; 
	unsigned m_uPageInfo[6][2];

};




//sngtable
//牌桌玩家信息
struct SngRoleInfo
{
	unsigned		m_rankid ;
	std::string		m_icon;
	std::string		m_name;
	unsigned		m_cash ;
	unsigned		m_charid ;
	unsigned		m_uid ;
};
//奖励
struct SngReward
{
	unsigned		m_rankid;
	unsigned		m_cashReward;
	unsigned		m_score ;
};
//升盲
struct SngShengmang
{
	unsigned		m_level ;
	std::string		m_mangzhu;
	unsigned		m_qiangzhu;
	unsigned		m_time ;
};
//牌桌信息
struct GlobalSngGameInfo
{
	std::string		m_tableName ;
	unsigned		m_playerCount ;//玩家人数
	unsigned		m_roudIdex ;//游戏回合数
	unsigned		m_mangzhuIndex ;//升盲
	unsigned		m_tableId ;//房间id
	bool			m_isSnglose ;
	unsigned		m_rank ;//自己的排名
	unsigned		m_rewardCount ;//前几名有奖励
	bool			m_isGameStart;//游戏开始
	bool			m_isGameOver;//游戏结束
	bool			m_signupformGame;//再战一场进去

	inline void clear(){
		m_roudIdex = 0 ;
		m_mangzhuIndex =0 ;
		m_isSnglose = false ;
		m_rank = 1 ;
		m_isGameStart = false ;
		m_isGameOver = false ;
		m_signupformGame = false ;
	} ;

	std::vector<SngRoleInfo> m_sngRoleInfo;
	std::vector<SngReward> m_sngReward;
	std::vector<SngShengmang> m_sngShengmang;	
};

/************************************************************************/
/* sng结束                                                                 */
/************************************************************************/






/************************************************************************/
/* mtt 开始                                                                    */
/************************************************************************/

//概述
struct MttTableDes
{
	std::string		m_siginDes ;//报名费文本
	std::string		m_matchStartTime;//比赛时间
	unsigned		m_minRole;//最小比赛人数
	unsigned		m_maxRole;//最大比赛人数

	unsigned		m_firstMoney;//初始筹码

	unsigned		m_rebuyCount;//重购次数
	unsigned		m_rebuyMin; //初始场次
	unsigned		m_rebuyMax;	//最大场次
	unsigned		m_rebuyMoney;//使用的德州币
	unsigned		m_rebuyToCarch;//兑换的筹码

	unsigned		m_addOnCount;//增购次数
	unsigned		m_addOnLevelMin;//可增购的场次
	unsigned		m_addOnLevelMax;//可增购的场次
	unsigned		m_addOnMoney;//使用的德州币
	unsigned		m_addOnToCarch;//增购的筹码

};
//排行
struct MttRankInTable
{
	unsigned		m_rankid ;
	std::string		m_icon;
	std::string		m_name;
	unsigned		m_cash ;
	unsigned		m_charid ;
	unsigned		m_uid ;
};

//盲注
struct MttBlind
{
	unsigned		m_level ;
	std::string		m_blind;//盲注
	unsigned		m_preBlind;//前注
	unsigned		m_time ;//升盲时间

	bool			m_isRebuy;//是否重购
	bool			m_isAddon;//是否增购
};



//奖励
struct MttReward
{
	unsigned		m_rankid;
	std::string		m_rewardInfo ; //道具说明
	unsigned		m_money ;
	unsigned		m_score ;

	bool		    m_bShowItem;//是否显示物品
	bool		    m_bShowGold;//是否显示德州币 
	bool		    m_bShowScore;//是否显示大师分 

	//浮动赛增加的配置
	std::string		m_roleIntervals ; //人数区间 排名
	unsigned m_rolePre;
	unsigned m_roleEnd;
};


struct SingleFloatReward
{
	unsigned m_rolePre;
	unsigned m_roleEnd;
	std::vector<MttReward> m_MttReward;//奖励
};

/************************************************************************/
/* 浮动赛奖励                                                                     */
/************************************************************************/
struct MttFloatReward
{
	std::vector<SingleFloatReward> m_fLoatReward;//人数和奖励关系  浮动赛才有
	unsigned		m_maxReward ;//总奖池
	unsigned		m_realRegistCount;//真正的报名人数 浮动赛中使用

};



struct GameMttTableInfo
{
	unsigned		m_GameType;//类型
	std::string		m_iconurl;//图片地址
	std::string		m_tableName ;//桌子名称
	unsigned	    m_tableID ;//桌子id
	unsigned		m_roleCountInGame ;//当前人数

	bool			m_isRebuy;//重购
	bool			m_isAddOn;//增购

	unsigned		m_TimeType ;
	std::string		m_titleDay ;//  日期比赛日期描述   今天 日期等
	std::string		m_TimeDes ;//延迟 即将
	std::string		m_strTime ;//具体时间       未开放的时间描述
	unsigned		m_matchTimeBeforeStart;//未开放时 显示 赛前多少小时开放
	//int				m_lastTime;//倒计时时间  即将开始的和延迟开始的到技术
	
	unsigned		m_entryFee ;//报名费
	unsigned		m_fee ;//服务费
	unsigned		m_freeTicket ;//比赛需要的门票
	unsigned		m_myTicket;//当前拥有的门票

	unsigned		m_SiginState;////0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散 7延迟报名

	int				m_realTime;// 倒计时时间  即将开始的和延迟开始的到技术 报名时间 实际时间用来排序
	bool			m_isdelayRegist; //延迟报名

	bool			m_canUseTicket;//是否可用门票
	bool			m_canUseMoney;//是否可用德州币报名

	bool			m_isFreeMathch; //是否免费
	bool			m_showCountDown;//是否显示倒计时 

	bool			m_isFloatMatch ; //是否是浮动赛

	
	//本场赛况
	MttTableDes		m_mttTableDes ;//本场赛况
	std::vector<MttRankInTable> m_MttRank;//排行
	std::vector<MttBlind> m_MttBilnd; //升盲
	std::vector<MttReward> m_MttReward;//奖励
	//std::vector<MttFudongReward> m_MttFudongReward;//浮动赛奖励
    MttFloatReward m_MttFudongReward;

	unsigned m_Index ;//当前索引 0-8
	unsigned m_IndexInPage ;//显示的编号索引 0-135
};

struct MttTypeName
{
	int type;
	std::string name ;
};
struct GlobalMttHallInfo
{
	unsigned		m_rank ;//当前排名
	unsigned		m_masterRank ;//大师排名

	std::vector<int> m_mttTableType; //牌桌类型
	std::vector<MttTypeName> m_mttTypeName;//比赛类型对应的名字

	std::vector<GameMttTableInfo> m_mttTableInfo; //所有比赛



	unsigned		m_maxTableCount;//最大牌桌数量
	unsigned		m_curTableCount;//当前显示的牌桌数量
	unsigned		m_allTypeCount ;
};

struct TableAndTicket
{
	int id;
	int num;
};
struct GlobaMttChangedTicket
{
	std::vector<TableAndTicket> m_allTicket;

};


//牌桌信息
struct GlobalMttGameInfo
{
	std::string		m_tableName ;
	unsigned		m_playerCount ;//玩家人数
	unsigned		m_roudIdex ;//游戏回合数
	unsigned		m_mangzhuIndex ;//升盲
	unsigned		m_tableId ;//房间id
	bool			m_isMttlose ;
	unsigned		m_rank ;//自己的排名
	unsigned		m_rewardCount ;//前几名有奖励
	bool			m_isGameStart;//游戏开始
	bool			m_isGameOver;//游戏结束
	bool			m_signupformGame;//再战一场进去

	unsigned		m_state ;//1等待中 0其他
	
	int				m_rebuyCount ;
	int				m_addonCount ;

	bool			m_getRecord;

	bool			m_inWaittingRoom ;

	bool			m_hasJoinMatch ;

	bool			m_hasDismassTable;//解散桌子


	bool			m_showMttbutton ;//是否显示加购增购  服务器控制关闭
	inline void setCurMttMatchInfo(GameMttTableInfo &tableinfo){m_curMttTable = tableinfo;} ;
	//设置真实比赛信息

	inline void clear(){
		m_signupformGame = false ;
		m_roudIdex=0 ;
		m_mangzhuIndex=0 ;
		m_rank=1 ;
		m_isGameStart = false ;
		m_isMttlose=false ;
		m_state = 0;
		m_inWaittingRoom = false ;
		m_hasJoinMatch = false ;
		m_hasDismassTable = false ;
		m_showMttbutton = true ;
	} ;

	GameMttTableInfo m_curMttTable; //当前加入mtt的牌桌
};

/************************************************************************/
/* MTT结束                                                                     */
/************************************************************************/







//管理员头像
struct FamilyRoleBaseInfo
{
	unsigned		m_uid;
	std::string		m_iconUrl;
	std::string		m_uname;
	unsigned		m_power;//权限
	unsigned		m_contribution ;//贡献值
	unsigned		m_rank ;//贡献排行
	unsigned		m_viplv ;
	unsigned		m_hasCash ;//分配的基金
	bool			m_isOwnner;//是否是俱乐部所有者
};

struct GlobalMyFamilyInfo
{
public:

	std::vector<FamilyRoleBaseInfo> m_AllAdmin;   //存放管理员数据
	std::vector<FamilyRoleBaseInfo> m_allTopRank; //前五名数据
	std::vector<FamilyRoleBaseInfo> m_allRankRole;//所有成员数据 根据贡献排行
	std::vector<FamilyRoleBaseInfo> m_NoPowerRole;//设置权限时显示无权限的成员
	std::vector<FamilyRoleBaseInfo> m_allFriends; //所有可被邀请的成员
	//unsigned m_FamilyPower;//俱乐部权限 0普通,1 副族长,2族长

	std::string			m_familyName;//名称
	std::string			m_famliyIconUrl ;//icon地址
	unsigned			m_familyID; //俱乐部id
	unsigned			m_familyRankID; //排行
	unsigned			m_familylevel; //等级	
	unsigned			m_curRoleCount;//当前成员
	unsigned			m_maxRoleCount;//最大成员
	unsigned			m_familyIconID;	// 头像
	std::string			m_familyNotice;//通知
	unsigned			m_ownerUid; //俱乐部张id
	bool				m_isOwner ;//是否是族长
	double				m_fund ;
	std::vector<std::function<void()>> m_vecCBSkinChanged;

public:
	void WaitChange(std::function<void()> cbChange);
public:

	void	setFamilyIconID(unsigned name) ;
	inline  unsigned getFamilyIconID(){return m_familyIconID ;} ;

	void	setFamilyIconUrl(std::string url) ;
	inline std::string getFamilyIconUrl(){return m_famliyIconUrl ;} ;

	inline void setCurRank(unsigned id){m_familyRankID = id ;} ;
	inline unsigned getCurRank(){return m_familyRankID ;} ;

	inline void setFamilyName(std::string name){m_familyName = name ; } ;
	inline std::string getFamilyName(){return m_familyName ;} ;
	
	inline void setFamilyID(unsigned id){m_familyID = id ;} ;
	inline unsigned getFamilyID(){return m_familyID ;} ;

	inline void setFamilyLevel(unsigned id){m_familylevel = id ;} ;
	inline unsigned getFamilyLevel(){return m_familylevel ;} ;

	inline void setFamilyCurRole(unsigned id){m_curRoleCount = id ;} ;
	inline unsigned getFamilyCurRole(){return m_curRoleCount ;} ;

	inline void setFamilyMaxRole(unsigned id){m_maxRoleCount = id ;} ;
	inline unsigned getFamilyMaxRole(){return m_maxRoleCount ;} ;

	inline void setFamilyNotice(std::string name){m_familyNotice = name ; } ;
	inline std::string getFamilyNotice(){return m_familyNotice ;} ;


	void setJoinFamily(bool flag) ;
	bool isJoinFamily();

	void setFamilyPower(unsigned power) ;
	unsigned getFamilyPower() ;

	void cleanMyFamliyInfo() ;
};


struct GlobalContinueLogin
{
    unsigned dayNum;
    unsigned SignID[10];
    unsigned SignMoney[10];
    unsigned SignStatus[10];
};

struct GlobalMyAddr
{
	GlobalMyAddr()
		: m_bHasAddr(false)
	{
	}

	bool m_bHasAddr;
	std::string m_strXingMing;
	std::string m_strShouJi;
	std::string m_strQQ;
	std::string m_strEmail;
	std::string m_strDiZhi;
};

// 登录保存信息
struct GlobalSession
{
	void SetOpenKey(const char *pszOpenKey)
	{
		assert(pszOpenKey);
		strncpy(m_szOpenKey, pszOpenKey, 64);
	}

	void SetOpenID(const char *pszOpenID)
	{
		assert(pszOpenID);
		strncpy(m_szOpenId, pszOpenID, 64);
	}

	void SetSerectKey(const char *pKey)
	{
		assert(pKey);
		strncpy(m_szSecret, pKey, 64);
	}

	void SetTimeStamp(int iTimestamp)
	{
		m_iTimeStamp = iTimestamp;
	}

	char m_szOpenKey[64];
	char m_szOpenId[64];
	char m_szSecret[64];
	int m_iTimeStamp;
};

// 服务器信息
struct GlobalServerInfo
{
	GlobalServerInfo()
		: m_iGameID(GAME_ID)
		, m_uGamePort(0)
		, m_uChatPort(0)
//		, m_iCurRoomIdx(0)
	{
	}

	void SetGameServerInfo(const char *pIP, unsigned uPort)
	{
		m_strGameIp = pIP;
		m_uGamePort = uPort;
	}
	void SetChatServerInfo(const char *pIP, unsigned uPort)
	{
		m_strChatIp = pIP;
		m_uChatPort = uPort;
	}

	int m_iGameID;
	std::string m_strGameIp;
	unsigned m_uGamePort;

	std::string m_strChatIp;
	unsigned m_uChatPort;

	std::string m_strVideoServerApp; // rtmp://服务器地址/应用名称/
};

// 全局配置信息
struct GlobalConfig 
{
	GlobalConfig();
	~GlobalConfig();

	void SetLastLogonInfo(const std::string& strAccount, const std::string& strPwd, bool bRemPwd, bool bAutoLogon, bool bSuccess);
	void SetLastLogonResult(bool bSuccess, const std::string& strAutoLoginUser,  const std::string& strAutoLoginPwd);

	void SetSoundMusicValue(double dBgSoundValue);
	void SetSoundEffectValue(double dEffectSoundValue);
	void SetSoundValue(double dBgSoundValue, double dEffectSoundValue);

	void GetLastLogonInfo(std::string& strAccount, std::string& strPwd, bool& bRemPwd, bool& bAutoLogon, bool& bSuccess);
	void GetLastLogonResult(std::string& strAutoAccount, std::string& strAutoPwd, bool& bAutoLogon, bool& bSuccess);
	void GetSoundValue(double& dBgSoundValue, double& dEffectSoundValue);
private:
	char		m_szLastLogonAccount[BUFFER_SIZE];
	char		m_szLastLogonPassword[BUFFER_SIZE];
	char		m_szAutoLoginName[BUFFER_SIZE];
	char		m_szAutoLoginPwd[BUFFER_SIZE];
	bool		m_bRemPwd, m_bAutoLogon;
	int			m_iLastLogonSuccess;
	int			m_iBgSoundValue;
	int			m_iEffectSoundValue;
	int			m_iReserved0;

public:
	bool m_bJinZhiPangGuan;
	bool m_bJinZhiTongZhuo;
	bool m_bJuJueYaoQing;

	double m_dMaxWinPerDay;
	unsigned m_uFreezenMinWhenWin;
	double m_dMaxLosePerDay;
	unsigned m_uFreezenMinWhenLose;
	unsigned m_MaxOnlineHour;
	unsigned m_uFreezenMinWhenTimeFull;
};


struct PlayerInfo
{
	unsigned		m_uChairID;
	unsigned		m_uID;
	std::string		m_strNickName;
	std::string		m_strIconUrl;
	unsigned		m_uSkin;
	unsigned		m_uVoice;
	double			m_dCash;
	unsigned		m_uGender;
};

struct RewardListInfo
{
	unsigned m_uRankID;				// 排名ID
	unsigned m_uID;					// UID
	unsigned m_uUpdateCount;		// 实时刷新的奖励数量
	std::string m_strNickName;		// 中奖的排名
	bool m_bIfShow;					// 是否高亮显示小花
	double m_dRankCount;			// 用户中奖的数量
	double m_uRoyalCount;			// 皇家奖励百分比
	double m_uflowersCount;			// 同花奖励百分比
	unsigned m_uInput;				// 每局投入
};

enum TableMode
{
	TablePlaying,
	TableLooking,
	TableTeaching,
	TablePlayback
};

struct GlobalTableConfig
{
	GlobalTableConfig()
		: m_uAreaID(0)
		, m_uRoomID(0)
		, m_uChairID(0)
		, m_uTableID(-1)
		, m_uMaxPlayer(0)
		, m_uPotLimitTime(0)
		, m_dLittleBlind(0)
		, m_dBigBlind(0)
		, m_dRewardMoney(0)
		, m_uCardRangeLow(0)
		, m_uCardRangeHigh(0)
		, m_dMagicExpressPrice(0)
		, m_dAutoBringMoney(0)
		, m_dMinBringMoney(0)
		, m_dMaxBringMoney(0)
		, m_uAutoSupplyToMaxMoney(0)
		, m_dAutoSupplyMoney(0)
		, m_tableMode(TablePlaying)
		, m_ifInvite(false)
		, m_strPassword("")
		, m_enterMoney(0)
		, m_uSound(1)
	{
	}

	void Clear();
	void SetTableMode(TableMode tableMode);
	TableMode GetTableMode();

	void SetMyChairID(unsigned uChairID);
	unsigned GetMyChairID();
	unsigned GetMyViewChairID();
	unsigned GetViewChairID(unsigned uChairID);
	unsigned GetChairID(unsigned uViewChairID);

    unsigned GetRandChairID();
	void AddPlayer(unsigned uChairID, unsigned uID, double dCash, const std::string& strNickName, const std::string& strIconUrl, unsigned uVoice,unsigned uSkin, unsigned uGender);
	PlayerInfo *GetPlayerByID(unsigned uID);
	PlayerInfo *GetPlayerByChair(unsigned uChairID);
	PlayerInfo *GetPlayerByViewChair(unsigned uViewChairID);
	void RemovePlayerByID(unsigned uID);
	void RemovePlayerByChair(unsigned uChairID);
	void ClearPlayer();

	bool IsTableHasVideo(); // 是否是视频桌

	std::string m_strRoomName;
    unsigned m_uRoomType;
	unsigned m_uAreaID; // 0白手起家 1中产阶级视频场 2中产阶级9人场 3德州大亨
	unsigned m_uRoomID;
	unsigned m_uChairID, m_uMaxPlayer, m_uPotLimitTime;
	int m_uTableID;
	double m_dBigBlind, m_dLittleBlind, m_dRewardMoney;
	unsigned m_uCardRangeLow, m_uCardRangeHigh;
	double m_dMagicExpressPrice;
	double m_dAutoBringMoney, m_dMinBringMoney, m_dMaxBringMoney;
	unsigned m_uAutoSupplyToMaxMoney;
	double m_dAutoSupplyMoney;
	double m_enterMoney;
	unsigned m_uSound;	// 判断ALLIN是否在GamenEnd协议播放AILLIN

	TableMode m_tableMode;
	std::vector<PlayerInfo> m_vecPlayerInfo;
	RewardListInfo m_RewardListInfo[5]; 

	bool m_ifInvite;
    bool m_bIsMTTScene; //是否为MTT场
	std::string m_strPassword;

	unsigned m_reason;//0   1钱币超过80W
};

struct TableAreaInfo
{
	unsigned m_uAreaID;
	double m_dMaxCarry;
	unsigned m_uPlayers;
	double m_dLittleBlind, m_dBigBlind;
};
struct DeZhouTableInfo
{
    unsigned            m_uAreaId;
    unsigned            m_uBlindId;
    double              m_dBlindMoney;
    std::string         m_strBlindName;
};
struct DeZhouMagnateInfo	// 德州大亨
{
    unsigned			m_uDeskID;					//房间名的ID
    unsigned            m_uRoomName;                //房间名
	std::string         m_sRoomVipName;				//贵族开房的房间名
    unsigned            m_uMustBet;                 //有必下，房间名优先显示必下
    std::string         m_strPlayerName[2];         //玩家名 1代表贵族房主2普通玩家名
    std::string         m_strIconURL[2];            //头像路径
    int                 m_UserID;                //玩家ID
    unsigned            m_uCurPlayer;               //当前玩家数量
    unsigned            m_uOnlookNum;               //旁观人数
    unsigned            m_uMaxPlayer;               //最大玩家数量
    bool                m_bIsCamera;                //是否可视屏
    bool                m_bIsVip;                   //是否贵族
    bool                m_bIsChallenge;             //是否单挑
    bool                m_bIsMustPour;              //是否必下
};
struct GlobalTableAreaInfo
{
	TableAreaInfo m_areaBSQJ[6];
	TableAreaInfo m_areaZCJJ6[6];
	TableAreaInfo m_areaZCJJ9[6];
    std::vector<DeZhouTableInfo> m_areaDZPage;
    std::vector<DeZhouMagnateInfo> m_areaDZDH;
};

struct DefineRoomInfo // 贵族开房
{
	unsigned		m_uType;			// 房间类型  0个人 1俱乐部 
	unsigned		m_uAreaID;			// ID 
	std::string     m_strRoomName;		// 房间名描述
	std::string     m_strOwnerName;		// 房主名
	std::string     m_strRoomPwd;		// 密码

	 
	unsigned		m_uPlayers;			// 在线玩家数量
	unsigned		m_uCurPlayers;		// 当前房间在线数量
	unsigned		m_uMaxPlayers;		// 当前房间最大数量
	unsigned        m_uLookOnNum;		// 旁观者人数


	unsigned		m_minCollect;		//基金最小起收
	unsigned		m_maxCollect ;		//基金封顶
	unsigned		m_fundP;			//基金收取百分比

	double			m_dMinCarry;		// 最小携带数
	double			m_dMaxCarry;		// 最大携带数

	double			m_dLittleBlind;		// 最小盲注值
	double			m_dBigBlind;		// 最大盲注值


	double          m_dMustBet;			// 前注(必下)
	double          m_dCoverCharge;     // 服务费 备用

	double          m_dEnterMoney;		// 进场money 

	unsigned		m_state;			//房间状态  0等待 1进行中
	bool			m_isMine;			//是否是我的房间

};

struct  GlobalVipOpenRoom
{
	DefineRoomInfo m_areaNormal[4];
	DefineRoomInfo m_areaCreateRoom;

	std::vector<DefineRoomInfo>& GetDefineRoomInfo()
	{
		return m_UserDefineRoom;
	}
	std::vector<DefineRoomInfo> m_UserDefineRoom;
	std::vector<DefineRoomInfo> m_ClubDefineRoom;
};


inline unsigned GetMissionType(unsigned uCID)
{
    //#0其它未知 1新手任务等 2成就任务 3每日任务 4每周任务
	if (uCID == 1)//新手任务
	{
		return 1;
	}
    else if (uCID == 2)//成就任务
    {
        return 2;
    }
    else if (uCID == 3)//每日任务
    {
        return 0;
    }
    else if (uCID == 4)//本周任务
    {
        return 3;
    }
    else 
        return 0;
}

struct MissionInfo
{
	unsigned uMissionID;
	unsigned uMissionType; // 0日常任务 1新手任务 2成就任务 3本周任务
	unsigned uMissionDrawID; // 领取ID
	std::string strMissionName;
	std::string strMissionDesc;
	std::string strMissionIcon;
	std::string strMissionTagName;
	unsigned uMissionTagValue;
	// Json::Value missionTag;
    unsigned uScence;//任务所属场次 1.常规场  2.SNG场  3.MTT场  4.所有场次 5.富可敌国 6.勤劳致富 7.赌神之路 8.竞技之神 9.德州之王
	unsigned uCurStep;
	unsigned uAllStep;
	unsigned uMissonStatus; // 0未完成 1已完成 2已领取
	bool bHasDraw; // 是否已经领取

	unsigned uMissionTag;
	unsigned uAcceptLevel;
	double dAward;
	bool bHasChildMission;
	unsigned uNextMissionID;
	bool bTipNextMission;

	// 新手任务独有
	std::string strNextMissionName;
	std::string strNextMissionDescription;
    std::string strRewardName;//实物奖励时使用
};

struct GlobalMissionInfo
{
	std::vector<MissionInfo>& GetMissions(unsigned uMissionType)
	{
		return m_vecMission[uMissionType];
	}
	MissionInfo *GetMission(unsigned uMissionID)
	{
		for (auto& vecMission : m_vecMission)
		{
			for (auto& mission : vecMission)
			{
				if (mission.uMissionID == uMissionID)
				{
					return &mission;
				}
			}
		}
		return nullptr;
	}
	std::vector<MissionInfo> m_vecMission[4];
};


struct FollowUserInfo
{
	unsigned m_uID;
	std::string m_strNickName;
	std::string m_strIcon;
	double m_dCash;
    unsigned m_uAttendStatus;//1:关注我的玩家被我关注了，0：关注我的玩家没有被我关注
};

struct GlobalFollowUserInfo
{
	std::vector<FollowUserInfo> m_vecFollowUser[3]; // 0我的关注 1关注我的 2德女郎
	unsigned m_uPageInfo[3][2];
};

//我的推广列表
struct GlobalFriendsSpread
{
    bool    m_bIsSpread;        //是否有推广员
    int     m_iUserID;          //推广员ID
    std::string m_strIcon;      //推广员头像
    std::string m_strName;      //推广员昵称
    std::vector<FollowUserInfo> m_vecFriendsSpread;
};

struct RankUserInfo
{
	unsigned m_uID;
	std::string m_strNickName;
	std::string m_strIconUrl;
	double m_dWinValue; // 如果是战绩排名，这个值代表赢得的底池数，其他排名这个值代表德州币
	unsigned m_uRank;
};

struct GlobalRankUserInfo
{
	std::vector<RankUserInfo>& GetRankList(unsigned uRankType, unsigned uWinType)
	{
		return m_vecRankList[uWinType*2+uRankType];
	}
	void GetPageInfo(unsigned uRankType, unsigned uWinType,
		unsigned& uCurPage, unsigned& uMaxPage)
	{
		uCurPage = m_uPageInfo[uWinType*2+uRankType][0];
		uMaxPage = m_uPageInfo[uWinType*2+uRankType][1];
	}

	std::vector<RankUserInfo> m_vecRankList[8]; //  财富
	unsigned m_uPageInfo[8][2];
};

struct QuickFollowInfo
{
	unsigned m_uType; // 0代表我关注的， 1代表德女郎, 2邀请我的
	unsigned m_uUserID;
	std::string m_strIconUrl;
	std::string m_strNickName;
	double m_dCash;

	bool m_bOnLine;
	unsigned m_uAreaID;
	unsigned m_uTableID;
	double m_dBigBlind;
	double m_dLittleBlind;
	bool m_bHasPassword;
	unsigned m_uTableType ;//主播所在房间类型
};
//俱乐部信息
struct FamilyInfo
{
	std::string m_familyName;
	unsigned m_familyID; //id
	unsigned m_familyRankID; //id
	unsigned m_familylevel; //id	
	unsigned m_curRoleCount;
	unsigned m_maxRoleCount;
	unsigned m_familyIconID;	// 头像
	unsigned m_familyFatherID; //族长id
	bool     m_isOwner;//是否是俱乐部长
	bool	 m_hasJoin ;//是否加入该俱乐部
};
struct GlobalFamilyInfo
{
	std::vector<FamilyInfo> m_vecFamilyInfo; 
	unsigned familyListSize ;
};
//加入的俱乐部
struct GlobalSelfJoinClub
{
	std::vector<FamilyInfo> m_vecSelfJoinClub; 
};

struct GlobalQuickFollowInfo
{
	std::vector<QuickFollowInfo> m_vecQuickFollowInfo[3]; // 0-我关注的 1-德女郎 2-邀请我的
    bool m_bIsReceiveInvite;
};

struct ActivityInfo //大厅操作栏按钮消息
{
    bool     m_bIsShow;
    unsigned m_uID;       //活动ID
    unsigned m_uSortIndex;//排序索引
	unsigned m_uType; // 1常规活动 2最新活动
    unsigned m_uRedefine;// 跳转到哪个界面(0：不跳转，1：商城，2：消息，3：任务，4：活动，5：俱乐部，6：常规场，7：SNG场，8：MTT场)
    unsigned m_uSecondRedfine;//二级跳转 0：不跳转
	std::string m_strTitle;
	std::string m_strDescription;
	std::string m_strIcon;
	std::string m_strUrl;
};

struct PushActivityInfo //系统推送消息，广告信息
{
    bool m_bIsShow;		               // 是否显示弹窗
    std::string m_strID;                    // 推送消息ID
    unsigned m_uShowType;              // 显示类型(半屏、全屏、其他)
    unsigned m_uRedefine;              // 跳转到哪个界面(0：不跳转，1：商城，2：消息，3：任务，4：活动，5：俱乐部，6：常规场，7：SNG场，8：MTT场)
    std::string m_pActivityTitle;      // 活动的标题
    std::string m_pActivityUrl;        // 活动的链接地址
};

struct GlobalActivityInfo
{
    std::vector<PushActivityInfo> m_vecPushActivityInfo;
	std::vector<ActivityInfo> m_vecActivityInfo;
};

struct ActReward//活动奖励
{
    unsigned    m_uState;//0未完成 1完成没有领取 2已经领取
    unsigned    m_uActivityID;
    std::string m_strDes;
    std::string m_strURL;
};

struct GlobalActRewardInfo
{
    std::string             m_strTitle;//活动标题
    std::vector<ActReward>  m_vecReward;//活动的奖励
};


struct MessageInfo
{
	unsigned m_uType; // 1邮件消息 2系统消息
	unsigned m_uID;
	std::string m_strContent;
	double m_dReward;
	std::string m_strTime;
	std::string m_strDate;

	unsigned m_continueDays;
	//俱乐部消息新家接口
	unsigned m_familyid;
	std::string m_familyName;
	unsigned m_familyIcon;

	unsigned m_userid;
	std::string m_userName;
	std::string m_userIcon;


	unsigned m_eventType;
	unsigned m_cash;

	unsigned m_hasDeal ;


	double m_strLastTime;
	//bool m_hasDeal ;

};
struct GlobalMessageInfo
{
	std::vector<MessageInfo> m_vecMessageInfo[4]; // 0邮件消息 1系统消息 2群消息 3活动消息
};

struct AchievementInfo
{
	std::string m_strTip;
	std::string m_strUrl;
	double m_dNum;
	bool m_bActivated;
};

struct GlobalAchievementInfo
{
	std::vector<AchievementInfo> m_vecAchievementInfo[5];
};

struct FruitInfo
{
	unsigned m_uID;              // 水果ID
    std::string m_strName;       // 水果名称
	std::string m_strIconUrl;    // 水果图片URL
	double m_dPrice;             // 水果价格
	double m_dReturnCash;        // 水果返回德州币数
};
struct VIPInfo
{
	double m_dAddLeBi;
	unsigned m_uAddJingYan;
	double m_dDengLuFanHuan;
	unsigned m_uAddMeiLi;
	bool m_bOpenGuiZuBiaoQing;
	bool m_bOpenGuiZuMoFaBiaoQing;
	unsigned m_uJiLuPaiJu;
	bool m_bOpenSiMiKaiFang;
	bool m_bOpenZiDingYiKaiFang;
	double m_dPrice;
	std::string m_strVipName;
};
struct ExchangeInfo
{
	std::string m_strName;
	unsigned m_uID;
};

/************************************************************************/
/* 商城道具开始                                                                    */
/************************************************************************/
struct ItemInShopInfo
{
	std::string m_iconUrl ; //图片地址
    std::string m_iconUrl2; //头像商城有两个图片地址
	std::string m_name ;    //名称
	std::string m_des;		//描述
    std::string m_strIndate;//有效期
	double m_price;		    //价格
	unsigned m_uID;			//id编号
    unsigned m_uDiamondNum; //钻石个数
	bool isNull;	
    bool m_isPastDue;       //是否过期
    unsigned m_uTicketNum;     //兑换券的个数
    unsigned m_uTicketConsume; //兑换消耗
	unsigned m_index ;		//序列
    unsigned m_uLastTime;   //剩余天数 头像商城
};

//struct GlobalItemShop
//{
//	std::vector<ItemInShopInfo> m_vecShopItemList; 
//};

struct GlobalMallInfo
{
	FruitInfo m_fruitInfo[6];
	VIPInfo m_vipInfo[6];
	//ExchangeInfo m_exchangeInfo[6];
	std::vector<ItemInShopInfo> m_vecShopItemList; 
    std::vector<ItemInShopInfo> m_vecHeadShop;//头像商城
    std::vector<ItemInShopInfo> m_vecExchange;//兑换专区
    std::vector<ItemInShopInfo> m_vecDiamond;//购买钻石
	// double m_dVipPrice[6]; // 6个VIP的购买价格
};

struct GameRecordInfo
{
	std::string m_strID; // 唯一的ID，因为比较长，用字符串表示
	unsigned m_uIdx; // 显示的第几手牌
	std::string m_strTableName; // 场子名：比如白手起家
	unsigned m_uMaxPlayer; // 最大玩家数
	double m_dBigBlind;
	double m_dSmallBlind;
	double m_dWinLost; // 盈利
	double m_dTime; // 1970年到现在的秒数
	unsigned char m_cbHandCard[2]; // 两张手牌
};
enum
{
	GameRecordPerPage = 15
};
struct GlobalGameRecordInfo
{
	unsigned m_uAllPage;
	std::vector<unsigned> m_vecPageIdx[2]; // 每一页的开始偏移
	std::vector<GameRecordInfo> m_vecRecord[2]; // 0-按时间排序1-按输赢排序
};

// 游戏动作序列化类,任务更新功能
enum class AtomicAction
{
	StartFirstGuide,
	Call,
	Fold,
	Raise,
	PlayInBanker,
	OpenCardType,
	FollowPlayer,
	Chat,
	VChat,
	SendMagicExpress,
	OpenSupplyJetton,
	InviteFriend,

	GameBegin,		// 游戏开始
	GameEnd,		// 游戏结束
	WinGame,		// 赢得游戏
	UpdateCash,		// 更新德州币
    UpdateDiamond,  // 更新钻石

    //新手任务,暂时只写18个
    TaskGuide,      //1、新手引导
    Task800,        //2、800场对局一次
    TaskCall,       //3、漂亮的跟注
    TaskFold,       //4、潇洒的弃牌
    TaskRaise,      //5、该加注时就加注
    TaskButton,     //6、庄家位置玩牌一次
    TaskCardIntro,  //7、点开一次牌型介绍
    TaskFllow,      //8、关注一个玩家
    TaskChat,       //9、愉快的聊天
    TaskVChat,      //10、语音聊天,  暂时没有
    Task2000,       //11、2000场对局一次
    TaskMagic,      //12、发一个魔法表情
    TaskAddJetton,  //13、打开增加筹码界面
    Task4000,       //14、4000场对局一次
    Task6000,       //15、6000场对局一次
    Task10000,       //16、10000场对局一次
    Task20000,       //17、20000场对局一次
    Task30000,       //18、非视频30000场对局一次
    TaskV30000,       //19、视频30000场对局一次
};


class GameSerialization
{
public:
	GameSerialization();
	void AddAtomicAction(AtomicAction atomicAction);
	void OnWinGame();
	void OnGameStart();
	void OnGameEnd();
	bool IsInGameNow();
private:
	bool m_bGamePlaying;
};

extern GlobalTempData g_globalTempData;
extern GlobalMyData g_globalMyData;
extern GlobalSession g_globalSession;
extern GlobalServerInfo g_globalServerInfo;
extern GlobalConfig g_globalConfig;
extern GlobalMyAddr g_globalMyAddr;
extern GlobalTableConfig g_globalTableConfig;
extern GlobalMissionInfo g_globalMissionInfo;
extern GlobalTableAreaInfo g_globalTableAreaInfo;
extern GlobalFollowUserInfo g_globalFollowUserInfo;
extern GlobalRankUserInfo g_globalRankUserInfo;
extern GlobalQuickFollowInfo g_globalQuickFollowInfo;
extern GlobalActivityInfo g_globalActivityInfo;
extern GlobalActRewardInfo g_globalActRewardInfo;
extern GlobalMessageInfo g_globalMessageInfo;
extern GlobalAchievementInfo g_globalAchievementInfo;
extern GlobalMallInfo g_globalMallInfo;
extern GlobalVipOpenRoom g_globalVipOpenRoom;
extern GlobalGameRecordInfo g_globalGameRecord;
extern GameSerialization g_globalGameSerialization;
extern GlobalContinueLogin g_globalContinueLogin;

extern GlobalFamilyInfo g_globalFamilyInfo ;
extern GlobalMyFamilyInfo g_globalMyFamilyInfo;
//extern GlobalTargetFamilyInfo g_globalTargetFamilyInfo;

extern GlobalFamilySKin g_globalFamilySKin;

extern GlobaRatio g_globalRatio;

extern GlobalSngHallInfo g_globalSngHallInfo ;
extern GlobalSngGameInfo g_globalSngTableInfo ;
extern GlobalSngRankInfo g_globalSngRankInfo ;

extern GlobalMttHallInfo g_globalMttHallInfo ;



extern GlobalUserItemList g_globalUserItemList ;

extern GlobalUserSigin g_globalUserSigin ;
extern GlobalUpdateInfo g_globaUpdateInfo ;
extern GlobalMttGameInfo g_globaMttGameInfo ;

extern UpGradeUserAccount g_globalUpgradeAccount;


extern GlobaMttChangedTicket m_MttChangedTicket;


extern MttFloatReward m_mttFloatMatchRewardInfo ;

extern GlobalAllFundData m_globalAllFundData ;

extern GlobalFunData m_globalFundData ;

extern GlobalPrivateRoomInfo m_globaPrivateRoomInfo ;

extern GlobalSelfJoinClub g_globaSelfJoinClub ;

extern GlobalFriendsSpread g_globalFriendsSpread;
#endif
