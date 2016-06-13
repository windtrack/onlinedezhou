#ifndef  __ACHIEVEMENTUI_H__
#define  __ACHIEVEMENTUI_H__
#include "cocos2d.h"
#include "GameDialog.h"

class FrameSprite;
class MenuItemSpriteWithLabel;
class HttpSprite;
class FrameSpriteButton;
class LayerButton;
struct AchievementInfo;

enum MyAchievementType		// 按钮的4种状态ID
{
	ERICHERTHANAKING,		// 富可敌国
	EHARDTOGETRICH,			// 勤劳致富
	ETHEROADOFGAMBLERS,		// 赌神之路
	EGODOFTHECOMPETIVIVE,	// 竞技之神
    TEXASPOKERKING          // 德州之王
};

class AchievementUi : public GameDialog
{
public:
	CREATE_FUNC(AchievementUi);
	void UpdateAchievementList();
	// 更新成就函数
	void UpdateAchievementList(unsigned m_uRecordMenuCurrentStatusId);

	// 成就列表下按钮的所有回调函数
	void AllMenuCallback(Ref* pSender,  int num);

	void setVisible(bool bVisible) ;
protected:


	virtual bool init();

	virtual void onSetVisible(bool bVisible) override;

	struct UserAchievement			// 获取玩家成就的列表
	{
		cocos2d::Node*  m_pNode ;
		HttpSprite			*m_pSpriteBGItem;		// 滚动视图行的背景精灵图片
		cocos2d::Sprite		*m_pSpriteRewordImage;	// 玩家的获得相应成就的精灵图片
		cocos2d::Label		*m_pLabelName;			// 玩家获得成就的名字
		//cocos2d::Label		*m_pLabelCount;			// 玩家获得的成就的数量
		cocos2d::Label		*m_plavelImageCount;	// 玩家获得的成就数量描述
		cocos2d::EventListenerTouchOneByOne			*m_bgItemListen;		// 玩家单个滚动精灵背景的监听
		LayerButton *m_pBtDetail;
		unsigned m_type ;


	};
	UserAchievement *GetUserAchievementItemList(MyAchievementType etype, const AchievementInfo& achieveuserinfo);// 根据成就类型获取相应的坐标

	// 更新富可敌国的成就的列表
	void UpdateUserAchievementRichItemList(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo);

	// 富可敌国按钮的回调事件
	//void RichAchievementCallBack(UserAchievement *item,const AchievementInfo& achieveuserinfo, MyAchievementType etype);

	// 更新勤劳致富的成就的列表
	//void UpdateUserAchievementHardItemList(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo);

	// 富可勤劳致富的回调事件
	//void AchievementHardCallBack(UserAchievement *item,const AchievementInfo& achieveuserinfo, MyAchievementType etype);

	// 更新赌神之路的成就的列表
	//void UpdateUserAchievementRoadItemList(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo);

	// 富可赌神之路的回调事件
	//void AchievementRoadCallBack(UserAchievement *item,const AchievementInfo& achieveuserinfo,MyAchievementType etype);

	// 更新竞技之神的成就的列表
	//void UpdateUserAchievementGdGoItemList(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo);

	// 富可竞技之神的回调事件
	//void AchievementGdGoCallBack(UserAchievement *item,const AchievementInfo& achieveuserinfo, MyAchievementType etype);

	void menuClickBgItem(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo) ;
private:
	unsigned									m_uRecordMenuCurrentStatusId;					// 记录当前按钮的按下的ID
	int											m_nRecordAchievementCurrentValue;				// 记录总成的当前数量
	int											m_nRecordAchievementMaxValue;					// 记录总成的最大数量
	int											m_nRecordAchievementIconId;						// 记录图标的数量
	cocos2d::Layer								*m_pLayer[5];									// 当前按钮的Layer
	MenuItemSpriteWithLabel						*m_pAchievementMenu[5];							// 成就界面下的所有按钮
	
	cocos2d::Label						*m_pLabelCount;			// 玩家获得的成就的数量
	HttpSprite									*m_pbtRichSpriteBgImage;						// 成就任务富可敌国的高亮、正常、禁止的三种状态显示
	cocos2d::Label								*m_pRichlabelCount;								// 成就任务富可敌国显示的数字

	std::function<bool(cocos2d::Touch *touch, cocos2d::Event*)> m_funcTouchBeganCB;
	std::function<bool(cocos2d::Touch *touch, cocos2d::Event*)> m_funcTouchMovedCB;
	std::function<bool(cocos2d::Touch *touch, cocos2d::Event*)> m_funcTouchEndedCB;
	std::function<bool(cocos2d::Touch *touch, cocos2d::Event*)> m_funcTouchCancelledCB;


	cocos2d::Label								*m_pRichDescName;								// 显示玩家的富可敌国成就达到的内容					
	cocos2d::Sprite								*m_pRichSpriteBgItem;							// 所有成就任务显示的背景Label图片
	cocos2d::Label								*m_pRichCashCenterSizelabel;					// 显示玩家的富可敌国成就Label图片达到的数量
	std::vector<UserAchievement*>				m_vecAchievementListItemOld[5];					// 成就列表Old
	std::vector<UserAchievement*>				m_vecAchievementListItemInValidNew[5];			// 成就列表New


	cocos2d::Label		*m_plavelImageCount;	// 玩家获得的成就数量描述

};
#endif //__ACHIEVEMENTUI_H__