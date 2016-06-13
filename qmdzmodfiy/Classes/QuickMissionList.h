#ifndef __QUICKMISSIONLIST_H__
#define __QUICKMISSIONLIST_H__

#include "GameDialog.h"
#include "CCScrollView.h"

class FrameSprite;
struct MissionInfo;
class FrameSpriteButton;

class QuickMissionList : public GameDialog
{
public:
	CREATE_FUNC(QuickMissionList);
	void UpdateMissionView();
    virtual void setVisible(bool bVisible) override;
protected:
	bool init();
	virtual void onSetVisible(bool bVisible) override;
	
	struct MissionListItem
	{
		unsigned uMissionDrawID;
		cocos2d::Sprite *m_pSpriteBgItem;
		cocos2d::Label *m_pLabelMissionName;
		cocos2d::Label *m_pLabelAward;
		cocos2d::Sprite *m_pSpriteAwardType;
		FrameSpriteButton *m_pBtDrawAward;
		cocos2d::Label *m_pLabelMissionProgress;
	};
	MissionListItem *GetListItem(unsigned uMissionDrawID);
	void UpdateListItem(MissionListItem *, const MissionInfo&);

private:
	cocos2d::Label *m_pLabelMissionProgress;
	cocos2d::extension::ScrollView *m_pScrollView;
	std::vector<MissionListItem*> m_vecMissionListItem;
	std::vector<MissionListItem*> m_vecMissionListItemInValid;
    std::vector<MissionInfo>      m_vecMissionInfo;//过滤后需要显示的任务
};


#endif