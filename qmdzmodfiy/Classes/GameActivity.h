#ifndef _GAMEACTIVITY_H__
#define _GAMEACTIVITY_H__

#include "GameDialog.h"
#include "CCScrollView.h"
#include <vector>
#include <string.h>


struct ActivityInfo;
struct PushActivityInfo;

class FrameSpriteButton;
class HttpSprite;
class MenuItemScale9SpriteWithLabel;

class GameActivity : public GameDialog
{
public:
    CREATE_FUNC(GameActivity);
    void UpdateActivityView();
    void PopTaskOn(unsigned uID, unsigned uType, unsigned uType1, std::string strTitle);
    //void SortActivity(std::vector<ActivityInfo>& activityInfo);
    virtual void setVisible(bool Visible) override;
protected:
    virtual bool init();
	virtual void onSetVisible(bool bVisible);
    void TakePartImmediately(cocos2d::Ref *pSend);
    struct ActivityBtnItemInfo
    {
        unsigned             m_uID;             //活动ID
        std::string          m_strTitle;        //活动标题
        std::string          m_strContext;      //活动内容
        cocos2d::Label       *m_pLableTitle;    //活动标题lable
        HttpSprite           *m_pHttpSprite;    //按钮图标
        FrameSpriteButton    *m_pBtActivity;    //活动按钮
    };

private:
	//cocos2d::Label							   *m_pLableTitleContext;
    cocos2d::Label                             *m_LabelTips;
    cocos2d::Label                             *m_LableTitle;
    cocos2d::Label                             *m_LableContext;
    cocos2d::Sprite                            *m_bgContextItem;
    cocos2d::extension::ScrollView             *m_btnScrollView;
    cocos2d::extension::ScrollView             *m_contextScrollView;
    std::vector<ActivityBtnItemInfo*>          m_vecBtnItemInfo;
    MenuItemScale9SpriteWithLabel              *m_takePartBt;
    unsigned                                    m_uRedefineIndex;
    unsigned                                    m_uSceondRedefineIndex;
};

// 活动消息
class PushActivities : public GameLayer
{
public:
    CREATE_FUNC(PushActivities);

    // 更新特权奖励
    void UpdateActivateRewards(const PushActivityInfo &activityInfo);
    void setVisible(bool bVisible) ;
protected:
    virtual bool init();
    void OnClose(cocos2d::Ref *pSend);
    virtual void onKeyEscPressed();
    void OnMenuReadDetails(cocos2d::Ref*);
private:
    //cocos2d::Label						*m_pRewardContent;
    unsigned             m_uRedefineIndex;//进行页面跳转
    cocos2d::Label      *m_labelTitle;
    HttpSprite          *m_httpActivityBg;
    MenuItemScale9SpriteWithLabel  *m_btnReadDetails;//查看详情按钮
    unsigned            m_uVisibleCount;
};

#endif