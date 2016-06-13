#ifndef __DEALER_H__
#define __DEALER_H__

#include "cocos2d.h"
#include "UICommon.h"
class TextureNumber;


const std::string heguanAni_map[6]=
{
	"changtai",
	"zhayan",
	"qiaozhuozi",

	"waitouzhayan",
	"dashang01",
	"dashang02",

};



class Dealer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(Dealer);

	void PlayAniKiss();

private:
	enum AnimateType
	{
		AniNone = 0,
		AniEye,
		AniEyeMouse,
		AniEyeHand,
		AniKissBack1,
		AniKissBack2
	};
	void onSchedulePlayAni(float fDelta);
	void PlayAnimate(AnimateType aniType);

	void onScheduleEachTime();

	void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

private:
	//cocos2d::Sprite *m_pSpriteDealer;
	AnimateType m_playingAniType;


	Armature* heguanArmature ; 
	ArmatureAnimation* heguanAnimation ;

};

class Reward : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(Reward);

	void SetValue(double dFee);

private:
	TextureNumber *m_pFeeNumber;
};

#endif