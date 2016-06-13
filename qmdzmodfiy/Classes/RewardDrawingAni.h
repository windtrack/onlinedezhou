#ifndef  __LOGINREWARD_H__
#define  __LOGINREWARD_H__

#include "cocos2d.h"

class RewardDrawingAni : public cocos2d::Layer
{
public:
	static RewardDrawingAni* create();

	// 播放德州币帧动画
	void playGoldAnimation(double m_nGoldCount);

protected:
	virtual bool init();

	// 是否隐藏
	void showDesc();

	void doRemoveFromParentAndCleanup(Node* sender, unsigned goldCount, unsigned m_nGoldCount, bool cleanup);
private:
	unsigned int					m_uGoldCount;		
	bool							m_bIsReward;						// 是否奖励过
	unsigned int					m_nRewardValue;					// 记录值
	cocos2d::Sprite					*m_pLoginRewarSprite[256];		// 默认最大创建80个精灵
	cocos2d::Sprite					*m_pRewardGoldTitle;			// 创建显示的背景奖励框
	cocos2d::Label					*m_pLabelTip;
	cocos2d::Animation *animation;
};



#endif //__LOGINREWARD_H__