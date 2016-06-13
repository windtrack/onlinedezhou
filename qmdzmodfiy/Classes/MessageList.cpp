#include "MessageList.h"
#include "GrabTouch.h"
#include "FrameSprite.h"
#include "GameCommon.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "FrameScale9SpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemFrameSprite.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "GameHall.h"
#include "GameData.h"
#include "LineLabel.h"
#include "Tools.h"
#include "LogonLayerEx.h"

USING_NS_CC;

enum SrcollView	//	设置默认的滚动视图的大小
{
	ListScrollW = 716,
	ListScrollH = 440
};


enum MessageButtonType // 消息按钮类型
{
	EMESSAGEMAIL,	 // 邮件
	ESYSTEMMESSAGE,	 // 系统消息
	FAMILYMESSAGE,	 // 俱乐部消息
	ACTIVATEMESSAGE, // 活动消息
};

//---------------------------------------------------------------------------------------------------------------
void MessageList::UpdateView(unsigned uMessageType) // 0邮件消息 1系统消息
{
	//if (uMessageType == FAMILYMESSAGE)
	//{
	//	autoLoadNewItem() ;
	//	return ;
	//}


	ClearScrollView(uMessageType);
	unsigned uMessage = g_globalMessageInfo.m_vecMessageInfo[uMessageType].size();			// 日常任务的数量
	unsigned uAlluMessage = uMessage;												// 所有任务的数量
	unsigned uItemH = 110;																// 滚动视图的单个高度 
	unsigned uAllItemH = uItemH * uAlluMessage;											// 所有滚动视图的高度
	float fX = ListScrollW / 2, fY = uItemH / 2;
	if (uAllItemH < ListScrollH)
	{
		fY = ListScrollH - uAllItemH + uItemH / 2;
		uAllItemH = ListScrollH;		
	}

	/*for(auto& mission : g_globalMessageInfo.m_vecMessageInfo[uMessageType])
	{*/
	// 倒序遍历
	auto&& v = g_globalMessageInfo.m_vecMessageInfo[uMessageType];
	for(auto mission = v.rbegin(); mission != v.rend(); mission++ )
	{
		auto item = GetListItem(mission->m_uType-1,mission->m_eventType);
		UpdateListItemView(item, *mission);
		item->m_pBgItem->setPosition(fX, fY);
		item->m_pBgItem->setVisible(true);
		m_vecViewListItemNew[uMessageType].push_back(item);
		fY += uItemH;
	}

	float temp = uAllItemH - ListScrollH;

	//每次都显示滚动框的最上面
	m_pScrollView[uMessageType]->setContentOffset(Vec2(0,-temp-10), false);
	m_pScrollView[uMessageType]->setContentSize(Size(ListScrollW, uAllItemH));


	setShowDataLoading(false) ;
}

//---------------------------------------------------------------------------------------------------------------
bool MessageList::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}
	s_dataLoading = nullptr ;
	loadIndex = 0 ;
	curSize = 0 ;
	preSize = 0 ;


	Size sz = this->getContentSize() ;

	m_layerTop = GameLayer::create(sz.width, 143, false, false);
	m_layerTop->setPosition(Vec2(0, 497));
	this->addChild(m_layerTop, 1);



	m_layerBottom = GameLayer::create(sz.width, 10, false, false);
	m_layerBottom->setPosition(Vec2(0, 0));
	this->addChild(m_layerBottom, 1);


	auto title = CreateNode1AndPosAddTo<Sprite>("messagelist/title.png", 470, 568, this, 0);

	// 邮件消息
	m_pBt[EMESSAGEMAIL] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(MessageList::ActivateTo, this, EMESSAGEMAIL));
	m_pBt[EMESSAGEMAIL]->setAnchorPoint(Vec2(0,0));
	m_pBt[EMESSAGEMAIL]->setPosition(24,434);
	m_pBt[EMESSAGEMAIL]->setEnabled(false);
	auto menuMessaheMail = Menu::create(m_pBt[EMESSAGEMAIL],nullptr);
	menuMessaheMail->setPosition(Vec2::ZERO);
	addChild(menuMessaheMail);
	auto todayWorklabel = CreateLabelMSYHBD(24,"邮件");
	m_pBt[EMESSAGEMAIL]->SetLabel(todayWorklabel);
	m_pBt[EMESSAGEMAIL]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pBt[EMESSAGEMAIL]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	// 系统消息
	m_pBt[ESYSTEMMESSAGE] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(MessageList::ActivateTo, this, ESYSTEMMESSAGE));
	m_pBt[ESYSTEMMESSAGE]->setAnchorPoint(Vec2(0,0));
	m_pBt[ESYSTEMMESSAGE]->setPosition(24,366);
	m_pBt[ESYSTEMMESSAGE]->setEnabled(true);
	auto menuSystemMessage = Menu::create(m_pBt[ESYSTEMMESSAGE],nullptr);
	menuSystemMessage->setPosition(Vec2::ZERO);
	addChild(menuSystemMessage);
	auto nMail = CreateLabelMSYHBD(24,"系统消息");
	m_pBt[ESYSTEMMESSAGE]->SetLabel(nMail);
	m_pBt[ESYSTEMMESSAGE]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pBt[ESYSTEMMESSAGE]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));



	// 俱乐部
	m_pBt[FAMILYMESSAGE] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(MessageList::ActivateTo, this,FAMILYMESSAGE));
	m_pBt[FAMILYMESSAGE]->setAnchorPoint(Vec2(0,0));
	m_pBt[FAMILYMESSAGE]->setPosition(24,298);
	m_pBt[FAMILYMESSAGE]->setEnabled(true);

	auto menuFamilyMessage = Menu::create(m_pBt[FAMILYMESSAGE],nullptr);
	menuFamilyMessage->setPosition(Vec2::ZERO);
	addChild(menuFamilyMessage);
	auto nFamily = CreateLabelMSYHBD(24,"俱乐部消息");
	m_pBt[FAMILYMESSAGE]->SetLabel(nFamily);
	m_pBt[FAMILYMESSAGE]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pBt[FAMILYMESSAGE]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	// 活动消息
	m_pBt[ACTIVATEMESSAGE] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png",3,CC_CALLBACK_1(MessageList::ActivateTo, this, ACTIVATEMESSAGE));
	m_pBt[ACTIVATEMESSAGE]->setAnchorPoint(Vec2(0,0));
	m_pBt[ACTIVATEMESSAGE]->setPosition(24,230);
	m_pBt[ACTIVATEMESSAGE]->setEnabled(true);
	auto menuActivateMessage = Menu::create(m_pBt[ACTIVATEMESSAGE],nullptr);
	menuActivateMessage->setPosition(Vec2::ZERO);
	addChild(menuActivateMessage);
	auto activateWorklabel = CreateLabelMSYHBD(24,"活动消息");
	m_pBt[ACTIVATEMESSAGE]->SetLabel(activateWorklabel);
	m_pBt[ACTIVATEMESSAGE]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pBt[ACTIVATEMESSAGE]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));



	// 邮件的滚动View
	m_pScrollView[EMESSAGEMAIL] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 200, 60, this, 0);
	m_pScrollView[EMESSAGEMAIL]->setContentSize(Size(ListScrollW, ListScrollH));
	m_pScrollView[EMESSAGEMAIL]->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView[EMESSAGEMAIL]->setVisible(false);
	// 系统消息的滚动View
	m_pScrollView[ESYSTEMMESSAGE] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 200, 60, this, 0);
	m_pScrollView[ESYSTEMMESSAGE]->setContentSize(Size(ListScrollW, ListScrollH));
	m_pScrollView[ESYSTEMMESSAGE]->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView[ESYSTEMMESSAGE]->setVisible(true);

	//俱乐部消息
	m_pScrollView[FAMILYMESSAGE] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 200, 60, this, 0);
	m_pScrollView[FAMILYMESSAGE]->setContentSize(Size(ListScrollW, ListScrollH));
	m_pScrollView[FAMILYMESSAGE]->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView[FAMILYMESSAGE]->setVisible(true);
	m_pScrollView[FAMILYMESSAGE]->setDelegate(this);//看这里！！

	// 活动消息的滚动View
	m_pScrollView[ACTIVATEMESSAGE] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW, ListScrollH), 200, 60, this, 0);
	m_pScrollView[ACTIVATEMESSAGE]->setContentSize(Size(ListScrollW, ListScrollH));
	m_pScrollView[ACTIVATEMESSAGE]->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView[ACTIVATEMESSAGE]->setVisible(true);



	m_newFamilyMessageTips=Sprite::create("inviteflag.png") ;
	m_pBt[FAMILYMESSAGE]->addChild(m_newFamilyMessageTips);
    m_newFamilyMessageTips->setVisible(false);

	m_newFamilyMessageTips->setPosition(175,53);
	for (auto& item : m_bHasRequest)
	{
		item = false;
	}
	m_uMessageType = 0;
	return true;
}

void MessageList::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

	if (view == m_pScrollView[FAMILYMESSAGE])
	{
	
	}
}

//---------------------------------------------------------------------------------------------------------------
void MessageList::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		ActivateTo(this, m_uMessageType);
	}
	else
	{
		for (auto& item : m_bHasRequest)
		{
			item = false;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
void MessageList::ActivateTo(Ref* pSender, unsigned uMessageType)
{

	if (!m_bHasRequest[uMessageType])
	{
			
		Json::Value param;
		param["type"] = uMessageType;
		SendClientRequest(ClientRequest::kGetMessageInfo, param);

		m_bHasRequest[uMessageType] = true;

		if (uMessageType == FAMILYMESSAGE||uMessageType == ESYSTEMMESSAGE)
		{
			setShowDataLoading(true) ;
		}
	}

	for (int i=0;i<4;i++)
	{
		m_pScrollView[i]->setVisible(i==uMessageType);
		m_pBt[i]->setEnabled(i!=uMessageType);
	}


	m_uMessageType = uMessageType;
}

//---------------------------------------------------------------------------------------------------------------
void MessageList::ClearScrollView(unsigned uMessageType)
{
	for (auto item : m_vecViewListItemNew[uMessageType])
	{
		item->m_pBgItem->setVisible(false);
		m_vecViewListItemOld[uMessageType].push_back(item);
	}
	m_vecViewListItemNew[uMessageType].clear();

	if (uMessageType == FAMILYMESSAGE)
	{
		loadIndex = 0 ;
		curSize = 0 ;
		preSize = 0 ;
	}
}

//---------------------------------------------------------------------------------------------------------------
MessageList::ListItem* MessageList::GetListItem(unsigned uMessageType,unsigned eventType)
{
	ListItem *item = nullptr;
	auto szCur = m_pScrollView[uMessageType]->getContentSize();
	if (m_vecViewListItemOld[uMessageType].empty())	//判断OLD的容器数据是否为空 
	{
		HttpSprite * pTouXiang;
		FrameScale9SpriteButton* btDeal ;
		LayerButton* btHeadIcon;
		Label* labelDeal ;

		Label* labelUname ;
		Label* labelFname ;
		LineLabel* linlabelMessage ;
		LineLabel* labelSysEmailMessage ;

	

		auto labelAnchor = Vec2(0, 0);
		int labelSize = 24;
		int labelFenLeiSize = 22;
		auto labelClr = Color4B::WHITE;
		auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);

		// 加载滚动视图行的背景	
		auto bgItem = CreateNode1AndPosAddTo<Sprite>("messagelist/bgitem.png", 358, 239, m_pScrollView[uMessageType], 0);
		if(uMessageType == 0)
		{
			// 显示邮件图标左精灵图片
			m_pSpriteImage = CreateNode1AndPosAddTo<Sprite>("messagelist/messageicon.png",42, 54, bgItem, 0);
		}
		else if(uMessageType == 1)
		{
			// 显示系统图标左精灵图片
			m_pSpriteImage1 = CreateNode1AndPosAddTo<Sprite>("messagelist/systemicon.png",42, 54, bgItem, 0);
		}
		else if(uMessageType == 3)
		{
			// 显示系统图标左精灵图片
			m_pSpriteImage3 = CreateNode1AndPosAddTo<Sprite>("messagelist/systemicon.png",42, 54, bgItem, 0);
		}
		// 显示消息内容Label 
		if(uMessageType != 3)
		{
			nMessageNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 84, 40, Color4B::YELLOW, bgItem, 0);
		}
		else
		{
			nMessageNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 88, 15, Color4B::YELLOW, bgItem, 0);			
			nMessageNamelabel->setLineBreakWithoutSpace(true);
			nMessageNamelabel->setHorizontalAlignment(TextHAlignment::LEFT);
			nMessageNamelabel->setWidth(440);
		}


		Text* pTextContent = Text::create("","fzlt.ttf",22) ;
		pTextContent->ignoreContentAdaptWithSize(true) ;
		pTextContent->setContentSize(Size(465,50)) ;
		pTextContent->setSize(Size(465,60)) ;

		pTextContent->setPosition(Point(84, 40));
		pTextContent->setAnchorPoint(labelAnchor) ;
		pTextContent->setTextColor(Color4B::YELLOW) ;
		bgItem->addChild(pTextContent) ;

		// 显示乐币的数量
		//auto nLeBeCount = CreateLabelMSYHAndAnchorPosClrAddTo(20, "",labelAnchor, 450, 32, Color4B::YELLOW, bgItem, 0);
		//	游戏进行中右精灵按钮图片
		labelSysEmailMessage = LineLabel::create();
		labelSysEmailMessage->setAnchorPoint(labelAnchor);
		labelSysEmailMessage->setPosition(72,52) ;
		bgItem->addChild(labelSysEmailMessage) ;

        auto labelReward = LineLabel::create();
        labelReward->setAnchorPoint(labelAnchor);
        labelReward->setPosition(72,28);
        bgItem->addChild(labelReward);

		nMessageNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 125, 40, Color4B::WHITE, bgItem, 0);
		nMessageNamelabel->setVisible(false) ;


		auto nMessage = CreateNode1AndPosAddTo<Sprite>("messagelist/messagelist.png",621, 53, bgItem, 0);		// 滚动视图任务按钮的状态
		if(uMessageType == EMESSAGEMAIL)		// 邮件
		{
				//分类型
			//time	VIP ID	德州币	Num	您于time开通了VIP ID，获赠一次性奖励德州币Num	邮件消息
			//	time	VIP ID	德州币	Num	您当前是VIP ID，获赠每日德州币返还Num	邮件消息
			//	time	赛事ID	奖励物ID	Num	您参与的赛事ID已经结束，获得奖励物ID Num	邮件消息
			//	time		奖励物ID	Num	版本更新礼包，请笑纳	邮件消息

			// 显示时间
			m_pLabelMessageDay = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5, 0.5), 55, 38, Color4B(0x79,0xdc,0x98,255), nMessage, 0);//显示日期
		}
		else if(uMessageType == ESYSTEMMESSAGE) // 系统消息
		{
			//分类型
	/*			time	JewelID	德州币	Num	您于time购买了珠宝JewelID，获得德州币Num	系统消息
				time	VIP ID	德州币	Num	您于time开通了VIP ID，成功领取德州币Num	系统消息
				time	VIP ID	德州币	Num	您于time成功领取了VIP ID每日奖励，德州币Num	系统消息
				time	表情包ID	剩余天数	Num	您于time购买了表情包ID，剩余天数Num	系统消息
				time	任务ID	奖励物ID	Num	您于time完成了任务ID，获得奖励物ID Num	系统消息		如果奖励物不止一个，显示：获得奖励物A Num_A，奖励物B Num_B，奖励物C Num_C……
				time	赛事ID			您于time报名了赛事ID	系统消息
				time	奖励物ID	Num	您成功领取了版本更新礼包，获得奖励物ID Num	系统消息   */



			// 显示时间
			m_pLabelMessageTime = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5, 0.5), 55, 38, Color4B(0x79,0xdc,0x98,255), nMessage, 0);//显示时间
		}
		else if(uMessageType == FAMILYMESSAGE) // 俱乐部消息
		{
			linlabelMessage = LineLabel::create();
			linlabelMessage->setAnchorPoint(labelAnchor);
			linlabelMessage->setPosition(125,40) ;
			bgItem->addChild(linlabelMessage) ;

			nMessageNamelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(22 , "",labelAnchor, 125, 40, Color4B::WHITE, bgItem, 0);
			nMessageNamelabel->setVisible(false) ;

			labelUname =    CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),0,0,Color4B(0xff,0xd2,0x00,0xff),bgItem,0);

			labelFname =  CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),0,0,Color4B(0xff,0xd2,0x00,0xff),bgItem,0);

			// 显示时间
			m_pLabelMessageDay = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5, 0.5), 526, 25, Color4B(0x79,0xdc,0x98,255), bgItem, 0);//显示日期
			//nMessageNamelabel->setPosition(125,40) ;
			//std::string  str_eventType[12] =
			//{
			//	"您领取了俱乐部福利%d币" ,//0
			//	"%s申请加入您的俱乐部",//1 需要处理
			//	"您被%s逐出了%s俱乐部" ,//2
			//	"%s俱乐部已经通过你的加入申请" ,//3
			//	"%s俱乐部拒绝了您的加入申请" ,//4
			//	"%s邀请您加入%s俱乐部" ,//需要处理 5
			//	"您被%s任命为副族长" ,//6
			//	"您被%s撤销了副族长" ,//7
			//	"%s将%s俱乐部转让与您，您现在为新族长" ,//8
			//	"%s拒绝加入您的俱乐部" ,//9
			//	"%s同意加入您的俱乐部" ,//10
			//	"%s退出了俱乐部",//11
			//};
	
			if (eventType==0  || eventType==2 || eventType==3 || eventType==4 || eventType==5 || eventType==6 || eventType==7 || eventType==8)
			{
				 pTouXiang = CreateUserHeadSprite("family/defaulticon.png", "family/iconclipmask.png", bgItem, Vec2(18,7), "family/gf_family_icon.png");
			}
			if ( eventType==1||eventType==9 || eventType==10 || eventType==11|| eventType==12 )
			{
				 pTouXiang = CreateUserHeadSprite("defaulticon.png", "family/iconclipmask.png", bgItem, Vec2(18,7), "family/gf_family_icon.png");
			}

				
            //btDeal = FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn2.png",false,true);
			btDeal = FrameScale9SpriteButton::createWithScale9Sprite(2, Size(114, 86), false,true);
			labelDeal = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"处理",Vec2(0.5,0.5),0,0,Color4B::WHITE,btDeal,0);
			btDeal->setPosition(623,53);
			bgItem->addChild(btDeal);

			if (eventType ==1 || eventType ==5)
			{
				btDeal->setVisible(true) ;
			}
			else
			{
				btDeal->setVisible(false) ;
			}
			btHeadIcon = LayerButton::create(false, true);
			pTouXiang->addChild(btHeadIcon);
			btHeadIcon->setContentSize(pTouXiang->getContentSize()) ;
			//m_btDeal
		}
		else if(uMessageType == ACTIVATEMESSAGE) // 活动消息
		{
			nMessageNamelabel->setVisible(true);
			nMessageNamelabel->setAlignment(TextHAlignment::LEFT);
			nMessageNamelabel->setWidth(450);
			nMessageNamelabel->setPosition(Vec2(125, 50));
			nMessageNamelabel->setAnchorPoint(Vec2(0, 0.5));
			// 显示时间
			m_pLabelMessageActivateTime = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5, 0.5), 55, 38, Color4B(0x79,0xdc,0x98,255), nMessage, 0);//显示时间
		}


		auto pItem = new ListItem;
		pItem->m_pBgItem		= bgItem;				// 背景滚动视图行的背景
		pItem->m_pSpriteIcon	= m_pSpriteImage;		// 显示邮件图标左精灵图片
		pItem->m_pSpriteIcon1	= m_pSpriteImage1;		// 显示邮件图标左精灵图片
		pItem->m_pLabelContent	= nMessageNamelabel;	// 显示任务内容上Label
		//pItem->m_pLebeCount		= nLeBeCount;			// 显示乐币的数量
		pItem->m_pLabelDay		= m_pLabelMessageDay;	// 显示邮件的时间
		pItem->m_pLabelDate	    = m_pLabelMessageTime;  // 显示系统的时间
		pItem->m_pLabelActivate	    = m_pLabelMessageActivateTime;     // 显示活动消息时间
		pItem->m_pMessageButton	= nMessage;				// 显示时间的精灵图片
		pItem->m_pTextContent = pTextContent ;
		pItem->m_messageLabel = labelSysEmailMessage;
        pItem->m_labelReward = labelReward;
		if(uMessageType == FAMILYMESSAGE)
		{
			pItem->m_httpIcon = pTouXiang ;
			pItem->m_btDeal = btDeal ;
			pItem->m_btHeadIcon = btHeadIcon ;
			pItem->m_labeldealText = labelDeal;
			pItem->m_labelBtUname = labelUname ;
			pItem->m_labelBtFname = labelFname ;
			pItem->m_messageLabel = linlabelMessage ;
		}


		//俱乐部消息

		item = pItem;
	}
	else
	{
		item = m_vecViewListItemOld[uMessageType].back();
		m_vecViewListItemOld[uMessageType].pop_back();
	}
	return item;
}

//---------------------------------------------------------------------------------------------------------------
void MessageList::UpdateListItemView(MessageList::ListItem *item, const MessageInfo& messgeInfo)
{

    item->m_labelReward->setVisible(false);//拆分原先消息显示

	item->m_UID = messgeInfo.m_uID ;
	if(messgeInfo.m_uType == 1)			// 显示邮件的日期
	{
		// 显示乐币的数量
		char szText[256] = {0};
		sprintf(szText, "您于%s完成了%s任务，获得%s德州币",messgeInfo.m_strTime.c_str(),messgeInfo.m_strContent.c_str(),FormatCash(messgeInfo.m_dReward).c_str());
		//SetLabelStringWithBoundWidth(item->m_pLabelContent,szText, 465);
		//SetLabelString(item->m_pLabelDay, messgeInfo.m_strDate);
		item->m_messageLabel->resetLable() ;
		item->m_messageLabel->Appand(22,"fzlt.ttf","您于",Color4B::WHITE);
		item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_strTime.c_str(),Color4B(0xff,0xd2,0x00,0xff));
		item->m_messageLabel->Appand(22,"fzlt.ttf","完成了",Color4B::WHITE);
		item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_strContent.c_str(),Color4B(0xff,0xd2,0x00,0xff));
        unsigned contentLen = messgeInfo.m_strContent.size();
        if (contentLen > 25)
        {
            item->m_messageLabel->setPositionY(52);
            item->m_labelReward->setVisible(true);
            item->m_labelReward->resetLable();
            item->m_labelReward->Appand(22,"fzlt.ttf","获得",Color4B::WHITE);
            item->m_labelReward->Appand(22,"fzlt.ttf",FormatCash(messgeInfo.m_dReward),Color4B(0xff,0xd2,0x00,0xff));
            item->m_labelReward->Appand(22,"fzlt.ttf","德州币",Color4B::WHITE);
        }
        else
        {
            item->m_messageLabel->setPositionY(40);
            item->m_labelReward->setVisible(false);
            item->m_messageLabel->Appand(22,"fzlt.ttf","获得",Color4B::WHITE);
            item->m_messageLabel->Appand(22,"fzlt.ttf",FormatCash(messgeInfo.m_dReward),Color4B(0xff,0xd2,0x00,0xff));
            item->m_messageLabel->Appand(22,"fzlt.ttf","德州币",Color4B::WHITE);
        }

		SetLabelString(item->m_pLabelDay, messgeInfo.m_strDate);
        //item->m_btDeal->setVisible(true) ;
        //item->m_btDeal->SetEnable(true);
        //item->m_btDeal->SetHitCB([messgeInfo](Ref*){
        //    Json::Value param;
        //    param["missionDrawID"] = messgeInfo.m_uID ;
        //    SendClientRequest(ClientRequest::kDrawAward, param);
        //});
	}
	else if(messgeInfo.m_uType == 2)		// 显示系统的时间
	{
		// 显示乐币的数量
		/*char szText[256] = {0};
		sprintf(szText, "您于%s完成了%s任务，获得%s德州币",messgeInfo.m_strTime.c_str(),messgeInfo.m_strContent.c_str(),FormatCash(messgeInfo.m_dReward).c_str());
		SetLabelStringWithBoundWidth(item->m_pLabelContent,szText, 465);*/
		item->m_messageLabel->resetLable() ;
		item->m_messageLabel->Appand(22,"fzlt.ttf","您于",Color4B::WHITE);
		item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_strTime.c_str(),Color4B(0xff,0xd2,0x00,0xff));
		item->m_messageLabel->Appand(22,"fzlt.ttf","完成了",Color4B::WHITE);
		item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_strContent.c_str(),Color4B(0xff,0xd2,0x00,0xff));
        unsigned contentLen = messgeInfo.m_strContent.size();
        if (contentLen > 25)
        {
            item->m_messageLabel->setPositionY(52);
            item->m_labelReward->setVisible(true);
            item->m_labelReward->resetLable();
            item->m_labelReward->Appand(22,"fzlt.ttf","获得",Color4B::WHITE);
            item->m_labelReward->Appand(22,"fzlt.ttf",FormatCash(messgeInfo.m_dReward),Color4B(0xff,0xd2,0x00,0xff));
            item->m_labelReward->Appand(22,"fzlt.ttf","德州币",Color4B::WHITE);
        }
        else
        {
            item->m_messageLabel->setPositionY(40);
            item->m_labelReward->setVisible(false);
            item->m_messageLabel->Appand(22,"fzlt.ttf","获得",Color4B::WHITE);
            item->m_messageLabel->Appand(22,"fzlt.ttf",FormatCash(messgeInfo.m_dReward),Color4B(0xff,0xd2,0x00,0xff));
            item->m_messageLabel->Appand(22,"fzlt.ttf","德州币",Color4B::WHITE);
        }

		

		SetLabelString(item->m_pLabelDate, messgeInfo.m_strDate);
	}
	else if(messgeInfo.m_uType == 3)		
	{
        item->m_messageLabel->setPositionY(40);
		std::string strTime = CalculateTime(messgeInfo.m_strLastTime);
		SetLabelString(item->m_pLabelDay, strTime.c_str());
		//std::string  str_eventType[13] =
		//{
		//	"您领取了俱乐部福利%d币" ,//0
		//	 "%s申请加入您的俱乐部",//1 需要处理
		//	"您被%s逐出了%s俱乐部" ,//2
		//	"%s俱乐部已经通过你的加入申请" ,//3
		//	"%s俱乐部拒绝了您的加入申请" ,//4
		//	"%s邀请您加入%s俱乐部" ,//需要处理 5
		//	"您被%s任命为副族长" ,//6
		//	"您被%s撤销了副族长" ,//7
		//	"%s将%s俱乐部转让与您，您现在为新族长" ,//8
		//	"%s拒绝加入您的俱乐部" ,//9
		//	"%s同意加入您的俱乐部" ,//10
		//	"%s退出了俱乐部",//11
		//};

		auto functionUserInfo  = [messgeInfo](Ref*){

			Json::Value param;
			param["tid"] = messgeInfo.m_userid;
			SendClientRequest(ClientRequest::kPopUserDetailInfo, param);

		} ;
		
		auto functionFamilyInfo  = [messgeInfo](Ref*){
			Json::Value param;
			param["ufamilyid"] = messgeInfo.m_familyid;
			SendClientRequest(ClientRequest::kShowTargetFamilyInfo, param);
		} ;

		item->m_hasDeal = messgeInfo.m_hasDeal ;

		int labelWidth = 465 ;
		if (messgeInfo.m_eventType ==1||messgeInfo.m_eventType ==5)
		{
			if (item->m_hasDeal == 1)
			{
				item->m_btDeal->SetEnable(false) ;
				SetLabelString(item->m_labeldealText,"已同意") ;
				item->m_labeldealText->setTextColor(Color4B(0x79,0xdc,0x98,255));
			}
			else
			if (item->m_hasDeal == 2)
			{
				item->m_btDeal->SetEnable(false) ;
				SetLabelString(item->m_labeldealText,"已拒绝") ;
				item->m_labeldealText->setTextColor(Color4B(0x79,0xdc,0x98,255));
			}
			else
			{
				item->m_btDeal->SetEnable(true) ;
				SetLabelString(item->m_labeldealText,"处理") ;
				item->m_labeldealText->setTextColor(Color4B::WHITE) ;
			}
			labelWidth= 465 ;
		}
		else
		{
			labelWidth = 565;
			item->m_btDeal->setVisible(false) ;
			item->m_pMessageButton->setVisible(false) ;
		}

		item->m_messageLabel->resetLable() ;
		char buff0[128] ;
		if (messgeInfo.m_eventType ==0)
		{
			sprintf(buff0,"您领取了俱乐部福利%s币",FormatCash(messgeInfo.m_dReward).c_str()) ;
			item->m_messageLabel->Appand(22,"fzlt.ttf","您领取了俱乐部福利",Color4B::WHITE);
			item->m_messageLabel->Appand(22,"fzlt.ttf",FormatCash(messgeInfo.m_dReward).c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","币",Color4B::WHITE);
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, 465);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
				
		}
		if (messgeInfo.m_eventType ==1)
		{
			
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","申请加入您的俱乐部",Color4B::WHITE);
			sprintf(buff0,"%s申请加入您的俱乐部",messgeInfo.m_userName.c_str()) ;
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, 465);
			SetIconUrl(item->m_httpIcon, messgeInfo.m_userIcon);
			item->m_btDeal->setVisible(true) ;
			item->m_btHeadIcon->SetHitCB(functionUserInfo);
			item->m_btDeal->SetHitCB(std::bind(&MessageList::showDealApplyFamily,this,messgeInfo.m_familyid,messgeInfo.m_userid,messgeInfo.m_uID,
				messgeInfo.m_userName.c_str(),
				messgeInfo.m_userIcon
				)) ;	

		}

		if (messgeInfo.m_eventType ==2)
		{

			if (messgeInfo.m_userName.c_str()==g_globalMyData.GetNickName())
			{
				//sprintf(buff0,"您退出了%s俱乐部",messgeInfo.m_familyName.c_str()) ;
				item->m_messageLabel->Appand(22,"fzlt.ttf","您退出了",Color4B::WHITE);
				item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_familyName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
				item->m_messageLabel->Appand(22,"fzlt.ttf","俱乐部",Color4B::WHITE);

			}
			else
			{
				//sprintf(buff0,"%s被逐出了俱乐部",messgeInfo.m_userName.c_str(),messgeInfo.m_familyName.c_str()) ;

				item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
				item->m_messageLabel->Appand(22,"fzlt.ttf","被逐出了俱乐部",Color4B::WHITE);


			}

			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
		}
		if (messgeInfo.m_eventType ==3)
		{
			sprintf(buff0,"%s俱乐部已经通过你的加入申请",messgeInfo.m_familyName.c_str()) ;
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);


			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_familyName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","俱乐部已经通过你的加入申请",Color4B::WHITE);



			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
		}
		if (messgeInfo.m_eventType ==4)
		{
			sprintf(buff0,"%s俱乐部拒绝了您的加入申请",messgeInfo.m_familyName.c_str()) ;
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_familyName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","俱乐部拒绝了您的加入申请",Color4B::WHITE);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);

		}
		if (messgeInfo.m_eventType ==5)
		{
			sprintf(buff0,"%s邀请您加入%s俱乐部",messgeInfo.m_userName.c_str(),messgeInfo.m_familyName.c_str()) ;
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","邀请您加入",Color4B::WHITE);
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_familyName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","俱乐部",Color4B::WHITE);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
			item->m_btDeal->setVisible(true) ;
			item->m_btDeal->SetHitCB( 

				[messgeInfo](Ref*){

					PopTipWithBt2("是否同意加入该俱乐部?","同意","拒绝",[messgeInfo](unsigned ubt)
					{
						if(ubt == 0)
						{
							Json::Value param;
							param["add"] = 1;
							param["mid"] =messgeInfo.m_uID ;
							SendClientRequest(ClientRequest::kDealInvateJoinFamily, param);
						}
						if(ubt == 1)
						{
							Json::Value param;
							param["user_id"] = messgeInfo.m_userid;
							param["group_id"] = messgeInfo.m_familyid;
							param["add"] = 2;
							param["mid"] =messgeInfo.m_uID ;
							SendClientRequest(ClientRequest::kDealInvateJoinFamily, param);
						}
					});
				}

			) ; 


		}
		if (messgeInfo.m_eventType ==6)
		{
			if (messgeInfo.m_userName.c_str()==g_globalMyData.GetNickName())
			{
				sprintf(buff0,"您被任命为副族长",messgeInfo.m_userName.c_str()) ;
				item->m_messageLabel->Appand(22,"fzlt.ttf","您被任命为副族长",Color4B(0xff,0xd2,0x00,0xff));
			}
			else
			{
				sprintf(buff0,"%s被任命为副族长",messgeInfo.m_userName.c_str()) ;
				item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
				item->m_messageLabel->Appand(22,"fzlt.ttf","被任命为副族长",Color4B::WHITE);
	
			}

			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
			
		}
		if (messgeInfo.m_eventType ==7)
		{

			if (messgeInfo.m_userName.c_str()==g_globalMyData.GetNickName())
			{
					sprintf(buff0,"您被撤销了副族长",messgeInfo.m_userName.c_str()) ;
					item->m_messageLabel->Appand(22,"fzlt.ttf","您被撤销了副族长",Color4B(0xff,0xd2,0x00,0xff));
			}
			else
			{
				sprintf(buff0,"%s被撤销了副族长",messgeInfo.m_userName.c_str()) ;
				item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
				item->m_messageLabel->Appand(22,"fzlt.ttf","被撤销了副族长",Color4B::WHITE);
			}
		
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
		}
		if (messgeInfo.m_eventType ==8)
		{
			if (messgeInfo.m_userName.c_str()==g_globalMyData.GetNickName())
			{
				item->m_messageLabel->Appand(22,"fzlt.ttf","您成为新的俱乐部长",Color4B(0xff,0xd2,0x00,0xff));
			}
			else
			{
				sprintf(buff0,"%s成为新的族长",messgeInfo.m_userName.c_str()) ; 
				item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
				item->m_messageLabel->Appand(22,"fzlt.ttf","成为新的族长",Color4B::WHITE);
			}
				
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			SetFamilyIconUrl(item->m_httpIcon, g_globalMyData.GetFamilySkin(messgeInfo.m_familyIcon).m_strIconUrl);
			item->m_btHeadIcon->SetHitCB(functionFamilyInfo);
		}
		if (messgeInfo.m_eventType ==9)
		{
			sprintf(buff0,"%s拒绝加入您的俱乐部",messgeInfo.m_userName.c_str()) ;
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","拒绝加入您的俱乐部",Color4B::WHITE);
			SetIconUrl(item->m_httpIcon, messgeInfo.m_userIcon);
			item->m_btHeadIcon->SetHitCB(functionUserInfo);
		}
		if (messgeInfo.m_eventType ==10)
		{
			sprintf(buff0,"%s同意加入您的俱乐部",messgeInfo.m_userName.c_str()) ;
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","同意加入您的俱乐部",Color4B::WHITE);
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			SetIconUrl(item->m_httpIcon, messgeInfo.m_userIcon);
			item->m_btHeadIcon->SetHitCB(functionUserInfo);
		}
		if (messgeInfo.m_eventType ==11)
		{
			sprintf(buff0,"%s退出了俱乐部",messgeInfo.m_userName.c_str()) ;
			item->m_messageLabel->Appand(22,"fzlt.ttf",messgeInfo.m_userName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
			item->m_messageLabel->Appand(22,"fzlt.ttf","退出了俱乐部",Color4B::WHITE);
			SetLabelStringWithBoundWidth(item->m_pLabelContent,buff0, labelWidth);
			SetIconUrl(item->m_httpIcon, messgeInfo.m_userIcon);
			item->m_btHeadIcon->SetHitCB(functionUserInfo);
		}

	}
	else if(messgeInfo.m_uType == 4)			// 显示活动消息
	{
		SetLabelString(item->m_pLabelContent, messgeInfo.m_strContent);
		SetLabelString(item->m_pLabelActivate, messgeInfo.m_strDate);
	}


}

void MessageList::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    if (bVisible)
    {
		/* auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
		GameDialog::setVisible(true);
		Tools::callScaleAction(this) ;
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
		//GameDialog::setVisible(false);
		callHideAction() ;
		ClearScrollView(0);
		ClearScrollView(1);
		ClearScrollView(2);

		GameData::getInstance()->updateCurMessageReadTime() ;
		auto pHall = dynamic_cast<GameHall*>(getParent());
		pHall->checkNewMessage() ;
		/*     auto actScale = Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this](){

		GameDialog::setVisible(false);
		ClearScrollView(0);
		ClearScrollView(1);
		ClearScrollView(2);

		GameData::getInstance()->updateCurMessageReadTime() ;
		auto pHall = dynamic_cast<GameHall*>(getParent());
		pHall->checkNewMessage() ;

		}), nullptr));*/


    }
}



void MessageList::updateMessageFamily(const MessageInfo& info)
{

	for(auto list:m_vecViewListItemNew[FAMILYMESSAGE])
	{
		if (list->m_UID == info.m_uID)
		{
			UpdateListItemView(list,info);
		}
	}

}

std::string MessageList::CalculateTime(double gameTime)
{
	char strTime[64];
	time_t time_Now = time(NULL);
	unsigned difftime = time_Now - (time_t)gameTime;
	unsigned minute = difftime/60;
	unsigned hour = difftime/60/60;
	unsigned day = difftime/60/60/24;
	unsigned week = difftime/60/60/24/7;
	unsigned month = difftime/60/60/24/7/4;
	unsigned year = difftime/60/60/24/7/4/12;

	if(year!=0){
		sprintf(strTime,"%d年前",year);
	}else if(month!=0){
		sprintf(strTime,"%d月前",month);
	}else if(week!=0){
		sprintf(strTime,"%d星期前",week);
	}else if(day!=0){
		sprintf(strTime,"%d天前",day);
	}else if(hour!=0){
		sprintf(strTime,"%d小时前",hour);
	}else if(minute!=0){
		sprintf(strTime,"%d分前",minute);
	}else{
		sprintf(strTime,"%d秒前",difftime);
	}

	return strTime;
}




void MessageList::updateShowFamilyMessageTips(bool show)
{
	m_newFamilyMessageTips->setVisible(show) ;
}

void MessageList::showDealApplyFamily(unsigned fid,unsigned uid, unsigned mid,std::string uname,std::string uicon)
{
	auto pHall = dynamic_cast<GameHall*>(getParent());
	pHall->showDealApplyJoinFamily(true,fid,uid,mid,uname,uicon) ;

}




void MessageList::autoLoadNewItem()
{


}

void MessageList::setShowDataLoading(bool flag)
{
	if (s_dataLoading == nullptr)
	{
		//s_dataLoading = DataLoading::create();
		//s_dataLoading -> ignoreAnchorPointForPosition(false);
		//s_dataLoading -> setAnchorPoint(Point(0.5,0.5));
		//s_dataLoading->setPosition(702-35-16, 275);
		//s_dataLoading->setVisible(false);
		//this->addChild(s_dataLoading, 99);
		s_dataLoading = Tools::creatDataLoading(this,517, 289,99) ;
	}
	s_dataLoading->setVisible(flag) ;

}

void MessageList::hideMessageLoading()
{
	setShowDataLoading(false);
	//updateShowFamilyMessageTips(false) ;
}