#include "MttRankView.h"

#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "Util.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "GameFrame.h"
#include "RankTexture.h"
#include "HttpSprite.h"
#include "Tools.h"
USING_NS_CC;

bool MttRankView::init()
{

	if (!GameDialog::init("userinfo/bgdialog0.png",false)) 
	{
		return false ;
	}

    //获得的名次包括输、赢
    m_pRankTexture = RankTexture::create();
    m_pRankTexture->ignoreAnchorPointForPosition(false);
    m_pRankTexture->setAnchorPoint(Vec2(0,0.5));
    m_pRankTexture->setPosition(276, 288);
    this->addChild(m_pRankTexture);
    //奖杯及失败表情
    m_pWinCup = CreateNode1AndPosAddTo<Sprite>("gamesng/rankview/win2.png", 136, 270, this, 0);
    m_pFailedExpress = CreateNode1AndPosAddTo<Sprite>("gamesng/rankview/failed.png", 182, 250, this, 0);
    auto sz = this->getContentSize();
    m_pLabelWinDes = CreateLabelMSYHAndAnchorPosClrAddTo(24, "你牌技这么好，朋友们造吗？赶紧告诉他们吧~", Vec2(0.5,0.5), sz.width/2, 105, Color4B(0x50,0xbd,0x2e,0xff), this, 0);
    m_pLabelFailed = CreateLabelMSYHAndAnchorPosClrAddTo(28, "没杀进奖励圈，没关系，调\n整好状态再来一局吧", Vec2(0,0), 260, 160, Color4B(0x50,0xbd,0x2e,0xff), this, 0);
    m_pLabelFailed->setAlignment(TextHAlignment::LEFT);
    m_pLabelWin[0] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "奖    励：", Vec2(0,0), 277, 229, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_pLabelWin[1] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "大师分：", Vec2(0,0), 277, 192, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_pLabelWin[2] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "奖    金：", Vec2(0,0), 277, 155, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_pLabelReward[0] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0,0), 390, 229, Color4B(0xff,0xd2,0x00,0xff), this, 0);
    m_pLabelReward[1] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0,0), 390, 192, Color4B(0xff,0xd2,0x00,0xff), this, 0);
    m_pLabelReward[2] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0,0), 390, 155, Color4B(0xff,0xd2,0x00,0xff), this, 0);

    std::string strBt[2] = {"返回大厅", "留下旁观"};
    for (int i=0; i<2; i++)
    {
        m_pMenuItem[i] = MenuItemScale9SpriteWithLabel::create(0, Size(227, 70), CC_CALLBACK_1(MttRankView::onMenuItemBottomSelect, this));
        auto label = CreateLabelMSYHBD(30, strBt[i]);
        m_pMenuItem[i]->SetLabel(label);
        m_pMenuItem[i]->setAnchorPoint(Vec2(0.5, 0.5));
        m_pMenuItem[i]->setPosition(223+i*230, 50);
    }

    auto menu = Menu::create(m_pMenuItem[0], m_pMenuItem[1], nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    m_pRankFirst = nullptr;
    m_bIsWin = false;
	return true ;

}

void MttRankView::GetReword()
{
    m_uMaster = 100;
    m_dReword = 10000;
}

void MttRankView::SetAllSpriteVisible(bool bVisible)
{
    m_bIsWin = false;
    m_pWinCup->setVisible(bVisible);
    m_pFailedExpress->setVisible(bVisible);
    m_pLabelFailed->setVisible(bVisible);
    m_pLabelWinDes->setVisible(bVisible);
    m_pRankTexture->setVisible(bVisible);
    m_pRankTexture->resetTexTure();

    for (int i=0; i<3; i++)
    {
        m_pLabelWin[i]->setVisible(bVisible);
        m_pLabelReward[i]->setVisible(bVisible);
    }
    if (m_pRankFirst != nullptr)
        m_pRankFirst->setVisible(false);
}
/************************************************************************/
/* 
1 物品
2 德州币
3大师分
*/
/************************************************************************/
void MttRankView::updateMttRankView(unsigned uRankId,std::string desc,std::string icon,std::string name,unsigned cash,unsigned score)
{

	m_bShowItem = name==""?false:true ;
	m_bShowScore = score>0 ;
	m_bShowGold = cash>0 ;
	m_curRank = uRankId ;

	m_uMaster = score;
	m_dReword = cash;

    SetAllSpriteVisible(false);
    char buff[64] = {0};

	if (uRankId >=1 && uRankId <= 5)
	{
		//第一名特殊界面
        auto frameSz = this->getContentSize();
        if (m_pRankFirst == nullptr)
        {
            m_pRankFirst = MTTRankViewFirst::create();
            m_pRankFirst->ignoreAnchorPointForPosition(false);
            m_pRankFirst->setAnchorPoint(Vec2(0.5,0.5));
            m_pRankFirst->setPosition(frameSz.width/2, frameSz.height/2);
            this->addChild(m_pRankFirst);
        }
        m_pRankFirst->setVisible(true);
        m_pRankFirst->updateMTTRankViewFirst(uRankId, desc, icon, name, cash, score);
        m_bIsWin = true;

	}else if (!m_bShowItem && !m_bShowScore &&!m_bShowGold)
	{
		//无奖励界面
        m_pRankTexture->setVisible(true);
        m_pRankTexture->Appand("ranktexture/failedbg1.png");
        m_pRankTexture->Appand(0, uRankId);
        m_pRankTexture->Appand("ranktexture/failedbg2.png");
        m_pFailedExpress->setVisible(true);
        m_pLabelFailed->setVisible(true);
        //SetLabelString(m_pMenuItem[1]->GetLabel(), "留下旁观");
        m_bIsWin = false;
	}else
	{
		//第二名到所有有奖励的人  更具是否显示 物品 德州币 大师分 动态调控位置
		m_pWinCup->setVisible(true);
        m_pRankTexture->setVisible(true);
        m_pLabelWinDes->setVisible(true);
        m_pRankTexture->Appand("ranktexture/winbg1.png");
        m_pRankTexture->Appand(1, uRankId);
        m_pRankTexture->Appand("ranktexture/winbg2.png");
        //SetLabelString(m_pMenuItem[1]->GetLabel(), "告诉好友");
        for (int i=1; i<3; i++)
        {
            m_pLabelWin[i]->setVisible(true);
            m_pLabelReward[i]->setVisible(true);
        }
        if (m_bShowItem)
        {
            m_pLabelWin[0]->setVisible(true);
            m_pLabelReward[0]->setVisible(true);
        }
        //SetLabelString(m_pLabelReward[0], name);
        SetLabelStringWithBoundWidth(m_pLabelReward[0], name, 250);
        sprintf(buff, "%d", score);
        SetLabelString(m_pLabelReward[1], buff);
        sprintf(buff, "%d", cash);
        SetLabelString(m_pLabelReward[2], buff);
        m_bIsWin = true;
	}

}

void MttRankView::onMenuItemBottomSelect(cocos2d::Ref*pMenuItem)
{
    if (pMenuItem == m_pMenuItem[0])//返回大厅
    {
        GameDialog::setVisible(false) ;
        SendClientRequest(ClientRequest::kCallLevelTable) ;
    }
    else if (pMenuItem == m_pMenuItem[1])//告诉好友，留下旁观
    {
        if (m_bIsWin)//告诉好友
        {
            doLookUp() ;
        }
        else//留下旁观
        {
            doLookUp() ;
        }
    }
}

void MttRankView::onGrabTouchBegan(bool bHitDialog)
{

}

void MttRankView::doLookUp()
{

    setVisible(false) ;

	//SendClientRequest(ClientRequest::kStandUpByMttMatch);

	if (g_globaMttGameInfo.m_isGameOver)
	{
		PopTipWithBt1("游戏结束，请返回大厅", "确定", [this](unsigned uBt){
			if (uBt == 0)
			{
				SendClientRequest(ClientRequest::kCallLevelTable) ;
			}
		});
	
	}else if (g_globaMttGameInfo.m_hasDismassTable)
	{
		SendClientRequest(ClientRequest::kCallLevelTable) ;
		PopTipWithBt1("比赛已合桌，旁观用户请返回大厅","确定",nullptr) ;
	}

}

void MttRankView::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
	GameDialog::setVisible(bVisible) ;
    if (bVisible)
    {
		/*this->setScale(0.7f) ;
		ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
		this->runAction(easeBounceOut);*/
		GameDialog::setVisible(bVisible) ;
		Tools::callScaleAction(this) ;
    }
    else
    {
		if (m_pRankFirst!=nullptr)
		{
			m_pRankFirst->setVisible(false) ;
		}
		callHideAction() ;
		
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
//mtt第一名特殊界面
bool MTTRankViewFirst::init()
{
    if (!GameDialog::init("gamemtt/rankview/share_mttbg.png", false))
    {
        return false;
    }
    //赛事背景
    auto frameSz = this->getContentSize();
    auto bgLayer = Layer::create();
    bgLayer->setContentSize(this->getContentSize());
    this->addChild(bgLayer, 2);
    auto bgSpriteContextLeft = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/share_border.png", frameSz.width/4, frameSz.height/2, bgLayer, 2);
    auto bgSpriteContextRight = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/share_border.png", 3*frameSz.width/4, frameSz.height/2, bgLayer, 2);
    bgSpriteContextRight->setFlipX(true);
    //标题
    auto titleSprite = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/share_mtt-title.png", frameSz.width/2, 600, bgLayer, 0);
    //名次
    m_rankTexture = RankTexture::create();
    m_rankTexture->setPosition(frameSz.width/2-13, 446);
    bgLayer->addChild(m_rankTexture);
    //九宫格灰色背景
    auto scale9SpriiteBg = ui::Scale9Sprite::create("gamemtt/rankview/mtt_rewardbg.png", Rect(0,0,128,120), Rect(40,40,48,48));
    scale9SpriiteBg->setPreferredSize(Size(745, 308));
    scale9SpriiteBg->setPosition(frameSz.width/2, 271);
    bgLayer->addChild(scale9SpriiteBg);
    //说明赢得比赛的具体情况
    m_pLabelDes = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0.5,0.5), 745/2, 240, Color4B::WHITE, scale9SpriiteBg, 0);
    m_pLabelDes->setLineBreakWithoutSpace(true);
    m_pLabelDes->setMaxLineWidth(640);
    //3个奖励小背景
    m_pSpritReward[0] = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_rewardlist.png", 135, 125, scale9SpriiteBg, 0);
    m_pSpritReward[1] = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_rewardlist.png", 135+234+5, 125, scale9SpriiteBg, 0);
    m_pSpritReward[2] = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_rewardlist.png", 135+234*2+10, 125, scale9SpriiteBg, 0);
    //奖励图片及说明
    auto rewardBgSz = m_pSpritReward[0]->getContentSize();
    m_pSpriteReward = HttpSprite::create("sigin_default.png");
    auto httpSpriteSz = m_pSpriteReward->getContentSize();
    m_pSpriteReward->setPosition((rewardBgSz.width-httpSpriteSz.width)/2,  (rewardBgSz.height-httpSpriteSz.height)/2);
    m_pSpritReward[0]->addChild(m_pSpriteReward);
    auto spriteGold = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_reward.png", rewardBgSz.width/2, rewardBgSz.height/2, m_pSpritReward[1], 0);
    auto spriteScore = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_dsfbig.png", rewardBgSz.width/2, rewardBgSz.height/2, m_pSpritReward[2], 0);
    m_pLabelReward[0] = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0.5,0.5), rewardBgSz.width/2, -35, Color4B(0xff,0xd2,0x00,0xff), m_pSpritReward[0], 0);
    m_pLabelReward[1] = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0.5,0.5), rewardBgSz.width/2, -35, Color4B(0xff,0xd2,0x00,0xff), m_pSpritReward[1], 0);
    m_pLabelReward[2] = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0.5,0.5), rewardBgSz.width/2, -35, Color4B(0xff,0xd2,0x00,0xff), m_pSpritReward[2], 0);
    for (int i=0; i<3; i++)
    {
        m_pLabelReward[i]->setLineBreakWithoutSpace(true);
        m_pLabelReward[i]->setMaxLineWidth(rewardBgSz.width);
    }
    //操作按钮
    std::string strBt[3] = {"返回大厅", "告诉朋友", "留下旁观"};
    for (int i=0; i<3; i++)
    {
        m_pMenuItem[i] = MenuItemScale9SpriteWithLabel::create(1, Size(290, 80), CC_CALLBACK_1(MTTRankViewFirst::onMenuItemBottomSelect, this));
        auto label = CreateLabelMSYHBD(30, strBt[i]);
        m_pMenuItem[i]->SetLabel(label);
        m_pMenuItem[i]->setAnchorPoint(Vec2(0.5, 0.5));
        m_pMenuItem[i]->setPosition(228+i*340, 60);
    }
    auto menu = Menu::create(m_pMenuItem[0], m_pMenuItem[1], m_pMenuItem[2], nullptr);
    menu->setPosition(Vec2::ZERO);
    bgLayer->addChild(menu, 4);

    m_pSpriteMan = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_pman.png", 195, frameSz.height/2+25, this, 0);
    
    m_pSpriteGirl = CreateNode1AndPosAddTo<Sprite>("gamemtt/rankview/mtt_pgirl.png", 985, frameSz.height/2-80, this, 0);
    m_pSpriteGirl->setScale(0.75);
    return true;
}

void MTTRankViewFirst::updateMTTRankViewFirst(unsigned uRankID, std::string strDes, std::string strIcon, std::string rewardName, unsigned cash, unsigned score)
{
	m_rankTexture->resetTexTure() ;
    bool bShowItem = rewardName==""?false:true;
    if (bShowItem)
    {
        SetHttpSpriteURL(m_pSpriteReward, strIcon);
        SetLabelString(m_pLabelReward[0], rewardName);
        for (int i=0; i<3; i++)
        {
            m_pSpritReward[i]->setVisible(true);
            m_pSpritReward[i]->setPosition(135+i*(234+5), 125);
        }
    }
    else
    {
        m_pSpritReward[0]->setVisible(false);
        for (int i=1; i<3; i++)
        {
            m_pSpritReward[i]->setPosition(252+(i-1)*(234+5), 125);
        }
    }
    char buff[32] = {0};
    m_rankTexture->Appand("ranktexture/share_t1.png");
    m_rankTexture->Appand(2, uRankID);
    m_rankTexture->Appand("ranktexture/share_t2.png");
    SetLabelString(m_pLabelDes, strDes);
    SetLabelString(m_pLabelReward[1], "奖励："+FormatCash(cash));
    sprintf(buff, "大师分：%d", score);
    SetLabelString(m_pLabelReward[2], buff);
}

void MTTRankViewFirst::onMenuItemBottomSelect(cocos2d::Ref*pMenuItem)
{
    if (pMenuItem == m_pMenuItem[0])//返回大厅
    {
        this->getParent()->setVisible(false);
		GameDialog::setVisible(false) ;
		SendClientRequest(ClientRequest::kCallLevelTable) ;
    }
    else if (pMenuItem == m_pMenuItem[1])//告诉朋友
    {

    }
    else if (pMenuItem == m_pMenuItem[2])//留下旁观
    {
        this->getParent()->setVisible(false);
        GameDialog::setVisible(false) ;
        if (g_globaMttGameInfo.m_isGameOver)
        {
            PopTipWithBt1("游戏结束，请返回大厅", "确定", [this](unsigned uBt){
                if (uBt == 0)
                {
                    SendClientRequest(ClientRequest::kCallLevelTable) ;
                }
            });
        }else if (g_globaMttGameInfo.m_hasDismassTable)
        {
			SendClientRequest(ClientRequest::kCallLevelTable) ;
			PopTipWithBt1("比赛已合桌，旁观用户请返回大厅","确定",nullptr) ;
        }



    }

}