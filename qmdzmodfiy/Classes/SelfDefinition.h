#ifndef __SELFDEFINITION_H__
#define __SELFDEFINITION_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"

class FrameSpriteButton;
class FrameScale9SpriteButton;
struct DefineRoomInfo;

class SelfDefinition : public GameDialog,public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(SelfDefinition);


	void initWithDefaultValue() ;

	void setVisible(bool bVisible) ;

	void doUpdateRoomName(std::string name) ;
	inline void setCurRoomType(unsigned type) {m_curRoomType = type ;};
protected:
    virtual bool init();
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);


	void onSliderChanged(cocos2d::Ref* sender, cocos2d::ui::SliderEventType controlEvent );
	void OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type);
private:

	
	//确定创建
	void CreateRoom();

	void cleanTips() ;
private:
	unsigned						m_curRoomType ;


	cocos2d::Node					*s_uiroot ;
	cocos2d::ui::Slider				*m_slider_playerNum;
	cocos2d::ui::Slider				*m_slider_gameJijin;


	cocos2d::ui::Text				*m_text_playerNum ; //最大玩家数
	cocos2d::ui::Text				*m_text_jiJin ;//基金

	cocos2d::ui::Text				*m_text_valueDeflaut[7] ;
	cocos2d::ui::Text				*m_text_valueMod[7] ;
	cocos2d::ui::ImageView			*m_imgbg[7] ;

	cocos2d::ui::EditBox			*m_EditMod[7];

	cocos2d::ui::Button				*m_btCreatRoom ;

	std::string								m_roomName;//房间名称
	int										m_playerNum ; //最大玩家数
	int										m_defineFundp ;//基金
	unsigned								m_minCollectCash; //最小起收
	unsigned								m_maxCollectCash; //最大封顶
	unsigned								m_minCarryCash;//最小携带
	unsigned								m_maxCarryCash;//最大携带
	unsigned								m_minBlind;//大小盲
	unsigned								m_anteMoney;//前注
	
	std::vector<cocos2d::ui::EditBox*> m_vecEditBox;
	unsigned                            m_uRoomType;



	cocos2d::ui::Button				*m_btFund ;
	cocos2d::ui::Button				*m_btMinCollect ;
	cocos2d::ui::Button				*m_btMaxCollect ;


	cocos2d::ui::ImageView			*m_imgFund ;
	cocos2d::ui::ImageView			*m_imgMinCollect ;
	cocos2d::ui::ImageView			*m_imgMaxCollect ;

	//遮罩
	cocos2d::ui::Button* m_buttonTouchAll;
};

#endif