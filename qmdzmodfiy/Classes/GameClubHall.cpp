#include "GameClubHall.h"
#include "GlobalUserData.h"
#include "Tools.h"
#include "Util.h"
#include "external/ConvertUTF/ConvertUTF.h"
#include "GameHall.h"

#include "FrameSprite.h"
#include "FrameSpriteButton.h"
#include "FrameScale9SpriteButton.h"
#include "HttpSprite.h"
#include <vector>
#include "GrabTouch.h"

USING_NS_CC ;

enum
{
	Club_Rank=0,
	Club_SelfClub,
	Club_ClubInfo,
	Club_CreatClub,
};
enum BgItem
{
	ListBgItemW = 696,
	ListBgItemH = 106
};

bool GameClubHall::init()
{
	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}
	s_dataLoading = nullptr ;



	Size sz = this->getContentSize() ;
	s_uiroot = CSLoader::createNode("res/ui/club_hall.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(s_uiroot,1);

	//四个按钮
	const	char* m_strBtControll[4] = {"Button_clubrank", "Button_muclub", "Button_clubinfo", "Button_clubcreat", } ;
	const	char* m_strView[4] = {"Panel_rank", "Panel_myclub", "Panel_clubinfo", "Panel_creatclub", } ;
	for (int i=0; i<4; i++)
	{
		m_btControll[i] = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,m_strBtControll[i])) ;
		m_btControll[i]->addTouchEventListener(CC_CALLBACK_2(GameClubHall::OnMenuItemSelect, this));

		m_textControll[i] = static_cast<Text*>(Tools::getWidgetByName(m_btControll[i],"Text_btclub")) ;

		m_layoutClubView[i] =  static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,m_strView[i])) ;
	}

	m_layoutNoClub = static_cast<Layout*>(Tools::getWidgetByName(m_layoutClubView[Club_SelfClub],"Panel_noClub")) ;


	ImageView* imgeditbox = static_cast<cocos2d::ui::ImageView*>(Tools::getWidgetByName(m_layoutClubView[Club_Rank], "Image_showeditserach"));

	m_editBoxSearch   = CreateNode2AndPosAddTo<ui::EditBox>(Size(imgeditbox->getContentSize().width, imgeditbox->getContentSize().height),
		ui::Scale9Sprite::create("vipprivateroom/bgedit.png") , imgeditbox->getContentSize().width / 2, imgeditbox->getContentSize().height / 2,imgeditbox, 0);

	m_editBoxSearch->setFont("fzlt.ttf", 20);
	m_editBoxSearch->setFontColor(Color3B(0xff,0xd8,0x00));
	m_editBoxSearch->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_editBoxSearch->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editBoxSearch->setDelegate(this);
	m_textSerachName = static_cast<Text*>(Tools::getWidgetByName(m_layoutClubView[0],"Text_5")) ; 


	ImageView* imgeditbox2 = static_cast<cocos2d::ui::ImageView*>(Tools::getWidgetByName(m_layoutClubView[Club_CreatClub], "Image_cludeditbox"));

	m_editBoxCreatClub   = CreateNode2AndPosAddTo<ui::EditBox>(Size(imgeditbox2->getContentSize().width, imgeditbox2->getContentSize().height),
		ui::Scale9Sprite::create("vipprivateroom/bgedit.png") , imgeditbox2->getContentSize().width / 2, imgeditbox2->getContentSize().height / 2,imgeditbox2, 0);

	m_editBoxCreatClub->setFont("fzlt.ttf", 24);
	m_editBoxCreatClub->setFontColor(Color3B(0x79,0xdc,0x98));
	m_editBoxCreatClub->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_editBoxCreatClub->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_editBoxCreatClub->setDelegate(this);
	m_textCreatClubName = static_cast<Text*>(Tools::getWidgetByName(m_layoutClubView[Club_CreatClub],"Text_creatnameshow")) ; 




	m_btSelfCreatClub = static_cast<Button*>(Tools::getWidgetByName(m_layoutClubView[Club_SelfClub],"Button_btcreatclub")) ;
	m_btSelfCreatClub->addTouchEventListener(CC_CALLBACK_2(GameClubHall::OnMenuItemSelect, this));

	m_btSelfJoinClub = static_cast<Button*>(Tools::getWidgetByName(m_layoutClubView[Club_SelfClub],"Button_joinclub")) ;
	m_btSelfJoinClub->addTouchEventListener(CC_CALLBACK_2(GameClubHall::OnMenuItemSelect, this));

	m_btCreatClub = static_cast<Button*>(Tools::getWidgetByName(m_layoutClubView[Club_CreatClub],"Button_tocreat")) ;
	m_btCreatClub->addTouchEventListener(CC_CALLBACK_2(GameClubHall::OnMenuItemSelect, this));




	sizeListView.width = m_layoutClubView[Club_Rank]->getContentSize().width;
	sizeListView.height = m_layoutClubView[Club_Rank]->getContentSize().height;


	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,-10,4,m_layoutClubView[Club_Rank],0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);



	m_pSelfClubScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,-10,4,m_layoutClubView[Club_SelfClub],0);
	m_pSelfClubScrollView->setContentSize(sizeListView);
	m_pSelfClubScrollView->setBounceable(true);
	m_pSelfClubScrollView->setSwallowsTouches(false) ;
	m_pSelfClubScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pSelfClubScrollView->setVisible(true);


	return true ;
}

void GameClubHall::setVisible(bool bVisible)
{
	if (bVisible)
	{
		
		m_btControll[3]->setVisible(!g_globalMyData.getHasCreatFamily()) ;
		SendClientRequest(ClientRequest::kGetAllFamilyList);
		SendClientRequest(ClientRequest::kCallSelfJoinClub);
		setUITextString(m_textSerachName,"搜索俱乐部") ;
		setUITextString(m_textCreatClubName,"支持四个汉字以内的俱乐部名称") ;

		doBtSelect(0) ;

		callShowAction() ;
	}
	else
	{
		callHideAction() ;
        SendClientRequest(ClientRequest::kOptHallBgLayer);
	}
}

void GameClubHall::OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type)
{
	if (type == Button::TouchEventType::ENDED)
	{
		for (int i=0; i<4; i++)
		{
			if (pSend ==m_btControll[i])
			{
				doBtSelect(i) ;
				return ;
			}
		}

		if (pSend == m_btCreatClub)
		{
			if (m_clubName!="")
			{
				Json::Value msg;
				msg["fname"] = m_clubName;
				SendClientRequest(ClientRequest::kCreatFamily,msg);	
			}
			else
			{
				PopTipWithBt1("请输入俱乐部名字!", "确定", nullptr);
			}
		}
		else if (pSend == m_btSelfCreatClub)
		{
			doBtSelect(3) ;
		}
		else if (pSend == m_btSelfJoinClub)
		{
			doBtSelect(0) ;
		}

	}
}

void GameClubHall::doBtSelect(int index)
{
	for (int i=0; i<4; i++)
	{
		m_textControll[i]->setTextColor(i==index?Color4B(0xff,0xff,0xff,0xff):Color4B(0x37,0xa4,0x5c,0xff)) ;
		m_layoutClubView[i]->setVisible(i == index) ;
		m_btControll[i]->setBright(i != index);
	}

	if (index == 0)
	{
		
		//setShowDataLoading(true) ;
		//UpdateGameClubView() ;
	}
	else if (index == 1)
	{
		
		
		//setShowDataLoading(true) ;

	} 
	else if (index == 2)
	{
	} 
	else if (index == 3)
	{
	
	} 

}
void GameClubHall::editBoxEditingDidBegin(EditBox* editBox)
{
	if (editBox == m_editBoxSearch)
	{
		m_textSerachName->setText("") ;
		setUITextString(m_textSerachName,"") ;
		m_editBoxSearch->setText("") ;

	}
	if (editBox == m_editBoxCreatClub)
	{
		m_textCreatClubName->setText("") ;
		setUITextString(m_textCreatClubName,"") ;
		m_editBoxCreatClub->setText("") ;
	}

	
}



void GameClubHall::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	if (editBox == m_editBoxSearch)
	{
		std::string name = editBox->getText();
		m_editBoxSearch->setText("") ;
		m_clubSerachName = name ;
		setUITextString(m_textSerachName,name.c_str()) ;
		if (name!="")
		{

			Json::Value param;
			param["name"] = name;
			SendClientRequest(ClientRequest::kSearchFamily, param);

		}

	}




	if (editBox == m_editBoxCreatClub)
	{
		std::string name = editBox->getText();

		auto pStr = name.c_str();
		const UTF8*pUtf8Str = (const UTF8*)pStr;
		const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));

		unsigned uL = getUTF8StringLength(pUtf8Str);

		if (uL>4)
		{
			m_textCreatClubName->setText("");
			editBox->setText("") ;
			m_clubName = "" ;
			PopTipWithBt1("最多输入四个汉字","确定",nullptr) ;

	
		}
		else
		{
		
			bool isChinese = true ;
			int nlen = uL; 
			for(int i=0; i < nlen; i++) 
			{ 
				if( pStr[i] >= 0 && pStr[i] <= 127 ) //不是全角字符？
				{
		
					isChinese = false ;
				}
				
				else //是全角字符
				{
					 i++; //中文是2个字节,所以i++
		
				}
						
			} 


			if (!isChinese)
			{
				editBox->setText("") ;
				PopTipWithBt1("只能输入中文","确定",nullptr);
				return ;
			}

			m_textCreatClubName->setText(name);
			editBox->setText("") ;
			m_clubName = name ;
		}
	}
}





GameClubHall::FamilywListItem *GameClubHall::GetFamilyListItem(unsigned m_familyID)
{
	FamilywListItem *item = nullptr;
	if(m_vecFollowListInfoOld.empty())
	{
		
		auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("family/fanlilyblock.png", 1, 2, sizeListView.width / 2+50, 0, m_pScrollView, 0);
		auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
		touchGrabber->retain();
		touchGrabber->GetTouchProcer().SetHitCB(
			[bgItem](bool bSel, Node*, TouchGrabber*){
				bgItem->SetFrameIdx(bSel?1:0);
		}
		//[this,m_familyID](bool bSel, Node*, TouchGrabber*){if (bSel) menuClickBgItem(m_familyID);}

		);
			
		auto btDetail = LayerButton::create(false, true);
		bgItem->addChild(btDetail);
		//m_pScrollView->addChild(bgItem);
		//俱乐部头像
		auto pTouXiang = CreateUserHeadSprite("family/defaulticon.png", "family/iconclipmask.png", bgItem, Vec2(10,8), "family/gf_family_icon.png");
		//俱乐部昵称
		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),165,47,Color4B::WHITE,bgItem,0);

		//加入按钮
		auto JiaRuBg = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86),false,true);
		auto m_labelStr = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"加入",Vec2(0.5,0.5),0,0,Color4B::WHITE,JiaRuBg,0);
		JiaRuBg->setPosition(611,53);

		bgItem->addChild(JiaRuBg);

			

		auto RoleCount = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0.5,0.5),413,47,Color4B(0x79,0xdc,0x98,0xff),bgItem,0);

		auto btSpriteBgRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 96, bgItem, 3);

		btSpriteBgRank->SetFrameIdx(3);

		auto labelRandid = CreateLabelMSYHAndAnchorPosClrAddTo(16,"1",Vec2(0.5,0.5),20,94,Color4B(0xff,0xd2,0x00,0xff),bgItem,5);



		auto pItem = new FamilywListItem;
		pItem->m_pSpriteBgItem = bgItem;
		pItem->m_pBtDetail = btDetail;
		pItem->m_pSpriteFamilyTouXiang = pTouXiang;
		pItem->m_familyName = labelName;
		pItem->m_labelRankIndex = labelRandid ;
		pItem->m_labelRoleCount = RoleCount;
		pItem->m_pBtJiaRu = JiaRuBg;
		pItem->m_labelStr = m_labelStr ;
		pItem->m_spriteRank = btSpriteBgRank;
		item = pItem;


	}else{
		item=m_vecFollowListInfoOld.back();
		m_vecFollowListInfoOld.pop_back();
	}
	item->m_familyID = m_familyID;
	//加入回调
	item->m_pBtJiaRu->SetHitCB(std::bind(&GameClubHall::menuClickJiaRu, this, m_familyID));
	//背景回调
	item->m_pBtDetail->setContentSize(Size(ListBgItemW-150,ListBgItemH));
	//item->m_pBtDetail->SetHitCB(std::bind(&GameClubHall::menuClickBgItem, this,m_familyID));
	return item;
}


GameClubHall::FamilywListItem *GameClubHall::GetSelfClubFamilyListItem(unsigned m_familyID)
{
	FamilywListItem *item = nullptr;
	if(m_vecSelfClubFollowListInfoOld.empty())
	{

		auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("family/fanlilyblock.png", 1, 2, sizeListView.width / 2+50, 0, m_pSelfClubScrollView, 0);
		auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
		touchGrabber->retain();
		touchGrabber->GetTouchProcer().SetHitCB(
			[bgItem](bool bSel, Node*, TouchGrabber*){
				bgItem->SetFrameIdx(bSel?1:0);
		}
		//[this,m_familyID](bool bSel, Node*, TouchGrabber*){if (bSel) menuClickBgItem(m_familyID);}
		);

		auto btDetail = LayerButton::create(false, true);
		bgItem->addChild(btDetail);
		//m_pScrollView->addChild(bgItem);
		//俱乐部头像
		auto pTouXiang = CreateUserHeadSprite("family/defaulticon.png", "family/iconclipmask.png", bgItem, Vec2(10,8), "family/gf_family_icon.png");
		//俱乐部昵称
		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),165,47,Color4B::WHITE,bgItem,0);
		//加入按钮
		auto JiaRuBg = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86),false,true);
		auto m_labelStr = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"加入",Vec2(0.5,0.5),0,0,Color4B::WHITE,JiaRuBg,0);
		JiaRuBg->setPosition(611,53);
		bgItem->addChild(JiaRuBg);

		
		auto RoleCount = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0.5,0.5),413,47,Color4B(0x79,0xdc,0x98,0xff),bgItem,0);
		auto btSpriteBgRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 96, bgItem, 3);
		btSpriteBgRank->SetFrameIdx(3);
		auto labelRandid = CreateLabelMSYHAndAnchorPosClrAddTo(16,"1",Vec2(0.5,0.5),20,94,Color4B(0xff,0xd2,0x00,0xff),bgItem,5);


		auto pItem = new FamilywListItem;
		pItem->m_pSpriteBgItem = bgItem;
		pItem->m_pBtDetail = btDetail;
		pItem->m_pSpriteFamilyTouXiang = pTouXiang;
		pItem->m_familyName = labelName;
		pItem->m_labelRankIndex = labelRandid ;
		pItem->m_labelRoleCount = RoleCount;
		pItem->m_pBtJiaRu = JiaRuBg;
		pItem->m_labelStr = m_labelStr ;
		pItem->m_pBtJiaRu ->setVisible(false) ;
		pItem->m_spriteRank = btSpriteBgRank;
		item = pItem;

	}else{
		item=m_vecSelfClubFollowListInfoOld.back();
		m_vecSelfClubFollowListInfoOld.pop_back();
	}
	item->m_familyID = m_familyID;
	//加入回调
	item->m_pBtJiaRu->SetHitCB(std::bind(&GameClubHall::menuClickBgItem, this, m_familyID));
	//背景回调
	item->m_pBtDetail->setContentSize(Size(ListBgItemW-150,ListBgItemH));
	item->m_pBtDetail->SetHitCB(std::bind(&GameClubHall::menuClickBgItem, this,m_familyID));
	return item;
}

void GameClubHall::UpdateItem(GameClubHall::FamilywListItem *item, const FamilyInfo& followInfo)
{
	if (g_globalMyData.GetFamilySkin(followInfo.m_familyIconID).m_strIconUrl!="")
	{
		SetFamilyIconUrl(item->m_pSpriteFamilyTouXiang, g_globalMyData.GetFamilySkin( followInfo.m_familyIconID).m_strIconUrl);
	}
	else
	{
		item->m_pSpriteFamilyTouXiang->ResetSourceUrl("family/defaulticon.png"); 
	}


	SetLabelStringWithBoundWidth(item->m_familyName,followInfo.m_familyName.c_str(), 120);



	char s_buf[64] ;

	//人数
	sprintf(s_buf,"%d/%d",followInfo.m_curRoleCount,followInfo.m_maxRoleCount) ;
	SetLabelStringWithBoundWidth(item->m_labelRoleCount,s_buf, 120);

	sprintf(s_buf,"%d",(followInfo.m_familyRankID+1)) ;
	//SetLabelStringWithBoundWidth(item->m_labelRankIndex,s_buf, 120);


	if (followInfo.m_familyRankID<=2)
	{
		item->m_spriteRank->setPosition(20,96) ;
		item->m_spriteRank->SetFrameIdx(followInfo.m_familyRankID);
		SetLabelStringWithBoundWidth(item->m_labelRankIndex,"", 120);
	}
	else
	{

		item->m_spriteRank->setPosition(20,93) ;
		item->m_spriteRank->SetFrameIdx(3);
		SetLabelStringWithBoundWidth(item->m_labelRankIndex,s_buf, 120);
	}

	item->m_pBtDetail->setContentSize(Size(ListBgItemW-150,ListBgItemH));
	item->m_pBtJiaRu->setVisible(true);

	if (followInfo.m_hasJoin)
	{
		SetLabelString(item->m_labelStr,"进入") ;
		item->m_pBtJiaRu->SetHitCB(std::bind(&GameClubHall::menuClickBgItem, this, followInfo.m_familyID));
	}
	else
	{
		SetLabelString(item->m_labelStr,"加入") ;
		item->m_pBtJiaRu->SetHitCB(std::bind(&GameClubHall::menuClickJiaRu, this, followInfo.m_familyID));
	}

	
}

/************************************************************************/
/* 加入俱乐部                                                                     */
/************************************************************************/
void GameClubHall::menuClickJiaRu(unsigned ufamilyID)
{
	//g_globalMyData.m_iUserId

	//if (g_globalMyData.getJoinFamily())
	//{
	//	PopTipWithBt1("您已加入俱乐部!", "确定", nullptr);
	//	return ;
	//}
	Json::Value msg;
	msg["ufamilyid"] = ufamilyID;
	SendClientRequest(ClientRequest::kJoinFamily,msg);

	CCLOG("加入俱乐部") ;

}
/************************************************************************/
/* 显示俱乐部                                                                     */
/************************************************************************/
void GameClubHall::menuClickBgItem(unsigned m_familyID)
{


	auto pHall = dynamic_cast<GameHall*>(getParent());
	pHall->showSelfClub(true,m_familyID) ;

	//Json::Value msg;
	//msg["ufamilyid"] = m_familyID;
	//SendClientRequest(ClientRequest::kTargetFamilyInfo,msg);



}



/************************************************************************/
/* 数据更新                                                                     */
/************************************************************************/

//初始化
void GameClubHall::UpdateGameClubView()
{
	//log("update view %d", uType);
	for(auto item:m_vecFollowListInfoNew)
	{
		item->m_pSpriteBgItem->setVisible(false);
		m_vecFollowListInfoOld.push_back(item);
	}

	m_vecFollowListInfoNew.clear();
	unsigned uCount = g_globalFamilyInfo.m_vecFamilyInfo.size();
	unsigned ListBgItemH = 686;
	unsigned uItemH = 112;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2+10;
	float fY = fRealH - uItemH / 2 -10;




	for(auto& followList:g_globalFamilyInfo.m_vecFamilyInfo)
	{
		auto item = GetFamilyListItem(followList.m_familyID);
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







//初始化
void GameClubHall::UpdateSelfClubView()
{


	if (g_globaSelfJoinClub.m_vecSelfJoinClub.size() ==0)
	{
		m_layoutNoClub->setVisible(true) ;
	}
	else
	{
		m_layoutNoClub->setVisible(false) ;
	}


	//log("update view %d", uType);
	for(auto item:m_vecSelfClubFollowListInfoNew)
	{
		item->m_pSpriteBgItem->setVisible(false);
		m_vecSelfClubFollowListInfoOld.push_back(item);
	}

	m_vecSelfClubFollowListInfoNew.clear();
	unsigned uCount = g_globaSelfJoinClub.m_vecSelfJoinClub.size();
	unsigned ListBgItemH = 686;
	unsigned uItemH = 112;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2+10;
	float fY = fRealH - uItemH / 2 -10;


	for(auto& followList:g_globaSelfJoinClub.m_vecSelfJoinClub)
	{
		auto item = GetSelfClubFamilyListItem(followList.m_familyID);
		UpdateItem(item, followList);
		item->m_pSpriteBgItem->setPosition(fX, fY);
		item->m_pSpriteBgItem->setVisible(true); 
		m_vecSelfClubFollowListInfoNew.push_back(item);
		fY -= uItemH;     
	}
	float temp = fRealH - sizeListView.height;

	//每次都显示滚动框的最上面
	m_pSelfClubScrollView->setContentOffset(Vec2(0,-temp), false);
	m_pSelfClubScrollView->setContentSize(Size(sizeListView.width,fRealH));
	m_pSelfClubScrollView->setViewSize(sizeListView);
	setShowDataLoading(false) ;
}










void GameClubHall::setShowDataLoading(bool flag)
{
	if (s_dataLoading == nullptr)
	{

		s_dataLoading = Tools::creatDataLoading(this,514, 293,99) ;
	}
	s_dataLoading->setVisible(flag) ;

}