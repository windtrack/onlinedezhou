//#include "MyFamily.h"
//#include "Tools.h"
//#include "MenuItemSpriteWithLabel.h"
//#include "GameCommon.h"
//#include "Util.h"
//#include "GameFrame.h"
//#include "GlobalUserData.h"
//#include "GameData.h"
//#include "FrameSprite.h"
//#include "FrameSpriteButton.h"
//#include "HttpSprite.h"
//#include <vector>
//#include "Tools.h"
//#include "GrabTouch.h"
//#include "GameFamily.h"
//#include "ContributionRankList.h"
//#include "ChooseFamilyIcon.h"
//#include "GameHall.h"
//#include "EditFamilyRole.h"
//#include "LogonLayerEx.h"
//#include <sstream>
//#include "external/ConvertUTF/ConvertUTF.h"
//MyFamily* MyFamily::creatWithLayer(GameFamily* layer)
//{
//	MyFamily* sFamliy = new MyFamily() ;
//	if (sFamliy && sFamliy->initWithLayer(layer))
//	{
//		sFamliy->autorelease();
//		return sFamliy ;
//	}
//	else
//	{
//		CC_SAFE_DELETE(sFamliy);
//		return nullptr;
//	}
//}
//
//bool MyFamily::initWithLayer(GameFamily* layer)
//{
//	if (!Layer::init())
//	{
//		return false;
//	}
//
//
//	//updateTargetFamily() ;
//
//	return true ;
//}
//
//bool MyFamily::init()
//{
//	if(!GameDialog::init("dialog/bg.png",true))
//	{
//		return false;
//	}
//
//	initLayer();
//
//	return true ;
//}
//
//
//void MyFamily::initLayer()
//{
//	//REG_TOUCH_ONEBYONE_SWALLOW_B(MyFamily::onTouchBegan, this);
//	s_dataLoading = nullptr ;
//	s_ChooseIconLayer = nullptr ;
//	s_ContributionLayer = nullptr ;
//
//	Size sz = this->getContentSize() ;
//	s_uiroot = CSLoader::createNode("res/ui/myfamily.csb");
//	s_uiroot->ignoreAnchorPointForPosition(false);
//	s_uiroot->setAnchorPoint(Point(0.5,0.5));
//	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
//
//	//rootNode->setScale(0.5);
//	this->addChild(s_uiroot,1);
//
//	s_logicRoot = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_myfarmily")) ;
//
//	//Node* headicon = Tools::getWidgetByName(s_uiroot,"Panel_familyheadicon") ;
//	//headicon->setVisible(false) ;
//	s_familyName = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_title")) ;
//	s_familyName->setText("dfsdf");
//	s_fangilyID = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_id")) ;
//	s_curLevel = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_curfamlevel")) ;
//	s_nextLevel = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_nextfamlevel")) ;
//	s_curFanli = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_realfanli")) ;
//	s_netFanLi = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_xiajifanlibi")) ;
//	s_day = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_day")) ;
//	s_hours = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_hourb")) ;
//	s_hoursf = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_houf")) ;
//
//	s_familyGongGao = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_jiazugonggao")) ;
//	s_curRole = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_chenyuanshuliang")) ;
//
//
//	s_TextGetFuli = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_lingquwuli")) ;
//
//	s_buttonGetFuli = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_lingqufuli")) ;
//	s_buttonGetFuli->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//	s_buttonTips = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_fulihelp")) ;
//	s_buttonTips->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//	s_button_exit = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_back")) ;
//	s_button_exit->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//	s_buttonInvite = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_yaoqing")) ;
//	s_buttonInvite->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//	s_buttonEdit = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_editgonggao")) ;
//	s_buttonEdit->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//
//	s_button_GameFamily = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_gamefamily")) ;
//	s_button_GameFamily->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//	
//
//
//	s_buttonIconEdit =  static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_icon")) ;
//
//
//	s_buttonIconEdit->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//	buttonRank =  static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_rank")) ;
//	buttonRank->addTouchEventListener(this,toucheventselector(MyFamily::onButtonCallBack));
//
//	s_levelPercent =  static_cast<LoadingBar*>(Tools::getWidgetByName(s_uiroot,"LoadingBar_family")) ;
//	s_textLevelPercent = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,"Text_percent")) ;
//
//
//	//没有成员的其实
//	s_nothingRoleTips =  static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Image_noroletips")) ;
//	s_nothingRoleTips->setVisible(false);
//	//成员显示
//	s_RoleShow =  static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_roleshow")) ;
//	s_nothingRoleTips->setVisible(false);
//	//时间
//	s_TimeLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_xiacilingqutime")) ;
//
//
//
//	s_nodeFanLiTips = Tools::getWidgetByName(s_uiroot,"Sprite_fanliblock");
//	s_nodeFanLiTips->setVisible(false);
//
//	s_textFanliTips = static_cast<Text*>(Tools::getWidgetByName(s_nodeFanLiTips,"Text_fanlitips")) ;
//
//
//	s_buttonEditGongGao= static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Button_gonggao")) ;
//	//Node* panel_Count = Tools::getWidgetByName(s_creatFamilyLayer, "Sprite_creatfamily_block");
//	Size szCount = s_buttonEditGongGao->getContentSize();
//	s_editBoxGongGao = CreateNode2AndPosAddTo<ui::EditBox>(Size(szCount.width, szCount.height), ui::Scale9Sprite::create("res/ui/bgedit.png") , szCount.width/2, szCount.height/2, s_buttonEditGongGao, 0);
//	s_editBoxGongGao->setFont("fzlt.ttf", 24);
//	s_editBoxGongGao->setFontColor(Color3B::BLACK);
//	s_editBoxGongGao->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
//    s_editBoxGongGao->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
//	s_editBoxGongGao->setDelegate(this);	
//	/*s_editBoxFamilyName =  static_cast<Text*>(Tools::getWidgetByName(s_creatFamilyLayer,"Text_jiazuname"));*/
//
//
//	//icon
//
//
//
//	// 头像
//	auto pClipMask = Sprite::create("family/defaulticon.png");
//	pClipMask->setPosition(s_buttonIconEdit->getContentSize().width/2,s_buttonIconEdit->getContentSize().height/2);
//	pClipMask->retain();
//	cocos2d::ClippingNode *m_pClipTouXiang = CreateNodeAndPosAddTo<ClippingNode>(0,0, s_buttonIconEdit, 0);
//	m_pClipTouXiang->setContentSize(Size(195, 195));
//	s_FamilyIcon = CreateNode2AndPosAddTo<HttpSprite>("family/defaulticon.png", Size(188, 188), 0, 0, m_pClipTouXiang, 0);
//	s_FamilyIcon->ignoreAnchorPointForPosition(false);
//	s_FamilyIcon->setAnchorPoint(Point(0.5,0.5));
//	s_FamilyIcon->setPosition(s_buttonIconEdit->getContentSize().width/2,s_buttonIconEdit->getContentSize().height/2);
//
//	m_pClipTouXiang->setStencil(pClipMask);
//	m_pClipTouXiang->setAlphaThreshold(0.5);
//
//
//	g_globalMyFamilyInfo.WaitChange( std::bind(&MyFamily::updateIcon, this));
//
//
//	m_layerTipsEditIcon = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_editicon")) ;
//
//	m_LayerAdmin = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_zuzhang"));
//	//管理人员列表
//
//	char* c_zuzhangbutton[3] = {"Button_zuzhang","Button_fuzuzhang0","Button_fuzuzhang1"} ;
//	char* c_zuzhangbuttonbg[3] = {"Button_zuzhangbg","Button_fuzuzhangbg0","Button_fuzuzhangbg1"} ;
//	char* c_zuzhangbuttonbgnull[3] = {"Button_zuzhangbg_0","Button_fuzuzhangbg0_0","Button_fuzuzhangbg1_0"} ;
//	for (int i=0; i<3 ;i++)
//	{
//		m_buttonAdmin[i] = static_cast<Button*>(Tools::getWidgetByName(m_LayerAdmin,c_zuzhangbutton[i]));
//		m_buttonAdmin[i]->setVisible(true) ;
//		m_buttonAdmin[i]->addTouchEventListener(this,toucheventselector(MyFamily::callAdminCallBack)) ;
//		m_buttonAdminBG[i] =  static_cast<Button*>(Tools::getWidgetByName(m_LayerAdmin,c_zuzhangbuttonbg[i]));
//		m_buttonAdminBG[i]->setEnabled(false);
//
//		m_buttonAdminBGNull[i] =static_cast<Button*>(Tools::getWidgetByName(m_LayerAdmin,c_zuzhangbuttonbgnull[i]));
//		m_buttonAdminBGNull[i]->setEnabled(false);
//		if (i==0)
//		{
//
//			m_spriteAdmin[i] =  CreateUserHeadSprite("defaulticon.png", "family/iconclipmaskbig98.png", m_buttonAdminBG[i], Vec2(0,0), "hall/iconround.png");
//		}
//		else
//		{  
//			m_spriteAdmin[i] =  CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", m_buttonAdminBG[i], Vec2(0,0), "family/gf_family_icon.png");
//		}
//
//	}
//
//	char* c_rankbutton[5] = {"sprite_role0","sprite_role0_0","sprite_role0_1","sprite_role0_2","sprite_role0_3"} ;
//	for (int i=0; i<5 ;i++)
//	{
//		m_imgRankList[i] = static_cast<ImageView*>(Tools::getWidgetByName(s_RoleShow,c_rankbutton[i]));
//		m_spriteRankList[i] = CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", m_imgRankList[i], Vec2(0,0), "family/gf_family_icon.png");
//	}
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
//	ImageView* imgCurLevel = static_cast<ImageView*>(Tools::getWidgetByName(s_uiroot,"Image_srar0")) ;
//
//	ImageView* imgNextLevel = static_cast<ImageView*>(Tools::getWidgetByName(s_uiroot,"Image_srar1")) ;
//
//
//	m_sprite_curLevel = Sprite::create("family/gf_family_levelbg_1.png");
//	m_sprite_nextLevel = Sprite::create("family/gf_family_levelbg_0.png");
//
//	imgCurLevel->setAnchorPoint(Point(0,0)) ;
//	imgCurLevel->addChild(m_sprite_curLevel);
//	imgNextLevel->setAnchorPoint(Point(0,0)) ;
//	imgNextLevel->addChild(m_sprite_nextLevel);
//	//等级显示
//
//	
//
//
//}
//
//void MyFamily::onMenuItemCloseSelected(cocos2d::Ref * ref)
//{
//			setVisible(false) ;
//		//GameDialog::setVisible(false);
//		//s_pLayer->showMyFamily(false) ;
//}
//
//void MyFamily::updateFamilyInfo( )
//{
//
//	//s_nodeFanLiTips->setVisible(false);
//	//char buff[64];
//
//	//updateIcon();
//	////SetIconUrl(s_FamilyIcon, g_globalMyData.GetSkin(g_globalMyFamilyInfo.getFamilyIconUrl()).m_strIconUrl);
//
//	////sprintf(buff,"%s",g_globalMyFamilyInfo.m_familyName) ;
//	//setUITextString(s_familyName,g_globalMyFamilyInfo.m_familyName.c_str());
//	//sprintf(buff,"ID:%d",g_globalMyFamilyInfo.getFamilyID()) ;
//	//s_fangilyID->setText(buff);
//	//setUITextString(s_fangilyID,buff);
//
//
//
//	//int curLevel = g_globalMyFamilyInfo.getFamilyLevel();
//	//
//
//	//int nextLevel =  curLevel+1;
//
//
//	//if (curLevel<=2)
//	//{
//	//	m_sprite_curLevel->setTexture(spriteStar[0]->getTexture()) ;
//	//}
//
//	//if (curLevel>2&&curLevel<=5)
//	//{
//	//	m_sprite_curLevel->setTexture(spriteStar[1]->getTexture()) ;
//	//}
//	//if (curLevel>=6&&curLevel<=8)
//	//{
//	//	m_sprite_curLevel->setTexture(spriteStar[2]->getTexture()) ;
//	//}
//	//if (curLevel>=8&&curLevel<=10)
//	//{
//	//	m_sprite_curLevel->setTexture(spriteStar[3]->getTexture()) ;
//	//}
//	//
//
//	//if (nextLevel<=2)
//	//{
//	//	//m_sprite_nextLevel->setTexture(spriteStar[0]->getTexture()) ;
//	//}
//
//	//if (nextLevel>2&&nextLevel<=5)
//	//{
//	//	//m_sprite_nextLevel->setTexture(spriteStar[1]->getTexture()) ;
//	//}
//	//if (nextLevel>=6&&nextLevel<=8)
//	//{
//	//	//m_sprite_nextLevel->setTexture(spriteStar[2]->getTexture()) ;
//	//}
//	//if (nextLevel>=8&&nextLevel<=10)
//	//{
//	//	//m_sprite_nextLevel->setTexture(spriteStar[3]->getTexture()) ;
//	//}
//	//if (nextLevel==11)
//	//{
//	//	//m_sprite_nextLevel->setVisible(false) ;
//	//	//s_nextLevel->setVisible(false) ;
//	//}
//	//else
//	//{
//	//	m_sprite_nextLevel->setVisible(true) ;
//	//	s_nextLevel->setVisible(true) ;
//	//}
//
//
//
//	//sprintf(buff,"LV%d",curLevel) ;
//	//setUITextString(s_curLevel,buff);
//	//if (nextLevel<=10)
//	//{
//	//	sprintf(buff,"LV%d",nextLevel) ;
//	//	setUITextString(s_nextLevel,buff);
//	//}
//	//else
//	//{
//	//	setUITextString(s_nextLevel,"max");
//	//}
//
//
//	//s_levelPercent->setPercent(g_globalMyFamilyInfo.getFamilyLevelPercent());
//
//	//sprintf(buff,"%d%%",g_globalMyFamilyInfo.getFamilyLevelPercent()) ;
//	//setUITextString(s_textLevelPercent,buff);
//	//
//
//	//sprintf(buff,"%d%%",g_globalMyFamilyInfo.getCurFanli()) ;
//	//setUITextString(s_curFanli,buff);
//	//sprintf(buff,"下级返利比%d%%",g_globalMyFamilyInfo.getNextFanli()) ;
//	//setUITextString(s_netFanLi,buff);
//
//
//	//m_strNotice = g_globalMyFamilyInfo.m_familyNotice.c_str() ;
//	//setUITextString(s_familyGongGao,g_globalMyFamilyInfo.m_familyNotice.c_str());
//
//	//sprintf(buff,"帮会成员（%d/%d）",g_globalMyFamilyInfo.getFamilyCurRole(),g_globalMyFamilyInfo.getFamilyMaxRole()) ;
//	////s_curRole->setText(buff);
//	//setUITextString(s_curRole,buff);
//
//	////俱乐部张可额外获得俱乐部所有成员返利总值的1%
//	//sprintf(buff,"俱乐部长可额外获得俱乐部所有成员返利总值的%d%%",g_globalMyFamilyInfo.m_fatherFanli) ;
//	//setUITextString(s_textFanliTips,buff);
//
//	//int lastTime = g_globalMyFamilyInfo.getLastTime();
//	//setGetReward(lastTime) ;
//
//
//
//
//	//updateAdminList();
//	//updateRankIconShow();
//	////权限不足
//	//if (g_globalMyData.getFamilyPower()==0)
//	//{
//	//	s_buttonEditGongGao->setEnabled(false);
//	//	s_buttonIconEdit->setEnabled(false);
//	//	s_buttonInvite->setVisible(false);
//	//	s_editBoxGongGao->setEnabled(false);
//	//	s_buttonEdit->setVisible(false) ;
//	//}
//	//else
//	//{
//	//	s_buttonEditGongGao->setEnabled(true);
//	//	s_buttonIconEdit->setEnabled(true);
//	//	s_buttonInvite->setVisible(true);
//	//	s_editBoxGongGao->setEnabled(true);
//	//	s_buttonEdit->setVisible(true) ;
//	//}
//
//	//if (g_globalMyData.getFamilyPower()==2)
//	//{
//	//	m_layerTipsEditIcon->setVisible(true);
//	//}
//	//else
//	//{
//	//	m_layerTipsEditIcon->setVisible(false);
//	//}
//	//s_nodeFanLiTips->setVisible(false);
//	//setShowDataLoading(false) ;
//	//s_logicRoot->setEnabled(true) ;
//
//}
//
//
//void MyFamily::editBoxReturn(cocos2d::ui::EditBox* editBox)
//{
//	if (editBox == s_editBoxGongGao)
//	{
//		int a =0 ;
//		s_editBoxGongGao->setVisible(true) ;
//	}
//}
//
//void MyFamily::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
//{
//	if (editBox == s_editBoxGongGao)
//	{
//		//std::string sgongao= g_globalMyFamilyInfo.m_familuGongGao ;
//		//s_editBoxGongGao->setText(m_strNotice.c_str());
//
//
//		//s_editBoxGongGao->setVisible(false) ;
//	}
//}
//
//void MyFamily::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
//{
//	if (editBox == s_editBoxGongGao)
//	{
//		s_editBoxGongGao->setVisible(true) ;
//
//		std::string s_gonggao = editBox->getText();
//
//		auto pStr = s_gonggao.c_str();
//		const UTF8*pUtf8Str = (const UTF8*)pStr;
//		const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
//
//		unsigned uL = getUTF8StringLength(pUtf8Str);
//
//
//		if (uL>55)
//		{
//			setUITextString(s_familyGongGao,m_strNotice.c_str());
//			editBox->setText("") ;
//			PopTipWithBt1("公告字数过度，请输入小于55个字","确定",nullptr);
//			return ;
//		}
//
//		setUITextString(s_familyGongGao,s_gonggao);
//		m_strNotice = s_gonggao ;
//		editBox->setText("") ;
//
//		//修改俱乐部公告EditFamliyNotice
//		Json::Value msg;
//		msg["fnotice"] = s_gonggao;
//		msg["ufamilyid"] = g_globalMyData.getFamilyID();
//		SendClientRequest(ClientRequest::kEditFamliyNotice,msg);
//
//	}
//}
//
//void MyFamily::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
//{
//	if (editBox == s_editBoxGongGao)
//	{
//		int a =0 ;
//		s_editBoxGongGao->setVisible(true) ;
//	}
//}
//
//
//
//void MyFamily::onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//	if (type == TOUCH_EVENT_BEGAN)
//	{
//		s_nodeFanLiTips->setVisible(false);
//	}
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		//贡献排行
//		if (pItem == buttonRank)
//		{
//
//			auto pHall = dynamic_cast<GameHall*>(getParent());
//			pHall->showContrubutionRankList(true);
//			Json::Value msg;
//			msg["ufamilyid"] = g_globalMyData.getFamilyID();
//			SendClientRequest(ClientRequest::kGetFamilyRankRole,msg);
//
//			return ;
//		}
//
//
//		//编辑头像
//		if (pItem == s_buttonIconEdit)
//		{
//			if (g_globalMyData.getFamilyPower()!=2)
//			{
//				return ;
//			}
//			auto pHall = dynamic_cast<GameHall*>(getParent());
//			pHall->showChooseFamilyIcon(true);
//
//			return ;
//		}
//
//		if (pItem == s_button_GameFamily)
//		{
//			auto pHall = dynamic_cast<GameHall*>(getParent());
//			pHall->showMyFamily(false,false);
//			pHall->showTargetFamily(false,false) ;
//			pHall->showGameFamily(false,true);
//		}
//
//
//
//
//		if (pItem == s_button_exit)
//		{
//					
//			if (g_globalMyData.getFamilyPower()==2)
//			{
//				PopTipWithBt1("您需要转让俱乐部后才能退出!", "确定", nullptr);
//			}
//			else
//			{
//				PopTipWithBt2("是否确定退出俱乐部！","退出","取消",[](unsigned ubt)
//			{
//			if(ubt == 0)
//			{
//				Json::Value param;
//				param["user_id"] = g_globalMyData.m_iUserId;
//				param["group_id"] = g_globalMyData.getFamilyID();
//				SendClientRequest(ClientRequest::kPushOutFamily, param);
//			}
//			});
//			}
//			return ;
//		}
//		if (pItem == s_buttonEdit)
//		{
//
//			return ;
//		}
//		//领取奖励
//		if (pItem == s_buttonGetFuli)
//		{
//
//			if (s_buttonGetFuli->isBright())
//			{
//				Json::Value msg;
//				msg["ufamilyid"] = g_globalMyData.getFamilyID();
//				SendClientRequest(ClientRequest::kGetFamiliyReward,msg);
//					}
//				else
//				{
//					SendGameTip("还未到领取时间！");
//				}
//
//			return ;
//		}
//		if (pItem == s_buttonInvite)
//		{
//
//			auto pHall = dynamic_cast<GameHall*>(getParent());
//			pHall->showInviteFriends(true) ;
//			return ;
//		}
//		if (pItem == s_buttonTips)
//		{
//			s_nodeFanLiTips->setVisible(true) ;
//			return ;
//		}
//		if (pItem == s_buttonEditGongGao)
//		{
//
//			return ;
//		}
//
//
//	}
//
//
//}
//
//
//
//
//void MyFamily::updateIcon()
//{
//	//SetIconUrl(s_FamilyIcon, g_globalFamilySKin.m_vecAllFamiluSkin.at(g_globalMyFamilyInfo.getFamilyIconUrl()).m_strIconUrl);
//	if (g_globalMyData.GetFamilySkin(g_globalMyFamilyInfo.getFamilyIconID()).m_strIconUrl!="")
//	{
//		SetFamilyIconUrl(s_FamilyIcon, g_globalMyData.GetFamilySkin(g_globalMyFamilyInfo.getFamilyIconID()).m_strIconUrl);
//	}
//	else
//	{
//		s_FamilyIcon->ResetToLocalSprite("family/defaulticon.png"); 
//		//s_FamilyIcon = CreateNode2AndPosAddTo<HttpSprite>("family/defaulticon.png", Size(188, 188), 0, 0, m_pClipTouXiang, 0);
//	}
//	
//	
//}
//
//
//
//void MyFamily::onGrabTouchBegan(bool bHitDialog)
//{
//
//	GameDialog::onGrabTouchBegan(bHitDialog) ;
//
//	if (bHitDialog)
//	{
//		s_nodeFanLiTips->setVisible(false);
//	}
//
//}
//
//
///************************************************************************/
///* 更新管理员                                                                     */
///************************************************************************/
//void MyFamily::updateAdminList()
//{
//	m_LayerAdmin->setVisible(true) ;
//	m_LayerAdmin->setEnabled(true) ;
//	int fatherid = -1 ;
//
//	std::vector<FamilyRoleBaseInfo> m_oldValue ;
//	for (int i=0;i< g_globalMyFamilyInfo.m_AllAdmin.size();i++)
//	{
//		if (g_globalMyFamilyInfo.m_AllAdmin.at(i).m_power==2)
//		{
//			fatherid =i;
//		}
//	}
//	if (fatherid!=-1)
//	{
//			m_oldValue.push_back( g_globalMyFamilyInfo.m_AllAdmin.at(fatherid)) ;
//	}
//
//	for (int i=0;i< g_globalMyFamilyInfo.m_AllAdmin.size();i++)
//	{
//		if (g_globalMyFamilyInfo.m_AllAdmin.at(i).m_power!=2)
//		{
//			m_oldValue.push_back( g_globalMyFamilyInfo.m_AllAdmin.at(i)) ;
//		}
//	
//	}
//	g_globalMyFamilyInfo.m_AllAdmin.clear() ;
//	for (int i=0;i<m_oldValue.size();i++)
//	{
//		g_globalMyFamilyInfo.m_AllAdmin.push_back(m_oldValue.at(i)) ;
//	}
//
//
//	for (int i=0;i<3;i++)
//	{
//		if (i<g_globalMyFamilyInfo.m_AllAdmin.size()&&g_globalMyFamilyInfo.m_AllAdmin.size()>=1)
//		{
//
//			m_spriteAdmin[i]->setVisible(true);
//			
//			if (g_globalMyFamilyInfo.m_AllAdmin.at(i).m_iconUrl!="")
//			{
//				SetIconUrl(m_spriteAdmin[i], g_globalMyFamilyInfo.m_AllAdmin.at(i).m_iconUrl);
//			}
//			else
//			{
//				m_spriteAdmin[i]->ResetToLocalSprite("defaulticon.png"); 
//			}
//
//			m_buttonAdmin[i]->setVisible(true);
//
//			m_buttonAdminBGNull[i]->setVisible(false);
//			m_buttonAdminBG[i]->setVisible(true);
//		}
//		else
//		{
//			m_spriteAdmin[i]->setVisible(false);
//
//			m_buttonAdminBGNull[i]->setVisible(true);
//			m_buttonAdminBG[i]->setVisible(false);
//
//		}
//	}
//}
//
//void MyFamily::updateTop5RankList()
//{
//	updateRankIconShow() ;
//
//
//}
//
//
//void MyFamily::updateRankIconShow()
//{
//	if (g_globalMyFamilyInfo.m_allTopRank.size()==0 && g_globalMyData.getFamilyPower()!=0)
//	{
//		s_nothingRoleTips->setVisible(true);
//		s_RoleShow->setVisible(false);
//	}
//	else
//	{
//
//		s_nothingRoleTips->setVisible(false);
//		s_RoleShow->setVisible(true);
//		for (int i = 0; i < 5; i++)
//		{
//			if (i<g_globalMyFamilyInfo.m_allTopRank.size())
//			{
//				SetIconUrl(m_spriteRankList[i], g_globalMyFamilyInfo.m_allTopRank.at(i).m_iconUrl);
//				m_imgRankList[i]->setVisible(true);
//
//			}
//			else
//			{
//				m_imgRankList[i]->setVisible(false);
//			}
//		}
//	}
//}
//
//
//void MyFamily::callAdminCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		s_nodeFanLiTips->setVisible(false);
//		Button* button = (Button*)pItem ;
//		//再次检测下权限问题 只有族长可操作
//	/*	if (g_globalMyData.getFamilyPower()==2)
//		{*/
//			for (int i=0; i<3 ; i++)
//			{
//				if (button == m_buttonAdmin[i])
//				{
//
//					if (g_globalMyFamilyInfo.m_isOwner)
//					{
//						if (m_buttonAdminBGNull[i]->isVisible())
//						{
//							auto pHall = dynamic_cast<GameHall*>(getParent());
//							pHall->showChangeShaikh(true) ;
//						}
//						else
//						{
//							if (g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid == g_globalMyData.m_iUserId)
//							{
//								Json::Value param;
//								param["tid"] = g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid;
//								SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
//							}
//							else
//							{
//								/*auto pHall = dynamic_cast<GameHall*>(getParent());
//								pHall->showEditFamilyRole(true,g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid,
//								(Edit_LookInf0|Edit_No_ViceShaikh|Edit_ChangeShaikh|Edit_Push));*/
//							}
//							
//						}
//					}
//					else
//					{
//						if (!m_buttonAdminBGNull[i]->isVisible())
//						{
//							Json::Value param;
//							param["tid"] = g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid;
//							SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
//							auto pHall = dynamic_cast<GameHall*>(getParent());
//							//pHall->setUserInfoZorder(20) ;
//						}
//						
//					}
//				
//				}
//			}
//		}
//}
//
//void MyFamily::showEditAdminLayer(bool flag)
//{
//
//}
//
//
//void MyFamily::updateRoleRankList()
//{
//	s_ContributionLayer->UpdateView();
//}
//
//void MyFamily::setVisible(bool bVisible)
//{
//
//	PlayVisibleSound(bVisible);
//	if (bVisible)
//	{
//
//		/*	auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
//		runAction(Sequence::create(CallFunc::create([this](){
//		setOpacity(0);setScale(0);
//		GameDialog::setVisible(true);
//		initShow() ;
//
//		}), actScale, nullptr));*/
//		GameDialog::setVisible(true);
//		initShow() ;
//		Tools::callScaleAction(this) ;
//	}
//	else
//	{
//		SendClientRequest(ClientRequest::kOptHallBgLayer);
//		GameDialog::setVisible(false);
//		initShow() ;
//		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
//		runAction(Sequence::create(actScale, CallFunc::create([this](){
//			GameDialog::setVisible(false);
//		initShow() ;
//		}), nullptr));*/
//	}
//		setShowDataLoading(bVisible) ;
//}
//
//void MyFamily::setOnVisible(bool bVisible)
//{
//	GameDialog::setOnVisible(bVisible) ;
//
//	initShow() ;
//	setShowDataLoading(bVisible) ;
//}
//
//void MyFamily::initShow()
//
//{
//	m_LayerAdmin->setEnabled(false) ;
//	s_RoleShow->setVisible(false) ;
//	s_nothingRoleTips->setVisible(false) ;
//	setUITextString(s_familyName,"");
//	setUITextString(s_fangilyID,"");
//	setUITextString(s_familyGongGao,"");
//	setUITextString(s_curRole,"帮会成员（0/0）");
//	s_TimeLayer->setVisible(false) ;
//
//	for (int i=0;i<3;i++)
//	{
//		m_buttonAdminBGNull[i]->setVisible(true);
//		m_spriteAdmin[i]->setVisible(false) ;
//	}
//	m_buttonAdminBG[0]->setVisible(false) ; 
//	m_layerTipsEditIcon->setVisible(false) ;
//	s_buttonEdit->setVisible(false) ;
//	s_buttonInvite->setVisible(false) ;
//	s_FamilyIcon->ResetToLocalSprite("family/defaulticon.png"); 
//	s_logicRoot->setEnabled(false) ;
//}
//
//void MyFamily::setGetReward(long time)
//{
//	s_TimeLayer->setVisible(true) ;
//	s_buttonGetFuli->setBright(time==0);
//
//
//	int lastTime = time;
//	char buff[64];
//	int day = lastTime/60/60/24;
//	int i_hours = lastTime/60/60%24;
//	sprintf(buff,"%d",day) ;
//	setUITextString(s_day,buff);
//
//
//	int maxH = i_hours/10 ;
//	sprintf(buff,"%d",maxH) ;
//	setUITextString(s_hoursf,buff);
//	int maxM = i_hours%10 ;
//	sprintf(buff,"%d",maxM) ;
//	setUITextString(s_hours,buff);
//
//}
//
//
//void MyFamily::cleanPower()
//{
//
//	s_buttonEdit->setVisible(false);
//
//	s_buttonEditGongGao->setEnabled(false);
//	s_buttonIconEdit->setEnabled(false);
//	s_buttonInvite->setVisible(false);
//	s_editBoxGongGao->setEnabled(false);
//	
//}
//
//void MyFamily::updateNotice(std::string notice)
//{
//	setUITextString(s_familyGongGao,notice.c_str());
//}
//
//
//void MyFamily::setShowDataLoading(bool flag)
//{
//	if (s_dataLoading == nullptr)
//	{
//
//		s_dataLoading = Tools::creatDataLoading(this,587, 266,99) ;
//	}
//	s_dataLoading->setVisible(flag) ;
//
//}