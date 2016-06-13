#include "PrivateRoomInfo.h"
#include "Tools.h"
#include "Util.h"
#include "GlobalUserData.h"
USING_NS_CC;

bool PrivateRoomInfo::init()
{
	if (!GameDialog::init("family/informationbg.png", false))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	Node* s_uiroot = CSLoader::createNode("res/ui/private_roominfo.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(s_uiroot,1);



	m_personalRoom = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_personal")) ;
	m_clubRoom = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_club")) ;
	m_playerNum = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_playernum")) ;
	m_ownerName = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_owername_0")) ;
	m_password = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_password_0")) ;
	m_str_password = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_password")) ;
	m_blind = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_blind0")) ;
	m_ante = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_ante_0")) ;

	m_minCarry = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_mincarry_0")) ;
	m_maxCarry = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_maxcarry0")) ;
	m_curFund = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_curCollect_0")) ;

	m_curFundTitle = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_curCollect")) ;

	m_fundP = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_fundP_0")) ;
	m_minCollect = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_mincollect_0")) ;
	m_maxCollect = static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_maxcollect_0")) ;



	//unsigned		m_Type;//0 个人 1俱乐部
	//unsigned		m_playerNum;
	//std::string		m_ownerName;
	//std::string		m_password;
	//unsigned		m_minBlind;
	//unsigned		m_maxBlind;
	//unsigned		m_ante;
	//unsigned		m_minCarry;
	//unsigned		m_maxCarry;
	//unsigned		m_curCollectFund;
	//unsigned		m_fundP;
	//unsigned		m_minCollect;
	//unsigned		m_maxCollect ;
	//测试数据
	//m_globaPrivateRoomInfo.m_Type = 1 ;
	//m_globaPrivateRoomInfo.m_playerNum = 9 ;
	//m_globaPrivateRoomInfo.m_ownerName = "瞌睡的大熊";
	//m_globaPrivateRoomInfo.m_password = "1310" ;
	//m_globaPrivateRoomInfo.m_minBlind = 20000 ;
	//m_globaPrivateRoomInfo.m_minBlind = 20000 ;
	//m_globaPrivateRoomInfo.m_maxBlind = m_globaPrivateRoomInfo.m_minBlind*2 ;
	//m_globaPrivateRoomInfo.m_ante = 20000 ;
	//m_globaPrivateRoomInfo.m_minCarry = 500000 ;
	//m_globaPrivateRoomInfo.m_maxCarry = 10000000 ;
	//m_globaPrivateRoomInfo.m_curCollectFund = 1111111111111 ;
	//m_globaPrivateRoomInfo.m_fundP = 3 ;
	//m_globaPrivateRoomInfo.m_minCollect = 10000 ;
	//m_globaPrivateRoomInfo.m_maxCollect = 250000 ;

	return true ;
}

void PrivateRoomInfo::updateCurRoomInfo(bool flag)
{
	char buff[64] ;
	m_personalRoom->setVisible(false) ;
	m_clubRoom->setVisible(false) ;
	if (m_globaPrivateRoomInfo.m_Type == 0)
	{
		m_personalRoom->setVisible(true) ;
		setUITextString(m_str_password,"口令  :");
	}
	if (m_globaPrivateRoomInfo.m_Type == 1)
	{
		m_clubRoom->setVisible(true) ;
		setUITextString(m_str_password,"俱乐部:");
	}

	//房主才能看到基金
	m_curFund->setVisible(flag) ;
	m_curFundTitle->setVisible(flag);

	
		

	sprintf(buff,"%d人房",m_globaPrivateRoomInfo.m_playerNum) ;
	setUITextString(m_playerNum,buff);
	setUITextString(m_ownerName,m_globaPrivateRoomInfo.m_ownerName.c_str());
	setUITextString(m_password,m_globaPrivateRoomInfo.m_password.c_str());

	sprintf(buff,"%s/%s", FormatCash(m_globaPrivateRoomInfo.m_minBlind).c_str(),FormatCash(m_globaPrivateRoomInfo.m_maxBlind).c_str()) ;
	setUITextString(m_blind, buff);

	sprintf(buff,"%s", FormatCash(m_globaPrivateRoomInfo.m_ante).c_str()) ;
	setUITextString(m_ante, buff);

	sprintf(buff,"%s", FormatCash(m_globaPrivateRoomInfo.m_minCarry).c_str()) ;
	setUITextString(m_minCarry, buff);

	sprintf(buff,"%s", FormatCash(m_globaPrivateRoomInfo.m_maxCarry).c_str()) ;
	setUITextString(m_maxCarry, buff);

	sprintf(buff,"%s", FormatCash3(m_globaPrivateRoomInfo.m_curCollectFund).c_str()) ;
	setUITextString(m_curFund, buff);

	sprintf(buff,"%s%%", FormatCash(m_globaPrivateRoomInfo.m_fundP).c_str()) ;
	setUITextString(m_fundP, buff);

	sprintf(buff,"%s", FormatCash(m_globaPrivateRoomInfo.m_minCollect).c_str()) ;
	setUITextString(m_minCollect, buff);

	sprintf(buff,"%s", FormatCash(m_globaPrivateRoomInfo.m_maxCollect).c_str()) ;
	setUITextString(m_maxCollect, buff);
}


void PrivateRoomInfo::setVisible(bool bVisible)
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

//void PrivateRoomInfo::onGrabTouchBegan(bool bHitDialog)
//{
//	if (!bHitDialog)
//	{
//		callHideAction() ;
//	}
//}