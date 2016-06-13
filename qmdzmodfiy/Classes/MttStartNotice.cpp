#include "MttStartNotice.h"
#include "Util.h"
#include "GameData.h"
#include "MenuItemScale9Sprite.h"
#include "LineLabel.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
USING_NS_CC ;
bool MttStartNotice::init()
{
	if (!Layer::init())
	{
		return false;
	}
	this->setContentSize(Size(GameData::getInstance()->getGameWidth(),55)) ;
	Size sz = this->getContentSize() ;
	LayerColor* colorBg = LayerColor::create(Color4B(0x00, 0x00, 0x00, 0xcc), GameData::getInstance()->getGameWidth(), sz.height) ;
	colorBg->ignoreAnchorPointForPosition(false) ;
	colorBg->setAnchorPoint(Point(0.5,0.5)) ;
	colorBg->setPosition(GameData::getInstance()->getGameWidth()/2,sz.height/2) ;
	this->addChild(colorBg);

	ccMenuCallback menuCB2 = CC_CALLBACK_1(MttStartNotice::onMenuItemSelected, this);
	//m_btclose =MenuItemFrameSprite::create("quickshop/btok.png", 3,menuCB);
	m_bt_quiteMatch =MenuItemScale9Sprite::create(4, Size(104, 55),menuCB2);
	m_bt_quiteMatch->setPosition(856,sz.height/2);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(26,"退赛",Vec2(0.5,0.5),m_bt_quiteMatch->getContentSize().width/2,m_bt_quiteMatch->getContentSize().height/2,Color4B::WHITE,m_bt_quiteMatch,0);

	m_bt_joinMatch =MenuItemScale9Sprite::create(1, Size(127,55),menuCB2);
	m_bt_joinMatch->setPosition(996,sz.height/2);
	CreateLabelMSYHBDAndAnchorPosClrAddTo(26,"立即进入",Vec2(0.5,0.5),m_bt_joinMatch->getContentSize().width/2,m_bt_joinMatch->getContentSize().height/2,Color4B::WHITE,m_bt_joinMatch,0);

	m_bt_close   = MenuItemImage::create("gamemtt/mtt_tips_close.png","gamemtt/mtt_tips_close.png",menuCB2);
	m_bt_close->setPosition(1109,sz.height/2);


	auto pMenu = Menu::create(m_bt_quiteMatch,m_bt_joinMatch,m_bt_close,nullptr);
	pMenu->setPosition(Vec2::ZERO);
	colorBg->addChild(pMenu, 1);


	m_lineLabelTips = LineLabel::create() ;
	m_lineLabelTips->ignoreAnchorPointForPosition(false) ;
	m_lineLabelTips->setAnchorPoint(Point(1,0.5)) ;
	m_lineLabelTips->setPosition(792,sz.height/2) ;
	colorBg->addChild(m_lineLabelTips);

	m_time = 0 ;
	m_matchName = "" ;
	showOneMins = false ;
	return true ;
}

void MttStartNotice::onMenuItemSelected(cocos2d::Ref * ref)
{
	if (ref == m_bt_quiteMatch)
	{
		this->stopAllActions() ;
		this->setVisible(false) ;

		Json::Value param;
		param["id"] = m_tableID;
		SendClientRequest(ClientRequest::kMttMatchQuit,param);
		showOneMins = false ;
		return ;
	}
	if (ref == m_bt_joinMatch)
	{
		showOneMins = false ;
		if (g_pGameFrame->isInTable() ) //在等候室中
		{
			bool flag = false ;
			if (g_pGameFrame->getTableType() == NormalTable)
			{
				if (g_globalGameSerialization.IsInGameNow() && g_globalTableConfig.GetTableMode() == TablePlaying)
				{
					flag = true ;
				}
			}
			if (g_pGameFrame->getTableType() == SngTable)
			{
				if (!g_globalSngTableInfo.m_isGameOver)
				{
					flag = true ;
				}
			}
			if (g_pGameFrame->getTableType() == MttTable)
			{
				if (!g_globaMttGameInfo.m_isMttlose)
				{
					flag = true ;
				}
			}
			if (flag)
			{
				PopTipWithBt2("牌局尚未结束，现在退出已下注筹码和报名费用将不会返还，您是否要进入其他牌局?","确定","取消",[this](unsigned ubt)
				{
					if(ubt == 0)
					{
						/*SendClientRequest(ClientRequest::kCallLevelTable) ;*/
						this->stopAllActions() ;
						this->setVisible(false) ;
						g_pGameFrame->doJoinMttMatch(m_tableID,m_isdelayRegist);
					}
					if(ubt == 1)
					{

					}
				});
			}
			else
			{
				this->stopAllActions() ;
				this->setVisible(false) ;
				g_pGameFrame->doJoinMttMatch(m_tableID,m_isdelayRegist);
			}
		
		}
		else
		{
			this->stopAllActions() ;
			this->setVisible(false) ;
			g_pGameFrame->doJoinMttMatch(m_tableID,m_isdelayRegist);
		}
		//if (g_globaMttGameInfo.m_isGameStart||g_globalSngTableInfo.m_isGameStart)
		//{
		//	PopTipWithBt2("比赛中途退出，您的报名费将不会返还，并且无法回来，是否参加新的比赛?","新的比赛","取消",[this](unsigned ubt)
		//	{
		//		if(ubt == 0)
		//		{
		//			/*SendClientRequest(ClientRequest::kCallLevelTable) ;*/
		//			this->stopAllActions() ;
		//			this->setVisible(false) ;
		//			g_pGameFrame->doJoinMttMatch(m_tableID,m_isdelayRegist);
		//		}
		//		if(ubt == 1)
		//		{

		//		}
		//	});
		//}

		return ;
	}
	if (ref == m_bt_close)
	{
		showOneMins = false ;
		this->stopAllActions() ;
		this->setVisible(false) ;
		return ;
	}
}


void MttStartNotice::addTips(unsigned tableid,std::string matchName, unsigned time ,bool delayRegist)
{
	showOneMins = true ;
	m_tableID = tableid ;
	m_matchName = matchName ;
	m_time = time ;
	m_isdelayRegist = delayRegist ;

	m_bt_quiteMatch->setVisible(true) ;
	m_bt_joinMatch->setVisible(true) ;
	this->stopAllActions() ;

	this->setPositionY(GameData::getInstance()->getGameHeight()+50) ;



	updateTime() ;
}

void MttStartNotice::updateTime()
{
	m_lineLabelTips->resetLable() ;
	m_lineLabelTips->setAnchorPoint(Point(1,0.5));
	m_lineLabelTips->setPositionX(792) ;
	auto updateTime = [this](){

		if (m_time<0)
		{
			this->stopAllActions() ;
			this->setVisible(false) ;
			showOneMins = false ;
			return ;
		}
		m_lineLabelTips->resetLable() ;
		char buff[64] ;
		m_lineLabelTips->Appand(24,"fzlt.ttf","您刚刚报名的",Color4B::WHITE);
		m_lineLabelTips->Appand(24,"fzlt.ttf",m_matchName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
		m_lineLabelTips->Appand(24,"fzlt.ttf","还剩",Color4B::WHITE);
		sprintf(buff,"%d秒",m_time) ;
		m_lineLabelTips->Appand(24,"fzlt.ttf",buff,Color4B(0xff,0xd2,0x00,0xff));
		m_lineLabelTips->Appand(24,"fzlt.ttf","开赛,赶紧进入比赛吧。",Color4B::WHITE);
		m_time-=1 ;

	} ;
	CCFiniteTimeAction *repeat = CCRepeat::create(Sequence::create(CallFunc::create(updateTime),DelayTime::create(1.0f),NULL), 9999);
	auto moveDown = MoveTo::create(0.3,Point(this->getPositionX(),GameData::getInstance()->getGameHeight())) ;
	this->runAction(Spawn::create(repeat,moveDown,NULL));
}
void MttStartNotice::addMissTime(float time)
{
	if (!this->isVisible())
	{
		return ;
	}
	if (!showOneMins)
	{
		return ;
	}
	this->stopAllActions() ;
	m_time-=time ;
	updateTime() ;

}

void MttStartNotice::callMatchTips(std::string matchName)
{
	if (showOneMins)
	{
		return ;
	}
	m_matchName = matchName ;

	m_bt_quiteMatch->setVisible(false) ;
	m_bt_joinMatch->setVisible(false) ;

	m_lineLabelTips->setAnchorPoint(Point(0.5,0.5));
	m_lineLabelTips->setPositionX(this->getContentSize().width/2) ;

	this->stopAllActions() ;
	this->setPositionY(GameData::getInstance()->getGameHeight()+50) ;
	m_lineLabelTips->resetLable() ;

	m_lineLabelTips->Appand(24,"fzlt.ttf","您刚刚报名的",Color4B::WHITE);
	m_lineLabelTips->Appand(24,"fzlt.ttf",m_matchName.c_str(),Color4B(0xff,0xd2,0x00,0xff));
	m_lineLabelTips->Appand(24,"fzlt.ttf","马上开始了,快准备开始吧~",Color4B::WHITE);

	auto moveDown = MoveTo::create(0.3,Point(this->getPositionX(),GameData::getInstance()->getGameHeight())) ;

	this->runAction(Sequence::create(moveDown,DelayTime::create(3),CallFunc::create([this](){this->setVisible(false) ;}),NULL));
}