#include "MttJoinMatchTips.h"
#include "Util.h"
#include "MenuItemScale9Sprite.h"
#include "Tools.h"
#include "LineLabel.h"
#include "FrameSpriteButton.h"
#include "GameFrame.h"
#include "GlobalUserData.h"

USING_NS_CC ;
bool  MttJoinMatchTips::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",true))
	{
		return false ;
	}
	Size sz = this->getContentSize() ;

	m_lineUp = LineLabel::create() ;
	m_lineUp->ignoreAnchorPointForPosition(false) ;
	m_lineUp->setAnchorPoint(Point(0.5,0.5)) ;
	m_lineUp->setPosition(Point(sz.width/2,226)) ;
	this->addChild(m_lineUp);

	m_lineDown = LineLabel::create() ;
	m_lineDown->ignoreAnchorPointForPosition(false) ;
	m_lineDown->setAnchorPoint(Point(0,0.5)) ;
	m_lineDown->setPosition(Point(61,172)) ;
	this->addChild(m_lineDown);



	ccMenuCallback menuCB2 = CC_CALLBACK_1(MttJoinMatchTips::onMenuItemClick, this);
	//m_btclose =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
	m_btSure =MenuItemScale9Sprite::create(0, Size(226, 70),menuCB2);
	m_btSure->setPosition(sz.width/2,58);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(26,"进入",Vec2(0.5,0.5),m_btSure->getContentSize().width/2,m_btSure->getContentSize().height/2,Color4B::WHITE,m_btSure,0);
	auto pMenu = Menu::create(m_btSure,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 100);


	m_time = 0 ;
	return true ;
}

void MttJoinMatchTips::setVisible(bool bVisible)
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

void MttJoinMatchTips::onGrabTouchBegan(bool bVisible)
{
	if (bVisible)
	{
	}
}

void MttJoinMatchTips::onMenuItemClick(cocos2d::Ref *)
{
	this->stopAllActions() ;
	setVisible(false) ;
	g_pGameFrame->doJoinMttMatch(m_tableID,m_isdelayRegist);
}

void MttJoinMatchTips::updateView(std::string name ,unsigned time,unsigned tableid ,bool delayRegist)
{
	this->stopAllActions() ;
	m_matchName = name ;
	m_time = time ;
	m_tableID = tableid ;
	m_isdelayRegist = delayRegist ;

	m_lineUp->resetLable() ;
	m_lineDown->resetLable() ;

	auto updateTime = [this](){

		if (m_time<0)
		{
			this->stopAllActions() ;
			this->setVisible(false) ;
			return ;
		}
		m_lineUp->resetLable() ;
		m_lineUp->Appand(28,"fzlt.ttf","您报名的",Color4B::WHITE);
		m_lineUp->Appand(28,"fzlt.ttf",m_matchName.c_str(),Color4B::WHITE);
			char buff[64] ;
		sprintf(buff,"%d秒",m_time) ;
		m_lineUp->Appand(28,"fzlt.ttf",buff,Color4B(0xff,0xd2,0x00,0xff));
		m_lineUp->Appand(28,"fzlt.ttf","后开始",Color4B::WHITE);
		m_time-=1 ;

	} ;
	CCFiniteTimeAction *repeat = CCRepeat::create(Sequence::create(CallFunc::create(updateTime),DelayTime::create(1.0f),NULL), 9999);
	this->runAction(repeat);


}


void MttJoinMatchTips::doAddMissTime(float time)
{
	if (!this->isVisible())
	{
		return ;
	}
	this->stopAllActions() ;

	m_time -=time ;
	updateView(m_matchName,m_time,m_tableID,m_isdelayRegist) ;
}









bool  MttMatchDisMissTips::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false ;
	}
	Size sz = this->getContentSize() ;

	m_lineUp = LineLabel::create() ;
	m_lineUp->ignoreAnchorPointForPosition(false) ;
	m_lineUp->setAnchorPoint(Point(0.5,0.5)) ;
	m_lineUp->setPosition(Point(sz.width/2,226)) ;
	this->addChild(m_lineUp);

	m_lineDown = LineLabel::create() ;
	m_lineDown->ignoreAnchorPointForPosition(false) ;
	m_lineDown->setAnchorPoint(Point(0,0.5)) ;
	m_lineDown->setPosition(Point(61,172)) ;
	this->addChild(m_lineDown);



	ccMenuCallback menuCB2 = CC_CALLBACK_1(MttMatchDisMissTips::onMenuItemClick, this);
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

void MttMatchDisMissTips::setVisible(bool bVisible)
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

void MttMatchDisMissTips::onGrabTouchBegan(bool bVisible)
{
	if (bVisible)
	{
	}
}

void MttMatchDisMissTips::onMenuItemClick(cocos2d::Ref *)
{
	this->stopAllActions() ;
	setVisible(false) ;
	//解散了
	if (g_pGameFrame->isInTable()&&g_pGameFrame->getTableType() == MttTable &&!g_globaMttGameInfo.m_isGameStart && m_tableID ==g_globaMttGameInfo.m_tableId) //在等候室中
	{
		SendClientRequest(ClientRequest::kCallLevelTable) ;
	}
	
}

void MttMatchDisMissTips::updateView(std::string name ,std::string reason,unsigned tableid)
{
	this->stopAllActions() ;
	m_matchName = name ;
	m_tableID = tableid ;

	m_lineUp->resetLable() ;
	m_lineDown->resetLable() ;

	m_lineUp->resetLable() ;
	m_lineUp->Appand(28,"fzlt.ttf","您报名的",Color4B::WHITE);
	m_lineUp->Appand(28,"fzlt.ttf",m_matchName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
	m_lineUp->Appand(28,"fzlt.ttf",reason.c_str(),Color4B::WHITE);
}
