#include "RegisterView.h"
#include "FrameSprite.h"
#include "FrameSpriteButton.h"
#include "LogonLayerEx.h"
#include "Util.h"
#include "GameCommon.h"
#include "Tools.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool RegisterView::initRegisterView(LogonLayerEx *logoLayer)
{
    //手机号编辑框
    m_editPhoneNum = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_phoneReg")));
    m_editPhoneNum->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_editPhoneNum->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_editPhoneNum->setFontSize(24);
    m_editPhoneNum->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this));
    m_editPhoneNum->setPlaceHolderColor(Color3B(0x7c, 0x7c, 0x7c));
    m_editPhoneNum->setPlaceHolder("请输入手机号");

    //验证码编辑框
    m_editSecurityCode = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_codeReg")));
    m_editSecurityCode->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_editSecurityCode->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_editSecurityCode->setFontSize(24);
    m_editSecurityCode->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this));
    m_editSecurityCode->setPlaceHolderColor(Color3B(0x7c, 0x7c, 0x7c));
    m_editSecurityCode->setPlaceHolder("请输入验证码");

    //密码编辑框
    m_editPassword = dynamic_cast<TextField*>(Tools::getWidgetByName(this, std::string("TextField_passwordReg")));
    m_editPassword->setTextHorizontalAlignment(TextHAlignment::LEFT);
    m_editPassword->setTextVerticalAlignment(TextVAlignment::CENTER);
    m_editPassword->setFontSize(24);
    m_editPassword->addEventListener(CC_CALLBACK_2(RegisterView::textFieldEvent, this));
    m_editPassword->setPlaceHolderColor(Color3B(0x7c, 0x7c, 0x7c));
    m_editPassword->setPlaceHolder("支持6-24位字符");

    //获取验证码按钮
    auto SecurityCodeBg = dynamic_cast<Layout*>(Tools::getWidgetByName(this, std::string("Panel_codeReg")));
    m_btGetSecurityCode = createButton("res/ui/securitycode.png", true, 60, Color4B(0, 0, 0, 0), Color4B(0, 0, 0, 40));
    Size sz = SecurityCodeBg->getContentSize();
    Size szSelf = m_btGetSecurityCode->getContentSize();
    m_btGetSecurityCode->setPosition(sz.width - szSelf.width/2, sz.height - szSelf.height/2 + 1);
    m_btGetSecurityCode->SetHitCB(std::bind(&RegisterView::GetSecurityCode, this));
    SecurityCodeBg->addChild(m_btGetSecurityCode);

    //授权并注册按钮
     m_btAuthorityAndRegster = dynamic_cast<Button*>(Tools::getWidgetByName(this, std::string("bt_receiveAndReg")));
     m_btAuthorityAndRegster->addTouchEventListener(CC_CALLBACK_2(RegisterView::AuthorityAndRegister, this));

    //返回按钮
    m_btExitRegister = dynamic_cast<Button*>(Tools::getWidgetByName(this, std::string("bt_backReg")));
    m_btExitRegister->addTouchEventListener(CC_CALLBACK_2(RegisterView::ExitRegister, this));

    //输入是否正确
    m_pUserSpriteImage[0] = dynamic_cast<Button*>(Tools::getWidgetByName(this, std::string("bt_judgePhoneReg")));
    m_pUserSpriteImage[0]->addTouchEventListener(CC_CALLBACK_1(RegisterView::menuWrongOrRight, this));
    m_pUserSpriteImage[0]->setVisible(false);

    m_pUserSpriteImage[1] = dynamic_cast<Button*>(Tools::getWidgetByName(this, std::string("bt_judgeCodeReg")));
    m_pUserSpriteImage[1]->addTouchEventListener(CC_CALLBACK_1(RegisterView::menuWrongOrRight, this));
    m_pUserSpriteImage[1]->setVisible(false);

    //用户协议
    auto labelRead = CreateLabelMSYH(20, "查看用户协议");// Label::createWithTTF(, "fzlt.ttf", 20);//查看用户协议
    labelRead->setTextColor(Color4B(0xa5, 0xa5, 0xa5, 0xff));
    auto btRead = MenuItemLabel::create(labelRead, CC_CALLBACK_1(RegisterView::ReadUserProtocol, this));
    auto posBt = m_btAuthorityAndRegster->getPosition();
    auto szBt = m_btAuthorityAndRegster->getContentSize();
    btRead->setPosition(posBt.x, posBt.y + szBt.height/2 + 15);
    btRead->setAnchorPoint(Vec2(0.5, 0.5));
    auto menu = Menu::create(btRead, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    m_LogoLayerEx = logoLayer;

    return true;
}

void RegisterView::textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    if (type == TextField::EventType::DETACH_WITH_IME)
    {
        if(sender == m_editPhoneNum)	// 判断用户名对错显示的图片标志
        {
            std::string strPhone = m_editPhoneNum->getString();
            if (strPhone.length() == 11)
            {
                m_pUserSpriteImage[0]->setVisible(true);
                m_pUserSpriteImage[0]->setBright(false);
            }
            else
            {
                m_pUserSpriteImage[0]->setVisible(true);
                m_pUserSpriteImage[0]->setBright(true);
            }
        }
        if(sender == m_editSecurityCode)	// 判断验证码对错显示的图片标志
        {
            std::string strPasswd = m_editSecurityCode->getString();
            if(strPasswd.length() > 0)
            {
                m_pUserSpriteImage[0]->setVisible(true);
                m_pUserSpriteImage[1]->setVisible(true);
                m_pUserSpriteImage[1]->setBright(false);
            }
            else
            {
                m_pUserSpriteImage[1]->setVisible(true);
                m_pUserSpriteImage[1]->setBright(true);
            }
        }
    }
}

//void RegisterView::editBoxReturn(cocos2d::ui::EditBox* editBox)
//{
//	if(editBox == m_editPhoneNum)	// 判断用户名对错显示的图片标志
//	{
//		std::string strPhone = m_editPhoneNum->getText();
//		if (strPhone.length() == 11)
//		{
//			m_pUserSpriteImage[0]->setEnabled(false);
//			m_pUserSpriteImage[0]->setVisible(true);
//		}
//		else
//		{
//			m_pUserSpriteImage[0]->setEnabled(false);
//			m_pUserSpriteImage[0]->setVisible(true);
//		}
//	}
//	if(editBox == m_editSecurityCode)	// 判断验证码对错显示的图片标志
//	{
//		std::string strPasswd = m_editSecurityCode->getText();
//		if(strPasswd.length() > 0)
//		{
//			m_pUserSpriteImage[1]->setEnabled(false);
//			m_pUserSpriteImage[0]->setVisible(true);
//			m_pUserSpriteImage[1]->setVisible(true);
//		}
//		else
//		{
//			m_pUserSpriteImage[1]->setEnabled(true);
//			m_pUserSpriteImage[1]->setVisible(true);
//		}
//	}
//	return;
//}

//void RegisterView::onGrabTouchBegan(bool bHitDialog)
//{
//	if (!bHitDialog)
//	{
//		auto pParent = getParent();
//		LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
//		pLogon->ShowAccountLogon();
//	}
//}

//void RegisterView::onKeyEscPressed()
//{
//	auto pParent = getParent();
//	LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
//	pLogon->ShowAccountLogon();
//}

void RegisterView::GetSecurityCode()
{
	std::string strPhone = m_editPhoneNum->getString();
	if (strPhone.length() == 11)
	{
		Json::Value param;
		param["phone"] = strPhone;
		SendClientRequest(ClientRequest::kGetRegisterCode, param);
	}
	else
	{
		m_btGetSecurityCode->SetEnable(true);
	}
}

void RegisterView::AuthorityAndRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        std::string strPhone = m_editPhoneNum->getString();
        std::string strCode = m_editSecurityCode->getString();
        std::string strPwd = m_editPassword->getString();
        if (strPhone.length() == 11 && strCode.length() > 0 && strPwd.length() > 0)
        {
            Json::Value param;
            param["phone"] = strPhone;
            param["code"] = strCode;
            param["pwd"] = strPwd;
            SendClientRequest(ClientRequest::kRegisit, param);

            m_LogoLayerEx->ShowLoading();
        }
    }
}
void RegisterView::menuWrongOrRight(Ref* pSender){}

void RegisterView::ReadUserProtocol(cocos2d::Ref* pSender)
{
	m_LogoLayerEx->ShowLicence();  
}

void RegisterView::ExitRegister(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        m_LogoLayerEx->ShowAccountLogon();
    }
}

//enum ProtocolSrcollView	//	设置默认的滚动视图的大小
//{
//    ScrollW = 1070,
//    ScrollH = 487
//};

UserProtocol* UserProtocol::create(cocos2d::ui::Layout* userPro)
{
    auto pRet = new UserProtocol;
    if (pRet->init(userPro))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

bool UserProtocol::init(cocos2d::ui::Layout* userPro)
{
    return true;
}

bool UserProtocol::init()
{
    Size frameSz = Director::getInstance()->getVisibleSize();
    unsigned ScrollW = frameSz.width - 100;
    unsigned ScrollH = frameSz.height - 160;
	if (!GameDialog::init(frameSz.width, frameSz.height, false))
    {
        return false;
    }
	auto clrBg = LayerColor::create(Color4B(2, 90, 42, 255));
	addChild(clrBg);
    Size szBg = this->getContentSize();
    CreateNode1AndPosAddTo<Sprite>("res/ui/titleprotocol.png", szBg.width/2, szBg.height - 65, this, 0);
    CreateNode1AndPosAddTo<Sprite>("res/ui/interval.png", szBg.width/2, szBg.height - 110, this, 0);

    m_protocolScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollW, ScrollH), 54, 25, this, 0);
    m_protocolScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
    //m_protocolScrollView->setBounceable(false);

    m_btExitProtocol = FrameSpriteButton::createWithVertFrameSprite("res/ui/btback.png", false, true);
    m_btExitProtocol->setPosition(68, szBg.height - 45);
    m_btExitProtocol->SetHitCB(std::bind(&UserProtocol::ExitProtocol, this));
    this->addChild(m_btExitProtocol);

    m_labelProtocol = CreateLabelMSYHAndAnchorPosClrAddTo(18, "", Vec2(0, 0), 0, 0, Color4B(0x79,0xd5,0x96,0xff), m_protocolScrollView, 0);
    m_labelProtocol->setHorizontalAlignment(TextHAlignment::LEFT);
    m_labelProtocol->setLineHeight(30);
    m_labelProtocol->setWidth(szBg.width - 106);
    m_labelProtocol->setLineBreakWithoutSpace(true);
    std::string strtext = "中国视频娱乐网服务使用条例\n第1条 特别提示\n1.1中国视频娱乐网（以下合称“中视”）同意按照本协议的规定及其不时发布的操作规则提供基于互联网以及移动网的相关服务（以下称“网络服务”），为获得网络服务，服务使用人（以下称“用户”）应当同意本协议的全部条款并按照页面上的提示完成全部的注册程序。用户在进行注册程序过程中点击“同意”按钮即表示用户完全接受本协议项下的全部条款。\n1.2用户注册成功后，中视将给予每个用户一个用户帐号及相应的密码，该用户帐号和密码由用户负责保管；用户应当对以其用户帐号进行的所有活动和事件负法律责任。\n第2条 服务内容\n2.1中视网络服务的具体内容由中视根据实际情况提供，例如视频直播、视频点播、搜索、交友、论坛(BBS)、聊天室、电子邮件、发表评论等。\n2.2中视提供的部分网络服务为收费的网络服务，用户使用收费网络服务需要向中视支付一定的费用。对于收费的网络服务，中视会在用户使用之前给予用户明确的提示，只有用户根据提示确认其愿意支付相关费用，用户才能使用该等收费网络服务。如用户拒绝支付相关费用，则中视有权不向用户提供该等收费网络服务。\n2.3中视仅提供相关的网络服务，除此之外与相关网络服务有关的设备（如个人电脑、手机、及其他与接入互联网或移动网有关的装置）及所需的费用（如为接入互联网而支付的电话费及上网费、为使用移动网而支付的手机费）均应由用户自行负担。\n第3条 服务变更、中断或终止\n3.1鉴于网络服务的特殊性，用户同意中视有权随时变更、中断或终止部分或全部的网络服务（包括收费网络服务）。如变更、中断或终止的网络服务属于免费网络服务，中视无需通知用户，也无需对任何用户或任何第三方承担任何责任；如变更、中断或终止的网络服务属于收费网络服务，中视应当在变更、中断或终止之前事先通知用户，并应向受影响的用户提供等值的替代性的收费网络服务，如用户不愿意接受替代性的收费网络服务，就该用户已经向中视支付的服务费，中视应当按照该用户实际使用相应收费网络服务的情况扣除相应服务费之后将剩余的服务费退还给该用户。\n3.2用户理解，中视需要定期或不定期地对提供网络服务的平台（如互联网网站、移动网络等）或相关的设备进行检修或者维护，如因此类情况而造成收费网络服务在合理时间内的中断，中视无需为此承担任何责任，但中视应尽可能事先进行通告。\n3.3如发生下列任何一种情形，中视有权随时中断或终止向用户提供本协议项下的网络服务（包括收费网络服务）而无需对用户或任何第三方承担任何责任：\n3.3.1用户提供的个人资料不真实；\n3.3.2用户违反本协议中规定的使用规则；\n3.3.3用户在使用收费网络服务时未按规定向中视支付相应的服务费；\n3.3.4如用户注册的免费网络服务的帐号在任何连续90日内未实际使用，或者用户注册的收费网络服务的帐号在其订购的收费网络服务的服务期满之后连续180日内未实际使用，则中视有权删除该帐号并停止为该用户提供相关的网络服务；\n3.3.5用户注册的免费帐号昵称如存在违反法律法规或国家政策要求，或侵犯任何第三方合法权益的情况，中视有权收回该账号昵称；\n第4条 使用规则\n4.1用户在申请使用中视网络服务时，必须向中视提供准确的个人资料，如个人资料有任何变动，必须及时更新。\n4.2用户不应将其帐号、密码转让或出借予他人使用。如用户发现其帐号遭他人非法使用，应立即通知中视。因黑客行为或用户的保管疏忽导致帐号、密码遭他人非法使用，中视不承担任何责任。\n4.3用户同意中视有权在提供网络服务过程中以各种方式投放各种商业性广告或其他任何类型的商业信息（包括但不限于在中视网站的任何页面上投放广告），并且，用户同意接受中视通过电子邮件或其他方式向用户发送商品促销或其他相关商业信息。\n4.4对于用户通过中视网络服务（包括但不限于论坛、BBS、评论、个人微博）上传到中视网站上可公开获取区域的任何内容，用户同意中视在全世界范围内具有免费的、永久性的、不可撤销的、非独家的和完全再许可的权利和许可，以使用、复制、修改、改编、出版、翻译、据以创作衍生作品、传播、表演和展示此等内容（整体或部分），和/或将此等内容编入当前已知的或以后开发的其他任何形式的作品、媒体或技术中。\n4.5用户在使用中视网络服务过程中，必须遵循以下原则；\n4.5.1遵守中国有关的法律和法规；\n4.5.2遵守所有与网络服务有关的网络协议、规定和程序；\n4.5.3不得为任何非法目的而使用网络服务系统；\n4.5.4不得以任何形式使用中视网络服务侵犯中视的商业利益，包括并不限于发布非经中视许可的商业广告；\n4.5.5不得利用中视网络服务系统进行任何可能对互联网或移动网正常运转造成不利影响的行为；\n4.5.6不得利用中视提供的网络服务上传、展示或传播任何虚假的、骚扰性的、中伤他人的、辱骂性的、恐吓性的、庸俗淫秽的或其他任何非法的信息资料；\n4.5.7不得侵犯其他任何第三方的专利权、著作权、商标权、名誉权或其他任何合法权益；\n4.5.8不得利用中视网络服务系统进行任何不利于中视的行为；\n4.6中视有权对用户使用中视网络服务的情况进行审查和监督(包括但不限于对用户存储在中视的内容进行审核)，如用户在使用网络服务时违反任何上述规定，中视或其授权的人有权要求用户改正或直接采取一切必要的措施（包括但不限于更改或删除用户张贴的内容等、暂停或终止用户使用网络服务的权利）以减轻用户不当行为造成的影响。\n4.7中视针对某些特定的中视网络服务的使用通过各种方式（包括但不限于网页公告、电子邮件、短信提醒等）作出的任何声明、通知、警示等内容视为本协议的一部分，用户如使用该等中视网络服务，视为用户同意该等声明、通知、警示的内容。\n第5条 知识产权\n5.1中视提供的网络服务中包含的任何文本、图片、图形、音频和/或视频资料均受版权、商标和/或其它财产所有权法律的保护，未经相关权利人同意，上述资料均不得在任何媒体直接或间接发布、播放、出于播放或发布目的而改写或再发行，或者被用于其他任何商业目的。所有这些资料或资料的任何部分仅可作为私人和非商业用途而保存在某台计算机内。中视不就由上述资料产生或在传送或递交全部或部分上述资料过程中产生的延误、不准确、错误和遗漏或从中产生或由此产生的任何损害赔偿，以任何形式，向用户或任何第三方负责。\n5.2中视为提供网络服务而使用的任何软件（包括但不限于软件中所含的任何图象、照片、动画、录像、录音、音乐、文字和附加程序、随附的帮助材料）的一切权利均属于该软件的著作权人，未经该软件的著作权人许可，用户不得对该软件进行反向工程（reverse engineer）、反向编译（decompile）或反汇编（disassemble）。\n第6条 隐私保护\n6.1保护用户隐私是中视的一项基本政策，中视保证不对外公开或向第三方提供单个用户的注册资料及用户在使用网络服务时存储在中视的非公开内容，但下列情况除外：\n6.1.1事先获得用户的明确授权；\n6.1.2根据有关的法律法规要求；\n6.1.3按照相关政府主管部门的要求；\n6.1.4为维护社会公众的利益；\n6.1.5为维护中视的合法权益。\n6.2中视可能会与第三方合作向用户提供相关的网络服务，在此情况下，如该第三方同意承担与中视同等的保护用户隐私的责任，则中视有权将用户的注册资料等提供给该第三方。\n 6.3在不透露单个用户隐私资料的前提下，中视有权对整个用户数据库进行分析并对用户数据库进行商业上的利用。\n第7条 免责声明\n7.1用户明确同意其使用中视网络服务所存在的风险将完全由其自己承担；因其使用中视网络服务而产生的一切后果也由其自己承担，中视对用户不承担任何责任。\n7.2中视不担保网络服务一定能满足用户的要求，也不担保网络服务不会中断，对网络服务的及时性、安全性、准确性也都不作担保。\n7.3中视不保证为向用户提供便利而设置的外部链接的准确性和完整性，同时，对于该等外部链接指向的不由中视实际控制的任何网页上的内容，中视不承担任何责任。\n7.4对于因不可抗力或中视不能控制的原因造成的网络服务中断或其它缺陷，中视不承担任何责任，但将尽力减少因此而给用户造成的损失和影响。\n7.5用户同意，对于中视向用户提供的下列产品或者服务的质量缺陷本身及其引发的任何损失，中视无需承担任何责任：\n7.5.1向用户免费提供的各项网络服务；\n7.5.2中视向用户赠送的任何产品或者服务；\n7.5.3中视向收费网络服务用户附赠的各种产品或者服务。\n第8条 违约赔偿\n8.1如因中视违反有关法律、法规或本协议项下的任何条款而给用户造成损失，中视同意承担由此造成的损害赔偿责任。\n8.2用户同意保障和维护中视及其他用户的利益，如因用户违反有关法律、法规或本协议项下的任何条款而给中视或任何其他第三人造成损失，用户同意承担由此造成的损害赔偿责任。\n第9条 协议修改\n9.1中视有权随时修改本协议的任何条款，一旦本协议的内容发生变动，中视将会直接在中视网站上公布修改之后的协议内容，该公布行为视为中视已经通知用户修改内容。中视也可通过其他适当方式向用户提示修改内容。";
    SetLabelString(m_labelProtocol, strtext);

    Size sz = m_labelProtocol->getContentSize();
    unsigned fY = sz.height / 2;
    unsigned uHeight = sz.height;
    if (sz.height < ScrollH)
    {
        fY = ScrollH - uHeight;
        uHeight = ScrollH;
    }
    else
    {
        fY = 0;
    }
    float offset = ScrollH - (float)uHeight;
    m_labelProtocol->setPosition(0, fY);
    m_protocolScrollView->setContentOffset(Vec2(0, offset), false);
    m_protocolScrollView->setContentSize(Size(ScrollW, uHeight));

    return true;
}

void UserProtocol::ExitProtocol()
{
	auto pParent = getParent();
	LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
	pLogon->ShowRegisit();
}

void UserProtocol::onKeyEscPressed()
{
	auto pParent = getParent();
	LogonLayerEx *pLogon = dynamic_cast<LogonLayerEx*>(pParent);
	pLogon->ShowAccountLogon();
}