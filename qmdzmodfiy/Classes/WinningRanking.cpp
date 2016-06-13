#include "WinningRanking.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"

USING_NS_CC;

bool WinningRaking::init()
{
	if (!GameDialog::init("rewardranklist/winninglistbg.png", false))
	{
		return false;
	}

	m_pRewardRanking = CreateLabelMSYHAndAnchorPosClrAddTo(18 , "中奖排名",Vec2::ANCHOR_BOTTOM_LEFT, 80, 170,Color4B(0xff,0xf8,0x89,0xff), this, 0);

	for(int i = 0;i < 5;i++)
	{
		m_pRewardNum[i] = CreateLabelMSYHAndAnchorPosClrAddTo(15 , "",Vec2::ANCHOR_BOTTOM_LEFT, 15, 140 - i* 28,Color4B::WHITE, this, 0);
		m_pRewardName[i] = CreateLabelMSYHAndAnchorPosClrAddTo(15 , "",Vec2::ANCHOR_MIDDLE_LEFT, 40, 150 - i* 28,Color4B::WHITE, this, 0);
		m_pSpriteFlowers[i] = CreateNode3AndPosAddTo<FrameSprite>("rewardranklist/royalflush.png", 1, 2, 140, 152 - i* 28, this, 0);
		m_pRewardCount[i] = CreateLabelMSYHAndAnchorPosClrAddTo(15 , "",Vec2::ANCHOR_MIDDLE_RIGHT, 227, 152 - i* 28,Color4B::WHITE, this, 0);
	}
	m_pRewardContext[0] = CreateLabelMSYHAndAnchorPosClrAddTo(15 , "",Vec2(0,0.5), 12, 15,Color4B(0xff,0xf8,0x89,0xff), this, 0);
	m_pRewardContext[1] = CreateLabelMSYHAndAnchorPosClrAddTo(15 , "",Vec2::ANCHOR_MIDDLE_RIGHT, 228, 15,Color4B(0xff,0xf8,0x89,0xff), this, 0);
	return true;
}

void WinningRaking::UpdateView()
{
	RewardListInfo* prewardlistinfo = nullptr;
	prewardlistinfo = g_globalTableConfig.m_RewardListInfo;
	if (prewardlistinfo)
	{
		for(int i = 0;i < 5; i++)
		{
			RewardListInfo& info = prewardlistinfo[i];
			SetLabelString(m_pRewardNum[i], FormatCash(info.m_uRankID));
			SetLabelStringWithBoundWidth(m_pRewardName[i], info.m_strNickName.c_str(), 90);
			if(info.m_bIfShow)
			{
				m_pSpriteFlowers[i]->SetFrameIdx(0);
			}
			else
			{
				m_pSpriteFlowers[i]->SetFrameIdx(1);
			}
			SetLabelString(m_pRewardCount[i], FormatCash(info.m_dRankCount).c_str());
		}
	}
	char szText[100] = {0};
	sprintf(szText, "皇家%s%%奖池", do_fraction(prewardlistinfo->m_uRoyalCount, 2).c_str());
	SetLabelString(m_pRewardContext[0], szText);
	sprintf(szText, "同花顺%s%%奖池", do_fraction(prewardlistinfo->m_uflowersCount,2).c_str());
	SetLabelString(m_pRewardContext[1], szText);
}