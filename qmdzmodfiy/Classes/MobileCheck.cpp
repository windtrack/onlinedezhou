#include "MobileCheck.h"

#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "BaseButton.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
USING_NS_CC;

enum BgSize
{
	BgSizeW = 550,
	BgSizeH = 312
};

bool MobileCheck::init()
{
	if(!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false;
	}

	//编辑框
	/*手机号编辑框*/
	m_editPhoneNum = CreateNode2AndPosAddTo<ui::EditBox>(Size(365, 55), ui::Scale9Sprite::create("mobilebind/bgedit.png") , 253, 205,this, 0);
	std::string  str = "请输入绑定手机号";
	m_editPhoneNum->setPlaceHolder(str.c_str());
	m_editPhoneNum->setPlaceholderFontSize(24);
	m_editPhoneNum->setPlaceholderFontColor(Color3B(0x7c,0x7c,0x7c));
	m_editPhoneNum->setFont("fzlt.ttf", 24);
	m_editPhoneNum->setFontColor(Color3B::BLACK);
	m_editPhoneNum->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
    m_editPhoneNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editPhoneNum->setDelegate(this);
	m_editPhoneNum->setEnabled(false) ;

	/*验证码编辑框*/
	m_editSecurityCode = CreateNode2AndPosAddTo<ui::EditBox>(Size(365, 55), ui::Scale9Sprite::create("mobilebind/bgedit.png") , 253, 100,this, 0);
	str = "请输入验证码";
	m_editSecurityCode->setPlaceHolder(str.c_str());
	m_editSecurityCode->setPlaceholderFontSize(26);
	m_editSecurityCode->setPlaceholderFontColor(Color3B(0x7c,0x7c,0x7c));
	m_editSecurityCode->setFont("fzlt.ttf", 24);
	m_editSecurityCode->setFontColor(Color3B::BLACK);
	m_editSecurityCode->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editSecurityCode->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editSecurityCode->setDelegate(this);
	m_editSecurityCode->setText("请输入验证码");

	//验证码
	m_btGetSeccode = createButton("mobilebind/identifying.png", true, 30.0f, Color4B(0, 0, 0, 0), Color4B(12, 79, 31, 40));// FrameSpriteButton::createWithVertFrameSprite("mobilebind/identifying.png",false,true);
	m_btGetSeccode->SetHitCB(CC_CALLBACK_1(MobileCheck::menuClickGetSeccode,this));
	m_btGetSeccode->setPosition(515,205);
	this->addChild(m_btGetSeccode);

	//确认绑定
	auto confirmBind =  MenuItemSpriteWithLabel::create("mobilebind/confirm.png",3,CC_CALLBACK_1(MobileCheck::menuClickConfirmBind,this));
	confirmBind->setPosition(515,100);
	auto label = CreateLabelMSYHBD(26,"确认转让");
	label->setPosition(96,30);
	confirmBind->addChild(label);
	auto menu = Menu::create(confirmBind,nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//文字说明
	label = CreateLabelMSYH(30,"请验证手机号");
	label->setMaxLineWidth(595);
	label->setTextColor(Color4B(0xff,0xd2,0x00,0xff));
	label->setHorizontalAlignment(TextHAlignment::LEFT);
	label->setLineBreakWithoutSpace(true);
	label->setPosition(351,317);
	this->addChild(label);
	SetLabelString(label,"请验证手机号") ;
	return true;
}
//获取验证码
void MobileCheck::menuClickGetSeccode(Ref* pSender)
{
	std::string strPhone = m_editPhoneNum->getText();
	if (strPhone.length() == 11)
	{
		Json::Value param;
		param["phone"] = strPhone;
		SendClientRequest(ClientRequest::kGetChangeShaiKhCode, param);
	}
	else
	{
		m_btGetSeccode->SetEnable(true);
		PopTipWithBt1("请输入正确的手机号","确定",nullptr) ;
	}
}
//确认绑定按钮
void MobileCheck::menuClickConfirmBind(Ref* pSender)
{
	std::string strPhone = m_editPhoneNum->getText();
	std::string strCode = m_editSecurityCode->getText();
	if (strPhone.length() == 11 && strCode.length() > 0)
	{
		Json::Value param;
		param["group_id"] = g_globalMyFamilyInfo.getFamilyID();
		param["to_user_id"] = m_newID;
		param["phone"] = strPhone;
		param["code"] = strCode;
		SendClientRequest(ClientRequest::kChangeFamilyShaikh, param);
		setVisible(false);
	}
}

void MobileCheck::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	if (editBox == m_editPhoneNum)
	{
		std::string strText = editBox->getText();
		if (strText.length() == 11)
		{
			m_btGetSeccode->SetEnable(true);
		}
	}
}

void MobileCheck::setBindPhone(std::string iphone)
{
	m_editPhoneNum->setText(iphone.c_str()) ;
}