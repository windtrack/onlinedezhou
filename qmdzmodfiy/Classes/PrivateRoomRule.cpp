#include "PrivateRoomRule.h"
USING_NS_CC ;

bool PrivateRoomRule::init()
{
	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	Node* s_uiroot = CSLoader::createNode("res/ui/private_rule.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(s_uiroot,1);


	return true ;
}

void PrivateRoomRule::setVisible(bool bVisible)
{
	if (bVisible)
	{
		callShowAction() ;
	}
	else
	{
		callHideAction() ;
	}
}