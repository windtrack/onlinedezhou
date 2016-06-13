#include "TaskSet.h"
#include "GrabTouch.h"
#include "FrameSprite.h"
#include "GameCommon.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "FrameScale9SpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "FrameSpriteButton.h"
#include "cocos-ext.h"   
#include "GameHall.h"
#include "Tools.h"
USING_NS_CC_EXT;

enum SrcollView	//	设置默认的滚动视图的大小
{
	ListScrollW = 700,
	ListScrollH = 476
};


enum MissionType									///< 任务类型
{
	EDODAYWORK,			//	日常任务
	ENEWTASK,			//	新手任务
	EACHIEVEMENTTASK,	//	成就任务
    WEEKLYTASK,         //  本周任务
};

enum MissionStauts									///< 任务状态
{
	EUNFINISHED,		// 未完成
	ECOMPLETED,			// 已完成
	ERECEIVE			// 已领取
};

USING_NS_CC;


bool comp(const MissionInfo &a,const MissionInfo &b)
{
    return a.uScence < b.uScence;
}
//---------------------------------------------------------------------------------------------------------------
void TaskSet::onSetVisible(bool bVisible)
{
    //PlayVisibleSound(bVisible);
}

//---------------------------------------------------------------------------------------------------------------
bool TaskSet::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}

	auto title = CreateNode1AndPosAddTo<Sprite>("taskset/title.png", 470, 568, this, 0);

	//日常任务
	m_nTodayWork = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(TaskSet::TodayWork,this));
	m_nTodayWork->setAnchorPoint(Vec2(0,0));
	m_nTodayWork->setPosition(24,430);
	m_nTodayWork->setEnabled(false);
	auto menuTodayWork = Menu::create(m_nTodayWork,nullptr);
	menuTodayWork->setPosition(Vec2::ZERO);
	addChild(menuTodayWork);
	auto todayWorklabel = CreateLabelMSYHBD(26,"日常任务");
	m_nTodayWork->SetLabel(todayWorklabel);
	m_nTodayWork->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_nTodayWork->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
	// 日常任务的滚动View
	m_pDayScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 210, 22, this, 0);
	m_pDayScrollView->setContentSize(Size(ListScrollW, ListScrollH));
	m_pDayScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);

    //本周任务按钮
    m_btnWeeklyTask = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(TaskSet::WeeklyTask,this));
    m_btnWeeklyTask->setAnchorPoint(Vec2(0,0));
    m_btnWeeklyTask->setPosition(24,363);
    m_btnWeeklyTask->setEnabled(true);
    auto menuWeeklyTask = Menu::create(m_btnWeeklyTask,nullptr);
    menuWeeklyTask->setPosition(Vec2::ZERO);
    addChild(menuWeeklyTask);
    auto weeklyTasklabel = CreateLabelMSYHBD(26,"本周任务");
    m_btnWeeklyTask->SetLabel(weeklyTasklabel);
    m_btnWeeklyTask->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_btnWeeklyTask->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
    //本周任务滚动视图
    m_pWeeklyScrollview = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 210, 22, this, 0);
    m_pWeeklyScrollview->setContentSize(Size(ListScrollW, ListScrollH));
    m_pWeeklyScrollview->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_pWeeklyScrollview->setVisible(false);

	// 成就任务
	m_nAchievementTask = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(TaskSet::AchievementTask,this));
	m_nAchievementTask->setAnchorPoint(Vec2(0,0));
	m_nAchievementTask->setPosition(24,296);
	m_nAchievementTask->setEnabled(true);
	auto menuAchievementTask = Menu::create(m_nAchievementTask,nullptr);
	menuAchievementTask->setPosition(Vec2::ZERO);
	addChild(menuAchievementTask);
	auto achievementTasklabel = CreateLabelMSYHBD(26,"成就任务");
	m_nAchievementTask->SetLabel(achievementTasklabel);
	m_nAchievementTask->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_nAchievementTask->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
	// 成就任务的滚动View
	m_pAchieveScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 210, 22, this, 0);
	m_pAchieveScrollView->setContentSize(Size(ListScrollW, ListScrollH));
	m_pAchieveScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pAchieveScrollView->setVisible(false);
    //签到旋转背景
    m_pShineBg = Sprite::create("taskset/task_sign_shine.png");
    m_pShineBg->setPosition(85, 555);
    this->addChild(m_pShineBg);

    m_pFlagSprite = Sprite::create("inviteflag.png");
    m_pAnimateSign = Sprite::create("taskset/task_signbutton1.png");
    m_pAnimateSign->setPosition(85, 555);
    this->addChild(m_pAnimateSign);
    m_pFlagSprite->setPosition(57, 593);
    this->addChild(m_pFlagSprite);
    auto action1 = Sequence::create(ScaleTo::create(1.5f, 1.15, 1.15, 0), ScaleTo::create(1.5f, 1.0, 1.0), nullptr);
    auto action2 = Sequence::create(ScaleTo::create(1.5f, 1.15, 1.15, 0), ScaleTo::create(1.5f, 1.0, 1.0), nullptr);
    m_pFlagSprite->runAction(RepeatForever::create(action1));
    m_pAnimateSign->runAction(RepeatForever::create(action2));

    m_pAnimateSignEnd = Sprite::create("taskset/task_signbutton2.png");
    m_pAnimateSignEnd->setPosition(85, 555);
    this->addChild(m_pAnimateSignEnd);

    auto szBtn = m_pAnimateSignEnd->getContentSize();
    auto layerBtn = LayerButton::create(false, true);
    layerBtn->setContentSize(szBtn);
    layerBtn->setPosition(85-szBtn.width/2, 555-szBtn.height/2);
    this->addChild(layerBtn);
    layerBtn->SetHitCB(CC_CALLBACK_1(TaskSet::menuHitSignUp, this));

    schedule(schedule_selector(TaskSet::shineRotate));


    m_pSpriteTaskFlag[0] = CreateNode1AndPosAddTo<Sprite>("inviteflag.png", 175,53,m_nTodayWork,1);
    m_pSpriteTaskFlag[1] = CreateNode1AndPosAddTo<Sprite>("inviteflag.png", 175,53,m_btnWeeklyTask,1);
    m_pSpriteTaskFlag[2] = CreateNode1AndPosAddTo<Sprite>("inviteflag.png", 175,53,m_nAchievementTask,1);

	return true;
}

void TaskSet::menuHitSignUp(cocos2d::Ref* pSend)
{
    //签到动画回调
	auto pRef = (GameHall*)this->getParent() ;
	pRef->showDailySign(true) ;
}

void TaskSet::shineRotate(float fDelta)
{
    m_pShineBg->setRotation(m_pShineBg->getRotation()+1);
    if (g_globalUserSigin.m_hasGetSiginReward)
    {
        m_pAnimateSign->setVisible(false);
        m_pFlagSprite->setVisible(false);
        m_pShineBg->setVisible(false);
        m_pAnimateSignEnd->setVisible(true);
    }
    else
    {
        m_pAnimateSign->setVisible(true);
        m_pFlagSprite->setVisible(true);
        m_pShineBg->setVisible(true);
        m_pAnimateSignEnd->setVisible(false);
    }
}

//---------------------------------------------------------------------------------------------------------------
// 今日任务
void TaskSet::TodayWork(Ref* pSender)
{
	m_nTodayWork->setEnabled(false);
    m_btnWeeklyTask->setEnabled(true);
	m_nAchievementTask->setEnabled(true);
	m_pDayScrollView->setVisible(true);
    m_pWeeklyScrollview->setVisible(false);
	m_pAchieveScrollView->setVisible(false);
	return ;
}

//本周任务
void TaskSet::WeeklyTask(Ref* pSender)
{
    m_nTodayWork->setEnabled(true);
    m_btnWeeklyTask->setEnabled(false);
    m_nAchievementTask->setEnabled(true);
    m_pDayScrollView->setVisible(false);
    m_pWeeklyScrollview->setVisible(true);
    m_pAchieveScrollView->setVisible(false);
}
//---------------------------------------------------------------------------------------------------------------
// 成就任务
void TaskSet::AchievementTask(Ref* pSender)
{
	m_nTodayWork->setEnabled(true);
    m_btnWeeklyTask->setEnabled(true);
	m_nAchievementTask->setEnabled(false);
	m_pDayScrollView->setVisible(false);
    m_pWeeklyScrollview->setVisible(false);
	m_pAchieveScrollView->setVisible(true);
	return ;
}

//---------------------------------------------------------------------------------------------------------------
// 初始化所有日常任务
void TaskSet::UpdateDayTask()
{
	for (auto item : m_vecMissionADayInfoNew)			// 日常任务
	{
		item->m_pDaySpriteBgItem->setVisible(false);
		m_vecMissionDayInfoOld.push_back(item);			// PUSH到Old的Vector容器里面
	}
	m_vecMissionADayInfoNew.clear();					// 对New的Vector容器进行清空

	unsigned uDailyMission = g_globalMissionInfo.GetMissions(EDODAYWORK).size();			// 日常任务的数量
	unsigned uAllMission = uDailyMission;												// 所有任务的数量
	float uItemH = 110;																// 滚动视图的单个高度 
	float uAllItemH = uItemH * uAllMission;											// 所有滚动视图的高度
	float fX = ListScrollW / 2, fY = 0;
	if (uAllItemH < ListScrollH)
	{
		//fY = ListScrollH - uAllItemH + uItemH / 2;
		uAllItemH = ListScrollH;		
	}
    fY = uAllItemH - uItemH / 2;
    m_bShowFlag[0] = false;
	for (auto& mission : g_globalMissionInfo.GetMissions(EDODAYWORK))	// 日常任务
	{
        if (mission.uMissonStatus == 1)
            m_bShowFlag[0] = true;
		auto item = GetDayTaskListItem(mission.uMissionDrawID);
		UpdateDayListItem(item, mission);
		item->m_pDaySpriteBgItem->setPosition(fX, fY);
		item->m_pDaySpriteBgItem->setVisible(true);
		m_vecMissionADayInfoNew.push_back(item);
		fY -= uItemH;
	}
    m_pDayScrollView->setContentOffset(Vec2(0, ListScrollH-uAllItemH), false);
	m_pDayScrollView->setContentSize(Size(ListScrollW, uAllItemH));

}
//---------------------------------------------------------------------------------------------------------------
// 初始化所有本周任务
void TaskSet::UpdateWeeklyTask()
{
    for (auto item : m_vecMissionWeeklyNew)			// 本周任务
    {
        item->m_pDaySpriteBgItem->setVisible(false);
        m_vecMissionWeeklyOld.push_back(item);			// PUSH到Old的Vector容器里面
    }
    m_vecMissionWeeklyNew.clear();					// 对New的Vector容器进行清空

    unsigned uDailyMission = g_globalMissionInfo.GetMissions(WEEKLYTASK).size();			// 本周任务的数量
    unsigned uAllMission = uDailyMission;												// 所有任务的数量
    float uItemH = 110;																// 滚动视图的单个高度 
    float uAllItemH = uItemH * uAllMission;											// 所有滚动视图的高度
    float fX = ListScrollW / 2, fY = 0;
    if (uAllItemH < ListScrollH)
    {
        //fY = ListScrollH - uAllItemH + uItemH / 2;
        uAllItemH = ListScrollH;		
    }
    fY = uAllItemH - uItemH / 2;
    m_bShowFlag[1] = false;
    for (auto& mission : g_globalMissionInfo.GetMissions(WEEKLYTASK))	// 本周任务
    {
        if (mission.uMissonStatus == 1)
            m_bShowFlag[1] = true;
        auto item = GetWeeklyTaskItem(mission.uMissionDrawID);
        UpdateWeeklyListItem(item, mission);
        item->m_pDaySpriteBgItem->setPosition(fX, fY);
        item->m_pDaySpriteBgItem->setVisible(true);
        m_vecMissionWeeklyNew.push_back(item);
        fY -= uItemH;
    }
    m_pWeeklyScrollview->setContentOffset(Vec2(0, ListScrollH-uAllItemH), false);
    m_pWeeklyScrollview->setContentSize(Size(ListScrollW, uAllItemH));

}

//---------------------------------------------------------------------------------------------------------------
// 初始化所有成就任务
void TaskSet::UpdateAchievementTask()
{
	for (auto item : m_vecMissionAchievementInfoNew)		// 成就任务
	{
		item->m_pAchieveSpriteBgItem->setVisible(false);
		m_vecMissionAchievementInfoOld.push_back(item);		// PUSH到NEW的Vector容器里面
	}
	m_vecMissionAchievementInfoNew.clear();					// 对OLD的Vector容器进行清空
    m_vecMissionAchievementInfo.clear();

    for (auto itemInfo : g_globalMissionInfo.GetMissions(EACHIEVEMENTTASK))
    {
        if (itemInfo.uMissonStatus != 2)
        {
            m_vecMissionAchievementInfo.push_back(itemInfo);
        }
    }
    std::sort(m_vecMissionAchievementInfo.begin(), m_vecMissionAchievementInfo.end(), comp);
    unsigned uAchievementMission = m_vecMissionAchievementInfo.size();
	unsigned uAllMission = uAchievementMission;												// 所有任务的数量
	float uItemH = 110;																	// 滚动视图的单个高度 
	float uAllItemH = uItemH * uAllMission+6;												// 所有滚动视图的高度

	float fX = ListScrollW / 2, fY = 0;
	if (uAllItemH < ListScrollH)
	{
		//fY = ListScrollH - uAllItemH + uItemH / 2;
		uAllItemH = ListScrollH;		
	}
    fY = uAllItemH - uItemH / 2-6;
    m_bShowFlag[2] = false;
	for (auto& mission : m_vecMissionAchievementInfo)	// 日常任务
	{
        if (mission.uMissonStatus == 1)
            m_bShowFlag[2] = true;
		auto item = GetAchievementTaskListItem(mission.uMissionDrawID);
		UpdateAchievementListItem(item, mission);
		item->m_pAchieveSpriteBgItem->setPosition(fX, fY);
		item->m_pAchieveSpriteBgItem->setVisible(true);
		m_vecMissionAchievementInfoNew.push_back(item);
		fY -= uItemH;
	}
    m_pAchieveScrollView->setContentOffset(Vec2(0, ListScrollH-uAllItemH), false);
	m_pAchieveScrollView->setContentSize(Size(ListScrollW, uAllItemH));	//	成就任务

    //任务消息的标记在此刷新
    for (int i=0; i<3; i++)
    {
        m_pSpriteTaskFlag[i]->setVisible(m_bShowFlag[i]);
    }
}

//---------------------------------------------------------------------------------------------------------------
TaskSet::MissionDayTask* TaskSet::GetDayTaskListItem(unsigned uDayMissionDrawID)		// 获取日常任务任务列表
{
	MissionDayTask *item = nullptr;
	auto labelAnchor = Vec2(0, 0);
	int labelSize = 26;
	int labelFenLeiSize = 24;
	auto labelClr = Color4B::WHITE;
	auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);
	int MAX_Size = 1;
	auto szCur = m_pDayScrollView->getContentSize();
	if (m_vecMissionDayInfoOld.empty())		 //判断OLD的容器数据是否为空 
	{
		// 加载滚动视图行的背景	
		//auto bgItem = FrameSprite::create("taskset/bgitem.png", 1, 2);
		auto bgItem = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 358, 239, m_pDayScrollView, 0);

		// 显示任务图标左精灵图片
		auto httpSprite = HttpSprite::create("taskset/t1.png", Size(84, 84));
		httpSprite->setPosition(12, 10);
		bgItem->addChild(httpSprite);

		// 显示任务内容上Label 
		auto nTaskNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(26 , "",labelAnchor, 105, 52, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);

		// 显示任务内容下Label 
		auto nTaskNamelabe2 =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 105, 22, Color4B(0x50,0xbd,0x2e,255), bgItem, 0);
        // 任务奖励
        auto nTaskReward =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 310, 22, Color4B(0x50,0xbd,0x2e,255), bgItem, 0);

		//	游戏进行中右精灵按钮图片
        //auto nTasking = FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn2.png", false, true);
		auto nTasking = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86), false, true);		// 滚动视图任务按钮的状态
		nTasking->setPosition(612, 54);

		bgItem->addChild(nTasking);
		auto szBt = nTasking->getContentSize();

		// 显示任务完成进度
		auto labelMissionProgress = CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "", Vec2(0.5, 0.5), 0, 0, Color4B::WHITE, nTasking, 0);//显示领取和未领取进度

		auto pItem = new MissionDayTask;
		pItem->m_pDayBtDrawAward = nTasking;
		pItem->m_pDayLabelMissionNameUp = nTaskNamelabel;
        pItem->m_pDayLabelReward = nTaskReward;
		pItem->m_pDayLabelNameDown = nTaskNamelabe2;
		pItem->m_pDaySpriteAwardType = httpSprite;
		pItem->m_pDaySpriteBgItem = bgItem;
		pItem->m_pDayLabelMissionProgress = labelMissionProgress;
		item = pItem;
	}
	else
	{
		item = m_vecMissionDayInfoOld.back();
		m_vecMissionDayInfoOld.pop_back();
	}
	item->m_pDayBtDrawAward->SetHitCB([uDayMissionDrawID](Ref*){
			Json::Value param;
			param["missionDrawID"] = uDayMissionDrawID;
            param["rewardflag"] = 0;
			SendClientRequest(ClientRequest::kDrawAward, param);	
	});
	return item;
}

//---------------------------------------------------------------------------------------------------------------
void TaskSet::UpdateDayListItem(TaskSet::MissionDayTask *item, const MissionInfo& missionInfo)		// 更新日常任务列表
{
	// 得到当前的上Label任务内容
	SetLabelString(item->m_pDayLabelMissionNameUp, missionInfo.strMissionName);	
    char szTxt[128] = {0};
    if (missionInfo.strRewardName == "")
        sprintf(szTxt, "奖励：%.0f德州币", missionInfo.dAward);
    else
        sprintf(szTxt, "%s", missionInfo.strRewardName.c_str());
    SetLabelString(item->m_pDayLabelReward, szTxt);

	// 显示左边精灵下载的图片
	if (missionInfo.strMissionIcon.length() > 0)
	{
		SetHttpSpriteURL(item->m_pDaySpriteAwardType, missionInfo.strMissionIcon);
	}


	//	右精灵按钮Label点击事件的状态
	if (missionInfo.uMissonStatus == EUNFINISHED)		// 进行中
	{
		// 显示任务完成进度
		item->m_pDayBtDrawAward->SetEnable(false);
		//char szTxt[128] = {0};
		sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
		SetLabelString(item->m_pDayLabelNameDown, szTxt);
		SetLabelString(item->m_pDayLabelMissionProgress, "进行中");
		item->m_pDayLabelMissionProgress->setTextColor(Color4B(0x50,0xbd,0x2e,0xff));
	}
	else if (missionInfo.uMissonStatus == ECOMPLETED || missionInfo.uMissonStatus == ERECEIVE)	// 已经完成
	{
		if (missionInfo.bHasDraw)
		{
			item->m_pDayBtDrawAward->SetEnable(false);
			char szTxt[128] = {0};
			sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
			SetLabelString(item->m_pDayLabelNameDown, szTxt);
			SetLabelString(item->m_pDayLabelMissionProgress, "已领取");
			item->m_pDayLabelMissionProgress->setTextColor(Color4B::WHITE);
		}
		else
		{
			item->m_pDayBtDrawAward->SetEnable(true);
			char szTxt[128] = {0};
			sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
			SetLabelString(item->m_pDayLabelNameDown, szTxt);
			SetLabelString(item->m_pDayLabelMissionProgress, "领取");
			item->m_pDayLabelMissionProgress->setTextColor(Color4B::WHITE);
		}
	}
	
	return ;
}

//周任务
TaskSet::MissionDayTask* TaskSet::GetWeeklyTaskItem(unsigned uDayMissionDrawID)		// 获取日常任务任务列表
{
    MissionDayTask *item = nullptr;
    auto labelAnchor = Vec2(0, 0);
    int labelSize = 26;
    int labelFenLeiSize = 24;
    auto labelClr = Color4B::WHITE;
    auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);
    int MAX_Size = 1;
    //auto szCur = m_pWeeklyScrollview->getContentSize();
    if (m_vecMissionWeeklyOld.empty())		 //判断OLD的容器数据是否为空 
    {
        // 加载滚动视图行的背景	
        //auto bgItem = FrameSprite::create("taskset/bgitem.png", 1, 2);
        auto bgItem = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 358, 239, m_pWeeklyScrollview, 0);

        // 显示任务图标左精灵图片
        auto httpSprite = HttpSprite::create("taskset/t1.png", Size(84, 84));
        httpSprite->setPosition(12, 10);
        bgItem->addChild(httpSprite);

        // 显示任务内容上Label 
        auto nTaskNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(26 , "",labelAnchor, 105, 52, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);

        // 显示任务内容下Label 
        auto nTaskNamelabe2 =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 105, 22, Color4B(0x50,0xbd,0x2e,255), bgItem, 0);
        // 任务奖励
        auto nTaskReward =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 310, 22, Color4B(0x50,0xbd,0x2e,255), bgItem, 0);

        //	游戏进行中右精灵按钮图片
        //auto nTasking = FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn2.png", false, true);
        auto nTasking = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86), false, true);		// 滚动视图任务按钮的状态
        nTasking->setPosition(612, 54);

        bgItem->addChild(nTasking);
        auto szBt = nTasking->getContentSize();

        // 显示任务完成进度
        auto labelMissionProgress = CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "", Vec2(0.5, 0.5), 0, 0, Color4B::WHITE, nTasking, 0);//显示领取和未领取进度

        auto pItem = new MissionDayTask;
        pItem->m_pDayBtDrawAward = nTasking;
        pItem->m_pDayLabelMissionNameUp = nTaskNamelabel;
        pItem->m_pDayLabelReward = nTaskReward;
        pItem->m_pDayLabelNameDown = nTaskNamelabe2;
        pItem->m_pDaySpriteAwardType = httpSprite;
        pItem->m_pDaySpriteBgItem = bgItem;
        pItem->m_pDayLabelMissionProgress = labelMissionProgress;
        item = pItem;
    }
    else
    {
        item = m_vecMissionWeeklyOld.back();
        m_vecMissionWeeklyOld.pop_back();
    }
    item->m_pDayBtDrawAward->SetHitCB([uDayMissionDrawID](Ref*){
        Json::Value param;
        param["missionDrawID"] = uDayMissionDrawID;
        param["rewardflag"] = 0;
        SendClientRequest(ClientRequest::kDrawAward, param);	
    });
    return item;
}

void TaskSet::UpdateWeeklyListItem(TaskSet::MissionDayTask *item, const MissionInfo& missionInfo)		// 更新本周任务列表
{
    // 得到当前的上Label任务内容
    SetLabelString(item->m_pDayLabelMissionNameUp, missionInfo.strMissionName);	
    char szTxt[128] = {0};
    if (missionInfo.strRewardName == "")
        sprintf(szTxt, "奖励：%.0f德州币", missionInfo.dAward);
    else
        sprintf(szTxt, "%s", missionInfo.strRewardName.c_str());
    SetLabelString(item->m_pDayLabelReward, szTxt);

    // 显示左边精灵下载的图片
    if (missionInfo.strMissionIcon.length() > 0)
    {
        SetHttpSpriteURL(item->m_pDaySpriteAwardType, missionInfo.strMissionIcon);
    }


    //	右精灵按钮Label点击事件的状态
    if (missionInfo.uMissonStatus == EUNFINISHED)		// 进行中
    {
        // 显示任务完成进度
        item->m_pDayBtDrawAward->SetEnable(false);
        //char szTxt[128] = {0};
        sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
        SetLabelString(item->m_pDayLabelNameDown, szTxt);
        SetLabelString(item->m_pDayLabelMissionProgress, "进行中");
        item->m_pDayLabelMissionProgress->setTextColor(Color4B(0x50,0xbd,0x2e,0xff));
    }
    else if (missionInfo.uMissonStatus == ECOMPLETED || missionInfo.uMissonStatus == ERECEIVE)	// 已经完成
    {
        if (missionInfo.bHasDraw)
        {
            item->m_pDayBtDrawAward->SetEnable(false);
            char szTxt[128] = {0};
            sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
            SetLabelString(item->m_pDayLabelNameDown, szTxt);
            SetLabelString(item->m_pDayLabelMissionProgress, "已领取");
            item->m_pDayLabelMissionProgress->setTextColor(Color4B::WHITE);
        }
        else
        {
            item->m_pDayBtDrawAward->SetEnable(true);
            char szTxt[128] = {0};
            sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
            SetLabelString(item->m_pDayLabelNameDown, szTxt);
            SetLabelString(item->m_pDayLabelMissionProgress, "领取");
            item->m_pDayLabelMissionProgress->setTextColor(Color4B::WHITE);
        }
    }

    return ;
}


//---------------------------------------------------------------------------------------------------------------
TaskSet::MissionAchievementTask* TaskSet::GetAchievementTaskListItem(unsigned uAchieveMissionDrawID)	// 获取成就任务列表
{
	MissionAchievementTask *item = nullptr;
	auto labelAnchor = Vec2(0, 0);
	int labelSize = 26;
	int labelFenLeiSize = 24;
	auto labelClr = Color4B::WHITE;
	auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);
	int MAX_Size = 1;
	auto szCur = m_pAchieveScrollView->getContentSize();
	if (m_vecMissionAchievementInfoOld.empty())		// 判断OLD的容器数据是否为空
	{
		// 加载滚动视图行的背景	
		//auto bgItem = FrameSprite::create("taskset/bgitem.png", 1, 2);
		auto bgItemAchieve = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 358, 239, m_pAchieveScrollView, 0);

		// 显示任务图标左精灵图片
		auto httpSpriteAchieve = HttpSprite::create("taskset/t1.png", Size(84, 84));
		httpSpriteAchieve->setPosition(12, 10);
		bgItemAchieve->addChild(httpSpriteAchieve);
        // 标题图片
        auto frameSprite = FrameSprite::create("achievementui/cjiconall.png", 5, 1, 0);
        frameSprite->setPosition(40,85);
        bgItemAchieve->addChild(frameSprite);

		// 显示任务内容上Label 
		auto nTaskUpNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(26 , "",labelAnchor, 105, 52, Color4B::WHITE, bgItemAchieve, 0);

		// 显示任务内容下Label 
		//auto nTaskDownNamelabe2 =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 105, 18, Color4B(0x50,0xbd,0x2e,0xff), bgItemAchieve, 0);

        // 任务奖励
       auto pAchieveLabelMissionReward = CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 375, 18, Color4B(0x50,0xbd,0x2e,0xff), bgItemAchieve, 0);

		// 显示任务内容上右Label 
		auto nTaskRightDownNamelabe2 =  CreateLabelMSYHAndAnchorPosClrAddTo(20 , "",labelAnchor, 105, 18, Color4B(0x79,0xdc,0x98,0xff), bgItemAchieve, 0);

		//	游戏进行中右精灵按钮图片
        //auto nTasking = FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn2.png", false, true);
		auto nTasking = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86), false, true);		// 滚动视图任务按钮的状态
		nTasking->setPosition(612, 54);

		bgItemAchieve->addChild(nTasking);
		auto szBt = nTasking->getContentSize();

		// 显示任务完成进度
		auto labelMissionProgress = CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "", Vec2(0.5, 0.5), 0, 0, Color4B::WHITE, nTasking, 0);	// 显示进度

		auto pItem = new MissionAchievementTask;
		pItem->m_pAchieveBtDrawAward = nTasking;
		pItem->m_pAchieveLabelMissionUpName = nTaskUpNamelabel;
		//pItem->m_pAchieveLabelMissionDownName = nTaskDownNamelabe2;
		pItem->m_pAchieveLabelMissionNameUpRight = nTaskRightDownNamelabe2;
        pItem->m_pAchieveLabelMissionReward = pAchieveLabelMissionReward;
		pItem->m_pAchieveSpriteAwardType = httpSpriteAchieve;
        pItem->m_pFrameSprite = frameSprite;
		pItem->m_pAchieveSpriteBgItem = bgItemAchieve;
		pItem->m_pAchieveLabelMissionProgress = labelMissionProgress;
		item = pItem;
	}
	else
	{
		item = m_vecMissionAchievementInfoOld.back();
		m_vecMissionAchievementInfoOld.pop_back();
	}
	item->m_pAchieveBtDrawAward->SetHitCB([uAchieveMissionDrawID](Ref*){
		Json::Value param;
		param["missionDrawID"] = uAchieveMissionDrawID;
        param["rewardflag"] = 0;
		SendClientRequest(ClientRequest::kDrawAward, param);	
	});

	return item;
}


//---------------------------------------------------------------------------------------------------------------
void TaskSet::UpdateAchievementListItem(TaskSet::MissionAchievementTask *item, const MissionInfo& missionInfo)		// 更新成就任务列表
{
	// 得到当前的上Label任务内容
	SetLabelString(item->m_pAchieveLabelMissionUpName, missionInfo.strMissionDesc);	

	// 得到当前的下Label任务内容描述
	//SetLabelString(item->m_pAchieveLabelMissionDownName, missionInfo.strMissionDesc);
    char szTxt[128] = {0};
    if (missionInfo.strRewardName == "")
        sprintf(szTxt, "奖励：%s德州币", FormatCash(missionInfo.dAward).c_str());
    else
        sprintf(szTxt, "%s", missionInfo.strRewardName.c_str());
    SetLabelString(item->m_pAchieveLabelMissionReward, szTxt);

	// 显示左边精灵下载的图片
	if (missionInfo.strMissionIcon.length() > 0)
	{
		SetHttpSpriteURL(item->m_pAchieveSpriteAwardType, missionInfo.strMissionIcon);
	}
    //标题图片
    item->m_pFrameSprite->SetFrameIdx(missionInfo.uScence-5);
	

	//	右精灵按钮Label点击事件的状态
	if (missionInfo.uMissonStatus == EUNFINISHED)		// 进行中
	{
		item->m_pAchieveBtDrawAward->SetEnable(false);
		// 显示任务内容上右Label
		//char szTxt[128] = {0};
		sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
		SetLabelString(item->m_pAchieveLabelMissionNameUpRight, szTxt);
		SetLabelString(item->m_pAchieveLabelMissionProgress, "进行中");
		item->m_pAchieveLabelMissionProgress->setTextColor(Color4B(0x50,0xbd,0x2e,0xff));
	}
	else if (missionInfo.uMissonStatus == ECOMPLETED || missionInfo.uMissonStatus == ERECEIVE)	// 已经完成
	{
		if (missionInfo.bHasDraw)
		{
			item->m_pAchieveBtDrawAward->SetEnable(false);
			// 显示任务内容上右Label
			char szTxt[128] = {0};
			sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
			SetLabelString(item->m_pAchieveLabelMissionNameUpRight, szTxt);
			SetLabelString(item->m_pAchieveLabelMissionProgress, "已领取");
			item->m_pAchieveLabelMissionProgress->setTextColor(Color4B::WHITE);
		}
		else
		{
			item->m_pAchieveBtDrawAward->SetEnable(true);
			// 显示任务内容上右Label
			char szTxt[128] = {0};
			sprintf(szTxt, "%d/%d", missionInfo.uCurStep, missionInfo.uAllStep);
			SetLabelString(item->m_pAchieveLabelMissionNameUpRight, szTxt);
			SetLabelString(item->m_pAchieveLabelMissionProgress, "领取");
			item->m_pAchieveLabelMissionProgress->setTextColor(Color4B::WHITE);
		}
	}
	return ;
}

void TaskSet::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		// 初始化所有日常任务
		UpdateDayTask();
        // 初始化本周任务
        UpdateWeeklyTask();
		// 初始化所有成就任务
		UpdateAchievementTask();

		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		//GameDialog::setVisible(false);
		callHideAction() ;
      /*  auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
        runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*******************************************                  新手任务                ***********************************************************//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool NewPlayerTask::init()
{
    if (!GameDialog::init("family/informationbg.png", false))
    {
        return false;
    }
    //任务进行中
    m_spriteNewTaskCur = CreateNode1AndPosAddTo<Sprite>("taskset/newtask.png", 343, 400, this, 0);
    m_labelTaskTitleCur = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0.5, 0.5), 358, 400, Color4B(0xff,0xd2,0x00,0xff), this, 0);

    m_labelTaskContextCur = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0.5, 0.5), 343, 300, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_labelTaskContextCur->setAlignment(TextHAlignment::CENTER);
    m_labelTaskContextCur->setLineHeight(35);
    m_labelTaskContextCur->setLineBreakWithoutSpace(true);
    m_labelTaskContextCur->setWidth(600);

    m_labelTaskStatus = CreateLabelMSYHAndAnchorPosClrAddTo(28, "进行中...", Vec2(0.5, 0.5), 343, 117, Color4B(0x01,0xb6,0x56,0xff), this, 0);

    m_labelTaskProgress = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5, 0.5), 343, 56, Color4B(0x8c,0x8c,0x8c,0x8c), this, 0);

    //m_MenuBtn = MenuItemSpriteWithLabel::create("taskset/yellowbtn.png",3,CC_CALLBACK_1(NewPlayerTask::MenuBtnSelect, this));
    m_MenuBtn = MenuItemScale9SpriteWithLabel::create(0, Size(264, 70), CC_CALLBACK_1(NewPlayerTask::MenuBtnSelect, this));
    m_MenuBtn->setAnchorPoint(Vec2(0.5,0.5));
    m_MenuBtn->setPosition(343, 177);
    auto menuTodayWork = Menu::create(m_MenuBtn,nullptr);
    menuTodayWork->setPosition(Vec2::ZERO);
    auto szBtn = m_MenuBtn->getContentSize();
    addChild(menuTodayWork);
    m_labelBtnName = CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "", Vec2(0.5, 0.5), szBtn.width/2, szBtn.height/2, Color4B(0xff,0xff,0xff,0xff), m_MenuBtn, 0);
    //m_MenuBtn->SetLabel(m_labelBtnName);

    //任务完成，领取奖励
    m_spriteComplete = CreateNode1AndPosAddTo<Sprite>("taskset/compled.png", 577, 367, this, 0);

    m_labelTaskTitlePart = CreateLabelMSYHAndAnchorPosClrAddTo(30, "接下来：", Vec2(1, 0.5), 260, 219, Color4B(0xff,0xd2,0x00,0xff), this, 0);
    m_spriteNewTaskNext = CreateNode1AndPosAddTo<Sprite>("taskset/newtask.png", 280, 219, this, 0);
    m_labelTaskTitleNext = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0, 0.5), 310, 219, Color4B(0xff,0xd2,0x00,0xff), this, 0);
    m_labelTaskTitleNext->setAlignment(TextHAlignment::LEFT);
    m_labelTaskTitleNext->setLineBreakWithoutSpace(true);
    m_labelTaskTitleNext->setWidth(300);

    m_labelTaskContextNext = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0.5, 1), 343, 180, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_labelTaskContextNext->setAlignment(TextHAlignment::CENTER);
    m_labelTaskContextNext->setLineHeight(35);
    m_labelTaskContextNext->setLineBreakWithoutSpace(true);
    m_labelTaskContextNext->setWidth(600);

    m_uCurStep = 0;
    m_bVisible = true;

    return true;
}

void NewPlayerTask::MenuBtnSelect(cocos2d::Ref *pSend)
{
    Json::Value param;
    param["missionDrawID"] = g_globalMissionInfo.GetMissions(ENEWTASK)[0].uMissionDrawID;
    param["rewardflag"] = 0;
    SendClientRequest(ClientRequest::kDrawAward, param);	
    this->setVisible(false);

    if (g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep == 18)
    {
        m_MenuBtn->setVisible(false);
        m_spriteComplete->setVisible(false);
        m_labelTaskStatus->setVisible(false);
        SetLabelString(m_labelTaskTitleCur, "恭喜您，已完成所有新手任务！");
        g_globalMissionInfo.GetMissions(ENEWTASK)[0].uMissonStatus = 2;
    }
}
void NewPlayerTask::SetTaskCompletVisible(bool visible)
{
    m_spriteComplete->setVisible(visible);
    m_labelTaskTitlePart->setVisible(visible);
    m_spriteNewTaskNext->setVisible(visible);
    m_labelTaskTitleNext->setVisible(visible);
    m_labelTaskContextNext->setVisible(visible);
}

void NewPlayerTask::SetTaskUnCompletVisible(bool visible)
{
    m_labelTaskContextCur->setVisible(visible);
    m_labelTaskStatus->setVisible(visible);
}

void NewPlayerTask::UpdateTask(bool bVisible)
{
    auto TaskBgSz = this->getContentSize();

    if (g_globalMissionInfo.GetMissions(ENEWTASK).size() <= 0)
    {
        SetTaskUnCompletVisible(false);
        SetTaskCompletVisible(false);
        m_MenuBtn->setVisible(false);
        SetLabelString(m_labelTaskTitleCur, "亲~没有下一个可做的任务啦！");
        auto sz = m_labelTaskTitleCur->getContentSize();
        m_spriteNewTaskCur->setPosition((TaskBgSz.width- sz.width)/2 - 15, 400);
        return;
    }
    ////新手引导暂未实现，这里直接请求更新下一个任务
    //g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskGuide);

    char buff[64] = {0};
    //重置当前任务标题，重置当前任务名称前的精灵位置
    SetLabelString(m_labelTaskTitleCur, g_globalMissionInfo.GetMissions(ENEWTASK)[0].strMissionName);
    auto curTitleSz = m_labelTaskTitleCur->getContentSize();
    m_spriteNewTaskCur->setPosition((TaskBgSz.width- curTitleSz.width)/2 - 15, 400);
    //按钮文字

    sprintf(buff, "领取%.0f德州币", g_globalMissionInfo.GetMissions(ENEWTASK)[0].dAward);
	

    SetLabelString(m_labelBtnName, buff);
    //设置当前任务进度
    sprintf(buff, "任务全部达成将获得5千德州币奖励，已完成%d/%d", g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep, g_globalMissionInfo.GetMissions(ENEWTASK)[0].uAllStep);
    SetLabelString(m_labelTaskProgress, buff);

    if (g_globalMissionInfo.GetMissions(ENEWTASK)[0].uMissonStatus == 0)
    {
		m_labelBtnName->setOpacity(155) ;
        SetTaskCompletVisible(false);
        SetTaskUnCompletVisible(true);

        m_MenuBtn->setVisible(true);
        m_MenuBtn->setEnabled(false);
        m_MenuBtn->setPosition(343, 177);
        m_uCurStep = g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep;//任务更新为完成时，服务端返回的进度有时有问题
        SetLabelString(m_labelTaskContextCur, g_globalMissionInfo.GetMissions(ENEWTASK)[0].strMissionDesc);
    }
    else if (g_globalMissionInfo.GetMissions(ENEWTASK)[0].uMissonStatus == 1)
    {
		m_labelBtnName->setOpacity(255) ;
        char buff[64] = {0};
        SetTaskCompletVisible(true);
        SetTaskUnCompletVisible(false);

        m_MenuBtn->setVisible(true);
        m_MenuBtn->setEnabled(true);
        m_MenuBtn->setPosition(343, 305);

        SetLabelString(m_labelTaskTitleNext, g_globalMissionInfo.GetMissions(ENEWTASK)[0].strNextMissionName);
        SetLabelString(m_labelTaskContextNext, g_globalMissionInfo.GetMissions(ENEWTASK)[0].strNextMissionDescription);
        if (m_uCurStep != g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep)
        {   //任务更新为完成时，服务端返回的进度有时有问题,本地做验证
            if (m_uCurStep <= g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep)
                m_uCurStep = g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep;
            sprintf(buff, "任务全部达成将获得5千德州币奖励，已完成%d/%d", m_uCurStep, g_globalMissionInfo.GetMissions(ENEWTASK)[0].uAllStep);
            SetLabelString(m_labelTaskProgress, buff);
        }
        if (g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep == 18)
        {
            m_labelTaskTitlePart->setVisible(false);
            m_labelTaskTitleNext->setVisible(false);
            m_spriteNewTaskNext->setVisible(false);
            m_labelTaskContextNext->setVisible(false);
        }
        this->setVisible(bVisible);
    }
    else if (g_globalMissionInfo.GetMissions(ENEWTASK)[0].uMissonStatus == 2)
    {
		m_labelBtnName->setOpacity(155) ;
        if (g_globalMissionInfo.GetMissions(ENEWTASK)[0].uCurStep == 18 || (g_globalMissionInfo.GetMissions(ENEWTASK)[0].strMissionTagName == "texaszcdj" && g_globalMissionInfo.GetMissions(ENEWTASK)[0].uMissionTagValue == 5))
        {
            SetTaskCompletVisible(false);
            SetTaskUnCompletVisible(false);
            m_MenuBtn->setVisible(false);
            m_labelTaskStatus->setVisible(false);
            SetLabelString(m_labelTaskTitleCur, "恭喜您，已完成所有新手任务！");
            sprintf(buff, "任务全部达成将获得5千德州币奖励，已完成%d/%d", 18, g_globalMissionInfo.GetMissions(ENEWTASK)[0].uAllStep);
            SetLabelString(m_labelTaskProgress, buff);
            curTitleSz = m_labelTaskTitleCur->getContentSize();
            m_spriteNewTaskCur->setPosition((TaskBgSz.width- curTitleSz.width)/2 - 15, 400);
        }
        this->setVisible(bVisible);
    }

}

void NewPlayerTask::setVisible(bool bVisible)
{   
    //if (m_bVisible != bVisible)
    //{
        if (bVisible)
        {
			GameDialog::setVisible(true);
			/* auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
			runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0.618);GameDialog::setVisible(true);}), actScale, nullptr));*/
			Tools::callScaleAction(this) ;
        }
        else
        {
            SendClientRequest(ClientRequest::kOptHallBgLayer);
			callHideAction() ;
			/*auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
			runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
        }
    //}
    //m_bVisible = bVisible;
}