#include "MttRule.h"

#include "Tools.h"
#include "GameFrame.h"
USING_NS_CC ;
bool MttRule::init()
{

	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	Node* s_uiroot = CSLoader::createNode("res/ui/mttrule.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;

	//rootNode->setScale(0.5);
	this->addChild(s_uiroot,1);

	return true ;
}

void MttRule::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	PlayVisibleSound(bVisible);
	if (bVisible)
	{
		Tools::callScaleAction(this) ;

	}
	else
	{
		SendClientRequest(ClientRequest::kOptMTTHallBgLayer);

	}

}