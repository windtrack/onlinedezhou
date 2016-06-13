//#include "ShowFamily.h"
//#include "GlobalUserData.h"
//#include "Util.h"
//#include "GameFamily.h"
//#include "MenuItemSpriteWithLabel.h"
//#include "GameHall.h"
//
//ShowFamily* ShowFamily::creatWithLayer(GameFamily* layer)
//{
//	ShowFamily* sFamliy = new ShowFamily() ;
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
//bool ShowFamily::initWithLayer(GameFamily* layer)
//{
//
//	return true ;
//}
//
//
//bool ShowFamily::init()
//{
//
//
//	if (!GameDialog::init("dialog/bg.png",false))
//	{
//		return false ;
//	}
//	if (!MyFamily::init())
//	{
//		return false ;
//	}
//	Size sz = this->getContentSize() ;
//	m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(ShowFamily::onMenuItemCloseSelected, this));
//	m_btMenuItemClose->setPosition(sz.width - 25, sz.height - 38);
//	auto menu = Menu::create(m_btMenuItemClose, nullptr);
//	menu->setPosition(Vec2::ZERO);
//	addChild(menu, 100);
//
//
//
//	s_TimeLayer->setVisible(false);
//	s_buttonInvite->setVisible(false);
//	s_buttonEdit->setVisible(false);
//	s_editBoxGongGao->setEnabled(false);
//	s_buttonIconEdit->setEnabled(false) ;
//	buttonRank->setVisible(false);
//	s_button_exit->setVisible(false) ;
//	s_button_GameFamily->setVisible(false) ;
//	setUITextString(s_TextGetFuli,"加入俱乐部");
//	//s_buttonBack->addTouchEventListener(this,toucheventselector(ShowFamily::onButtonCallBack));
//	s_buttonGetFuli->addTouchEventListener(this,toucheventselector(ShowFamily::onButtonCallBack));
//
//
//
//	for (int i=0; i<3 ;i++)
//	{
//
//
//		m_buttonAdmin[i]->addTouchEventListener(this,toucheventselector(ShowFamily::callAdminCallBack)) ;
//
//	}
//
//
//	return true ;
//}
//
//
//void ShowFamily::callAdminCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//
//		if (type == TOUCH_EVENT_ENDED)
//	{
//		s_nodeFanLiTips->setVisible(false);
//		Button* button = (Button*)pItem ;
//
//			for (int i=0; i<3 ; i++)
//			{
//				if (button == m_buttonAdmin[i])
//				{
//
//				
//						if (!m_buttonAdminBGNull[i]->isVisible())
//						{
//							Json::Value param;
//							param["tid"] = g_globalMyFamilyInfo.m_AllAdmin.at(i).m_uid;
//							SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
//							//auto pHall = dynamic_cast<GameHall*>(getParent());
//							//pHall->setUserInfoZorder(20) ;
//						}
//				}
//			}
//		}
//
//}
//
//void ShowFamily::updateFamilyInfo()
//{
//
//	MyFamily::updateFamilyInfo();
//	s_buttonGetFuli->setBright(true);
//
//	s_TimeLayer->setVisible(false);
//
//	s_buttonEdit->setVisible(false);
//
//	buttonRank->setVisible(false);
//
//	s_nothingRoleTips->setVisible(false) ;
//
//	s_buttonEditGongGao->setEnabled(false);
//	s_buttonIconEdit->setEnabled(false);
//	s_buttonInvite->setVisible(false);
//	s_editBoxGongGao->setEnabled(false);
//	//m_LayerAdmin->setEnabled(false);
//	m_layerTipsEditIcon->setVisible(false);
//
//}
//
//void ShowFamily::onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
//{
//	if (type == TOUCH_EVENT_BEGAN)
//	{
//		s_nodeFanLiTips->setVisible(false);
//	}
//
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		if (pItem == s_button_exit)
//		{
//			
//
//			return ;
//		}
//		if (s_buttonGetFuli == pItem)
//		{
//			//加入俱乐部
//			if (g_globalMyData.getHasCreatFamily())
//			{
//				PopTipWithBt1("您已加入俱乐部!", "确定", nullptr);
//				return ;
//			}
//			Json::Value msg;
//			msg["ufamilyid"] = g_globalMyFamilyInfo.getFamilyID();
//			SendClientRequest(ClientRequest::kJoinFamily,msg);
//
//			return ;
//		}
//		
//
//	}
//
//}
//
//void ShowFamily::onMenuItemCloseSelected(cocos2d::Ref * ref)
//{
//
//	GameDialog::setOnVisible(false);
//	//s_pLayer->showMyFamily(false) ;
//}
//
//
