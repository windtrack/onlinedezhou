#include "MttQuitMatch.h"

#include "Util.h"
#include "MenuItemScale9Sprite.h"
#include "Tools.h"
#include "LineLabel.h"
#include "FrameSpriteButton.h"
USING_NS_CC ;
bool  MttQuitMatch::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false ;
	}
	Size sz = this->getContentSize() ;


	//CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"报名成功",Point(0.5,0.5),337,316,Color4B(0xff,0xd2,0x00,0xff),this,1) ;





	m_lineUp = LineLabel::create() ;
	m_lineUp->Appand(28,"fzlt.ttf","退赛成功！",Color4B::WHITE);
	m_lineUp->ignoreAnchorPointForPosition(false) ;
	m_lineUp->setAnchorPoint(Point(0.5,0.5)) ;
	m_lineUp->setPosition(Point(sz.width/2,254)) ;
	this->addChild(m_lineUp);

	m_lineDown = LineLabel::create() ;
	m_lineDown->Appand(28,"fzlt.ttf","报名费用已返回账户",Color4B::WHITE);
	m_lineDown->ignoreAnchorPointForPosition(false) ;
	m_lineDown->setAnchorPoint(Point(0.5,0.5)) ;
	m_lineDown->setPosition(Point(sz.width/2,196)) ;
	this->addChild(m_lineDown);



	ccMenuCallback menuCB2 = CC_CALLBACK_1(MttQuitMatch::onMenuItemClick, this);
	//m_btclose =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
	m_btSure =MenuItemScale9Sprite::create(0, Size(226, 70),menuCB2);
	m_btSure->setPosition(sz.width/2,58);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(26,"确定",Vec2(0.5,0.5),m_btSure->getContentSize().width/2,m_btSure->getContentSize().height/2,Color4B::WHITE,m_btSure,0);
	auto pMenu = Menu::create(m_btSure,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 100);


	m_time = 0 ;
	return true ;
}

void MttQuitMatch::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		Tools::callScaleAction(this) ;
	}
	else
	{
		this->stopAllActions() ;
		SendClientRequest(ClientRequest::kOptMTTHallBgLayer);
	}
}

void MttQuitMatch::onGrabTouchBegan(bool bVisible)
{
	if (bVisible)
	{
	}
}

void MttQuitMatch::onMenuItemClick(cocos2d::Ref *)
{
	this->stopAllActions() ;
	setVisible(false) ;

}
// 0金币  1 门票  2免费  
void MttQuitMatch::updateView(int type)
{
	//this->stopAllActions() ;
	//m_matchName = name ;
	//m_tableID = tableid ;

	m_lineDown->resetLable() ;

	//if (type)
	//{
	//	m_lineDown->Appand(28,"fzlt.ttf","门票已返还账户",Color4B::WHITE);
	//}
	//else
	//{
	//	m_lineDown->Appand(28,"fzlt.ttf","报名费用已返回账户",Color4B::WHITE);
	//}
	m_lineDown->setVisible(true) ;
	if (type ==0)
	{
		m_lineDown->Appand(28,"fzlt.ttf","报名费用已返回账户",Color4B::WHITE);
	}
	else if (type ==1)
	{
		m_lineDown->Appand(28,"fzlt.ttf","门票已返还账户",Color4B::WHITE);
	}
	else if (type ==2)
	{
		m_lineDown->setVisible(false) ;
	}
	

	/*	m_lineDown->ignoreAnchorPointForPosition(false) ;
	m_lineDown->setAnchorPoint(Point(0.5,0.5)) ;
	m_lineDown->setPosition(Point(sz.width/2,196)) */;


	//char buff[64] ;
	//m_lineUp->resetLable() ;
	//m_lineDown->resetLable() ;

}