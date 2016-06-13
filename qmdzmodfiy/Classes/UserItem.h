#ifndef _USERITEM_H_
#define _USERITEM_H_
/************************************************************************/
/* 
modify by sunjinfang 2015-9-15
商城买的道具 自动为使用状态


*/
/************************************************************************/

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
//#include "FrameSpriteButton.h"

class DataLoading ;
class HttpSprite ;
class FrameScale9SpriteButton;
class FrameSprite;
struct UserItemInfo ;
class UserItem : public GameDialog
{
public:
	CREATE_FUNC(UserItem) ;
	bool init() ;

	void updateItemView() ;
	void setShowDataLoading(bool flag) ;
	void setVisible(bool bVisible) ;
public:

	struct UItemLabel
	{
		FrameSprite		    *m_pSpriteBGItem;		// 滚动视图行的背景
		HttpSprite			*m_pSpriteIcon;			// 头像
		cocos2d::Sprite	    *m_pSpriteCountBlock ;	//道具数量背景框
		cocos2d::Label		*m_pLabelCount ;		//道具数量
		cocos2d::Label		*m_pLabelName;			//道具名称
		cocos2d::Label		*m_pLabelExpDate;		//有效期
		cocos2d::Label		*m_pLabelDes;			//道具描述
		FrameScale9SpriteButton   *m_pBt;					//按钮分类型显示文字	
		cocos2d::Label		*m_pLabelBtDes;			//按钮文字描述
		unsigned			*m_count ;				//
	};
	//获取列表显示
	UItemLabel* GetListItem(const UserItemInfo& itemInfo);
	void UpdateItem(UserItem::UItemLabel *item, const UserItemInfo& itemInfo);

private:
	void menuClickItem(UserItemInfo baseInfo);		//道具功能回调
	void callItemShop() ;							//打开道具商城
	void cleanView() ;
private:
	cocos2d::extension::ScrollView* m_ScrollView ;
	cocos2d::Size sizeListView;

	std::vector<UItemLabel*>	    m_vecItemListOld;
	DataLoading* s_dataLoading ;


	cocos2d::Node	*m_withoutItemLayer ;			//没有道具时的界面显示
	cocos2d::Sprite *m_spriteGril ;					//显示个妹子
	cocos2d::Label *m_lableWithoutItem ;			//没有道具的提示
	FrameScale9SpriteButton   *m_btItemShop;				//按钮分类型显示文字

};




#endif