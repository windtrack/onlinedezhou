#include "VipPrivateNormal.h"
#include "MenuItemFrameSprite.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "GameCommon.h"
#include "TextureNumber.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "SelfDefinition.h"
#include "Tools.h"
#include "GameData.h"
#include "FrameScale9SpriteButton.h"
#include "SelfFund.h"
#include "PrivateRoomRule.h"
USING_NS_CC;



//static int ScrollW = GAME_WIDTH ;
//static int ScrollH =  GAME_HEIGHT - 180 ;


bool VipPrivateNormal::init()
{
	if(!GameDialog::init())
	{
		return false;
	}
	m_selfDefinition = nullptr ;
	m_selfFund = nullptr ;

	m_privateRoomRule = nullptr ;
	auto rootNode = CSLoader::createNode("res/ui/private_hall.csb");
	Size frameSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(frameSize);
	ui::Helper::doLayout(rootNode);
	addChild(rootNode);

	m_pLayoutShow = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_alllayer"));

	//背景
	m_pSpriteBgTable = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_spritebg"));
	auto imageLeft = static_cast<Layout*>(Tools::getWidgetByName(m_pSpriteBgTable,"Image_bgleft"));
	auto imageRight = static_cast<Layout*>(Tools::getWidgetByName(m_pSpriteBgTable,"Image_bgright"));
	Sprite *bgLeft = Sprite::create("vipprivateroom/privatebg.png") ;
	Sprite *bgRignt = Sprite::createWithTexture(bgLeft->getTexture()) ;
	bgLeft->setPosition(imageLeft->getContentSize().width/2,imageLeft->getContentSize().height/2) ;
	bgRignt->setPosition(imageRight->getContentSize().width/2,imageRight->getContentSize().height/2);
	imageLeft->addChild(bgLeft);
	imageRight->addChild(bgRignt);

	//上层
	//m_pLayoutTop = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_VipRoom_Top"));

	// 底下的时间背景
	m_pVipTimeSprite = Sprite::create("vipprivateroom/sm_zs.png");
	addChild(m_pVipTimeSprite);



	//返回
	m_pBtBack = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_back"));
	m_pBtBack->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	//商城
	m_pBtEnterMall = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_shop"));
	m_pBtEnterMall->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	//客服
	m_pBtEnterClient = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_kefu"));
	m_pBtEnterClient->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));


	m_pBtSelfFund = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_myjijin"));
	m_pBtSelfFund->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	m_pBtClubFund = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_clubfund"));
	m_pBtClubFund->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));
	
	m_pBtHelp = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_help"));
	m_pBtHelp->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));


	m_btnCreatRoom = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_makeroom"));
	m_btnCreatRoom->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));


	m_pBtJoinRoom = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_joinroom"));
	m_pBtJoinRoom->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	
	//创建房间按钮
	/*m_btnCreatRoom = FrameScale9SpriteButton::createWithScale9Sprite(1, Size(234, 74),false, true);
	auto buttonDes = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"我要开房",Vec2(0.5,0.5),0,0,Color4B::WHITE,m_btnCreatRoom,0);
	m_btnCreatRoom->setPosition(GameData::getInstance()->getGameWidth()/2,40);
	this->addChild(m_btnCreatRoom);
	m_btnCreatRoom->SetHitCB(std::bind(&VipPrivateNormal::creatPrivateRoom, this));*/

	//滚动的线
	m_imageLineUp = static_cast<cocos2d::ui::ImageView*>(Tools::getWidgetByName(rootNode, "Image_lineup")); 
	m_imageLineUp->setVisible(false) ;

	//金币文本
	m_textCach = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(rootNode, "Text_money"));
	//更新金币
	g_globalMyData.WaitChange(1, std::bind(&VipPrivateNormal::UpdateCash, this, std::placeholders::_1));


	m_dOffSet = 0.0;
	//滚动层

	auto nodeScroll = Tools::getWidgetByName(rootNode, "Panel_VipRoom_Normal");
	auto szView = nodeScroll->getContentSize();
	m_sizeView = szView ;
	Point posPlayer = nodeScroll->getPosition() ;
	m_scrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(m_sizeView.width, m_sizeView.height), 0, 0, nodeScroll, 0);
	m_scrollView->setContentSize(Size(m_sizeView.width, m_sizeView.height));
	m_scrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_scrollView->setDelegate(this);


	m_textNulltips = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(rootNode, "Text_nulltips"));
	m_textNulltips->setVisible(false) ;
	m_textNulltipsClub = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(rootNode, "Text_nulltips_0"));
	m_textNulltipsClub->setVisible(false) ;
	
	//m_pBtRefsh = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_refsh"));
	//m_pBtRefsh->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	m_textSerach = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(rootNode, "Text_tips"));
	m_textJoinRoom  = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(rootNode, "Text_joinroom"));
	//m_pBtReset = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_reset"));
	//m_pBtReset->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	//m_pBtSerch = static_cast<cocos2d::ui::ImageView*>(Tools::getWidgetByName(rootNode, "Image_serchreal"));
	m_imgeditbox = static_cast<cocos2d::ui::ImageView*>(Tools::getWidgetByName(rootNode, "Image_block"));
	
	m_EditSerch  = CreateNode2AndPosAddTo<ui::EditBox>(Size(m_imgeditbox->getContentSize().width, m_imgeditbox->getContentSize().height),
		ui::Scale9Sprite::create("vipprivateroom/bgedit.png") , m_imgeditbox->getContentSize().width / 2, m_imgeditbox->getContentSize().height / 2,m_imgeditbox, 0);

	m_EditSerch->setFont("fzlt.ttf", 26);
	m_EditSerch->setFontColor(Color3B(0xff,0xd8,0x00));
	m_EditSerch->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_EditSerch->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_EditSerch->setDelegate(this);

	m_btPerosonalRoom = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_personal"));
	m_btPerosonalRoom->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	m_btClubRoomRoom = static_cast<cocos2d::ui::Button*>(Tools::getWidgetByName(rootNode, "Button_club"));
	m_btClubRoomRoom->addTouchEventListener(CC_CALLBACK_2(VipPrivateNormal::OnMenuItemSelect, this));

	m_textPersonal = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(m_btPerosonalRoom, "Text_personal"));
	m_textClub = static_cast<cocos2d::ui::Text*>(Tools::getWidgetByName(m_btClubRoomRoom, "Text_club"));

	m_curTypeIndex = 0 ;
	return true ;
}


void VipPrivateNormal::showRoomView(bool showAction)
{
	m_DefineRoom[0].clear() ;
	m_DefineRoom[1].clear() ;
	for (auto& roomInfo:g_globalVipOpenRoom.m_UserDefineRoom)
	{
		m_DefineRoom[0].push_back(roomInfo);
	}
	for (auto& roomInfo:g_globalVipOpenRoom.m_ClubDefineRoom)
	{
		m_DefineRoom[1].push_back(roomInfo);
	}

	doRoomTypeSelect(m_curTypeIndex) ;
	if (showAction)
	{
		callAction() ;
	}

}

void VipPrivateNormal::updateRoomView(unsigned type)
{

	std::vector<DefineRoomInfo> curRoomInfo = m_DefineRoom[type] ;
	//std::vector<DefineRoomInfo> curRoomInfo = g_globalVipOpenRoom.m_UserDefineRoom ;

	m_textNulltips->setVisible(curRoomInfo.size()==0 &&m_curTypeIndex ==0) ;
	m_textNulltipsClub->setVisible(curRoomInfo.size()==0 &&m_curTypeIndex ==1) ;


	for (auto newItem : m_vecDefineRoomNew)
	{
		newItem->m_spriteTableBg->setVisible(false);
		m_vecDefineRoomOld.push_back(newItem);
	}
	m_vecDefineRoomNew.clear();


	unsigned uRoomNum = curRoomInfo.size();
	unsigned uOneItemH = 182+45;//直径加间隔
	unsigned uOneItemW = 278;
	unsigned uAllItemH = 0;

	if (uRoomNum%3 == 0 && uRoomNum > 0)
	{
		uAllItemH = (uRoomNum/3)*uOneItemH;
	}
	else
	{
		uAllItemH = (uRoomNum/3)*uOneItemH + uOneItemH;
	}
	if (uAllItemH < m_sizeView.height)
	{
		uAllItemH = m_sizeView.height;
	}
	else
	{
		m_scrollView->setBounceable(true);
	}

	m_dOffSet= 0.0;

	double offSet = m_sizeView.height - (double)uAllItemH;

	m_dOffSet = offSet;
	
	double fX = 28 + uOneItemW/2,fY = uAllItemH- uOneItemH/2;
	unsigned uHorizontalNum = 0;
	unsigned uRoomCount = 0;
	unsigned uCurPlayers = 0;



	auto&& v = curRoomInfo;
	for(auto dataItem = v.rbegin(); dataItem != v.rend(); dataItem++)
	{
		auto  roomItem = GetDefineRoomItem(dataItem->m_uAreaID);
		UpdateDefineRoomItem(roomItem, *dataItem);
		m_vecDefineRoomNew.push_back(roomItem);
		uHorizontalNum = uRoomCount % 3;
		roomItem->m_spriteTableBg->setPosition(fX + uHorizontalNum*(278+25), fY);
		roomItem->m_spriteTableBg->setVisible(true);
		if ((uRoomCount+1)%3 == 0)
		{
			fY -= uOneItemH;
		}
		uRoomCount++;
	}
	m_scrollView->setContentOffset(Vec2(0, m_dOffSet), false);
	m_scrollView->setContentSize(Size(m_sizeView.width, uAllItemH));

	
}


void VipPrivateNormal::onKeyEscPressed()
{
	SendClientRequest(ClientRequest::kLeaveHall2);
}

void VipPrivateNormal::callAction()
{


	float s_curX,s_curY ;
	for (int i=0;i<m_vecDefineRoomNew.size(); i++)
	{
		DefineRoomItem *table = m_vecDefineRoomNew.at(i) ;
		//table->m_spriteTableBg->setVisible(table->m_needShow) ;
		table->m_spriteTableBg->setVisible(true) ;
		s_curX = table->m_spriteTableBg->getPositionX() ;
		s_curY = table->m_spriteTableBg->getPositionY() ;
		table->m_spriteTableBg->setPositionY(s_curY-640) ;
		table->m_spriteTableBg->runAction(Sequence::create(DelayTime::create(0.5f),
			CCEaseSineOut::create(MoveTo::create(0.3+i/3*0.25f, Vec2(s_curX, s_curY))), nullptr));
	}
}

void VipPrivateNormal::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		m_curTypeIndex = 0 ;
		//doRoomTypeSelect(m_curTypeIndex) ;
		setUITextString(m_textSerach,"请输入四位数房间口令");

		SendClientRequest(ClientRequest::kGetVipOpenRoomInfo);
			
		setJoinRoomButtonEnable(false) ;

		setUITextString(m_textCach, FormatCash(g_globalMyData.GetCash()));

		//callAction() ;
		float s_curX= m_pSpriteBgTable->getPositionX();
		float s_curY= m_pSpriteBgTable->getPositionY();
		auto sz = m_pSpriteBgTable->getContentSize();
		m_pSpriteBgTable->setPositionY(-sz.height / 2);
		m_pSpriteBgTable->runAction(Sequence::create(DelayTime::create(0.1f), EaseBackOut::create(MoveTo::create(0.3f, Vec2(s_curX, s_curY))), nullptr));
		m_pVipTimeSprite->runAction(Sequence::create(DelayTime::create(0.8f), MoveTo::create(0.2f, Vec2(58, 36)), nullptr));



		m_pLayoutShow->setOpacity(0) ;
		m_pLayoutShow->setVisible(true) ;
		ActionInterval* fadeIn = FadeTo::create(0.7f,255) ;
		m_pLayoutShow->runAction(Sequence::create(DelayTime::create(0.8f),fadeIn, nullptr));


		PlayBackGroundSound("sound/bghall1.mp3", true);
	}
	else
	{

		for (int i=0;i<m_vecDefineRoomNew.size(); i++)
		{
			DefineRoomItem *table = m_vecDefineRoomNew.at(i) ;
			//table->m_spriteTableBg->setVisible(table->m_needShow) ;
			table->m_spriteTableBg->setVisible(false) ;
		}


		m_pVipTimeSprite->setPosition(Vec2(-100, 36));
		StopBackGroundSound(false);
		m_scrollView->setContentOffset(Vec2(0, m_dOffSet), false);
	}
}


void VipPrivateNormal::OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type)
{
	auto button = (cocos2d::ui::Button*)pSend;
	if (type == cocos2d::ui::Button::TouchEventType::ENDED)
	{
		Json::Value param;
		if (m_btPerosonalRoom == button)
		{
			doRoomTypeSelect(0) ;
		}
		else if (m_btClubRoomRoom == button)
		{
			doRoomTypeSelect(1) ;
		}
		else if (button == m_pBtBack)
		{
			SendClientRequest(ClientRequest::kLeaveVipOpenRoom);
		}
		else if (button == m_pBtEnterMall)
		{
			//g_pGameFrame->showGameMall(1);
			param["idx"] = 0;
			SendClientRequest(ClientRequest::kPopMall, param);
		}
		else if (button == m_pBtEnterClient)
		{
			SendClientRequest(ClientRequest::kPopCustomerServices);
		}
		else if (button == m_pBtJoinRoom)
		{
			
			Json::Value param;
			param["enterType"] = m_globaPrivateRoomInfo.m_roomid;
			SendClientRequest(ClientRequest::kEnterPrivateTable, param);
		}
		else if (button == m_btnCreatRoom)
		{
			if (m_curTypeIndex ==0)
			{
				if(g_globalMyData.GetVipLevel()==0)
				{
					PopTipWithBt2("亲~此功能至少要【贵族】级别才能创建，快去提升【贵族】级别吧！","升级权限","取消",[](unsigned ubt)
					{
						if(ubt == 0)
						{
							Json::Value param;
							param["idx"] = 1;
							SendClientRequest(ClientRequest::kPopMall, param);
						}
					});
				}
				else
				{
					Json::Value value ;
					value["type"] = 1 ;
					SendClientRequest(ClientRequest::kCallCreatPrivateRoomName,value);
				}
			}
			else
			{
				if (g_globalMyData.getHasCreatFamily())
				{
					Json::Value value ;
					value["type"] = 2 ;
					value["clubname"] = g_globalMyData.getFamilyName() ;
					value["clubid"] = g_globalMyData.getFamilyID();
					SendClientRequest(ClientRequest::kCallCreatPrivateRoomName,value);
				}
				else
				{
					PopTipWithBt1("需要创建自己的俱乐部后才能创建俱乐部房间","确定",nullptr) ;
				}


			}
		
		
			
		}
		else if (button == m_pBtSelfFund || button == m_pBtClubFund)
		{


			g_pGameFrame->showSelfFundLayer(true,m_curTypeIndex) ;
		}
		else if (button == m_pBtHelp)
		{
			showPrivateRoomRule(true) ;
		}

	}
}





void VipPrivateNormal::UpdateCash(unsigned cashflag)
{
	if (cashflag == WAITCASHCHANGE)
	{
		setUITextString(m_textCach, FormatCash(g_globalMyData.GetCash()));
	}
}


void VipPrivateNormal::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
	auto offSet = m_scrollView->getContentOffset();
	if (offSet.y > m_dOffSet)
	{
		m_imageLineUp->setVisible(true);
	}
	else
	{
		m_imageLineUp->setVisible(false);
	}
}







VipPrivateNormal::DefineRoomItem* VipPrivateNormal::GetDefineRoomItem(unsigned uDeskId)
{
	DefineRoomItem *item = nullptr;
	if (m_vecDefineRoomOld.empty())
	{
		auto bgItem = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/sm_tableicon.png", 61, 62, m_scrollView, 0);
		//auto bgItem = FrameSpriteButton::createWithVertFrameSpriteByOne("vipprivateroom/sm_tableicon.png", false, true);

		//m_scrollView->addChild(bgItem);

		auto m_pBtDetail = LayerButton::create(false,true) ;
		bgItem->addChild(m_pBtDetail) ;

		Size sz = bgItem->getContentSize() ;
		m_pBtDetail->setContentSize(sz);

		auto spriteHuman = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/sm_gamepeople.png", 79, 91, bgItem, 0);
		auto spriteEye = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/sm_pgpeople.png", 185, 87, bgItem, 0);
		auto m_spriteAnti = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/label_bx.png", 10, 133, bgItem, 0);

		auto m_spriteRoomidBG = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/sm_idbg.png", sz.width/2, -8, bgItem, 0);

		auto m_spritestate = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/sm_statebg.png", 219, 28, bgItem, 0);
		auto m_spriteRoomMine = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/sm_myicon.png", 168, 28, bgItem, 0);


		auto m_labelRoomName = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), sz.width/2, 124, Color4B(0xff,0xec,0xbf,0xff), bgItem, 0);
		auto m_labelPlayersNum = CreateLabelMSYHBDAndAnchorPosClrAddTo(26, "", Vec2(0,0.5), 97, 91, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);
		auto m_labelLookOnNum = CreateLabelMSYHAndAnchorPosClrAddTo(18, "", Vec2(0,0.5), 203, 86, Color4B(0xff,0xff,0xff,0x99), bgItem, 0);
		auto m_labelBlind = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), sz.width/2, 56, Color4B(0xff,0xda,0x80,0xff), bgItem, 0);
		auto m_labelRoomOwnerName = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), m_spriteRoomidBG->getContentSize().width/2, m_spriteRoomidBG->getContentSize().height/2, Color4B(0x5f,0xc5,0x92,0xff), m_spriteRoomidBG, 0);

		auto m_labelRoomState = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), 35, 15, Color4B(0xb4,0xb4,0xb4,0xff), m_spritestate, 0);

		auto pItem = new DefineRoomItem;

		pItem->m_spriteTableBg = bgItem;
		pItem->m_spriteHuman = spriteHuman;
		pItem->m_spriteEye = spriteEye;
		pItem->m_spriteAnti = m_spriteAnti;
		pItem->m_spriteState = m_spritestate;
		pItem->m_spriteRoomidBG = m_spriteRoomidBG;
		pItem->m_spriteRoomMine = m_spriteRoomMine ;


		pItem->m_labelBlind = m_labelBlind;
		pItem->m_labelPlayersNum = m_labelPlayersNum;
		pItem->m_labelLookOnNum = m_labelLookOnNum;
		pItem->m_labelRoomscreat = m_labelRoomName;
		pItem->m_labelRoomOwnerName = m_labelRoomOwnerName;
		pItem->m_labelRoomstate = m_labelRoomState ;

		pItem->m_pBtDetail = m_pBtDetail ;
		item = pItem;
	}
	else
	{
		item = m_vecDefineRoomOld.back();
		m_vecDefineRoomOld.pop_back();
	}
	//item->m_spriteTableBg->SetHitCB([this, uDeskId](Ref*){
	//	//auto parent = dynamic_cast<VipPrivateNormal*>(m_rootNode->getParent());
	//	//parent->UpdateSelfEnterUI(uDeskId);
	//});

	return item;
}

void VipPrivateNormal::UpdateDefineRoomItem(VipPrivateNormal::DefineRoomItem *item, const DefineRoomInfo &data)
{
	char buff[64];
	//SetLabelStringWithBoundWidth(item->m_labelRoomscreat, data.m_strRoomName, 160);
	//当年玩家
	sprintf(buff, "%d/%d", data.m_uCurPlayers, data.m_uMaxPlayers);
	SetLabelString(item->m_labelPlayersNum, buff);
	//旁观人数
	sprintf(buff, "%d", data.m_uLookOnNum);
	SetLabelString(item->m_labelLookOnNum, buff);
	//大小盲
	std::string strMinBlind = FormatCash(data.m_dLittleBlind);
	std::string strMaxBlind = FormatCash(data.m_dBigBlind);
	sprintf(buff, "%s/%s", strMinBlind.c_str(), strMaxBlind.c_str());
	SetLabelString(item->m_labelBlind, buff);
	//房主名称
	SetLabelString(item->m_labelRoomOwnerName, data.m_strRoomName.c_str());
	if (m_curTypeIndex ==0)
	{
		//房间口令
		sprintf(buff, "口令:%s", data.m_strRoomPwd.c_str());
		SetLabelString(item->m_labelRoomscreat, buff);
	}
	else
	{
		//房间口令
		sprintf(buff, "%s", data.m_strRoomPwd.c_str());
		SetLabelString(item->m_labelRoomscreat, buff);
	}
	
	
	//是否是我的房间
	item->m_ismine = data.m_isMine ;

	item->m_spriteState->setVisible(true) ;
	//必下场
	item->m_spriteAnti->setVisible(data.m_dMustBet>0) ;
	//我的房间标志
	item->m_spriteRoomMine->setVisible(item->m_ismine) ;
	//item->m_spriteRoomMine->setVisible(data.m_isMine) ;
	//房间状态
	if (data.m_state == 0)
	{
		SetLabelString(item->m_labelRoomstate, "未开始");
	}
	else if (data.m_state == 1)
	{
		SetLabelString(item->m_labelRoomstate, "进行中");
	}
	else if (data.m_state == 2)
	{
		SetLabelString(item->m_labelRoomstate, "进行中");
	}
	
	item->m_pBtDetail->SetHitCB(std::bind(&VipPrivateNormal::doJoinRoom, this,data));
}

void VipPrivateNormal::doJoinRoom(const DefineRoomInfo &data)
{


	updateCurPrivateRoomInfo(data) ;

	Json::Value param;
	//param["enterType"] = 0;
	param["enterType"] = data.m_uAreaID;
	SendClientRequest(ClientRequest::kEnterPrivateTable, param);



}




void VipPrivateNormal::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	if (editBox == m_EditSerch)
	{
		//std::string str_text = editBox->getText() ;
		std::string showText = editBox->getText() ;

		setUITextString(m_textSerach,showText.c_str());
		m_EditSerch->setText("") ;

		std::string showid = "" ;

		//是数字并且 是4位 
		if (Tools::is_number(showText) && showText.length()==4)
		{
			//creatDemoRoom() ;
			//setJoinRoomButtonEnable(true) ;
		

			//m_showBySerch.clear() ;
			
			//发送请求

			Json::Value parm ;
			parm["password"] = showText ;
			SendClientRequest(kSerachPriavteRoom,parm);


			/*Json::Value parm ;
			parm["type"] = 0 ;
			SendClientRequest(kRequsetPrivateRoomInfo,parm);*/

		}
		else
		{
			setJoinRoomButtonEnable(false) ;
			//如果为空 返回默认界面
			PopTipWithBt1("请输入正确的口令！","确定",nullptr) ;
			m_textSerach->setText("") ;
			m_EditSerch->setText("") ;
		/*	if (showText.compare(""))
			{
				updateRoomView(g_globalVipOpenRoom.GetDefineRoomInfo()) ;

			}
			else
			{

			}*/
		}

	}




}

void VipPrivateNormal::editBoxTextChanged(EditBox* editBox, const std::string& text)
{

}

void VipPrivateNormal::editBoxEditingDidBegin(EditBox* editBox)
{
	if (editBox == m_EditSerch)
	{
		m_textSerach->setText("") ;
		m_EditSerch->setText("") ;

	}
}


void VipPrivateNormal::creatDemoRoom()
{
	for (unsigned i = 0; i < 1; i++)
	{
		DefineRoomInfo pDefineRoomInfo ;
		pDefineRoomInfo.m_uAreaID = 10000;
		pDefineRoomInfo.m_dLittleBlind = 2500;
		pDefineRoomInfo.m_dBigBlind = 5000;
		pDefineRoomInfo.m_dMaxCarry = 250000;
		pDefineRoomInfo.m_uPlayers = 5;
		pDefineRoomInfo.m_dCoverCharge = 0;
		pDefineRoomInfo.m_dEnterMoney = 0;
		pDefineRoomInfo.m_uLookOnNum = 85;
		pDefineRoomInfo.m_uMaxPlayers = 6;
		pDefineRoomInfo.m_uCurPlayers = 5;
		pDefineRoomInfo.m_dMustBet = 500;
		pDefineRoomInfo.m_strRoomName = "巡山的大王";
		pDefineRoomInfo.m_strRoomPwd = "4545";
		//pDefineRoomInfo.m_uRoomTime = 4;
		pDefineRoomInfo.m_uType = 0;
		pDefineRoomInfo.m_state = rand()%1 ;
		pDefineRoomInfo.m_isMine = rand()%1 ;
		//pDefineRoomInfo.m_uRoomID = rand()%7999+1000 ;
		g_globalVipOpenRoom.m_UserDefineRoom.push_back(pDefineRoomInfo);
	}


	doRoomTypeSelect(0) ;
}


void VipPrivateNormal::setJoinRoomButtonEnable(bool flag)
{
	m_pBtJoinRoom->setEnabled(flag);
	if (flag)
	{
		m_pBtJoinRoom->setBright(true) ;
		m_textJoinRoom->setOpacity(255) ;
		m_textJoinRoom->setOpacityModifyRGB(true) ;
	}
	else
	{
		m_pBtJoinRoom->setBright(false) ;
		m_textJoinRoom->setOpacity(80) ;
		m_textJoinRoom->setOpacityModifyRGB(true) ;


	}
}



void VipPrivateNormal::showCreatRoomLayer(bool flag)
{

	if (m_selfDefinition == nullptr)
	{
		m_selfDefinition = SelfDefinition::create() ;
		m_selfDefinition -> ignoreAnchorPointForPosition(false);
		m_selfDefinition -> setAnchorPoint(Point(0.5,0.5));
		m_selfDefinition->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		this->addChild(m_selfDefinition) ;
	}
	m_selfDefinition->initWithDefaultValue() ;
	m_selfDefinition->setVisible(flag);

}



void VipPrivateNormal::showPrivateRoomRule(bool flag)
{
	if (m_privateRoomRule == nullptr)
	{
		m_privateRoomRule = PrivateRoomRule::create() ;
		m_privateRoomRule -> ignoreAnchorPointForPosition(false);
		m_privateRoomRule -> setAnchorPoint(Point(0.5,0.5));
		m_privateRoomRule->setPosition(GameData::getInstance()->getGameWidth()/2,GameData::getInstance()->getGameHeight()/2);
		this->addChild(m_privateRoomRule) ;
	}
	m_privateRoomRule->setVisible(flag);
}


void VipPrivateNormal::updateJoinRommBtShow(bool flag)
{
	setJoinRoomButtonEnable(flag) ;
}


void VipPrivateNormal::updateCurPrivateRoomInfo(DefineRoomInfo pDefineOpenRoomInfo)
{
	m_globaPrivateRoomInfo.clean();
	m_globaPrivateRoomInfo.m_Type = pDefineOpenRoomInfo.m_uType ;
	m_globaPrivateRoomInfo.m_playerNum = pDefineOpenRoomInfo.m_uMaxPlayers ;
	m_globaPrivateRoomInfo.m_ownerName = pDefineOpenRoomInfo.m_strOwnerName ;
	m_globaPrivateRoomInfo.m_roomName = pDefineOpenRoomInfo.m_strRoomName ;
	m_globaPrivateRoomInfo.m_password = pDefineOpenRoomInfo.m_strRoomPwd ;
	m_globaPrivateRoomInfo.m_minBlind = pDefineOpenRoomInfo.m_dLittleBlind ;
	m_globaPrivateRoomInfo.m_maxBlind = pDefineOpenRoomInfo.m_dBigBlind ;
	m_globaPrivateRoomInfo.m_ante = pDefineOpenRoomInfo.m_dMustBet ;
	m_globaPrivateRoomInfo.m_minCarry = pDefineOpenRoomInfo.m_dMinCarry ;
	m_globaPrivateRoomInfo.m_maxCarry = pDefineOpenRoomInfo.m_dMaxCarry ;

	m_globaPrivateRoomInfo.m_fundP = pDefineOpenRoomInfo.m_fundP ;
	m_globaPrivateRoomInfo.m_minCollect = pDefineOpenRoomInfo.m_minCollect ;
	m_globaPrivateRoomInfo.m_maxCollect = pDefineOpenRoomInfo.m_maxCollect ;

	m_globaPrivateRoomInfo.m_roomid = pDefineOpenRoomInfo.m_uAreaID ;
	m_globaPrivateRoomInfo.m_curCollectFund = 0 ;
	m_globaPrivateRoomInfo.m_isOwner = pDefineOpenRoomInfo.m_isMine ;
	m_globaPrivateRoomInfo.m_isGameState = pDefineOpenRoomInfo.m_state ;
	m_globaPrivateRoomInfo.m_hasGetData = true ;
}



void VipPrivateNormal::callUpdateRoomName(std::string name)
{
	showCreatRoomLayer(true) ;
	if (m_selfDefinition != nullptr)
	{
		m_selfDefinition->doUpdateRoomName(name) ;
		m_selfDefinition->setCurRoomType(m_curTypeIndex) ;
	}
}

void VipPrivateNormal::updateFundView(int index)
{
	if (m_selfFund != nullptr)
	{
		m_selfFund->updateView(index) ;
	}
	
}

void VipPrivateNormal::doRoomTypeSelect(unsigned index)
{
	m_curTypeIndex = index ;
	m_btPerosonalRoom->setBright(index!=0) ;
	m_btClubRoomRoom->setBright(index==0) ;
	m_textPersonal->setTextColor(0==index?Color4B(0xff,0xff,0xff,0xff):Color4B(0x37,0xa4,0x5c,0xff)) ;
	m_textClub->setTextColor(0!=index?Color4B(0xff,0xff,0xff,0xff):Color4B(0x37,0xa4,0x5c,0xff)) ;

	m_pBtSelfFund->setVisible(index ==0) ;
	m_pBtClubFund->setVisible(index ==1) ;

	m_imgeditbox->setVisible(index ==0) ;
	updateRoomView(index) ;
}