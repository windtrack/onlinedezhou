#include "ChangeViceShaikh.h"
#include "MenuItemFrameSprite.h"
#include "MenuItemScale9Sprite.h"
#include "GlobalUserData.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "HttpSprite.h"
#include "FrameSprite.h"



USING_NS_CC ;


enum 
{
	ShaikhScrollW = 586,
	ShaikhScrollH = 275
};

enum 
{
	ShaikhBTW = 130,
	ShaikhBTH = 140
};

bool ChangeViceShaikh::init()
{
	if (!GameDialog::init())
	{
		return false ;
	}


	auto bg = Sprite::create("family/informationbg.png");
	auto sz = bg->getContentSize();
	bg->setPosition(sz.width / 2, sz.height / 2);
	setContentSize(sz);
	addChild(bg);

	m_layerTop = GameLayer::create(sz.width, 143, false, false);
	m_layerTop->setPosition(Vec2(0, 367));
	this->addChild(m_layerTop, 1);



	m_layerBottom = GameLayer::create(sz.width, 86, false, false);
	m_layerBottom->setPosition(Vec2(0, 0));
	this->addChild(m_layerBottom, 1);

		//滚动视图
	m_ScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ShaikhScrollW, ShaikhScrollH), sz.width/2,230, this, 0);
	m_ScrollView->ignoreAnchorPointForPosition(false);
	m_ScrollView->setAnchorPoint(Point(0.5,0.5)) ;
	m_ScrollView->setContentSize(Size(ShaikhScrollW,ShaikhScrollH));
	m_ScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	//m_pScrollView->setContentSize(Size(ListScrollW,ListScrollH));
	m_ScrollView->setBounceable(true);
	m_ScrollView->setSwallowsTouches(false) ;
	m_ScrollView->setVisible(true);


	m_spriteselect = Sprite::create("family/gf_family_select_icon_2.png");
	m_spriteselect->setVisible(false) ;
	m_ScrollView->addChild(m_spriteselect,100);




	m_lableTipsup =  CreateLabelMSYHAndAnchorPosClrAddTo(30,"选择一个成员",Vec2(0.5,0.5),348,433,Color4B(0xff,0xd2,0x00,0xff),this,0);
	m_lableTipsdown =  CreateLabelMSYHAndAnchorPosClrAddTo(22,"副族长人数不得超过两人",Vec2(0.5,0.5),348,392,Color4B::WHITE,this,0);




	ccMenuCallback menuCB = CC_CALLBACK_1(ChangeViceShaikh::onMenuItemSelected, this);
	//m_bttoViceShaikh =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
    m_bttoViceShaikh =MenuItemScale9Sprite::create(0, Size(204, 74),menuCB);
	m_bttoViceShaikh->setPosition(228,43);
	m_bttoViceShaikh->ignoreAnchorPointForPosition(false) ;
	m_bttoViceShaikh->setAnchorPoint(Point(0.5,0.5)) ;

	auto labelVc =  CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"任命副族长",Vec2(0.5,0.5),101,33,Color4B::WHITE,m_bttoViceShaikh,0);
	labelVc->ignoreAnchorPointForPosition(false) ;
	labelVc->setAnchorPoint(Point(0.5,0.5)) ;


	ccMenuCallback menuCB2 = CC_CALLBACK_1(ChangeViceShaikh::onMenuItemSelected, this);
	//m_btclose =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
    m_btclose =MenuItemScale9Sprite::create(0, Size(204, 74),menuCB);
	m_btclose->setPosition(469,44);

	auto labelclose =  CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"取消",Vec2(0.5,0.5),101,33,Color4B::WHITE,m_btclose,0);

	auto pMenu = Menu::create(m_bttoViceShaikh,m_btclose,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu, 1);


	return true ;
}



void ChangeViceShaikh::updateView()
{




	std::vector<RoleIconItem*>::iterator iter;
		
	while (!m_vecNoPowerRoleOld.empty())
	{
		iter = m_vecNoPowerRoleOld.begin();
		RoleIconItem* item = m_vecNoPowerRoleOld.at(0) ;
		item->m_pSpriteBgItem->removeFromParent() ;
		m_vecNoPowerRoleOld.erase(iter) ;
		
	}
	


	m_spriteselect->setVisible(false) ;
	m_curUID = 0 ;																																										

	Size sz = this->getContentSize() ;


	unsigned uCount = g_globalMyFamilyInfo.m_NoPowerRole.size();
	unsigned ListBgItemH = 143;
	unsigned uItemH = ShaikhBTH;
	unsigned fRealH = uCount/4 * uItemH;

	if (uCount%4!=0)
	{
		fRealH+=uItemH ;
	}
	if(fRealH < ShaikhScrollH)
	{
		fRealH = ShaikhScrollH;
	}


	float fX = 0;
	float fY = fRealH - uItemH / 2 - 20;




	for (int i=0; i<g_globalMyFamilyInfo.m_NoPowerRole.size(); i++)
	{
			auto followList = g_globalMyFamilyInfo.m_NoPowerRole.at(i) ;
			auto item = GetFamilyListItem(followList);		
			UpdateItem(item, followList);

			fX = (i%4*150) ;
			if (i%4==0&&i>0)
			{
				fY-=143;
			}
			item->m_pSpriteBgItem->setPosition(fX, fY);

			m_vecNoPowerRoleOld.push_back(item);
			//m_vecFollowListInfoOld.push_back(item);
	}

	float temp = fRealH - ShaikhScrollH;

	//每次都显示滚动框的最上面
	m_ScrollView->setContentOffset(Vec2(0,-temp), false);
	m_ScrollView->setContentSize(Size(ShaikhScrollW,fRealH));

}





ChangeViceShaikh::RoleIconItem *ChangeViceShaikh::GetFamilyListItem(FamilyRoleBaseInfo baseInfo)
{
	RoleIconItem *item = nullptr;
	auto node = Node::create() ;
	node->setContentSize(Size(ShaikhBTW,ShaikhBTH));
	m_ScrollView->addChild(node) ;
	//auto spbg = Sprite::create("family/gf_family_select_icon_1.png");
	//m_ScrollView->addChild(spbg) ;
	HttpSprite* m_spriteAdmin =  CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", node, Vec2(0,0), "family/gf_family_select_icon_1.png");


	LayerButton* btDetail = LayerButton::create(false, true);
	m_spriteAdmin->addChild(btDetail);

	//人物名称
	Label* labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),88/2,-26,Color4B::WHITE,node,1);
	labelName->setHorizontalAlignment(TextHAlignment::CENTER);
	labelName->setVerticalAlignment(TextVAlignment::CENTER);
	auto pItem = new RoleIconItem;
	pItem->m_pSpriteBgItem = node ;
	pItem->m_pSpriteRoleIcon = m_spriteAdmin;
	pItem->m_pBtDetail = btDetail;
	pItem->m_uName = labelName;

	item = pItem;

	//背景回调
	item->m_pBtDetail->setContentSize(Size(88,100));
	return item;
}

void ChangeViceShaikh::UpdateItem(ChangeViceShaikh::RoleIconItem *item, const FamilyRoleBaseInfo& followInfo)
{
	SetIconUrl(item->m_pSpriteRoleIcon, followInfo.m_iconUrl);
	SetLabelStringWithBoundWidth(item->m_uName,followInfo.m_uname, 100);


	item->m_pBtDetail->setContentSize(Size(88,100));
	item->m_pBtDetail->SetHitCB(std::bind(&ChangeViceShaikh::menuClickBgItem, this,followInfo,item));

	

}


void ChangeViceShaikh::menuClickBgItem(FamilyRoleBaseInfo baseInfo,ChangeViceShaikh::RoleIconItem *item)
{
	m_curUID = baseInfo.m_uid;
	m_spriteselect->setVisible(true);
	Point pos = item->m_pSpriteBgItem->getPosition() ;
	m_spriteselect->setPosition(pos.x+44,pos.y+44);
	//Json::Value param;
	//param["tid"] = baseInfo.m_uid;
	//SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
	
}


void ChangeViceShaikh::onMenuItemSelected(cocos2d::Ref * ref)
{
	if (ref == m_btclose)
	{
		m_spriteselect->setVisible(false) ;
		GameDialog::setVisible(false) ;
		return ;
	}
	if (ref == m_bttoViceShaikh)
	{
		if (m_curUID!=0)
		{

			Json::Value param;
			param["user_id"] = m_curUID;
			param["group_id"] = g_globalMyData.getFamilyID();
			param["con"] =1;
			SendClientRequest(ClientRequest::kTo_ViceShaikh, param);


			m_spriteselect->setVisible(false) ;
			GameDialog::setVisible(false) ;

		}
	}
}


void ChangeViceShaikh::onGrabTouchBegan(bool bHitDialog)
{
	GameDialog::onGrabTouchBegan(bHitDialog);
	if (!bHitDialog)
	{
		m_spriteselect->setVisible(false) ;
	}

}
