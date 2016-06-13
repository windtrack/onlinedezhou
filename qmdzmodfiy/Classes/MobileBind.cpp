#include "MobileBind.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "BaseButton.h"
#include "GameCommon.h"

USING_NS_CC;

enum BgSize
{
	BgSizeW = 550,
	BgSizeH = 312
};

bool MobileBind::init()
{
	if(!GameDialog::init("userinfo/bgdialog0.png",false))
	{
		return false;
	}

	//编辑框
	/*手机号编辑框*/
	m_editPhoneNum = CreateNode2AndPosAddTo<ui::EditBox>(Size(365, 55), ui::Scale9Sprite::create("mobilebind/bgedit.png") , 253, 317,this, 0);
	std::string  str = "请输入绑定手机号";
	m_editPhoneNum->setPlaceHolder(str.c_str());
	m_editPhoneNum->setPlaceholderFontSize(24);
	m_editPhoneNum->setPlaceholderFontColor(Color3B(0x7c,0x7c,0x7c));
	m_editPhoneNum->setFont("fzlt.ttf", 24);
	m_editPhoneNum->setFontColor(Color3B::BLACK);
	m_editPhoneNum->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
    m_editPhoneNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editPhoneNum->setDelegate(this);
	//m_editPhoneNum->setText(str.c_str());
	/*验证码编辑框*/
	m_editSecurityCode = CreateNode2AndPosAddTo<ui::EditBox>(Size(365, 55), ui::Scale9Sprite::create("mobilebind/bgedit.png") , 253, 227,this, 0);
	str = "请输入验证码";
	m_editSecurityCode->setPlaceHolder(str.c_str());
	m_editSecurityCode->setPlaceholderFontSize(26);
	m_editSecurityCode->setPlaceholderFontColor(Color3B(0x7c,0x7c,0x7c));
	m_editSecurityCode->setFont("fzlt.ttf", 24);
	m_editSecurityCode->setFontColor(Color3B::BLACK);
	m_editSecurityCode->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editSecurityCode->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editSecurityCode->setDelegate(this);

	//验证码
	m_btGetSeccode = createButton("mobilebind/identifying.png", true, 30.0f, Color4B(0, 0, 0, 0), Color4B(12, 79, 31, 100));// FrameSpriteButton::createWithVertFrameSprite("mobilebind/identifying.png",false,true);
	m_btGetSeccode->SetHitCB(CC_CALLBACK_1(MobileBind::menuClickGetSeccode,this));
	m_btGetSeccode->setPosition(515,317);
	this->addChild(m_btGetSeccode);

	//确认绑定
	auto confirmBind =  MenuItemSpriteWithLabel::create("mobilebind/confirm.png",3,CC_CALLBACK_1(MobileBind::menuClickConfirmBind,this));
	confirmBind->setPosition(515,227);
	auto label = CreateLabelMSYHBD(26,"确认绑定");
	label->setPosition(96,30);
	confirmBind->addChild(label);
	auto menu = Menu::create(confirmBind,nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

    auto editPhoneSz = m_editPhoneNum->getContentSize();
    auto editPhonePos = m_editPhoneNum->getPosition();
    m_labelPhoneTips = CreateLabelMSYH(20, "您输入的手机号码格式不正确");
    m_labelPhoneTips->setTextColor(Color4B(0x5c,0xc3,0x80,0xff));
    m_labelPhoneTips->setAnchorPoint(Vec2(0, 1));
    m_labelPhoneTips->setPosition(editPhonePos.x - editPhoneSz.width/2, editPhonePos.y - editPhoneSz.height/2 - 2);
    addChild(m_labelPhoneTips);
    m_labelPhoneTips->setVisible(false);

	//文字说明
	label = CreateLabelMSYH(24,"手机绑定可以更好地保障您的账号安全，账号出现非本人操作的异常情况，均可通过绑定的手机号码来证明本人身份。忘记密码，也可以通过绑定的手机号找回密码。");
	label->setMaxLineWidth(595);
	label->setTextColor(Color4B(0x5c,0xc3,0x80,0xff));
	label->setHorizontalAlignment(TextHAlignment::LEFT);
	label->setLineBreakWithoutSpace(true);
	label->setPosition(351,125);
	this->addChild(label);

	return true;
}
//获取验证码
void MobileBind::menuClickGetSeccode(Ref* pSender)
{
	std::string strPhone = m_editPhoneNum->getText();
	if (strPhone.length() == 11)
	{
		Json::Value param;
		param["phone"] = strPhone;
		SendClientRequest(ClientRequest::kGetBindSafeCode, param);
        m_labelPhoneTips->setVisible(false);
	}
    else
    {
        m_btGetSeccode->SetEnable(true);

		PopTipWithBt1("请输入正确的手机号","确定",nullptr) ;
    }
}
//确认绑定按钮
void MobileBind::menuClickConfirmBind(Ref* pSender)
{
	std::string strPhone = m_editPhoneNum->getText();
	std::string strCode = m_editSecurityCode->getText();
	if (strPhone.length() == 11 && strCode.length() > 0)
	{
		Json::Value param;
		param["phone"] = strPhone;
		param["code"] = strCode;
		SendClientRequest(ClientRequest::kBindPhone, param);
		setVisible(false);
	}
}

void MobileBind::editBoxReturn(cocos2d::ui::EditBox* editBox)
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