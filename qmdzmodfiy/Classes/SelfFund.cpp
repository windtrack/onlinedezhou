#include "SelfFund.h"
#include "Tools.h"
#include "Util.h"
#include "GlobalUserData.h"

USING_NS_CC ;


enum 
{
	LineWidth = 760,
	LineHeight = 34 ,
};

bool SelfFund::init()
{
	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	s_uiroot = CSLoader::createNode("res/ui/private_clubFund.csb");
	s_uiroot->ignoreAnchorPointForPosition(false);
	s_uiroot->setAnchorPoint(Point(0.5,0.5));
	s_uiroot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(s_uiroot,1);


	//初始化数据

	//cocos2d::ui::Button						*m_btControllerPerosonal[FundViewSize] ; //三个按钮
	//cocos2d::ui::Button						*m_btControllerClue[FundViewSize] ; //三个按钮


	//m_btController[0] = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot,"Button_7days")) ;
	//m_btController[0]->addTouchEventListener(CC_CALLBACK_2(SelfFund::OnMenuItemSelect, this)) ;
	//m_textbtController[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_btController[0],"Text_show_fundall")) ;

	//m_btController[1] = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot,"Button_today")) ;
	//m_btController[1]->addTouchEventListener(CC_CALLBACK_2(SelfFund::OnMenuItemSelect, this)) ;
	//m_textbtController[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_btController[1],"Text_show_fundall_0")) ;

	//m_btController[2] = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot,"Button_yestoday")) ;
	//m_btController[2]->addTouchEventListener(CC_CALLBACK_2(SelfFund::OnMenuItemSelect, this)) ;
	//m_textbtController[2] = static_cast<ui::Text*>(Tools::getWidgetByName(m_btController[2],"Text_show_fundall_0")) ;

	//m_btController[3] = static_cast<ui::Button*>(Tools::getWidgetByName(s_uiroot,"Button_clubControll")) ;
	//m_btController[3]->addTouchEventListener(CC_CALLBACK_2(SelfFund::OnMenuItemSelect, this)) ;
	//m_textbtController[3] = static_cast<ui::Text*>(Tools::getWidgetByName(m_btController[3],"Text_show_clubfund")) ;


	m_layerFund[0] =  static_cast<ui::Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_7days")) ;
	m_layerFund[1] =  static_cast<ui::Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_today")) ;
	m_layerFund[2] =  static_cast<ui::Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_yestoday")) ;
	m_layerFund[3] =  static_cast<ui::Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_clubfund")) ;


	m_text_fundAll = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[0],"Text_1_0")) ;

	m_layerView[0] = static_cast<ui::Layout*>(Tools::getWidgetByName(m_layerFund[0],"Panel_7dayblock")) ;
	m_layerView[1] = static_cast<ui::Layout*>(Tools::getWidgetByName(m_layerFund[1],"Panel_todayblock")) ;
	m_layerView[2] = static_cast<ui::Layout*>(Tools::getWidgetByName(m_layerFund[2],"Panel_yestodayblock")) ;
	m_layerView[3] = static_cast<ui::Layout*>(Tools::getWidgetByName(m_layerFund[3],"Panel_clubdayblock")) ;

	//m_textFundAllCash[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[0],"Text_7daysjijinzongliang")) ;
	//m_textFundAllCash[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[1],"Text_todayjijinzongliang")) ;
	//m_textFundAllCash[2] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[2],"Text_yestodayjijinzongliang")) ;

	m_textFundPersonalCash[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[0],"Text_7daysgerenfangzongliang")) ;
	m_textFundPersonalCash[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[1],"Text_todaygerenfangzongliang")) ;
	m_textFundPersonalCash[2] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[2],"Text_yestodaygerenfangzongliang")) ;
	m_textFundPersonalCash[3] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[3],"Text_club7daysgerenfangzongliang")) ;

	//m_textFundClubCash[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[0],"Text_7daysjulebuzongliang")) ;
	//m_textFundClubCash[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[1],"Text_todayjulebuzongliang")) ;
	//m_textFundClubCash[2] = static_cast<ui::Text*>(Tools::getWidgetByName(m_layerFund[2],"Text_yestodayjulebuzongliang")) ;


	m_layout_btController[0] =  static_cast<ui::Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_myFund")) ;
	m_layout_btController[1] =  static_cast<ui::Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_myclubfund")) ;


	const char* m_Btstr[FundViewSize] = {"Button_7days","Button_today","Button_yestoday","Button_clubControll", } ;
	for (int i=0; i<FundViewSize; i++)
	{
		m_btControllerPerosonal[0][i] = static_cast<ui::Button*>(Tools::getWidgetByName(m_layout_btController[0],m_Btstr[i])) ;
		m_textbtControllerPersonal[0][i] = static_cast<ui::Text*>(Tools::getWidgetByName(m_btControllerPerosonal[0][i],"Text_fund")) ;

		m_btControllerPerosonal[1][i] = static_cast<ui::Button*>(Tools::getWidgetByName(m_layout_btController[1],m_Btstr[i])) ;
		m_textbtControllerPersonal[1][i] = static_cast<ui::Text*>(Tools::getWidgetByName(m_btControllerPerosonal[1][i],"Text_fund")) ;
	}


	m_sizeView = m_layerView[0]->getContentSize() ;

	for (int i=0 ;i<4 ;i++)
	{
		m_scrollView[i] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(m_sizeView.width, m_sizeView.height), 0, 0, m_layerView[i], 0);
		m_scrollView[i]->setContentSize(Size(m_sizeView.width, m_sizeView.height));
		m_scrollView[i]->setDirection(extension::ScrollView::Direction::VERTICAL);
		m_scrollView[i]->setDelegate(this);
	}

	m_textNoFundInfo =  static_cast<ui::Text*>(Tools::getWidgetByName(s_uiroot,"Text_noFundInfo")) ;
	setCurFundType(0) ;
	//模拟假数据

	//for (int i=0;i<3;i++)
	//{
	//	
	//	m_globalFundData.m_clubFund = 10000000000000+rand()%100000000 ;
	//	m_globalFundData.m_personalRoomFund = 100000000000+rand()%10000000 ;
	//	m_globalFundData.m_allFund = m_globalFundData.m_clubFund + m_globalFundData.m_personalRoomFund ;

	//	for (int j=0 ;j<30; j++)
	//	{
	//		SingleFundData data;
	//		data.m_index = j ;
	//		data.m_cash = rand()%2555555 ;
	//		data.m_date = "2016.2.15  16:42" ;
	//		data.m_isPersonalRoom = rand()%2;
	//		data.m_roomName = "的接口是否";

	//		m_globalFundData.m_vecOneFundData.push_back(data);
	//	}
	//	
	//	m_globalAllFundData.m_vecAllFundData.push_back(m_globalFundData);

	//}


	
	return true ;
}

void SelfFund::callView(int index)
{
	m_curIndex = index ;
	for (int i=0 ;i<FundViewSize; i++)
	{
		m_btController[i]->setBright(i!=index) ;
		m_layerFund[i]->setVisible(i==index) ;
		m_scrollView[i]->setVisible(i==index) ;
		m_textbtController[i]->setTextColor(i==index?Color4B(0xff,0xff,0xff,0xff):Color4B(0x37,0xa4,0x5c,0xff)) ;
	}

	updateView(index) ;
}


void SelfFund::setVisible(bool bVisible)
{
	if (bVisible)
	{
		
		for (int i=0 ;i<FundViewSize ;i++)
		{
			if (m_curFundType == 0 && i==3)
			{
				continue;
			}
			Json::Value param ;
			param["kind"] = m_curFundType+1;
			param["type"] = i+1;
		
			SendClientRequest(ClientRequest::kCallPrivateFundData, param);
		}
		callShowAction() ;
	}
	else
	{
		callHideAction() ;
	}
}

void SelfFund::OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type)
{
	if (type == Button::TouchEventType::ENDED)
	{
		for (int i=0; i<FundViewSize; i++)
		{
			if (pSend == m_btController[i])
			{
				callView(i) ;
			}
		}
	}
}


SelfFund::LineFundData* SelfFund::GetLineFundDataItem(unsigned uIndex)
{

	LineFundData *item = nullptr;
	if (m_vecLineFundOld.empty())
	{

	
     	auto bgItem = Layer::create();
		bgItem->ignoreAnchorPointForPosition(true) ;
		bgItem->setAnchorPoint(Point(0,0)) ;
		bgItem->setContentSize(Size(LineWidth,LineHeight)) ;
		m_scrollView[uIndex]->addChild(bgItem) ;

		auto m_spriteColorBg = LayerColor::create(Color4B(0x0b,0x57,0x2f,0xff),LineWidth,LineHeight);
		m_spriteColorBg->ignoreAnchorPointForPosition(true) ;
		m_spriteColorBg->setAnchorPoint(Point(0,0)) ;
		m_spriteColorBg->setContentSize(Size(LineWidth,LineHeight)) ;
		bgItem->addChild(m_spriteColorBg) ;

		Size sz = bgItem->getContentSize() ;
		auto m_spritePersonal = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/fund_typepersonal.png", 258, 15, bgItem, 0);
		auto m_spriteClub = CreateNode1AndPosAddTo<Sprite>("vipprivateroom/fund_typeclub.png", 258, 15, bgItem, 0);

		auto m_labelDate = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 89, 15, Color4B(0xff,0xec,0xbf,0xff), bgItem, 0);
		auto m_labelRoomName = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0,0.5), 275, 15, Color4B(0xff,0xec,0xbf,0xff), bgItem, 0);
		auto m_labelCash = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(1,0.5), 740, 15, Color4B(0xff,0xec,0xbf,0xff), bgItem, 0);

		auto pItem = new LineFundData;

		pItem->m_spriteTableBg = bgItem;
		pItem->m_spriteColorBg = m_spriteColorBg;
		pItem->m_spritePersonal = m_spritePersonal;
		pItem->m_spriteClub = m_spriteClub;
		pItem->m_labelDate = m_labelDate;
		pItem->m_labelRoomName = m_labelRoomName;
		pItem->m_labelCash = m_labelCash;
		item = pItem;
	}
	else
	{
		item = m_vecLineFundOld.back();
		m_vecLineFundOld.pop_back();
	}

	return item;


}

void SelfFund::UpdateFundDataView(SelfFund::LineFundData* item, const SingleFundData &data,int index)
{
	char buff[64] ;
	item->m_spriteColorBg->setVisible(data.m_index%2==0) ;
	
	SetLabelString(item->m_labelDate,data.m_date.c_str()) ;
	SetLabelString(item->m_labelRoomName,data.m_roomName.c_str()) ;
	sprintf(buff,"%s",FormatCash(data.m_cash).c_str()) ;
	SetLabelString(item->m_labelCash,buff) ;
	
	item->m_spritePersonal->setVisible(data.m_isPersonalRoom) ;
	item->m_spriteClub->setVisible(!data.m_isPersonalRoom) ;

	if (index==0)
	{
		item->m_labelDate->setPosition(95+5,15);

	}
	else
	{
		item->m_labelDate->setPosition(75-5,15);
	}




}


void SelfFund::updateView(int index)
{

	for (auto newItem : m_vecLineFundNew)
	{
		newItem->m_spriteTableBg->setVisible(false);
		m_vecLineFundOld.push_back(newItem);
	}
	m_vecLineFundOld.clear();

	GlobalFunData curRoomInfo = m_globalAllFundData.m_vecAllFundData[m_curFundType][index] ;

	m_textNoFundInfo->setVisible(curRoomInfo.m_vecOneFundData.size()==0) ;

	unsigned uRoomNum = curRoomInfo.m_vecOneFundData.size();
	unsigned uOneItemH = LineHeight;//直径加间隔
	unsigned uOneItemW = LineWidth;
	unsigned uAllItemH = 0;

	uAllItemH = (uRoomNum)*uOneItemH;
	if (uAllItemH < m_sizeView.height)
	{
		uAllItemH = m_sizeView.height;
	}
	else
	{
		m_scrollView[index]->setBounceable(true);
	}


	double offSet = m_sizeView.height - (double)uAllItemH;


	double fX =0,fY = uAllItemH- uOneItemH;
	unsigned uRoomCount = 0;
	unsigned uCurPlayers = 0;

	auto&& v = curRoomInfo.m_vecOneFundData;
	for(auto dataItem = v.rbegin(); dataItem != v.rend(); dataItem++)
	{
		auto  roomItem = GetLineFundDataItem(index);

		UpdateFundDataView(roomItem, *dataItem,index);
		m_vecLineFundNew.push_back(roomItem);
		roomItem->m_spriteTableBg->setPosition(fX , fY);
		roomItem->m_spriteTableBg->setVisible(true);
		
		fY -= LineHeight;
	}

	//setUITextString(m_textFundAllCash[index],FormatCash(curRoomInfo.m_allFund)) ;
	setUITextString(m_textFundPersonalCash[index],FormatCash(curRoomInfo.m_allFund)) ;
	//setUITextString(m_textFundClubCash[index],FormatCash(curRoomInfo.m_clubFund)) ;
	


	setUITextString(m_text_fundAll,m_curFundType==0?"个人房基金总量：":"俱乐部房基金总量：") ;



	m_scrollView[index]->setContentOffset(Vec2(0, offSet), false);
	m_scrollView[index]->setContentSize(Size(m_sizeView.width, uAllItemH));



}

void SelfFund::setCurFundType(unsigned type)
{
	m_curFundType = type ;
	m_curIndex = 0 ;
	m_layout_btController[0]->setVisible(type==0) ;
	m_layout_btController[1]->setVisible(type==1) ;

	for (int i=0; i<FundViewSize; i++)
	{
		m_btController[i] = m_btControllerPerosonal[type][i] ;
		m_btController[i]->addTouchEventListener(CC_CALLBACK_2(SelfFund::OnMenuItemSelect, this)) ;
		m_textbtController[i] =  m_textbtControllerPersonal[type][i] ;
	}
	
	callView(0) ;

}

void SelfFund::updateData()
{
	updateView(m_curIndex) ;
	//callView(m_curIndex) ;
}