#ifndef _DAILYSIGN_H__
#define _DAILYSIGN_H__

#include "cocos2d.h"
#include "GameDialog.h"
class FrameSprite;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel ;
class HttpSprite ;
class LineLabel ;


class ContinueSignReward : public GameDialog
{
public:
	//static DailySignReward *create(std::string strRewardName, unsigned uNum);
	CREATE_FUNC(ContinueSignReward);

	void setVisible(bool bVisible) ;
	void updateView() ;
	void onGrabTouchBegan(bool bHitDialog) ;
protected:
	virtual bool init();
	void OnImmediatelyCheck(cocos2d::Ref *pSend);

	void doRotate(float delta) ;
	void onMenuItemCloseSelected(cocos2d::Ref *) ;

	
private:
	MenuItemScale9SpriteWithLabel *m_menuCheck;
	cocos2d::Sprite* shineSprite ;
	LineLabel			*m_messageLabel ;
	cocos2d::Label* m_lableDays;
	HttpSprite* ticketSprite ;

	cocos2d::Node* m_Node ;
};



class DailySign : public GameDialog
{
public:
    //static DailySign* create(unsigned size, unsigned *status, unsigned *money);
     CREATE_FUNC(DailySign);
	 void setVisible(bool bVisible) ;
	 void onGrabTouchBegan(bool bHitDialog) ;
	 void onMenuItemCloseSelected(cocos2d::Ref *) ;

	 void updateView() ;
protected:
    virtual bool init();
    bool initWithGoldReward(unsigned size, unsigned *status, unsigned *money);
    bool initWithTicketReward();
    void OnImmediatelyGetGold(cocos2d::Ref *pSend);
    void OnImmediatelyGetSiginReward(cocos2d::Ref *pSend);

	void doLoadProgressbar() ;
	void callContinousSiginReward() ;

	void showContinueReward(bool flag) ;
private:
    FrameSprite             *m_frameSprite[5];
    cocos2d::Sprite         *m_spriteAlreadyGet[5];
    cocos2d::Label          *m_labelGold[5];
    MenuItemScale9SpriteWithLabel *m_menuImmediatelyGetGold;
    MenuItemScale9SpriteWithLabel *m_menuImmediatelyGetTicket;

	cocos2d::Sprite			*m_scaleSprite ;

	cocos2d::ProgressTimer			*m_loadProgress ;
	cocos2d::Label          *m_labelButton;
	ContinueSignReward* mReward ;


	cocos2d::Sprite         *m_spriteSignGet[7];
	cocos2d::Sprite *itemBgSprite[7];

	cocos2d::Sprite *itemBgSpriteToday;

	cocos2d::Label *m_labelGet;
};








class DailySignReward : public GameDialog
{
public:
    //static DailySignReward *create(std::string strRewardName, unsigned uNum);
    CREATE_FUNC(DailySignReward);

	void setVisible(bool bVisible) ;
	void updateView(bool isCash) ;
protected:
    virtual bool init();
    void OnImmediatelyCheck(cocos2d::Ref *pSend);

	void doRotate(float delta) ;
	void onMenuItemCloseSelected(cocos2d::Ref *) ;
	
	//void update(float delta) ;

private:
    MenuItemScale9SpriteWithLabel *m_menuCheck;
	cocos2d::Sprite* shineSprite ;
	LineLabel			*m_messageLabel ;
	HttpSprite* ticketSprite ;


};

#endif