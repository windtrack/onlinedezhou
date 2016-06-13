#include "SelfClub.h"

#include "Tools.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "external/ConvertUTF/ConvertUTF.h"
#include "GameHall.h"
#include "GameFrame.h"
#include "FrameSpriteButton.h"
#include "FrameSprite.h"
#include "GrabTouch.h"
#include "EditFamilyRole.h"
#include "FrameScale9SpriteButton.h"
#include "AllotFund.h"
USING_NS_CC ;

enum 
{
    RankScrollW = 816,
    RankScrollH = 461
};

enum BgItem
{
	ListBgItemW = 806,
	ListBgItemH = 112
};




bool SelfClub::init()
{
	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	s_uiroot = CSLoader::createNode("res/ui/club_myclub.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(s_uiroot,1);


	s_dataLoading = nullptr;

	m_img_edit = static_cast<ImageView*>(Tools::getWidgetByName(s_uiroot,"Image_6")) ;
	m_text_clubNotice = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_1")) ;

	ImageView*  m_img_noticeBg= static_cast<ImageView*>(Tools::getWidgetByName(s_uiroot,"Image_5")) ;
	Size szCount = m_img_noticeBg->getContentSize();
	m_editBox_Notice = CreateNode2AndPosAddTo<ui::EditBox>(Size(szCount.width, szCount.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szCount.width/2, szCount.height/2, m_img_noticeBg, 0);
	m_editBox_Notice->setFont("fzlt.ttf", 24);
	m_editBox_Notice->setFontColor(Color3B::BLACK);
	m_editBox_Notice->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_editBox_Notice->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editBox_Notice->setDelegate(this);	


	// 头像
	m_bt_editIcon = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_editIcon")) ;
	m_bt_editIcon->addTouchEventListener(CC_CALLBACK_2(SelfClub::OnMenuItemSelect, this));
	auto pClipMask = Sprite::create("family/club_iconmask.png");
	pClipMask->setPosition(m_bt_editIcon->getContentSize().width/2,m_bt_editIcon->getContentSize().height/2);
	pClipMask->retain();

	cocos2d::ClippingNode *m_pClipTouXiang = CreateNodeAndPosAddTo<ClippingNode>(0,0, m_bt_editIcon, 0);
	m_pClipTouXiang->setContentSize(Size(195, 195));
	m_httpSprite_clubIcon = CreateNode2AndPosAddTo<HttpSprite>("family/defaulticon.png", Size(188, 188), 0, 0, m_pClipTouXiang, 0);
	m_httpSprite_clubIcon->ignoreAnchorPointForPosition(false);
	m_httpSprite_clubIcon->setAnchorPoint(Point(0.5,0.5));
	m_httpSprite_clubIcon->setPosition(m_bt_editIcon->getContentSize().width/2,m_bt_editIcon->getContentSize().height/2);
	m_pClipTouXiang->setStencil(pClipMask);
	m_pClipTouXiang->setAlphaThreshold(0.5);
	g_globalMyFamilyInfo.WaitChange( std::bind(&SelfClub::updateIcon, this));
	

	m_text_clubName = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_clubname")) ;
	m_text_clubFund = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_money")) ;

	m_bt_FundInfo = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_fundinfo")) ;
	m_bt_FundInfo->addTouchEventListener(CC_CALLBACK_2(SelfClub::OnMenuItemSelect, this));

	m_bt_QuitClub = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_quite")) ;
	m_bt_QuitClub->addTouchEventListener(CC_CALLBACK_2(SelfClub::OnMenuItemSelect, this));

	m_bt_invateRole = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_invate")) ;
	m_bt_invateRole->addTouchEventListener(CC_CALLBACK_2(SelfClub::OnMenuItemSelect, this));


	m_imgNoRoleTips = static_cast<ImageView*>(Tools::getWidgetByName(s_uiroot,"Image_noplayer")) ;
	m_text_curRole = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_playernum")) ;
	m_text_maxRole = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_playernum")) ;

	m_img_editIcon = static_cast<ImageView*>(Tools::getWidgetByName(s_uiroot,"Image_8")) ;


	Layout* m_allRole = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_playershow")) ;
	sizeListView.width = m_allRole->getContentSize().width;
	sizeListView.height = m_allRole->getContentSize().height;


	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,-10,4,m_allRole,0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);


	return true ;
}

void SelfClub::setVisible(bool bVisible)
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

void SelfClub::updateSelfClubInfo()
{
	//g_globaSelfJoinClub
	//g_globalMyFamilyInfo.get
	if (g_globalMyFamilyInfo.m_isOwner)
	{
		m_imgNoRoleTips->setVisible(g_globalMyFamilyInfo.m_allTopRank.size()<=0) ;
	}
	else
	{
		m_imgNoRoleTips->setVisible(false) ;
	}
	

	updateIcon();
	char buff[64];
	setUITextString(m_text_clubName,g_globalMyFamilyInfo.m_familyName.c_str());

	setUITextString(m_text_clubFund,FormatCash(g_globalMyFamilyInfo.m_fund).c_str());

	m_str_notice = g_globalMyFamilyInfo.m_familyNotice.c_str();
	setUITextString(m_text_clubNotice,g_globalMyFamilyInfo.m_familyNotice.c_str());
	
	sprintf(buff,"(%d/%d)",g_globalMyFamilyInfo.m_curRoleCount,g_globalMyFamilyInfo.m_maxRoleCount) ;
	setUITextString(m_text_curRole,buff);


	m_img_editIcon->setVisible(g_globalMyFamilyInfo.m_isOwner);
	m_img_edit->setVisible(g_globalMyFamilyInfo.m_isOwner);
	m_bt_editIcon->setEnabled(g_globalMyFamilyInfo.m_isOwner) ;
	m_bt_invateRole->setVisible(g_globalMyFamilyInfo.m_isOwner) ;

	m_editBox_Notice->setEnabled(g_globalMyFamilyInfo.m_isOwner);
	updateAllRoleView() ;
}

void SelfClub::OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type)
{
	if (type == Button::TouchEventType::ENDED)
	{
		if (pSend == m_bt_FundInfo)
		{
			g_pGameFrame->showSelfFundLayer(true,1) ;
		}
		else if (pSend == m_bt_invateRole)
		{
			auto pHall = dynamic_cast<GameHall*>(getParent());
			pHall->showInviteFriends(true) ;
		}
		else if (pSend == m_bt_QuitClub)
		{
			if (g_globalMyFamilyInfo.m_isOwner)
			{
				PopTipWithBt1("您需要转让俱乐部后才能退出!", "确定", nullptr);
			}
			else
			{
				PopTipWithBt2("是否确定退出俱乐部！","退出","取消",[](unsigned ubt)
				{
					if(ubt == 0)
					{
						Json::Value param;
						param["user_id"] = g_globalMyData.m_iUserId;
						param["group_id"] = g_globalMyFamilyInfo.m_familyID;
						SendClientRequest(ClientRequest::kPushOutFamily, param);
					}
				});
			}
		}
		else if (pSend == m_bt_editIcon)
		{
			if (!g_globalMyFamilyInfo.m_isOwner)
			{
				return ;
			}
			auto pHall = dynamic_cast<GameHall*>(getParent());
			pHall->showChooseFamilyIcon(true);
		}
	}
}

void SelfClub::editBoxEditingDidBegin(EditBox* editBox)
{
	if (editBox == m_editBox_Notice)
	{
		editBox->setText(m_str_notice.c_str()) ;
	}
}
void SelfClub::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	if (editBox == m_editBox_Notice)
	{
		m_editBox_Notice->setVisible(true) ;

		std::string s_gonggao = editBox->getText();

		auto pStr = s_gonggao.c_str();
		const UTF8*pUtf8Str = (const UTF8*)pStr;
		const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));

		unsigned uL = getUTF8StringLength(pUtf8Str);


		if (uL>30)
		{
			setUITextString(m_text_clubNotice,m_str_notice.c_str());
			editBox->setText("") ;
			PopTipWithBt1("公告字数过度，请输入小于30个字","确定",nullptr);
			return ;
		}

		setUITextString(m_text_clubNotice,s_gonggao);
		m_str_notice = s_gonggao ;
		editBox->setText("") ;

		//修改俱乐部公告EditFamliyNotice
		Json::Value msg;
		msg["fnotice"] = s_gonggao;
		msg["ufamilyid"] = g_globalMyFamilyInfo.m_familyID;
		SendClientRequest(ClientRequest::kEditFamliyNotice,msg);

	}
}

void SelfClub::updateIcon()
{
	if (g_globalMyData.GetFamilySkin(g_globalMyFamilyInfo.getFamilyIconID()).m_strIconUrl!="")
	{
		SetFamilyIconUrl(m_httpSprite_clubIcon, g_globalMyData.GetFamilySkin(g_globalMyFamilyInfo.getFamilyIconID()).m_strIconUrl);
	}
	else
	{
		m_httpSprite_clubIcon->ResetToLocalSprite("family/defaulticon.png"); 
	}

}

void SelfClub::updateAllRoleView()
{
	//log("update view %d", uType);
	for(auto item:m_vecFollowListInfoNew)
	{
		item->m_pSpriteBgItem->setVisible(false);
		m_vecFollowListInfoOld.push_back(item);
	}

	m_vecFollowListInfoNew.clear();
	unsigned uCount = g_globalMyFamilyInfo.m_allTopRank.size();
	unsigned ListBgItemH = 686;
	unsigned uItemH = 112;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2+10;
	float fY = fRealH - uItemH / 2 ;

	for(auto& followList:g_globalMyFamilyInfo.m_allTopRank)
	{
		auto item = GetFamilyListItem(followList);
		UpdateItem(item, followList);
		item->m_pSpriteBgItem->setPosition(fX, fY);
		item->m_pSpriteBgItem->setVisible(true); 
		m_vecFollowListInfoNew.push_back(item);
		fY -= uItemH;     
	}
	float temp = fRealH - sizeListView.height;

	//每次都显示滚动框的最上面
	m_pScrollView->setContentOffset(Vec2(0,-temp), false);
	m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
	m_pScrollView->setViewSize(sizeListView);
	setShowDataLoading(false) ;
}




SelfClub::RankRoleItem *SelfClub::GetFamilyListItem(FamilyRoleBaseInfo baseInfo)
{
	RankRoleItem *item = nullptr;

	Scale9Sprite *bgItem = Scale9Sprite::create("family/club_itembg.png",Rect(0,0,164,110),Rect(46,25,160-46*2,110-67));
	bgItem->setPreferredSize(Size(584,110)) ;
	m_pScrollView->addChild(bgItem) ;


	unsigned m_uid = baseInfo.m_uid ;
	auto btDetail = LayerButton::create(false, true);
	bgItem->addChild(btDetail);


	auto pClipMask = Sprite::create("family/iconclipmask.png");
	auto sz = pClipMask->getContentSize();
	pClipMask->setPosition(sz.width / 2, sz.height / 2);
	pClipMask->retain();
	auto pClip = CreateNodeAndPosAddTo<ClippingNode>(18,10, bgItem, 0);
	pClip->setContentSize(sz);

	auto m_pHettpSpriteIcon = CreateNode2AndPosAddTo<HttpSprite>("defaulticon.png", sz, 0, 0, pClip, 0);
	auto m_pSpriteNormalIcon = CreateNode1AndPosAddTo<Sprite>("family/gf_family_icon.png", 18 + sz.width / 2,10 + sz.height / 2, bgItem, 0);
	auto m_pSpriteOwnerIcon = CreateNode1AndPosAddTo<Sprite>("family/club_ownericon.png", 18 + sz.width / 2,10 + sz.height / 2, bgItem, 0);
	pClip->setStencil(pClipMask);
	pClip->setAlphaThreshold(0.5);



	//人物名称
	auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0,0.5),123,78,Color4B(0xff,0xd2,0x00,0xff),bgItem,0);
	//已分配
	auto hasFengpei = CreateLabelMSYHAndAnchorPosClrAddTo(22,"已分配",Vec2(0,0.5),123,17,Color4B(0x79,0xdc,0x98,0xff),bgItem,0);

	auto label_owner = CreateLabelMSYHAndAnchorPosClrAddTo(20,"俱乐部所有者",Vec2(0,0.5),123,47,Color4B(0x50,0xbd,0x2e,0xff),bgItem,0);


	//加入按钮
	auto JiaRuBg = FrameScale9SpriteButton::createWithScale9Sprite(1, Size(132, 54),false,true);
	auto labeljiaru = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"分配",Vec2(0.5,0.5),0,0,Color4B::WHITE,JiaRuBg,0);
	JiaRuBg->setPosition(477+15,110/2);
	bgItem->addChild(JiaRuBg);

	auto pItem = new RankRoleItem;
	pItem->m_pSpriteBgItem = bgItem;
	pItem->m_pBtDetail = btDetail;
	pItem->m_pHttpSpriteIcon = m_pHettpSpriteIcon;
	pItem->m_pSpriteNormalIcon = m_pSpriteNormalIcon ;
	pItem->m_pSpriteOwnerIcon = m_pSpriteOwnerIcon ;
	pItem->m_familyName = labelName;
	pItem->m_uid = m_uid ;
	pItem->m_pBtCon = JiaRuBg ;
	pItem->m_hasCash = hasFengpei ;
	pItem->m_Owner = label_owner ;
	item = pItem;

	//背景回调
	item->m_pBtDetail->setContentSize(Size(RankScrollW,106));
	return item;
}

void SelfClub::UpdateItem(SelfClub::RankRoleItem *item, const FamilyRoleBaseInfo& followInfo)
{
	item->m_uid = followInfo.m_uid ;
	
	char s_buf[64] ;
	//头像
	SetIconUrl(item->m_pHttpSpriteIcon, followInfo.m_iconUrl);

	item->m_pSpriteOwnerIcon->setVisible(followInfo.m_isOwnner) ;
	item->m_pSpriteNormalIcon->setVisible(!followInfo.m_isOwnner) ;
	//名称
	SetLabelString(item->m_familyName,followInfo.m_uname.c_str());

	sprintf(s_buf,"已分配：%s",FormatCash(followInfo.m_hasCash).c_str()) ;
	//排名

	SetLabelString(item->m_hasCash,s_buf);
	item->m_pBtDetail->setContentSize(Size(415,95));
	item->m_pBtDetail->SetHitCB(std::bind(&SelfClub::menuClickBgItem, this,followInfo));
	item->m_pBtCon->SetHitCB(std::bind(&SelfClub::doAllotFund, this,followInfo));

	item->m_pBtCon->setVisible(g_globalMyFamilyInfo.m_isOwner) ;
	item->m_hasCash->setVisible(g_globalMyFamilyInfo.m_isOwner);
	item->m_Owner->setVisible(followInfo.m_isOwnner) ;
}

void SelfClub::doAllotFund(FamilyRoleBaseInfo baseInfo)
{
	auto pHall = dynamic_cast<GameHall*>(getParent());
	pHall->showAllotFund(true,g_globalMyFamilyInfo.m_familyID,baseInfo.m_uid,g_globalMyData.m_iUserId,baseInfo.m_uname) ;

}
void SelfClub::menuClickBgItem(FamilyRoleBaseInfo baseInfo)
{
		auto pHall = dynamic_cast<GameHall*>(getParent());
	if (!g_globalMyFamilyInfo.m_isOwner)
	{
		Json::Value param;
		param["tid"] = baseInfo.m_uid;
		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
		return ;
	}


	//点的自己
	if (g_globalMyData.m_iUserId==baseInfo.m_uid)
	{
		Json::Value param;
		param["tid"] = baseInfo.m_uid;
		SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
		return ;
	}

	if (g_globalMyFamilyInfo.m_isOwner)
	{
		pHall->showEditFamilyRole(true,baseInfo.m_uid,baseInfo.m_uname,(Edit_LookInf0|Edit_To_EditFund|Edit_ChangeShaikh|Edit_Push));
	}

}

void SelfClub::setShowDataLoading(bool flag)
{
	if (s_dataLoading == nullptr)
	{
		s_dataLoading = Tools::creatDataLoading(this,587, 266,99) ;
	}
	s_dataLoading->setVisible(flag) ;
}

void SelfClub::updateRoleFund(double clubFund,unsigned uid,double uFund)
{
		g_globalMyFamilyInfo.m_fund =  clubFund ;
		setUITextString(m_text_clubFund,FormatCash(clubFund).c_str());

		for (auto &role:g_globalMyFamilyInfo.m_allTopRank)
		{
			if (role.m_uid == uid)
			{
				role.m_hasCash = uFund ;
				for(auto item:m_vecFollowListInfoNew)
				{
					if (item->m_uid == uid)
					{
						UpdateItem(item,role);
					}
				}
			}
		}
}





void SelfClub::updateNotice(std::string notice)
{
	m_str_notice = notice;
	g_globalMyFamilyInfo.m_familyNotice = notice ;
	setUITextString(m_text_clubNotice,m_str_notice.c_str());
}


void SelfClub::updateRoleItem(const FamilyRoleBaseInfo& baseInfo)
{

	for(auto item:m_vecFollowListInfoNew)
	{
		if (item->m_uid == baseInfo.m_uid)
		{
			UpdateItem(item,baseInfo);
		}
	}

}

void SelfClub::removeRole(unsigned uid)
{
		for(int i=0; i<m_vecFollowListInfoNew.size(); i++)
		{
			if(m_vecFollowListInfoNew[i]->m_uid == uid)
			{
				unsigned uItemH = ListBgItemH;
				m_vecFollowListInfoNew[i]->m_pSpriteBgItem->setVisible(false);
				m_vecFollowListInfoNew.erase(m_vecFollowListInfoNew.begin() + i);
				unsigned uCount = m_vecFollowListInfoNew.size();
				unsigned fRealH = uCount * uItemH;
				if(fRealH < sizeListView.height)
				{
					fRealH = sizeListView.height;
				}
				float fX = sizeListView.width / 2+10;
				float fY = fRealH - uItemH / 2 ;
	
				for(auto& followList:m_vecFollowListInfoNew)
				{
					followList->m_pSpriteBgItem->setPosition(fX, fY);
					followList->m_pSpriteBgItem->setVisible(true); 
					fY -= uItemH;     
				}
				float temp = fRealH - sizeListView.height;
				//每次都显示滚动框的最上面
				m_pScrollView->setContentOffset(Vec2(0,-temp), false);
				m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
			}
		}
}