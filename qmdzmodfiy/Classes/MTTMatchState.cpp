#include "MTTMatchState.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"

#include "LineLabel.h"
#include "FrameSprite.h"
#include "FrameSpriteButton.h"
#include "GameMttHall.h"
#include "Tools.h"
#include "Util.h"
#include "GameFrame.h"
#include "LogonLayerEx.h"
USING_NS_CC;

enum
{
	GaishuBtn,
	PaimingBtn,
	MangZhuBtn,
	JiangLiBtn
};

enum
{
	//概述滚动视图
	GaiShuScrollW = 686,
	GaiShuScrollH = 380,
	PaIMingScrollW = 696,
	PaiMingScrollH = 422,
	MangZhuScrollW = 686,
	MangZhuScrollH = 363,
	JiangLiScrollW = 686,
	JiangLiScrollH = 363,
    FuDongScrollW = 150,//浮动赛专有
    FuDongScrollH = 325
};

#define OnePageItemNum 15

bool MTTMatchState::init()
{
	if (!GameDialog::init("dialog/bg.png", true))
	{
		return false;
	}
	//标题
	auto dialogSz = this->getContentSize();
	auto titleSprite = CreateNode1AndPosAddTo<Sprite>("gamemtt/matchstatetitle.png", dialogSz.width/2, dialogSz.height-70, this, 0);
	//显示内容九宫格背景
	m_pScale9ContentBg = ui::Scale9Sprite::create("gamemtt/matchstatebg.png", Rect(0,0,140,140), Rect(40,40,60,60));
	m_pScale9ContentBg->setPreferredSize(Size(686, 390));
	m_pScale9ContentBg->setPosition(560, 305);
	this->addChild(m_pScale9ContentBg);

	//操作按钮创建
	const char* btnName[4] = {"概述", "排名", "盲注", "奖励"};

	//std::string btnName[4] = {"概述", "排名", "排名", "奖励"};
	for (int i=0; i<4; i++)
	{
		m_pMenuItemType[i] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(MTTMatchState::OnMenuItemSelected, this, 

			i));
		m_pMenuItemType[i]->setAnchorPoint(Vec2(0, 0));
		m_pMenuItemType[i]->setPosition(24, 434-68*i);
		m_pMenuItemType[i]->SetLabel(CreateLabelMSYHBD(24, btnName[i]));
		m_pMenuItemType[i]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
		m_pMenuItemType[i]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));
	}
	//报名 退赛 进入按钮
	m_menuSignUp = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(MTTMatchState::OnMenuSignUp, this));
	m_menuSignUp->setPosition(560, 68);
	m_menuSignUp->SetLabel(CreateLabelMSYHBD(28, "报名"));
	m_menuCalcelSign = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(MTTMatchState::OnMenuSignUp, this));
	m_menuCalcelSign->setPosition(720, 68);
	m_menuCalcelSign->SetLabel(CreateLabelMSYHBD(28, "退赛"));
	m_menuEnterGame = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(MTTMatchState::OnMenuSignUp, this));
	m_menuEnterGame->setPosition(400, 68);
	m_menuEnterGame->SetLabel(CreateLabelMSYHBD(28, "进入"));
	m_menuSignUp->setVisible(false);
	m_menuCalcelSign->setVisible(false);
	m_menuEnterGame->setVisible(false);
	auto menu = Menu::create(m_pMenuItemType[0], m_pMenuItemType[1], m_pMenuItemType[2], m_pMenuItemType[3], m_menuSignUp, m_menuCalcelSign, 

		m_menuEnterGame, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	//滚动视图创建
	//unsigned scrollPos[4][2] = {GaiShuScrollW, 

	//	GaiShuScrollH,PaIMingScrollW,PaiMingScrollH,MangZhuScrollW,MangZhuScrollH,JiangLiScrollW,JiangLiScrollH
	//
	//};
	m_pScrollViewType[0] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(GaiShuScrollW, GaiShuScrollH), 217, 115, this, 0);
	m_pScrollViewType[0]->setContentSize(Size(GaiShuScrollW, GaiShuScrollH));
	m_pScrollViewType[1] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(PaIMingScrollW, PaiMingScrollH), 207, 52, this, 0);
	m_pScrollViewType[1]->setContentSize(Size(PaIMingScrollW, PaiMingScrollH));
	m_pScrollViewType[2] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(MangZhuScrollW,MangZhuScrollH), 217, 52, this, 0);
	m_pScrollViewType[2]->setContentSize(Size(MangZhuScrollW,MangZhuScrollH));
	m_pScrollViewType[4] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(FuDongScrollW, FuDongScrollH), 222, 50+73, this, 0);
	m_pScrollViewType[4]->setContentSize(Size(FuDongScrollW, FuDongScrollH));
	m_pScrollViewType[3] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(JiangLiScrollW,JiangLiScrollH), 450, 42, this, 0);
	m_pScrollViewType[3]->setContentSize(Size(JiangLiScrollW,JiangLiScrollH));

    for (int i=0;i<5;i++)
    {
        m_pScrollViewType[i]->setDirection(extension::ScrollView::Direction::VERTICAL);
        m_pScrollViewType[i]->setVisible(false);
    }
	m_pScrollViewType[1]->setDelegate(this);
	//盲注列表表头
	auto scale9SpriteSz = m_pScale9ContentBg->getContentSize();
    //m_pSpriteListTitle = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_mangzhu_title.png", scale9SpriteSz.width/2, scale9SpriteSz.height-33, m_pScale9ContentBg, 1);
    m_pSpriteListTitle = ui::Scale9Sprite::create("gamemtt/mtt_mangzhu_title.png", Rect(0,0,663,40), Rect(50,10,563,20));
    m_pSpriteListTitle->setPreferredSize(Size(663, 40));
    m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2, scale9SpriteSz.height-33);
    m_pScale9ContentBg->addChild(m_pSpriteListTitle, 1);
	const char* titleName[4] = {"级别","盲注","前注","升盲时间"};
	float titlePosY[4] = {60, 223, 398, 590};
	for (int i=0; i<4; i++)
	{
		m_labelMangZhuTitle[i] = CreateLabelMSYHAndAnchorPosClrAddTo(24, titleName[i], Vec2(0.5, 0.5), titlePosY[i], 20, Color4B(0xff,0xd2,0x00,0xff), 

			m_pSpriteListTitle, 0);;
		m_labelMangZhuTitle[i]->setVisible(false);
	}
	m_pSpriteListTitle->setVisible(false);
	//奖励列表表头
	const char* rewardName[3] = {"名次","奖励","大师分"};
	float rewardPosY[3] = {75, 190, 590};
	for (int i=0; i<3; i++)
	{
		m_labelJiangLiTitle[i] = CreateLabelMSYHAndAnchorPosClrAddTo(24, rewardName[i], Vec2(0.5, 0.5), rewardPosY[i], 20, Color4B(0xff,0xd2,0x00,0xff), 

			m_pSpriteListTitle, 0);;
		m_labelJiangLiTitle[i]->setVisible(false);
	}
	m_labelPaiMingDes = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), 560, 316, Color4B(0xff,0xff,0xff,0xff), this, 0);
	m_labelMTTTableName = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(1,0.5), dialogSz.width-50, 490, Color4B(0x79,0xDC,0x98,0xff), this, 0);
	m_labelCurPaiming = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(1,0.5), dialogSz.width-50, 490, Color4B(0x79,0xDC,0x98,0xff), this, 0);
	m_labelMTTTableName->setVisible(false);
	m_labelCurPaiming->setVisible(false);
	m_labelPaiMingDes->setVisible(false);
    m_pSpriteFuDongGold = CreateNode1AndPosAddTo<Sprite>("gamemtt/mttcoin.png",27,33, m_pScale9ContentBg, 0);
    m_labelFuDong[0] = CreateLabelMSYHAndAnchorPosClrAddTo(20,"当前奖池总额", Vec2(0.5,0.5), 72, 65, Color4B(0x50,0xbd,0x2e,0xff), m_pScale9ContentBg, 0);
    m_labelFuDong[1] = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0,0.5), 50, 33, Color4B(0xff,0xd2,0x00,0xff), m_pScale9ContentBg, 0);
    m_pSpriteFuDongGold->setVisible(false);
    m_labelFuDong[0]->setVisible(false);
    m_labelFuDong[1]->setVisible(false);
	m_pGaishuItem = nullptr;
	s_dataLoading = nullptr;
	m_pSpriteBlindBg = nullptr;
	m_bSignUp = false;
	m_bRequestData = false;
    m_bCurDataIsNULL = false;
    m_iGameTableType = -3;
	m_uSizePaiMing = 0;
	m_uCurMyRankId = 0;
	m_iLastTime = 0;
	m_uCurSelectIndex = 0;
	m_uCurPageIndex = 1;
	m_uTotalOldItem = -1;
	m_dBlindOffset = 0;
	m_uTableIdOld = 0;

	return true;
}

//概述 排名 盲注 奖励 函数回调
void MTTMatchState::OnMenuItemSelected(cocos2d::Ref*, unsigned uType)
{
	for (int i=0; i<4; i++)
	{
		m_pMenuItemType[i]->setEnabled(i==uType?false:true);
		m_labelMangZhuTitle[i]->setVisible(false);
		m_pScrollViewType[i]->setVisible(i==uType?true:false);
	}

	m_pScrollViewType[3]->setVisible(uType==3);
	m_pScrollViewType[4]->setVisible(uType==3);

    //m_pScrollViewType[4]->setVisible(false);
    m_pSpriteFuDongGold->setVisible(false);
    m_labelFuDong[0]->setVisible(false);
    m_labelFuDong[1]->setVisible(false);

	char buff[32] = {0};
	m_labelMTTTableName->setVisible(false);
	m_labelCurPaiming->setVisible(false);
	m_labelJiangLiTitle[0]->setVisible(false);
	m_labelJiangLiTitle[1]->setVisible(false);
	m_labelJiangLiTitle[2]->setVisible(false);
	m_menuSignUp->setVisible(false);
	m_menuCalcelSign->setVisible(false);
	m_menuEnterGame->setVisible(false);
	m_labelPaiMingDes->setVisible(false);
	m_uCurSelectIndex = uType;
    if (m_bCurDataIsNULL)
    {
        showDataLoading(true);
        return;
    }else
    {
        showDataLoading(uType==PaimingBtn && g_globaMttGameInfo.m_isGameStart && m_bRequestData);
    }
	switch (uType)
	{
	case GaishuBtn:
		{
			m_pSpriteListTitle->setVisible(false);
			m_pScale9ContentBg->setVisible(true);
			m_pScale9ContentBg->setPreferredSize(Size(686, 390));
			m_pScale9ContentBg->setPosition(560, 305);

			//比赛中 等候室  所有按钮隐藏
			if (g_pGameFrame->isInTable() 
				||g_globaMttGameInfo.m_curMttTable.m_SiginState ==3
				||g_globaMttGameInfo.m_curMttTable.m_SiginState ==0
				)
			{
				m_menuSignUp->setVisible(false);
				m_menuCalcelSign->setVisible(false);
				m_menuEnterGame->setVisible(false);
			}
			else
					//1 mtt大厅 
			{
				
				/*		m_menuEnterGame->setPosition(400, 78);
				m_menuEnterGame->setPosition(560, 78);
				m_menuCalcelSign->setPosition(720, 78);*/
				//0未开放  1报名   2进行中  3已结束   4已报名 5 可进入 6 已解散 7延迟报名
				//未报名 
				if (g_globaMttGameInfo.m_curMttTable.m_SiginState == 1)
				{
					m_menuSignUp->setVisible(true);
					m_menuSignUp->setPosition(560, 68);
				}
				//已报名   可退赛 不可进入
				else if (g_globaMttGameInfo.m_curMttTable.m_SiginState == 4)
				{
					m_menuCalcelSign->setVisible(true);
					m_menuCalcelSign->setPosition(560, 68);
				}

				//即将开始 可进入  退赛  可进入
				else if (g_globaMttGameInfo.m_curMttTable.m_SiginState == 5 &&!g_globaMttGameInfo.m_curMttTable.m_isdelayRegist)
				{
					m_menuCalcelSign->setVisible(true);
					m_menuEnterGame->setVisible(true);

					m_menuEnterGame->setPosition(400, 68);
					m_menuCalcelSign->setPosition(720, 68);
					
					
				}
				//延时报名 可进入
				else if(g_globaMttGameInfo.m_curMttTable.m_isdelayRegist&&g_globaMttGameInfo.m_curMttTable.m_SiginState == 5)
				{
					m_menuEnterGame->setVisible(true);
					m_menuEnterGame->setPosition(560, 68);
				}

			}
		}break;
	case PaimingBtn:
		{
			m_pSpriteListTitle->setVisible(false);
			m_pScale9ContentBg->setVisible(false);
			m_labelMTTTableName->setVisible(false);
			if (!g_globaMttGameInfo.m_isGameStart)
			{
				m_labelPaiMingDes->setVisible(true);
				m_pScrollViewType[1]->setVisible(false);
				if(g_globaMttGameInfo.m_curMttTable.m_SiginState==3)
				{
					SetLabelString(m_labelPaiMingDes, "比赛已结束");
				}
				else
				{
					SetLabelString(m_labelPaiMingDes, "比赛尚未开始，暂时没有排名。开赛后可以来看您的排名~");
				}
				

			}
			else
			{
				m_labelCurPaiming->setVisible(true);
				m_pScrollViewType[1]->setVisible(true);
				sprintf(buff, "当前排名第%d名", m_uCurMyRankId);
				SetLabelString(m_labelCurPaiming, buff);
			}
		}break;
	case MangZhuBtn:
		{
			for (int i=0; i<4; i++)
			{
				m_labelMangZhuTitle[i]->setVisible(true);
			}
			m_pSpriteListTitle->setVisible(true);
			m_pScale9ContentBg->setVisible(true);
			m_labelMTTTableName->setVisible(true);
			m_pScale9ContentBg->setPreferredSize(Size(686, 428));
			m_pScale9ContentBg->setPosition(560, 254);
			auto scale9SpriteSz = m_pScale9ContentBg->getContentSize();
			//m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2, scale9SpriteSz.height-30);
            m_pSpriteListTitle->setPreferredSize(Size(663, 40));
            m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2, scale9SpriteSz.height-33);
			m_pScrollViewType[2]->setContentOffset(Vec2(0, m_dBlindOffset), false);
		}break;
	case JiangLiBtn:
		{
			m_labelJiangLiTitle[0]->setVisible(true);
			m_labelJiangLiTitle[1]->setVisible(true);
			m_labelJiangLiTitle[2]->setVisible(true);
			m_pScale9ContentBg->setVisible(true);
			m_labelMTTTableName->setVisible(true);
			m_pSpriteListTitle->setVisible(true);
			m_pScale9ContentBg->setPreferredSize(Size(686, 428));
			m_pScale9ContentBg->setPosition(560, 254);
			auto scale9SpriteSz = m_pScale9ContentBg->getContentSize();
            //m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2, scale9SpriteSz.height-30);
            if (g_globaMttGameInfo.m_curMttTable.m_isFloatMatch)
            {
                m_pSpriteListTitle->setPreferredSize(Size(663-150, 40));
                m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2+70, scale9SpriteSz.height-33);

                m_pScrollViewType[4]->setVisible(true);
                m_pSpriteFuDongGold->setVisible(true);
                m_labelFuDong[0]->setVisible(true);
                m_labelFuDong[1]->setVisible(true);
            }
            else
            {
                m_pSpriteListTitle->setPreferredSize(Size(663, 40));
                m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2, scale9SpriteSz.height-33);
                m_pScrollViewType[4]->setVisible(false);
                m_labelFuDong[0]->setVisible(false);
                m_labelFuDong[1]->setVisible(false);
                m_pSpriteFuDongGold->setVisible(false);
            }
		}
	default:
		break;
	}
}

//报名&退赛 回调函数
void MTTMatchState::OnMenuSignUp(cocos2d::Ref* pSend)
{
	if (pSend == m_menuSignUp)//报名按钮回调
	{
		Json::Value param;
		param["id"] = g_globaMttGameInfo.m_curMttTable.m_tableID;
		SendClientRequest(ClientRequest::kMttMatchSiginUp, param);
	}
	else if (pSend == m_menuCalcelSign)//退赛按钮回调
	{
		Json::Value param;
		param["id"] = g_globaMttGameInfo.m_curMttTable.m_tableID;
		SendClientRequest(ClientRequest::kMttMatchQuit, param);

	}
	else if (pSend == m_menuEnterGame)//进入比赛回调
	{
        g_pGameFrame->doJoinMttMatch(g_globaMttGameInfo.m_curMttTable.m_tableID, 0);
	}
}

void MTTMatchState::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
	if (view == m_pScrollViewType[1])
	{
		auto offSet = m_pScrollViewType[1]->getContentOffset();
		if(offSet.y == 0)
		{
			unsigned allPage = 0;
			allPage = m_uSizePaiMing%OnePageItemNum==0?(m_uSizePaiMing/OnePageItemNum):(m_uSizePaiMing/OnePageItemNum+1);
			m_uCurPageIndex++;
			if (m_uCurPageIndex > allPage)
				return;
			UpdatePaiMingInfo();
		}
	}
}

//更新 概述 排名 盲注 奖励
void MTTMatchState::UpdateMTTMatchStateInfo(unsigned index)
{
    m_uCurPageIndex = 1;
    if (g_globaMttGameInfo.m_curMttTable.m_MttReward.size() == 0 && g_globaMttGameInfo.m_curMttTable.m_MttBilnd.size() == 0)//当前桌的信息为空
    {
        m_bCurDataIsNULL = true;
    }
    else
    {
        m_bCurDataIsNULL = false;
    }


    m_uSizePaiMing = g_globaMttGameInfo.m_curMttTable.m_MttRank.size();
	UpdateGaiShuInfo(g_globaMttGameInfo.m_curMttTable);
	//UpdatePaiMingInfo();
	UpdateMangZhuInfo();
	UpdateJiangLiInfo(g_globaMttGameInfo.m_curMttTable.m_MttReward, g_globaMttGameInfo.m_curMttTable.m_tableName);
	OnMenuItemSelected(nullptr, index);

    auto scale9SpriteSz = m_pScale9ContentBg->getContentSize();
    if (g_globaMttGameInfo.m_curMttTable.m_isFloatMatch)
    {
        m_pScrollViewType[3]->setContentSize(Size(JiangLiScrollW-145, JiangLiScrollH));
        m_pScrollViewType[3]->setPosition(217+150, 50);
		//m_pScrollViewType[3]->setPosition(217+150, 50);
        //m_pScrollViewType[4]->setVisible(true);
        m_pSpriteListTitle->setPreferredSize(Size(663-150, 40));
        m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2+70, scale9SpriteSz.height-33);
        float rewardPosX[3] = {75-30, 223-30, 570-150};
        for (int i=0; i<3; i++)
        {
            m_labelJiangLiTitle[i]->setPosition(rewardPosX[i], 20);
        }
   /*     m_pSpriteFuDongGold->setVisible(true);
        m_labelFuDong[0]->setVisible(true);
        m_labelFuDong[1]->setVisible(true);*/
        SetLabelString(m_labelFuDong[1],FormatCash(g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_maxReward));
        UpdateFuDongInfo();
    } 
    else
    {
        m_pScrollViewType[3]->setContentSize(Size(JiangLiScrollW, JiangLiScrollH));
        m_pScrollViewType[3]->setPosition(217, 50);
        m_pScrollViewType[4]->setVisible(false);
        m_pSpriteListTitle->setPreferredSize(Size(663, 40));
        m_pSpriteListTitle->setPosition(scale9SpriteSz.width/2, scale9SpriteSz.height-33);
        float rewardPosX[3] = {75, 223, 570};
        for (int i=0; i<3; i++)
        {
            m_labelJiangLiTitle[i]->setPosition(rewardPosX[i], 20);
        }
        m_pSpriteFuDongGold->setVisible(false);
        m_labelFuDong[0]->setVisible(false);
        m_labelFuDong[1]->setVisible(false);
        UpdateJiangLiInfo(g_globaMttGameInfo.m_curMttTable.m_MttReward, g_globaMttGameInfo.m_curMttTable.m_tableName);
    }
}

//更新所有排名项
void MTTMatchState::UpdatePaiMingInfo()
{
	for (auto item : m_vecPaiMingNew)
	{
		item->m_spriteItemBg->setVisible(false);
		m_vecPaiMingOld.push_back(item);
	}
	m_vecPaiMingNew.clear();
    m_uSizePaiMing = g_globaMttGameInfo.m_curMttTable.m_MttRank.size();
	float oneItemH = 100 + 8;//背景宽加间隔,间隔在背景上面
	unsigned totalPage = (m_uSizePaiMing%OnePageItemNum)==0?(m_uSizePaiMing/OnePageItemNum):(m_uSizePaiMing/OnePageItemNum+1);
	unsigned totalItem = m_uCurPageIndex==totalPage?m_uSizePaiMing:OnePageItemNum*m_uCurPageIndex;
	float totalItemH = totalItem*oneItemH;
	if (PaiMingScrollH > totalItemH)
	{
		totalItemH = PaiMingScrollH;
		//m_pScrollViewType[1]->setBounceable(false);
	}
	else
	{
		m_pScrollViewType[1]->setBounceable(true);
	}
	if (m_uCurPageIndex == 1)
		m_pScrollViewType[1]->setContentOffset(Vec2(0, PaiMingScrollH-totalItemH), false);
	else
		m_pScrollViewType[1]->setContentOffset(Vec2(0,PaiMingScrollH -(totalItem-m_uTotalOldItem)*oneItemH-PaiMingScrollH), false);

	float posY = totalItemH - oneItemH/2-10;
	SetLabelString(m_labelMTTTableName, g_globaMttGameInfo.m_tableName);
	for (int i=0; i<OnePageItemNum*m_uCurPageIndex; i++)
	{
		if (i >= m_uSizePaiMing) break;
		auto item = GetPaiMingItem();
		UpdatePaiMingItem(item, g_globaMttGameInfo.m_curMttTable.m_MttRank[i]);
		m_vecPaiMingNew.push_back(item);
		item->m_spriteItemBg->setVisible(true);
		item->m_spriteItemBg->setPosition(PaIMingScrollW/2+5, posY);
		posY -= oneItemH;
	}

	m_uCurMyRankId = g_globaMttGameInfo.m_rank ;

	m_pScrollViewType[1]->setContentSize(Size(PaIMingScrollW, totalItemH));
	m_uTotalOldItem = totalItem;
	showDataLoading(false);
	m_bRequestData = false;
}

//更新所有盲注项
void MTTMatchState::UpdateMangZhuInfo()
{
	if (m_pSpriteBlindBg == nullptr)
	{
		m_pSpriteBlindBg = CreateNode1AndPosAddTo<Sprite>("gamemtt/cur_blind_bg.png", MangZhuScrollW/2,0, m_pScrollViewType[2],4);
	}
	//g_globaMttGameInfo.m_isGameStart = true ;
	//g_globaMttGameInfo.m_mangzhuIndex = 19;
	//g_globaMttGameInfo.m_mangzhuIndex = rand()%g_globaMttGameInfo.m_curMttTable.m_MttBilnd.size();
	float oneItemH = 46.0;
	float totalItemH = g_globaMttGameInfo.m_curMttTable.m_MttBilnd.size()*oneItemH;
	float fY = totalItemH - oneItemH+5;
	if (g_globaMttGameInfo.m_isGameStart)
	{
		m_dBlindOffset = fY - oneItemH*(g_globaMttGameInfo.m_mangzhuIndex-1);
	}
	else
	{
		m_dBlindOffset = fY;
	}
	m_pSpriteBlindBg->setPosition(MangZhuScrollW/2, m_dBlindOffset+oneItemH/2);
	if (g_globaMttGameInfo.m_curMttTable.m_tableID != m_uTableIdOld)
	{
		for (auto item : m_vecMangZhuNew)
		{
			item->m_spriteLine->setVisible(false);
			m_vecMangZhuOld.push_back(item);
		}
		m_vecMangZhuNew.clear();

		SetLabelString(m_labelMTTTableName, g_globaMttGameInfo.m_tableName);
		for (auto itemBlindInfo : g_globaMttGameInfo.m_curMttTable.m_MttBilnd)
		{
			auto blindItem = GetMangZhuItem();
			UpdateMangZhuItem(blindItem, itemBlindInfo);
			m_vecMangZhuNew.push_back(blindItem);
			blindItem->m_spriteLine->setVisible(true);
			blindItem->m_spriteLine->setPosition(MangZhuScrollW/2, fY);
			//if (itemBlindInfo.m_level == g_globaMttGameInfo.m_mangzhuIndex && g_globaMttGameInfo.m_isGameStart)
			//{
			//    blindItem->m_spriteCurBlind->setVisible(true);
			//    m_dBlindOffset = fY;
			//}
			fY-=oneItemH;
		}
		if (totalItemH < MangZhuScrollH)
		{
			//m_pScrollViewType[2]->setBounceable(false);
		}
		else
		{
			m_pScrollViewType[2]->setBounceable(true);
		}
		m_uTableIdOld = g_globaMttGameInfo.m_curMttTable.m_tableID;
	}

	if (m_dBlindOffset > totalItemH - MangZhuScrollH)
		m_dBlindOffset = MangZhuScrollH - totalItemH;
	else if (m_dBlindOffset < MangZhuScrollH)
		m_dBlindOffset = 0;
	else
		m_dBlindOffset = MangZhuScrollH/2- m_dBlindOffset;



	m_pSpriteBlindBg->setVisible(g_globaMttGameInfo.m_isGameStart) ;


	m_pScrollViewType[2]->setContentOffset(Vec2(0, m_dBlindOffset), false);
	m_pScrollViewType[2]->setContentSize(Size(MangZhuScrollW, totalItemH));
}

//更新所有奖励项
void MTTMatchState::UpdateJiangLiInfo(std::vector<MttReward> vecReward, std::string strTableName)
{
    for (auto item : m_vecFuDongJiangLiNew)
    {
        item->m_spriteLine->setVisible(false);
        m_vecFuDongJiangLiOld.push_back(item);
    }
    m_vecFuDongJiangLiNew.clear();

	for (auto item : m_vecJiangLiNew)
	{
		item->m_spriteLine->setVisible(false);
		m_vecJiangLiOld.push_back(item);
	}
	m_vecJiangLiNew.clear();

    float oneItemH = 71.0;
    float totalItemH = vecReward.size()*oneItemH;
    if (totalItemH < JiangLiScrollH)
    {
        totalItemH = JiangLiScrollH;
        //m_pScrollViewType[3]->setBounceable(false);
    }
    else
    {
        m_pScrollViewType[3]->setBounceable(true);
    }
    m_pScrollViewType[3]->setContentOffset(Vec2(0, JiangLiScrollH-totalItemH), false);
    float posY = totalItemH - oneItemH + 5;
    SetLabelString(m_labelMTTTableName, strTableName);
    unsigned scrollViewW = 0;
    if (g_globaMttGameInfo.m_curMttTable.m_isFloatMatch)
        scrollViewW = JiangLiScrollW-150;
    else
        scrollViewW = JiangLiScrollW;
    for (auto rewardInfo : vecReward)
    {
        JiangLiItem *item = nullptr;
        if (g_globaMttGameInfo.m_curMttTable.m_isFloatMatch)
        {
            item = GetFuDongJiangLiItem();
            m_vecFuDongJiangLiNew.push_back(item);
        }
        else
        {
            item = GetJiangLiItem();
            m_vecJiangLiNew.push_back(item);
        }
        UpdateJiangLiItem(item, rewardInfo);
        item->m_spriteLine->setPosition(scrollViewW/2, posY);
        item->m_spriteLine->setVisible(true);
        posY-=oneItemH;
    }
    m_pScrollViewType[3]->setContentSize(Size(scrollViewW, totalItemH));
}

void MTTMatchState::UpdateFuDongInfo()
{
    for (auto item : m_vecFuDongNew)
    {
        item->m_spriteButton->setVisible(false);
        m_vecFuDongOld.push_back(item);
    }
    m_vecFuDongNew.clear();

    float oneItemH = 50.0;
    float totalItemH = g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_fLoatReward.size()*oneItemH;
    if (totalItemH < FuDongScrollH)
    {
        totalItemH = FuDongScrollH;
        //m_pScrollViewType[4]->setBounceable(false);
		 //m_pScrollViewType[4]->setBounceable(true);
    }
    else
    {
        m_pScrollViewType[4]->setBounceable(true);
    }
    float fY = totalItemH - oneItemH/2;
    //unsigned uCurRoleCountInGame = g_globaMttGameInfo.m_curMttTable.m_roleCountInGame;

	unsigned uCurRoleCountInGame = g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_realRegistCount ;
	
	bool flag = false ;
	unsigned m_curIndex = 0 ;
    for (auto itemInfo : g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_fLoatReward)
    {
        auto item = GetFuDongItem(itemInfo.m_MttReward, g_globaMttGameInfo.m_curMttTable.m_tableName);
        UpdateFuDongItem(item, itemInfo);
        item->m_spriteButton->setPosition(FuDongScrollW/2, fY);
        item->m_spriteButton->setVisible(true);

		item->m_floatIndex = m_curIndex ;
        if (itemInfo.m_rolePre <= uCurRoleCountInGame && itemInfo.m_roleEnd >= uCurRoleCountInGame)
        {
            UpdateJiangLiInfo(itemInfo.m_MttReward, g_globaMttGameInfo.m_curMttTable.m_tableName);
            item->m_spriteCurSel->setVisible(true);
            item->m_spriteBtnBg->setVisible(true);
            item->m_labelPlayerNum->setTextColor(Color4B(0xff,0xff,0xff,0xff));
			flag = true ;
        }
        else
        {
            item->m_spriteCurSel->setVisible(false);
            item->m_spriteBtnBg->setVisible(false);
            item->m_labelPlayerNum->setTextColor(Color4B(0x50,0xbd,0x2e,0xff));
        }
		m_curIndex++ ;

        m_vecFuDongNew.push_back(item);
        fY -= oneItemH;
    }

	if (!flag)
	{
		for (auto item : m_vecFuDongNew)
		{
			item->m_spriteBtnBg->setVisible(item->m_floatIndex==0);
			item->m_labelPlayerNum->setTextColor(Color4B(0x50,0xbd,0x2e,0xff));
			if (item->m_floatIndex==0&&g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_fLoatReward.size() > 0)
			{
				std::vector<MttReward> m_MttReward = g_globaMttGameInfo.m_curMttTable.m_MttFudongReward.m_fLoatReward.at(0).m_MttReward;
				UpdateJiangLiInfo(m_MttReward,  g_globaMttGameInfo.m_curMttTable.m_tableName);
			}
			
		}
		//auto item = GetFuDongItem(itemInfo.m_MttReward, g_globaMttGameInfo.m_curMttTable.m_tableName);
	}

    m_pScrollViewType[4]->setContentOffset(Vec2(0, FuDongScrollH-totalItemH), false);
    m_pScrollViewType[4]->setContentSize(Size(FuDongScrollW, totalItemH));
}

//获取概述项
void MTTMatchState::GetGaiShuItem()
{
	if (m_pGaishuItem == nullptr)
	{
		GaishuItem *gaishuItem = new GaishuItem;
		std::string spriteName[8] = {"mtt_bmtitle","mttcoin","mtt_time_icon","mtt_human_icon","mtt_first-money","mtt_buttonr","mtt_buttona","mtt_delay_icon"};
			for (int i=0; i<8; i++)
			{
				gaishuItem->m_GaishuSptite[i] = CreateNode1AndPosAddTo<Sprite>("gamemtt/"+spriteName[i]+".png", 50, 0, m_pScrollViewType[0], 0);
				gaishuItem->m_GaiShuLine[i] = CreateNodeAndPosAddTo<LineLabel>(80, 10, m_pScrollViewType[0], 0);
			}
			gaishuItem->m_labelTD[0] = CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "", Vec2(0.5,0.5), 299, 25, Color4B(0xff,0xd2,0x00,0xff), gaishuItem->m_GaishuSptite[0], 0);
			gaishuItem->m_labelTD[1] = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0,0.5), 80, 20+24+10, Color4B::WHITE, m_pScrollViewType[0], 0);
			m_pGaishuItem = gaishuItem;
		}
	}

	//更新概述项
	void MTTMatchState::UpdateGaiShuInfo(const GameMttTableInfo& tableInfo)
	{
		GetGaiShuItem();
		m_bSignUp = (tableInfo.m_SiginState==4 ||tableInfo.m_SiginState==5)?true:false;
		OnMenuItemSelected(nullptr, 0);
		m_iLastTime = tableInfo.m_realTime;
		if (m_pGaishuItem != nullptr)
		{
			for (int i=0;i<8;i++)
			{
				m_pGaishuItem->m_GaishuSptite[i]->setVisible(false);
				m_pGaishuItem->m_GaiShuLine[i]->setVisible(false);
				m_pGaishuItem->m_GaiShuLine[i]->resetLable();
				m_pGaishuItem->m_GaiShuLine[i]->ignoreAnchorPointForPosition(false);
				m_pGaishuItem->m_GaiShuLine[i]->setAnchorPoint(Vec2(0, 0.5));
			}
			SetLabelString(m_pGaishuItem->m_labelTD[0], "");
			SetLabelString(m_pGaishuItem->m_labelTD[1], "");
			SetLabelString(m_pGaishuItem->m_labelTD[0], tableInfo.m_tableName);
			////滚动视图中内容最低Y坐标
			float posY = 20;
			char buff[32] = {0};
			bool bDelaySign = false;
			if (tableInfo.m_isdelayRegist)//延时报名
			{
				m_pGaishuItem->m_GaishuSptite[7]->setVisible(true);
				m_pGaishuItem->m_labelTD[1]->setPosition(80, posY);
				m_pGaishuItem->m_GaishuSptite[7]->setPosition(50, posY);
				SetLabelString(m_pGaishuItem->m_labelTD[1], "延时报名：比赛开始后，可以在延时时段内报名参赛");
				bDelaySign = true;
			}
			if (tableInfo.m_isAddOn)//可增购
			{
				posY+=39;
				m_pGaishuItem->m_GaiShuLine[7]->setVisible(true);
				m_pGaishuItem->m_GaiShuLine[6]->setVisible(true);
				m_pGaishuItem->m_GaishuSptite[6]->setVisible(true);
				m_pGaishuItem->m_GaiShuLine[7]->setPosition(180, posY);
				posY+=30;
				m_pGaishuItem->m_GaiShuLine[6]->setPosition(80, posY);
				m_pGaishuItem->m_GaishuSptite[6]->setPosition(50, posY);
				m_pGaishuItem->m_GaiShuLine[6]->Appand(22, "fzlt.ttf", "(Add On)：可增购比赛，次数：", Color4B::WHITE);
				sprintf(buff, "%d", tableInfo.m_mttTableDes.m_addOnCount);
				m_pGaishuItem->m_GaiShuLine[6]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[6]->Appand(22, "fzlt.ttf", "次", Color4B::WHITE);
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", "第", Color4B::WHITE);
				//sprintf(buff, "%d", tableInfo.m_mttTableDes.m_addOnLevelMin);
                if (tableInfo.m_mttTableDes.m_addOnLevelMin == tableInfo.m_mttTableDes.m_addOnLevelMax)
                {
                    sprintf(buff, "%d", tableInfo.m_mttTableDes.m_addOnLevelMin);
                }
                else
				    sprintf(buff, "%d-%d", tableInfo.m_mttTableDes.m_addOnLevelMin, tableInfo.m_mttTableDes.m_addOnLevelMax);
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", "个盲注级别可用", Color4B::WHITE);
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", FormatCash(tableInfo.m_mttTableDes.m_addOnMoney), Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", "德州币兑换", Color4B::WHITE);
				//sprintf(buff, "%d", tableInfo.m_mttTableDes.m_addOnToCarch);
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", FormatCash(tableInfo.m_mttTableDes.m_addOnToCarch), Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[7]->Appand(22, "fzlt.ttf", "筹码", Color4B::WHITE);
			}
			if (tableInfo.m_isRebuy)
			{
				posY+=39;
				m_pGaishuItem->m_GaiShuLine[5]->setVisible(true);
				m_pGaishuItem->m_GaiShuLine[4]->setVisible(true);
				m_pGaishuItem->m_GaishuSptite[5]->setVisible(true);
				m_pGaishuItem->m_GaiShuLine[5]->setPosition(180, posY);
				posY+=30;
				m_pGaishuItem->m_GaiShuLine[4]->setPosition(80, posY);
				m_pGaishuItem->m_GaishuSptite[5]->setPosition(50, posY);
				m_pGaishuItem->m_GaiShuLine[4]->Appand(22, "fzlt.ttf", "(rebuy)：可重购比赛，次数：", Color4B::WHITE);
				sprintf(buff, "%d", tableInfo.m_mttTableDes.m_rebuyCount);
				m_pGaishuItem->m_GaiShuLine[4]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[4]->Appand(22, "fzlt.ttf", "次", Color4B::WHITE);
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", "第", Color4B::WHITE);
                 if (tableInfo.m_mttTableDes.m_rebuyMin == tableInfo.m_mttTableDes.m_rebuyMax)
                 {
                     sprintf(buff, "%d", tableInfo.m_mttTableDes.m_rebuyMin);
                 }
                 else
				    sprintf(buff, "%d-%d", tableInfo.m_mttTableDes.m_rebuyMin, tableInfo.m_mttTableDes.m_rebuyMax);
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", "个盲注级别前可用", Color4B::WHITE);
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", FormatCash(tableInfo.m_mttTableDes.m_rebuyMoney), Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", "德州币兑换", Color4B::WHITE);
				//sprintf(buff, "%d", tableInfo.m_mttTableDes.m_rebuyToCarch);
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", FormatCash(tableInfo.m_mttTableDes.m_rebuyToCarch), Color4B(0xff,0xd2,0x00,0xff));
				m_pGaishuItem->m_GaiShuLine[5]->Appand(22, "fzlt.ttf", "筹码", Color4B::WHITE);
			}
			if (posY == 20 && !bDelaySign)
				posY += 52;//没有增购、重购、延时报名时，其他坐标调整
			else if (posY == 20 && bDelaySign)//没有增购、重购其他坐标调整
			{
				posY += 52;
				m_pGaishuItem->m_labelTD[1]->setPosition(80, posY);
				m_pGaishuItem->m_GaishuSptite[7]->setPosition(50, posY);
			}
			for (int i=3; i>=0; i--)
			{
				m_pGaishuItem->m_GaiShuLine[i]->setVisible(true);
				m_pGaishuItem->m_GaishuSptite[i+1]->setVisible(true);
				posY+=39;
				m_pGaishuItem->m_GaiShuLine[i]->setPosition(80, posY);
				m_pGaishuItem->m_GaishuSptite[i+1]->setPosition(50, posY);
			}
			posY+=70;
			m_pGaishuItem->m_GaishuSptite[0]->setVisible(true);
			m_pGaishuItem->m_GaishuSptite[0]->setPosition(GaiShuScrollW/2, posY);
			posY+=60;
			//sprintf(buff, "%d", tableInfo.m_mttTableDes.m_siginDes);
			m_pGaishuItem->m_GaiShuLine[0]->Appand(22, "fzlt.ttf", "报名费用：", Color4B::WHITE);
			m_pGaishuItem->m_GaiShuLine[0]->Appand(22, "fzlt.ttf", tableInfo.m_mttTableDes.m_siginDes.c_str(), Color4B(0xff,0xd2,0x00,0xff));
			sprintf(buff, "%s", tableInfo.m_mttTableDes.m_matchStartTime.c_str());
			m_pGaishuItem->m_GaiShuLine[1]->Appand(22, "fzlt.ttf", "比赛时间：", Color4B::WHITE);
			m_pGaishuItem->m_GaiShuLine[1]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
			sprintf(buff, "%d-%d人", tableInfo.m_mttTableDes.m_minRole, tableInfo.m_mttTableDes.m_maxRole);
			m_pGaishuItem->m_GaiShuLine[2]->Appand(22, "fzlt.ttf", "比赛人数：", Color4B::WHITE);
			m_pGaishuItem->m_GaiShuLine[2]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
			sprintf(buff, "%d", tableInfo.m_mttTableDes.m_firstMoney);
			m_pGaishuItem->m_GaiShuLine[3]->Appand(22, "fzlt.ttf", "初始筹码：", Color4B::WHITE);
			m_pGaishuItem->m_GaiShuLine[3]->Appand(22, "fzlt.ttf", buff, Color4B(0xff,0xd2,0x00,0xff));
			if (posY > GaiShuScrollH)
			{
				m_pScrollViewType[0]->setBounceable(true);
			}
			else
			{
				m_pScrollViewType[0]->setBounceable(false);
			}
			m_pScrollViewType[0]->setContentOffset(Vec2(0, GaiShuScrollH-posY));
			m_pScrollViewType[0]->setContentSize(Size(GaiShuScrollW, posY));
		}
	}

	//获取一个排名项
	MTTMatchState::PaiMingItem* MTTMatchState::GetPaiMingItem()
	{
		PaiMingItem *paimingItem = nullptr;
		if (m_vecPaiMingOld.empty())
		{
			auto spriteBg = ui::Scale9Sprite::create("gamefriendlist/bgitem.png", Rect(0,0,662,90), Rect(150,20,362,50));
			spriteBg->setPreferredSize(Size(686, 100));
			m_pScrollViewType[1]->addChild(spriteBg);
			auto httpSprite = CreateUserHeadSprite("defaulticon.png", "gamefriendlist/iconclipmask.png", spriteBg, Vec2(10, 4), 

				"gamefriendlist/iconround.png");
			auto rankFlag = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 96, spriteBg, 0);
			auto spriteCoin = CreateNode1AndPosAddTo<Sprite>("gamemtt/mttcoin.png", 450, 50, spriteBg, 0);
			auto labelRankId = CreateLabelMSYHAndAnchorPosClrAddTo(18, "", Vec2(0.5,0.5), 31, 21, Color4B(0xff,0xd2,0x00,0xff), rankFlag, 0);
			auto labelPlayerName = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0,0.5), 120, 50, Color4B::WHITE, spriteBg, 0);
			auto labelCash = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0,0.5), 475, 50, Color4B(0xff,0xd2,0x00,0xff), spriteBg, 0);
			PaiMingItem *item= new PaiMingItem;
			item->m_spriteItemBg = spriteBg;
			item->m_httpSpriteIcon = httpSprite;
			item->m_spriteRankFlag = rankFlag;
			item->m_spriteCoin = spriteCoin;
			item->m_labelRankId = labelRankId;
			item->m_labelPlayerName = labelPlayerName;
			item->m_labelCash = labelCash;
			paimingItem = item;
		}
		else
		{
			paimingItem = m_vecPaiMingOld.back();
			m_vecPaiMingOld.pop_back();
		}
		return paimingItem;
	}

	//更新一个排名项
	void MTTMatchState::UpdatePaiMingItem(MTTMatchState::PaiMingItem* item, const MttRankInTable& mttRank)
	{
		SetIconUrl(item->m_httpSpriteIcon, mttRank.m_icon);
		char buff[32] = {0};
		if (mttRank.m_rankid <=3 && mttRank.m_rankid>=1)
		{
			item->m_spriteRankFlag->SetFrameIdx(mttRank.m_rankid-1);
			item->m_labelRankId->setVisible(false);
		}
		else
		{
			item->m_labelRankId->setVisible(true);
			item->m_spriteRankFlag->SetFrameIdx(3);
			sprintf(buff, "%d", mttRank.m_rankid);
			SetLabelString(item->m_labelRankId, buff);
		}
		SetLabelString(item->m_labelPlayerName, mttRank.m_name);
		SetLabelString(item->m_labelCash, FormatCash(mttRank.m_cash));

	}

	//获取一个盲注项
	MTTMatchState::MangZhuItem* MTTMatchState::GetMangZhuItem()
	{
		MangZhuItem *mangZhuItem = nullptr;
		if (m_vecMangZhuOld.empty())
		{
			auto spriteLine = CreateNode1AndPosAddTo<Sprite>("gamesng/sng_table_background-9.png", 0, 0, m_pScrollViewType[2], 0);
			auto labelLevel = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 45, 20, Color4B::WHITE, spriteLine, 0);
			auto spriteAddon = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_buttona.png", 80, 20, spriteLine, 0);
			auto spriteRebuy = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_buttonr.png", 110, 20, spriteLine, 0);
			auto spriteCurBlindBg = CreateNode1AndPosAddTo<Sprite>("gamemtt/cur_blind_bg.png", 329, 24, spriteLine, 0);
			auto labelBlind = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 220, 20, Color4B::WHITE, spriteLine, 0);
			auto labelAnti = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 395, 20, Color4B::WHITE, spriteLine, 0);
			auto labelTime = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 585, 20, Color4B::WHITE, spriteLine, 0);
			spriteAddon->setVisible(false);
			spriteRebuy->setVisible(false);
			spriteCurBlindBg->setVisible(false);
			MangZhuItem *blindItem = new MangZhuItem;
			blindItem->m_spriteLine = spriteLine;
			blindItem->m_labelLevel = labelLevel;
			blindItem->m_spriteAddOn = spriteAddon;
			blindItem->m_spriteRebuy = spriteRebuy;
			blindItem->m_labelBlind = labelBlind;
			blindItem->m_labelAnti = labelAnti;
			blindItem->m_labelTime = labelTime;
			blindItem->m_spriteCurBlind = spriteCurBlindBg;
			mangZhuItem = blindItem;
		}
		else
		{
			mangZhuItem = m_vecMangZhuOld.back();
			mangZhuItem->m_spriteCurBlind->setVisible(false);
			mangZhuItem->m_spriteAddOn->setVisible(false);
			mangZhuItem->m_spriteRebuy->setVisible(false);
			m_vecMangZhuOld.pop_back();
		}

		return mangZhuItem;
	}
	//更新一个盲注项
	void MTTMatchState::UpdateMangZhuItem(MTTMatchState::MangZhuItem* item, const MttBlind& mttBlind)
	{
		char buff[32] = {0};
		sprintf(buff, "%d", mttBlind.m_level);
		SetLabelString(item->m_labelLevel, buff);

		item->m_spriteAddOn->setVisible(mttBlind.m_isAddon);
		item->m_spriteRebuy->setVisible(mttBlind.m_isRebuy);

		if (mttBlind.m_isAddon)
		{
			item->m_spriteAddOn->setVisible(true);
			if (mttBlind.m_isRebuy)
			{
				item->m_spriteRebuy->setVisible(true);
				item->m_spriteRebuy->setPosition(110, 20);
			}
		}
		else
		{
			if (mttBlind.m_isRebuy)
			{
				item->m_spriteRebuy->setVisible(true);
				item->m_spriteRebuy->setPosition(80, 20);
			}
		}

		SetLabelString(item->m_labelBlind, mttBlind.m_blind);
		sprintf(buff, "%d", mttBlind.m_preBlind);
		SetLabelString(item->m_labelAnti, buff);
		sprintf(buff, "%d秒", mttBlind.m_time);
		SetLabelString(item->m_labelTime, buff);
	}

	//获取一个奖励项
	MTTMatchState::JiangLiItem* MTTMatchState::GetJiangLiItem()
	{
		JiangLiItem *jiangLiItem = nullptr;
		if (m_vecJiangLiOld.empty())
		{
			auto spriteLine = CreateNode1AndPosAddTo<Sprite>("gamesng/sng_table_background-9.png", 0, 0, m_pScrollViewType[3], 0);
			auto spriteRewardCup1 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup1.png", 73, 35, spriteLine, 0);
			auto spriteRewardCup2 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup2.png", 73, 35, spriteLine, 0);
			auto spriteRewardCup3 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup3.png", 73, 35, spriteLine, 0);
			auto spriteRewardCup4 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup4.png", 73, 35, spriteLine, 0);
			auto spriteRewardCup5 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup5.png", 73, 35, spriteLine, 0);
			auto spriteGold = CreateNode1AndPosAddTo<Sprite>("gamemtt/mttcoin.png", 170, 35, spriteLine, 0);
			auto labelRankId0 = CreateLabelMSYHBDAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 72, 37, Color4B::WHITE, spriteLine, 0);
			auto labelRankId1 = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 72, 37, Color4B(0x24,0x10,0x00,0xff), spriteLine, 0);
			auto labelReward = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0,0.5), 190, 35, Color4B::WHITE, spriteLine, 0);
			auto labelScore = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), 586, 35, Color4B::WHITE, spriteLine, 0);
			JiangLiItem *item = new JiangLiItem;
			item->m_spriteLine = spriteLine;
			item->m_spriteRewardCup[0] = spriteRewardCup1;
			item->m_spriteRewardCup[1] = spriteRewardCup2;
			item->m_spriteRewardCup[2] = spriteRewardCup3;
			item->m_spriteRewardCup[3] = spriteRewardCup4;
			item->m_spriteRewardCup[4] = spriteRewardCup5;
			item->m_spriteGold = spriteGold;
			item->m_labelRankId[0] = labelRankId0;
			item->m_labelRankId[1] = labelRankId1;
			item->m_labelReward = labelReward;
			item->m_labelScore = labelScore;
			jiangLiItem = item;
		}
		else
		{
			jiangLiItem = m_vecJiangLiOld.back();
			m_vecJiangLiOld.pop_back();
		}

		return jiangLiItem;
	}

//获取一个浮动奖励项
MTTMatchState::JiangLiItem* MTTMatchState::GetFuDongJiangLiItem()
{
    JiangLiItem *jiangLiItem = nullptr;
    if (m_vecFuDongJiangLiOld.empty())
    {
        auto spriteLine = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_fd_line.png", 0, 0, m_pScrollViewType[3], 0);
        auto spriteRewardCup1 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup1.png", 41, 35, spriteLine, 0);
        auto spriteRewardCup2 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup2.png", 41, 35, spriteLine, 0);
        auto spriteRewardCup3 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup3.png", 41, 35, spriteLine, 0);
        auto spriteRewardCup4 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup4.png", 41, 35, spriteLine, 0);
		auto spriteRewardCup5 = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_cup5.png", 41, 37, spriteLine, 0);
        auto spriteGold = CreateNode1AndPosAddTo<Sprite>("gamemtt/mttcoin.png", 200, 35, spriteLine, 0);
        auto labelRankId0 = CreateLabelMSYHBDAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 40, 37, Color4B::WHITE, spriteLine, 0);
        auto labelRankId1 = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5), 40, 37, Color4B(0x24,0x10,0x00,0xff), spriteLine, 0);
        auto labelReward = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0,0.5), 167, 35, Color4B::WHITE, spriteLine, 0);
        auto labelScore = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), 416, 35, Color4B::WHITE, spriteLine, 0);
        JiangLiItem *item = new JiangLiItem;
        item->m_spriteLine = spriteLine;
        item->m_spriteRewardCup[0] = spriteRewardCup1;
        item->m_spriteRewardCup[1] = spriteRewardCup2;
        item->m_spriteRewardCup[2] = spriteRewardCup3;
        item->m_spriteRewardCup[3] = spriteRewardCup4;
		item->m_spriteRewardCup[4] = spriteRewardCup5;
        item->m_spriteGold = spriteGold;
        item->m_labelRankId[0] = labelRankId0;
        item->m_labelRankId[1] = labelRankId1;
        item->m_labelReward = labelReward;
        item->m_labelScore = labelScore;
        jiangLiItem = item;
    }
    else
    {
        jiangLiItem = m_vecFuDongJiangLiOld.back();
        m_vecFuDongJiangLiOld.pop_back();
    }

    return jiangLiItem;
}
	//更新一个奖励项
	void MTTMatchState::UpdateJiangLiItem(MTTMatchState::JiangLiItem* item, const MttReward& mttReward)
	{
		char buff[64] = {0};
		for (int i=0; i<5; i++)
			item->m_spriteRewardCup[i]->setVisible(false);

		if (g_globaMttGameInfo.m_curMttTable.m_isFloatMatch)
		{
			if (mttReward.m_rankid <= 3)
			{
				item->m_spriteRewardCup[mttReward.m_rankid-1]->setVisible(true) ;

				item->m_labelRankId[0]->setVisible(true);
				item->m_labelRankId[1]->setVisible(false);
				sprintf(buff, "%s", mttReward.m_roleIntervals.c_str());
				SetLabelString(item->m_labelRankId[0], buff);
			}
			else
			{
				
				item->m_labelRankId[0]->setVisible(false);
				item->m_labelRankId[1]->setVisible(true);

				
				if (mttReward.m_rolePre==mttReward.m_roleEnd)
				{
					item->m_spriteRewardCup[3]->setVisible(true);
					item->m_labelRankId[1]->setTextColor(Color4B(0x24,0x10,0x00,0xff));
				}
				else
				{
					item->m_spriteRewardCup[4]->setVisible(true);
					item->m_labelRankId[1]->setTextColor(Color4B(0xbc,0x9c,0x58,0xff));
				}
				sprintf(buff, "%s", mttReward.m_roleIntervals.c_str());
				SetLabelString(item->m_labelRankId[1], buff);
			}





			item->m_labelReward->setAnchorPoint(Point(1,0.5)) ;
			item->m_labelReward->setPosition(265, 35);
			item->m_spriteGold->setVisible(false);

			sprintf(buff,"%s",mttReward.m_rewardInfo.c_str()) ;
			SetLabelString(item->m_labelReward, buff);

			sprintf(buff, "%d", mttReward.m_score);
			SetLabelString(item->m_labelScore, buff);


		}
		else
		{
			if (mttReward.m_rankid <= 3)
			{
				item->m_spriteRewardCup[mttReward.m_rankid-1]->setVisible(true) ;

				item->m_labelRankId[0]->setVisible(true);
				item->m_labelRankId[1]->setVisible(false);
				sprintf(buff, "%d", mttReward.m_rankid);
				SetLabelString(item->m_labelRankId[0], buff);
			}
			else
			{
				item->m_spriteRewardCup[3]->setVisible(true);
				item->m_labelRankId[0]->setVisible(false);
				item->m_labelRankId[1]->setVisible(true);
				sprintf(buff, "%d", mttReward.m_rankid);
				SetLabelString(item->m_labelRankId[1], buff);
			}
			if (!mttReward.m_bShowGold)
			{
				item->m_labelReward->setPosition(160, 35);
			}
			else
			{
				item->m_labelReward->setPosition(190, 35);
			}
			if (g_globaMttGameInfo.m_curMttTable.m_isFloatMatch)
				item->m_labelReward->setPosition(167, 35);
			item->m_spriteGold->setVisible(mttReward.m_bShowGold);

			if (mttReward.m_bShowItem&&mttReward.m_bShowGold)
			{
				sprintf(buff,"%s+德州币%s",mttReward.m_rewardInfo.c_str(),FormatCash(mttReward.m_money).c_str()) ;
			}else if (!mttReward.m_bShowItem&&mttReward.m_bShowGold)
			{
				sprintf(buff,"德州币%s",FormatCash(mttReward.m_money).c_str()) ;
			}else if (mttReward.m_bShowItem&&!mttReward.m_bShowGold)
			{
				sprintf(buff,"%s",mttReward.m_rewardInfo.c_str()) ;
			}
			item->m_labelReward->setAnchorPoint(Point(0,0.5)) ;
			SetLabelString(item->m_labelReward, buff);
			sprintf(buff, "%d", mttReward.m_score);
			SetLabelString(item->m_labelScore, buff);
		}




	}

	void MTTMatchState::setVisible(bool bVisible)
	{
        m_bIsVisible = bVisible;
		
		PlayVisibleSound(bVisible);
		if (bVisible)
		{
			GameDialog::setVisible(bVisible);
			if(g_globaMttGameInfo.m_isGameStart)
			{
				SendClientRequest(ClientRequest::kCallMttMatchRankList) ;
				showDataLoading(m_uCurSelectIndex==PaimingBtn && g_globaMttGameInfo.m_isGameStart);
				m_bRequestData = true;
			}

			Tools::callScaleAction(this);
			//SendClientRequest(ClientRequest::kGetActivityInfo);
		}
		else
		{
			GameDialog::setVisible(bVisible);
			SendClientRequest(ClientRequest::kOptMTTHallBgLayer);
			//callHideAction() ;
		}
	}

	void MTTMatchState::onGrabTouchBegan(bool bHitDialog)
	{
	}
	
MTTMatchState::FuDongItem* MTTMatchState::GetFuDongItem(std::vector<MttReward> vecReward, std::string tableName)
{
    FuDongItem* fudongItem = nullptr;
    if (m_vecFuDongOld.empty())
    {
        auto spriteBtn = FrameSpriteButton::createWithVertFrameSprite("gamemtt/mtt_fd_tab.png", false, true);
        m_pScrollViewType[4]->addChild(spriteBtn);
        auto sz = spriteBtn->getContentSize();
        auto spriteBg = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_fd_tab1.png", 0, 0, spriteBtn, 0);
        auto label = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5,0.5), 0, 0, Color4B(0x50,0xbd,0x2e,0xff), spriteBtn, 0);
        auto spriteCur = CreateNode1AndPosAddTo<Sprite>("gamemtt/mtt_fd_bq.png", -50, 5, spriteBtn, 0);
        FuDongItem *item = new FuDongItem;
        item->m_spriteButton = spriteBtn;
        item->m_spriteCurSel = spriteCur;
        item->m_spriteBtnBg = spriteBg;
        item->m_labelPlayerNum = label;
        item->m_spriteCurSel->setVisible(false);
        item->m_spriteBtnBg->setVisible(false);
        fudongItem = item;
    }
    else
    {
        fudongItem = m_vecFuDongOld.back();
        m_vecFuDongOld.pop_back();
    }
    fudongItem->m_spriteButton->SetHitCB([this, vecReward, tableName, fudongItem](Ref*)
    {
        UpdateJiangLiInfo(vecReward, tableName);
        for (auto item : m_vecFuDongNew)
        {
            item->m_spriteBtnBg->setVisible(false);
            item->m_labelPlayerNum->setTextColor(Color4B(0x50,0xbd,0x2e,0xff));
        }
        fudongItem->m_spriteBtnBg->setVisible(true);
        fudongItem->m_labelPlayerNum->setTextColor(Color4B(0xff,0xff,0xff,0xff));
    });
    return fudongItem;
}

void MTTMatchState::UpdateFuDongItem(MTTMatchState::FuDongItem* item, const SingleFloatReward& mttReward)
{
    char buff[32] = {0};
    sprintf(buff, "%d-%d人", mttReward.m_rolePre, mttReward.m_roleEnd);
    SetLabelString(item->m_labelPlayerNum, buff);
}

	void MTTMatchState::showDataLoading(bool bVisible)
	{
		if (s_dataLoading == nullptr)
		{
			s_dataLoading = Tools::creatDataLoading(this,514, 293,99) ;
		}
		s_dataLoading->setVisible(bVisible) ;
	}