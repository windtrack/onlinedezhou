#ifndef __GAMEHALL2_H__
#define __GAMEHALL2_H__

#include "GameDialog.h"
#include "UICommon.h"
class MenuItemSpriteWithLabel;
class MenuItemFrameSprite;
class DeZhouMagnate;
class LayerButton;
class AnimateViewer;
class AnimationManager;

class GameHall2 : public GameDialog
{
public:
	CREATE_FUNC(GameHall2);
	void UpdateView();
    void UpdateDeZhouPage();

	void refreshHallInfo();

	void reSetActivateInsex();
protected:
	virtual bool init();
	virtual void onSetVisible(bool bVisible) override;
	virtual void onKeyEscPressed() override;
    
    virtual void onGrabTouchBegan(bool bHitDialog) override;
    
	void onBtSwitchTable6or9(cocos2d::Ref *pBt);
	void onBtQuickEnter(cocos2d::Ref *pBt, cocos2d::ui::TouchEventType type);

	void Activate(unsigned uIdx);

	void SetAreaPlayersCount(unsigned uArea, unsigned uCount);

	void OnMyDataChanged(unsigned uDataType);

	void onGameHall2TypeSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type);
    //初出茅庐 小有成就。。场次引导
    void createSceneGuid();
    //贵族开房引导
    void createVipDefineGuid();
    //视频桌6人桌切换按钮引导
    void createSwitchVideoOr6Guid();

private:
	unsigned m_uCurIdx;
	unsigned m_uButtionIndex;
	AnimationManager *m_pVipSpriteAni;
	cocos2d::ui::Layout *m_pSpriteBgTable;
	cocos2d::ui::ImageView *m_pSpriteBgCard;
	cocos2d::ui::Button *m_pBtBack;
	cocos2d::ui::Button *m_pBtSwitch[3];
	cocos2d::ui::ImageView *m_pImageMenuBg;
	cocos2d::Label      *m_pLabelSwitch[3];
	cocos2d::ui::Text *m_pBtText[3];
	cocos2d::ui::Text *m_pLabelAllInfo;
	cocos2d::ui::Text *m_pLabelCash;
	cocos2d::ui::ImageView *m_pSprite$;
	cocos2d::ui::Button *m_pBtEnterMall;
	cocos2d::ui::Button *m_pBtEnterKeFu;

	cocos2d::Menu *m_pMenu;
	cocos2d::MenuItemToggle * m_pRadio6Or9;

	cocos2d::ui::Button *m_pBtQuickDoors[6];
	cocos2d::ui::ImageView *m_pSpriteBlindInfo[6];

	cocos2d::ui::Text *m_pLabelMaxCarry[6];
	cocos2d::ui::ImageView *m_pSpritePlayersFlag[6];
    cocos2d::ui::ImageView *m_pSpriteCamera[6];
	cocos2d::ui::Text *m_pLabelPlayers[6];
	// TextureNumber *m_pNumPlayers[6];
	cocos2d::ui::Text *m_pLabelBlindInfo[6];
	cocos2d::ui::ImageView *m_pImageLine;
    DeZhouMagnate *m_pDeZhouMagnate;
};


#endif