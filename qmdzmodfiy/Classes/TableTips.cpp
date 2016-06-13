#include "TableTips.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "Util.h"
#include "MenuItemScale9Sprite.h"
#include "GameFrame.h"
#include "Tools.h"
#include "GlobalUserData.h"
USING_NS_CC ;


bool MttRebuyTips::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false ;
	}


	auto sz = this->getContentSize();



	m_pLabelContent = cocos2d::Label::createWithTTF("是否花费%d德州币增购%d筹码?", "fzlt.ttf", 30);
	SetLabelString(m_pLabelContent,"是否花费%d德州币增购%d筹码?");
	m_pLabelContent->setTextColor(Color4B::WHITE);
	m_pLabelContent->setMaxLineWidth(510);
	m_pLabelContent->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelContent->setPosition(Point(sz.width/2, 255));
	this->addChild(m_pLabelContent,1);


	CreateLabelMSYHAndAnchorPosClrAddTo(24,"允许增购次数(       )",Point(0.5,0.5),sz.width/2,120,Color4B(0x50,0xbd,0x2e,0xff),this,0) ;


	m_pLabelCount = cocos2d::Label::createWithTTF("3次", "fzlt.ttf", 24);
	SetLabelString(m_pLabelCount,"3次");
	m_pLabelCount->setTextColor(Color4B(0xff,0xd2,0x00,0xff));
	m_pLabelCount->setMaxLineWidth(510);
	m_pLabelCount->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelCount->setPosition(Point(409, 120));
	this->addChild(m_pLabelCount,1);


	auto pMenu = cocos2d::Menu::create(nullptr);
	pMenu->setPosition(cocos2d::Vec2::ZERO);
	unsigned uBtCount = 2;

	static cocos2d::Vec2 s_ptBt[] = {
		cocos2d::Vec2(220, 57),
		cocos2d::Vec2(460, 57)
	};
	std::string strBt[2] = {"购买","取消"} ;
	for (unsigned u = 0; u < uBtCount; ++u)
	{
		//auto menuItem = MenuItemFrameSprite::create("userinfo/btred.png", 3, CC_CALLBACK_1(DealApplyFamily::onMenuItemSelect, this,u));
		auto menuItem = MenuItemScale9Sprite::create(0, Size(203, 70), CC_CALLBACK_1(MttRebuyTips::onMenuRebuyItemSelect, this,u));
		auto szBt = menuItem->getContentSize();
		auto label = cocos2d::Label::createWithTTF(strBt[u], "fzltbd.ttf", 30);
		label->setTextColor(Color4B::WHITE);
		label->setPosition(szBt.width / 2, szBt.height / 2);
		menuItem->addChild(label);
		menuItem->setPosition(s_ptBt[u]);
		pMenu->addChild(menuItem);


		SetLabelString(label,strBt[u].c_str());
	}
	this->addChild(pMenu);


	return true ;
}

void MttRebuyTips::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		updateMttRebuyTips() ;
		Tools::callScaleAction(this) ;
	}

}

void MttRebuyTips::onGrabTouchBegan(bool bHitDialog)
{

}

void MttRebuyTips::onMenuRebuyItemSelect(cocos2d::Ref* ref,int index)
{
	if (index ==0)
	{
		Json::Value param;
		setVisible(false) ;
		param["add"] = 1;
		SendClientRequest(ClientRequest::kCallMttMatchRebuy);
	}
	if (index ==1)
	{
		setVisible(false) ;
	}
}


void MttRebuyTips::updateMttRebuyTips()
{
	char buff[64] ;
	sprintf(buff,"是否花费%d德州币增购%d筹码?",
		g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyMoney,g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyToCarch) ;
	SetLabelString(m_pLabelContent,buff);
	sprintf(buff,"%d次",g_globaMttGameInfo.m_rebuyCount);
	SetLabelString(m_pLabelCount,buff);

}


/************************************************************************/
/* 增购                                                                     */
/************************************************************************/

bool MttAddonTips::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false ;
	}
	auto sz = this->getContentSize();



	m_pLabelContent = cocos2d::Label::createWithTTF("是否花费%d德州币加购%d筹码?", "fzlt.ttf", 30);
	SetLabelString(m_pLabelContent,"是否花费%d德州币加购%d筹码?");
	m_pLabelContent->setTextColor(Color4B::WHITE);
	m_pLabelContent->setMaxLineWidth(510);
	m_pLabelContent->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelContent->setPosition(Point(sz.width/2, 255));
	this->addChild(m_pLabelContent,1);


	CreateLabelMSYHAndAnchorPosClrAddTo(24,"允许加购次数(       )",Point(0.5,0.5),sz.width/2,120,Color4B(0x50,0xbd,0x2e,0xff),this,0) ;


	m_pLabelCount = cocos2d::Label::createWithTTF("%d次", "fzlt.ttf", 24);
	SetLabelString(m_pLabelCount,"3次");
	m_pLabelCount->setTextColor(Color4B(0xff,0xd2,0x00,0xff));
	m_pLabelCount->setMaxLineWidth(510);
	m_pLabelCount->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelCount->setPosition(Point(409, 120));
	this->addChild(m_pLabelCount,1);


	auto pMenu = cocos2d::Menu::create(nullptr);
	pMenu->setPosition(cocos2d::Vec2::ZERO);
	unsigned uBtCount = 2;

	static cocos2d::Vec2 s_ptBt[] = {
		cocos2d::Vec2(220, 57),
		cocos2d::Vec2(460, 57)
	};
	std::string strBt[2] = {"购买","取消"} ;
	for (unsigned u = 0; u < uBtCount; ++u)
	{
		//auto menuItem = MenuItemFrameSprite::create("userinfo/btred.png", 3, CC_CALLBACK_1(DealApplyFamily::onMenuItemSelect, this,u));
		auto menuItem = MenuItemScale9Sprite::create(0, Size(203, 70), CC_CALLBACK_1(MttAddonTips::onMenuAddonItemSelect, this,u));
		auto szBt = menuItem->getContentSize();
		auto label = cocos2d::Label::createWithTTF(strBt[u], "fzltbd.ttf", 30);
		label->setTextColor(Color4B::WHITE);
		label->setPosition(szBt.width / 2, szBt.height / 2);
		menuItem->addChild(label);
		menuItem->setPosition(s_ptBt[u]);
		pMenu->addChild(menuItem);


		SetLabelString(label,strBt[u].c_str());
	}
	this->addChild(pMenu);
	return true ;
}

void MttAddonTips::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		updateMttAddonTips() ;
		Tools::callScaleAction(this) ;
	}

}

void MttAddonTips::onGrabTouchBegan(bool bHitDialog)
{

}

void MttAddonTips::onMenuAddonItemSelect(cocos2d::Ref* ref,int index)
{
	if (index ==0)
	{

		if (g_globalMyData.GetCash()<g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnMoney)
		{
			PopTipWithBt2("您的德州币不足，是否购买进入商城购买德州币！","取消","商城",[](unsigned ubt)
			{
				if(ubt == 1)
				{
					Json::Value param;
					param["idx"] = 1;
					SendClientRequest(ClientRequest::kPopMall, param);
				}
			});
			return ;
		}

		Json::Value param;
		
		setVisible(false) ;
		//param["add"] = 1;
		//SendClientRequest(ClientRequest::kDealApplyFamily, param);
		SendClientRequest(ClientRequest::kCallMttMatchAddon);
	}
	if (index ==1)
	{
		setVisible(false) ;

	}
}


void MttAddonTips::updateMttAddonTips()
{
	char buff[64] ;
	sprintf(buff,"是否花费%d德州币加购%d筹码?",
		g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnMoney,g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_addOnToCarch) ;
	SetLabelString(m_pLabelContent,buff);
	sprintf(buff,"%d次",g_globaMttGameInfo.m_addonCount);
	SetLabelString(m_pLabelCount,buff);
}


/************************************************************************/
/* 服务器通知                                                                     */
/************************************************************************/



bool MttRebuyTipsByServer::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false ;
	}


	auto sz = this->getContentSize();



	m_pLabelContent = cocos2d::Label::createWithTTF("是否花费%d德州币增购%d筹码?", "fzlt.ttf", 30);
	SetLabelString(m_pLabelContent,"是否花费%d德州币增购%d筹码?");
	m_pLabelContent->setTextColor(Color4B::WHITE);
	m_pLabelContent->setMaxLineWidth(510);
	m_pLabelContent->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelContent->setPosition(Point(sz.width/2, 255));
	this->addChild(m_pLabelContent,1);


	m_pLabelCan = CreateLabelMSYHAndAnchorPosClrAddTo(24,"允许增购次数(       )",Point(0.5,0.5),sz.width/2,120,Color4B(0x50,0xbd,0x2e,0xff),this,0) ;


	m_pLabelCount = cocos2d::Label::createWithTTF("3次", "fzlt.ttf", 24);
	SetLabelString(m_pLabelCount,"3次");
	m_pLabelCount->setTextColor(Color4B(0xff,0xd2,0x00,0xff));
	m_pLabelCount->setMaxLineWidth(510);
	m_pLabelCount->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelCount->setPosition(Point(409, 120));
	this->addChild(m_pLabelCount,1);



	
	m_pLabelTime = cocos2d::Label::createWithTTF("是否花费%d德州币增购%d筹码?", "fzlt.ttf", 30);
	SetLabelString(m_pLabelTime,"10s后关闭此界面");
	m_pLabelTime->setTextColor(Color4B::WHITE);
	m_pLabelTime->setMaxLineWidth(510);
	m_pLabelTime->setAnchorPoint(Point(0.5,0.5)) ;
	m_pLabelTime->setPosition(Point(sz.width/2, 140));
	this->addChild(m_pLabelTime,1);


	auto pMenu = cocos2d::Menu::create(nullptr);
	pMenu->setPosition(cocos2d::Vec2::ZERO);
	unsigned uBtCount = 2;

	static cocos2d::Vec2 s_ptBt[] = {
		cocos2d::Vec2(220, 57),
		cocos2d::Vec2(460, 57)
	};
	std::string strBt[2] = {"购买","取消"} ;
	for (unsigned u = 0; u < uBtCount; ++u)
	{
		//auto menuItem = MenuItemFrameSprite::create("userinfo/btred.png", 3, CC_CALLBACK_1(DealApplyFamily::onMenuItemSelect, this,u));
		auto menuItem = MenuItemScale9Sprite::create(0, Size(203, 70), CC_CALLBACK_1(MttRebuyTipsByServer::onMenuRebuyItemSelect, this,u));
		auto szBt = menuItem->getContentSize();
		auto label = cocos2d::Label::createWithTTF(strBt[u], "fzltbd.ttf", 30);
		label->setTextColor(Color4B::WHITE);
		label->setPosition(szBt.width / 2, szBt.height / 2);
		menuItem->addChild(label);
		menuItem->setPosition(s_ptBt[u]);
		pMenu->addChild(menuItem);

		SetLabelString(label,strBt[u].c_str());
	}
	this->addChild(pMenu);


	return true ;
}

void MttRebuyTipsByServer::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		//updateMttRebuyTips() ;
		Tools::callScaleAction(this) ;
	}

}

void MttRebuyTipsByServer::onGrabTouchBegan(bool bHitDialog)
{

}

void MttRebuyTipsByServer::onMenuRebuyItemSelect(cocos2d::Ref* ref,int index)
{
	if (index ==0)
	{
		if (g_globalMyData.GetCash()<g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyMoney)
		{
			PopTipWithBt2("您的德州币不足，是否购买进入商城购买德州币！","取消","商城",[](unsigned ubt)
			{
				if(ubt == 1)
				{
					Json::Value param;
					param["idx"] = 1;
					SendClientRequest(ClientRequest::kPopMall, param);
				}
			});
			return ;
		}

		Json::Value param;
		setVisible(false) ;
		param["add"] = 1;
		SendClientRequest(ClientRequest::kCallMttMatchRebuy);
		this->stopAllActions() ;


	}
	if (index ==1)
	{
		setVisible(false) ;
		SendClientRequest(ClientRequest::kCallMttRebuyCannel) ;
		this->stopAllActions() ;
	}
}


void MttRebuyTipsByServer::updateMttRebuyTips(int time)
{
	m_time = time ;
	char buff[64] ;
	sprintf(buff,"是否花费%d德州币增购%d筹码?",
		g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyMoney,g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyToCarch) ;
	SetLabelString(m_pLabelContent,buff);
	sprintf(buff,"%d次",g_globaMttGameInfo.m_curMttTable.m_mttTableDes.m_rebuyCount);
	SetLabelString(m_pLabelCount,buff);
	m_pLabelCount->setVisible(false) ;

	sprintf(buff,"%dS后关闭此界面",m_time);
	SetLabelString(m_pLabelTime,buff);
	m_pLabelCan->setVisible(false) ;

	auto updateTime = [this](){

		if (m_time<0)
		{
			this->stopAllActions() ;
			this->setVisible(false) ;
			SendClientRequest(ClientRequest::kCallMttRebuyCannel) ;
			return ;
		}
		char buff2[64] ;
		sprintf(buff2,"%dS后关闭此界面",m_time);
		SetLabelString(m_pLabelTime,buff2);
		m_time-=1 ;
	} ;
	CCFiniteTimeAction *repeat = CCRepeat::create(Sequence::create(CallFunc::create(updateTime),DelayTime::create(1.0f),NULL), 9999);
	this->runAction(repeat);

}
