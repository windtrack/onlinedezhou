#include "RewardDrawingAni.h"
#include "GameCommon.h"
#include "Util.h"
#include "GameData.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h> //使用当前时钟做种子
#include "GameTipsManager.h"
USING_NS_CC;

//---------------------------------------------------------------------------------------------------------------
RewardDrawingAni* RewardDrawingAni::create()
{
	RewardDrawingAni* ret = new RewardDrawingAni();

	//调用完自动释放内存
	if(ret && ret->init()){
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);//安全删除
	return nullptr;
}

//---------------------------------------------------------------------------------------------------------------
bool RewardDrawingAni::init()
{
	if(!Layer::init())
	{
		return false;
	}
	setContentSize(Size(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()));

	// 创建精灵
	m_pRewardGoldTitle = Sprite::create("drawaward/rewardgold.png");
	m_pRewardGoldTitle->setAnchorPoint(Vec2(0.5, 0.5));
	m_pRewardGoldTitle->setPosition(GameData::getInstance()->getGameWidth()/2, GameData::getInstance()->getGameHeight()/2);
	addChild(m_pRewardGoldTitle);
	// 显示内容
	auto pRewardDesc =  CreateLabelMSYHBDAndAnchorPosClrAddTo(40 , "恭喜您获得",Vec2(0, 0), 410, 42, Color4B::YELLOW, m_pRewardGoldTitle, 0);
	// 显示奖金的图片精灵
	auto pRewarSprite = CreateNode1AndPosAddTo<Sprite>("drawaward/reword.png", 625, 67, m_pRewardGoldTitle, 0);
	// 显示获得奖金的数量
	m_pLabelTip = CreateLabelMSYHBDAndAnchorPosClrAddTo(40 , "",Vec2(0,0), 645, 42, Color4B::YELLOW, m_pRewardGoldTitle, 0);

	setVisible(false);
	return true;
}

void RewardDrawingAni::playGoldAnimation(double goldCount)
{
	srand((unsigned)time(NULL));//初始化随机数
	setVisible(true);
	m_nRewardValue = 0;
	auto s = Director::getInstance()->getWinSize();
	m_uGoldCount = 40;
	float size = s.width/m_uGoldCount;
	std::string strName;
	for(unsigned i = 0; i < m_uGoldCount; i++)
	{
		m_pLoginRewarSprite[i] = Sprite::create("drawaward/reward_gold_01.png");
		m_pLoginRewarSprite[i]->setScale(0.7);
		m_pLoginRewarSprite[i]->setAnchorPoint(Vec2(0.5,0.5));
		if(m_uGoldCount < 15)
		{
			m_pLoginRewarSprite[i]->setPosition(Vec2(s.width - size * i + 15, s.height + m_pLoginRewarSprite[i]->getContentSize().height / 2 + 100));
		}
		else if(m_uGoldCount >= 15 && m_uGoldCount <= 35)
		{
			m_pLoginRewarSprite[i]->setPosition(Vec2(s.width - size * i, s.height + m_pLoginRewarSprite[i]->getContentSize().height / 2 + 120));
		}
		else if(m_uGoldCount > 35)
		{
			m_pLoginRewarSprite[i]->setPosition(Vec2(s.width - size * i - 15, s.height + m_pLoginRewarSprite[i]->getContentSize().height / 2 + 100));
		}

		addChild(m_pLoginRewarSprite[i], 1);
		if(i == m_uGoldCount - 1)
		{
			for(i = 0; i < m_uGoldCount; i++)
			{
				animation = Animation::create();
				char szName[100] = {0};
				for( int j=1;j<10;j++)
				{
					sprintf(szName, "drawaward/reward_gold_0%d.png",rand()%9 + 1);
					animation->addSpriteFrameWithFile(szName);
				}
				animation->setDelayPerUnit(2.8f / 23.0f);
				animation->setRestoreOriginalFrame(true);

				auto pGoldAction = Animate::create(animation);
				m_pLoginRewarSprite[i]->runAction(Sequence::create(pGoldAction, pGoldAction->reverse(), nullptr));
				m_pLoginRewarSprite[i]->runAction(RepeatForever::create( Animate::create(animation) ) );     
				int nRecoderNum1 = rand()%2 ? 30 : -30;
				float time1 = CCRANDOM_0_1() * 0.2 + 0.15;
				auto action = Spawn::create(
					MoveBy::create(time1, Vec2(0, -s.height - m_pLoginRewarSprite[i]->getContentSize().height - 80)),
					JumpBy::create(CCRANDOM_0_1() * 0.65 + 0.35, Vec2(0,50), /*120*/160 - rand()%70, 2),
					nullptr);
				auto action1 = Sequence::create(
					JumpBy::create(0.25, Vec2(nRecoderNum1,-5), 40, 1),
					JumpBy::create(0.15, Vec2(nRecoderNum1,-50), 2, 1),
					MoveBy::create(CCRANDOM_0_1() * 0.1+ 0.1, Vec2(0, -500)),nullptr);
				auto act = Sequence::create(DelayTime::create(CCRANDOM_0_1() * 0.7), action,action1,/*RepeatForever::create(actionUp)*/CallFuncN::create( CC_CALLBACK_1(RewardDrawingAni::doRemoveFromParentAndCleanup,this, m_uGoldCount, goldCount, true)), nullptr);
				m_pLoginRewarSprite[i]->runAction(act);
			}
		}
	}

	// 显示奖励内容
	char szTxt[100] = {0};
	sprintf(szTxt, "%s", FormatCash(goldCount).c_str());
	SetLabelString(m_pLabelTip, szTxt);
}
void RewardDrawingAni::doRemoveFromParentAndCleanup(Node* sender, unsigned m_uGoldCount, unsigned goldCount, bool cleanup)
{
	sender->removeFromParentAndCleanup(cleanup);
	++m_nRewardValue;
	if(m_nRewardValue == 0)
	{
		setVisible(true);
		showDesc();
	}
	if(m_nRewardValue == (m_uGoldCount-1))
	{
		GameTipsManager::getInstance()->hideTips();
		setVisible(false);
	}
}

//---------------------------------------------------------------------------------------------------------------
void RewardDrawingAni::showDesc()
{
	m_pRewardGoldTitle->runAction(Sequence::create(Show::create(), DelayTime::create(1.5), Hide::create(), CallFunc::create([this](){/*setVisible(false);*/}), nullptr));
}