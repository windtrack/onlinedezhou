#include "InviteFriends.h"

#include "MenuItemFrameSprite.h"
#include "MenuItemScale9Sprite.h"
#include "GlobalUserData.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "HttpSprite.h"
#include "FrameSprite.h"
#include <string.h>
#include <stdio.h>

USING_NS_CC ;
using namespace  std;

enum 
{
	FriendsScrollW = 586,
	FriendsScrollH = 275
};

enum 
{
	FriendsBTW = 130,
	FriendsBTH = 140
};

bool InviteFriends::init()
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
	m_ScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(FriendsScrollW, FriendsScrollH), sz.width/2,230, this, 0);
	m_ScrollView->ignoreAnchorPointForPosition(false);
	m_ScrollView->setAnchorPoint(Point(0.5,0.5)) ;
	m_ScrollView->setContentSize(Size(FriendsScrollW,FriendsScrollH));
	m_ScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_ScrollView->setBounceable(true);
	m_ScrollView->setSwallowsTouches(false) ;
	m_ScrollView->setVisible(true);



	m_lableTipsup =  CreateLabelMSYHAndAnchorPosClrAddTo(30,"邀请好友加入俱乐部",Vec2(0.5,0.5),348,433,Color4B(0xff,0xd2,0x00,0xff),this,0);
	m_lableTipsdown =  CreateLabelMSYHAndAnchorPosClrAddTo(22,"当年人数：0人 成员上限：0人",Vec2(0.5,0.5),348,392,Color4B::WHITE,this,0);



	ccMenuCallback menuCB = CC_CALLBACK_1(InviteFriends::onMenuItemSelected, this);
	//m_bttoViceShaikh =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
    m_bttoViceShaikh = MenuItemScale9Sprite::create(0, Size(204, 74),menuCB);
	m_bttoViceShaikh->setPosition(228,43);
	m_bttoViceShaikh->ignoreAnchorPointForPosition(false) ;
	m_bttoViceShaikh->setAnchorPoint(Point(0.5,0.5)) ;

	auto labelVc =  CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"邀请",Vec2(0.5,0.5),101,33,Color4B::WHITE,m_bttoViceShaikh,0);
	labelVc->ignoreAnchorPointForPosition(false) ;
	labelVc->setAnchorPoint(Point(0.5,0.5)) ;


	ccMenuCallback menuCB2 = CC_CALLBACK_1(InviteFriends::onMenuItemSelected, this);
	m_btclose = MenuItemScale9Sprite::create(0, Size(204, 74),menuCB2);
	m_btclose->setPosition(469,44);

	auto labelclose =  CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"取消",Vec2(0.5,0.5),101,33,Color4B::WHITE,m_btclose,0);

	auto pMenu = Menu::create(m_bttoViceShaikh,m_btclose,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu, 1);


	return true ;
}



void InviteFriends::updateView()
{


	char buff[64] ;
	sprintf(buff,"当前人数：%d人 成员上限: %d人",g_globalMyFamilyInfo.getFamilyCurRole(),g_globalMyFamilyInfo.getFamilyMaxRole()) ;

	SetLabelStringWithBoundWidth(m_lableTipsdown,buff,400);


	std::vector<RoleIconItem*>::iterator iter;

	while (!m_vecFriendsOld.empty())
	{
		iter = m_vecFriendsOld.begin();
		RoleIconItem* item = m_vecFriendsOld.at(0) ;
		item->m_pSpriteBgItem->removeFromParent() ;
		m_vecFriendsOld.erase(iter) ;

	}

	m_vecInviteID.clear() ;
	m_vecFriendsOld.clear();																																							

	Size sz = this->getContentSize() ;
	unsigned uCount = g_globalMyFamilyInfo.m_allFriends.size();
	unsigned ListBgItemH = 143;
	unsigned uItemH = FriendsBTH;
	unsigned fRealH = uCount/4 * uItemH;
	if (uCount%4!=0)
	{
		fRealH+=uItemH ;
	}

	if(fRealH < FriendsScrollH)
	{
		fRealH = FriendsScrollH;
	}
	float fX = 0;
	float fY = fRealH - uItemH / 2 - 20;

	for (int i=0; i<g_globalMyFamilyInfo.m_allFriends.size(); i++)
	{
		auto followList = g_globalMyFamilyInfo.m_allFriends.at(i) ;
		auto item = GetFamilyListItem(followList);		
		UpdateItem(item, followList);

		fX = (i%4*150) ;
		if (i%4==0&&i>0)
		{
			fY-=143;
		}
		item->m_pSpriteBgItem->setPosition(fX, fY);
		m_vecFriendsOld.push_back(item);
	}

	float temp = fRealH - FriendsScrollH;

	//每次都显示滚动框的最上面
	m_ScrollView->setContentOffset(Vec2(0,-temp), false);
	m_ScrollView->setContentSize(Size(FriendsScrollW,fRealH));

}





InviteFriends::RoleIconItem *InviteFriends::GetFamilyListItem(FamilyRoleBaseInfo baseInfo)
{
	RoleIconItem *item = nullptr;
	auto node = Node::create() ;
	node->setContentSize(Size(FriendsBTW,FriendsBTH));
	m_ScrollView->addChild(node) ;

	HttpSprite* m_spriteAdmin =  CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", node, Vec2(0,0), "family/gf_family_select_icon_1.png");


	LayerButton* btDetail = LayerButton::create(false, true);
	m_spriteAdmin->addChild(btDetail);


	//人物名称
	Label* labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),88/2,-26,Color4B::WHITE,node,1);



	 auto m_spriteselect = Sprite::create("family/gf_family_select_icon_2.png");
	 node->addChild(m_spriteselect);
	 m_spriteselect->setPosition(44,44);
	 m_spriteselect->setVisible(false) ;

	auto pItem = new RoleIconItem;
	pItem->m_pSpriteBgItem = node ;
	pItem->m_pSpriteRoleIcon = m_spriteAdmin;
	pItem->m_pBtDetail = btDetail;
	pItem->m_uName = labelName;
	pItem->m_pSpriteSelect = m_spriteselect ;

	item = pItem;

	//背景回调
	item->m_pBtDetail->setContentSize(Size(88,100));
	return item;
}

void InviteFriends::UpdateItem(InviteFriends::RoleIconItem *item, const FamilyRoleBaseInfo& followInfo)
{
	SetIconUrl(item->m_pSpriteRoleIcon, followInfo.m_iconUrl);
	SetLabelStringWithBoundWidth(item->m_uName,followInfo.m_uname.c_str(), 100);


	item->m_pBtDetail->setContentSize(Size(88,100));
	item->m_pBtDetail->SetHitCB(std::bind(&InviteFriends::menuClickBgItem, this,followInfo,item));


	item->m_uid = followInfo.m_uid ;
}


void InviteFriends::menuClickBgItem(FamilyRoleBaseInfo baseInfo,InviteFriends::RoleIconItem *item)
{

	//baseInfo.m_uid;

	if (item->m_pSpriteSelect->isVisible())
	{
		item->m_pSpriteSelect->setVisible(false) ;

		std::vector<int>::iterator iter;

		iter = m_vecInviteID.begin();

		for (int i=0;i<m_vecInviteID.size();i++)
		{
			if (m_vecInviteID.at(i) == baseInfo.m_uid)
			{
				m_vecInviteID.erase((iter+i));
			}
		}
	}
	else
	{
		item->m_pSpriteSelect->setVisible(true);
		m_vecInviteID.push_back(baseInfo.m_uid);
	}
	
}


void InviteFriends::onMenuItemSelected(cocos2d::Ref * ref)
{
	if (ref == m_btclose)
	{
		clearAllSelect() ;
		GameDialog::setVisible(false) ;
		return ;
	}
	if (ref == m_bttoViceShaikh)
	{
		if (m_vecInviteID.size()==0)
		{
			PopTipWithBt1("请选择一个好友","确定",nullptr) ;
			return ;
		}
		Json::Value msg;
		msg["ufamilyid"] = g_globalMyFamilyInfo.getFamilyID();
		char suid[128] ;
		std::string sid ="";
		for (int i=0 ;i< m_vecInviteID.size(); i++)
		{
			
			sprintf(suid,"%d,",m_vecInviteID.at(i)) ;
			sid += suid ;
		
			for (auto list:m_vecFriendsOld)
			{
				if (list->m_uid == m_vecInviteID.at(i))
				{
					list->m_pSpriteSelect->setVisible(false) ;
				}
			}
		}
			msg["uid"] = sid;
			SendClientRequest(ClientRequest::kInviteFreinds,msg);

			m_vecInviteID.clear();



	}
}


void InviteFriends::onGrabTouchBegan(bool bHitDialog)
{
	GameDialog::onGrabTouchBegan(bHitDialog);
	if (!bHitDialog)
	{
		//m_spriteselect->setVisible(false) ;
		clearAllSelect() ;
	}

}

/************************************************************************/
/* 清除所有选择状态                                                                     */
/************************************************************************/
void InviteFriends::clearAllSelect()
{
	std::vector<RoleIconItem*>::iterator iter;

	while (!m_vecFriendsOld.empty())
	{
		iter = m_vecFriendsOld.begin();
		RoleIconItem* item = m_vecFriendsOld.at(0) ;
		item->m_pSpriteBgItem->removeFromParent() ;
		m_vecFriendsOld.erase(iter) ;
	}

	m_vecInviteID.clear() ;
	m_vecFriendsOld.clear();	
}
/************************************************************************/
/* 移除指定的item                                                                     */
/************************************************************************/
void InviteFriends::removeRoleItem(std::string aluid)
{


	
	std::stringstream ss(aluid);
	std::string sub_str;
	while(getline(ss,sub_str,','))
	{
		/*	char uuid[64] ;
		sprintf(uuid,"%d",sub_str) ;*/

		unsigned uid = std::atoi( sub_str.c_str() );;
		for (int i=0;i<m_vecFriendsOld.size();i++)
		{
			RoleIconItem* item = m_vecFriendsOld.at(i);
			if (item->m_uid == uid)
			{
				item->m_pSpriteBgItem->setVisible(false) ;
				m_vecFriendsOld.erase(m_vecFriendsOld.begin()+i);


				//return ;
			}

		}
	}
		//cout << sub_str << endl;





	Size sz = this->getContentSize() ;
	unsigned uCount = m_vecFriendsOld.size();
	unsigned ListBgItemH = 143;
	unsigned uItemH = FriendsBTH;
	unsigned fRealH = uCount/4 * uItemH;
	if (uCount%4!=0)
	{
		fRealH+=uItemH ;
	}

	if(fRealH < FriendsScrollH)
	{
		fRealH = FriendsScrollH;
	}
	float fX = 0;
	float fY = fRealH - uItemH / 2 - 20;

	for (int i=0; i<m_vecFriendsOld.size(); i++)
	{
		auto followList = m_vecFriendsOld.at(i) ;

		fX = (i%4*150) ;
		if (i%4==0&&i>0)
		{
			fY-=143;
		}
		followList->m_pSpriteBgItem->setPosition(fX, fY);
	}

	float temp = fRealH - FriendsScrollH;
	m_ScrollView->setContentOffset(Vec2(0,-temp), false);
	m_ScrollView->setContentSize(Size(FriendsScrollW,fRealH));


}