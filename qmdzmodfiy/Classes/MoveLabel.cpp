#include "MoveLabel.h"
#include "GameData.h"


void MoveLabel::doMove(std::function<void()> callBack,std::function<void()> callBack2)
{
	float m_width = this->getContentSize().width ;
	float maxWidth = GameData::getInstance()->getGameWidth() ;
	//float dis1 = abs(m_width-this->getPositionX()-200) ;
	float dis = m_width + 200 ;
	float dis1 = maxWidth - dis ;

	auto moveTo1 = MoveTo::create(dis/100*0.9,Point(this->getPosition().x-dis,this->getPositionY())) ;
	//auto moveTo1 = MoveTo::create(dis1/100*0.5,Point(this->getPositionX()-dis1,this->getPositionY())) ;
	auto action1 = [this,callBack,callBack2,maxWidth](){
		callBack() ;
		float dis2 = abs(maxWidth) ;
		auto moveTo2 = MoveTo::create(dis2/100*0.9,Point(this->getPositionX()-dis2,this->getPositionY())) ;
		auto actionend = [this,callBack2](){
			callBack2() ;
			this->removeFromParentAndCleanup(true) ;
		} ;

		this->runAction(Sequence::create(moveTo2,CallFunc::create(actionend),NULL)) ;
	} ;

	this->runAction(Sequence::create(moveTo1,CallFunc::create(action1),NULL)) ;
}