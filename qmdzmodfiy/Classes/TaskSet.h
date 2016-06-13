#ifndef  __TASKSET_H__
#define  __TASKSET_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"


class FrameSprite;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class HttpSprite;
class FrameSpriteButton;
class FrameScale9SpriteButton;
class AnimateViewer;
struct MissionInfo;

class TaskSet : public GameDialog
{
public:
	CREATE_FUNC(TaskSet);
	// 初始化所有日常任务
	void UpdateDayTask();
    // 初始化本周任务
    void UpdateWeeklyTask();

	// 初始化所有成就任务
	void UpdateAchievementTask();

	// 今日任务
	void TodayWork(Ref* pSender);
    //本周任务
    void WeeklyTask(Ref* pSender);
	// 成就任务
	void AchievementTask(Ref* pSender);
    virtual void setVisible(bool Visible) override;
protected:
	virtual bool init();
	virtual void onSetVisible(bool bVisible) override;
    void shineRotate(float fDelta);//回调  发光背景旋转
    void menuHitSignUp(cocos2d::Ref* pSend);//点击签到按钮回调

	struct	MissionDayTask				//	日常任务保存的数据结构
	{
		unsigned uDayMissionDrawID;								// 领取任务的ID
		cocos2d::Sprite *m_pDaySpriteBgItem;					// 滚动视图行的背景	
		cocos2d::Label *m_pDayLabelMissionNameUp;				// 上Label 
		cocos2d::Label *m_pDayLabelNameDown;					// 下Label
        cocos2d::Label *m_pDayLabelReward;                      // 任务奖励
		HttpSprite *m_pDaySpriteAwardType;						// 左精灵图片
		FrameScale9SpriteButton *m_pDayBtDrawAward;					// 右精灵按钮图片
		cocos2d::Label *m_pDayLabelMissionProgress;				// 右精灵按钮Label
	};
	MissionDayTask *GetDayTaskListItem(unsigned uDayMissionDrawID);												// 获取日常任务任务列表
    MissionDayTask *GetWeeklyTaskItem(unsigned uDayMissionDrawID);

	struct	MissionAchievementTask		//	成就任务保存的数据结构
	{
		unsigned uAchieveMissionDrawID;							// 领取任务的ID
		cocos2d::Sprite *m_pAchieveSpriteBgItem;				// 滚动视图行的背景	
		cocos2d::Label *m_pAchieveLabelMissionUpName;			// 上Label 
		cocos2d::Label *m_pAchieveLabelMissionDownName;			// 下Label
		cocos2d::Label *m_pAchieveLabelMissionNameUpRight;		// 上右Label
        cocos2d::Label *m_pAchieveLabelMissionReward;           // 任务奖励
		HttpSprite *m_pAchieveSpriteAwardType;					// 左精灵图片
        FrameSprite *m_pFrameSprite;                            // 成就标题图片
		FrameScale9SpriteButton *m_pAchieveBtDrawAward;				// 右精灵按钮图片
		cocos2d::Label *m_pAchieveLabelMissionProgress;			// 右精灵按钮Label
	};
	MissionAchievementTask *GetAchievementTaskListItem(unsigned uAchieveMissionDrawID);							// 获取成就任务列表


	void UpdateDayListItem(TaskSet::MissionDayTask *item, const MissionInfo& missionInfo);						// 更新日常任务列表
    void UpdateWeeklyListItem(TaskSet::MissionDayTask *item, const MissionInfo& missionInfo);

	void UpdateAchievementListItem(TaskSet::MissionAchievementTask *item, const MissionInfo& missionInfo);		// 更新日常任务列表

	TaskSet*							m_nMissionInfo;								// 任务
	GameDialog*							m_nGameDialog;								// 游戏对话框


private:
	MenuItemSpriteWithLabel*				m_nTodayWork;							// 日常任务按钮
    MenuItemSpriteWithLabel*                m_btnWeeklyTask;                          // 周任务
	MenuItemSpriteWithLabel*				m_nAchievementTask;						// 成就任务按钮
	//HttpSprite*								httpSprite;								// 网上下载小图片
	//cocos2d::Sprite*						bgItem;									// 滚动背景
	//HttpSprite*								httpSpriteAchieve;						// 成就任务网上下载小图片
	//cocos2d::Sprite*						bgItemAchieve;							// 成就任务滚动背景
	cocos2d::extension::ScrollView			*m_pDayScrollView;						// 日常任务的滚动View
    cocos2d::extension::ScrollView          *m_pWeeklyScrollview;                   // 本周任务
	cocos2d::extension::ScrollView			*m_pAchieveScrollView;					// 成就任务的滚动View
	std::vector<MissionDayTask*>			m_vecMissionDayInfoOld;					// 日常任务服务器的原始数据
	std::vector<MissionDayTask*>			m_vecMissionADayInfoNew;				// 日常任务用户点击事件后新的数据
    std::vector<MissionDayTask*>            m_vecMissionWeeklyNew;
    std::vector<MissionDayTask*>            m_vecMissionWeeklyOld;
	std::vector<MissionAchievementTask*>	m_vecMissionAchievementInfoOld;			// 成就任务服务器的原始数据
	std::vector<MissionAchievementTask*>	m_vecMissionAchievementInfoNew;			// 成就任务用户点击事件后新的数据
    std::vector<MissionInfo>                m_vecMissionAchievementInfo;            // 筛选后的成就任务
    cocos2d::Sprite                         *m_pAnimateSign;                        // 签到动画
    cocos2d::Sprite                         *m_pAnimateSignEnd;                     // 签到动画结束
    cocos2d::Sprite                         *m_pShineBg;                            // 签到旋转背景
    cocos2d::Sprite                         *m_pFlagSprite;                         // 签到动画中的红点

    cocos2d::Sprite                         *m_pSpriteTaskFlag[3];                  // 任务消息标记
    bool                                     m_bShowFlag[3];                        // 是否显示标记
	

};

class NewPlayerTask : public GameDialog
{
public:
    CREATE_FUNC(NewPlayerTask);
    void UpdateTask(bool bVisible);
    virtual void setVisible(bool bVisible);
protected:
    virtual bool init();
    void MenuBtnSelect(cocos2d::Ref *pSend);
    void SetTaskCompletVisible(bool visible);
    void SetTaskUnCompletVisible(bool visible);
    
private:
    cocos2d::Sprite                     *m_spriteNewTaskCur;
    cocos2d::Sprite                     *m_spriteNewTaskNext;
    cocos2d::Sprite                     *m_spriteComplete;
    cocos2d::Label                      *m_labelTaskTitleCur;
    cocos2d::Label                      *m_labelTaskTitleNext;
    cocos2d::Label                      *m_labelTaskTitlePart;
    cocos2d::Label                      *m_labelTaskContextCur;
    cocos2d::Label                      *m_labelTaskContextNext;
    cocos2d::Label                      *m_labelTaskStatus;
    cocos2d::Label                      *m_labelTaskProgress;
    cocos2d::Label                      *m_labelBtnName;
    unsigned                             m_uCurStep;
    bool                                 m_bVisible;
    MenuItemScale9SpriteWithLabel    *m_MenuBtn;
};

#endif //__TASKSET_H__