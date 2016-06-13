#ifndef __GAMETABLEPLAYER_H__
#define __GAMETABLEPLAYER_H__

#include "GameDialog.h"
#include "jsonrpc/json/value.h"
#include "GameTable.h"
#include "ChairViewConvertor.h"
class AnimationManager;
class ActionBar;
struct TableBaseInfo
{
	std::string m_strTableName;
	unsigned m_uChairID, m_uBankerChairID, m_uMaxPlayer, m_uPotLimitTime;
	double m_dBigBlind, m_dSmallBlind, m_dAnti;
};

struct GameScenePublicCard
{
    unsigned m_uScene;
    std::vector<unsigned char> m_vecPublicCard;
};

class GameTableViewer : public cocos2d::Layer
{
public:
	CREATE_FUNC(GameTableViewer);
	void Clear();
	void SetTableInfo(const TableBaseInfo& tableBaseInfo);
	void SetTableName(const std::string& strTableInfo);
    void SetGameID(const std::string& strGameID);

	void SetDPosition(unsigned uChairID);
	void SetSidePot(const std::vector<double>& vecCurAllPot);
	void SetAllPot(double dAllPot);
	void SetUser(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID, unsigned uVoice, unsigned uSkin, bool bAni = false);
	void SetUsrBetedJetton(unsigned uChairID, double dBetCash);
	void SetUsrCash(unsigned uChairID, double dCash);
	void SetUsrStatus(unsigned uChairID, unsigned uStatus);
	void SetPublicCard(std::vector<unsigned char> vecPublicCard);
	void SetHandCard(unsigned char cbHandCard[2]);

    bool PreDispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard);
    void StartDispatchPublicCard();
    void DishpatchPublicCard(unsigned uIdx, unsigned char cbCardData, bool bDelay);
    void OnPublicCardDispatched(unsigned uIdx);

	void UsrSitDown(unsigned uChairID, const std::string& strNickName, const std::string& strIcon, double dCash, unsigned uUID, unsigned uVoice,unsigned uSkin, bool bAni = true);
	void UsrStandUp(unsigned uChairID, bool bAni = true);
	
	void UsrPlaceJetton(unsigned uChairID, double dBetValue);

	void DispatchHandCard(const std::vector<unsigned>& vecDispatchingChairID, unsigned char cbMyCardData[2]);
	void DispatchPublicCard(const std::vector<unsigned char>& vecDispatchingPublicCard);
	
	void WaitUsrAction(unsigned uChairID);
	void UsrCheck(unsigned uChairID);
	void UsrCall(unsigned uChairID, double dBetValue);
	void UsrRaise(unsigned uChairID, double dBetValue);
	void UsrAllIn(unsigned uChairID, double dBetValue);
	void UsrFoldCard(unsigned uChairID);

	void ShowMeHasActioned();
	void ShowMeHasWin();

	void CollectUsrBetToPot(const std::vector<double>& vecCurAllPot);
	void ActivateUserStatus(unsigned uS, unsigned uExceptS);
	void OpenUsrCard(const std::vector<unsigned>& vecOpenCardUsr, const std::vector<unsigned>& vecOpenCardData);
	void ShowUsrWinInfo(unsigned uWinType, const std::vector<unsigned>& vecWinChairs, bool bWaitOpenCard, const std::vector<UsrWinInfo>& vecUsrWinInfo);
    void SetDispatchHandCardVisible(unsigned idx);
    void GetPublicCard(GameScenePublicCard scenceItem);
    void SetCardType(unsigned scene);
    void ClearPublicCard();
	void SetCallback(std::function<void()> callfun1, std::function<void()> cf2, std::function<void()> cf3);
    void StopWinAction();
    void ShowMaxCardType(bool IsVisible);



	void setTableType(TableType type) ;
	TableType getTableType() ;
private:
	TableType m_curTableType ;
	TableType m_preTableType ;
	Sprite* m_pNormalSpriteBG ;//背景底
	Sprite* m_spritetableTitle ;

	cocos2d::ui::ImageView* m_img_tableLeft ;//编辑器中 桌子左边
	cocos2d::ui::ImageView* m_img_tableRight ;//编辑器中 桌子右边

	cocos2d::Sprite* m_sprite_tableLeft ;//重设为图片
	cocos2d::Sprite* m_sprite_tableRight ;//重设为图片

	cocos2d::ui::Button* m_buttonSngRank ;//排名

	cocos2d::ui::Layout* m_layerRightUpNormal ;//右边普通场按钮
	cocos2d::ui::Layout* m_layerRightUpSng ;//右边sng场
	cocos2d::ui::Text* m_text_sngRank ;//sng当前排名

	cocos2d::ui::Button *m_sngReturn;//退出

	cocos2d::Sprite* m_sprtieRankView[9] ;//sng失败后显示的排名

	cocos2d::ui::Layout* m_text_sngScrollTips;//滚动提示
	cocos2d::ui::Layout* m_text_sngNeedRoleTips;//sng需要人的提示
	cocos2d::ui::Text*   m_text_curSngRoleCount;//sng需要人的提示

	cocos2d::ui::Layout* m_sngAutoLayer ;
	cocos2d::ui::Button* m_buttomSngAuto ;

	cocos2d::ui::ImageView* m_imgtitle ;

	cocos2d::ui::Button* m_pMenuItemPopMenu ;
protected:
	bool init();

	void SetUsrBeting(unsigned uChairID);
	void SetUsrBetFinished(unsigned uChairID);
	unsigned GetUsrBeting();
	void SetAllBetFinishedCB(std::function<void()>);
	void SetJettonCollecting(bool bCollecting);
	bool IsJettonCollecting();
	void SetJettonCollectFinishedCB(std::function<void()>);
	
	void DispatchPublicCardImpl(const std::vector<unsigned char>& vecDispatchingPublicCard);
	void SetAllPublicCardDispatchedCB(std::function<void()>);
	void DispatchWinJettonToUsr(const std::vector<UsrWinInfo>& vecUsrWinInfo);

	void onScheduleShakeHandCard(float fDelta);
	void onScheduleFoldCard(float fDelta);

	void OnScheduleDispatchHandCard(float fDelta);
	void OnScheduleDispatchPublicCard(float fDelta);
	void OnScheduleOpenUsrCardFinished(float fDelta);
	void OnScheduleWaitUsrCardOpenedAndAllPublicCardDispatched(float fDelta);
	void OnScheduleDispatchWinJettonToUsr(float fDelta);
    void SetPublicCardDispatchedCB(std::function<void(unsigned uAllPublicCard)>);

	void PlayUsrActSound(unsigned uChairID, unsigned uAct);

	void GetJettonIdxs(double dValue, std::vector<int>& vecJettonIdx);
	unsigned GetJettonIdx(double dValue);
	cocos2d::Vec2 GetSidePotPosition(unsigned uPotCount, unsigned uPotIdx);

	cocos2d::Sprite *GetSpriteDispatchingCard(unsigned uRoundId, unsigned uChairId);
	cocos2d::Sprite *GetHandCard(unsigned uChairID, unsigned uCardIdx);
	FrameSprite *GetSpriteMyCard(unsigned uCardId);
	cocos2d::Sprite *GetSpriteFoldingCard(unsigned uChairId);
	FrameSprite *GetSpritePublicCard(unsigned uCardId);
	UserView* GetUserView(unsigned uChairID);
	TextureNumber *GetUserWinNumber(unsigned uChairID);

    void OnMenuAction(Ref *pSend, ui::Button::TouchEventType type);//
private:
    struct UserInfo
    {
        std::string m_strName;
        std::string m_strIcon;
        double m_dCash;
        unsigned m_uUID;
        unsigned m_uSkin;
    };
public:
    cocos2d::ui::Button     *m_btnQiPai;
    cocos2d::ui::Button     *m_btnGenZhu;
    cocos2d::ui::Button     *m_btnJiaZhu;
    cocos2d::ui::Text       *m_textGenZhu;
    cocos2d::ui::ImageView  *m_imgCardType;
private:
    std::vector<GameScenePublicCard> m_vecScencePublicCard;
	cocos2d::Node* s_uiRoot ;
	cocos2d::Node* s_gameLayer ;
	cocos2d::Node* m_rootUser[GAME_PLAYER];
    UserInfo m_usrInfo[GAME_PLAYER];

	TableBaseInfo m_tableBaseInfo;
	unsigned m_uDispatchingIdx;
	std::vector<unsigned> m_vecDispatchingChairID;
	unsigned char m_cbMyCardData[2];
	std::vector<unsigned char> m_vecPublicCardData;
	std::deque<unsigned char> m_deqDispatchingPublicCardData;
	bool m_bAllPublicCardDispatched;

	unsigned m_uBetingCount;
	std::function<void()> m_funCBAllBetingFinished;
    std::function<void(unsigned)> m_funCBPublicCardDispatched;

	bool m_bCollecting;
	std::function<void()> m_funCBCollectingFinished;

	std::function<void()> m_funCBAllPublicCardDispatched;

	bool m_bOpenCarding;
	bool m_bOpenCardFinished;
	unsigned m_uWinType;
	std::vector<unsigned> m_vecWinChairs;
	std::vector<UsrWinInfo> m_vecUsrWinInfo;

	// view
	cocos2d::ui::Text *m_pLabelTableInfo;
    cocos2d::ui::Text *m_pLabelGameID;

	JettonFactory *m_pJettonFactory;
	
	Dealer *m_pDealer;											// 美女荷官

	double m_dAllPlacedJettonValue;
	//cocos2d::Label *m_pLabelAllPlacedJettonValue;				// 底池数量
	//cocos2d::Sprite* m_pSpriteDispatchingCard[GAME_PLAYER*2];	// 18张正在发送的小牌
	//cocos2d::Sprite* m_pSpriteFoldingCard[GAME_PLAYER];			// 9张弃牌
	//UserView *m_pUsers[GAME_PLAYER];							// 9个玩家视图
	//cocos2d::Sprite *m_pSpriteDFlag;							// D标志
    //私密房
    cocos2d::ui::Layout		*m_layout_PrivateTable ;
    cocos2d::ui::Button		*m_bt_Private_RoomInfo;//房间信息

	UserPos m_usePosInfo[GAME_PLAYER];

	cocos2d::ui::Text *m_pLabelAllPlacedJettonValue;				// 底池数量
	cocos2d::Sprite* m_pSpriteDispatchingCard[GAME_PLAYER*2];	// 18张正在发送的小牌
	cocos2d::Sprite* m_pSpriteFoldingCard[GAME_PLAYER*2];			// 9张弃牌
	UserView *m_pUsers[GAME_PLAYER];							// 9个玩家视图
	cocos2d::Sprite *m_pSpriteDFlag;							// D标志



	cocos2d::Sprite *m_pSpriteShowMaxCard;
	//cocos2d::Animate *m_pAniShowMaxCard[4];
	AnimationManager *m_pAniShowMaxCard[4]; 
	cocos2d::Sprite *m_pSpriteShowMeWin;
	//cocos2d::Animate *m_pAniShowMeWin;
	AnimationManager* m_pAniShowMeWin;
	FrameSprite *m_pSpritePublicCard[5];					// 5张公牌
	FrameSprite *m_pSpriteMyCard[2];						// 2张自己的手牌
	FrameSprite *m_pSpriteMaxCardType;
	CardTypeView *m_pCardTypeView;






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

    std::function<void()> funQiPai;
    std::function<void()> funGenZhu;
    std::function<void()> funJiaZhu;
};

struct PlayBackPlayerInfo
{
	unsigned m_uChairID;
	unsigned m_uUserID;
	std::string m_strIcon;
	std::string m_strName;
	unsigned m_uUserLastAction;
	double m_dUserBeted;
	double m_dRestCash;
    unsigned m_uSkin;
	unsigned m_uVoice;
};

// 局点场景恢复消息
struct GameRecordScence
{
	double m_dTick;
	unsigned m_uGamePoint; // 0-3 开局、翻牌、转牌、河牌 
	std::vector<PlayBackPlayerInfo> m_vecPlayer;
	std::vector<unsigned char> m_vecPublicCard;
	//std::vector<unsigned char> m_vecHandCard;
	double m_dAllPot;
	std::vector<double> m_vecSidePot;
	unsigned m_uNextCmdId;

	//bool operator < (const GameRecordScence& rv) const
	//{
	//	return m_uGamePoint < rv.m_uGamePoint;
	//}
};

inline bool operator < (const GameRecordScence& lv, const GameRecordScence& rv)
{
	return lv.m_uGamePoint < rv.m_uGamePoint;
}

//inline bool operator < (const GameRecordScence& lv, const GameRecordScence& rv)
//{
//	return lv.m_uGamePoint < rv.m_uGamePoint;
//}


class GameTablePlayer : public GameDialog
{
public:
	CREATE_FUNC(GameTablePlayer);
	void StartPlay(const Json::Value& gameRecord);

private:
	// 游戏命令
	struct GameRecordCmdItem
	{
		unsigned m_uID;
		double m_dTick;
		unsigned m_uCmd;
		Json::Value m_cmdData;

		bool operator < (const GameRecordCmdItem& rv) const
		{
			return m_uID < rv.m_uID;
		}
	};


	bool init();
	void ClearTable();
	void JumpToPoint(unsigned uGamePoint);
	void OnCmd(const GameRecordCmdItem& cmd);
	virtual void onSetVisible(bool bVisible) override;
	void onMenuItemPlay();
	void onMenuItemPause();
	void onMenuItemLeave();
	void onMenuItemJumpToPoint(cocos2d::Ref*);// 0-3 开局、翻牌、转牌、河牌

	void onScheduleProgress(float fDelta);
	void onSchedultGetNextCmd(float fDelta);


private:




private:

	std::string m_strRecordID;
	ChairViewConvertor m_chairViewConvertor;
	GameTableViewer *m_pGameTable;
	cocos2d::ProgressTimer *m_pTimeLine;
	cocos2d::MenuItem *m_pMenuItemGamePoint[4];
	cocos2d::MenuItem *m_pMenuItemPlay;
	cocos2d::MenuItem *m_pMenuItemPause;
	cocos2d::Label *m_pLabelAllTime;
	double m_dAllTimeInMs;
	int m_iCurCmd;
	double m_dNextCmdWaitTime;
	bool m_bWaitingCmd;
	std::vector<GameRecordCmdItem> m_vecCmdItem;
	std::vector<GameRecordScence> m_vecScence;
    std::vector<unsigned char> m_vecHandCard;
};



#endif