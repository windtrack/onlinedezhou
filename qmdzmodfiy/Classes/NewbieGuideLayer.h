#ifndef __NEWBIEGUIDELAYER_H__
#define __NEWBIEGUIDELAYER_H__

#include "cocos2d.h"
#include <functional>
#include "GameDialog.h"
#include "GameTablePlayer.h"

class NewerTrain;
class TrainTips;
class TextTip;

class NewbieTouchLayer : public GameDialog
{
public:
	CREATE_FUNC(NewbieTouchLayer);

	bool init();
	virtual void onGrabTouchBegan(bool bHitDialog) override;
    virtual void onKeyEscPressed() override;
	void setTouchCallback(std::function<void()> touchDelegate);

private:
	std::function<void()> m_touchDelegate;
};

class NewbieGuideLayer : public GameDialog
{
public:
	CREATE_FUNC(NewbieGuideLayer);
	void StartPlay(const Json::Value& gameRecord);

	virtual void onEnter() override;
	virtual void onExit() override;

    void restart();
    void onMenuItemLeave();
private:
	// 游戏命令
	struct GameRecordCmdItem
	{
		unsigned m_uID;
		double m_dTick;
		unsigned m_uCmd;
		Json::Value m_cmdData;

		bool operator < (const GameRecordCmdItem& rv) const
		{
			return m_uID < rv.m_uID;
		}
	};

protected:
	bool init();
	void ClearTable();
	void JumpToPoint(unsigned uGamePoint);
	void OnCmd(const GameRecordCmdItem& cmd);
	virtual void onSetVisible(bool bVisible) override;

	void onScheduleProgress(float fDelta);
	void onSchedultGetNextCmd(float fDelta);

	virtual void onGrabTouchBegan(bool bHitDialog) override;
	virtual void onKeyEscPressed() override;

    void PlayGuideStep(int step);

    void OnMenuQiPai();
    void OnMenuGenZhu();
    void OnMenuJiaZhu();

    void OnNewTrainBtn();
    void OnNewTrainLable();
    void OnStartGameBtn();
    void OnStartGameLable();

    void OnMenuMaxCardType();

	void parseCmdItems(Json::Value& value);

	void OnScheduleCmdItem(float delta);

	bool execCommandFinished();
	void execCommandItem(int step);
    void setTouchDelegateEnabled(bool enabled);
    cocos2d::ui::Scale9Sprite* PopMinTips(unsigned direction, std::string strContent, cocos2d::Size szContent, unsigned arrowLen, Vec2 pt, Node* parent);
    cocos2d::ui::Scale9Sprite* createBorder(Size sz, Vec2 pt, Node* parent);
    void ShowTips(float fDelta);
    void StopWinAction(float fDelta);
    void SetTouchEnableVisible(float fDelta);
    void SpiltStep(float fDelta);
    void DelayShowLayer(float fDelta);
    TableTip* GetTextTip();
private:

	std::string m_strRecordID;
	ChairViewConvertor m_chairViewConvertor;
	GameTableViewer *m_pGameTable;
	double m_dAllTimeInMs;
	int m_iCurCmd;
	double m_dNextCmdWaitTime;
	bool m_bWaitingCmd;
	std::vector<GameRecordCmdItem> m_vecCmdItem;
	std::vector<GameRecordCmdItem> m_vecTmpCmdItems;
	unsigned int m_vecTmpCmdIndex;

	std::vector<GameRecordScence> m_vecScence;
	std::vector<unsigned char> m_vecHandCard;
	bool m_stayInStep;
	NewbieTouchLayer* m_pTouchHandler;

    bool                       m_IsVisibleMenu;
    unsigned                   m_uSpiltCurStep;
    unsigned                   m_uCurStep;         //当前进度
    TrainTips                 *m_trainTips;        //提示界面
    NewerTrain                *m_newerTrain;       //操作开始前的弹框
    cocos2d::Sprite           *m_spriteArrow;      //箭头动画
    cocos2d::Sprite           *m_spriteCardType;   //牌型介绍
    cocos2d::Sprite           *m_CardTypeEffect;   //最大牌型
    cocos2d::ui::Scale9Sprite *m_sprite9Border[5]; //黄色边框
    cocos2d::ui::Scale9Sprite *m_sprite9Tips[11];  //界面小提示
    cocos2d::Sprite           *m_spriteHeadBorder; //人物头像边框
    cocos2d::Sprite           *m_spriteVideoplay[3]; //视频图标
    TableTip *m_pTextTip;
};


#endif // __NEWBIEGUIDELAYER_H__