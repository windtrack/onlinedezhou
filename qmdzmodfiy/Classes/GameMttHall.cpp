#include "GameMttHall.h"
#include "Tools.h"
#include "GameFrame.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"

#include "GameData.h"
#include "GuideTips.h"
#include "FrameScale9SpriteButton.h"
#include "FrameSpriteButton.h"
#include "AnimateViewer.h"
#include "GameFrame.h"
#include "MttRule.h"
#include "MttTypeSelect.h"
#include "MttSignUpSuccess.h"


#include "GrabTouch.h"
#include "FrameSprite.h"
#include "GuideTips.h"
#include "GameTipsManager.h"
#include "AnimationManager.h"
enum 
{
	TableWidth = 922 ,
	TableHeight = 135 ,
};

enum 
{
	ZOrder_Rule=10 ,
	ZOrder_Rank,
	ZOrder_HelpGuide,
	ZOrder_GameTypeSelect,
};

bool compareByType(const int &a,const int &b)
{
	return a < b;
}

bool compareByTime(const GameMttTableInfo &a,const GameMttTableInfo &b)
{
	return a.m_realTime < b.m_realTime;
}

bool GameMttHall::init()
{
	if(!GameDialog::init())
	{
		return false;
	}
	//载入布局
	s_uiroot = CSLoader::createNode("res/ui/mtthall.csb");
	Size frameSize = Director::getInstance()->getVisibleSize(); 
	s_uiroot->setContentSize(frameSize); 
	ui::Helper::doLayout(s_uiroot);  
	addChild(s_uiroot);
	setContentSize(frameSize); 

	auto frameSz = Director::getInstance()->getVisibleSize();
	m_layerColorBg = LayerColor::create(Color4B(0,0,0,120), frameSz.width, frameSz.height);
	m_layerColorBg->setPosition(0, 0);
	addChild(m_layerColorBg);
	m_layerColorBg->setVisible(false);


	//初始化背景
	Layout* m_TableBG = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_spritebg"));
	m_img_tableLeft  = static_cast<ImageView*>(Tools::getWidgetByName(m_TableBG,"Image_bgleft"));
	m_img_tableRight  = static_cast<ImageView*>(Tools::getWidgetByName(m_TableBG,"Image_bgright"));

	m_spriteHallBg[0] = Sprite::create("hall2half.png") ;
	m_spriteHallBg[1] = Sprite::createWithTexture(m_spriteHallBg[0]->getTexture()) ;
	m_spriteHallBg[1]->setScaleX(-1) ;
	m_spriteHallBg[0]->setPosition(m_img_tableLeft->getContentSize().width/2,m_img_tableLeft->getContentSize().height/2) ;
	m_spriteHallBg[1]->setPosition(m_img_tableRight->getContentSize().width/2,m_img_tableRight->getContentSize().height/2) ;

	m_img_tableLeft->addChild(m_spriteHallBg[0]);
	m_img_tableRight->addChild(m_spriteHallBg[1]);


	//初始化layerout
	s_layoutTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_mtthall")) ;
	m_pSpriteBgTable = static_cast<Layout*>(Tools::getWidgetByName(s_uiroot,"Panel_spritebg"));

	m_HeroInfo = static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_heroinfo"));

	m_leftupButton = static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_leftup"));

	m_rightupButton = static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_buttonrightup"));

	m_middleUpButtonLayer = static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_middelButton"));

	m_middleDownButtonLayer = static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_title"));

	m_SngTips= static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_tips"));
	m_SngTips->setVisible(false) ;

	//返回
	m_buttonBack = static_cast<Button*>(Tools::getWidgetByName(m_leftupButton,"Button_back"));
	m_buttonBack->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	//左上
	m_buttonhelp = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_help"));
	m_buttonhelp->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_buttonrank = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_rank"));
	m_buttonrank->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_buttonshop = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_shop"));
	m_buttonshop->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_buttonkefu = static_cast<Button*>(Tools::getWidgetByName(m_rightupButton,"Button_kefu"));
	m_buttonkefu->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;





	//中间上面
	m_buttonPageUp = static_cast<Button*>(Tools::getWidgetByName(m_middleUpButtonLayer,"Button_pageup"));
	m_buttonPageUp->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_buttonPageDown = static_cast<Button*>(Tools::getWidgetByName(m_middleUpButtonLayer,"Button_pagedown"));
	m_buttonPageDown->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;
	ImageView* mImgMiddleButton = static_cast<ImageView*>(Tools::getWidgetByName(m_middleUpButtonLayer,"Image_middlebtbg"));

	m_button_WillSiginUp = static_cast<Button*>(Tools::getWidgetByName(mImgMiddleButton,"Button_zhengzaibaoming"));
	m_button_WillSiginUp->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_button_hasSiginUp = static_cast<Button*>(Tools::getWidgetByName(mImgMiddleButton,"Button_yibaoming"));
	m_button_hasSiginUp->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_button_gameover = static_cast<Button*>(Tools::getWidgetByName(mImgMiddleButton,"Button_hasover"));
	m_button_gameover->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;


	m_Text_WillSiginUp = static_cast<Text*>(Tools::getWidgetByName(m_button_WillSiginUp,"Text_zhengzaibaoming"));
	m_Text_hasSiginUp = static_cast<Text*>(Tools::getWidgetByName(m_button_hasSiginUp,"Text_yibaoming"));
	m_Text_gameover = static_cast<Text*>(Tools::getWidgetByName(m_button_gameover,"Text_hasover"));

	

	//中间导航
	//m_imgTypeBG = static_cast<ImageView*>(Tools::getWidgetByName(m_middleDownButtonLayer,"Image_matchtypebg"));
	//m_imgTypeBG->setVisible(false) ;

	m_button_MathcType = static_cast<Button*>(Tools::getWidgetByName(m_middleDownButtonLayer,"Button_matchtype"));
	m_button_MathcType->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_button_MatchTime = static_cast<Button*>(Tools::getWidgetByName(m_middleDownButtonLayer,"Button_matchtime"));
	m_button_MatchTime->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_button_SiginTips = static_cast<Button*>(Tools::getWidgetByName(m_middleDownButtonLayer,"Button_siginmoney"));
	m_button_SiginTips->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	//遮罩
	m_buttonTouchAll = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Panel_screentouch"));
	m_buttonTouchAll->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	m_buttonTouchUp = static_cast<Button*>(Tools::getWidgetByName(s_uiroot,"Panel_buttontouch"));
	m_buttonTouchUp->addTouchEventListener(this,toucheventselector(GameMttHall::onMenuItemBottomSelect)) ;

	//人物信息
	m_textName =  static_cast<Text*>(Tools::getWidgetByName(m_HeroInfo,"Text_name"));
	m_textMoney =  static_cast<Text*>(Tools::getWidgetByName(m_HeroInfo,"Text_money"));
	m_textrank =  static_cast<Text*>(Tools::getWidgetByName(m_HeroInfo,"text_rolerank"));
	m_textDashiRank =  static_cast<Text*>(Tools::getWidgetByName(m_HeroInfo,"text_roledashirank"));
	ImageView* imgHero = static_cast<ImageView*>(Tools::getWidgetByName(m_HeroInfo,"Image_head"));
	m_spriteHero =  CreateUserHeadSprite("defaulticon.png", "family/iconclipmaskbig98.png", imgHero, Vec2(0, 0), "hall/iconround.png");
	m_spriteHero->setCascadeOpacityEnabled(true) ;


	m_imgPuke = static_cast<ImageView*>(Tools::getWidgetByName(s_layoutTable,"Image_puke"));

	//滚动层
	Node* joinFamilyNode = Tools::getWidgetByName(s_layoutTable,"Panel_scroll");
	sizeListView.width = joinFamilyNode->getContentSize().width;
	sizeListView.height = joinFamilyNode->getContentSize().height;
	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,0,2,joinFamilyNode,0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);
	m_pScrollView->setDelegate(this);

	

	layerClip =  static_cast<Layout*>(Tools::getWidgetByName(s_layoutTable,"Panel_clip"));

	g_globalMyData.WaitChange(WAITCASHCHANGE , std::bind(&GameMttHall::OnUpdateMyData, this, std::placeholders::_1));
	initHallInfo() ;

	m_mttTypeSelect = nullptr ;
	m_mttrule = nullptr ;
	
	return true;
}


void GameMttHall::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
	if (view == m_pScrollView)
	{
		m_SngTips->setVisible(false) ;
	}
}

void GameMttHall::onMenuItemBottomSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (m_isInAction)
		{
			return ;
		}

		if (pMenuItem == m_button_SiginTips)
		{
			m_SngTips->setVisible(!m_SngTips->isVisible()) ;
			return ;
		}

		m_SngTips->setVisible(false) ;
		//提示关闭
		if (pMenuItem == m_buttonTouchAll || pMenuItem == m_buttonTouchUp)
		{
			m_SngTips->setVisible(false) ;
			return ;
		}

		//上一页
		if (pMenuItem == m_buttonPageUp)
		{
			if (m_curPageIndex>0)
			{
				m_curPageIndex-- ;
				updateCurUsedMatchInfo() ;
				updatePageCountAndView(m_vecCurUesdTableInfo,m_useSort,m_curPageIndex) ;
			}

			updatePageButtonShow() ;
			return ;
		}
		//下一页
		if (pMenuItem == m_buttonPageDown)
		{
			if (m_curPageIndex<m_maxPageCount-1)
			{
				m_curPageIndex++ ;
				updateCurUsedMatchInfo() ;
				updatePageCountAndView(m_vecCurUesdTableInfo,m_useSort,m_curPageIndex) ;
			}
			
			updatePageButtonShow() ;
			return ;
		}
		//正在报名
		if (pMenuItem == m_button_WillSiginUp)
		{
			m_useSort = false ;
			m_button_MatchTime->setBright(!m_useSort);
			m_curSelectIndex = 0 ;
			updateMenuIndex(m_curSelectIndex) ;
			setCurMatchState(MatchState_IsSigining) ;
			setCurMttType(0) ;
	
			updatePageCountAndView(m_vecFirstStateInfo);
			return ;
		}
		//已报名
		if (pMenuItem == m_button_hasSiginUp)
		{
			m_useSort = false ;
			m_button_MatchTime->setBright(!m_useSort);
			m_curSelectIndex = 1 ;
			updateMenuIndex(m_curSelectIndex) ;
			setCurMatchState(MatchState_hasSigin) ;
			setCurMttType(0) ;
	
			updatePageCountAndView(m_vecFirstStateInfo);
			return ;
		}
		//已结束
		if (pMenuItem == m_button_gameover)
		{
			m_useSort = false ;
			m_button_MatchTime->setBright(!m_useSort);
			m_curSelectIndex = 2 ;
			updateMenuIndex(m_curSelectIndex) ;
			setCurMatchState(MatchState_GameOver) ;
			setCurMttType(0) ;

			updatePageCountAndView(m_vecFirstStateInfo);
			return ;
		}

		//赛事类型
		if (pMenuItem == m_button_MathcType)
		{
		
			showMttTypeSelect(true) ;
			return ;
		}
		//开赛时间
		if (pMenuItem == m_button_MatchTime)
		{
			m_useSort = !m_useSort ;
			m_button_MatchTime->setBright(!m_useSort);
			updateCurUsedMatchInfo() ;
			updatePageCountAndView(m_vecCurUesdTableInfo,m_useSort);

			return ;
		}


		//返回
		if (pMenuItem == m_buttonBack)
		{
			SendClientRequest(kLeaveGameMttHall) ;
			return ;
		}


		//规则
		if (pMenuItem == m_buttonhelp)
		{
			showMttRule(true) ;
		}
		//排行
		if (pMenuItem == m_buttonrank)
		{
			g_pGameFrame->showMasterRankList(true) ;
		}
		//商城
		if (pMenuItem == m_buttonshop)
		{
			Json::Value param;
			param["idx"] = 0;
			SendClientRequest(ClientRequest::kPopMall, param);
			return ;
		}
		//客服
		if (pMenuItem == m_buttonkefu)
		{
			SendClientRequest(ClientRequest::kPopCustomerServices);
			return ;
		}


	}
}

void GameMttHall::onKeyEscPressed()
{
	SendClientRequest(ClientRequest::kLeaveGameSngHall);
}

void GameMttHall::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		Json::Value param;
		param["tuid"] = g_globalMyData.m_iUserId;
		SendClientRequest(ClientRequest::kCallMttHallInfo, param);
		m_isInAction = false ;

		m_SngTips->setVisible(false) ;
		m_useSort = false ;
		m_hasSort = false ;
		m_button_MatchTime->setBright(!m_useSort);

	
		
		m_curSelectIndex = 0 ;

		updateMenuIndex(m_curSelectIndex) ;

		m_layerAph = 0 ;
		m_rightupButton->setOpacity(m_layerAph) ;
		m_leftupButton->setOpacity(m_layerAph) ;
		m_middleUpButtonLayer->setOpacity(m_layerAph) ;
		m_middleDownButtonLayer->setOpacity(m_layerAph) ;
		m_HeroInfo->setOpacity(m_layerAph) ;
		m_spriteHero->setOpacity(m_layerAph);



		float puke_curX = m_imgPuke->getPositionX() ;
		float puke_curY = m_imgPuke->getPositionY() ;
		m_imgPuke->setPositionX(puke_curX-150) ;
		m_imgPuke->setPositionY(puke_curY-150) ;


		float s_curX= m_pSpriteBgTable->getPositionX();
		float s_curY= m_pSpriteBgTable->getPositionY();
		auto sz = m_pSpriteBgTable->getContentSize();
		m_pSpriteBgTable->setPositionY(s_curY-GameData::getInstance()->getGameHeight());



		auto sendmes = [this](){
				//SendClientRequest(ClientRequest::kMttHallInfo);
			if (!m_openByHall)
			{
				updateMttHallList(0) ;

				
			}
			m_openByHall = false ;

		} ;
        auto funGuideTips = [this](){
            GameTipsManager::getInstance()->clean();
            createGuideTips();
            GameTipsManager::getInstance()->callNextTips();
        };

		m_pSpriteBgTable->runAction(Sequence::create(DelayTime::create(0.3f), EaseBackOut::create(MoveTo::create(0.3f, Vec2(s_curX, s_curY))), 
			CallFunc::create(sendmes),DelayTime::create(1.0),CallFunc::create(funGuideTips),
			nullptr));

		//g_pGameFrame->PopLoading(true);
		
	}
	else
	{
		setShowItems(false) ;
		for (int i=0;i<m_vecSngTableListInfoNew.size(); i++)
		{
			MttTableItem *table = m_vecSngTableListInfoNew.at(i) ;

			table->m_pSpriteBgItem->setVisible(false) ;
			table->m_needShow = false ;
		}
		//unschedule(schedule_selector(GameMttHall::updateItemTime));//删除倒计时
	}
}

void GameMttHall::callHallAction()
{

	m_isInAction = true ;
	float s_curX= 0;
	float s_curY= 0;



	for (int i=0;i<m_vecSngTableListInfoNew.size(); i++)
	{
		MttTableItem *table = m_vecSngTableListInfoNew.at(i) ;
		table->m_pSpriteBgItem->setVisible(table->m_needShow) ;
		s_curX = table->m_pSpriteBgItem->getPositionX() ;
		s_curY = table->m_pSpriteBgItem->getPositionY() ;
		table->m_pSpriteBgItem->setPositionY(s_curY-640) ;
		table->m_pSpriteBgItem->runAction(Sequence::create(DelayTime::create(0.5f),
			CCEaseSineOut::create(MoveTo::create(0.3+i*0.25f, Vec2(s_curX, s_curY))), nullptr));
	}



	auto loadAction = [this](){

		m_layerAph+=10 ;
		if (m_layerAph>=255)
		{
			m_layerAph = 255 ;
			this->stopAllActions() ;
			m_isInAction = false ;
		}
		m_rightupButton->setOpacity(m_layerAph) ;
		m_leftupButton->setOpacity(m_layerAph) ;
		m_middleUpButtonLayer->setOpacity(m_layerAph) ;
		m_middleDownButtonLayer->setOpacity(m_layerAph) ;
		m_HeroInfo->setOpacity(m_layerAph) ;
		m_spriteHero->setOpacity(m_layerAph);

		
	} ;

	CCFiniteTimeAction *repeat = CCRepeat::create(Sequence::create(CallFunc::create(loadAction),DelayTime::create(1.0/60.0f),NULL), 9999);



	//float puke_curX = m_imgPuke->getPositionX() ;
	//float puke_curY = m_imgPuke->getPositionY() ;
	//m_imgPuke->setPositionX(puke_curX-150) ;
	//m_imgPuke->setPositionY(puke_curY-150) ;
	auto pukeAcion = [this](){

		float s_curX = m_imgPuke->getPositionX() ;
		float s_curY = m_imgPuke->getPositionY() ;
		m_imgPuke->runAction(Sequence::create(DelayTime::create(0.5f),
			CCEaseSineOut::create(MoveTo::create(0.3, Vec2(s_curX+150, s_curY+150))), nullptr));
	} ;


	this->runAction(Sequence::create(DelayTime::create(0.7),CallFunc::create(pukeAcion),repeat,NULL)) ;



}



void GameMttHall::showBlack(bool flag)
{
	m_layerColorBg->setVisible(flag) ;
}

void GameMttHall::initHallInfo()
{
	m_maxPageCount = 0 ;
	m_curPageIndex = 0 ;

	g_globaMttGameInfo.m_signupformGame = false ;
	g_globaMttGameInfo.m_roudIdex=0 ;
	g_globaMttGameInfo.m_mangzhuIndex=0 ;
	g_globaMttGameInfo.m_rank=1 ;
	g_globaMttGameInfo.m_isGameStart = false ;
	g_globaMttGameInfo.m_isMttlose=false ;
	g_globaMttGameInfo.m_isGameOver = false ;

	g_globaMttGameInfo.m_curMttTable.m_MttRank.clear() ;
}

/************************************************************************/
/* 更新页数    
vecTable 当前数据
updateTime  是否使用时间排序
pageIndex 默认页面0
*/
/************************************************************************/
void GameMttHall::updatePageCountAndView(std::vector<GameMttTableInfo> vecTable,bool updateTime ,unsigned pageIndex)
{
	std::vector<GameMttTableInfo> vecRealTable ;
	if (updateTime)
	{
		if (!m_hasSort)
		{
			std::sort( vecTable.begin(), vecTable.end(), compareByTime);
			m_hasSort = true ;
			m_vecCurUesdInfoByTime = vecTable ;
		}
		vecRealTable = m_vecCurUesdInfoByTime ;
	}
	else
	{
		m_vecCurUesdTableInfo = vecTable;
		vecRealTable = m_vecCurUesdTableInfo ;
	}

	g_globalMttHallInfo.m_curTableCount = vecRealTable.size() ;
	m_maxPageCount = g_globalMttHallInfo.m_curTableCount%SinglePage == 0 ?g_globalMttHallInfo.m_curTableCount/SinglePage:(g_globalMttHallInfo.m_curTableCount/SinglePage +1) ;
	m_curPageIndex = pageIndex ;
	if (m_curPageIndex>m_maxPageCount-1)//溢出控制
	{
		m_curPageIndex = m_maxPageCount-1;
	}
	updatePageButtonShow() ;
	updateVeiw(vecRealTable,m_curPageIndex);
	
}



void GameMttHall::updateVeiw(std::vector<GameMttTableInfo> vecTable,unsigned pageIndex)
{

	//长度
	int count =0;
	if (SinglePage*(pageIndex+1)>vecTable.size())
	{
		count = vecTable.size()-SinglePage*pageIndex ;
	}
	else
	{
		count = SinglePage ;
	}



	unsigned uCount = count;
	unsigned ListBgItemH = sizeListView.width;
	unsigned uItemH = TableHeight;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2;
	float fY = fRealH - uItemH / 2 - 10;

	for(auto item:m_vecSngTableListInfoNew)
	{
		item->m_pSpriteBgItem->setVisible(false) ;
		item->m_needShow = false ;
	}

	for (int i=SinglePage*pageIndex;i<vecTable.size()&&i<(SinglePage*(pageIndex+1));i++)
	{
		MttTableItem* item;
		int index = i%SinglePage ;
	
		GameMttTableInfo followList = vecTable.at(i) ;

		if (index>=m_vecSngTableListInfoNew.size())
		{
			item = GetMttTableListItem(index);
			m_vecSngTableListInfoNew.push_back(item);
		}
		else
		{
			item = m_vecSngTableListInfoNew.at(i%SinglePage);

		}
		item->m_needShow = true ;
		UpdateItem(item, followList);

		if (!m_isInAction)
		{
			item->m_pSpriteBgItem->setPosition(fX, fY);
		}

		item->m_pSpriteBgItem->setVisible(m_showItems); 
		//item->m_pSpriteBgItem->setVisible(true); 

		fY -= uItemH; 
			
	}

	float temp = fRealH - sizeListView.height;
	//每次都显示滚动框的最上面
	m_pScrollView->setContentOffset(Vec2(0,-temp), false);
	m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
		
}




GameMttHall::MttTableItem* GameMttHall::GetMttTableListItem(unsigned index)
{
	MttTableItem *item = new MttTableItem;

	//背景
	//auto m_pSpriteBgItem = Sprite::create("gamesng/sng_hall_background-1.png") ;


	auto m_pSpriteBgItem = CreateNode3AndPosAddTo<FrameSprite>("gamemtt/mtt_hall_background-1.png", 1, 2, sizeListView.width / 2, 0, m_pScrollView, 0);
	auto btDetail = LayerButton::create(false, true);
	m_pSpriteBgItem->addChild(btDetail);			
	Size sz = m_pSpriteBgItem->getContentSize() ;

	//cocos2d::Sprite			*m_pAniBg;			   
	//cocos2d::Sprite			*m_pAni;			   
	//cocos2d::Sprite			*m_pAniUpScale;			
	//动画层
	auto m_pAniBg = Sprite::create("gamemtt/mtt_typebg.png") ;
	m_pAniBg->setPosition(85,sz.height/2+5) ;
	m_pAniBg->ignoreAnchorPointForPosition(false);
	m_pAniBg->setAnchorPoint(Point(0.5,0.5)) ;
	m_pSpriteBgItem->addChild(m_pAniBg) ;

	auto m_pAniUpScale = Sprite::create("gamemtt/mtt_typeshine.png") ;
	m_pSpriteBgItem->addChild(m_pAniUpScale,10) ;
	m_pAniUpScale->setPosition(88,sz.height+5) ;
	m_pAniUpScale->setAnchorPoint(Point(0.5,1)) ;
	Tools::callScaleAction(m_pAniUpScale,false,0.8,0.8,1.0,0.8) ;


	auto m_pAniViewer = AnimateViewer::create("gamemtt/mtt_typebg_shine.png", true, 1/ 15.0f, 4, 4, 16);
	m_pAniViewer->setPosition(83, 22);
	m_pSpriteBgItem->addChild(m_pAniViewer,9);
	m_pAniViewer->setVisible(true);
		//道具
	HttpSprite *m_httpIcon = HttpSprite::create("sigin_default.png");
	m_httpIcon->setPosition(78+7,sz.height/2) ;
	m_httpIcon->ignoreAnchorPointForPosition(false);
	m_httpIcon->setAnchorPoint(Point(0.5,0.5)) ;
	m_pSpriteBgItem->addChild(m_httpIcon) ;
	m_httpIcon->setScale(0.67) ;


	//auto m_labelIndex = CreateLabelMSYHAndAnchorPosClrAddTo(20,"",Vec2(0,0.5),80,20,Color4B(0xff,0xff,0xff,0xff),m_pSpriteBgItem,10);

	auto m_labelName = CreateLabelMSYHAndAnchorPosClrAddTo(28,"",Vec2(0,0.5),160,77,Color4B(0xff,0xff,0xff,0xff),m_pSpriteBgItem,10);

	//但前人数标志
	auto m_spriteRoleCount = Sprite::create("gamesng/sng_hall_icon-1.png") ;
	m_spriteRoleCount->setPosition(170,27) ;
	m_pSpriteBgItem->addChild(m_spriteRoleCount) ;
	//参赛人数
	auto m_LableroleCount = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0.5),188,27,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);

	//重购
	auto m_spriteReBuy = Sprite::create("gamemtt/mtt_buttonr.png") ;
	m_spriteReBuy->setPosition(270,29) ;
	m_pSpriteBgItem->addChild(m_spriteReBuy) ;
	//增购
	auto m_spriteAddon = Sprite::create("gamemtt/mtt_buttona.png") ;
	m_spriteAddon->setPosition(305,29) ;
	m_pSpriteBgItem->addChild(m_spriteAddon	) ;
	//浮动赛标志
	auto m_spriteFloatMatch = Sprite::create("gamemtt/mtt_fd.png") ;
	m_spriteFloatMatch->setPosition(340,29) ;
	m_pSpriteBgItem->addChild(m_spriteFloatMatch) ;
	//延迟比赛标志
	auto m_spriteDelayMatch = Sprite::create("gamemtt/mtt_delay_icon.png") ;
	m_spriteDelayMatch->setPosition(375,29) ;
	m_pSpriteBgItem->addChild(m_spriteDelayMatch) ;
	
	//赛事开始
	//即将报名
	auto m_spriteTimeBG = Sprite::create("gamemtt/mtt_timebackground.png") ;
	m_spriteTimeBG->setPosition(495,sz.height/2) ;
	m_pSpriteBgItem->addChild(m_spriteTimeBG) ;

	//即将开始
	auto m_spriteJijiangkaishiBg = Sprite::create("gamemtt/mtt_djsgreen.png") ;
	m_spriteJijiangkaishiBg->setPosition(495,42) ;
	m_pSpriteBgItem->addChild(m_spriteJijiangkaishiBg) ;

	//延时开始
	auto m_spriteYanShikaishiBg = Sprite::create("gamemtt/mtt_djsblue.png") ;
	m_spriteYanShikaishiBg->setPosition(495,42) ;
	m_pSpriteBgItem->addChild(m_spriteYanShikaishiBg) ;

	auto m_lable_TimeDes = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0.5,0.5),490,90,Color4B(0x50,0xbd,0x2e,0xff),m_pSpriteBgItem,2);
	auto m_lable_titleDay = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0.5,0.5),490,90,Color4B(0x50,0xbd,0x2e,0xff),m_pSpriteBgItem,2);
	auto m_lable_statrTime = CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),490,42,Color4B(0x79,0xdc,0x98,0xff),m_pSpriteBgItem,2);

	cocos2d::Label			*m_lable_JiJiangCountDown[4] ; //倒计时

	int timePos[4] = {429,468,520,560} ;
	for (int i=0;i<4;i++)
	{
		m_lable_JiJiangCountDown[i] =  CreateLabelMSYHAndAnchorPosClrAddTo(26,"",Vec2(0.5,0.5),154,29,Color4B(0xff,0xff,0xff,0xff),m_pSpriteBgItem,2);
		m_lable_JiJiangCountDown[i]->setPositionY(42) ;
		m_lable_JiJiangCountDown[i]->setPositionX(timePos[i]) ;
	}



	//报名费德州币标志
	auto m_spriteMoney = Sprite::create("gamesng/sng_hall_goldCoin-2.png") ;
	m_spriteMoney->setPosition(602,sz.height/2) ;
	m_pSpriteBgItem->addChild(m_spriteMoney) ;
	//报名费
	auto m_lableMoney = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0.5),622,sz.height/2,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);

	//门票标志
	auto m_spriteFreesticket = Sprite::create("gamesng/sng_hall_ticket-1.png") ;
	m_spriteFreesticket->setPosition(648,77) ;
	m_pSpriteBgItem->addChild(m_spriteFreesticket) ;
	// 门票数量
	auto m_labelFreeStickCount = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0.5),681,77,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);
	auto m_labelMyStickCount=  CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0.5,0.5),671,41,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);

	//报名
	FrameScale9SpriteButton* m_pBtJiaRu = FrameScale9SpriteButton::createWithScale9Sprite(2 ,Size(112, 252/3),false,true) ;
	auto m_labelJiaru = CreateLabelMSYHBDAndAnchorPosClrAddTo(30,"报名",Vec2(0.5,0.5),0,0,Color4B::WHITE,m_pBtJiaRu,1);

	m_pBtJiaRu->ignoreAnchorPointForPosition(false);
	m_pBtJiaRu->setAnchorPoint(Point(0,0)) ;
	m_pBtJiaRu->setPosition(830,sz.height/2);
	m_pSpriteBgItem->addChild(m_pBtJiaRu);


	auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(m_pSpriteBgItem, false);
	touchGrabber->retain();
	touchGrabber->GetTouchProcer().SetHitCB(
		[m_pSpriteBgItem,m_pBtJiaRu](bool bSel, Node*, TouchGrabber*){

			if (bSel)
			{
				if (!m_pBtJiaRu->isSelected())
				{
					m_pSpriteBgItem->SetFrameIdx(1);
				}
			}
			else
			{
				m_pSpriteBgItem->SetFrameIdx(bSel?1:0);
			}
	}

	);

	AnimationManager *m_aniBtJiaru  = AnimationManager::creatArmature("mtt_entershine");
	m_pBtJiaRu->addChild(m_aniBtJiaru);
	m_aniBtJiaru->playByIndexAndRepatForever(0) ;
	m_aniBtJiaru->setVisible(false) ;
	//比赛未开始
	cocos2d::Label	*m_labelNotStart =CreateLabelMSYHAndAnchorPosClrAddTo(26,"未开放",Vec2(0.5,0.5),834,75,Color4B(0x79,0xdc,0x98,0xff),m_pSpriteBgItem,2);
	cocos2d::Label	*m_labelNotStartDes  =CreateLabelMSYHAndAnchorPosClrAddTo(18,"赛前10小时开放",Vec2(0.5,0.5),834,50,Color4B(0x79,0xdc,0x98,0xff),m_pSpriteBgItem,2);
	//等待开始
	cocos2d::Label	*m_wait  =CreateLabelMSYHAndAnchorPosClrAddTo(22,"等待开始",Vec2(0.5,0.5),834,58,Color4B(0xff,0xd2,0x00,0xff),m_pSpriteBgItem,2);

	item->m_pSpriteBgItem = m_pSpriteBgItem;			    // 滚动视图行的背景	
	item->m_pBtDetail = btDetail;
	item->m_httpIcon = m_httpIcon ;
	item->m_labelName = m_labelName ;
	item->m_spriteRoleCount = m_spriteRoleCount ;
	item->m_LableroleCount = m_LableroleCount ;
	item->m_spriteReBuy = m_spriteReBuy;
	item->m_spriteAddon = m_spriteAddon;
	item->m_spriteFloatMatch = m_spriteFloatMatch ;
	item->m_spriteDelayMatch = m_spriteDelayMatch ;
	//即将开始的背景
	item->m_spriteTimeBG = m_spriteTimeBG ;
	item->m_spriteJijiangkaishiBg = m_spriteJijiangkaishiBg ;
	item->m_spriteYanShikaishiBg = m_spriteYanShikaishiBg ;
	item->m_lable_TimeDes =m_lable_TimeDes ;
	item->m_lable_titleDay = m_lable_titleDay ;
	item->m_lable_statrTime=m_lable_statrTime ;
	item->m_wait = m_wait ;
	//倒计时时间 
	for (int i=0; i<4; i++)
	{
		item->m_lable_JiJiangCountDown[i] = m_lable_JiJiangCountDown[i] ; //倒计时
	}
	item->m_spriteMoney = m_spriteMoney ;
	item->m_lableMoney = m_lableMoney ;
	item->m_spriteFreesticket = m_spriteFreesticket ;
	item->m_labelFreeStickCount = m_labelFreeStickCount ;
	item->m_labelMyStickCount = m_labelMyStickCount ;
	item->m_pBtJiaRu = m_pBtJiaRu;					//加入按钮
	item->m_aniBtJiaru = m_aniBtJiaru ;
	item->m_labelJiaru = m_labelJiaru ;
	item->m_labelNotStart = m_labelNotStart ;
	item->m_labelNotStartDes = m_labelNotStartDes ;
	item->m_indexInPage = index ;
	item->m_pBtDetail->setContentSize(Size(TableWidth-143,TableHeight) ) ;
	return item ;
}

void GameMttHall::UpdateItem(GameMttHall::MttTableItem *item, const GameMttTableInfo& followInfo)
{
	item->m_index = followInfo.m_Index ;
	item->m_time = followInfo.m_realTime ;
	item->m_TimeType = followInfo.m_TimeType ;
	item->m_tableId = followInfo.m_tableID ;
	item->m_CountDown = followInfo.m_showCountDown ;
	item->updateTime(item->m_time) ;
	char buff[64] ;
	float curX;
	float curY ;
	//图片
	item->m_httpIcon->useDownLoadSize();
	SetHttpSpriteURL(item->m_httpIcon, followInfo.m_iconurl);
	item->m_httpIcon->setScale(0.67) ;

	//名字
	SetLabelString(item->m_labelName,followInfo.m_tableName.c_str()) ;
	//玩家数量
	sprintf(buff,"%d",followInfo.m_roleCountInGame) ;
	SetLabelString(item->m_LableroleCount,buff) ;


	item->m_spriteReBuy->setVisible(followInfo.m_isRebuy) ;
	item->m_spriteAddon->setVisible(followInfo.m_isAddOn) ;
	item->m_spriteFloatMatch->setVisible(followInfo.m_isFloatMatch) ;
	item->m_spriteDelayMatch->setVisible(followInfo.m_isdelayRegist) ;

	item->m_vecIcon.clear() ;
	if (followInfo.m_isRebuy)
	{
		item->m_vecIcon.push_back(item->m_spriteReBuy);
	}
	if (followInfo.m_isAddOn)
	{
		item->m_vecIcon.push_back(item->m_spriteAddon);
	}
	if (followInfo.m_isdelayRegist)
	{
		item->m_vecIcon.push_back(item->m_spriteDelayMatch);
	}
	if (followInfo.m_isFloatMatch)
	{
		item->m_vecIcon.push_back(item->m_spriteFloatMatch);
	}

	for (int i=0;i<item->m_vecIcon.size();i++)
	{
		Node* node = item->m_vecIcon.at(i) ;
		node->setVisible(true) ;
		node->setPositionX(270+35*i) ;
	}


	item->m_spriteRoleCount->setVisible(followInfo.m_SiginState!=3) ;
	item->m_LableroleCount->setVisible(followInfo.m_SiginState!=3) ;

	//比赛时间类型
	item->m_spriteTimeBG->setVisible(false) ;	
	item->m_spriteJijiangkaishiBg->setVisible(false) ;
	item->m_spriteYanShikaishiBg->setVisible(false) ;
	item->m_lable_titleDay ->setVisible(false) ;
	item->m_lable_TimeDes->setVisible(false) ;
	item->m_lable_statrTime->setVisible(false) ;
	item->m_wait->setVisible(false) ;
	item->m_pBtJiaRu->setVisible(false) ;
	item->m_labelNotStart->setVisible(false) ; 
	item->m_labelNotStartDes->setVisible(false) ; 
	item->m_aniBtJiaru->setVisible(false) ;
	

	for (int i=0;i<4;i++)
	{
		item->m_lable_JiJiangCountDown[i]->setVisible(false) ;
	}




	if (followInfo.m_showCountDown) //显示倒计时
	{
	
		item->m_lable_TimeDes->setVisible(true) ;
		SetLabelString(item->m_lable_TimeDes,followInfo.m_TimeDes.c_str()) ;
		if (followInfo.m_isdelayRegist) 
		{
			item->m_spriteYanShikaishiBg->setVisible(true) ;//延迟开始背景
			item->m_lable_TimeDes->setTextColor(Color4B(0x44,0xb0,0xe3,0xff)) ;//44b0e3
		}
		else
		{
			item->m_spriteJijiangkaishiBg->setVisible(true) ;//即将开始背景
			item->m_lable_TimeDes->setTextColor(Color4B(0x50,0xbd,0x2e,0xff)) ;
		}
		//打开倒计时面板
		for (int i=0;i<4;i++)
		{
			item->m_lable_JiJiangCountDown[i]->setVisible(true) ;
		}
	}
	else
	{
		item->m_spriteTimeBG->setVisible(true) ;//未开始背景
		item->m_lable_titleDay->setVisible(true) ; //今天等
		item->m_lable_statrTime->setVisible(true) ;//具体开始时间

		SetLabelString(item->m_lable_titleDay,followInfo.m_titleDay.c_str()) ;
		item->m_lable_titleDay->setTextColor(Color4B(0x50,0xbd,0x2e,0xff)) ;
		SetLabelString(item->m_lable_statrTime,followInfo.m_strTime.c_str());
	}
	doGuoDong(false,item->m_pBtJiaRu) ;
	////0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散  7延迟报名
	if (followInfo.m_SiginState == 0)
	{
		item->m_labelNotStart->setVisible(true) ;//未开始
		item->m_labelNotStartDes->setVisible(true) ;//赛前多少小时开始
		
		sprintf(buff,"赛前%d小时开放",followInfo.m_matchTimeBeforeStart) ;
		SetLabelString(item->m_labelNotStartDes,buff) ;

	}else if (followInfo.m_SiginState == 1)
	{
		item->m_pBtJiaRu->setVisible(true) ;
		SetLabelString(item->m_labelJiaru,"报名") ;
	}else if (followInfo.m_SiginState == 2)
	{

	}else if (followInfo.m_SiginState == 3)
	{
		SetLabelString(item->m_wait,"已结束") ;
		item->m_wait->setVisible(true) ;
	}else if (followInfo.m_SiginState == 4)
	{
		SetLabelString(item->m_wait,"等待开始") ;
		item->m_wait->setVisible(true) ;

	}else if (followInfo.m_SiginState == 5)
	{
		item->m_pBtJiaRu->setVisible(true) ;
		SetLabelString(item->m_labelJiaru,"进入") ;
		doGuoDong(true,item->m_pBtJiaRu) ;
		item->m_aniBtJiaru->setVisible(true) ;
	}else if (followInfo.m_SiginState == 6)
	{

	}else if (followInfo.m_SiginState == 7)
	{
		item->m_pBtJiaRu->setVisible(true) ;
		SetLabelString(item->m_labelJiaru,"报名") ;
	}
	item->m_spriteMoney->setVisible(!(followInfo.m_myTicket>=1)) ;
	item->m_lableMoney->setVisible(!(followInfo.m_myTicket>=1)) ;

	item->m_spriteFreesticket->setVisible(followInfo.m_myTicket>=1) ;
	item->m_labelFreeStickCount->setVisible(followInfo.m_myTicket>=1) ;
	item->m_labelMyStickCount->setVisible(followInfo.m_myTicket>=1) ;
	//报名费用

	if (followInfo.m_isFreeMathch)
	{
		item->m_spriteMoney->setVisible(false) ;
		item->m_lableMoney->setVisible(true) ;

		SetLabelString(item->m_lableMoney,"免费") ;
	}
	else
	{
		//德州币门票都可用
		if (followInfo.m_canUseMoney && followInfo.m_canUseTicket)
		{
			if (followInfo.m_myTicket>=1)
			{
				sprintf(buff,"X%d",1) ;
				SetLabelString(item->m_labelFreeStickCount,buff) ;
				sprintf(buff,"您拥有%d张",followInfo.m_myTicket) ;
				SetLabelString(item->m_labelMyStickCount,buff) ;
			}
			else
			{
				if (followInfo.m_fee!=0)
				{
					sprintf(buff,"%s+%s",FormatCash4(followInfo.m_entryFee).c_str(),FormatCash4(followInfo.m_fee).c_str()) ;
				}
				else
				{
					sprintf(buff,"%s",FormatCash4(followInfo.m_entryFee).c_str()) ;
				}

				SetLabelString(item->m_lableMoney,buff) ;
			}
		}

		//可用德州币不可用门票
		if (followInfo.m_canUseMoney && !followInfo.m_canUseTicket)
		{
			item->m_spriteMoney->setVisible(true) ;
			item->m_lableMoney->setVisible(true) ;
			if (followInfo.m_fee!=0)
			{
				sprintf(buff,"%s+%s",FormatCash4(followInfo.m_entryFee).c_str(),FormatCash4(followInfo.m_fee).c_str()) ;
			}
			else
			{
				sprintf(buff,"%s",FormatCash4(followInfo.m_entryFee).c_str()) ;
			}

			SetLabelString(item->m_lableMoney,buff) ;
		}

		//可用门票不可用德州币
		if (followInfo.m_canUseTicket && followInfo.m_canUseMoney)
		{
			sprintf(buff,"X%d",1) ;
			SetLabelString(item->m_labelFreeStickCount,buff) ;
			sprintf(buff,"您拥有%d张",followInfo.m_myTicket) ;
			SetLabelString(item->m_labelMyStickCount,buff) ;
		}
	}


	//加入回调
	item->m_pBtJiaRu->SetHitCB(std::bind(&GameMttHall::menuClickSigin, this, followInfo));
	//背景回调
	item->m_pBtDetail->SetHitCB(std::bind(&GameMttHall::showTableState, this,followInfo));


}



void GameMttHall::updateItemList(GameMttTableInfo& followInfo)
{
	for(auto item:m_vecSngTableListInfoNew)
	{
		if(item->m_tableId == followInfo.m_tableID)
		{
			UpdateItem(item, followInfo);
		}
	}
}

void GameMttHall::updateItemTime(float DelayTime)
{
	//if (!this->isVisible())
	//{
	//	return ;
	//}
	for (auto item:m_vecSngTableListInfoNew)
	{
		if (item->m_pSpriteBgItem->isVisible()&&item->m_CountDown)
		{
			bool flag =item->updateTime(DelayTime) ;
			if (flag)
			{
				//removeItemByTableID(item->m_tableId) ;
			}
		}
	}
	GameMttTableInfo mFirsttable ;
	bool moveItem =false;
	//更新时间数据
	for (auto& table:g_globalMttHallInfo.m_mttTableInfo)
	{
		if (table.m_realTime<0)
		{
			//移除过期赛事
			//g_globalMttHallInfo.m_mttTableInfo.erase(iter);
			//updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;
			////更新游戏类型
			//g_globalMttHallInfo.m_mttTableType.clear() ;
			//break ;
			continue;
		}
		table.m_realTime-=1 ;
		//一分钟移动到第一页
		if (table.m_realTime==60)
		{
			mFirsttable = table ;
			moveItem = true ;
		}
	}

	if (moveItem)
	{
		moveItemToFirst(mFirsttable) ;
	}
	//有空加状态控制  去更新数据
	//updateCurUsedMatchInfo() ;
}

bool GameMttHall::MttTableItem::updateTime(float DelayTime)
{
	if (m_time<0)
	{
		//倒计时结束 请求数据
		//removeItemByTableID(m_tableId) ;

		//for (int i=0; i<4; i++)
		//{
		//	if (m_lable_JiJiangCountDown[i]->isVisible())
		//	{
		//		m_lable_JiJiangCountDown[i]->setVisible(false) ;
		//	}
		//}
		return true;
	}
	int lastTime = m_time;

	//int day = lastTime/60/60/24;
	//int i_hours = lastTime/60/60%24;
	int i_min = lastTime/60%60;
	int i_sec = lastTime%60;

	m_timeShow[0] = i_min/10 ;
	m_timeShow[1] = i_min%10 ;

	m_timeShow[2] = i_sec/10 ;
	m_timeShow[3] = i_sec%10 ;

	for (int i=0; i<4; i++)
	{
		char buff[64];
		sprintf(buff,"%d",m_timeShow[i]) ;
		SetLabelString(m_lable_JiJiangCountDown[i],buff) ;
	}
	m_time-=1;
	return false ;
}


/************************************************************************/
/* 报名                                                                     */
/************************************************************************/
void GameMttHall::menuClickSigin(GameMttTableInfo& followInfo)
{
	if (m_isInAction)
	{
		return ;
	}
	g_globaMttGameInfo.setCurMttMatchInfo(followInfo);


	//0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散
	if (followInfo.m_SiginState==5)
	{
		g_pGameFrame->doJoinMttMatch(followInfo.m_tableID,followInfo.m_isdelayRegist);
		return ;
	}


	bool flag = false ;
	bool flagNoMoney = false ;
	bool flagNoTicket = false ;
	int allcash = followInfo.m_fee +followInfo.m_entryFee ;

	if (followInfo.m_canUseMoney && followInfo.m_canUseTicket)
	{
		if (g_globalMyData.GetCash()>=allcash)
		{
			flag = true ;
		}else
		{
			flagNoMoney = true ;
		}
		if (followInfo.m_myTicket>0)
		{
			flag = true ;
		}
		else
		{
			flagNoTicket = true ;
		}
	} else if (followInfo.m_canUseMoney && !followInfo.m_canUseTicket)
	{
		if (g_globalMyData.GetCash()>=allcash)
		{
			flag = true ;
		}
		else
		{
			flagNoMoney = true ;
		}
	}else if (!followInfo.m_canUseMoney && followInfo.m_canUseTicket)
	{
		if (followInfo.m_myTicket>0)
		{
			flag = true ;
		}
		else
		{
			flagNoTicket = true ;
		}
	}
	//免费比赛
	if (followInfo.m_isFreeMathch)
	{
		flag = true ;
	}


	if (flag)
	{
		Json::Value param;
		param["id"] = followInfo.m_tableID;
		SendClientRequest(ClientRequest::kMttMatchSiginUp,param);
	}
	else
	{
		if (flagNoMoney)
		{
			PopTipWithBt2("亲爱的玩家，您的德州币不够啦，要去商城买点吗？商城折扣活动火热进行中","暂不需要","进入商城",[](unsigned ubt)
			{
				if(ubt == 1)
				{
					Json::Value param;
					param["idx"] = 0;
					SendClientRequest(ClientRequest::kPopMall, param);
				}
			});
		} else if (flagNoMoney)
		{
			PopTipWithBt1("门票不足","确定",nullptr) ;
		}
	}

}
/************************************************************************/
/* 牌局状态                                                                     */
/************************************************************************/
void GameMttHall::showTableState( GameMttTableInfo& followInfo)
{

	showMttMatchState(followInfo) ;
}

void GameMttHall::updateMenuIndex(unsigned index)
{

	m_button_WillSiginUp->setEnabled(0!=index) ;
	m_button_hasSiginUp->setEnabled(1!=index) ;
	m_button_gameover->setEnabled(2!=index) ;

	m_button_WillSiginUp->setBright(0!=index) ;
	m_button_hasSiginUp->setBright(1!=index) ;
	m_button_gameover->setBright(2!=index) ;


	m_Text_WillSiginUp->setTextColor((0!=index?Color4B(0x94,0xd0,0xa3,0xff):Color4B(0xff,0xff,0xff,0xff))) ;
	m_Text_hasSiginUp->setTextColor((1!=index?Color4B(0x94,0xd0,0xa3,0xff):Color4B(0xff,0xff,0xff,0xff))) ;
	m_Text_gameover->setTextColor((2!=index?Color4B(0x94,0xd0,0xa3,0xff):Color4B(0xff,0xff,0xff,0xff))) ;

}
/************************************************************************/
/* 下拉 牌局类型界面                                                                     */
/************************************************************************/
void GameMttHall::showMttTypeSelect(bool flag)
{
	if (m_mttTypeSelect == nullptr)
	{
		m_mttTypeSelect = MttTypeSelect::create() ;
		m_mttTypeSelect -> ignoreAnchorPointForPosition(false);
		m_mttTypeSelect -> setAnchorPoint(Point(0,1));
		m_mttTypeSelect->setPosition(108,398.5);
		this->addChild(m_mttTypeSelect,ZOrder_GameTypeSelect);
	}
	layerClip->setVisible(flag) ;
	m_mttTypeSelect->setVisible(flag) ;

	if (flag)
	{
		updateGameTypeCount() ;
		m_mttTypeSelect->doMoveIn() ;
	}
	m_button_MathcType->setBright(!flag);
}

/************************************************************************/
/* 规则界面                                                                     */
/************************************************************************/
void GameMttHall::showMttRule(bool flag)
{
	if (m_mttrule == nullptr)
	{
		m_mttrule = MttRule::create() ;
		m_mttrule -> ignoreAnchorPointForPosition(false);
		auto itemSz = m_mttrule->getContentSize();;
		m_mttrule -> setAnchorPoint(Point(0.5,0.5));
		m_mttrule->setPosition(GameData::getInstance()->getGameWidth()/2 ,GameData::getInstance()->getGameHeight()/2 );
		addChild(m_mttrule,ZOrder_Rule);	
	}

	m_mttrule->setVisible(flag) ;
	showBlack(flag) ;
}
/************************************************************************/
/* 翻页状态更新                                                                     */
/************************************************************************/
void GameMttHall::updatePageButtonShow()
{
	if (m_curPageIndex==0)
	{
		m_buttonPageUp->setOpacity(100) ;
		m_buttonPageUp->setEnabled(false) ;
	}
	else
	{
		m_buttonPageUp->setOpacity(255) ;
		m_buttonPageUp->setEnabled(true) ;
	}

	if (m_curPageIndex==m_maxPageCount-1||m_maxPageCount==1||m_maxPageCount==0)
	{
		m_buttonPageDown->setOpacity(100) ;
		m_buttonPageDown->setEnabled(false) ;
	}
	else
	{
		m_buttonPageDown->setOpacity(255) ;
		m_buttonPageDown->setEnabled(true) ;
	}
}




void GameMttHall::OnUpdateMyData(unsigned uDataType)
{
	if (uDataType == WAITCASHCHANGE)
	{
		setUITextString(m_textMoney, FormatCash4(g_globalMyData.GetCash())) ;
	}
}
/************************************************************************/
/* 显示本厂赛况                                                                     */
/************************************************************************/
void GameMttHall::showMttMatchState(GameMttTableInfo& followInfo)
{
	//g_globaMttGameInfo.m_curMttTable = followInfo ;
	g_globaMttGameInfo.setCurMttMatchInfo(followInfo);
	g_pGameFrame->showMttMatchState(true, 3) ;
}

void GameMttHall::updateCurUsedMatchInfo()
{
	updateMatchInfoByState() ;

	int type =  getCurMttType() ;
	m_vecCurUesdInfoByType.clear() ;

	if (type ==0) //显示全部
	{
		m_vecCurUesdInfoByType = m_vecFirstStateInfo;
	}
	else
	{
		for (auto table:m_vecFirstStateInfo)
		{
			int gameType = table.m_GameType ;
			if (gameType == type)
			{
				m_vecCurUesdInfoByType.push_back(table);
			}
		}
	}
	m_vecCurUesdTableInfo = m_vecCurUesdInfoByType ;
	if (m_useSort)
	{
		std::sort( m_vecCurUesdInfoByType.begin(), m_vecCurUesdInfoByType.end(), compareByTime);
		m_vecCurUesdInfoByTime = m_vecCurUesdInfoByType ;
		m_vecCurUesdTableInfo = m_vecCurUesdInfoByTime ;
	}

}

void GameMttHall::updateMatchViewByGameType(int type,bool updateTime ,unsigned pageIndex)
{
	if (!updateTime)
	{
		m_useSort = false ;
		m_button_MatchTime->setBright(!m_useSort);
	}
	m_hasSort = false ;
	setCurMttType(type);
	m_vecCurUesdInfoByType.clear() ;
	updateMatchInfoByState() ;
	if (type ==0) //显示全部
	{
		m_vecCurUesdInfoByType = m_vecFirstStateInfo;
	}
	else
	{
		for (auto table:m_vecFirstStateInfo)
		{
			int gameType = table.m_GameType ;
			if (gameType == type)
			{
				m_vecCurUesdInfoByType.push_back(table);
			}
		}
	}
	updatePageCountAndView(m_vecCurUesdInfoByType,updateTime,pageIndex);
}


void GameMttHall::updateItemByTableID(unsigned tableid ,unsigned state,std::string title,std::string reason,bool delayRegist,int delayRegistTime,bool showCountDown)
{
	 for(std::vector<GameMttTableInfo>::iterator iter = g_globalMttHallInfo.m_mttTableInfo.begin(); iter!=g_globalMttHallInfo.m_mttTableInfo.end();++iter)
	 {
		 if (iter->m_tableID == tableid)
		 {

			 //可进入更新赛况
			 if(iter->m_SiginState!=5&&state)
			 {
				 if (g_pGameFrame->isMttMatchStateVisible())
				 {
					 if (g_pGameFrame->getMttMatchStateIndex() == 0 && g_globaMttGameInfo.m_curMttTable.m_tableID == tableid )
					 {
						 g_globaMttGameInfo.m_curMttTable.m_SiginState = state;
						 g_pGameFrame->updateMttMatchState();
					 }
				 }
			 }


			 iter->m_SiginState = state ;////0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散 //7延迟比赛
			 iter->m_showCountDown = showCountDown ;
			 if (state == 2||state == 6) //已开始 和解散的从列表中删除  
			 {
				 if (state == 6)
				 {
					 //对已报名的发解散消息
					 //if (iter->m_SiginState == 4 ||iter->m_SiginState == 5 ||iter->m_SiginState ==2)
					 //{
						//  g_pGameFrame->showMttMatchDisMissTips(true,iter->m_tableName,reason) ;
					 //}
				 }
				 g_globalMttHallInfo.m_mttTableInfo.erase(iter);
				 updateMatchInfoByState() ; //更新一级数据
				 updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;//更新二级数据
				 	
				 return ;
			 }
			 //已结束的 重新分类
			 if (state ==3)
			 {
				 //显示 更具服务器数据
				 iter->m_showCountDown = false ;
				 updateMatchInfoByState() ; //更新一级数据
				 updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;//更新二级数据
			 }
			

			 if( title.compare("") != 0)
			 {
				  iter->m_TimeDes = title ;
			 }

			 //bool isdelayRegist = delayRegistTime>0 ;
			  iter->m_realTime = delayRegistTime ;
			 //普通赛事变延迟开始
			 if (!iter->m_isdelayRegist&&delayRegist)
			 {
				 iter->m_isdelayRegist = delayRegist ;
				 moveItemToFirst(*iter);

                 if (g_pGameFrame->isMttMatchStateVisible())
                 {
                     if (g_pGameFrame->getMttMatchStateIndex() == 0 && g_globaMttGameInfo.m_curMttTable.m_tableID == tableid )
                     {
                         g_globaMttGameInfo.m_curMttTable.m_isdelayRegist = delayRegist;
                          g_pGameFrame->updateMttMatchState();
                     }
                 }
				 return ;
			 }
			 else
			 {
				 updateCurUsedMatchInfo() ;
			 }
		
			for(auto item:m_vecSngTableListInfoNew)
			{
				if (item->m_tableId == tableid)
				{
					UpdateItem(item,*iter);  //更新当前显示中的列表数据
					return ;
				}
			}
			 break;
		 }
	 }
	 		
	 //if (!m_useSort)
	 //{
		//updateMatchInfoByState() ; //更新一级数据
		//updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;
	 //}

}

void GameMttHall::removeItemByTableID(unsigned tableid)
{
	std::vector<GameMttTableInfo>::iterator iter ;
	for( iter = g_globalMttHallInfo.m_mttTableInfo.begin(); iter!=g_globalMttHallInfo.m_mttTableInfo.end();++iter)
	{
		if (iter->m_tableID == tableid)
		{
			g_globalMttHallInfo.m_mttTableInfo.erase(iter);
			//updateMatchInfoByState() ; //更新一级数据
			//updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;//更新二级数据

			break ;
		}
	}

}

void GameMttHall::updateGameTypeCount()
{
	updateMatchInfoByState() ;
	MatchState firstState = m_curSelectState ;

	g_globalMttHallInfo.m_mttTableType.clear() ;
	g_globalMttHallInfo.m_mttTableType.push_back(0) ; //0 是全部
	std::vector<GameMttTableInfo>::iterator iter ;
	for( iter = m_vecFirstStateInfo.begin(); iter!=m_vecFirstStateInfo.end();++iter)
	{
		bool flag = false ;
		for (int j=0; j<g_globalMttHallInfo.m_mttTableType.size(); j++)
		{
			int type = g_globalMttHallInfo.m_mttTableType.at(j) ;
			//0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散 7//延迟比赛
		
			if (iter->m_GameType==type)
			{
				flag = true ;
	
			}

		}
		if (!flag)
		{
			g_globalMttHallInfo.m_mttTableType.push_back(iter->m_GameType);
		}
	}
	std::sort( g_globalMttHallInfo.m_mttTableType.begin(), g_globalMttHallInfo.m_mttTableType.end(), compareByType);
}

void GameMttHall::moveItemToFirst(GameMttTableInfo table)
{

	std::vector<GameMttTableInfo>::iterator iter ;
	for( iter = g_globalMttHallInfo.m_mttTableInfo.begin(); iter!=g_globalMttHallInfo.m_mttTableInfo.end();++iter)
	{
		if (iter->m_tableID == table.m_tableID)
		{
			g_globalMttHallInfo.m_mttTableInfo.erase(iter);
			g_globalMttHallInfo.m_mttTableInfo.insert( g_globalMttHallInfo.m_mttTableInfo.begin(), table);  //插入元素到开头
			updateCurUsedMatchInfo() ;
			updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;
			break ;
		}
	}

}
/************************************************************************/
/* 获取数据后进行分类初始化                                                                     */
/************************************************************************/
void GameMttHall::updateMttHallList(float dealTime)
{
	setShowItems(false) ;
	long startTime = getCurrentTime() ;
	this->schedule(schedule_selector(GameMttHall::updateItemTime),1.0f);
	initHallInfo() ;
	updateMttHallInfo() ;//更新上方显示数据
	//setCurMatchState(MatchState_IsSigining) ;//一次状态
	m_curSelectState = MatchState_IsSigining ;
	setCurMttType(0) ;//二级状态

	
	float endTime = (getCurrentTime()-startTime)/1000.00f +dealTime ; //获取总差值
	for (auto& table:g_globalMttHallInfo.m_mttTableInfo)
	{
		table.m_realTime-=(endTime+0.5f) ;
	}
	updateMatchViewByGameType(0);
	updateItemTime(0);//先更新一次
	setShowItems(true) ;
	callHallAction() ;


	//leftTime-=(mpTime+0.5f) ;
}


void GameMttHall::updateCurViewList(float dealTime)
{
	//float endTime = (getCurrentTime()-startTime)/1000.00f +dealTime ; //获取总差值
	for (auto& table:g_globalMttHallInfo.m_mttTableInfo)
	{
		table.m_realTime-=(dealTime+0.5f) ;

		for(auto item:m_vecSngTableListInfoNew)
		{
			if (item->m_tableId == table.m_tableID)
			{
				UpdateItem(item,table);  
			}
		}
	}
	updateCurUsedMatchInfo();

}

void GameMttHall::updateMatchInfoByState()
{

	//0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散 7延迟报名

	m_vecFirstStateInfo.clear() ;
	MatchState curState = getCurSelectState() ;
	for (auto table:g_globalMttHallInfo.m_mttTableInfo)
	{
		int state = table.m_SiginState ;
		if (curState == MatchState_IsSigining)
		{
			if (state ==0 || state==1 || state ==4 || state ==5 ||state ==7)
			{
				m_vecFirstStateInfo.push_back(table);
			}
		}
		if (curState == MatchState_hasSigin)
		{
			if (state ==4 || state ==5)
			{
				m_vecFirstStateInfo.push_back(table);
			}
		}
		if (curState == MatchState_GameOver)
		{
			if (state ==3)
			{
				m_vecFirstStateInfo.push_back(table);
			}
		}
	}
}

void GameMttHall::setCurMatchState(MatchState state)
{
	m_curSelectState = state;
	updateMatchInfoByState() ;
}

/************************************************************************/
/* 更新当前比赛显示类型       暂时 弃用                                                              */
/************************************************************************/
int GameMttHall::checkTimeType(GameMttTableInfo &table)
{

	////0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散 7延迟报名
	int type = 0 ;

	bool delayRegist = table.m_isdelayRegist ;
	int minLeftTime = 60 ;
	if (table.m_SiginState ==3)
	{
		table.m_TimeType =8 ;
	}
	if (delayRegist&&table.m_showCountDown &&table.m_SiginState ==5)
	{
		table.m_TimeType =7 ;
	}
	if (delayRegist&&table.m_showCountDown &&table.m_SiginState ==1)
	{
		table.m_TimeType =6 ;
	}

	if (!delayRegist&&table.m_showCountDown &&table.m_SiginState ==5)
	{
		table.m_TimeType =5 ;
	}

	if (!delayRegist&&table.m_showCountDown &&table.m_SiginState ==4&&table.m_realTime>minLeftTime)
	{
		table.m_TimeType =4 ;
	}

	if (!delayRegist&&table.m_realTime!=0 &&table.m_SiginState ==1)
	{
		table.m_TimeType =3 ;
	}

	if (!table.m_showCountDown &&table.m_SiginState ==4)
	{
		table.m_TimeType =2 ;
	}
	if (!table.m_showCountDown &&table.m_SiginState ==1)
	{
		table.m_TimeType =1 ;
	}
	if (!table.m_showCountDown &&table.m_SiginState ==0)
	{
		table.m_TimeType =0 ;
	}
	type = table.m_TimeType ;

	return type ;
}

void GameMttHall::updateMttHallInfo()
{

	setUITextString(m_textName, g_globalMyData.GetNickName().c_str()) ;
	setUITextString(m_textMoney, FormatCash4(g_globalMyData.GetCash())) ;

	char tips[64] ;
	int rank = g_globalMyData.getMttMasterRank() ;

	if (rank == 0 )
	{
		setUITextString(m_textrank, "排名：暂无") ;
	}
	else
	{
		sprintf(tips,"排名：%d",rank) ;
		setUITextString(m_textrank, tips) ;
	}

	int score = g_globalMyData.getMttMasterScore() ;
	sprintf(tips,"大师分：%d",score) ;
	setUITextString(m_textDashiRank, tips) ;


	if (g_globalMyData.GetSkinID() == 0) {
		SetIconUrl(m_spriteHero, g_globalMyData.GetIconURL());
	} else {
		SetIconUrl(m_spriteHero, g_globalMyData.GetSkin(g_globalMyData.GetSkinID()).m_strIconUrl);
	}

}


void GameMttHall::updateTicket()
{
	//updateCurUsedMatchInfo() ;
	//m_MttChangedTicket.m_allTicket


	for (auto& tableInfo:g_globalMttHallInfo.m_mttTableInfo)
	{
		for (auto table:m_MttChangedTicket.m_allTicket)
		{
			if (tableInfo.m_tableID == table.id)
			{

				tableInfo.m_myTicket = table.num ;

				for(auto item:m_vecSngTableListInfoNew)
				{
					if (item->m_tableId == tableInfo.m_tableID)
					{
						UpdateItem(item,tableInfo);  

					}
					
				}
				
			}
		}
	}
	updateCurUsedMatchInfo() ;




}

void GameMttHall::updateMatchPlayerNum(unsigned tableid ,unsigned num)
{

	for (auto& tableInfo:g_globalMttHallInfo.m_mttTableInfo)
	{

			if (tableInfo.m_tableID == tableid)
			{

				tableInfo.m_roleCountInGame = num ;

				for(auto item:m_vecSngTableListInfoNew)
				{
					if (item->m_tableId == tableInfo.m_tableID)
					{
						//UpdateItem(item,tableInfo);  
						char buff[64];
						sprintf(buff,"%d",tableInfo.m_roleCountInGame) ;
						SetLabelString(item->m_LableroleCount,buff) ;
					}
				}
			}
	}
}

void GameMttHall::updateFixTime(float time)
{


	for (auto& table:g_globalMttHallInfo.m_mttTableInfo)
	{
		table.m_realTime-=(time+0.5f) ;
	}

	setEnterFormHall(false) ;
	SendClientRequest(ClientRequest::kMttHallInfo);



}

void GameMttHall::updateCurViewListByQuitMatch()
{

	if (getCurSelectState() == MatchState_hasSigin)
	{
		//updateMatchInfoByState() ;

		 updateMatchViewByGameType(m_curGameType,m_useSort,m_curPageIndex) ;//更新二级数据
	}

}

void GameMttHall::doGuoDong(bool flag,cocos2d::Node* node)
{
	//if (!flag)
	//{
	//		node->stopAllActions() ;
	//}
	//else
	//{
	//	node->stopAllActions() ;
	//	auto scaleX_max = ScaleTo::create(0.8,1.08,0.97);
	//	auto scaleX_min = ScaleTo::create(1.1,0.97,1.08);

	//	node->runAction(RepeatForever::create(Sequence::create(scaleX_max,scaleX_min,NULL))) ;
	//}


}
void GameMttHall::createGuideTips()
{
    //MTTHelp,//MTT帮助
    if (!GuideInfo->readGuideInfo(MTTHelp))
    {
        auto pos = m_buttonhelp->convertToWorldSpace(Vec2(0,0));
        auto sz = m_buttonhelp->getContentSize();
        auto guideTips = GuideTips::create("guidetips/mtthelptips.png", "guidetips/mtthelpflag.png");
        guideTips->setFlagPosition(Vec2(pos.x+sz.width/2, pos.y+sz.height/2));
        guideTips->setBgPosition(Vec2(pos.x-85, pos.y-112));
        auto pClip = CreateClippingMask("guidetips/mtthelpmask.png", Vec2(pos.x+sz.width/2, pos.y+sz.height/2), guideTips);
        this->addChild(pClip, 20);
        guideTips->setTipsID(MTTHelp);
        GameTipsManager::getInstance()->addTips(pClip);
    }
    //MTTSign,//MTT操作导航栏
    if (!GuideInfo->readGuideInfo(MTTSign))
    {
        auto pos = m_button_hasSiginUp->convertToWorldSpace(Vec2(0,0));
        auto sz = m_button_hasSiginUp->getContentSize();
        auto guideTips = GuideTips::create("guidetips/mttsigntips.png", "guidetips/mttsignflag.png");
        guideTips->setFlagPosition(Vec2(pos.x+sz.width/2, pos.y+sz.height/2));
        guideTips->setBgPosition(Vec2(pos.x+60, pos.y-125));
        auto pClip = CreateClippingMask("guidetips/mttsignmask.png", Vec2(pos.x+sz.width/2, pos.y+sz.height/2), guideTips);
        this->addChild(pClip, 19);
        guideTips->setTipsID(MTTSign);
        GameTipsManager::getInstance()->addTips(pClip);
    }
    //MTTFilter,//MTT赛事筛选
    if (!GuideInfo->readGuideInfo(MTTFilter))
    {
        auto pos = m_button_MathcType->convertToWorldSpace(Vec2(0,0));
        auto sz = m_button_MathcType->getContentSize();
        auto guideTips = GuideTips::create("guidetips/mttfiltertips.png", "guidetips/mttfilterflag.png");
        guideTips->setFlagPosition(Vec2(pos.x+sz.width/2, pos.y+sz.height/2));
        guideTips->setBgPosition(Vec2(pos.x+140, pos.y-115));
        auto pClip = CreateClippingMask("guidetips/mttfiltermask.png", Vec2(pos.x+sz.width/2, pos.y+sz.height/2), guideTips);
        this->addChild(pClip, 18);
        guideTips->setTipsID(MTTFilter);
        GameTipsManager::getInstance()->addTips(pClip);
    }

    //MTTDescription,//MTT具体场次信息
    if (!GuideInfo->readGuideInfo(MTTDescription))
    {
        auto posScrolView = m_pScrollView->convertToWorldSpace(Vec2(0,0));
        auto pos = Vec2(posScrolView.x+sizeListView.width/2, posScrolView.y+sizeListView.height);
        auto guideTips = GuideTips::create("guidetips/mttdestips.png", "guidetips/mttdesflag.png");
        guideTips->setFlagPosition(Vec2(pos.x, pos.y-TableHeight/2-10));
        guideTips->setBgPosition(Vec2(pos.x, pos.y-TableHeight-120));
        auto pClip = CreateClippingMask("guidetips/mttdesmask.png", Vec2(pos.x, pos.y-TableHeight/2-10), guideTips);
        this->addChild(pClip, 17);
        guideTips->setTipsID(MTTDescription);
        GameTipsManager::getInstance()->addTips(pClip);
    }

    //MTTPageUpAndDown//MTT上下翻页
    if(!GuideInfo->readGuideInfo(MTTPageUpAndDown))
    {
        auto pos1 = m_buttonPageUp->convertToWorldSpace(Vec2(0,0));
        auto pos2 = m_buttonPageDown->convertToWorldSpace(Vec2(0,0));
        auto sz = m_buttonPageDown->getContentSize();
        auto guideTips = GuideTips::create("guidetips/mttpagetips.png", "guidetips/mttpageflag.png");
        guideTips->setFlagPosition(Vec2(pos2.x+sz.width/2, pos2.y+sz.height/2+2));
        guideTips->setBgPosition(Vec2(pos2.x-15, pos2.y-120));
        guideTips->addFlagSpriteAndPos("guidetips/mttpageflag.png", Vec2(pos1.x+sz.width/2, pos1.y+sz.height/2+2));
        auto pClip = CreateClippingMask("guidetips/mttpagemask.png", Vec2(pos2.x+sz.width/2, pos2.y+sz.height/2+2), guideTips, "guidetips/mttpagemask.png", Vec2(pos1.x+sz.width/2, pos1.y+sz.height/2+2));
        this->addChild(pClip, 16);
        guideTips->setTipsID(MTTPageUpAndDown);
        GameTipsManager::getInstance()->addTips(pClip);
    }
}