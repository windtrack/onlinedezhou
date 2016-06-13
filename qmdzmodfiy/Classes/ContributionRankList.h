//
//#ifndef _ContributionRankList_H__
//#define _ContributionRankList_H__
//
//#include "GameDialog.h"
//#include "CCScrollView.h"
//#include "vector"
//#include "cocos2d.h"
//#include "ui/UIEditBox/UIEditBox.h"
//#include "GameDialog.h"
//class DataLoading ;
//class MenuItemSpriteWithLabel;
//class HttpSprite;
//class FrameSprite;
//class LayerButton;
//
//
//struct FamilyRoleBaseInfo;
//class ContributionRankList : public GameDialog
//{
//public:
//
//	CREATE_FUNC(ContributionRankList);
//	void UpdateView();
//
//	void RemoveItemView( unsigned uUserID);
//	void updateItemView(const FamilyRoleBaseInfo &baseInfo) ;
//	void setShowDataLoading(bool flag) ;
//protected:
//	virtual bool init();
//	virtual void onMenuItemCloseSelected(cocos2d::Ref * ref);
//	void menuClickBgItem(FamilyRoleBaseInfo baseInfo);
//
//
//public:
//
//	struct RankRoleItem
//	{
//		FrameSprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
//		LayerButton *m_pBtDetail;
//		HttpSprite *m_pSpriteRoleIcon;	
//		cocos2d::Label* m_familyName;	
//		FrameSprite* m_spriteRank;
//		cocos2d::Label* m_rankid;
//		cocos2d::Label* m_contributionValue;
//		unsigned m_uid ;
//	};
//
//	//获取列表显示
//	RankRoleItem *GetFamilyListItem(FamilyRoleBaseInfo baseInfo);
//	void UpdateItem(ContributionRankList::RankRoleItem *item, const FamilyRoleBaseInfo& roleInfo);
//
//private:
//
//	DataLoading* s_dataLoading ;
//	cocos2d::Size sizeListView;
//	cocos2d::Sprite* spriteTitle;
//	cocos2d::Label                              *m_lableTips;                   //警告
//	cocos2d::extension::ScrollView              *m_ScrollView;              //滚动视图
//	FrameSprite                                 *m_SpriteUsing;                 //使用头像
//	std::vector<RankRoleItem*>		            m_vecRanRoleOld;
//
//	GameLayer                               *m_layerTop;
//	GameLayer                               *m_layerBottom;
//};
//
//#endif