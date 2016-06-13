#include "DealApplyFamily.h"
#include "GameTipLayer.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9Sprite.h"
#include "GameFrame.h"

#include "external/ConvertUTF/ConvertUTF.h"
USING_NS_CC ;

bool DealApplyFamily::init()
{
	if (!GameDialog::init())
	{
		return false;
	}

	m_pTipLayerWithBt2 = GameTipLayer::create<GameTipLytBt2Cfg>();



	auto bgSprite = cocos2d::Sprite::create("userinfo/bgdialog0.png");
	auto sz = bgSprite->getContentSize();
	bgSprite->setPosition(sz.width / 2, sz.height / 2);
	setContentSize(sz);
	addChild(bgSprite);


	

	m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(GameDialog::onMenuItemCloseSelected, this));
	m_btMenuItemClose->setPosition(sz.width - 25, sz.height - 28);
	auto menu = Menu::create(m_btMenuItemClose, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 100);
	
	m_pLabelContent = cocos2d::Label::createWithTTF("申请加入俱乐部", "fzlt.ttf", 30);

	SetLabelString(m_pLabelContent,"申请加入俱乐部?");
	m_pLabelContent->setTextColor(Color4B::WHITE);
	m_pLabelContent->setMaxLineWidth(510);
	m_pLabelContent->setAnchorPoint(Point(0,0.5)) ;
	m_pLabelContent->setPosition(Point(339-25, 255));
	bgSprite->addChild(m_pLabelContent,1);

	auto pMenu = cocos2d::Menu::create(nullptr);
	pMenu->setPosition(cocos2d::Vec2::ZERO);
	unsigned uBtCount = 2;

	static cocos2d::Vec2 s_ptBt[] = {
		cocos2d::Vec2(220, 57),
		cocos2d::Vec2(460, 57)
	};
	std::string strBt[2] = {"同意","拒绝"} ;
	for (unsigned u = 0; u < uBtCount; ++u)
	{
		//auto menuItem = MenuItemFrameSprite::create("userinfo/btred.png", 3, CC_CALLBACK_1(DealApplyFamily::onMenuItemSelect, this,u));
        auto menuItem = MenuItemScale9Sprite::create(0, Size(203, 70), CC_CALLBACK_1(DealApplyFamily::onMenuItemSelect, this,u));
		auto szBt = menuItem->getContentSize();
		auto label = cocos2d::Label::createWithTTF(strBt[u], "fzltbd.ttf", 30);
		label->setTextColor(Color4B::WHITE);
		label->setPosition(szBt.width / 2, szBt.height / 2);
		menuItem->addChild(label);
		menuItem->setPosition(s_ptBt[u]);
		pMenu->addChild(menuItem);


		SetLabelString(label,strBt[u].c_str());
	}
	bgSprite->addChild(pMenu);
	//m_pTouchGrabber = nullptr;

	
	auto pMenuItem0 = MenuItemSpriteWithLabel::create("quickshop/btautobuyjettonunsel.png", 2, nullptr);
	auto pMenuItem1 = MenuItemSpriteWithLabel::create("quickshop/btautobuyjettonsel.png", 2, nullptr);
	auto pLabel0 = CreateLabelMSYH(24);
	auto pLabel1 = CreateLabelMSYH(24);
	pMenuItem0->SetEnabledColor(Color4B::WHITE);
	pMenuItem1->SetEnabledColor(Color4B::WHITE);
	SetLabelString(pLabel0, "拒绝再次申请");
	SetLabelString(pLabel1, "拒绝再次申请");
	pMenuItem0->SetLabel(pLabel0);
	pMenuItem1->SetLabel(pLabel1);
	pLabel0->setPositionX(120);
	pLabel1->setPositionX(120);
	m_pMenuItemAutoBuyJetton2Max = MenuItemToggle::create(pMenuItem0);
	m_pMenuItemAutoBuyJetton2Max->addSubItem(pMenuItem1);
	m_pMenuItemAutoBuyJetton2Max->setPosition(450, 149);

	pMenu->addChild(m_pMenuItemAutoBuyJetton2Max);

	m_pMenuItemAutoBuyJetton2Max->setSelectedIndex(0);

	httpIcon = CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", bgSprite, Vec2(75,217), "family/gf_family_icon.png");
	lableName = CreateLabelMSYHAndAnchorPosClrAddTo(30,"",Vec2(0,0.5),210-25,255,Color4B(0xff,0xffd2,0x00,0xff),bgSprite,0);


	return true ;
}




void DealApplyFamily::setInfo(unsigned fid,unsigned uid,unsigned mid,std::string uname,std::string uicon)
{
	m_mid = mid ;
	m_fid = fid ;
	m_uid = uid ;
	m_uname = uname ;
	m_uicon = uicon ;
	SetLabelString(lableName,m_uname.c_str()) ;
	SetIconUrl(httpIcon, m_uicon);

	m_pLabelContent->setPositionX(lableName->getPositionX()+lableName->getContentSize().width) ;
}



void DealApplyFamily::onMenuItemSelect(cocos2d::Ref* ref,int index)
{

	int con = m_pMenuItemAutoBuyJetton2Max->getSelectedIndex() ;
		Json::Value param;
		param["user_id"] =m_uid;
		param["group_id"] = m_fid;
	if (index ==0)
	{
		setVisible(false) ;
		param["add"] = 1;
		param["mid"] =m_mid ;
		SendClientRequest(ClientRequest::kDealApplyFamily, param);

	}
	if (index ==1)
	{
		setVisible(false) ;
		if (con==0)
		{
				param["add"] =3;
		}
		else
		{
			param["add"] = 2;
		}
		param["mid"] =m_mid ;
		SendClientRequest(ClientRequest::kDealApplyFamily, param);
	}
	
}

void DealApplyFamily::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible);
	if (bVisible)
	{
		m_pMenuItemAutoBuyJetton2Max->setSelectedIndex(1) ;
	}
}