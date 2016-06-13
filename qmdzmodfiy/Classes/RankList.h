#ifndef __RANKLIST_H__
#define __RANKLIST_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"

class FrameSprite;
class MenuItemSpriteWithLabel;
class HttpSprite;
class FrameSpriteButton;
class LayerButton;
struct RankUserInfo;


class RankList : public GameDialog
{
public:
	RankList();
	~RankList();
	CREATE_FUNC(RankList);
	// 更新函数
	void UpdateRankList(unsigned uRankType, unsigned uWinType);

	// 初始化底池周战绩排行榜
	void FriendWorldWeeksRecordRichList(unsigned uRankType, unsigned uWinType);

	// 初始化奖金周盈利、日单局、财富榜排行榜
	void FriendWorldRemainRichList(unsigned uRankType, unsigned uWinType);

	// 上一页
	void PageUpMenu(unsigned nServerCurrentPage, unsigned nServerMaxPage);

	// 下一页
	void PageDownMenu(unsigned nServerCurrentPage, unsigned nServerMaxPage);

	// 好友排名的回调函数
	void GoodFriendMenuCallback(Ref* pSender,  int num);

	// 好友列表下按钮的所有回调函数
	void AllMenuCallback(Ref* pSender,  int num);

	// 清理所有的底池的vector
	void ClearWeeksRecordRichList();

	// 清理所有的奖金的vector
	void ClearRemainRichList();
    virtual void setVisible(bool Visible) override;
protected:
	virtual bool init();

	virtual void onSetVisible(bool bVisible) override;

	struct UserWinPotListItem			// 玩家赢得奖金
	{
		cocos2d::Sprite		*m_pSpriteBGItem;		// 滚动视图行的背景
		HttpSprite			*m_pSpriteIcon;			// 玩家的头像
		FrameSprite			*m_pSpriteBgRank;		// 玩家的背景排名
		cocos2d::Sprite		*m_pSpriteRewordImage;	// 玩家的奖金精灵图片
		cocos2d::Label		*m_pLabelRank;			// 玩家文字的排名
		cocos2d::Label		*m_pLabelName;			// 玩家的称呼
		cocos2d::Label		*m_pLabelWinPot;		// 玩家获得的金额数量
		LayerButton			*m_bgItemWinPotListen;	// 玩家整个滚动视图背景的监听
	};
	UserWinPotListItem *GetUserWinPotListItem(unsigned m_uUserWinPotID);						// 获取玩家奖金的列表

	struct UserWinCashListItem			// 玩家赢得底池
	{
		cocos2d::Sprite		*m_pSpriteBGItem;		// 滚动视图行的背景
		HttpSprite			*m_pSpriteIcon;			// 玩家的头像
		FrameSprite			*m_pSpriteBgRank;		// 玩家头像的皇冠、银冠、铜冠、绿色背景排名
		cocos2d::Sprite		*m_pSpriteCashImage;	// 玩家的奖金底池图片
		cocos2d::Label		*m_pLabelRank;			// 玩家文字的排名
		cocos2d::Label		*m_pLabelName;			// 玩家的称呼
		cocos2d::Label		*m_pLabelWinCash;		// 玩家获得的底池数量
		LayerButton			*m_bgItemCashListen;	// 玩家整个滚动视图背景的监听
	};
	UserWinCashListItem *GetUserWinCashListItem(unsigned m_uUserWinCashID);						// 获取玩家底池的列表

	// 更新奖金的列表
	void UpdateUserWinPotListItem(RankList::UserWinPotListItem *item, const RankUserInfo& rankuserinfo);

	// 更新底池的列表
	void UpdateUserWinCashListItem(RankList::UserWinCashListItem *item, const RankUserInfo& rankuserinfo);	

private:
	bool m_bHasRequest[8];

	unsigned									m_uRecordFriendWorldID;							// 记录好友和世界的ID
	unsigned									m_uRecordMenuCurrentStatusId;					// 记录当前按钮的按下的ID
	cocos2d::extension::ScrollView				*m_pScrollView;									// 排行榜的滚动VIew

	MenuItemSpriteWithLabel						*m_pFriendsWorldMenu[4];						// 好友排名排行榜下的所有按钮
	MenuItemSpriteWithLabel						*m_pGoodFriendMenu;								// 好友排名的按钮
	MenuItemSpriteWithLabel						*m_pTheWorldMenu;								// 世界排名的按钮

	FrameSpriteButton* m_nPageUp;																// 上一页的按钮精灵
	FrameSpriteButton* m_nPageDown;																// 下一页的按钮精灵

	std::vector<FrameSprite*> m_vecSprintBg;
	unsigned uGoodFriendFirstRankNum;					
	unsigned uFriendWorldRemainRankNum;
	unsigned RecoderuRankType;			// 记录服务器的好友还是世界排名排名
	unsigned RecoderuWinType;			// 记录服务器的按钮列表
	unsigned nServerCurrentPage;		// 服务器的当前页
	unsigned nServerMaxPage;			// 服务器的最大页
	unsigned uAllConentsizeHigh;
	cocos2d::Size m_szItem;
	GameLayer* m_pLayerTop;

	std::vector<UserWinPotListItem*>			m_vecMissionListWinPotItem;						// 玩家奖金Old 
	std::vector<UserWinPotListItem*>			m_vecMissionListWinPotItemInValid;				// 玩家奖金New 
	std::vector<UserWinCashListItem*>			m_vecMissionWinCashListItem;					// 玩家底池Old
	std::vector<UserWinCashListItem*>			m_vecMissionWinCashListItemInValid;				// 玩家底池New
};



#endif