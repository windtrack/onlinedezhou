#ifndef _GAMECLUBHALL_H_
#define _GAMECLUBHALL_H_
#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
class FrameSprite ;
class HttpSprite;
class LayerButton;
class FrameSpriteButton;
class FrameScale9SpriteButton;
class MenuItemSpriteWithLabel;
class DataLoading ;
struct FamilyInfo;
struct GlobalMyFamilyInfo;
class GameClubHall :public GameDialog,public  cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(GameClubHall) ;
	bool init() ;

	void setVisible(bool bVisible) ;

	void doBtSelect(int index) ;
	void OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type);

	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	void UpdateGameClubView() ;
	void UpdateSelfClubView() ;


	
private:
	//加入俱乐部
	void menuClickJiaRu(unsigned ufamilyID);
	//显示俱乐部信息
	void menuClickBgItem(unsigned m_familyID);

	void setShowDataLoading(bool flag) ;
protected:
	struct FamilywListItem
	{
		FrameSprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
		LayerButton *m_pBtDetail;
		HttpSprite *m_pSpriteFamilyTouXiang;	
		cocos2d::Label* m_familyName;

		unsigned m_curRoleCount;
		unsigned m_maxRoleCount;
		cocos2d::Label* m_labelRoleCount;
		cocos2d::Label* m_roleCount;
		FrameScale9SpriteButton *m_pBtJiaRu;					//加入按钮
		cocos2d::Label	*m_labelStr;
		unsigned m_familyID; //id
		cocos2d::Label* m_labelRankIndex;
	
		FrameSprite* m_spriteRank;
		unsigned m_familyRankID;//排名
	};

	//获取列表显示
	FamilywListItem *GetFamilyListItem(unsigned m_familyID);
	FamilywListItem *GetSelfClubFamilyListItem(unsigned m_familyID);
	void UpdateItem(GameClubHall::FamilywListItem *item, const FamilyInfo& followInfo);
public:

private:


	cocos2d::Node							*s_uiroot ;//逻辑
	cocos2d::ui::Button						*m_btControll[4] ;
	cocos2d::ui::Text						*m_textControll[4] ;
	//排行
	cocos2d::ui::Button						*m_btSerch ;//搜索
	//我的俱乐部
	cocos2d::ui::Button						*m_btSelfCreatClub ;
	cocos2d::ui::Button						*m_btSelfJoinClub ;

	cocos2d::ui::Layout						*m_layoutNoClub ;
	//俱乐部介绍
	//创建俱乐部
	cocos2d::ui::Button						*m_btCreatClub ;

	cocos2d::ui::Layout						*m_layoutClubView[4] ;

	cocos2d::ui::EditBox					*m_editBoxSearch ;
	cocos2d::ui::Text						*m_textSerachName ;

	cocos2d::ui::EditBox					*m_editBoxCreatClub ;
	cocos2d::ui::Text						*m_textCreatClubName ;


private:
	unsigned								m_curIndex ;
	std::string								m_clubName ;
	std::string								m_clubSerachName ;

	DataLoading* s_dataLoading ;

	cocos2d::extension::ScrollView			*m_pScrollView;			
	std::vector<FamilywListItem*>			m_vecFollowListInfoOld;		//原始数据
	std::vector<FamilywListItem*>			m_vecFollowListInfoNew;
	cocos2d::Size							sizeListView;

	cocos2d::extension::ScrollView			*m_pSelfClubScrollView;			
	std::vector<FamilywListItem*>			m_vecSelfClubFollowListInfoOld;		//原始数据
	std::vector<FamilywListItem*>			m_vecSelfClubFollowListInfoNew;

};
#endif
