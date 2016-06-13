#include "AnimationManager.h"


AnimationManager* AnimationManager::creatArmature(const std::string& name)
{
	auto animMgr = new (std::nothrow)AnimationManager();
	if (animMgr && animMgr->initArmature(name)) {
		animMgr->autorelease();
		return animMgr;
	} else {
		CC_SAFE_DELETE(animMgr);
		return nullptr;
	}
}

bool AnimationManager::initArmature(const std::string& name)
{
	if (! Armature::init(name))
		return false;
	s_curPlayIndex=0;
	s_curLoopCount =0 ;
	s_maxloopCount =0 ;
	s_stopAtLastFrame = false ;
	s_repatForever = false ;
	this->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(AnimationManager::animationEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); 
	return true ;
}

void AnimationManager::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{

	if (s_stopAtLastFrame)
	{
		return ;
	}
	if (movementType == LOOP_COMPLETE || movementType==COMPLETE )
	{

		if (m_cbFinished)
		{
			m_cbFinished() ;
		}

		if (s_repatForever)
		{
			this->getAnimation()->playByIndex(s_curPlayIndex);
			return ;
		}

		s_curLoopCount++;
		if (s_curLoopCount>=s_maxloopCount)
		{
			this->stopAni() ;
			//this->stopAllActions() ;
			this->setVisible(false) ;
			return ;
		}
	}
}

void AnimationManager::play(const std::string& name)
{
	this->setVisible(true) ;
}
void AnimationManager::play(const std::string& name,int loopCount)
{
	this->setVisible(true) ;
	s_maxloopCount = loopCount ;
	s_curLoopCount =0 ;
	this->getAnimation()->play(name) ;
}

void AnimationManager::playByIndex(int index)
{

}
void AnimationManager::playByIndex(int index,int loopCount)
{
	s_curPlayIndex = index ;
	this->setVisible(true) ;
	s_maxloopCount = loopCount ;
	s_curLoopCount =0 ;
	s_stopAtLastFrame = false ;
	this->getAnimation()->playByIndex(index);
}

void AnimationManager::playOnceByIndex(int index)
{
	s_curPlayIndex = index ;
	s_stopAtLastFrame = false ;
	playByIndex(index,1) ;

}

void AnimationManager::playOnceByIndexAndStopAtLastFrame(int index)
{
	s_curPlayIndex = index ;
	playByIndex(index,1) ;
	s_stopAtLastFrame = true ;
}
void AnimationManager::playByIndexAndRepatForever(int index)
{
	s_curPlayIndex = index ;
	this->setVisible(true) ;
	s_repatForever= true ;
	s_curLoopCount =0 ;
	this->getAnimation()->playByIndex(index);
}

void AnimationManager::pauseAni()
{
	this->getAnimation()->pause();
}

void AnimationManager::stopAni()
{
	this->getAnimation()->stop();
}


void AnimationManager::playTools()
{

}

void AnimationManager::SetFinishCB(std::function<void()> cbFinished)
{
	m_cbFinished = cbFinished;
}