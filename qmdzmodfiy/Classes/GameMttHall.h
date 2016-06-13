
#ifndef _GAMEMTTHALL_H_
#define  _GAMEMTTHALL_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
//#include "MttTypeSelect.h"
struct GameMttTableInfo ;
class HttpSprite ;
class FrameScale9SpriteButton;
class LayerButton;
class MttTypeSelect ;

class MttRule ;

class MttJoinMatchTips ;
class FrameSprite;
class AnimationManager ;
//
//enum Mtt_Type
//{
//	Match_Type_All_0=0,//全部
//	Match_Type_GameMoney_1=1,//打金赛
//	Match_Type_FULI_2=2,//福利赛
//	Match_Type_Qbi_3=3,//q币
//	Match_Type_SHIWU_4=4,//实物
//	Match_Type_WEPT_5=5,//门票赛
//	//Match_Type_FUDong=6,//浮动赛
//	Match_Type_W_7=6,//门票赛
//	Match_Type_W_8=7,//门票赛
//
//};

enum MatchState
{
	MatchState_IsSigining=0,
	MatchState_hasSigin,
	MatchState_GameOver
};


#define  SinglePage  8

class GameMttHall :public GameDialog ,public cocos2d::extension::ScrollViewDelegate
{
public:
	CREATE_FUNC(GameMttHall) ;
	bool init() ;
	void onSetVisible(bool bVisible) ;
	void showBlack(bool flag) ;
	virtual void onKeyEscPressed() override;


public:

	void updateMttHallList(float dealTime) ;

	void updatePageCountAndView(std::vector<GameMttTableInfo> vecTable, bool updateTime = false,unsigned pageIndex=0) ;//更新页面数量

	void updateVeiw(std::vector<GameMttTableInfo> vecTable,unsigned pageIndex=0) ;

	void updateItemList( GameMttTableInfo& followInfo) ;

	void updateItemTime(float DelayTime);

	void showMttTypeSelect(bool flag) ;//赛事类型选择

	void showQuitMatch(bool flag) ;//退出比赛提示界面

	void updateMatchViewByGameType(int type,bool updateTime = false,unsigned pageIndex=0) ;//通过游戏类型更新列表

	void updateCurUsedMatchInfo() ;//更新当前使用的赛事信息

	void updateItemByTableID(unsigned tableid ,unsigned state,std::string title ="",std::string reason="",bool delayRegis=false ,int delayRegistTime=0,bool showCountDown=false) ;//更新列表信息

	void removeItemByTableID(unsigned tableid) ;//删除item

	int checkTimeType(GameMttTableInfo &tableInfo) ;

	void updateMttHallInfo() ;

	void updateTicket() ;

	void updateMatchPlayerNum(unsigned tableid ,unsigned  num) ;

	inline void setEnterFormHall(bool flag){m_openByHall =flag; } ;

	inline bool getEnterFormHall(){return m_openByHall;} ;

	void updateFixTime(float time) ;

	void updateCurViewList(float time) ;

	void updateCurViewListByQuitMatch() ;


	inline void setShowItems(bool flag){m_showItems = flag ;} ;
protected:
	void showMttRule(bool flag) ;

	void showMttMatchState( GameMttTableInfo& followInfo) ;
private:

	inline void setCurMttType(int type){m_curGameType = type;} ;

	inline int getCurMttType(){return m_curGameType ;} ;


	void setCurMatchState(MatchState state) ;

	inline MatchState getCurSelectState(){return m_curSelectState ;} ;

	void OnUpdateMyData(unsigned uDataType) ;

	void initHallInfo() ;

	void onMenuItemBottomSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type);

	void callHallAction() ;//入场动画

	void menuClickSigin(  GameMttTableInfo& followInfo) ;//报名回调

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) ;

	void showTableState( GameMttTableInfo& followInfo) ;//显示牌局状态
	
	void updateMenuIndex(unsigned index) ;//导航按钮颜色与状态

	void updatePageButtonShow() ;//上一页下一页状态更新

	void updateGameTypeCount() ;//更新当前有哪些赛事

	void moveItemToFirst(GameMttTableInfo table) ;//将赛事移动到第一页

	void updateMatchInfoByState() ;

    void createGuideTips();
private:
	MttTypeSelect* m_mttTypeSelect ;
	MttRule* m_mttrule ;


	
public:

	
	struct MttTableItem
	{
		FrameSprite			    *m_pSpriteBgItem;			    // 滚动视图行的背景	
		cocos2d::Sprite			*m_pAniBg;			   
		cocos2d::Sprite			*m_pAni;			   
		cocos2d::Sprite			*m_pAniUpScale;			   
		LayerButton				*m_pBtDetail;
		HttpSprite				*m_httpIcon ;
		cocos2d::Label			*m_labelName ;
		cocos2d::Sprite			*m_spriteRoleCount;	
		cocos2d::Label			*m_LableroleCount ;
		cocos2d::Sprite			*m_spriteReBuy;
		cocos2d::Sprite			*m_spriteAddon;
		cocos2d::Sprite			*m_spriteFloatMatch;
		cocos2d::Sprite			*m_spriteDelayMatch;

		cocos2d::Label			*m_labelIndex ;
		unsigned				m_TimeType ;
		cocos2d::Sprite			*m_spriteTimeBG ;  //未开始背景
		cocos2d::Sprite			*m_spriteJijiangkaishiBg ;//即将开始背景
		cocos2d::Sprite			*m_spriteYanShikaishiBg ;//延迟开始背景
		cocos2d::Label			*m_lable_titleDay ; //今天 日期等
		cocos2d::Label			*m_lable_TimeDes ; //即将开始
		cocos2d::Label			*m_lable_statrTime ;//未开始显示具体开赛时间
		cocos2d::Label			*m_lable_JiJiangCountDown[4] ; //倒计时
		cocos2d::Sprite			*m_spriteMoney ;
		cocos2d::Label			*m_lableMoney ;
		cocos2d::Sprite			*m_spriteFreesticket ;
		cocos2d::Label			*m_labelFreeStickCount ;
		cocos2d::Label			*m_labelMyStickCount ;
		FrameScale9SpriteButton *m_pBtJiaRu;					//加入按钮
		cocos2d::Label			*m_labelJiaru;					//按钮文字
		cocos2d::Label			*m_wait;//等待开始
		cocos2d::Label			*m_labelNotStart ;		//未开始
		cocos2d::Label			*m_labelNotStartDes ;   //赛前多久开始
		AnimationManager		*m_aniBtJiaru;

		unsigned m_tableId ;
		unsigned m_index ;    
		unsigned m_indexInPage ; 
		bool updateTime(float DelayTime) ;
		float m_time ;
		int m_timeShow[4] ;

		bool m_needShow ;
		bool m_CountDown ;

		std::vector<cocos2d::Node*> m_vecIcon ;
	};
	//获取列表显示
	MttTableItem* GetMttTableListItem(unsigned tableId);
	void UpdateItem(GameMttHall::MttTableItem *item, const GameMttTableInfo& followInfo);


private:
	std::vector<MttTableItem*>		m_vecSngTableListInfoNew;
	cocos2d::Sprite* m_spriteIcon[5] ;
	cocos2d::extension::ScrollView* m_pScrollView ;
	cocos2d::Size sizeListView;

	cocos2d::Node* s_uiroot ;
	cocos2d::ui::Layout* s_layoutTable ;
	cocos2d::ui::Layout *m_pSpriteBgTable;
	cocos2d::ui::Layout *m_rightupButton;
	cocos2d::ui::Layout *m_leftupButton;
	cocos2d::ui::Layout *m_middleUpButtonLayer;
	cocos2d::ui::Layout *m_middleDownButtonLayer;
	cocos2d::ui::Layout *m_SngTips;
	cocos2d::ui::Layout *m_HeroInfo;
	//按钮
	cocos2d::ui::Button* m_buttonBack ;
	cocos2d::ui::Button* m_buttonhelp;
	cocos2d::ui::Button* m_buttonrank;
	cocos2d::ui::Button* m_buttonshop;
	cocos2d::ui::Button* m_buttonkefu;

	//中间上面
	cocos2d::ui::Button* m_buttonPageUp;
	cocos2d::ui::Button* m_buttonPageDown;
	cocos2d::ui::Button* m_button_WillSiginUp;
	cocos2d::ui::Button* m_button_hasSiginUp;
	cocos2d::ui::Button* m_button_gameover;

	cocos2d::ui::Text* m_Text_WillSiginUp;
	cocos2d::ui::Text* m_Text_hasSiginUp;
	cocos2d::ui::Text* m_Text_gameover;

	//中间下面
	cocos2d::ui::Button* m_button_MathcType;
	cocos2d::ui::Button* m_button_MatchTime;
	cocos2d::ui::Button* m_button_SiginTips;

	//遮罩
	cocos2d::ui::Button* m_buttonTouchAll;
	cocos2d::ui::Button* m_buttonTouchUp;

	//人物信息
	cocos2d::ui::Text* m_textName ;
	cocos2d::ui::Text* m_textMoney ;
	cocos2d::ui::Text* m_textrank ;
	cocos2d::ui::Text* m_textDashiRank ;
	HttpSprite* m_spriteHero ;

	//背景
	cocos2d::ui::ImageView* m_img_tableLeft ;//编辑器中 桌子左边
	cocos2d::ui::ImageView* m_img_tableRight ;//编辑器中 桌子右边
	cocos2d::Sprite* m_spriteHallBg[2] ;
	
	cocos2d::ui::ImageView	*m_imgPuke ;  //左下方一个扑克图片动画
	//遮罩显示
	cocos2d::ui::Layout	*layerClip ;

	//赛事类型背景
	//cocos2d::ui::ImageView* m_imgTypeBG ;
private:
	cocos2d::LayerColor  *m_layerColorBg;
	unsigned m_curPageIndex ;
	unsigned m_maxPageCount ;
	unsigned m_layerAph ;
	MatchState m_curSelectState ;// 当前选中的类型
	MatchState m_preSelectState ;// 当前选中的类型

	int m_curGameType ;// 当前显示的游戏类型
	bool m_useSort ;  //时候使用排序
	bool m_hasSort ;  //是否排序过  切状态的时候清除时间排序
	unsigned m_curSelectIndex ;
	std::vector<GameMttTableInfo> m_vecFirstStateInfo ;//一级数据  状态 报名 已报名 报名结束
	std::vector<GameMttTableInfo> m_vecCurUesdInfoByType ; //当前使用的大厅信息类型排序后  二级数据 类型
	std::vector<GameMttTableInfo> m_vecCurUesdTableInfo ; //当前使用的大厅信息 二级原始数据 
	std::vector<GameMttTableInfo> m_vecCurUesdInfoByTime ; //当前使用的大厅信息时间排序后  时间排序

//逻辑
private:
	bool m_openByHall ;
	bool m_isInAction ;
	bool m_showItems ;
private:
	void doGuoDong(bool flag,cocos2d::Node* node) ;

};





#endif
