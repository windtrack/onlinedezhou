#ifndef _GAMEDATA_H_
#define  _GAMEDATA_H_


#include "GameCommon.h"
#include "GameTable.h"
#include "MoveLabel.h"
#define DataMgr GameData::getInstance()




class GameData
{
public:
	static GameData* getInstance() ;


	//皮肤按钮
	void readShowSkinBt(unsigned uid) ;
	void setHasTeachSkinButton(unsigned uid) ;
    bool getShowSkinButton(unsigned uid) ;
	void initGameTablePos(cocos2d::Node*& root);


	inline void setGameWidth(float width){ S_WIDTH = width ;};
	inline void setGameHeight(float height){ S_HEIGHT = height;} ;

	inline float getGameWidth(){return S_WIDTH;} ;
	inline float getGameHeight(){return S_HEIGHT;} ;

	void setGameScale(float scale){ s_GameScale = scale ;} ;
	inline float getGameScale(){return s_GameScale ;} ;


	inline bool getSoundOn(){return s_inGameTableSoundOn;} ;
	inline void setSoundOn(bool flag){ s_inGameTableSoundOn = flag ;} ;

	void setVibrate(bool flags);
	bool getVibrate();

    void updateCurMessageReadTime() ;
	int getMessageLastReadTime() ;
	inline bool getHasNewMessage(){return s_hasNewMessage ;} ;
    void setNewMessageTime(long time) ;



	//第一次创建俱乐部并进入我的俱乐部界面控制
	void setFirstCreatFamily(unsigned uid,bool flag) ;
	void setFistInMyfamily(unsigned uid,bool flag) ;

	 bool getIsFirstCreatFamily(unsigned uid);
	 bool getFirstInMyFamily(unsigned uid) ;


	 void setPauseNetWork(bool flag) ;
	 inline bool getpauseNetWork(){return pauseNetWork ;} ;

	 inline void setPauseNetWorkClear(bool flag) {clearNetWorkInfo = flag ;};
	 inline bool getClearNetWorkInfo(){return clearNetWorkInfo ;} ;

	 inline void setOpenUserInfoFormGameHall(bool flag) {s_openUserInfoFormGameHall = flag ;};
	 inline bool getOpenUserInfoFormGameHall(){return s_openUserInfoFormGameHall ;} ;

	 void readIpAddress(std::string ip) ;
	 inline std::string getIpAddress(){return m_ipaddress ;} ;

	 void readSngOnline(unsigned uid) ;
	 inline bool getHasShowSngOnline(){ return s_showSngOnline ;} ;
	 void saveHasShowSngOnline(unsigned uid) ;


	 void inline setUpdateVersion(bool flag){s_needUpdate =flag ;} ; 
	 bool inline getNeedUpdate(){ return s_needUpdate ;} ;

	 inline void setNewVersionCode(std::string vercode){s_versionCode = vercode ;} ;
	 inline std::string getNewVersionCode(){return s_versionCode ;} ;

	 inline void setUpdateContent(std::string updateInfo){s_updateContent.push_back(updateInfo) ;} ;
	 inline std::vector<std::string> getUpdateContent(){return s_updateContent ;} ;

	 inline void setGetMttFloatMathcInfo(bool flag){m_hasLoadMttFloatMatchInfo = flag ;} ;
	 inline bool getHasLoadMttFloatMatchInfo(){return m_hasLoadMttFloatMatchInfo ;} ;

	 inline void setHasRequestMatch(bool flag){m_hasRequestMatch = flag ;} ;
	 inline bool getHasRequestMatch(){return m_hasRequestMatch ;} ;

public:
    void addGameTips(MoveLabel* label);

	inline void removeGameTips(MoveLabel* label){ m_vecMoveLineLabel.eraseObject(label); } ;

	inline cocos2d::Vector<MoveLabel*> getAllMoveLabel(){return m_vecMoveLineLabel ;} ;

	inline void cleanMoveLable(){m_vecMoveLineLabel.clear() ;} ;

	inline bool hasMoveLable(){return  m_vecMoveLineLabel.size()>0 ;} ;

	MoveLabel* getOneMoveLable(); 


    void readTeach(int index) ;
    void saveTeach(int index) ;

public:

	std::string m_ipaddress ;

	UserPos m_usePosInfo[GAME_PLAYER];
	
	//聊天位置
	 Vec2 s_ptChatTip[GAME_PLAYER] ;

	//牌局中 牌的坐标
	 Vec2 s_ptPublicCard[5] ;

	//原始玩家坐标
	 Vec2 s_ptUserViewDefault[GAME_PLAYER] ;
	//入场 桌子外面
	 Vec2 s_ptUserViewOutTable[GAME_PLAYER] ;

	 Vec2 s_ptUserViewDefaultCenter[GAME_PLAYER] ;
	 Vec2 s_allinChizi[GAME_PLAYER];


	 Vec2 s_ptDispatchedCard[GAME_PLAYER*2] ;

	//D的位置
	 Vec2 s_ptDPosition[GAME_PLAYER] ;

	// 用户池下注背景 
	 Vec2 s_ptBgBet[GAME_PLAYER] ;
	//用户下注数字
	 Vec2 s_ptLabelBet[GAME_PLAYER];
	// 用户下注筹码-从用户池飞向边池
	 Vec2 s_ptJettonDst[GAME_PLAYER];

	 Vec2 s_ptMyCard[2];

	 Vec2* s_ptPots[GAME_PLAYER];

	 Vec2 s_ptFoldingCard[GAME_PLAYER] ;

	 Vec2 s_ptOpen[GAME_PLAYER];

	 Vec2 s_ptPlayer[GAME_PLAYER];
     //底池
     Vec2 s_ptDiChi;

private:
	//牌桌内滚动数据
	cocos2d::Vector<MoveLabel*>      m_vecMoveLineLabel;

private:
	
	static GameData* _instance ;


	bool s_showSkinButton ;

	bool s_inGameTableSoundOn ;


	int s_messageLastReadTime ;//消息阅读时间
	int s_messageNewMessageTime;//最新消息时间

	bool s_hasNewMessage ;//是否有新消息


	bool s_firstCreatFamily ;
	bool s_firstInMyGamily ;


	bool pauseNetWork ;
	bool clearNetWorkInfo ;

	bool s_openUserInfoFormGameHall ;

	bool s_showSngOnline ;

	bool s_needUpdate ;

	std::string s_versionCode ;
	std::vector<std::string> s_updateContent ;

	bool m_hasLoadMttFloatMatchInfo ;
	bool m_hasRequestMatch ;


	int m_channelID ;
public:
	 float S_WIDTH;
	 float S_HEIGHT ;
	 float s_GameScale ;
};



#endif
