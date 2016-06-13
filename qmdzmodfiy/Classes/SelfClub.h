#ifndef _SELFCLUB_H_
#define _SELFCLUB_H_
#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"

class ChooseFamilyIcon;
class ContributionRankList ;
class DataLoading ;

class HttpSprite;
class LayerButton;
class FrameSpriteButton;
class MenuItemSpriteWithLabel;
class GameHall;
class TouchGrabber;
class FrameSprite;
class FrameScale9SpriteButton ;

struct FamilyRoleBaseInfo;


class SelfClub :public GameDialog,public  cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(SelfClub) ;
	bool init() ;
	void setVisible(bool bVisible) ;
	void updateSelfClubInfo() ;

	void OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type);
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

	void updateNotice(std::string notice) ;
	void updateIcon() ;

	void updateRoleFund(double clubFund,unsigned uid,double uFund) ;

	void removeRole(unsigned uid) ;

	void updateRoleItem(const FamilyRoleBaseInfo& baseInfo) ;
private:
	void showEditClubIcon(bool flag) ;
	void showInvateRole(bool flag) ;
	void showEditRole(bool flag) ;

	void setShowDataLoading(bool flag) ;

	void updateAllRoleView() ;

	void menuClickBgItem(FamilyRoleBaseInfo baseInfo);
	void doAllotFund(FamilyRoleBaseInfo baseInfo) ;

public:

protected:
	struct RankRoleItem
	{
		Scale9Sprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
		LayerButton *m_pBtDetail;
		HttpSprite *m_pHttpSpriteIcon;	

		cocos2d::Sprite *m_pSpriteNormalIcon;
		cocos2d::Sprite *m_pSpriteOwnerIcon;
		cocos2d::Label* m_familyName;	
		FrameSprite* m_spriteRank;
		cocos2d::Label* m_rankid;

		cocos2d::Label* m_hasCash;
		cocos2d::Label* m_Owner;
		FrameScale9SpriteButton *m_pBtCon;					//分配按钮
		unsigned m_uid ;
	};

	//获取列表显示
	RankRoleItem *GetFamilyListItem(FamilyRoleBaseInfo baseInfo);
	void UpdateItem(SelfClub::RankRoleItem *item, const FamilyRoleBaseInfo& roleInfo);
private:
	cocos2d::Node							*s_uiroot ;//逻辑

	DataLoading								*s_dataLoading ;



	std::string								m_str_notice ;

	cocos2d::ui::EditBox					*m_editBox_Notice ;
	cocos2d::ui::Text						*m_text_clubNotice;
	cocos2d::ui::ImageView					*m_img_edit ;
	
	cocos2d::ui::Button						*m_bt_editIcon;
	HttpSprite								*m_httpSprite_clubIcon;
	cocos2d::ui::Text						*m_text_clubName;
	cocos2d::ui::Text						*m_text_clubFund;

	cocos2d::ui::Button						*m_bt_FundInfo;
	cocos2d::ui::Button						*m_bt_QuitClub;
	cocos2d::ui::Button						*m_bt_invateRole;

	cocos2d::ui::ImageView					*m_imgNoRoleTips ;
	cocos2d::ui::Text						*m_text_curRole;
	cocos2d::ui::Text						*m_text_maxRole;

	cocos2d::ui::ImageView					*m_img_editIcon ;


private:
	cocos2d::extension::ScrollView			*m_pScrollView;			
	std::vector<RankRoleItem*>				m_vecFollowListInfoOld;		//原始数据
	std::vector<RankRoleItem*>				m_vecFollowListInfoNew;
	cocos2d::Size							sizeListView;


	
};

#endif