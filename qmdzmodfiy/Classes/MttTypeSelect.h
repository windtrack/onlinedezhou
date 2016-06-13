#ifndef _MttTypeSelect_H_
#define _MttTypeSelect_H_

#include "cocos2d.h"
#include "UICommon.h"
#include "GameDialog.h"

class LayerButton;
class TouchGrabber;
class MenuItemSpriteWithLabel;
class FrameSprite;


//enum Mtt_Type
//{
//	Match_Type_All_0=1,//全部
//	Match_Type_GameMoney_1=2,//打金赛
//	Match_Type_FULI_2=4,//福利赛
//	Match_Type_Qbi_3=8,//q币
//	Match_Type_SHIWU_4=16,//实物
//	Match_Type_WEPT_5=32,//门票赛
//	Match_Type_W_6=64,//门票赛
//	Match_Type_W_7=128,//门票赛
//	Match_Type_W_8=256,//门票赛
//	Match_Type_W_9=512,//门票赛
//	Match_Type_W_10=1024,//门票赛
//	Match_Type_W_11=2048,//门票赛
//	Match_Type_W_12=5096,//门票赛
//	Match_Type_W_13=10192,//门票赛
//};


class MttTypeSelect:public GameDialog
{
public:
	CREATE_FUNC(MttTypeSelect) ;
	bool init() ;

	void updateView() ;

	void initTypeButton() ;//初始化按钮

	void doMoveIn() ;
	void setVisible(bool visible) ;

	void onGrabTouchBegan(bool bHitDialog) ;

protected:
	void onGameTypeSelect(cocos2d::Ref* pItem) ;
	void resetAllButton() ;
private:
	cocos2d::ui::Scale9Sprite *m_spriteBG ;
	cocos2d::ui::Scale9Sprite *m_spriteRealBG ;
	cocos2d::extension::ScrollView* m_pScrollView ;
	cocos2d::Size sizeListView;

	cocos2d::Node* m_nodePos ;

	std::vector<FrameSprite*> m_editBox;

	std::vector<FrameSprite*> m_buttonType ;

	unsigned m_curTypeCount ;

	float m_curoffsetY;
};



#endif
