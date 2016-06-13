#ifndef _ANIMATIONMANAGER_H_
#define  _ANIMATIONMANAGER_H_
#include "cocos2d.h"
#include "UICommon.h"
class AnimationManager :public cocostudio::Armature
{
public:

	static AnimationManager* creatArmature(const std::string& name) ;
	bool initArmature(const std::string& name);
	void play(const std::string& name) ; 
	void playByIndex(int index) ;
	void play(const std::string& name,int loopCount) ; 
	void playByIndex(int index,int loopCount) ; 
	void playByIndexAndRepatForever(int index) ; 
	void playOnceByIndex(int index) ; 
	void playOnceByIndexAndStopAtLastFrame(int index) ;

	void pauseAni() ;
	void stopAni() ;



	void playTools() ;
	void SetFinishCB(std::function<void()> cbFinished);

public:
	std::function<void()> m_cbFinished ;
private:

	void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);

private:
	int s_curPlayIndex ;

	int s_maxloopCount ;
	int s_curLoopCount ;

	bool s_stopAtLastFrame ;
	bool s_repatForever;

	
};


#endif

