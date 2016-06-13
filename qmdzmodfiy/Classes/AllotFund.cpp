#include "AllotFund.h"
#include "GameTipLayer.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9Sprite.h"
#include "GameFrame.h"
#include "Tools.h"
#include "external/ConvertUTF/ConvertUTF.h"
#include "GlobalUserData.h"

USING_NS_CC ;

bool AllotFund::init()
{
	if (!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false;
	}
	Size sz = this->getContentSize() ;

	m_pLabelContent = cocos2d::Label::createWithTTF("您正在将基金分配给：", "fzlt.ttf", 30);

	SetLabelString(m_pLabelContent,"您正在将基金分配给：");
	m_pLabelContent->setTextColor(Color4B::WHITE);
	m_pLabelContent->setMaxLineWidth(510);
	m_pLabelContent->setAnchorPoint(Point(0,0.5)) ;
	m_pLabelContent->setPosition(Point(77, 269));
	this->addChild(m_pLabelContent,1);


	lableName = CreateLabelMSYHAndAnchorPosClrAddTo(30,"",Vec2(0,0.5),364,269,Color4B(0xff,0xffd2,0x00,0xff),this,0);

	auto lableCash = CreateLabelMSYHAndAnchorPosClrAddTo(30,"分配金额:",Vec2(0,0.5),90,151,Color4B::WHITE,this,0);


	auto spriteEdit = Scale9Sprite::createWithSpriteFrameName("club_text.png",Rect(10,10,124-10*2,62-10*2)) ;
	spriteEdit->setPosition(378,151) ;
	spriteEdit->setPreferredSize(Size(305,62)) ;
	this->addChild(spriteEdit);

	Size szCount = spriteEdit->getContentSize();
	m_editBox = CreateNode2AndPosAddTo<ui::EditBox>(Size(szCount.width, szCount.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szCount.width/2, szCount.height/2, spriteEdit, 0);
	m_editBox->setFont("fzlt.ttf", 30);
	m_editBox->setFontColor(Color3B(0xff,0xd2,0x00));
	m_editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editBox->setDelegate(this);	
	m_editBox->setText("0") ;


	auto pMenu = cocos2d::Menu::create(nullptr);
	pMenu->setPosition(cocos2d::Vec2::ZERO);
	unsigned uBtCount = 2;

	static cocos2d::Vec2 s_ptBt[] = {
		cocos2d::Vec2(220, 57),
		cocos2d::Vec2(460, 57)
	};
	std::string strBt[2] = {"确定","取消"} ;
	for (unsigned u = 0; u < uBtCount; ++u)
	{
		//auto menuItem = MenuItemFrameSprite::create("userinfo/btred.png", 3, CC_CALLBACK_1(DealApplyFamily::onMenuItemSelect, this,u));
		auto menuItem = MenuItemScale9Sprite::create(0, Size(203, 70), CC_CALLBACK_1(AllotFund::onMenuItemSelect, this,u));
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




void AllotFund::setInfo(unsigned fid,unsigned uid,unsigned mid,std::string uname)
{
	m_mid = mid ;
	m_fid = fid ;
	m_uid = uid ;
	m_uname = uname ;
	SetLabelString(lableName,m_uname.c_str()) ;
	//SetIconUrl(httpIcon, m_uicon);

	//m_pLabelContent->setPositionX(lableName->getPositionX()+lableName->getContentSize().width) ;
}



void AllotFund::onMenuItemSelect(cocos2d::Ref* ref,int index)
{


	if (index ==0)
	{

		if (m_edit_fund ==0)
		{
			PopTipWithBt1("请输入正确的金额","确定",nullptr) ;
			return ;
		}
		setVisible(false) ;

		Json::Value param;
		param["clubid"] = g_globalMyFamilyInfo.m_familyID ;
		param["uid"] = m_uid ;
		param["fund"] = m_edit_fund ;
		param["uname"] = m_uname ;
		param["clubname"] = g_globalMyFamilyInfo.m_familyName ;
		//SendClientRequest(ClientRequest::kCallAllotFunToRole, param);
		SendClientRequest(ClientRequest::kCallUpdatePlayerCash, param);
	}
	if (index ==1)
	{
		setVisible(false) ;

	}

}

void AllotFund::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible);
	if (bVisible)
	{
		m_edit_fund = 0 ;
		m_editBox->setText("0") ;
	}
}

void AllotFund::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	if (editBox == m_editBox)
	{
		std::string  showText = editBox->getText() ;
		if ( showText.length() ==0)
		{
			m_edit_fund = 0 ;
			return ;
		}
		
		if (Tools::is_number(showText) && showText.length()!=0)
		{
			m_edit_fund  = atoi(showText.c_str());
		}
		else
		{
			PopTipWithBt1("请输入正确的金额","确定",nullptr) ;
		}
	}
}