#include "Delaer.h"
#include "Util.h"
#include "TextureNumber.h"

USING_NS_CC;

bool Dealer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//m_pSpriteDealer = Sprite::create("delaer/delaer.webp");
	//setContentSize(m_pSpriteDealer->getContentSize());
	//ADD_NODE_CENTER(m_pSpriteDealer, 0);

	//m_playingAniType = -1;
	scheduleOnce(schedule_selector(Dealer::onSchedulePlayAni), (rand() % 7) + 1.0f);

	//加载荷官

	heguanArmature = Armature::create("heguan");  
	heguanArmature->ignoreAnchorPointForPosition(false);
	heguanArmature->setAnchorPoint(Point(0.5,0.5));
	heguanArmature->setPosition(0,0) ;
	this->addChild(heguanArmature);  
	heguanAnimation = heguanArmature->getAnimation() ;

	heguanArmature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Dealer::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 
	PlayAnimate(AniNone) ;
	return true;
}

void Dealer::PlayAniKiss()
{
	// if (m_playingAniType != AniKissBack1 && AniKissBack1 != AniKissBack2)
	if (m_playingAniType == AniNone)
	{
		PlayAnimate(AnimateType(AniKissBack1 + rand() % 2));
	}	
}

void Dealer::onSchedulePlayAni(float fDelta)
{
	if (m_playingAniType == AniNone)
	{
		PlayAnimate(AnimateType(rand() % (unsigned)AniEyeHand + 1));
	}
	else
	{
		scheduleOnce(schedule_selector(Dealer::onSchedulePlayAni), (rand() % 7) + 1.0f);
	}
}

void Dealer::PlayAnimate(Dealer::AnimateType aniType)
{
	if (aniType!=m_playingAniType)
	{
		m_playingAniType = aniType;
		heguanAnimation->play(heguanAni_map[aniType]);
	}


	//if (m_playingAniType != AniNone)
	//{
	//	m_pSpriteDealer->stopAllActions();
	//	m_pSpriteDealer->setVisible(true);
	//	m_playingAniType = AniNone;
	//}
	//m_playingAniType = aniType;
	//auto funEndPlaying = [this]()
	//{
	//	m_pSpriteDealer->stopAllActions();
	//	m_playingAniType = AniNone;
	//	m_pSpriteDealer->setVisible(true);
	//	scheduleOnce(schedule_selector(Dealer::onSchedulePlayAni), (rand() % 5) + 1.0f);
	//};
	//if (m_playingAniType == AniEye)
	//{
	//	auto actAnimate = CreateAnimate("delaer/animatedealereye.webp", 6, 2, 10);
	//	m_pSpriteDealer->runAction(Sequence::create(actAnimate, Show::create(), CallFunc::create(funEndPlaying), nullptr));
	//}
	//else if (m_playingAniType == AniEyeMouse)
	//{
	//	auto actAnimate = CreateAnimate("delaer/animatedealereyemouse.webp", 6, 2, 11);
	//	m_pSpriteDealer->runAction(Sequence::create(actAnimate, Show::create(), CallFunc::create(funEndPlaying), nullptr));
	//}
	//else if (m_playingAniType == AniEyeHand)
	//{
	//	auto actAnimate = CreateAnimate("delaer/animatedealereyehand.webp", 6, 2, 11);
	//	m_pSpriteDealer->runAction(Sequence::create(actAnimate, Show::create(), CallFunc::create(funEndPlaying), nullptr));
	//}
	//else if (m_playingAniType == AniKissBack1)
	//{
	//	auto actAnimate = CreateAnimate("delaer/animatedealerkissback1.webp", 6, 3, 16);
	//	m_pSpriteDealer->runAction(Sequence::create(actAnimate, Show::create(), CallFunc::create(funEndPlaying), nullptr));
	//}
	//else if (m_playingAniType == AniKissBack2)
	//{
	//	auto actAnimate = CreateAnimate("delaer/animatedealerkissback2.webp", 6, 3, 18);
	//	m_pSpriteDealer->runAction(Sequence::create(actAnimate, Show::create(), CallFunc::create(funEndPlaying), nullptr));
	//}
}

void Dealer::onScheduleEachTime()
{
	m_playingAniType = AniNone;
	heguanArmature->setVisible(true);
	scheduleOnce(schedule_selector(Dealer::onSchedulePlayAni), (rand() % 5) + 1.0f);
}

void Dealer::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{

	//if (movementType == COMPLETE)
	//{
	//	return ;
	//}
	if (movementType == LOOP_COMPLETE||movementType == COMPLETE)
	{
		if (movementID == heguanAni_map[AniNone])
		{	
			armature->runAction(Sequence::create(CallFunc::create( CC_CALLBACK_0(Dealer::onScheduleEachTime, this)), nullptr));
			PlayAnimate(AniNone) ;
		}
		else if (movementID == heguanAni_map[AniEye])
		{		
			armature->runAction(Sequence::create(CallFunc::create( CC_CALLBACK_0(Dealer::onScheduleEachTime, this)), nullptr));
			PlayAnimate(AniNone) ;
		}
		else if (movementID == heguanAni_map[AniEyeMouse])
		{	
			armature->runAction(Sequence::create(CallFunc::create( CC_CALLBACK_0(Dealer::onScheduleEachTime, this)), nullptr));
			PlayAnimate(AniNone) ;
		}
		else if (movementID == heguanAni_map[AniEyeHand])
		{	
			armature->runAction(Sequence::create(CallFunc::create( CC_CALLBACK_0(Dealer::onScheduleEachTime, this)), nullptr));
			PlayAnimate(AniNone) ;
		}
		else if (movementID == heguanAni_map[AniKissBack1])
		{	
			armature->runAction(Sequence::create(CallFunc::create( CC_CALLBACK_0(Dealer::onScheduleEachTime, this)), nullptr));
			PlayAnimate(AniNone) ;
		}
		else if (movementID == heguanAni_map[AniKissBack2])
		{		
			armature->runAction(Sequence::create(CallFunc::create( CC_CALLBACK_0(Dealer::onScheduleEachTime, this)), nullptr));
			PlayAnimate(AniNone) ;
		}
	}
}

bool Reward::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto pBg = Sprite::create("delaer/bgreward.png");
	Size sz = pBg->getContentSize();
	setContentSize(sz);
	pBg->setPosition(sz.width / 2, sz.height / 2);
	addChild(pBg, 0);

	m_pFeeNumber = TextureNumber::create("delaer/feenumber.png", "delaer/feedot.png", "delaer/feeunit.png", false);
	m_pFeeNumber->setPosition(sz.width / 2, sz.height);
	m_pFeeNumber->SetVAlignment(TextureNumberVAlignment::Bottom);
	m_pFeeNumber->setCascadeOpacityEnabled(true);
	addChild(m_pFeeNumber);
	return true;
}

void Reward::SetValue(double dFee)
{
	m_pFeeNumber->UpdateNumber(dFee);
}

