#ifndef  __WINNINGRANKING_H__
#define  __WINNINGRANKING_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "BaseButton.h"

class FrameSprite;
struct RewardListInfo;

class WinningRaking : public GameDialog
{
public:
	CREATE_FUNC(WinningRaking);
	void UpdateView();

protected:
	virtual bool init();

private:
	cocos2d::Label *m_pRewardRanking;
	cocos2d::Label *m_pRewardNum[5];
	cocos2d::Label *m_pRewardName[5];
	cocos2d::Label *m_pRewardCount[5];
	cocos2d::Label *m_pRewardContext[2];
	FrameSprite *m_pSpriteFlowers[5];
};

#endif //__WINNINGRANKING_H__