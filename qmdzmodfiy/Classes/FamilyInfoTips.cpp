//#include "FamilyInfoTips.h"
//#include "GameFrame.h"
//#include "Tools.h"
//#include "GlobalUserData.h"
//#include "Util.h"
//
//USING_NS_CC ;
//bool FamilyInfoTips::init()
//{
//	if(!GameDialog::init("dialog/bg.png",true))
//	{
//		return false;
//	}
//
//
//	Size sz = this->getContentSize() ;
//	s_uiroot = CSLoader::createNode("res/ui/familyinfo.csb");
//	s_uiroot->ignoreAnchorPointForPosition(false);
//	s_uiroot->setAnchorPoint(Point(0.5,0.5));
//	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
//	this->addChild(s_uiroot,2);
//
//
//	s_familyInfoBackroundLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_familyinfo"));
//	s_familyInfoBackroundLayer->setVisible(true);
//
//	s_familyInfoRuleLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_family_rule"));
//	s_familyInfoRuleLayer->setVisible(true);
//
//	s_familyInfoFuliLayer = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_family_fuliinfo"));
//	s_familyInfoFuliLayer->setVisible(true);
//
//	s_familyInfoManager[0] = s_familyInfoRuleLayer ;
//	s_familyInfoManager[1] = s_familyInfoFuliLayer ;
//
//
//	char* stirng_buttonfamilyMenu[2]= {"Button_familyinfo_rule", "Button_familyinfo_fuli",} ;
//	char* stirng_buttonfamilyMenuText[2]= {"Text_fi_rule", "Text_fi_fili", } ;
//
//	for (int i=0; i<2; i++)
//	{
//		s_button_familyInfoMenu[i] = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,stirng_buttonfamilyMenu[i]));
//		s_button_familyInfoMenu[i]->setBright(true);
//		s_button_familyInfoMenu[i] ->addTouchEventListener(this,toucheventselector(FamilyInfoTips::onFamilyInfoMenuItemClick));
//		s_button_familyInfoMenuText[i] = static_cast<Text*>(Tools::getWidgetByName(s_uiroot,stirng_buttonfamilyMenuText[i]));
//	}
//
//
//
//	//初始化福利数据
//	std::string s_personRation[10] = {"Text_gerfanli1", "Text_gerfanli1_8", "Text_gerfanli1_16", "Text_gerfanli1_8_24", "Text_gerfanli1_32", "Text_gerfanli1_8_40", "Text_gerfanli1_48", "Text_gerfanli1_8_56","Text_gerfanli1_8_56_4","Text_gerfanli1_8_56_12" } ;
//	std::string s_fatherRation[10] = {"Text_jiazufanli1", "Text_jiazufanli1_10", "Text_jiazufanli1_18", "Text_jiazufanli1_10_26", "Text_jiazufanli1_34", "Text_jiazufanli1_10_42", "Text_jiazufanli1_50", "Text_jiazufanli1_10_58", "Text_jiazufanli1_10_58_6","Text_jiazufanli1_10_58_14"} ;
//	std::string s_maxrole[10] = {"Text_maxrole1", "Text_maxrole1_12", "Text_maxrole1_20", "Text_maxrole1_12_28", "Text_maxrole1_36", "Text_maxrole1_12_44", "Text_maxrole1_52", "Text_maxrole1_12_60", "Text_maxrole1_12_60_8","Text_maxrole1_12_60_16"} ;
//
//
//	for (int i=0;i<10;i++)
//	{
//		Text* textPersonRatio = static_cast<Text*>(Tools::getWidgetByName(s_familyInfoFuliLayer,s_personRation[i]));
//		char tData[64] ;
//		sprintf(tData,"%d%%",g_globalRatio.m_dataRatio[i]->m_personRatio) ;
//		setUITextString(textPersonRatio,tData) ;
//
//		textPersonRatio = static_cast<Text*>(Tools::getWidgetByName(s_familyInfoFuliLayer,s_fatherRation[i]));
//
//		sprintf(tData,"%d%%",g_globalRatio.m_dataRatio[i]->m_fatherRatio) ;
//		setUITextString(textPersonRatio,tData) ;
//
//		textPersonRatio = static_cast<Text*>(Tools::getWidgetByName(s_familyInfoFuliLayer,s_maxrole[i]));
//
//		sprintf(tData,"%d",g_globalRatio.m_dataRatio[i]->maxRole) ;
//		setUITextString(textPersonRatio,tData) ;
//
//	}
//	showFamilyInfoLayer(0) ;
//	return true ;
//}
//
//
//void FamilyInfoTips::setVisible(bool bVisible)
//{
//
//	PlayVisibleSound(bVisible);
//
//	if (bVisible)
//	{
//		GameDialog::setVisible(true);
//		showFamilyInfoLayer(0) ;
//		callShowAction() ;
//	/*	auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
//		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
//		//GameDialog::setVisible(true);
//	}
//	else
//	{
//		callHideAction() ;
//		//SendClientRequest(ClientRequest::kOptHallBgLayer);
//		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
//		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
//	}
//
//}
//
//
//void FamilyInfoTips::onFamilyInfoMenuItemClick(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		int index = 0 ;
//
//
//
//		for (int i=0;i<2; i++)
//		{
//			if (pItem == s_button_familyInfoMenu[i])
//			{
//				showFamilyInfoLayer(i) ;
//			}
//		}
//
//
//
//
//	}
//}
//
//void FamilyInfoTips::showFamilyInfoLayer(int index)
//{
//	for (int i=0;i<2; i++)
//	{
//
//			s_button_familyInfoMenu[i]->setBright(i!=index) ;
//			s_button_familyInfoMenuText[i]->setTextColor(i!=index?Color4B(0x37,0xa4,0x5c,0xff):Color4B(0xff,0xff,0xff,0xff));
//			s_familyInfoManager[i]->setVisible(i==index);
//	}
//}