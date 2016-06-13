#ifndef _CHANHEVICESHAIKH_H_
#define _CHANHEVICESHAIKH_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"

#include "FrameSpriteButton.h"


class MenuItemFrameSprite ;
class MenuItemScale9Sprite;
class HttpSprite;
struct FamilyRoleBaseInfo;
class FrameSpriteButton;

class ChangeViceShaikh :public GameDialog
{
public:
	CREATE_FUNC(ChangeViceShaikh) ;

	bool init() ;
	void  updateView() ;

protected:
	void onMenuItemSelected(cocos2d::Ref * ref) ;

	struct RoleIconItem
	{
		//cocos2d::Sprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
		cocos2d::Node* m_pSpriteBgItem ;
		LayerButton *m_pBtDetail;
		HttpSprite *m_pSpriteRoleIcon;	
		cocos2d::Label *m_uName;	

	};

	//获取列表显示
	RoleIconItem *GetFamilyListItem(FamilyRoleBaseInfo baseInfo);
	void UpdateItem(ChangeViceShaikh::RoleIconItem *item, const FamilyRoleBaseInfo& roleInfo);
	void menuClickBgItem(FamilyRoleBaseInfo baseInfo,ChangeViceShaikh::RoleIconItem *item);
	virtual void onGrabTouchBegan(bool bHitDialog);
private:
	cocos2d::Label                              *m_lableTips;                   //提示
	cocos2d::extension::ScrollView              *m_ScrollView;					//滚动视图
	cocos2d::Label                              *m_lableTipsup;                   //提示
	cocos2d::Label                              *m_lableTipsdown;                   //提示

	MenuItemScale9Sprite                         *m_bttoViceShaikh ;
	MenuItemScale9Sprite                         *m_btclose ;

	cocos2d::Sprite								*m_spriteselect ;							
	int m_curUID ;

	std::vector <RoleIconItem*>					m_vecNoPowerRoleOld;		//原始数据

	GameLayer								    *m_layerTop;
	GameLayer									*m_layerBottom;

};


#endif
