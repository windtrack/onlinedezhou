/********************************************************************
	created:	2015/04/10
	created:	2015:04:10  15:53
	filename: 	PrivilegesRewards.h
	file path:	D:\WORK\texaspoker\Classes\GameDialog.h
	file base:	hall
	file ext:	
	author:		刘先
	
	purpose:	大厅界面后领取活动相关操作
*********************************************************************/
#ifndef  __PRIVILEGESREWARDS_H__
#define  __PRIVILEGESREWARDS_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"

class MenuItemSpriteWithLabel;
class HttpSprite;
struct ActReward;

class PrivilegesRewards : public GameDialog
{
public:
	CREATE_FUNC(PrivilegesRewards);

	// 更新特权奖励
	void UpdatePrivilegesRewards();
	void setVisible(bool bVisible) ;
protected:
	virtual bool init();
    virtual void onGrabTouchBegan(bool bHitDialog);
	virtual void onKeyEscPressed();

	void RewardButton(Ref *pSender);

private:
	cocos2d::Label						*m_pRewardContent;
};

// 德女郎奖励
class WaveGirlRewards : public GameDialog
{
public:
	CREATE_FUNC(WaveGirlRewards);

	// 更新特权奖励
	void UpdateWaveGirlRewards();
    void setVisible(bool bVisible) ;
protected:
	virtual bool init();
    virtual void onGrabTouchBegan(bool bHitDialog);
	virtual void onKeyEscPressed();

	void RewardButton(Ref *pSender);

private:
	cocos2d::Label						*m_pRewardContent;
};

class ActivityRewards : public GameDialog
{
public:
    CREATE_FUNC(ActivityRewards);

    // 更新特权奖励
    void UpdateActivityRewards();
    void setVisible(bool bVisible) ;
protected:
    struct ActivityItem
    {
        cocos2d::Layer   *m_layerBg;
        HttpSprite       *m_spriteReward;
        cocos2d::Label   *m_labelDes;
    };
    ActivityItem* GetActivityItem(ActReward rewardInfo);
    void UpdateRewardItem(ActivityItem *item, ActReward rewardInfo);
    virtual bool init();
    virtual void onGrabTouchBegan(bool bHitDialog);
    virtual void onKeyEscPressed();

    void RewardButton(Ref *pSender);

private:
    cocos2d::extension::ScrollView      *m_scrollView;
    cocos2d::Label		                *m_pRewardContent;
    std::vector<ActivityItem*>          m_vecRewardItemNew;
    std::vector<ActivityItem*>          m_vecRewardItemOld;
};

#endif //__PRIVILEGESREWARDS_H__