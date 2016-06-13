#include "AchievementUi.h"
#include "FrameSprite.h"
#include "GameCommon.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "GrabTouch.h"

enum //设置默认的滚动视图的大小
{
	ListScrollW = 820,
	ListScrollH = 350
};

USING_NS_CC;

//---------------------------------------------------------------------------------------------------------------
void AchievementUi::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		UpdateAchievementList(m_uRecordMenuCurrentStatusId);		// 根据当前按钮的记录状态值
		m_pLayer[m_uRecordMenuCurrentStatusId]->setVisible(true);

	}
	else
	{
        m_pLayer[m_uRecordMenuCurrentStatusId]->setVisible(false);

	}
}

//---------------------------------------------------------------------------------------------------------------
bool AchievementUi::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}
	auto sprite = Sprite::create("achievementui/bg2.png");
	sprite->setAnchorPoint(Vec2(0, 0));
	sprite->setPosition(61, 33);
	this->addChild(sprite);

	m_uRecordMenuCurrentStatusId = 0;
	m_nRecordAchievementCurrentValue = 0;
	m_nRecordAchievementMaxValue = 0;
	m_nRecordAchievementIconId = 0;

	auto title = CreateNode1AndPosAddTo<Sprite>("achievementui/title.png", 470, 568, this, 0);

	// 成就相应按钮的Layer
	for(int i = 0;i < 5;i++)
	{
		m_pLayer[i]=  Layer::create();
		m_pLayer[i]->setContentSize(Size(ListScrollW, ListScrollH));
		m_pLayer[i]->setPosition(50, 50);
		addChild(m_pLayer[i], 5 - i);
        m_pLayer[i]->setVisible(i==0?true:false);
	}
	m_pLabelCount = CreateLabelMSYHBDAndAnchorPosClrAddTo(24 , "",Vec2(0, 0), 405, 495,cocos2d::Color4B(0x65,0xae,0x7c,0xff), this, 0);
	//m_pLayer[0]->setVisible(true);
	//m_pLayer[1]->setVisible(false);
	//m_pLayer[2]->setVisible(false);
	//m_pLayer[3]->setVisible(false);

	// 成就界面下的所有按钮
    std::string resPath[] = {"left.png","middle.png","middle.png","middle.png","right.png"};
	for(int i = 0;i < 5; i++)
	{
		m_pAchievementMenu[i] = MenuItemSpriteWithLabel::create("achievementui/chengjiu_"+resPath[i], 3, CC_CALLBACK_1(AchievementUi::AllMenuCallback, this, i));
		m_pAchievementMenu[i]->setAnchorPoint(Vec2(0,0));
		m_pAchievementMenu[i]->setPosition(55 + i*165, 428);
		auto AchievementRecord = Menu::create(m_pAchievementMenu[i],nullptr);
		AchievementRecord->setPosition(Vec2::ZERO);
		addChild(AchievementRecord);
	}
	// 成就任务精灵总成就的数字
	m_pRichlabelCount = CreateLabelMSYHBDAndAnchorPosClrAddTo(24 , "",Vec2(0, 0), 405, 495,cocos2d::Color4B(0x65,0xae,0x7c,0xff), this, 0);

	m_pAchievementMenu[0]->setEnabled(false);
    const char* menuName[] = {"富可敌国","勤劳致富","赌神之路","竞技之神","德州之王"};
    for (int i=0; i<5; i++)
    {
        auto nAchievementMenu = CreateLabelMSYHBD(24, menuName[i]);
        m_pAchievementMenu[i]->SetLabel(nAchievementMenu);
        m_pAchievementMenu[i]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
        m_pAchievementMenu[i]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
    }
	/*auto nAchievementMenu1 = CreateLabelMSYHBD(26,"富可敌国");
	m_pAchievementMenu[0]->SetLabel(nAchievementMenu1);
	m_pAchievementMenu[0]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pAchievementMenu[0]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	auto nAchievementMenu2 = CreateLabelMSYHBD(26,"勤劳致富");
	m_pAchievementMenu[1]->SetLabel(nAchievementMenu2);
	m_pAchievementMenu[1]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pAchievementMenu[1]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	auto nAchievementMenu3 = CreateLabelMSYHBD(26,"赌神之路");
	m_pAchievementMenu[2]->SetLabel(nAchievementMenu3);
	m_pAchievementMenu[2]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pAchievementMenu[2]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

	auto nAchievementMenu4 = CreateLabelMSYHBD(26,"竞技之神");
	m_pAchievementMenu[3]->SetLabel(nAchievementMenu4);
	m_pAchievementMenu[3]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
	m_pAchievementMenu[3]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));*/

	// 显示玩家的成就内容数量的背景
    m_pRichSpriteBgItem = CreateNode1AndPosAddTo<Sprite>("chattip/bgchattipt.png", 0, 0, this, 99);
	m_pRichSpriteBgItem->setVisible(false);
	// 显示玩家的成就达到的数量 
	 m_pRichCashCenterSizelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(24 , "",Vec2(0.5, 0.5), m_pRichSpriteBgItem->getContentSize().width/2, m_pRichSpriteBgItem->getContentSize().height/2,
		Color4B::WHITE, m_pRichSpriteBgItem, 10);
	 //m_pRichCashCenterSizelabel->setVisible(false) ;
	return true;
}


void AchievementUi::UpdateAchievementList()
{
	// 默认调用所有的按钮
    m_nRecordAchievementCurrentValue = 0;
    m_nRecordAchievementMaxValue = 0;
	UpdateAchievementList(ERICHERTHANAKING);
	UpdateAchievementList(EHARDTOGETRICH);
	UpdateAchievementList(ETHEROADOFGAMBLERS);
	UpdateAchievementList(EGODOFTHECOMPETIVIVE);
    UpdateAchievementList(TEXASPOKERKING);

}

//---------------------------------------------------------------------------------------------------------------
// 更新富可敌国成就函数
void AchievementUi::UpdateAchievementList(unsigned m_uRecordMenuCurrentStatusId)				// 根据当前按钮的记录状态值
{
	for (auto item : m_vecAchievementListItemInValidNew[m_uRecordMenuCurrentStatusId])			// 对成就任务容器进行遍历进行
	{
		item->m_pSpriteBGItem->setVisible(false);
		m_vecAchievementListItemOld[m_uRecordMenuCurrentStatusId].push_back(item);				// PUSH到Old的Vector容器里面
	}
	//for (auto& mission : m_vecAchievementListItemInValidNew[m_uRecordMenuCurrentStatusId])		// 遍历结构体的数据
	//{
	//	mission->m_pSpriteRewordImage->setVisible(false);
	//	mission->m_plavelImageCount->setVisible(false);
	//}
	m_vecAchievementListItemInValidNew[m_uRecordMenuCurrentStatusId].clear();					// 对New的Vector容器进行清空
	// 根据点击按钮状态的ID得到容器的数量
	unsigned uAchievementNum = g_globalAchievementInfo.m_vecAchievementInfo[m_uRecordMenuCurrentStatusId].size();
	unsigned uItemH = 200;																		// 滚动视图的单个高度
	unsigned uItemW = 150;																		// 滚动视图的单个宽度
	unsigned uAllItemH = ListScrollH;															// 所有滚动视图的总高度
	float fX = 60, fY = uAllItemH / 2 + 40;
	for (auto& mission : g_globalAchievementInfo.m_vecAchievementInfo[m_uRecordMenuCurrentStatusId])	// 按钮的所有事件
	{
		auto item = GetUserAchievementItemList((MyAchievementType)m_uRecordMenuCurrentStatusId, mission);
		UpdateUserAchievementRichItemList(item, mission);
		item->m_pSpriteBGItem->setPosition(fX, fY);
		item->m_pSpriteBGItem->setVisible(true);
		m_vecAchievementListItemInValidNew[m_uRecordMenuCurrentStatusId].push_back(item);
		++m_nRecordAchievementIconId;

		if(m_nRecordAchievementIconId < 5)
		{
			fX = uItemW + fX;	// 多少个成就任务的X坐标
		}
		else if(m_nRecordAchievementIconId == 5)
		{
			fX = 60;
			fY = 50; 
		}
		else if( m_nRecordAchievementIconId > 5 && m_nRecordAchievementIconId <= 9)
		{
			fX = uItemW + fX;
		}
        if(mission.m_bActivated == 1)	// 记录当前成就的数量
        {
            ++m_nRecordAchievementCurrentValue;
        }
        ++m_nRecordAchievementMaxValue;
	}
	m_nRecordAchievementIconId = 0;
	m_pLayer[m_uRecordMenuCurrentStatusId]->setContentSize(Size(ListScrollW, ListScrollH));


	// 显示总成就的数字
	char szTxt[100] = {0};
	sprintf(szTxt, "总成就:%d/%d", m_nRecordAchievementCurrentValue,m_nRecordAchievementMaxValue);
	SetLabelString(m_pLabelCount, szTxt);
}

//---------------------------------------------------------------------------------------------------------------
//成就列表下按钮的所有回调函数
void AchievementUi::AllMenuCallback(Ref* pSender,  int num)
{
	m_pRichSpriteBgItem->setVisible(false) ;
	for(int i = 0;i < 5;i++)
	{
		if(i == num)
		{
			m_pAchievementMenu[i]->setEnabled(false);
			m_pLayer[i]->setVisible(true);
			m_pLayer[i]->setTouchEnabled(true);
			reorderChild(m_pLayer[i],10);
			m_uRecordMenuCurrentStatusId = i;	// 记录当前按钮的按下的ID
			//UpdateAchievementList(m_uRecordMenuCurrentStatusId);
		}
		else
		{
			m_pAchievementMenu[i]->setEnabled(true);
			m_pLayer[i]->setVisible(false);
			m_pLayer[i]->setTouchEnabled(false);
			//m_uRecordMenuCurrentStatusId = 0;	// 记录当前按钮的按下的ID默认为0
		}
	}
	return ;
}

//---------------------------------------------------------------------------------------------------------------
// 根据成就类型获取相应的坐标
AchievementUi::UserAchievement* AchievementUi::GetUserAchievementItemList(MyAchievementType etype, const AchievementInfo& achieveuserinfo)
{
	UserAchievement *item = nullptr;
	if (m_vecAchievementListItemOld[etype].empty())		// 判断OLD的容器数据是否为空
	{
		auto labelAnchor = Vec2(0, 0);
		int labelSize = 26;
		int labelFenLeiSize = 24;
		auto labelClr = Color4B::WHITE;
		auto labelFenLeiClr = Color4B(0x50, 0xbd, 0x2e, 0xff);
		auto szCur = m_pLayer[etype]->getContentSize();
		// 成就任务的高亮、正常、禁止的三种状态显示
		HttpSprite* m_pbtRichSpriteBgImage = HttpSprite::create("achievementui/achievement1.png", Size(120, 120));
		m_pbtRichSpriteBgImage->setPosition(45, 250);

		LayerButton* btDetail = LayerButton::create(false, true);
		m_pbtRichSpriteBgImage->addChild(btDetail);

		m_pLayer[etype]->addChild(m_pbtRichSpriteBgImage);

		// 成就任务精灵总成就的数字
		auto m_pRichlabelCount = CreateLabelMSYHBDAndAnchorPosClrAddTo(24 , "",Vec2(0, 0), 405, 495,cocos2d::Color4B(0x65,0xae,0x7c,0xff), this, 0);

		// 成就任务显示的名称
		auto m_pRichDescName = CreateLabelMSYHAndAnchorPosClrAddTo(24 , "",Vec2::ANCHOR_MIDDLE, 61, -28, Color4B(0x35,0x92,0x57,0xff), m_pbtRichSpriteBgImage, 0);
		// 显示玩家的成就内容数量的背景
		auto m_pRichSpriteBgItem = CreateNode1AndPosAddTo<Sprite>("chattip/bgchattipt.png", 63, -40, m_pbtRichSpriteBgImage, 0);
		m_pRichSpriteBgItem->setVisible(false);
		// 显示玩家的成就达到的数量 
		auto m_pRichCashCenterSizelabel =  CreateLabelMSYHAndAnchorPosClrAddTo(24 , "",Vec2(0.5, 0.5), 65, -40,
			Color4B::WHITE, m_pbtRichSpriteBgImage, 0);

		auto pItem = new UserAchievement;
		pItem->m_pSpriteBGItem			 = m_pbtRichSpriteBgImage;			// 显示玩家的成就背景精灵图片
		pItem->m_pLabelName				= m_pRichDescName;					// 成就任务显示的名称			
		pItem->m_plavelImageCount		 = m_pRichCashCenterSizelabel;		// 显示玩家的成就达到的数量 
		pItem->m_pSpriteRewordImage		= m_pRichSpriteBgItem;				// 显示玩家的成就内容数量的背景
		pItem->m_pBtDetail = btDetail ;
		pItem->m_type = etype ;

		pItem->m_pBtDetail->setContentSize(m_pbtRichSpriteBgImage->getContentSize());

		item = pItem;
		item->m_pBtDetail->setContentSize(Size(120,120)) ;
	}
	else
	{
		item = m_vecAchievementListItemOld[etype].back();
		m_vecAchievementListItemOld[etype].pop_back();
	}
	// 对每个显示内容的背景框做监听

	return item;
}

//---------------------------------------------------------------------------------------------------------------
// 更新富可敌国成就的列表
void AchievementUi::UpdateUserAchievementRichItemList(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo)
{
	// 得到当前的成就的称呼
	SetLabelString(item->m_pLabelName, achieveuserinfo.m_strTip);
	// 显示所有Icon下载的图片
	SetHttpSpriteURL(item->m_pSpriteBGItem, achieveuserinfo.m_strUrl);
	if(achieveuserinfo.m_bActivated == 1)
	{
		item->m_pLabelName->setTextColor(Color4B::YELLOW);
	}
	else
	{
		item->m_pLabelName->setTextColor(Color4B(0x35,0x92,0x57,0xff));
	}
	item->m_pBtDetail->setContentSize(Size(120,120)) ;
	item->m_pBtDetail->SetHitCB(std::bind(&AchievementUi::menuClickBgItem, this,item,achieveuserinfo));
}
void AchievementUi::menuClickBgItem(AchievementUi::UserAchievement *item, const AchievementInfo& achieveuserinfo)
{
	char szTxt[100] = {0};

	if(item->m_type == ERICHERTHANAKING)
	{
			sprintf(szTxt, "德州币总额达到%s", FormatCash(achieveuserinfo.m_dNum).c_str());
	}
	else if(item->m_type == EHARDTOGETRICH)
	{
			sprintf(szTxt, "常规游戏场对局%s次", FormatCash(achieveuserinfo.m_dNum).c_str());
	}
	else if(item->m_type == ETHEROADOFGAMBLERS)
	{
			sprintf(szTxt, "常规游戏场对局胜利%s次", FormatCash(achieveuserinfo.m_dNum).c_str());
	}
	else if(item->m_type == EGODOFTHECOMPETIVIVE)
	{
			sprintf(szTxt, "SNG比赛获得%s次名次", FormatCash(achieveuserinfo.m_dNum).c_str());
	}
    else if (item->m_type == TEXASPOKERKING)
    {
        sprintf(szTxt, "MTT场对局%s次", FormatCash(achieveuserinfo.m_dNum).c_str());
    }

	SetLabelString(m_pRichCashCenterSizelabel, szTxt);

	m_pRichSpriteBgItem->setPosition(Point(item->m_pSpriteBGItem->getPosition().x+110,item->m_pSpriteBGItem->getPosition().y+30)) ;
	// 得到当前的玩家的称呼
	m_pRichSpriteBgItem->setVisible(true);
	m_pRichCashCenterSizelabel->setVisible(true);


}

void AchievementUi::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	m_pRichSpriteBgItem->setVisible(false) ;
}