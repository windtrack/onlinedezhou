#ifndef __GAMETABLE_H__
#define __GAMETABLE_H__

#include "cocos2d.h"
#include "GameCommon.h"
#include "GameDialog.h"
#include "UICommon.h"
#include "VideoPlay.h"
#include "GameFrame.h"
#include "MoveLabel.h"
namespace Json
{
	class Value;
}

class FrameSprite;
class Dealer;
class JettonFactory;
class UserView;
class TouchGrabber;
class SidePot;
class TextureNumber;
class Reward;
class MenuItemFrameSprite;
class MenuItemSpriteWithLabel;
class ActionBar;
class PreActionBar;
class BrightActionBar;
class DoorMenu;
class CardTypeView;
class QuickShop;
class ChatBox;
class ShowLabelInLineSequence;
class TableTip;
class UserInfoView;
struct UserDetailInfo;
class QuickMissionList;
class QuickInviteList;
struct QuickFollowInfo;
class AnimateViewer;
class WinningRaking;
class LayerButton;
class WaitBeginGame;
class AnimationManager;
class NewPlayerTask;
class ShowFamily;
class LineLabel ;
//SNG
class SngMatchState ;
class SngRankView ;

class MttAddonTips ;
class MttRebuyTips ;
class  MttRebuyTipsByServer ;
class MttRankView;
//struct RewardListInfo;
struct MissionInfo;
class PrivateRoomInfo ;

struct UsrWinInfo
{
	unsigned uChair;
	unsigned uWinPots;
	unsigned uWinPotsIdx[GAME_PLAYER];
	double dPotsWinValue[GAME_PLAYER];
	double dAllWinValue;
    bool bIsWinner;
};

struct UserPos
{
	cocos2d::Point u_headPos;
	cocos2d::Point u_namePos;
	cocos2d::Point u_vipPos;
	cocos2d::Point u_cashPos;
	cocos2d::Point u_thinkPos;
	cocos2d::Point u_biaojiPos;
	cocos2d::Point u_denvlangPos;
};


struct UsrHandCard
{
    unsigned char handCard[2] ;
    unsigned uChair;
};




class GameTable : public GameDialog// cocos2d::Layer
{
public:
	virtual bool init(); 
	CREATE_FUNC(GameTable);

	// CAODANDE begin
	void CaoDanDeSendReady();
	void CaoDanDeSendLittleBlind();
	void CaoDanDeSendBigBlind();
	// CAODANDE end

	void CleanTableAfterEnd(); // 游戏结束发放筹码结算之后，发送准备消息之前要清理下桌面，否则手牌信息公牌，牌型提示，任务头像状态等信息还会保留
	void CleanTable();
	void CleanPlayer();
	void SendTip(const std::string& strTip);

	void ResetTable();

	// 初始化桌子界面

	void SetLookingOn(bool bLookingOn);
	void SetDPosition(unsigned uChairID);
	void SetSidePot(const std::vector<double>& vecCurAllPot);
	void SetAllPot(double dAllPot);
	void SetUser(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID,  unsigned uVoice,unsigned uSkin, unsigned m_uLevel, int uBiaoJiIdx, unsigned uLadyId, unsigned uGender, const std::string& strVideoStreamUrl);
	void SetUserShowName(unsigned uChairID, int playerStatus);
	void SetUsrBetedJetton(unsigned uChairID, double dBetCash);
	void SetUsrCash(unsigned uChairID, double dCash);
	void SetPublicCard(std::vector<unsigned char> vecPublicCard);

	// 游戏状态下动作响应
	void UsrSitDown(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID,  unsigned uVoiceid, unsigned uSkinID, unsigned m_uLevel, int uBiaoJiIdx,unsigned uLadyId, unsigned uGender, const std::string& strVideoStreamUrl, bool bAni = true, bool newPlayer = false);
	void UsrStandUp(unsigned uChairID, bool bAni = true);
	
	
	void UsrPlaceJetton(unsigned uChairID, double dBetValue);
	//void UsrPlaceBlindJetton(unsigned uChaidID[2], double dBlindValue[2], double dRestValue[2]);
	//void UsrPrePlaceJetton(const std::vector<unsigned>& vecChairID, const std::vector<double>& vecPlacedValue, const std::vector<double>& vecRestValue);

	void DispatchHandCard(const std::vector<unsigned>& vecDispatchingChairID, unsigned char cbMyCardData[2]); //仿照天天德州的发牌动画
	void DispatchHandCard2(const std::vector<unsigned>& vecDispatchingChairID, unsigned char cbMyCardData[2]); // 浓浓flash味的发牌动画

	void DispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard);
	bool PreDispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard);
	void StartDispatchPublicCard();
	void DispatchPublicCardImpl(const std::vector<unsigned char>& vecDispatchingPublicCard);
	bool IsDispatchingPublicCard();
	
	int GetCurThinkingUsr(); // 获取当前正在等待说话的玩家椅子号没有return-1
	void WaitUsrAction(unsigned uChairID);
	void SetSelfActionInfo(unsigned uBetType ,double dBalance, double dMinRaise, double dMaxRaise, bool bHasDispatchPublicCard);
	void UpdateSelfMinCall(double dBalance);	

	void UsrCheck(unsigned uChairID);
	void UsrCall(unsigned uChairID, double dBetValue);
	void UsrRaise(unsigned uChairID, double dBetValue);
	void UsrAllIn(unsigned uChairID, double dBetValue);
	void UsrFoldCard(unsigned uChairID);

	void PlayUsrActSound(unsigned uChairID, unsigned uAct);

	void ShowMeHasActioned();
	void ShowMeHasFold(unsigned strTip);
	void ShowMeHasWin();
	void ShowGameHasEnd(bool bOpenedCard);

	void CollectUsrBetToPot(const std::vector<double>& vecCurAllPot);
	void ActivateUserStatus(unsigned uS, unsigned uExceptS);
	//void OpenUsrCard(unsigned uChairID, unsigned char cbCardData[2]);
	//void SetUsrWinType(unsigned uChairID, unsigned uWinType);
    void HideHandCard();//开牌前先隐藏两张倾斜的手牌
	void OpenUsrCard(const std::vector<unsigned>& vecOpenCardUsr, const std::vector<unsigned>& vecOpenCardData);
	void ShowUsrWinInfo(unsigned uWinType, const std::vector<unsigned>& vecWinChairs, bool bWaitOpenCard, const std::vector<UsrWinInfo>& vecUsrWinInfo);

	void UsrShowCard(unsigned uChairID, unsigned char cbCardData[2]);
	void UsrChat(unsigned uChairID, const std::string& strChat);
    void RewardBubble();
	void UsrExpress(unsigned uChairID, unsigned uIdx, unsigned uType);
	void LookonUsrChat(unsigned uUID, const std::string& strNickName, const std::string& strIcon,  const std::string& strChat);
	void LookonUsrExpress(const std::string& strNickName, unsigned uIdx, unsigned uType);

	void UsrTool(unsigned uChairIDSrc, unsigned uChairIDDst, unsigned uToolIdx);
	void UsrAward(unsigned uChairID, double dCash);
	void UserFollowUser(unsigned uSrcCh, unsigned uDstCh);

	void PopUserInfo(const UserDetailInfo& usrInfo);
	void SetGuanZhuResult(unsigned uTUID, int iGuanZhuType, int iRet);
	void SetPingBiResult(unsigned uTUID, int iPingBiType, int iRet);
	void SetBiaoJiOrBeiZhuResult(unsigned uTUID, const std::string& strBeiZhu, int iBiaoJi, const std::string& strBiaoJi);
	void UpdateMissionList();
    void UpdateNewPlayerTask();
	void UpdateOnlineFollow(unsigned uType);
	void SetValue(double dFee);
	void UpdateItemBox();
	// uOpType 0-remove 1-appand 2-update 3-updateandtop
	void OperateOnlineFollow(unsigned uOpType, unsigned uType, const QuickFollowInfo& quickFollowInfo);
    void PopChatBox();

	void UpdateWaitPlayerNum(unsigned uWaitNum);
    void DryAshTwoMinCard(unsigned uChairId, unsigned char *handCard);//灰化两张小牌,0结束亮牌，1开始发牌
    void ReSetPublicCardPos(unsigned dispatchCount);//重新设置工牌坐标（前面小牌y坐标下移了）

	int checkSitdown();

	void GetShowFamily();
	Node* GetGameHallNode();
	void showTargetFamily(bool useZoom,bool flag);
	void callUpdateTargetFamilyInfo();
	//void onEnter();
	//void onExit() ;

	void PlayEffectSound2(const std::string& strSound, bool bLoop = false);

	void SetHandCard(unsigned uChairId, int status);

	void SetActiveStatus(unsigned uChairID, unsigned status);

    void SetWaitSitDownStatus(bool IsWait);

	void OnCancleHideQuickShopUI(bool bVisible);

	void setTableType(TableType type) ;

	TableType getTableType() ;

	void callShowSngRankInFailed(unsigned uChairID,unsigned rankid) ;

	void updateSngTableRankTips() ;//更新桌面sng排名

	void updateMatchBindTipsInTable() ;//更新当前显示的盲注

	void updateSngUIBlind() ;//更新牌桌盲注数据

	void updateSngRankList() ;//更新排行列表

	void showSngRankView(bool flag ,int rankId =0,int cash=0,int score = 0) ;//显示sng胜利或失败的弹出框

	void cleanRankShow() ;

	void resetTimeOve() ;//重设托管器时间

	void showGameAutoTrust(bool flag) ;//托管

	void showSngGameOver(bool flag) ;//sng结束提示 

	void reSetScorlllTips() ;//重置滚动数据

	void updateCurSngPlayerCount() ;//更新当前sng场玩家数量

	void callNextScrollText() ;//通知下一个滚动提示
    //好友按钮界面引导
    void createFriendGuide();
    //增加关注界面引导
    void createAttentionGuide();

    bool getOtherPlayerPos(unsigned &uChair, Vec2 &pos);

	void updateMttTime(int time) ;

	void updateMttButtonShow() ;

	void updateMttRankInTable(bool flag) ;

	void showMttRankView(bool flag,unsigned uRankId=0,std::string desc="",std::string icon="",std::string name="",unsigned cash=0,unsigned score=0);

	void showMttGameOver(bool flag) ;

	void showMttRebuyTipsByServer(bool flag,int time=0) ;

	void reSetMttTable(bool flag) ;

	void doDismassMttTable() ;

	void callShowJoinTableEffect(int chairId,int vipLev, int skinid,bool newPalyer) ;

	void doControllPrivateTable(bool isOwner,unsigned state) ;

	void setPrivateGameControllButtonVisiable(bool flag) ;

	void updatePrivateTipsPosition() ;

	void updatePrivateFund(bool flag) ;
private:
	GameTable();
	~GameTable();

	void SetUsrBeting(unsigned uChairID);
	void SetUsrBetFinished(unsigned uChairID);
	unsigned GetUsrBeting();
	void SetAllBetFinishedCB(std::function<void()>);

	void SetJettonCollecting(bool bCollecting);
	bool IsJettonCollecting();
	void SetJettonCollectFinishedCB(std::function<void()>);

	void SetPublicCardDispatchedCB(std::function<void(unsigned uAllPublicCard)>);
	
//	void DispatchWinJettonToUsr(const std::vector<unsigned>& vecUsrChairId, const std::vector<std::vector<std::pair<unsigned, double>>>& vecvecPotWinValue);
	void DispatchWinJettonToUsr(const std::vector<UsrWinInfo>& vecUsrWinInfo, bool bOpenedCard);

	virtual void onSetVisible(bool bVisible) override;
	virtual void onKeyEscPressed() override;

    virtual void onGrabTouchBegan(bool bHitDialog) override;
    
	void GetJettonIdxs(double dValue, std::vector<int>& vecJettonIdx);
	unsigned GetJettonIdx(double dValue);
	cocos2d::Vec2 GetSidePotPosition(unsigned uPotCount, unsigned uPotIdx);

	// call back
	void onMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
	void onSngMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
	void onMttMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
	void onPrivateMenuItemClicked(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);

	void onScheduleUpdateTime(float fDelta);

	void onScheduleShakeHandCard(float fDelta);
	void onScheduleFoldCard(float fDelta);
	void onScheduleSngFoldCard(float fDelta);
	void OnScheduleDispatchHandCard(float fDelta); 
	void DishpatchPublicCard(unsigned uIdx, unsigned char cbCardData, bool bDelay);
	void OnPublicCardDispatched(unsigned uIdx);
	void OnScheduleOpenUsrCardFinished(float fDelta);
	void OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched(float fDelta);
	void OnScheduleDispatchWinJettonToUsr(float fDelta);

	void onMenuItemAction(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);

	//
	cocos2d::Sprite *GetSpriteDispatchingCard(unsigned uRoundId, unsigned uChairId);
	cocos2d::Sprite *GetHandCard(unsigned uChairID, unsigned uCardIdx);
	FrameSprite *GetSpriteMyCard(unsigned uCardId);
	cocos2d::Sprite *GetSpriteFoldingCard(unsigned uChairId, unsigned uIdx);
	FrameSprite *GetSpritePublicCard(unsigned uCardId);
	UserView* GetUserView(unsigned uChairID);
	TextureNumber *GetUserWinNumber(unsigned uChairID);
	ChatBox *GetChatBox();
	TableTip *GetTableTip();

	void PopUserInfo(unsigned uChairID);
	//清理牌局动画
	void cleanAllPaijuAction() ;

	//Sng
	void initSngTable() ;//初始化sng
	void showSngMatchState(bool flag) ;//显示sng游戏状态
	void callSngExit() ;//sng退出面板

	//mtt
	void initMttTable() ;
	void showMttRebuyTips(bool flag) ;
	void showMttAddonTips(bool flag) ;
	//表情单独提出来
	void initUsrExpress() ;

	
	//私密房
	void initPrivateRoomInfo() ;
	void showPrivateRoomInfoLayer(bool flag) ;

private:
	struct UserInfo
	{
		std::string m_strName;
		std::string m_strIcon;
		double m_dCash;
		unsigned m_uUID;
		unsigned m_uSkin;
		unsigned m_uGender;
		unsigned m_uShowGirlLvl;
		std::string m_strVideoStreamUrl;	
	};
private:
		Dealer *m_pDealer;											// 美女荷官
		NewPlayerTask* m_pNewPlayerTask;
		DoorMenu *m_pDoorMenu;
		CardTypeView *m_pCardTypeView;
		QuickShop *m_pQuickShop;
		ChatBox *m_pChatBox;
		ShowLabelInLineSequence *m_pUsrChatTip[GAME_PLAYER];
		TableTip *m_pTableTip;
		UserInfoView *m_pUserInfoView;
		UserInfoView *m_pMyInfoView;
		ShowFamily* m_targetFamily;
		ShowFamily *m_pShowFamily;
		QuickMissionList *m_pQuickMissionList;
		QuickInviteList *m_pQuickInviteList;
		SngMatchState* m_sngMatchState ;
		SngRankView* m_sngRankView ;

		MttAddonTips* m_mttAddonTips;
		MttRebuyTips* m_mttRebuyTips ;
		MttRebuyTipsByServer *m_mttRebuyTipsBySercer ;

		PrivateRoomInfo		*m_privateRoomInfo ;
private:
	std::string m_strOldChat;
	cocos2d::Node* s_uiRoot ;
	cocos2d::Node* s_gameLayer ;
    unsigned m_uPreThinkChairID;
    bool m_IsWaitingSitDown;
	bool m_bDispatchingHandCard;
	unsigned m_uOverTime; // 超时次数，如果超时次数过3，则自动站起，并提示连续多次超时，是否太累，先旁观一会
	// 一旦用户点击站起或者离开，该值清零

	UserInfo m_usrInfo[GAME_PLAYER];
	bool m_bUserValid[GAME_PLAYER];
    unsigned char m_cbMaxCardType[5]; 
	unsigned m_uDispatchingIdx;
	std::vector<unsigned> m_vecDispatchingChairID;
	unsigned char m_cbMyCardData[2]; // 非常重要，如果都==0，则证明没有参与这一把游戏，游戏开始后再进来的，否则证明有参加游戏
	std::vector<unsigned char> m_vecPublicCardData;
	std::deque<unsigned char> m_deqDispatchingPublicCardData;
	bool m_bAllPublicCardDispatched;

	unsigned m_uBetingCount;
	bool ifVipSign;
	std::function<void()> m_funCBAllBetingFinished;

	bool m_bCollecting;
	std::function<void()> m_funCBCollectingFinished;

	std::function<void(unsigned)> m_funCBPublicCardDispatched;

	bool m_bOpenCarding;
	bool m_bOpenCardFinished;
	unsigned m_uWinType;
	std::vector<unsigned> m_vecWinChairs;
	std::vector<UsrWinInfo> m_vecUsrWinInfo;

	UserPos m_usePosInfo[GAME_PLAYER];
	// view
	VideoPlayMgr m_videoPlayMgr;
	JettonFactory *m_pJettonFactory;

	cocos2d::ui::Text *m_pLabelTime;
	//cocos2d::Label *m_pLabelTime;

	cocos2d::ui::Button *m_pMenuItemReward;//奖励按钮
	cocos2d::ui::Button *m_pMenuItemPopMenu;//左上角菜单
	cocos2d::ui::Button *m_pMenuItemPopChat;//聊天按钮
	cocos2d::ui::Button *m_pMenuItemRecordSound;//语音按钮
	cocos2d::ui::Button *m_pMenuItemPopMissionReward;//宝箱奖励
	cocos2d::ui::Button *m_pMenuItemPopMall;//商城
	cocos2d::ui::Button *m_pMenuItemPopInvite;//邀请
	cocos2d::ui::Button *m_pMenuItemPopMission;//任务
	cocos2d::ui::Button *m_pMenuItemWinningRanking;//奖池

	cocos2d::ui::Button *m_pMenuItemPopMissionRewardAni;//宝箱动画
	cocos2d::ui::Button *m_pMenuItemBrightCard;//亮牌
	AnimationManager *m_pRewardAnimateViewer;
	TextureNumber *m_pRewardNumber;



	cocos2d::ui::ImageView* m_rootUser[GAME_PLAYER];

	double m_dAllPlacedJettonValue;
	cocos2d::ui::Text *m_pLabelAllPlacedJettonValue;				// 底池数量
	cocos2d::Sprite* m_pSpriteDispatchingCard[GAME_PLAYER*2];	// 18张正在发送的小牌
	cocos2d::Sprite* m_pSpriteFoldingCard[GAME_PLAYER*2];			// 9张弃牌
	UserView *m_pUsers[GAME_PLAYER];							// 9个玩家视图
	TouchGrabber *m_pUsersViewTouchGrabber[GAME_PLAYER];					// 
	cocos2d::Sprite *m_pSpriteDFlag;							// D标志

	cocos2d::Sprite *m_pSpriteShowMaxCard;
	//cocos2d::Animate *m_pAniShowMaxCard[4];
	AnimationManager *m_pAniShowMaxCard[4]; 
	cocostudio::ArmatureAnimation *m_pAnimationShowMaxCard[4]; 

	cocos2d::Sprite *m_pSpriteShowMeWin;
	//cocostudio::Armature *m_pAniShowMeWin;
	//cocos2d::Animate *m_pAniShowMeWin;
	AnimationManager* m_pAniShowMeWin;
	FrameSprite *m_pSpritePublicCard[5];					// 5张公牌
	bool		m_hasPublicCard[5] ;
	FrameSprite *m_pSpriteMyCard[2];						// 2张自己的手牌
	FrameSprite *m_pSpriteMaxCardType;
    cocos2d::Sprite *m_pSpritePublicCardCash[5];            // 5张公牌灰化

	double m_dUsrPotValue[GAME_PLAYER];							// 用户已下注的筹码值
	cocos2d::Sprite *m_pSpriteJetton2UsrPot[GAME_PLAYER];		// 用户下注筹码-从用户飞向用户池
	cocos2d::Sprite *m_pSpriteJetton2SidePot[GAME_PLAYER];		// 用户下注筹码-从用户池飞向边池
	cocos2d::Sprite *m_pSpriteBgUsrPot[GAME_PLAYER];			// 用户池下注背景 
	cocos2d::Label *m_pLabelUsrPotValue[GAME_PLAYER];			// 用户池下注数字

	std::vector<double> m_vecSidePotValue;						// 动画完成后的边池数值
	std::vector<SidePot*> m_vecPSidePot;						// 所有创建过的边池（含隐藏的）
	unsigned m_uCurSidePotCount;								// 当前的边池数量

	TextureNumber *m_pWinNumber[GAME_PLAYER];					// 赢取的数额标志

	ActionBar *m_pActionBar;
	PreActionBar *m_pPreActionBar;
	BrightActionBar *m_pBrightActionBar;
	WinningRaking * m_pWinningRaking;

	cocos2d::ui::Button *m_pMenuItemAutoSitDown; // 自动加入
	cocos2d::ui::Button *m_pMenuItemSitDown; // 加入

	cocos2d::ui::Text *m_pLabelWaitPlayerCount; // 已有(x人)等待
	AnimateViewer *m_pAniWaitSitDown; // 点击自动左下的按钮之后，提示的等待左下的动画
	cocos2d::ui::Text *m_pLabelWaitSitDown;//点击自动左下的按钮之后，提示的等待左下的文字
	




    cocos2d::Sprite *m_pSpriteInviteFlag;
    bool             m_bIsReceiveInvite;
    std::string m_strNickName;
    std::vector<UsrHandCard> m_UsrHandCard;
    WaitBeginGame *m_WaitBeginGame;
	//表情动画
	AnimationManager* m_aniTools[12] ;
	AnimationManager* m_aniToolsRuhua ;
	cocos2d::ui::Text* m_pLabelTableDesc;


	//聊天位置
	cocos2d::Vec2 s_ptChatTip[GAME_PLAYER] ;

	//牌局中 牌的坐标
	cocos2d::Vec2 s_ptPublicCard[5] ;

	//原始玩家坐标
	cocos2d::Vec2 s_ptUserViewDefault[GAME_PLAYER] ;
	//入场 桌子外面
	cocos2d::Vec2 s_ptUserViewOutTable[GAME_PLAYER] ;

	cocos2d::Vec2 s_ptUserViewDefaultCenter[GAME_PLAYER] ;
	cocos2d::Vec2 s_allinChizi[GAME_PLAYER];

	cocos2d::Vec2 s_ptDispatchedCard[GAME_PLAYER*2] ;

	//D的位置
	cocos2d::Vec2 s_ptDPosition[GAME_PLAYER] ;

	// 用户池下注背景 
	cocos2d::Vec2 s_ptBgBet[GAME_PLAYER] ;
	//用户下注数字
	cocos2d::Vec2 s_ptLabelBet[GAME_PLAYER];
	// 用户下注筹码-从用户池飞向边池
	cocos2d::Vec2 s_ptJettonDst[GAME_PLAYER];

	cocos2d::Vec2* s_ptPots[GAME_PLAYER];

	cocos2d::Vec2 s_ptMyCard[2];

	cocos2d::Vec2 s_ptFoldingCard[GAME_PLAYER] ;


	TableType m_curTableType ;
	TableType m_preTableType ;

	//SNG
	bool isSngAutoMove ;//自动托管
	bool isSngAutoMoveFoever ;//永远自动托管
	bool m_autoTrust ;

	cocos2d::ui::Layout* m_sngAutoLayer ;
	cocos2d::ui::Button* m_buttomSngAuto ;


	cocos2d::ui::ImageView* m_imgtitle ;

	Sprite* m_pNormalSpriteBG ;//背景底
	Sprite* m_spritetableTitle ;
	cocos2d::Sprite* m_spriteSngAuto ;

	cocos2d::ui::ImageView* m_img_tableLeft ;//编辑器中 桌子左边
	cocos2d::ui::ImageView* m_img_tableRight ;//编辑器中 桌子右边

	cocos2d::Sprite* m_sprite_tableLeft ;//重设为图片
	cocos2d::Sprite* m_sprite_tableRight ;//重设为图片


	cocos2d::ui::Button* m_buttonSngRank ;//排名  sng mtt公用

	cocos2d::ui::Layout* m_layerRightUpNormal ;//右边普通场按钮
	cocos2d::ui::Layout* m_layerRightUpSng ;//右边sng场
	cocos2d::ui::Text* m_text_sngRank ;//sng当前排名
	cocos2d::ui::Text* m_text_sngRankbg ;

	cocos2d::ui::Button *m_sngReturn;//退出

	cocos2d::Sprite* m_sprtieRankView[9] ;//sng失败后显示的排名

	cocos2d::ui::Layout* m_text_sngScrollTips;//滚动提示
	cocos2d::ui::Layout* m_text_sngNeedRoleTips;//sng需要人的提示
	cocos2d::ui::Text*   m_text_curSngRoleCount;//sng需要人的提示

	int m_curPlayerCount ;
	//滚动提示
	//cocos2d::Vector<MoveLabel*>     m_vecLineLabel;
	bool m_isScrollTextMove ;
	MoveLabel* m_curLineLabel ;
	float m_curScrollTextWidth ;
	float m_MaxScrollTextWidth ;

	cocos2d::ClippingNode* m_cliper ;

	//排名
	cocos2d::Sprite* spriteRank[9] ;
	//聊天表情
	AnimationManager* m_biaoqingAni[GAME_PLAYER][2];
    ShowLabelInLineSequence     *m_RewardBubble;

	bool m_sngInitialized;


	//mtt
	cocos2d::ui::Layout*  m_layer_GameTable ;

	cocos2d::ui::Button*  m_bt_mtt_rebuy ;
	cocos2d::ui::Button*  m_bt_mtt_addon ;
	cocos2d::ui::Layout*  m_layer_mtt_time ;
	cocos2d::ui::Text*	  m_text_mtttime[4] ;
	cocos2d::ui::Text*	  m_text_mttRankF ;
	cocos2d::ui::Text*	  m_text_mttRankAfter ;

	cocos2d::Node*	m_node_denvlang ;
    MttRankView     *m_MTTRankView;
	int m_mtttimeShow[4] ;
	int m_mttTime ;
    std::vector<MissionInfo>      m_vecMissionInfo;//当前场次应该显示的所有成就任务列表


	//私密房
	cocos2d::ui::Layout		*m_layout_PrivateTable ;
	cocos2d::ui::Button		*m_bt_Private_startOrEndGame ; //开始结束游戏
	cocos2d::ui::Button		*m_bt_Private_Box;//盒子
	cocos2d::ui::Button		*m_bt_Private_task;//任务
	cocos2d::ui::Button		*m_bt_Private_shop;//商城

	cocos2d::ui::Layout		*m_layout_Private_btBox;
	cocos2d::ui::Button		*m_bt_Private_InvateFriends; //邀请好友
	cocos2d::ui::Button		*m_bt_Private_Share;//分享

	cocos2d::ui::Button		*m_bt_Private_touch;//遮罩


	cocos2d::ui::Button		*m_bt_Private_RoomInfo;//房间信息
	cocos2d::ui::ImageView	*m_image_Private_roomInfo;
	
	cocos2d::ui::Text		*m_text_private_startOrEndGame ;

public:
	void callUserControl(bool flag,cocos2d::Node* node = nullptr) ;
	std::vector<cocos2d::Node* > m_vecLayer ;
	int m_CurTouchChairId ;
};



#endif