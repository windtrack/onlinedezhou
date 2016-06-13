//#include "GameFamily.h"
//#include "Tools.h"
//#include "MenuItemSpriteWithLabel.h"
//#include "GameCommon.h"
//#include "Util.h"
//#include "GameFrame.h"
//#include "GlobalUserData.h"
//
//#include "FrameSprite.h"
//#include "FrameSpriteButton.h"
//#include "FrameScale9SpriteButton.h"
//#include "HttpSprite.h"
//#include <vector>
//#include "Tools.h"
//#include "GrabTouch.h"
//
//#include "MyFamily.h"
//#include "GameHall.h"
//#include "ShowFamily.h"
//
//#include <sstream>
//#include "external/ConvertUTF/ConvertUTF.h"
//
//enum 
//{
//	Bt_Rank=0,
//	Bt_MyFamily,
//	Bt_FamilyInfo,
//	Bt_CreatFamily,
//};
//
//
//enum
//{
//	Layer_Rank=0,
//	Layer_MyFamily,
//	Layer_FamilyInfo,
//	Layer_CreatFamily,
//};
//
//
//
//enum BgItem
//{
//	ListBgItemW = 696,
//	ListBgItemH = 106
//};
//
//
//enum SrcollView	//	设置默认的滚动视图的大小
//{
//	ListScrollW = 686,
//	ListScrollH = 460
//};
//
//
//bool GameFamily::init()
//{
//	if(!GameDialog::init("dialog/bg.png",true))
//	{
//		return false;
//	}
//
//	s_dataLoading =nullptr ;
//	s_familyName = "";
//
//
//	Size sz = this->getContentSize() ;
//	s_uiroot = CSLoader::createNode("res/ui/gamefamily.csb");
//	s_uiroot->ignoreAnchorPointForPosition(false);
//	s_uiroot->setAnchorPoint(Point(0.5,0.5));
//	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
//	this->addChild(s_uiroot,2);
//
//
//
//	s_familybackgroundLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_gf_background"));
//	s_familybackgroundLayer->setVisible(true);
//
//
//
//
//	s_rankLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_rank"));
//	s_rankLayer->setVisible(false);
//
//	s_myFamilyLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_myfamily"));
//	s_myFamilyLayer->setVisible(false);
//
//
//
//
//
//
//	s_creatFamilyLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_creatfamily"));
//	s_creatFamilyLayer->setVisible(false);
//
//	s_familyManager[0]=s_rankLayer;
//	s_familyManager[1]=s_myFamilyLayer;
//
//	s_familyManager[3]=s_creatFamilyLayer;
//
//
//	m_layerTop = GameLayer::create(679, 143, false, false);
//	m_layerTop->setPosition(Vec2(226, 497));
//
//	s_rankLayer->addChild(m_layerTop, 3);
//	m_layerTop->setColor(Color3B::WHITE) ;
//
//
//	m_layerBottom = GameLayer::create(sz.width, 5, false, false);
//	m_layerBottom->setPosition(Vec2(0, 0));
//	s_rankLayer->addChild(m_layerBottom, 3);
//
//
//
//	char* stirng_buttonfamilyMenu[4]= {"bt_rank", "bt_myfamily", "bt_familyinfo", "bt_creatfamily",} ;
//	char* stirng_buttonfamilyMenuText[4]= {"Text_rank", "Text_myfamily", "Text_myfamilyinfo", "Text_creatfamliy",} ;
//
//	for (int i=0; i<4; i++)
//	{
//		s_button_myFamilyMenu[i] = static_cast<Button*>(Tools::getWidgetByName(s_familybackgroundLayer,stirng_buttonfamilyMenu[i]));
//		s_button_myFamilyMenu[i]->setBright(true);
//		s_button_myFamilyMenu[i] ->addTouchEventListener(this,toucheventselector(GameFamily::onFamilyMenuItemClick));
//		s_button_myFamilyMenuText[i] = static_cast<Text*>(Tools::getWidgetByName(s_familybackgroundLayer,stirng_buttonfamilyMenuText[i]));
//	}
//
//
//
//
//
//	//创建俱乐部 --创建俱乐部
//	Node* panel_Count = Tools::getWidgetByName(s_creatFamilyLayer, "Sprite_creatfamily_block");
//	Size szCount = panel_Count->getContentSize();
//	s_editBoxCreatFamliy = CreateNode2AndPosAddTo<ui::EditBox>(Size(szCount.width, szCount.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szCount.width/2, szCount.height/2, panel_Count, 0);
//	s_editBoxCreatFamliy->setFont("fzlt.ttf", 24);
//	s_editBoxCreatFamliy->setFontColor(Color3B::BLACK);
//	s_editBoxCreatFamliy->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
//    s_editBoxCreatFamliy->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
//	s_editBoxCreatFamliy->setDelegate(this);	
//	s_editBoxFamilyName =  static_cast<Text*>(Tools::getWidgetByName(s_creatFamilyLayer,"Text_jiazuname"));
//
//
//	//我的俱乐部——创建俱乐部
//	buttomMyFamilyCreatFamily = static_cast<Button*>(Tools::getWidgetByName(s_myFamilyLayer,"Button_my_creatfamliy"));
//	buttomMyFamilyCreatFamily->addTouchEventListener(this,toucheventselector(GameFamily::onButtonCallBack));
//
//	//搜索俱乐部
//	Node* panel_search = Tools::getWidgetByName(s_rankLayer, "Button_seach");
//	Size ssz = panel_search->getContentSize();
//	s_editBoxSearch = CreateNode2AndPosAddTo<ui::EditBox>(Size(ssz.width, ssz.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , 
//		ssz.width/2, ssz.height/2, this, 5);
//	s_editBoxSearch->setPosition(panel_search->getPosition()) ;
//	s_editBoxSearch->setFont("fzlt.ttf", 24);
//	s_editBoxSearch->setFontColor(Color3B::BLACK);
//	s_editBoxSearch->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
//    s_editBoxSearch->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
//	s_editBoxSearch->setDelegate(this);	
//
//
//	//我的俱乐部——加入俱乐部
//	buttomMyFamilyJoinFamily = static_cast<Button*>(Tools::getWidgetByName(s_myFamilyLayer,"Button_my_joinfamily"));
//	buttomMyFamilyJoinFamily->addTouchEventListener(this,toucheventselector(GameFamily::onButtonCallBack));
//
//	//创建俱乐部
//	buttomCreatFamily = static_cast<Button*>(Tools::getWidgetByName(s_creatFamilyLayer,"Button_creatfamily_creat"));
//	buttomCreatFamily->addTouchEventListener(this,toucheventselector(GameFamily::onButtonCallBack));
//
//	//俱乐部滚动
//	Node* joinFamilyNode = Tools::getWidgetByName(s_rankLayer,"ListView_rank");
//	sizeListView.width = joinFamilyNode->getContentSize().width+10;
//	sizeListView.height = joinFamilyNode->getContentSize().height+10;
//
//
//	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,-10,4,joinFamilyNode,0);
//	m_pScrollView->setContentSize(sizeListView);
//	m_pScrollView->setBounceable(true);
//	m_pScrollView->setSwallowsTouches(false) ;
//	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
//	m_pScrollView->setVisible(true);
//
//
//	std::string starPath[4] = {"family/gf_family_levelbg_1.png","family/gf_family_levelbg_2.png","family/gf_family_levelbg_3.png","family/gf_family_levelbg_4.png",} ;
//	for (int i=0; i<4; i++)
//	{
//		spriteStar[i] = Sprite::create(starPath[i]) ;
//		this->addChild(spriteStar[i]) ;
//		spriteStar[i]->setVisible(false) ;
//
//	}
//
//
//	spriteFamilyTips = Sprite::create("family/familytips.png") ;
//	spriteFamilyTips->setPosition(s_button_myFamilyMenu[2]->getPosition()) ;
//
//
//
//	this->addChild(spriteFamilyTips) ;
//
//	callScaleBig( spriteFamilyTips) ;
//
//
//	s_cloudCreat =  static_cast<Layout*>(Tools::getWidgetByName(s_creatFamilyLayer,"Panel_creat"));
//	s_vipLow =  static_cast<Layout*>(Tools::getWidgetByName(s_creatFamilyLayer,"Panel_viplow"));
//	s_hasFamily =  static_cast<Layout*>(Tools::getWidgetByName(s_creatFamilyLayer,"Panel_hasfamily"));
//
//	
//	s_buttonShop = static_cast<Button*>(Tools::getWidgetByName(s_creatFamilyLayer,"Button_shop"));
//	s_buttonShop->addTouchEventListener(this,toucheventselector(GameFamily::onButtonCallBack));
//	s_buttonExit = static_cast<Button*>(Tools::getWidgetByName(s_creatFamilyLayer,"Button_hasfamily"));
//	s_buttonExit->addTouchEventListener(this,toucheventselector(GameFamily::onButtonCallBack));
//
//
//	g_globalMyData.WaitChange(WAITVIPCHANGE, std::bind(&GameFamily::updateBuyVip, this, std::placeholders::_1));
//
//	return true ;
//
//}
//void GameFamily::onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		if (pItem == s_buttonShop)
//		{
//			Json::Value param;
//			param["idx"] = 1;
//			SendClientRequest(ClientRequest::kPopMall, param);
//			return ;
//		}
//		if (pItem == s_buttonExit)
//		{
//
//			PopTipWithBt2("是否确定退出俱乐部?","确定","取消",[this](unsigned ubt)
//			{
//				if(ubt == 0)
//				{
//					Json::Value param;
//					param["user_id"] = g_globalMyData.m_iUserId;
//					param["group_id"] = g_globalMyData.getFamilyID();
//					SendClientRequest(ClientRequest::kPushOutFamily, param);
//				}
//				if(ubt == 1)
//				{
//
//				}
//			});
//
//			return ;
//		}
//		if (pItem == buttomMyFamilyCreatFamily)
//		{
//			showFamilyLayer(Bt_CreatFamily);
//			return ;
//		}
//		if (pItem == buttomMyFamilyJoinFamily)
//		{
//			showFamilyLayer(Bt_Rank);
//			return ;
//		}
//		if (pItem == buttomCreatFamily)
//		{
//		
//			//创建俱乐部
//			if (g_globalMyData.getHasCreatFamily())
//			{
//				PopTipWithBt1("您已加入俱乐部!", "确定", nullptr);
//				return ;
//			}
//			if (g_globalMyData.GetVipLevel()==5)
//			{
//				if (s_familyName!="")
//				{
//					Json::Value msg;
//					msg["fname"] = s_familyName;
//					SendClientRequest(ClientRequest::kCreatFamily,msg);	
//				}
//				else
//				{
//					PopTipWithBt1("请输入俱乐部名字!", "确定", nullptr);
//				}
//			}
//			else
//			{
//				PopTipWithBt2("您的VIP等级不足，是否购买vip！","取消","商城",[](unsigned ubt)
//				{
//					if(ubt == 1)
//					{
//						Json::Value param;
//						param["idx"] = 1;
//						SendClientRequest(ClientRequest::kPopMall, param);
//					}
//				});
//			}
//
//			return ;
//		}
//	}
//}
//
//void GameFamily::onFamilyMenuItemClick(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//	if (type == TouchEventType::TOUCH_EVENT_ENDED)
//	{
//
//
//
//		for (int i = 0; i < 4; i++)
//		{
//			if (pItem == s_button_myFamilyMenu[i])
//			{
//				showFamilyLayer(i);
//				setShowDataLoading(false) ;
//			}
//		}
//
//	}
//}
//
//void GameFamily::showFamilyLayer(int index)
//{
//	if (index == Layer_FamilyInfo)
//	{
//		auto pHall = dynamic_cast<GameHall*>(getParent());
//		pHall->showFamilyInfoTips(true) ;
//		return ;
//	}
//
//	m_curLayerIndex = index ;
//
//
//	if (index==Layer_MyFamily)
//	{
//		if (g_globalMyData.getHasCreatFamily())
//		{
//	
//			showMyFamily(true);
//		}
//
//	}
//	if (index==Bt_Rank)
//	{
//		SendClientRequest(ClientRequest::kGetAllFamilyList);
//		setShowDataLoading(true) ;
//	}
//
//	for (int i=0;i<4; i++)
//	{
//		if (i!=Bt_FamilyInfo)
//		{
//			s_button_myFamilyMenu[i]->setBright(i!=index) ;
//			s_button_myFamilyMenuText[i]->setTextColor(i!=index?Color4B(0x37,0xa4,0x5c,0xff):Color4B(0xff,0xff,0xff,0xff));
//			s_familyManager[i]->setVisible(i==index);
//		}
//
//	}
//
//
//	if (index == Bt_Rank)
//	{
//		s_editBoxSearch->setEnabled(true) ;
//	}
//	else
//	{
//		s_editBoxSearch->setEnabled(false) ;
//	}
//
//	if (index == Bt_CreatFamily)
//	{
//		updateCreatFamilyLayer();
//
//	}
//
//
//}
//
//
//
//void GameFamily::onMenuItemCloseSelected(cocos2d::Ref * ref)
//{
//	setVisible(false) ;
//	//if (s_creatFamilyLayer->isVisible())
//	//{
//	//	s_creatFamilyLayer->setVisible(false);
//	//	showMyFamily(false);
//	//	return ;
//	//}
//
//
//	//if (s_myFamilyInof->isVisible())
//	//{
//	//	showFamilyLayer(Bt_MyFamily) ;
//	//	return ;
//	//}
//	//else
//	//{
//	//	this->setVisible(false);
//	//}
//}
//
//void GameFamily::setVisible(bool bVisible)
//{
//	
//	PlayVisibleSound(bVisible);
//
//	if (bVisible)
//	{
//		initFamilyShow() ;
//		/*	auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
//		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
//		GameDialog::setVisible(true);
//		Tools::callScaleAction(this);
//	}
//	else
//	{
//
//		//if (m_curLayerIndex==Layer_FamilyInfo)
//		//{
//
//		//	showFamilyLayer(Layer_Rank) ;
//		//}
//		//else
//		//{
//		//	SendClientRequest(ClientRequest::kOptHallBgLayer);
//		//	auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
//		//	runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));
//		//}
//
//		SendClientRequest(ClientRequest::kOptHallBgLayer);
//		GameDialog::setVisible(false);
//		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
//		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
//		
//	}
//
//
//
//}
//
//void GameFamily::setOnVisible(bool bVisible)
//{
//	PlayVisibleSound(bVisible);
//	if (bVisible)
//	{
//		initFamilyShow() ;
//		GameDialog::setVisible(true);
//	}
//	else
//	{
//		SendClientRequest(ClientRequest::kOptHallBgLayer);
//		GameDialog::setVisible(false);
//	}
//}
//
//
//void GameFamily::initFamilyShow()
//{
//
//		showFamilyLayer(Bt_Rank) ;
//
//		s_button_myFamilyMenu[Bt_CreatFamily]->setVisible(g_globalMyData.getFamilyPower()!=2);
//
//}
//
//void GameFamily::editBoxReturn(cocos2d::ui::EditBox* editBox)
//{
//	if (editBox == s_editBoxCreatFamliy)
//	{
//		int a =0 ;
//	}
//}
//
//void GameFamily::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
//{
//	if (editBox == s_editBoxCreatFamliy)
//	{
//		//s_editBoxFamilyName->setText("");
//	}
//}
//
//void GameFamily::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
//{
//	if (editBox == s_editBoxCreatFamliy)
//	{
//
//
//
//		std::string name = editBox->getText();
//
//		auto pStr = name.c_str();
//		const UTF8*pUtf8Str = (const UTF8*)pStr;
//		const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
//
//		unsigned uL = getUTF8StringLength(pUtf8Str);
//
//		if (uL>4)
//		{
//			s_editBoxFamilyName->setText("");
//			editBox->setText("") ;
//			s_familyName = "" ;
//			PopTipWithBt1("最多输入四个汉字","确定",nullptr) ;
//
//			
//		}
//		else
//		{
//		
//			bool isChinese = true ;
//			int nlen = uL; 
//			for(int i=0; i < nlen; i++) 
//			{ 
//				if( pStr[i] >= 0 && pStr[i] <= 127 ) //不是全角字符？
//				{
//		
//					isChinese = false ;
//				}
//				
//				else //是全角字符
//				{
//					 i++; //中文是2个字节,所以i++
//		
//				}
//						
//			} 
//
//	/*		if (!checkTextChineseOrNumberOrLetter(name))
//			{
//				editBox->setText("") ;
//				PopTipWithBt1("只能输入中文","确定",nullptr);
//				return ;
//			}*/
//
//			if (!isChinese)
//			{
//				editBox->setText("") ;
//				PopTipWithBt1("只能输入中文","确定",nullptr);
//				return ;
//			}
//
//
//
//
//
//			s_editBoxFamilyName->setText(name);
//			editBox->setText("") ;
//			s_familyName = name ;
//		}
//		//s_editBoxFamilyName->setText(name);
//		//editBox->setText("") ;
//		//s_familyName = name ;
//
//	}
//
//	if (editBox == s_editBoxSearch)
//	{
//
//		if (!s_rankLayer->isVisible())
//		{
//			return ;
//		}
//
//		std::string name = editBox->getText();
//		//s_editBoxFamilyName->setText(name);
//		s_editBoxSearch->setText("") ;
//		//s_familyName = name ;
//
//		if (name!="")
//		{
//
//			Json::Value param;
//			param["name"] = name;
//			SendClientRequest(ClientRequest::kSearchFamily, param);
//
//		}
//
//	}
//}
//
//void GameFamily::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
//{
//	if (editBox == s_editBoxCreatFamliy)
//	{
//		int a =0 ;
//	}
//}
//
//
//
//
///************************************************************************/
///* 数据更新                                                                     */
///************************************************************************/
//
////初始化
//void GameFamily::UpdateView()
//{
//	//log("update view %d", uType);
//	for(auto item:m_vecFollowListInfoNew)
//	{
//		item->m_pSpriteBgItem->setVisible(false);
//		m_vecFollowListInfoOld.push_back(item);
//	}
//
//
//
//
//
//	m_vecFollowListInfoNew.clear();
//	unsigned uCount = g_globalFamilyInfo.m_vecFamilyInfo.size();
//	unsigned ListBgItemH = 686;
//	unsigned uItemH = 112;
//	unsigned fRealH = uCount * uItemH;
//	if(fRealH < sizeListView.height)
//	{
//		fRealH = sizeListView.height;
//	}
//	float fX = sizeListView.width / 2;
//	float fY = fRealH - uItemH / 2 - 10;
//
//
//
//
//	for(auto& followList:g_globalFamilyInfo.m_vecFamilyInfo)
//	{
//		auto item = GetFamilyListItem(followList.m_familyID);
//		UpdateItem(item, followList);
//		item->m_pSpriteBgItem->setPosition(fX, fY);
//		item->m_pSpriteBgItem->setVisible(true); 
//		m_vecFollowListInfoNew.push_back(item);
//		fY -= uItemH;     
//	}
//	float temp = fRealH - sizeListView.height;
//
//	//每次都显示滚动框的最上面
//	m_pScrollView->setContentOffset(Vec2(0,-temp), false);
//	m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
//
//	setShowDataLoading(false) ;
//}
//
//
//GameFamily::FamilywListItem *GameFamily::GetFamilyListItem(unsigned m_familyID)
//{
//	FamilywListItem *item = nullptr;
//	if(m_vecFollowListInfoOld.empty())
//	{
//		auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("family/fanlilyblock.png", 1, 2, sizeListView.width / 2, 0, m_pScrollView, 0);
//		auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
//		touchGrabber->retain();
//		touchGrabber->GetTouchProcer().SetHitCB(
//			[bgItem](bool bSel, Node*, TouchGrabber*){
//				bgItem->SetFrameIdx(bSel?1:0);
//		}
//			//[this,m_familyID](bool bSel, Node*, TouchGrabber*){if (bSel) menuClickBgItem(m_familyID);}
//		
//		);
//
//		m_pTouchGrabberBG = touchGrabber;
//
//		//auto bgItem = Scale9Sprite ::create(Rect(0,0,ListBgItemW,ListBgItemH),"family/gf_family_item_2.png");
//		////bgItem->setContentSize(Size(ListBgItemW,ListBgItemH));
//		//bgItem->setPosition(sizeListView.width / 2, 0);
//		auto btDetail = LayerButton::create(false, true);
//		bgItem->addChild(btDetail);
//		//m_pScrollView->addChild(bgItem);
//		//俱乐部头像
//		auto pTouXiang = CreateUserHeadSprite("family/defaulticon.png", "family/iconclipmask.png", bgItem, Vec2(10,8), "family/gf_family_icon.png");
//		//俱乐部昵称
//		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),165,76,Color4B::WHITE,bgItem,0);
//
//		//加入按钮
//		auto JiaRuBg = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86),false,true);
//		auto labeljiaru = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"加入",Vec2(0.5,0.5),0,0,Color4B::WHITE,JiaRuBg,0);
//		JiaRuBg->setPosition(611,53);
//
//		bgItem->addChild(JiaRuBg);
//
//		auto levelbg = Sprite::create("family/familyLevelbg.png");
//		levelbg->ignoreAnchorPointForPosition(false);
//		levelbg->setAnchorPoint(Point(0.5,0.5));
//		levelbg->setPosition(160,36);
//		bgItem->addChild(levelbg,1);
//		//俱乐部等级
//		auto labellevel = CreateLabelMSYHAndAnchorPosClrAddTo(16,"",Vec2(0.5,0.5),160,16,Color4B(0xff,0xd2,0x00,0xff),bgItem,2);
//	
//		
//	
//		auto RoleCount = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0.5,0.5),413,68,Color4B(0x79,0xdc,0x98,0xff),bgItem,0);
//	
//		auto btSpriteBgRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 96, bgItem, 3);
//	
//		btSpriteBgRank->SetFrameIdx(3);
//
//		auto labelRandid = CreateLabelMSYHAndAnchorPosClrAddTo(16,"1",Vec2(0.5,0.5),20,94,Color4B(0xff,0xd2,0x00,0xff),bgItem,5);
//		
//
//
//		auto pItem = new FamilywListItem;
//		pItem->m_pSpriteBgItem = bgItem;
//		pItem->m_pBtDetail = btDetail;
//		pItem->m_pSpriteFamilyTouXiang = pTouXiang;
//		pItem->m_familyName = labelName;
//		pItem->m_pSpriteStar = levelbg;
//		pItem->m_labellevel = labellevel;
//		pItem->m_labelRankIndex = labelRandid ;
//		pItem->m_labelRoleCount = RoleCount;
//		pItem->m_pBtJiaRu = JiaRuBg;
//		pItem->m_spriteRank = btSpriteBgRank;
//		item = pItem;
//
//		
//	}else{
//		item=m_vecFollowListInfoOld.back();
//		m_vecFollowListInfoOld.pop_back();
//	}
//	item->m_familyID = m_familyID;
//	//加入回调
//	item->m_pBtJiaRu->SetHitCB(std::bind(&GameFamily::menuClickJiaRu, this, m_familyID));
//	//背景回调
//	item->m_pBtDetail->setContentSize(Size(ListBgItemW-150,ListBgItemH));
//	item->m_pBtDetail->SetHitCB(std::bind(&GameFamily::menuClickBgItem, this,m_familyID));
//	return item;
//}
//
//void GameFamily::UpdateItem(GameFamily::FamilywListItem *item, const FamilyInfo& followInfo)
//{
//	if (g_globalMyData.GetFamilySkin(followInfo.m_familyIconID).m_strIconUrl!="")
//	{
//		SetFamilyIconUrl(item->m_pSpriteFamilyTouXiang, g_globalMyData.GetFamilySkin( followInfo.m_familyIconID).m_strIconUrl);
//	}
//	else
//	{
//		item->m_pSpriteFamilyTouXiang->ResetSourceUrl("family/defaulticon.png"); 
//	}
//
//	
//	SetLabelStringWithBoundWidth(item->m_familyName,followInfo.m_familyName.c_str(), 120);
//	
//	//等级
//	int s_level = followInfo.m_familylevel;
//	
//	char s_buf[64] ;
//	sprintf(s_buf,"LV%d",s_level) ;
//	
//	if (s_level<=8)
//	{
//		item->m_labellevel->setTextColor(Color4B(0xff,0xff,0xff,0xff)) ;
//	}
//	else
//	{
//		item->m_labellevel->setTextColor(Color4B(0xff,0xd2,0x00,0xff)) ;
//	}
//	if (s_level<=2)
//	{
//		item->m_pSpriteStar->setTexture(spriteStar[0]->getTexture()) ;
//	}
//
//	if (s_level>2&&s_level<=5)
//	{
//		item->m_pSpriteStar->setTexture(spriteStar[1]->getTexture()) ;
//	}
//	if (s_level>=6&&s_level<=8)
//	{
//		item->m_pSpriteStar->setTexture(spriteStar[2]->getTexture()) ;
//	}
//	if (s_level>=8&&s_level<=10)
//	{
//		item->m_pSpriteStar->setTexture(spriteStar[3]->getTexture()) ;
//	}
//	//item->m_pSpriteStar->setTexture()
//
//	SetLabelStringWithBoundWidth(item->m_labellevel,s_buf, 120);
//
//	//人数
//	sprintf(s_buf,"%d/%d",followInfo.m_curRoleCount,followInfo.m_maxRoleCount) ;
//	SetLabelStringWithBoundWidth(item->m_labelRoleCount,s_buf, 120);
//
//	sprintf(s_buf,"%d",(followInfo.m_familyRankID+1)) ;
//	//SetLabelStringWithBoundWidth(item->m_labelRankIndex,s_buf, 120);
//
//
//	if (followInfo.m_familyRankID<=2)
//	{
//		item->m_spriteRank->setPosition(20,96) ;
//		item->m_spriteRank->SetFrameIdx(followInfo.m_familyRankID);
//		SetLabelStringWithBoundWidth(item->m_labelRankIndex,"", 120);
//		//item->m_labelRankIndex->setVisible(false);
//	}
//	else
//	{
//	
//		item->m_spriteRank->setPosition(20,93) ;
//		item->m_spriteRank->SetFrameIdx(3);
//		SetLabelStringWithBoundWidth(item->m_labelRankIndex,s_buf, 120);
//	}
//
//	item->m_pBtDetail->setContentSize(Size(ListBgItemW-150,ListBgItemH));
//	item->m_pBtJiaRu->setVisible(true);
//
//
//	item->m_pBtDetail->SetHitCB(std::bind(&GameFamily::menuClickBgItem, this, followInfo.m_familyID));
//	item->m_pBtJiaRu->SetHitCB(std::bind(&GameFamily::menuClickJiaRu, this, followInfo.m_familyID));
//}
//
//
//
////改变对应的记录
//void GameFamily::UpdateItemView( const FamilyInfo& userInfo)
//{
//	for(auto item:m_vecFollowListInfoNew)
//	{
//		if(item->m_familyID == userInfo.m_familyID)
//		{
//			UpdateItem(item, userInfo);
//		}
//	}
//}
//
////追加记录
//void GameFamily::AppadnItemView(const FamilyInfo& userInfo)
//{
//	auto item = GetFamilyListItem(userInfo.m_familyID);
//	m_vecFollowListInfoNew.push_back(item);
//	item->m_pSpriteBgItem->setVisible(true);
//	UpdateItem(item, userInfo);
//	unsigned uItemH = 106;
//	auto sz = m_pScrollView->getContentSize();
//	float fRealH = m_vecFollowListInfoNew.size() * uItemH;
//	if(fRealH < sizeListView.height){
//		m_pScrollView->setContentSize(sizeListView);
//		m_pScrollView->setContentOffset(Vec2(0,0));
//		float fX = sizeListView.width / 2,fY = sizeListView.height - fRealH;
//		for(auto followList:m_vecFollowListInfoNew)
//		{
//			followList->m_pSpriteBgItem->setPosition(fX+5, fY + ListBgItemH / 2);
//			fY += uItemH;     
//		}
//	}else{
//		m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
//		sz = m_pScrollView->getContentSize();
//		float fY = 5 + ListBgItemH / 2;
//		for(auto followList:m_vecFollowListInfoNew)
//		{
//			followList->m_pSpriteBgItem->setPositionY(fY);
//			fY += uItemH;     
//		}
//		float temp = fRealH - sizeListView.height;
//		m_pScrollView->setContentOffset(Vec2(0,-temp),false);
//	}
//}
//
////删除记录
//void GameFamily:: RemoveItemView( unsigned uUserID)
//{
//	for(int i=0; i<m_vecFollowListInfoNew.size(); i++)
//	{
//		if(m_vecFollowListInfoNew[i]->m_familyID == uUserID)
//		{
//			unsigned uItemH = 106;
//			m_vecFollowListInfoNew[i]->m_pSpriteBgItem->setVisible(false);
//			m_vecFollowListInfoNew.erase(m_vecFollowListInfoNew.begin() + i);
//			auto sz = m_pScrollView->getContentSize();
//			float fRealH = m_vecFollowListInfoNew.size() * uItemH;
//			if(fRealH < sizeListView.height){
//				m_pScrollView->setContentSize(sizeListView);
//				m_pScrollView->setContentOffset(Vec2(0,0));
//				float fX = sizeListView.width / 2,fY = sizeListView.height - fRealH;
//				for(auto followList:m_vecFollowListInfoNew)
//				{
//					followList->m_pSpriteBgItem->setPosition(fX, fY + ListBgItemH / 2);
//					fY += uItemH;     
//				}
//				break;
//			}else{
//				float fX = sizeListView.width / 2,fY = 0;
//				m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
//				for(auto followList:m_vecFollowListInfoNew)
//				{
//					followList->m_pSpriteBgItem->setPosition(fX, fY + ListBgItemH / 2);
//					fY += uItemH;     
//				}
//				float temp = fRealH - sizeListView.height;
//				m_pScrollView->setContentOffset(Vec2(0,-temp),false);
//				break;
//			}
//		}
//	}
//}
//
//
//
///************************************************************************/
///* 加入俱乐部                                                                     */
///************************************************************************/
//void GameFamily::menuClickJiaRu(unsigned ufamilyID)
//{
//	//g_globalMyData.m_iUserId
//
//	if (g_globalMyData.getHasCreatFamily())
//	{
//		PopTipWithBt1("您已加入俱乐部!", "确定", nullptr);
//		return ;
//	}
//	Json::Value msg;
//	msg["ufamilyid"] = ufamilyID;
//	SendClientRequest(ClientRequest::kJoinFamily,msg);
//
//	CCLOG("加入俱乐部") ;
//
//}
///************************************************************************/
///* 显示俱乐部                                                                     */
///************************************************************************/
//void GameFamily::menuClickBgItem(unsigned m_familyID)
//{
//
//
//	auto pHall = dynamic_cast<GameHall*>(getParent());
//	pHall->showTargetFamily(false,true) ;
//
//	Json::Value msg;
//	msg["ufamilyid"] = m_familyID;
//	SendClientRequest(ClientRequest::kTargetFamilyInfo,msg);
//
//
//}
///************************************************************************/
///* 创建俱乐部                                                                     */
///************************************************************************/
//void GameFamily::menuClickCeatFamily(std::string name)
//{
//	//g_globalMyData.m_iUserId
//
//
//}
//
//
//
//
//
//void GameFamily::showMyFamily(bool flag)
//{
//
//
//	if (!flag)
//	{
//		showFamilyLayer(Bt_Rank);
//	}
//	else
//	{
//		auto pHall = dynamic_cast<GameHall*>(getParent());
//		pHall->showMyFamily(false,true);
//		pHall->showGameFamily(false,false) ;
//	}
//
//
//
//
//}
//
//
//
//
//void GameFamily::updateRankList()
//{
//	UpdateView();
//	//showFamilyLayer(Bt_Rank);
//}
//
//void GameFamily::updateMyFamilyData()
//{
//	//m_myFamily->updateFamilyInfo();
//}
//void GameFamily::updateTargetFamilyData()
//{
//	//m_targetFamily->updateFamilyInfo();
//}
//
//void GameFamily::setCloseButtonShow(bool flag)
//{
//	//m_btMenuItemClose->setVisible(flag);
//}
//
//void GameFamily::updateMyFamilyRoleRankList()
//{
//	//m_myFamily->updateRoleRankList();
//}
//
//
//bool  GameFamily::checkTextChineseOrNumberOrLetter(std::string str)
//{
//
//
//
//
//	//bool ConvertUTF8toWide(unsigned WideCharWidth, const std::string& Source,
//	//	char *&ResultPtr, const UTF8 *&ErrorPtr) 
//
//	//int badNum = 0;  
//
//	//int size = str.length();  
//	//if (size <= 0)  
//	//	return false;  
//
//	//const char* pStr = str.c_str();  
//
//	//////strcpy(pStr, str.c_str());  
//
//
//	/*char* pName =new char[1024];
//	const UTF8 *ErrorPtr ;*/
//	//llvm::ConvertUTF8toWide(2,str,pName,ErrorPtr) ;
//
//	//while(*pName)
//	//{
//	//	if((*pName)&0x80){
//
//	//		pName++;//知道是汉字的话跳过一个字节检测
//	//	}
//	//	else
//	//	{
//	//		return false ;
//	//	}
//
//	//	pName++;
//	//}
//
//
//
//
//
//
//
//	//auto pStr = str.c_str();
//	//const UTF8*pUtf8Str = (const UTF8*)pStr;
//	//const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
//
//	//unsigned uL = getUTF8StringLength(pUtf8Str);
//
//
//	//int aa =0 ;
//	//for (int i = 0; i < uL; i++)  
//	//{  
//
//
//	//	// 判断下是不是中文，这里根据Unicode汉字编码表返回进行判断，这里要根据系统的内存布局来调整  
//	//	unsigned char* pCh = (unsigned char*)&pStr[i];  
//	//	if (((*pCh >= 0) && (*pCh <= 0xff)) && (*(pCh + 1) >= 0x4e && *(pCh + 1) <= 0x95))  
//	//		continue;  
//	//	else  
//	//	{
//	//		//return false ;
//	//		aa++ ;
//	//	}
//	//}  
//
//
//
//
//
//	return true; 
//}
//
//
//void GameFamily::callScaleSmall(cocos2d::Node* node)
//{
//	ActionInterval* scaleTo = ScaleTo::create(0.5,1.0) ;
//	node->runAction(Sequence::create(scaleTo, CallFunc::create([this,node](){callScaleBig(node);}), nullptr)) ;
//}
//
//void GameFamily::callScaleBig(cocos2d::Node* node)
//{
//	ActionInterval* scaleTo = ScaleTo::create(0.5,1.2) ;
//	node->runAction(Sequence::create(scaleTo, CallFunc::create([this,node](){callScaleSmall(node);}), nullptr)) ;
//}
//
//void GameFamily::updateBuyVip(unsigned uDataType)
//{
//	if (uDataType == WAITVIPCHANGE)
//	{
//		updateCreatFamilyLayer() ;
//	}
//}
//
//void GameFamily::updateCreatFamilyLayer()
//{
//	if (g_globalMyData.m_JoinFamily)
//	{
//		s_vipLow->setVisible(false) ;
//		s_cloudCreat->setVisible(false) ;
//		s_hasFamily->setVisible(true) ;
//		return ;
//	}
//
//	if (g_globalMyData.GetVipLevel()<5)
//	{
//		s_vipLow->setVisible(true) ;
//		s_cloudCreat->setVisible(false) ;
//		s_hasFamily->setVisible(false) ;
//		return ;
//	}
//
//	s_vipLow->setVisible(false) ;
//	s_cloudCreat->setVisible(true) ;
//	s_hasFamily->setVisible(false) ;
//}
//
//
//void GameFamily::setShowDataLoading(bool flag)
//{
//	if (s_dataLoading == nullptr)
//	{
//
//		s_dataLoading = Tools::creatDataLoading(this,514, 293,99) ;
//	}
//	s_dataLoading->setVisible(flag) ;
//
//}