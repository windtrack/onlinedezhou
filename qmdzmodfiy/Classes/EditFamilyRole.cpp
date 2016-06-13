#include "EditFamilyRole.h"

#include "Util.h"
#include "GrabTouch.h"
#include "GameFrame.h"
#include <vector>
#include "GameData.h"
#include "FrameSpriteButton.h"
#include "Util.h"
#include "GameHall.h"
#include "MenuItemFrameSprite.h"

#include "GlobalUserData.h"
using namespace cocos2d;

enum ButtonSize
{
	B_Width = 437,
	B_Height = 70 ,
};


bool EditFamilyRole::init()
{
	if (!GameDialog::init())
	{
		return false ;
	}

	//this->setContentSize(Size(GameData::getInstance()->getGameWidth(),GameData::getInstance()->getGameHeight())) ;

	m_bigBlock =Sprite::create("family/family_caozbg4.png");
	m_bigBlock->setPosition  (GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2) ;
	addChild(m_bigBlock);

	m_smallBlock =Sprite::create("family/family_caozbg2.png");
	m_smallBlock->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2) ;
	addChild(m_smallBlock);
	std::string s_editname[5] = 
	{
		"查看资料",
		"分配",
		"转让俱乐部",
		"逐出俱乐部",
	} ;

	for (int i=0;i<4;i++)
	{
		ccMenuCallback menuCB = CC_CALLBACK_1(EditFamilyRole::onMenuItemSelected, this);
		auto bgItem =MenuItemFrameSprite::create("family/family_caozuobutton.png", 2,menuCB);
		auto btDetail = LayerButton::create(false, true);
		bgItem->addChild(btDetail);
		btDetail->setContentSize(Size(B_Width,B_Height-5)) ;

		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,s_editname[i],Vec2(0.5,0.5),B_Width/2,B_Height/2,Color4B::WHITE,bgItem,0);
		bgItem->setPosition(B_Width/2,B_Height/2);

		if (i==0){m_btInfo =bgItem ;}
		if (i==1){m_bt_conFund =bgItem ;}
		if (i==2){m_bt_changeShaikh =bgItem ;}
		if (i==3){m_bt_pushRole =bgItem ;}

	}
	auto pMenu = Menu::create(m_btInfo,m_bt_conFund, m_bt_changeShaikh,m_bt_pushRole,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu, 1);

	return true ;
}


void  EditFamilyRole::updateView(unsigned value)
{
	//载入操作
	m_editBox.clear();
	resetAllButton() ;
	if (value&Edit_LookInf0)
	{
		m_editBox.push_back(m_btInfo);
	}
	if (value&Edit_To_EditFund)
	{
		m_editBox.push_back(m_bt_conFund);
	}
	//if (value&Edit_No_ViceShaikh)
	//{
	//	m_editBox.push_back(m_bt_no_viceShaikh);
	//}
	if (value&Edit_ChangeShaikh)
	{
		m_editBox.push_back(m_bt_changeShaikh);
	}
	if (value&Edit_Push)
	{
		m_editBox.push_back(m_bt_pushRole);
	}

	float PX=0,PY=0;
	float PH = B_Height ;

	float maxHeight = PH*m_editBox.size()-B_Height/2-25;

	this->setContentSize(Size(B_Width,PH*m_editBox.size())) ;
	Size sc = this->getContentSize() ;
	if (m_editBox.size()==2)
	{
		m_bigBlock->setVisible(false);
		m_smallBlock->setVisible(true);
		m_smallBlock->setPosition(sc.width/2,sc.height/2);
	}
	if (m_editBox.size()==4)
	{
		m_bigBlock->setVisible(true);
		m_smallBlock->setVisible(false);
		m_bigBlock->setPosition(sc.width/2,sc.height/2);
	}
		PY = maxHeight +B_Height/2-8;
	for (int i=0;i<m_editBox.size();i++)
	{
		auto s_lbt = m_editBox.at(i);
		s_lbt->setVisible(true);
		s_lbt->setPositionX(sc.width/2);
		s_lbt->setPositionY(PY);
		PY-=(B_Height+2);
	}

}

void EditFamilyRole::onMenuItemSelected(cocos2d::Ref* item)
{


	if (item == m_btInfo)
	{
		Json::Value param;
		param["tid"] = m_Uid;
		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);

		setVisible(false) ;
	}
	//if (item == m_bt_to_viceShaikh)
	//{
	//	//[uid,secrt,group_id,user_id,1|0 ]
	//	Json::Value param;
	//	param["user_id"] = m_Uid;
	//	param["group_id"] = g_globalMyData.getFamilyID();
	//	param["con"] =1;
	//	SendClientRequest(ClientRequest::kTo_ViceShaikh, param);
	//	auto pHall = dynamic_cast<GameHall*>(getParent());
	//	pHall->showEditFamilyRole(false,0,0);
	//}
	if (item == m_bt_conFund)
	{
		auto pHall = dynamic_cast<GameHall*>(getParent());
		pHall->showAllotFund(true,g_globalMyFamilyInfo.m_familyID,m_Uid,g_globalMyData.m_iUserId,m_uNmae) ;
		setVisible(false) ;
	}
	if (item == m_bt_changeShaikh)
	{

		auto m_pGameHall = dynamic_cast<GameHall*>(getParent());
		if(g_globalMyData.IsBindPhone())
		{
			m_pGameHall->showMobileCheck(true,m_Uid) ;
			//m_pGameHall->callUpdateMobileCheckPhone("15955185033") ;
			m_pGameHall->callUpdateMobileCheckPhone(g_globalMyData.getBindPhoneNumber()) ;
		}
		else
		{
			m_pGameHall->showMobileBind(true) ;
		}
			
		setVisible(false) ;
	}
	//提出玩家
	if (item == m_bt_pushRole)
	{

		PopTipWithBt2("是否确定踢出玩家?","确定","取消",[this](unsigned ubt)
		{
			if(ubt == 0)
			{
				Json::Value param;
				param["user_id"] = m_Uid;
				param["group_id"] = g_globalMyData.getFamilyID();
				SendClientRequest(ClientRequest::kPushOutFamily, param);

				auto pHall = dynamic_cast<GameHall*>(getParent());
				pHall->showEditFamilyRole(false);
			}
			if(ubt == 1)
			{
	
			}
		});
		setVisible(false) ;
	}



}

void EditFamilyRole::resetAllButton()
{
	m_btInfo->setVisible(false);
	m_bt_conFund->setVisible(false);
	m_bt_changeShaikh->setVisible(false);
	m_bt_pushRole->setVisible(false);
}
