#ifndef __GAMEFRAME_H__
#define __GAMEFRAME_H__

#include "cocos2d.h"
#include "GameCommon.h"
#include "RpcRoutine.h"
#include "RoomMessageIO.h"
#include "GameMessageIO.h"
#include "GrabKeyboard.h"
#include "SyncDeque.h"

class LogonLayerEx;
class GameHall;
class GameHall2;
class GameMall;
class GameTable;
class TextTip2;
class GameLoading;
class RewardDrawingAni;
class GameTipLayer;
class VipPrivateNormal;
class GameTablePlayer;
class GameRecord;
class GameSet;
class EnterVipRoomEx;
class PaymentLoading;
class CustomerServices;
class OpenUpVip;
class ShowFamily;
class NewbieGuideLayer;
class NewPlayerTask;
class LogoLayer ;
class GameSngHall ;
class GameMttHall ;
class SngSignUp;
class UpdateManager ;
class SngRankList ;
class MttStartNotice ;
class MTTMatchState;
class MttMatchDisMissTips ;
class MttQuitMatch ;
class MttSignUpSuccess ;
class MttJoinMatchTips ;
class ActivityRewards;
class SelfFund ;

struct GlobalSngTableInfo ;

enum SocketNoticeType
{
	Notic_JoinFamily =0,
	Notic_ToViceSHaikh,
	Notic_NOViceSHaikh,
	Notic_NewShaikh,
	Notic_PushOutFamily,
	
};

enum TableType
{
	NullType = -1 ,
	NormalTable=0,
	SngTable,
	MttTable,
	PrivateTable,//私密房
};

class GameFrame : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameFrame);
	void PopTipLayerWithBt1(const std::string& strContent, const std::string& strBt, std::function<void(unsigned)> cbBt = nullptr);
	void PopTipLayerWithBt2(const std::string& strContent, const std::string& strBt1, const std::string& strBt2, std::function<void(unsigned)> cbBt = nullptr);
	void OnClientRequest(ClientRequest request, const Json::Value& param);
	GlobalKeyGrabber* GetEscKeyGrabber();
	void PushNonUIMsg(const Json::Value& msg);

	void AddCashChangedCB(std::function<void()> cbCashChange);
	void AddNickNameChangedCB(std::function<void()> cbNickNameChange);
	void AddVIPChangedCB(std::function<void()> cbVipChange);
	void AddIconChangedCB(std::function<void()> cbIconChange);

	void forceLeaveTable();
	void forceLeaveHall();
	void showAutoLogin();

    void jumpToLogin();
	void forceLeave();
	void reconnectGameServer(float delta);
	inline void setCheckTime(float time ){ m_checkTime = time ;} ;
	void setTableType(TableType type) ;
    TableType getTableType() ;
    void resetCheckNetwork();
	void startCheckNetwork();
    void PauseHeartBeatCheck(unsigned num);
    bool isLoginVisible();

	void checkOnline(float dt = 0.1f);
	void updateCheckOnline(float dt);
	void popupNetworkLoading(bool visible);
	void showLoginLayer(bool flag) ;

	void showLoadingLayer(bool flag) ;

	void showGameRun(bool flag) ;
	void DestroyNewbieGuide();
    
    void PopLoading(bool bVisible, const char* msg = "");

	void callShowSngSignUp(bool flag,const GlobalSngTableInfo& followInfo ) ;//显示sng报名界面

	void hideUserItemLayer()  ;//隐藏sng报名界面
	void showGameMall(int index) ;//显示商城界面
	void callShowUserItem() ;//显示我的道具界面

	void cleanLogin() ;//登录失败后清空rpc登录消息

	void callGameTipsMove() ;

	bool getLoginFlag() { return m_loginFlag;}

	void showBgLayer(bool flag) ;

	void showMasterRankList(bool flag) ;
	void showMttStartNotice(bool flag,unsigned tableid =0,std::string name="",unsigned time=0) ;
	void showMttGameStart(bool flag,std::string name="") ;

	void showMttMatchState(bool flag, unsigned index) ;
	void showMttMatchDisMissTips(bool flag ,unsigned tablid=0,std::string name="",std::string reason="") ;

	void doJoinMttMatch(unsigned tableid,bool delayRegist = false) ;
	void JoinToWaitingRoom(unsigned tableid, std::string tablename,int time) ;

	void showQuitMatch(bool flag ,int type) ;

	void showSignUpSucess(bool flag,std::string name="",std::string money="",std::string time="") ;//报名成功

	bool isInTable() ;

	void updateMttHallFixTime(float time) ;

    unsigned getMttMatchStateIndex();

    bool isMttMatchStateVisible();

    void updateMttMatchState();

	void EnterGameMttHall() ;

	void showJoinMatchTips(bool flag,std::string name="",unsigned time =0,unsigned tableid = 0) ;//赛前20秒提示界面

	inline MTTMatchState* getMttMatchState(){return m_pMTTMatchState ;} ;
	bool isMttStateVisiable();

	inline NewPlayerTask* getNewPlayerTask(){return m_pNewPlayerTask ;} ;
	bool isNewPlayerTaskVisiable();
    void popHallBottomDialog(unsigned uType);
	
	void requestHasMatchByInTable(bool flag) ;

	void doShankeTable(float px,float py,float time, int count) ;


	void showSelfFundLayer(bool flag,int index = 0) ;
	
protected:
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
    
	virtual void update(float delta);

	void OnNonUIMsg(const Json::Value& msg);
	
	void OnRpcResult(const RPC& rpc);
	void OnRpcResultSuccess(RPCCmd rpcID, const Json::Value& result, const RPC& rpc);
	void OnRpcResultFailed(RPCCmd rpcID, const char *pFaildInfos);
	virtual void OnGameIOAbord();
	virtual void OnGameMessage(const GameMsgHead& msgHead, const char*pBody, unsigned uBodySize);

	virtual void OnRoomIOAbord();
	virtual void OnRoomMessage(RoomMsgCmdS cmdS, const char *pMsgBody, unsigned uMsgBodySize);

	void updateDialogVisible(float delta);


	void callSocketTips(SocketNoticeType type,unsigned toUid,std::string fanme,unsigned fid) ;

	void onScheduleIfRemoveShouYaoQing(float fDelat);

	void readData() ;//读取相关存档
    void CheckLogin(float fDelt);

	
private:
	GameFrame();
	~GameFrame();

	bool ConnectGameServer();
	bool ConnectChatServer();

	

    void PopPaymentLoading(bool bVisible);
	void PopDrawingAwardAni(double dAward);
	void PopTablePlayBack(const Json::Value& gameRecord);
	void PopPassWordInputBox(std::function<void(const std::string&)> cbBt);
    void PopNewerTask();
    void PopActivityReward();

	void preloadHallTable();
	void EnterGameHall();
	void EnterGameHall2();
	void EnterGameTable();
	void EnterVipOpenRoom(); // 进入贵族开房
	void BackToPrevView();

	void ShowGameSet(bool bNeedConfirm);


	void EnterGameSngHall() ;


	void updateHeartBeat(float dt);
	void resetOnlineVars();

    void doUploadHeader(std::string& openId, std::string& openKey, int timestamp);

	
	void UpdateSngRankList(unsigned uRankType, unsigned uWinType);
    void updateNormalTask();
    void updateSNGTask();
    void updateMTTTask();

    bool checkTableSocket(unsigned tid) ;
private:
	GlobalKeyGrabber m_s_escGrabber;
	SelfFund	*m_selfFund ;
	RpcRoutine m_rpcRoutine;
	GameMessageIO m_gameMsgIO;
	RoomMessageIO m_roomMsgIO;

	//LogoLayer *m_loadingLayer ;
	LogonLayerEx *m_pLogonLayerEx;
	GameHall *m_pGameHall;
	GameHall2 *m_pGameHall2;
	GameSngHall* m_pGameSngHall ;
	GameMttHall* m_pGameMttHall ;

	GameSet *m_pGameSet;
	VipPrivateNormal *m_pVipPrivateNormal;
	GameTable *m_pGameTable;
    NewPlayerTask* m_pNewPlayerTask;
	GameMall *m_pGameMall;
	GameTablePlayer *m_pTablePlayer;
	GameLoading *m_pGameLoading;
    PaymentLoading *m_pPaymentLoading;
	TextTip2 *m_pTextTip;
	RewardDrawingAni *m_pRewardDrawingAni;
	GameTipLayer *m_pTipLayerWithBt1;
	GameTipLayer *m_pTipLayerWithBt2;
    GameRecord *m_pGameRecord;
	EnterVipRoomEx *m_pPwdInput;
    CustomerServices *m_CurtomerServices;
	OpenUpVip* m_pOpenUpVip;
	ShowFamily* m_pShowFamily;
	std::deque<cocos2d::Node*> m_deqHallOrTable;
	SyncDeque<Json::Value> m_syncDeqNonUIMsg;

	cocos2d::EventListenerCustom* m_networkListener;

	SngSignUp* m_sngSignUp ;
    cocos2d::LayerColor* m_bgLayer;
	SngRankList* m_masterRankList ;

	MttStartNotice* m_mttStartNotice ;
    MTTMatchState   *m_pMTTMatchState;
	MttMatchDisMissTips *m_mttMatchDisMissTips ;
	MttQuitMatch *m_mttQuitMatch ;
	MttSignUpSuccess *m_mttSignUpSucess ;
	MttJoinMatchTips* m_mttJoinMatchTips ;
    ActivityRewards  *m_pActivityRewards;


public:
    NewbieGuideLayer* m_pNewbieGuide;
	float m_checkTime;
	bool m_loginFlag;
	bool m_bIfYaoQing;
	float m_checkDialog;
	int m_dlgOriginalStatus;
	static bool m_tipDialogVisible;
    bool m_bJumpOverGuide;
	bool clearNetWork ;

    bool m_checkNetwork;
    float m_checkNetworkDelta;
    float m_checkDuration;

	unsigned int m_onlineFlags;
	float m_onlineDelta;
    Json::Value m_paramPurchase;//保存购买道具名称及价格

	long m_lastDeltaTime;

	bool m_needCheckOnlineByLoginAgian;//是否是重复登录后的断线重连
	bool m_showReconnect ;
    bool m_bIsRecvGameScence;//有没有收到205消息
};

extern GameFrame *g_pGameFrame;


#endif