#include "DailySign.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "GameCommon.h"
#include "RewardDrawingAni.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "FrameSprite.h"
#include "GameConfig.h"
#include "LineLabel.h"
#include "GameFrame.h"
#include "GameTipsManager.h"
#include "Tools.h"
#include "GameData.h"
#include "MenuItemScale9SpriteWithLabel.h"
USING_NS_CC;



bool DailySign::init()
{
    if (g_globalMyData.m_continueLoginType == 1)
        return initWithTicketReward();
    else if (g_globalMyData.m_continueLoginType == 0)
        return initWithGoldReward(g_globalContinueLogin.dayNum, g_globalContinueLogin.SignStatus, g_globalContinueLogin.SignMoney);
}

bool DailySign::initWithGoldReward(unsigned size, unsigned *status, unsigned *money)
{
    if (!GameDialog::init("signboard/signbg.png", false))
    {
        return false;
    }
    char buff[30];
    for (int i=0; i<size; i++)
    {
        sprintf(buff, "signboard/day%d.png", i+1);
        m_frameSprite[i] = FrameSprite::create(buff, 1, 2);
        m_frameSprite[i]->setPosition(88 + i*168, 196);
        this->addChild(m_frameSprite[i]);
        Size szSprite = m_frameSprite[i]->getContentSize();
        sprintf(buff, "%d德州币", money[i]);
        CreateLabelMSYHBDAndAnchorPosClrAddTo(20, buff, Vec2(0.5, 0.5), szSprite.width/2, 25, Color4B(0x03,0x2e,0x10,0xff), m_frameSprite[i], 0);
        m_spriteAlreadyGet[i] = CreateNode1AndPosAddTo<Sprite>("signboard/alreadget.png",88+i*168, 176, this, 1);
        m_spriteAlreadyGet[i]->setVisible(false);

        if (status[i] == 1)
        {
            m_frameSprite[i]->SetFrameIdx(1);
            g_globalContinueLogin.SignStatus[i] = 2;
        }
        else if (status[i] == 2)
        {
            m_spriteAlreadyGet[i]->setVisible(true);
        }
    }

    m_menuImmediatelyGetGold = MenuItemScale9SpriteWithLabel::create(0, Size(203, 70), CC_CALLBACK_1(DailySign::OnImmediatelyGetGold, this));
    m_menuImmediatelyGetGold->setPosition(424, 41);
    Size szMenu = m_menuImmediatelyGetGold->getContentSize();
    m_labelGet = CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "立即领取", Vec2(0.5, 0.5), szMenu.width/2, szMenu.height/2, Color4B(0xff,0xff,0xff,0xff), m_menuImmediatelyGetGold, 0);
    auto menuGet = Menu::create(m_menuImmediatelyGetGold, nullptr);
    menuGet->setPosition(Vec2::ZERO);
    this->addChild(menuGet);

    return true;
}

bool DailySign::initWithTicketReward()
{
    if (!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }
	m_scaleSprite = nullptr ;
    auto bgSize = this->getContentSize();
    //加载每日签到.plist
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("plist/siginreward.plist", "plist/siginreward.png");
    auto titleSprite = Sprite::createWithSpriteFrameName("sign_title.png");
    titleSprite->setPosition(bgSize.width/2, bgSize.height-70);
    this->addChild(titleSprite);

	//标题
	auto m_pRichCashCenterSizelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(24 , "每周累计登陆到达以下天数，可获得相应奖励",Vec2(0.5, 0.5), bgSize.width/2, 502,
		Color4B(0x79, 0xdc,0x98,255), this, 0);
	//进度条背景
	auto m_SpritecontinusBg = Sprite::createWithSpriteFrameName("sign_progress-bg.png");
	m_SpritecontinusBg->setPosition(bgSize.width/2-20, 397);
	this->addChild(m_SpritecontinusBg);

	m_loadProgress = ProgressTimer::create(Sprite::createWithSpriteFrameName("sign_progress-bar.png"));//创建一个进程条
	m_loadProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率
	m_loadProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型
	m_loadProgress->setMidpoint(Point(0,1));//设置进度的运动方向
	m_loadProgress->setPosition(Point(bgSize.width/2-20,397));
	m_loadProgress->setPercentage(0.0f);//设置初始值为0
	this->addChild(m_loadProgress,0);

	float maxPro = (g_globalUserSigin.m_continusdays-1)*100/7.00f ;
	if (maxPro<0)
	{
		maxPro = 0 ;
	}
	m_loadProgress->setPercentage(maxPro) ;




	//连续登录
	for (int i=0;i< g_globalUserSigin.m_vecUserContinuousSingin.size(); i++)
	{
		char buff[30];
		ContinuousSingin item = g_globalUserSigin.m_vecUserContinuousSingin.at(i) ;

		if (item.m_hasItem)
		{
			//背景
			auto itemBgSprite = Node::create() ;
			itemBgSprite->setContentSize(Size(138,213)) ;
			auto itemSize = itemBgSprite->getContentSize();
			itemBgSprite->setPosition(180-20+i*111-itemSize.width/2,397-itemSize.height/2) ;
			this->addChild(itemBgSprite,1);


			//第几天
			auto slabelDay  = CreateLabelMSYH(22, "", TextHAlignment::CENTER, TextVAlignment::CENTER);
			slabelDay->setPosition(itemSize.width/2, 165);
			slabelDay->setAnchorPoint(Vec2(0.5, 0.5));
			slabelDay->setColor(Color3B(0xff, 0xd2,0x00));
			itemBgSprite->addChild(slabelDay);
			char buf[64] ;
			sprintf(buf,"累计%d天",(i+1)) ;
			SetLabelString(slabelDay, buf);

			//道具
			auto ticketSprite = HttpSprite::create("sigin_default.png");
			ticketSprite->ignoreAnchorPointForPosition(false);
			ticketSprite->setAnchorPoint(Point(0.5,0.5)) ;
			ticketSprite->useDownLoadSize() ;
			itemBgSprite->addChild(ticketSprite);
			SetHttpSpriteURL(ticketSprite, item.m_iconUrl);
			ticketSprite->setPosition(itemSize.width/2 , itemSize.height/2);
			ticketSprite->setScale(0.67) ;
			//名称
			auto ticketNameLabel  = CreateLabelMSYH(20, "", TextHAlignment::CENTER, TextVAlignment::CENTER);

			ticketNameLabel->setPosition(itemSize.width/2, 53);
			ticketNameLabel->setAnchorPoint(Vec2(0.5, 0.5));
			itemBgSprite->addChild(ticketNameLabel);

			char buff[64] ;
			sprintf(buff,"%sx%d",item.m_itemName.c_str(),item.m_count) ;
			SetLabelString(ticketNameLabel, buff);
			//SetLabelString(ticketNameLabel, item.m_itemName);
			ticketNameLabel->setColor(Color3B(0x79, 0xdc,0x98));

		}

	}

	const char* sItemBg[4] = {"sign_z-miss.png","sign_z-miss.png","sign_z-present.png","sign_z-miss.png",} ;
	//const char* sItemBg[4] = {"sign_z-miss.png","sign_z-get.png","sign_z-present.png","sign_z-miss.png",} ;
	const char* sSinginDay[7] = {"sign_day_1.png","sign_day_2.png","sign_day_3.png","sign_day_4.png","sign_day_5.png","sign_day_6.png","sign_day_7.png",} ;

	const Color3B sTextColot[4] = {
		Color3B(0x54, 0x7e,0x62),Color3B(0x54, 0x7e,0x62),Color3B(0xff, 0xd2,0x00),Color3B(0x79, 0xdc,0x98)
	} ;


	m_scaleSprite = nullptr ;
	for (int i=0;i< g_globalUserSigin.m_vecUserSigin.size(); i++)
	{
		char buff[30];
		SiginDay item = g_globalUserSigin.m_vecUserSigin.at(i) ;

		//背景
		if ((g_globalUserSigin.m_today-1)==i)
		{
			itemBgSprite[i]  = Sprite::createWithSpriteFrameName(sItemBg[2]);
		}
		else
		{
			itemBgSprite[i]  = Sprite::createWithSpriteFrameName(sItemBg[item.m_state]);
		}
		

		itemBgSprite[i]->setPosition(89+i*127,215) ;
		this->addChild(itemBgSprite[i]);

		
		

		auto itemSize = itemBgSprite[i]->getContentSize();
		//周几
		Sprite *itemDay  = Sprite::createWithSpriteFrameName(sSinginDay[i]);
		itemDay->setPosition(itemBgSprite[i]->getContentSize().width/2,185) ;
		itemBgSprite[i]->addChild(itemDay);


		//if ((g_globalUserSigin.m_today-1)==i)
		//{
		//	itemBgSprite[i]  = Sprite::createWithSpriteFrameName("sign_z-miss.png");
		//	//itemBgSpriteToday  = Sprite::createWithSpriteFrameName("sign_z-get.png");
		//	itemBgSprite[i]->setPosition(89+(g_globalUserSigin.m_today-1)*127,215) ;
		//	//itemBgSpriteToday->setVisible(false) ;
		//	//this->addChild(itemBgSpriteToday);

		//	//Sprite *itemToDay  = Sprite::createWithSpriteFrameName(sSinginDay[i]);
		//	//itemToDay->setPosition(itemBgSprite[i]->getContentSize().width/2,185) ;
		//	//itemBgSpriteToday->addChild(itemToDay) ;
		//}


		//if ((g_globalUserSigin.m_today-1)==i)
		//{
		//	itemBgSpriteToday  = Sprite::createWithSpriteFrameName("sign_z-miss.png");
		//	//itemBgSpriteToday  = Sprite::createWithSpriteFrameName("sign_z-get.png");
		//	itemBgSpriteToday->setPosition(89+(g_globalUserSigin.m_today-1)*127,215) ;
		//	//itemBgSpriteToday->setVisible(false) ;
		//	this->addChild(itemBgSpriteToday);

		//	Sprite *itemToDay  = Sprite::createWithSpriteFrameName(sSinginDay[i]);
		//	itemToDay->setPosition(itemBgSprite[i]->getContentSize().width/2,185) ;
		//	itemBgSpriteToday->addChild(itemToDay) ;
		//}


		//unsigned		m_state;//丢失  已领取  当前领取  未领取  0，1，2，3
		//if (item.m_state != 1)
		//{
			if (item.m_state==2)
			{
				m_scaleSprite = Sprite::createWithSpriteFrameName("sign_z-present-shine.png");
				m_scaleSprite->setScale(0.67) ;
				m_scaleSprite->setPosition(itemSize.width/2 , 105);
				itemBgSprite[i]->addChild(m_scaleSprite);
				
				auto scaleSprite = m_scaleSprite ;
				auto rotateAction = [scaleSprite](){
					scaleSprite->setRotation(scaleSprite->getRotation()+1) ;
				} ;
				CCFiniteTimeAction *repeat = CCRepeatForever::create(Sequence::create(CallFunc::create(rotateAction),DelayTime::create(1.0/60.0f),NULL));
			 //CCFiniteTimeAction *repeat = CCRepeat::create(Sequence::create(CallFunc::create(rotateAction),DelayTime::create(1.0/60.0f),NULL), 999999999);
				m_scaleSprite->runAction(repeat);
			}

			//道具
			auto ticketSprite = HttpSprite::create("sigin_default.png");
			ticketSprite->ignoreAnchorPointForPosition(false);
			ticketSprite->setAnchorPoint(Point(0.5,0.5)) ;
			ticketSprite->useDownLoadSize() ;
			itemBgSprite[i]->addChild(ticketSprite);
			SetHttpSpriteURL(ticketSprite, item.m_iconUrl);
			ticketSprite->setPosition(itemSize.width/2 , 105);
			ticketSprite->setScale(0.67) ;

			if (item.m_state==0 || item.m_state==1)
			{
				ticketSprite->getSprite()->setOpacity(80) ;
			}
			auto ticketNameLabel  = CreateLabelMSYH(20, "", TextHAlignment::CENTER, TextVAlignment::CENTER);
			auto ticketNumLabel  = CreateLabelMSYH(20, "", TextHAlignment::CENTER, TextVAlignment::CENTER);

			ticketNameLabel->setPosition(itemSize.width/2, 45);
			ticketNameLabel->setAnchorPoint(Vec2(0.5, 0.5));
			itemBgSprite[i]->addChild(ticketNameLabel);

			ticketNumLabel->setPosition(itemSize.width/2, 23);
			ticketNumLabel->setAnchorPoint(Vec2(0.5, 0.5));
			itemBgSprite[i]->addChild(ticketNumLabel);


			SetLabelStringWithBoundWidth(ticketNameLabel, item.m_itemName,105);
			sprintf(buff, "x %d", item.m_count);
			SetLabelString(ticketNumLabel, buff);

			ticketNameLabel->setColor(sTextColot[item.m_state]);
			ticketNumLabel->setColor(sTextColot[item.m_state]);



			m_spriteSignGet[i] =  Sprite::createWithSpriteFrameName("sign_z-get.png");
			itemBgSprite[i]->addChild(m_spriteSignGet[i]) ;

			m_spriteSignGet[i]->setPosition(itemSize.width/2,itemSize.height/2) ;

			m_spriteSignGet[i]->setVisible(item.m_state==1);
		//}

	}



	m_menuImmediatelyGetTicket = MenuItemScale9SpriteWithLabel::create(
		0,
		Size(230,204/3),
		CC_CALLBACK_1(DailySign::OnImmediatelyGetSiginReward, this));

	Size szMenu = m_menuImmediatelyGetTicket->getContentSize();


	m_menuImmediatelyGetTicket->setPosition(bgSize.width/2, szMenu.height/2+30);
	m_labelButton = CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "签到领取", Vec2(0.5, 0.5), szMenu.width/2, szMenu.height/2, Color4B(0xff,0xff,0xff,0xff), m_menuImmediatelyGetTicket, 0);
	auto menuGet = Menu::create(m_menuImmediatelyGetTicket, nullptr);
	menuGet->setPosition(Vec2::ZERO);
	this->addChild(menuGet);
	if (g_globalUserSigin.m_hasGetSiginReward)
	{
		SetLabelString(m_labelButton,"已领取") ;
		m_menuImmediatelyGetTicket->setEnabled(false) ;
	}
	
	mReward = nullptr ;

    return true;
}



void DailySign::updateView()
{
	m_labelButton->setOpacity(255) ;
	if (g_globalUserSigin.m_hasGetSiginReward)
	{
		SetLabelString(m_labelButton,"已领取") ;
		m_menuImmediatelyGetTicket->setEnabled(false) ;
		m_labelButton->setOpacity(155) ;
	}
	
	if (g_globalUserSigin.m_hasGetSiginReward)
	{
		m_spriteSignGet[g_globalUserSigin.m_today-1]->setVisible(true) ;
		if (m_scaleSprite!=nullptr)
		{
			m_scaleSprite->stopAllActions();
			m_scaleSprite->setVisible(false) ;
		}
		
		//itemBgSprite[g_globalUserSigin.m_today-1]->setVisible(false) ;
		//itemBgSpriteToday->setVisible(true) ;

	}


}



void DailySign::OnImmediatelyGetGold(cocos2d::Ref *pSend)
{
    Json::Value param;
    SendClientRequest(ClientRequest::kContinueLogOn);
    this->setVisible(false);
}

void DailySign::OnImmediatelyGetSiginReward(cocos2d::Ref *pSend)
{

    //this->setVisible(false);
	//this->removeFromParentAndCleanup(true) ;
	SendClientRequest(ClientRequest::kOptHallBgLayer);
    SendClientRequest(ClientRequest::kCallGetSiginReward);
    //SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("plist/siginreward.plist");
	
}

void DailySign::onMenuItemCloseSelected(cocos2d::Ref *)
{
	this->setVisible(false);
	GameTipsManager::getInstance()->hideTips() ;
	//this->removeFromParentAndCleanup(true) ;
	//SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("plist/siginreward.plist");

}


void DailySign::setVisible(bool bVisible)
{
	
	if (bVisible)
	{
		GameDialog::setVisible(bVisible) ;
		updateView() ;
		Size sz = this->getContentSize() ;

		/*	this->setScale(0.7f) ;
		ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);

		this->runAction(easeBounceOut);*/
		Tools::callScaleAction(this) ;
		SendClientRequest(ClientRequest::kShowHallBgLayer);

		doLoadProgressbar();
		g_globalUserSigin.m_hasShowSiginLayer = true ;
	}
	else
	{
		GameDialog::setVisible(bVisible) ;
		//showContinueReward(false) ;
		SendClientRequest(ClientRequest::kOptHallBgLayer);
		if (mReward!=nullptr &&mReward->isVisible())
		{
			mReward->setVisible(false) ;
		}
		if (g_globalUserSigin.m_hasGetSiginReward)
		{
			GameTipsManager::getInstance()->hideTips() ;
		}
		
	}
}

void DailySign::showContinueReward(bool flag)
{
	if (!this->isVisible())
	{
		return ;
	}
	bool hasItem = g_globalUserSigin.m_vecUserContinuousSingin.at(g_globalUserSigin.m_continusdays-1).m_hasItem ;
	bool hasGet = g_globalUserSigin.m_hasGetContinuonsReward ;
	if (hasItem&&!hasGet&&flag)
	{
		if (mReward==nullptr)
		{
			Size sz = this->getContentSize() ;
			mReward = ContinueSignReward::create() ;
			mReward->ignoreAnchorPointForPosition(false) ;
			mReward->setAnchorPoint(Point(0.5,0.5)) ;
			mReward->setPosition(sz.width/2,sz.height/2) ;
			this->addChild(mReward,50);
		}
		//mReward->setVisible(flag) ;
		if (flag)
		{
			SendClientRequest(ClientRequest::kCallGetContinuesSiginReward);
		}
		
		g_globalUserSigin.m_hasGetContinuonsReward = true ;

	}
	if (mReward!=nullptr)
	{
		mReward->setVisible(flag) ;
	}



}

void DailySign::callContinousSiginReward()
{
	if (g_globalUserSigin.m_continusdays<=0)
	{
		return ;
	}

	auto showAction = [this](){

		bool flag = g_globalUserSigin.m_vecUserContinuousSingin.at(g_globalUserSigin.m_continusdays-1).m_hasItem ;
		bool hasGet = g_globalUserSigin.m_hasGetContinuonsReward ;
		if (flag&&!hasGet)
		{
			showContinueReward(true) ;
			SendClientRequest(ClientRequest::kCallGetContinuesSiginReward);
			g_globalUserSigin.m_hasGetContinuonsReward = true ;

		}
		

	} ;

	this->runAction(Sequence::create(DelayTime::create(0.3f),CallFunc::create(showAction),NULL));
}

void DailySign::doLoadProgressbar()
{

	float premaxPro = (g_globalUserSigin.m_continusdays-1)*100/7.00f ;
	if (premaxPro<0)
	{
		premaxPro = 0 ;
	}
	m_loadProgress->setPercentage(premaxPro) ;



	float maxPro = g_globalUserSigin.m_continusdays*100/7.00f ;

	auto progressBar = m_loadProgress ;
	auto loadAction = [this,progressBar,maxPro](){

		float curPro = progressBar->getPercentage() ;
		if (curPro+1<maxPro)
		{
			curPro+=0.5 ;
		}
		else
		{
			curPro = maxPro ;	
			progressBar->stopAllActions() ;
			callContinousSiginReward() ;
		}
		progressBar->setPercentage(curPro) ;

	} ;

	CCFiniteTimeAction *repeat = CCRepeat::create(Sequence::create(CallFunc::create(loadAction),DelayTime::create(1.0/60.0f),NULL), 9999);

	m_loadProgress->runAction(repeat);
}

void DailySign::onGrabTouchBegan(bool bHitDialog)
{
	if (bHitDialog)
	{
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DailySignReward::init()
{
    if (!GameDialog::init("userinfo/bgdialog0.png", false))
    {
        return false;
    }
	auto szBg = getContentSize();


	m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(DailySignReward::onMenuItemCloseSelected, this));
	m_btMenuItemClose->setPosition(szBg.width - 25, szBg.height - 28);
	auto menu = Menu::create(m_btMenuItemClose, nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu, 100);

    auto titleSprite = Sprite::createWithSpriteFrameName("sigingetrewardtitle.png");
    titleSprite->setPosition(szBg.width/2, szBg.height-8);
    this->addChild(titleSprite);

    shineSprite = Sprite::createWithSpriteFrameName("sigin_shine.png");
    shineSprite->setPosition(szBg.width/2, szBg.height/2);
    this->addChild(shineSprite);

	auto scaleSprite = shineSprite ;
	auto rotateAction = [scaleSprite](){
		scaleSprite->setRotation(scaleSprite->getRotation()+1) ;
	} ;
	CCFiniteTimeAction *repeat = CCRepeatForever::create(Sequence::create(CallFunc::create(rotateAction),DelayTime::create(1.0/60.0f),NULL));

	

    ticketSprite = HttpSprite::create("sigin_default.png");
    auto httpSpriteSz = ticketSprite->getContentSize();
    ticketSprite->setPosition(szBg.width/2 , szBg.height/2);
    addChild(ticketSprite);

	Size sz = this->getContentSize() ;
	m_messageLabel  = LineLabel::create() ;
	//m_messageLabel->Appand(28,"fzlt.ttf","获得",Color4B::WHITE);
	//m_messageLabel->Appand(28,"fzlt.ttf",buff,Color4B(0xff,0xd2,0x00,0xff));
	m_messageLabel->setPosition(sz.width/2,sz.height/2+70) ;
	this->addChild(m_messageLabel);

    //m_menuCheck = MenuItemSpriteWithLabel::create("update/btcancel.png", 3, CC_CALLBACK_1(DailySignReward::OnImmediatelyCheck, this));
    m_menuCheck = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(DailySignReward::OnImmediatelyCheck, this));
    Size szMenu = m_menuCheck->getContentSize();
    m_menuCheck->setPosition(szBg.width/2, szMenu.height/2+18);
    CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "查看", Vec2(0.5, 0.5), szMenu.width/2, szMenu.height/2, Color4B(0xff,0xff,0xff,0xff), m_menuCheck, 0);
    auto menuGet = Menu::create(m_menuCheck, nullptr);
    menuGet->setPosition(Vec2::ZERO);
    this->addChild(menuGet);

	 this->schedule(schedule_selector(DailySignReward::doRotate));  
	
    return true;
}

void DailySignReward::updateView(bool isCash)
{

	m_menuCheck->setVisible(!isCash);
	for(std::vector<SiginDay>::iterator iter = g_globalUserSigin.m_vecUserSigin.begin(); iter !=g_globalUserSigin.m_vecUserSigin.end();++iter)  
	{  

		if (iter->m_state == 2)
		{
			char buff[64] = {0};
			sprintf(buff, "%s x%d", iter->m_itemName.c_str(), iter->m_count);

			m_messageLabel->resetLable() ;
			m_messageLabel->Appand(28,"fzlt.ttf","获得",Color4B::WHITE);
			m_messageLabel->Appand(28,"fzlt.ttf",buff,Color4B(0xff,0xd2,0x00,0xff));

			ticketSprite->ignoreAnchorPointForPosition(false);
			ticketSprite->setAnchorPoint(Point(0.5,0.5)) ;
			ticketSprite->useDownLoadSize() ;
			SetHttpSpriteURL(ticketSprite, iter->m_iconUrl);
			ticketSprite->setScale(0.77) ;
			iter->m_state = 1 ;
			g_globalUserSigin.m_hasGetSiginReward = true ;
			SendClientRequest(ClientRequest::kCallExpressPower) ;
		}

	} 


	//for (auto item : g_globalUserSigin.m_vecUserSigin)
	//{
	//	if (item.m_state == 2)
	//	{
	//		char buff[64] = {0};
	//		sprintf(buff, "%s x%d", item.m_itemName.c_str(), item.m_count);

	//		m_messageLabel->resetLable() ;
	//		m_messageLabel->Appand(28,"fzlt.ttf","获得",Color4B::WHITE);
	//		m_messageLabel->Appand(28,"fzlt.ttf",buff,Color4B(0xff,0xd2,0x00,0xff));

	//		SetHttpSpriteURL(ticketSprite, item.m_iconUrl);
	//		item.m_state = 1 ;
	//		g_globalUserSigin.m_hasGetSiginReward = true ;
	//	}
	//}
}


void DailySignReward::OnImmediatelyCheck(Ref* pSend)
{
	setVisible(false) ;
	//this->unschedule(schedule_selector(DailySignReward::doRotate,1/60));
	g_pGameFrame->callShowUserItem() ;

	//GameTipsManager::getInstance()->hideTips() ;

}

void DailySignReward::doRotate(float delta)
{
	shineSprite->setRotation(shineSprite->getRotation()+1) ;
}

void DailySignReward::onMenuItemCloseSelected(cocos2d::Ref *)
{
	setVisible(false) ;
	//GameTipsManager::getInstance()->hideTips() ;
}

void DailySignReward::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		 this->setScale(0.7f) ;
		 ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		 CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
		 this->runAction(easeBounceOut);
	}
	else
	{
		 //g_globalUserSigin.m_vecUserSigin.clear() ;
	}

}




/************************************************************************/
/* 

连续登录奖励弹窗
*/
/************************************************************************/

bool ContinueSignReward::init()
{
	if (!GameDialog::init( ))
	{
		return false;
	}


	auto bg = Sprite::create("userinfo/bgdialog0.png");
	auto sz = bg->getContentSize();

	m_Node = Node::create() ;
	m_Node->ignoreAnchorPointForPosition(false) ;
	m_Node->setAnchorPoint(Point(0.5,0.5)) ;
	m_Node->setContentSize(sz) ;
	m_Node->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(m_Node) ;


	bg->setPosition(sz.width / 2, sz.height / 2);
	setContentSize(sz);
	m_Node->addChild(bg);


	auto colorLayer =LayerColor::create(Color4B(0x00,0x00,0x00,0xb2)) ;
	colorLayer->ignoreAnchorPointForPosition(false) ;
	colorLayer->setAnchorPoint(Point(0.5,0.5)) ;
	colorLayer->setContentSize(Size(DataMgr->getGameWidth(),DataMgr->getGameHeight())) ;
	colorLayer->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(colorLayer,-1);





	m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(ContinueSignReward::onMenuItemCloseSelected, this));
	m_btMenuItemClose->setPosition(sz.width - 25, sz.height - 28);
	auto menu = Menu::create(m_btMenuItemClose, nullptr);
	menu->setPosition(Vec2::ZERO);
	m_Node->addChild(menu, 100);
	//标题
	auto titleSprite = Sprite::createWithSpriteFrameName("sigingetrewardtitle.png");
	titleSprite->setPosition(sz.width/2, sz.height-8);
	m_Node->addChild(titleSprite);
	//光晕
	shineSprite = Sprite::createWithSpriteFrameName("sigin_shine.png");
	shineSprite->setPosition(sz.width/2, sz.height/2);
	m_Node->addChild(shineSprite);


	m_messageLabel  = LineLabel::create() ;
	m_messageLabel->setPosition(sz.width/2,sz.height/2+85) ;
	m_Node->addChild(m_messageLabel);


	char buff[64];
	sprintf(buff,"您本周已经连续登录%d天啦~",g_globalUserSigin.m_continusdays) ;
	auto m_lableDays = CreateLabelMSYHAndAnchorPosClrAddTo(28,buff,Point(0.5,0.5),sz.width/2,319,Color4B(0xff,0xd2,0x00,0xff),m_Node,0) ;
	auto mTipsThanks = CreateLabelMSYHAndAnchorPosClrAddTo(28,"感谢您的喜爱，收下我们的心意吧~",Point(0.5,0.5),sz.width/2,275,Color4B(0xff,0xd2,0x00,0xff),m_Node,0) ;
	auto mTipsMall = CreateLabelMSYHAndAnchorPosClrAddTo(22,"奖励已通过邮件发送，请注意查收",Point(0.5,0.5),sz.width/2,108,Color4B(0x20,0xc5,0x2b,0xff),m_Node,0) ;


	//默认奖励图片
	ticketSprite = HttpSprite::create("sigin_default.png");
	auto httpSpriteSz = ticketSprite->getContentSize();
	ticketSprite->setPosition(sz.width/2 , httpSpriteSz.height/2);
	m_Node->addChild(ticketSprite);
	ticketSprite->useDownLoadSize() ;
	ContinuousSingin item = g_globalUserSigin.m_vecUserContinuousSingin.at(g_globalUserSigin.m_continusdays-1);
	SetHttpSpriteURL(ticketSprite, item.m_iconUrl);
	ticketSprite->ignoreAnchorPointForPosition(false);
	ticketSprite->setAnchorPoint(Point(0.5,0.5)) ;
	ticketSprite->useDownLoadSize() ;
	ticketSprite->setPosition(sz.width/2 , sz.height/2);
	ticketSprite->setScale(0.77) ;
	//名称
	auto ticketNameLabel  = CreateLabelMSYH(22, "", TextHAlignment::CENTER, TextVAlignment::CENTER);

	ticketNameLabel->setPosition(sz.width/2, 148);
	ticketNameLabel->setAnchorPoint(Vec2(0.5, 0.5));
	m_Node->addChild(ticketNameLabel);

	//char buff[64] ;
	sprintf(buff,"%sx%d",item.m_itemName.c_str(),item.m_count) ;
	SetLabelString(ticketNameLabel, buff);
	ticketNameLabel->setColor(Color3B::WHITE);


	//m_menuCheck = MenuItemSpriteWithLabel::create("update/btcancel.png", 3, CC_CALLBACK_1(ContinueSignReward::OnImmediatelyCheck, this));
    m_menuCheck = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(ContinueSignReward::OnImmediatelyCheck, this));
	Size szMenu = m_menuCheck->getContentSize();
	m_menuCheck->setPosition(sz.width/2, szMenu.height/2+18);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "分享", Vec2(0.5, 0.5), szMenu.width/2, szMenu.height/2, Color4B(0xff,0xff,0xff,0xff), m_menuCheck, 0);
	auto menuGet = Menu::create(m_menuCheck, nullptr);
	menuGet->setPosition(Vec2::ZERO);
	m_Node->addChild(menuGet);
	m_menuCheck->setVisible(false) ;
	this->schedule(schedule_selector(ContinueSignReward::doRotate));  
	
	return true;
}

void ContinueSignReward::updateView()
{
	//for (auto item : g_globalUserSigin.m_vecUserSigin)
	//{
	//	if (item.m_state == 1)
	//	{
	//		char buff[64] = {0};
	//		sprintf(buff, "%s x%d", item.m_itemName.c_str(), item.m_count);

	//		m_messageLabel->resetLable() ;
	//		m_messageLabel->Appand(28,"fzlt.ttf","获得",Color4B::WHITE);
	//		m_messageLabel->Appand(28,"fzlt.ttf",buff,Color4B(0xff,0xd2,0x00,0xff));

	//		ticketSprite->useDownLoadSize() ;

	//		SetHttpSpriteURL(ticketSprite, item.m_iconUrl);
	//		ticketSprite->ignoreAnchorPointForPosition(false);
	//		ticketSprite->setAnchorPoint(Point(0.5,0.5)) ;
	//		ticketSprite->useDownLoadSize() ;
	//		Size sz = this->getContentSize() ;
	//		ticketSprite->setPosition(sz.width/2 , sz.height/2);

	//		item.m_state = 0 ;
	//	}
	//}
}


void ContinueSignReward::OnImmediatelyCheck(Ref* pSend)
{
	setVisible(false) ;

}

void ContinueSignReward::doRotate(float delta)
{
	shineSprite->setRotation(shineSprite->getRotation()+1) ;
}

void ContinueSignReward::onMenuItemCloseSelected(cocos2d::Ref *)
{
	setVisible(false) ;
	
}

void ContinueSignReward::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
		m_Node->setScale(0.7f) ;
		ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
		m_Node->runAction(easeBounceOut);
	}
	else
	{
		//g_globalUserSigin.m_vecUserSigin.clear() ;
	}

}

void ContinueSignReward::onGrabTouchBegan(bool bHitDialog)
{
	if (bHitDialog)
	{
	}
}