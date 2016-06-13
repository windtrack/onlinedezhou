#include "SngRule.h"
#include "GameFrame.h"
#include "Tools.h"
USING_NS_CC ;
bool SngRule::init()
{

	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	Node* s_uiroot = CSLoader::createNode("res/ui/rule.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;

	//rootNode->setScale(0.5);
	this->addChild(s_uiroot,1);

	//南山南，北秋悲
	//南山有谷堆
	//南风喃 ，北海北
	//北海有墓碑
	return true ;
}

void SngRule::setVisible(bool bVisible)
{
	//GameDialog::setVisible(bVisible) ;
    PlayVisibleSound(bVisible);
	if (bVisible)
	{
     /*   auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
        runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
		GameDialog::setVisible(bVisible) ;
		Tools::callScaleAction(this) ;
	}
	else
	{
		SendClientRequest(ClientRequest::kOptSNGHallBgLayer);
		callHideAction() ;
		/*  auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
	}

}