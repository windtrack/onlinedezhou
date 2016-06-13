#ifndef __QUICKFOLLOWLIST_H__
#define __QUICKFOLLOWLIST_H__

#include "cocos2d.h"
#include "CCScrollView.h"
#include "UICommon.h"

class HttpSprite;
class LayerButton;
class FrameSpriteButton;
class MenuItemSpriteWithLabel;
class GameHall;

struct QuickFollowInfo;

#define  MAX_PAGECOUNT 10

class QuickFollowList : public cocos2d::Layer,public cocos2d::extension::ScrollViewDelegate
{
public:
	CREATE_FUNC(QuickFollowList);
	static QuickFollowList* creatWithLayer(cocos2d::ui::ImageView* view) ;
	bool initWithLayer(cocos2d::ui::ImageView* view) ;

	void UpdateView(unsigned uType);
	void UpdateItemView(unsigned uType, const QuickFollowInfo& userInfo);
	void AppadnItemView(unsigned uType, const QuickFollowInfo& userInfo);
	void RemoveItemView(unsigned uType, unsigned uUserID);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) ;
private:

	void callNextPage(int type) ;

private:
	int m_maxPage[2] ;
	int m_curPage[2];
	


protected:
	virtual bool init();

	struct QuickFollowListItem
	{
		unsigned m_uUID;								// 用户的ID
		cocos2d::Sprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
		LayerButton *m_pBtDetail;
		HttpSprite *m_pSpriteTouXiang;					
		cocos2d::Label *m_pLabelUp;				        // 上Label 
		cocos2d::Label *m_pLabelDown;					//下Label
		FrameSpriteButton *m_pBtJiaRu;					//加入按钮
	};
	//获取列表显示
	//QuickFollowListItem *GetQuickFollowListItem(unsigned m_uUID, unsigned uAreaID, unsigned uTableID, bool bHasPassWord,unsigned uType,unsigned uTableType);
    QuickFollowListItem *GetQuickFollowListItem(QuickFollowInfo followInfo, unsigned uType);
	void UpdateItem(QuickFollowList::QuickFollowListItem *item, const QuickFollowInfo& followInfo,unsigned uType);

private:
	cocos2d::ui::Button*				m_nDLady;						//德女郎
	cocos2d::ui::Button*				m_nAttent;						//关注
	//MenuItemSpriteWithLabel*				m_nDLady;						//德女郎
	//MenuItemSpriteWithLabel*				m_nAttent;						//关注
	//cocos2d::ui::ScrollView			*m_pScrollView[2];			
	cocos2d::extension::ScrollView			*m_pScrollView[2];			
	std::vector<QuickFollowListItem*>		m_vecFollowListInfoOld[2];		//原始数据
	std::vector<QuickFollowListItem*>		m_vecFollowListInfoNew[2];

	cocos2d::Size sizeListView;

	bool isClickButton ;
	// 关注
	void menuClickAttent(Ref* pSender,cocos2d::ui::TouchEventType type);
	//德女郎
	void menuClickDLady(Ref* pSender,cocos2d::ui::TouchEventType type);

	void menuCTTouch(Ref* pSender,cocos2d::ui::TouchEventType type);
	//防穿透
	void menuClickLeft(Ref* pSender);
	void menuClickRight(Ref* pSender);
	//加入
	void menuClickJiaRu(unsigned uAreaID, unsigned uTableID, bool bHasPassword,unsigned uTableType, unsigned uUserID);
	//信息框
	void menuClickBgItem(unsigned uUID);
};
#endif