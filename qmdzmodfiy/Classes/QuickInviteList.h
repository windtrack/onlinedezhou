#ifndef __QUICKINVITELIST_H__
#define __QUICKINVITELIST_H__
#include "cocos2d.h"
#include "GameDialog.h"
#include "BaseButton.h"
#include "CCScrollView.h"

class HttpSprite;
class FrameSpriteButton;
class MenuItemSpriteWithLabel;

struct QuickFollowInfo;

class QuickInviteList : public GameDialog
{
public:
	CREATE_FUNC(QuickInviteList);
	void UpdateView(unsigned uType);
	void UpdateItemView(unsigned uType, const QuickFollowInfo& userInfo);
	void AppadnItemView(unsigned uType, const QuickFollowInfo& userInfo);
	void ReplaceAndTopItemView(unsigned uType, const QuickFollowInfo& userInfo);
	void RemoveItemView(unsigned uType, unsigned uUserID);
    void ShowInviteFlag(bool flag);
    virtual void setVisible(bool bVisible) override;
protected:
	virtual bool init();
	struct QuickInviteListItem
	{
		unsigned								m_uUID;					// 用户的ID
		cocos2d::Layer							*m_pLayer;				//背景层
		HttpSprite								*m_pSpriteTouXiang;		//头像		
		cocos2d::Label							*m_pLabelName;			//名字Label 
		cocos2d::Label							*m_pLabelBlind;			//大小盲Label
		AutoDelayEnabledFrameSpriteButton		*m_pBtYaoQing;			//邀请按钮
		FrameSpriteButton						*m_pBtJiaRu;			//加入按钮
		FrameSpriteButton						*m_pBtZhiHuiYaoQing;	//置灰按钮
        FrameSpriteButton                       *m_pBtHuLue;            //忽略按钮
		cocos2d::Sprite							*m_pUnderline;			//下划线
	};
	//获取列表显示
	QuickInviteListItem *GetQuickInviteListItem(unsigned uType, const QuickFollowInfo& userInfo);
	void UpdateItem(QuickInviteList::QuickInviteListItem *item, const QuickFollowInfo& followInfo,unsigned uType);

private:
	MenuItemSpriteWithLabel*				m_nDLady;						//德女郎
	MenuItemSpriteWithLabel*				m_nAttent;						//我的关注
	MenuItemSpriteWithLabel*				m_nInvite;						//邀请
	cocos2d::extension::ScrollView			*m_pScrollView[3];				//0--我关注，1--德女郎， 2--邀请我的
	std::vector<QuickInviteListItem*>		m_vecFollowListInfoOld[3];		//原始数据
	std::vector<QuickInviteListItem*>		m_vecFollowListInfoNew[3];			
    cocos2d::Sprite                         *m_pSpriteInviteFlag;
	// 关注
	void menuClickAttent(Ref* pSender);
	//德女郎
	void menuClickDLady(Ref* pSender);
	//邀请
	void menuClickInvite(Ref* pSender);
	//加入
	void menuClickJiaRu(unsigned uType, unsigned uAreaID, unsigned uTableID, bool bHasPassWord, unsigned uTableType, unsigned uUserID);
	//邀请
	void menuClickYaoQing(unsigned uType, unsigned uUID);
    //忽略
    void menuClickHuLue(unsigned uType, unsigned userID);

	//防点击穿透
	void menuClickLeft(Ref* pSender);
	void menuClickCenter(Ref* pSender);
	void menuClickRight(Ref* pSender);
};

#endif