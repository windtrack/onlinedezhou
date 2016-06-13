#include "MttSignUpSuccess.h"
#include "Util.h"
#include "MenuItemScale9Sprite.h"
#include "Tools.h"
#include "GameCommon.h"
USING_NS_CC ;
bool  MttSignUpSuccess::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false ;
	}
	Size sz = this->getContentSize() ;
	Sprite* m_spriteTitle = Sprite::create("gamemtt/mtt_bmtitle.png");
	m_spriteTitle->setPosition(sz.width/2,337) ;
	this->addChild(m_spriteTitle);

	CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"报名成功",Point(0.5,0.5),337,316,Color4B(0xff,0xd2,0x00,0xff),this,1) ;

	int px = 25 ;
	CreateLabelMSYHAndAnchorPosClrAddTo(26,"赛事名称：",Point(0,0.5),91-px,251,Color4B::WHITE,this,1) ;

	m_labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Point(0,0.5),218-px,251,Color4B(0xff,0xd2,0x00,0xff),this,1) ;

	CreateLabelMSYHAndAnchorPosClrAddTo(26,"报名费用：",Point(0,0.5),91-px,203,Color4B::WHITE,this,1) ;
	Sprite* m_spriteMoney = Sprite::create("gamemtt/mttcoin.png");
	m_spriteMoney->setPosition(225-px,203) ;
	this->addChild(m_spriteMoney);
	m_labelMoney = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Point(0,0.5),249-px,203,Color4B(0xff,0xd2,0x00,0xff),this,1) ;

	CreateLabelMSYHAndAnchorPosClrAddTo(26,"比赛时间：",Point(0,0.5),91-px,151,Color4B::WHITE,this,1) ;
	m_labelTime = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Point(0,0.5),218-px,151,Color4B::WHITE,this,1) ;

	CreateLabelMSYHAndAnchorPosClrAddTo(24,"比赛开始前将在游戏中提醒您。祝您手气长虹！",Point(0,0.5),91,105,Color4B(0x50,0xbd,0x2e,0xff),this,1) ;



	ccMenuCallback menuCB2 = CC_CALLBACK_1(MttSignUpSuccess::onMenuItemClick, this);
	//m_btclose =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
	m_btSure =MenuItemScale9Sprite::create(0, Size(226, 70),menuCB2);
	m_btSure->setPosition(sz.width/2,50);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(26,"确定",Vec2(0.5,0.5),m_btSure->getContentSize().width/2,m_btSure->getContentSize().height/2,Color4B::WHITE,m_btSure,0);

	auto pMenu = Menu::create(m_btSure,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);

	return true ;
}

void MttSignUpSuccess::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		Tools::callScaleAction(this) ;
	}
	else
	{
		SendClientRequest(ClientRequest::kOptMTTHallBgLayer);
	}
}

void MttSignUpSuccess::onGrabTouchBegan(bool bVisible)
{

}

void MttSignUpSuccess::onMenuItemClick(cocos2d::Ref *)
{

	setVisible(false) ;

}

void MttSignUpSuccess::updateView(std::string name,std::string money,std::string time)
{
	SetLabelString(m_labelName,name.c_str()) ;
	SetLabelString(m_labelMoney,money.c_str()) ;
	SetLabelString(m_labelTime,time.c_str()) ;
}