#include "SngOnline.h"
#include "GameTipsManager.h"
#include "GameData.h"
#include "GlobalUserData.h"

USING_NS_CC;


bool SngOnline::init()
{

	if ( this->initWithFile("newtips/tips_sngOnline.png"))
	{

	}

	return true ;
}

void SngOnline::setVisible(bool bVisible)
{
	Sprite::setVisible(bVisible) ;
	if (bVisible)
	{
		
		this->setScale(0.3) ;

		auto scaleToAction  = [this](){
			ActionInterval* scaleTo = ScaleTo::create(0.5,1.0) ;
			CCActionInterval * easeSineOut = CCEaseSineOut::create(scaleTo);
			CCActionInterval * easeBounceOut = CCEaseBounceOut ::create(easeSineOut);
			//this->setOpacity(0) ;
			//CCActionInterval * fadein = CCFadeIn::create(0.5);
			//this->runAction(Spawn::createWithTwoActions(easeBounceOut,fadein));
			this->runAction(easeBounceOut);
			Sprite::setVisible(true) ;
		} ;

	
		auto moveToAction  = [this](){
			CCMoveBy *move = CCMoveBy::create(1.0f,ccp(0,8));  
			this->runAction(CCRepeatForever::create(static_cast<CCSequence *>(CCSequence::create(move,move->reverse(),NULL))));  
		} ;

		auto hideLayer = [this](){
			this->stopAllActions() ;
			GameTipsManager::getInstance()->hideTips();
			Sprite::setVisible(false) ;
		} ;

		this->runAction(Sequence::create(CallFunc::create(scaleToAction),CallFunc::create(moveToAction),DelayTime::create(4.0f),CallFunc::create(hideLayer),NULL)) ;
		GameData::getInstance()->saveHasShowSngOnline(g_globalMyData.m_iUserId) ;
	}
    else
    {
        this->stopAllActions() ;
    }
}