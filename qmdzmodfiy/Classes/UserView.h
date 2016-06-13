#ifndef __USERVIEW_H__
#define __USERVIEW_H__

#include "cocos2d.h"
#include "GameCommon.h"
#include "RadioGroup.h"
#include "GameTable.h"
#include "UICommon.h"

class HttpSprite;
class FrameSprite;
class AnimateViewer;
class AnimationManager;



#define US_NORMAL 0
#define US_THINK 1
#define US_CHECK 2
#define US_CALL  4
#define US_RAISE 8
#define US_ALLIN 16
#define US_FOLD  32
#define US_OPENCARD  64
#define US_SHOWCARD  128
#define US_WINTYPE	256
#define US_WAITNEXTGAME 512


#define  THINKINGPART 9 
enum class UserAction
{
	Check,		// 让牌
	Call,		// 跟注
	Raise,		// 加注
	AllIn,		// AllIn
	Fold,		// 弃牌
};

class UserView : public cocos2d::Layer
{
public:

	static UserView* creatWithLayer(UserPos &pos) ;
	bool initWithLayer(UserPos &pos) ;

	virtual bool init(); 
	CREATE_FUNC(UserView);

	void SetCash(double dCash);
	void SetIconUrl(const std::string& strIconUrl);
	void SetVipSign(bool bIfVipSign, unsigned uLevel);
	void SetNickName(const std::string& strNickName);
	void SetBiaoJi(int uBiaoJiIdx);
    void SetLady(unsigned uLadyId);

	void ShowNickName();
	void ShowWaitNextGame();
	void ShowThinking();
	void ShowAction(UserAction userAction);
	void ShowCard(unsigned char cbCardData[2]);
	void ShowOpenCard(unsigned char cbCardData[2]);
	void ShowWinType(unsigned uWinType);
	void ActivateStatus(unsigned uActivateFlag);
	unsigned GetActivateFlag();

	
	// void ShowOpacity(bool bOpacity);

	void ShowExpress(unsigned uIdx);
    void DryCashMinHandCard(unsigned idx);
    void ActivateActionFold(bool bActivate);	// 弃牌
	void reset();
    void StopWinAction();

	void showSngRank(bool flag ,cocos2d::Texture2D* spriteRank) ;

	void showJoinTableEffect(int vipLev, int skinid) ;

	void callVipEffectFinished() ;
	void callChristmasEffectFinished() ;
	void callLightMove() ;
	void callNewYearEffectOver() ;


	void creatVipLight() ;
private:

	void ActivateNickName(bool bActivate);
	void ActivateThinking(bool bActivate);
	void ActivateActionCheck(bool bActivate);	// 让牌
	void ActivateActionCall(bool bActivate);	// 跟注
	void ActivateActionRaise(bool bActivate);	// 加注
	void ActivateActionAllIn(bool bActivate);	// AllIn
	//void ActivateActionFold(bool bActivate);	// 弃牌
	void ActivateShowCard(bool bActivate);		// 摊牌
	void ActivateOpenCard(bool bActivate);		// 开牌
	void ActivateWinType(bool bActivate);		// 赢牌类型
	void ActivateWaitNextGame(bool bActivate);		// 等待开始




	void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

	void doFade() ;

private:



	AnimationManager* m_pAniShowWin ; 

private:

	UserPos m_userPos ;

	std::string m_strNickName;
	std::string m_strIconUrl;
	unsigned char m_cbCardData[2];
	unsigned m_uWinType;

	cocos2d::Sprite *m_pSpriteBg;
	HttpSprite *m_pHttpSprite;
	
	//AnimationManager *m_pAniShowWin[3];

	cocos2d::Label *m_pLabelNickName;
	cocos2d::Label *m_pLabelUsrAction;
	cocos2d::Label *m_pLabelFold;
	cocos2d::Label *m_pLabelCash;
	cocos2d::Label *m_pLabelThinking;
	cocos2d::Node *m_pSpriteAniThinking;
	//cocos2d::Sprite *m_pSpriteAniThinking;
	AnimationManager *m_pSpriteAniAllIn;
	//cocos2d::Sprite *m_pSpriteAniAllIn;
	cocos2d::Sprite *m_pSpriteAniShowCard1;
	cocos2d::Sprite *m_pSpriteAniShowCard2;
	cocos2d::Sprite *m_pSpriteAniOpenCard;
	
	AnimationManager *m_pSpriteAniTipShowCard;//亮牌动画
	//cocos2d::Sprite *m_pSpriteAniTipShowCard;//亮牌动画
	cocos2d::Sprite *m_pSpriteWinType;
	FrameSprite *m_pSpriteCard[2];
	FrameSprite *m_pSpriteVipSign;
	FrameSprite *m_pSpriteBiaoJi;
    FrameSprite *m_pSpriteLady;
    cocos2d::Sprite *m_pSpriteCardCash[2];// 2张手牌灰化

	//cocos2d::Sprite * m_pSpriteAnimation;
	//AnimationManager* m_biaoqingAni[2];//贵族和普通表情被美术拆成两个动画

	RadioGroup m_radioGroupTopView;
	unsigned m_uActivateFlag;
	bool m_bActivate ;
	bool m_isEffectShow ;
	//sng
	cocos2d::Sprite* m_spriteRank ;
	cocos2d::Sprite* m_spriteSngAuto ;


	AnimateViewer *m_vipEffect ; //vip特效
	AnimateViewer *m_christmasEffect ;//圣诞特效
	AnimationManager* m_aniNewYear ;//新年特效

	AnimationManager* m_aniMonkey ;//猴子特效

	cocos2d::ClippingNode* m_clipNode;
	cocos2d::Sprite* m_spriteLight ;
	bool  m_ligheMoveing ;
private:

	void updateThinking(cocos2d::Ref* pSender,int index) ;
	void updateThinkingColor(int index) ;
	void initThinkingData() ;

	//创建 拐角的进度条
	cocos2d::ProgressTimer* creatProgressTimeBar(cocos2d::Sprite* spriteImg,cocos2d::ProgressTimer::Type type,cocos2d::Vec2 midpoint,cocos2d::Vec2 barChangeRate) ;
	void reSetThinkingBar() ;
	void runThinking() ;
	void stopThinking() ;
	void callPointShow() ;
private: 

	float s_maxThinkingTime  ;
	cocos2d::Sprite* spriteThinking[THINKINGPART] ;
	cocos2d::ProgressTimer* s_ProgressTimer[THINKINGPART] ;
	float s_ThinkingTime[THINKINGPART] ;
	cocos2d::Node* s_thinkingLayer  ;
	cocos2d::Sprite* spritePoint ;
	cocos2d::Node* nodeColorAction ;
};



#endif