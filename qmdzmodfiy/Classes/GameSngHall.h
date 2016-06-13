#ifndef _GAMESNGHALL_H_
#define  _GAMESNGHALL_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
//#include "FrameSpriteButton.h"
struct GlobalSngTableInfo ;
struct GlobalSngHallInfo ;
class HttpSprite ;

class SngRule ;
class SngSignUp ;
class FrameScale9SpriteButton;

class GameSngHall :public GameDialog ,public cocos2d::extension::ScrollViewDelegate
{
public:

	CREATE_FUNC(GameSngHall) ;

	bool init() ;

	void onSetVisible(bool bVisible) ;

	void updateGameSngData() ;
	void showBlack(bool flag) ;

	void updateItemList(const GlobalSngTableInfo& followInfo) ;
    

	void updateTableScrollView() ;

	virtual void onKeyEscPressed() override;

	//void showSngSignUp(bool flag) ;//显示sng报名界面
private:
	void onMenuItemBottomSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type);
	void onGrabTouchBegan(bool bHitDialog) ;


	void creatTableScrollView() ;

	void menuClickJiaRu( const GlobalSngTableInfo& followInfo) ;

	void cleanTableScrollView() ;
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) ;
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) ;

	void showSngRule(bool flag) ;
	//void showSngRankList(bool flag) ;
	void OnUpdateMyData(unsigned uDataType) ;
	void callHallAction() ;
public:


	struct SngTableInfo
	{
		cocos2d::Sprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
		cocos2d::Sprite* m_spriteIcon ;

		cocos2d::Label* m_labelName ;
		cocos2d::Label* m_labelMaxPlayerCount ;
		cocos2d::Label* m_LableroleInGame ;
		cocos2d::Sprite* m_spriteCup[3] ;
		cocos2d::Label* m_Lablereward[3] ;
		cocos2d::Sprite* m_spriteFee ;
		cocos2d::Label* m_lableFee ;

		cocos2d::Sprite* m_spriteFreesticket ;
		cocos2d::Label* m_labelFreeStickCount ;

		cocos2d::Label* m_labelMyStickCount ;

		FrameScale9SpriteButton *m_pBtJiaRu;					//加入按钮

		unsigned m_tableId ;
		unsigned m_needStick ;
	};
	//获取列表显示
	SngTableInfo* GetSngTableListItem(unsigned tableId);
	void UpdateItem(GameSngHall::SngTableInfo *item, const GlobalSngTableInfo& followInfo);
    //SNG帮助界面提示
    void createHelpGuide();
private:
	std::vector<SngTableInfo*>		m_vecSngTableListInfoNew;
	cocos2d::Sprite* m_spriteIcon[5] ;


	cocos2d::extension::ScrollView* m_pScrollView ;
	cocos2d::Size sizeListView;

	cocos2d::Node* s_uiroot ;
	cocos2d::ui::Layout* s_layoutTable ;
	//cocos2d::ui::Layout* s_layoutHelp ;
	cocos2d::ui::Layout *m_pSpriteBgTable;

	cocos2d::ui::Layout *m_rightupButton;
	cocos2d::ui::Layout *m_leftupButton;
	cocos2d::ui::Layout *m_middleShow;
	cocos2d::ui::Layout *m_middleTips;
	
	cocos2d::ui::Layout *m_SngTips;
	//按钮
	cocos2d::ui::Button* m_buttonBack ;
	cocos2d::ui::Button* m_buttonhelp;
	cocos2d::ui::Button* m_buttonrank;
	cocos2d::ui::Button* m_buttonshop;
	cocos2d::ui::Button* m_buttonkefu;
	cocos2d::ui::Button* m_buttonTips;

	cocos2d::ui::Button* m_buttonTouch;
	cocos2d::ui::Button* m_buttonTouch2;
	//人物信息
	cocos2d::ui::Text* m_textName ;
	cocos2d::ui::Text* m_textMoney ;
	cocos2d::ui::Text* m_textrank ;
	cocos2d::ui::Text* m_textDashiRank ;
	HttpSprite* m_spriteHero ;



private:

	cocos2d::LayerColor  *m_layerColorBg;
	SngRule* m_sngRule ;
	//SngRankList* m_sngRankList ;
	float m_tempDis ;

};
#endif
