#include "QuickMissionList.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "GrabTouch.h"
#include "FrameSpriteButton.h"
#include "GameCommon.h"
#include "GameFrame.h"

USING_NS_CC;

enum
{
	ListScrollW = 238,
	ListScrollH = 246
};

bool compare(const MissionInfo &a,const MissionInfo &b)
{
    return a.uMissonStatus > b.uMissonStatus;
}

void QuickMissionList::UpdateMissionView()
{
	for (auto item : m_vecMissionListItem)
	{
		item->m_pSpriteBgItem->setVisible(false);
		m_vecMissionListItemInValid.push_back(item);
	}
	m_vecMissionListItem.clear();
    m_vecMissionInfo.clear();

    unsigned uTableType = g_pGameFrame->getTableType();

	for (auto& mission : g_globalMissionInfo.GetMissions(0))//过滤每日任务
	{
		if ((mission.uMissonStatus != 2 && mission.uScence == uTableType+1) || (mission.uScence == 4))
		{
            m_vecMissionInfo.push_back(mission);
		}
	}
    for (auto& mission : g_globalMissionInfo.GetMissions(3))//过滤每周任务
    {
        if ((mission.uMissonStatus != 2 && mission.uScence == uTableType+1) || (mission.uScence == 4))
        {
            m_vecMissionInfo.push_back(mission);
        }
    }
    for (auto& mission : g_globalMissionInfo.GetMissions(2))//过滤成就任务
    {
        if (mission.uMissonStatus != 2)
        {
            m_vecMissionInfo.push_back(mission);
        }
    }

    std::sort(m_vecMissionInfo.begin(), m_vecMissionInfo.end(), compare);

	unsigned uAllMission = m_vecMissionInfo.size();
	float uItemH = 82;
	float uAllItemH = uItemH * uAllMission;
	float fX = ListScrollW / 2, fY = 0;
	if (uAllItemH < ListScrollH)
	{
		//fY = ListScrollH - uAllItemH;
		uAllItemH = ListScrollH;		
	}
	
    fY = uAllItemH - uItemH;
    for (auto& mission : m_vecMissionInfo)
	//for (auto& mission : g_globalMissionInfo.GetMissions(2))
	{
		if (mission.uMissonStatus != 2)
		{
			auto item = GetListItem(mission.uMissionDrawID);
			UpdateListItem(item, mission);
			item->m_pSpriteBgItem->setPosition(fX, fY);
			item->m_pSpriteBgItem->setVisible(true);
			m_vecMissionListItem.push_back(item);
			fY -= uItemH;	
		}
	}
	/*for (auto& mission : g_globalMissionInfo.GetMissions(0))
	{
		if (mission.uMissonStatus != 2)
		{
			auto item = GetListItem(mission.uMissionDrawID);
			UpdateListItem(item, mission);
			item->m_pSpriteBgItem->setPosition(fX, fY);
			item->m_pSpriteBgItem->setVisible(true);
			m_vecMissionListItem.push_back(item);
			fY += uItemH;	
		}
	}*/
	m_pScrollView->setContentOffset(Vec2(0, ListScrollH - uAllItemH), false);
    m_pScrollView->setContentSize(Size(ListScrollW, uAllItemH));
}

bool QuickMissionList::init()
{
	if (!GameDialog::init("quickmission/bgqucikmissionlist.png", false))
	{
		return false;
	}
	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 3, 13, this, 0);
	m_pScrollView->setContentSize(Size(ListScrollW, ListScrollH));
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	return true;
}

void QuickMissionList::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		//UpdateMissionView();
	}
}

QuickMissionList::MissionListItem *QuickMissionList::GetListItem(unsigned uMissionDrawID)
{
	MissionListItem *item = nullptr;
	if (m_vecMissionListItemInValid.empty())
	{		
		auto bgItem = CreateNode1AndPosAddTo<Sprite>("quickmission/bgitemline.png", 13, 0, m_pScrollView, 0);
		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0, 0), -2, 46, Color4B::WHITE, bgItem, 0);
		auto labelAward = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0, 0), 25, 12, Color4B(0xff, 0xd2, 0, 255), bgItem, 0);
		auto spriteAwardType = CreateNode1AndPosAddTo<Sprite>("dt_lebi.png", 10, 25, bgItem, 0);
		auto btDrawAward = FrameSpriteButton::createWithVertFrameSprite("quickmission/btdrawaward.png", false, true);
		btDrawAward->setPosition(160, 25);

		bgItem->addChild(btDrawAward);
		auto szBt = btDrawAward->getContentSize();
		m_pLabelMissionProgress = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5, 0.5), 0, 0, Color4B::WHITE, btDrawAward, 0);

		auto pItem = new MissionListItem;
		pItem->m_pBtDrawAward = btDrawAward;
		pItem->m_pLabelAward = labelAward;
		pItem->m_pLabelMissionName = labelName;
		pItem->m_pSpriteAwardType = spriteAwardType;
		pItem->m_pSpriteBgItem = bgItem;
		pItem->m_pLabelMissionProgress = m_pLabelMissionProgress;
		item = pItem;
	}
	else
	{
		item = m_vecMissionListItemInValid.back();
		m_vecMissionListItemInValid.pop_back();
		item->m_pLabelMissionProgress->setSystemFontSize(20);
	}
	item->m_pBtDrawAward->SetHitCB([uMissionDrawID](Ref*){
			Json::Value param;
			param["missionDrawID"] = uMissionDrawID;
            param["rewardflag"] = 0;
			SendClientRequest(ClientRequest::kDrawAward, param);
	});
	return item;
}

void QuickMissionList::UpdateListItem(QuickMissionList::MissionListItem *item, const MissionInfo&missionInfo)
{
	SetLabelString(item->m_pLabelMissionName, missionInfo.strMissionName);
    auto missionNameSz = item->m_pLabelMissionName->getContentSize();
    item->m_pLabelMissionName->stopAllActions();
    item->m_pLabelMissionName->setPosition(-2, 46);
    item->m_pLabelMissionName->setVisible(true);
    if (missionNameSz.width > 230)//任务名称太长，滚动显示
    {
        auto act1 = Spawn::create(CallFunc::create([item](){item->m_pLabelMissionName->setVisible(false);}), MoveTo::create(0.1, Vec2(240, 46)), nullptr);
        auto act2 = Sequence::create(MoveTo::create(10.0, Vec2(-280, 46)), act1, CallFunc::create([item](){item->m_pLabelMissionName->setVisible(true);}), nullptr);
        item->m_pLabelMissionName->runAction(RepeatForever::create(act2));
    }

    item->m_pLabelAward->stopAllActions();
    item->m_pLabelAward->setVisible(true);
    if (missionInfo.dAward == 0)//实物奖励名称太长，滚动显示
    {
        item->m_pSpriteAwardType->setVisible(false);
        item->m_pLabelAward->setPositionX(-3);
        SetLabelString(item->m_pLabelAward, missionInfo.strRewardName);
        auto lableSz = item->m_pLabelAward->getContentSize();
        if (lableSz.width > 105)
        {
            auto act1 = Sequence::create(MoveBy::create(3.0, Vec2(100-lableSz.width, 0)), MoveBy::create(4.0, Vec2(lableSz.width-100, 0)), nullptr);
            item->m_pLabelAward->runAction(RepeatForever::create(act1));
        }
    }
    else
    {
        item->m_pSpriteAwardType->setVisible(true);
        item->m_pLabelAward->setPositionX(25);
        SetLabelString(item->m_pLabelAward, FormatCash_miss(missionInfo.dAward));
    }
	
	if (missionInfo.uMissonStatus == 0) // 未完成
	{
		item->m_pBtDrawAward->SetEnable(false);
		char szTxt[128] = {0};
		sprintf(szTxt, "%s/%s", FormatCash_miss(missionInfo.uCurStep).c_str(), FormatCash_miss(missionInfo.uAllStep).c_str());
		// sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
		std::string szTextlen = szTxt;
		int Textlen = szTextlen.length();
		if(Textlen > 10)
		{
			item->m_pLabelMissionProgress->setSystemFontSize(16);
		}
			
		SetLabelString(item->m_pLabelMissionProgress, szTxt);
	}
	else if (missionInfo.uMissonStatus == 1) // 已经完成
	{
		if (missionInfo.bHasDraw)
		{
			item->m_pBtDrawAward->SetEnable(false);
			SetLabelString(item->m_pLabelMissionProgress, "已领取");
		}
		else
		{
			item->m_pBtDrawAward->SetEnable(true);
			SetLabelString(item->m_pLabelMissionProgress, "领取奖励");
		}
	}
}

void QuickMissionList::setVisible(bool bVisible)
{
    this->setAnchorPoint(Vec2(1, 0));
    if (bVisible)
    {
        auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
        runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0.3);GameDialog::setVisible(true);}), actScale, nullptr));
        UpdateMissionView();
    }
    else
    {
        auto actScale = Spawn::create(ScaleTo::create(0.2, 0.3), FadeOut::create(0.2), nullptr);
        runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));
    }
}