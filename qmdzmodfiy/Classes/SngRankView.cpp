#include "SngRankView.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "Util.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "GameFrame.h"
#include "RankTexture.h"

USING_NS_CC;

bool SngRankView::init()
{

	if (!GameDialog::init("userinfo/bgdialog0.png",false)) 
	{
		return false ;
	}

	auto sz = this->getContentSize();
	m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(SngRankView::onMenuItemCloseSelected, this));
	m_btMenuItemClose->setPosition(sz.width - 25, sz.height - 38);
	
    //获得名次
    char buff[32] = {0};
    for (int i=0; i<3; i++)
    {
        sprintf(buff, "gamesng/rankview/win%d.png", i);
        m_pWinCup[i] = CreateNode1AndPosAddTo<Sprite>(buff, 212, 243, this, 0);
    }
    m_pRankTexture = RankTexture::create();
    m_pRankTexture->setPosition(435, 273);
    this->addChild(m_pRankTexture);

    m_pFailedExpress = CreateNode1AndPosAddTo<Sprite>("gamesng/rankview/failed.png", 212, 243, this, 0);

    m_pLabelFailed = CreateLabelMSYHAndAnchorPosClrAddTo(28, "你离冠军不遥远，\n调整好状态再来一局吧", Vec2(0,0), 323, 135, Color4B(0x50,0xbd,0x2e,0xff), this, 0);
    m_pLabelFailed->setAlignment(TextHAlignment::LEFT);
    m_pLabelWin[0] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "大师分：", Vec2(0,0), 367, 182, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_pLabelWin[1] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "奖    金：", Vec2(0,0), 367, 125, Color4B(0xff,0xff,0xff,0xff), this, 0);
    m_pLabelReward[0] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0,0), 479, 182, Color4B(0xd2,0xb6,0x09,0xff), this, 0);
    m_pLabelReward[1] = CreateLabelMSYHAndAnchorPosClrAddTo(30, "", Vec2(0,0), 479, 125, Color4B(0xd2,0xb6,0x09,0xff), this, 0);

    std::string strBt[3] = {"留下旁观", "返回大厅", "再战一场"};
    for (int i=0; i<3; i++)
    {
        //m_pMenuItem[i] = MenuItemSpriteWithLabel::create("userinfo/btred.png", 3, CC_CALLBACK_1(SngRankView::onMenuItemBottomSelect, this));
        m_pMenuItem[i] = MenuItemScale9SpriteWithLabel::create(0, Size(203, 70), CC_CALLBACK_1(SngRankView::onMenuItemBottomSelect, this));
        auto label = CreateLabelMSYHBD(30, strBt[i]);
        m_pMenuItem[i]->SetLabel(label);
        m_pMenuItem[i]->setAnchorPoint(Vec2(0.5, 0.5));
        m_pMenuItem[i]->setPosition(222+i*111, 55);
    }

    auto menu = Menu::create(m_pMenuItem[0], m_pMenuItem[1], m_pMenuItem[2],m_btMenuItemClose, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

	return true ;

}

void SngRankView::GetReword()
{
    m_uMaster = 100;
    m_dReword = 10000;
}

void SngRankView::SetAllSpriteVisible(bool bVisible)
{
    m_pFailedExpress->setVisible(bVisible);
    m_pLabelFailed->setVisible(bVisible);
    m_pRankTexture->resetTexTure();
    m_pRankTexture->setVisible(bVisible);
    for (int i=0; i<3; i++)
    {
        m_pWinCup[i]->setVisible(bVisible);
        m_pMenuItem[i]->setVisible(bVisible);
    }
    for (int i=0; i<2; i++)
    {
        m_pLabelWin[i]->setVisible(bVisible);
        m_pLabelReward[i]->setVisible(bVisible);
    }
    //for (int i=0; i<7; i++)
    //{
    //    m_pFailedRank[i]->setVisible(bVisible);
    //}

}
/************************************************************************/
/* 传入排行 自动显示界面
6人桌 前两名有奖励
9人桌 前三名有奖励
/************************************************************************/
void SngRankView::updateSngRankView(unsigned uMaxPlayer, unsigned uRankId,unsigned cash,unsigned score)
{
	//uRankId+=1 ;
    //GetReword();
	m_maxRole = uMaxPlayer ;
	m_curRank = uRankId ;

	m_uMaster = score;
	m_dReword = cash;


    SetAllSpriteVisible(false);
    char buff[32] = {0};
    unsigned uReward = 0;
    if (uMaxPlayer == 6)
    {
        uReward = 2;
    }
    else if (uMaxPlayer == 9)
    {
        uReward =3;
    }

    if (uRankId >= 1 && uRankId <= uReward )
    {
        m_pWinCup[uRankId-1]->setVisible(true);
        m_pRankTexture->setVisible(true);
        m_pRankTexture->Appand("ranktexture/winbg1.png");
        m_pRankTexture->Appand(1, uRankId);
        m_pRankTexture->Appand("ranktexture/winbg2.png");
        for (int i=0; i<2; i++)
        {
            m_pLabelWin[i]->setVisible(true);
            m_pLabelReward[i]->setVisible(true);
        }
        sprintf(buff, "%d", m_uMaster);
        SetLabelString(m_pLabelReward[0], buff);
        SetLabelString(m_pLabelReward[1], FormatCash4(m_dReword));
        m_pMenuItem[1]->setVisible(true);
    }
    else if (uRankId >2 && uRankId <= uMaxPlayer)
    {
        m_pRankTexture->setVisible(true);
        m_pRankTexture->Appand("ranktexture/failedbg1.png");
        m_pRankTexture->Appand(0, uRankId);
        m_pRankTexture->Appand("ranktexture/failedbg2.png");
        m_pFailedExpress->setVisible(true);
        m_pLabelFailed->setVisible(true);
        m_pMenuItem[0]->setVisible(true);
        m_pMenuItem[2]->setVisible(true);
    }
}

void SngRankView::onMenuItemBottomSelect(cocos2d::Ref*pMenuItem)
{
    if (pMenuItem == m_pMenuItem[0])//留下旁观
    {
		doLookUp() ;
    }
    else if (pMenuItem == m_pMenuItem[1])//返回大厅
    {
		GameDialog::setVisible(false) ;

		SendClientRequest(ClientRequest::kCallLevelTable) ;
    }
    else if (pMenuItem == m_pMenuItem[2])//再战一场
    {
		GlobalSngTableInfo curTable =g_globalSngHallInfo.m_curJoinTable;

		g_globalSngTableInfo.m_signupformGame = true ;

		g_pGameFrame->callShowSngSignUp(true,curTable) ;

		//SendClientRequest(ClientRequest::kCallPlayAgain);

		//for (int i=0;i<g_globalSngHallInfo.m_sngTableInfo.size();i++)
		//{
		//	GlobalSngTableInfo tableInfo = g_globalSngHallInfo.m_sngTableInfo.at(i) ;

		//	if (g_globalSngTableInfo.m_tableId == tableInfo.m_tableID)
		//	{
		//		curTable = tableInfo ;
		//	}
		//}	



	/*	bool flag = false;

		int allcash = curTable.m_fee +curTable.m_entryFee ;
		if (g_globalMyData.GetCash()>=allcash)
		{
			flag = true ;
		}
		
		int count = curTable.m_myTicket-1 ;
		if (count>=0)
		{
			flag = true ;
		}

		if (flag)
		{
			SendClientRequest(ClientRequest::kCallPlayAgain);
			setVisible(false) ;
		}
		else
		{
			PopTipWithBt2("您的德州币不足，是否购买进入商城购买德州币！","取消","商城",[](unsigned ubt)
			{
				if(ubt == 1)
				{
					Json::Value param;
					param["idx"] = 1;
					SendClientRequest(ClientRequest::kPopMall, param);
				}
			});
		}*/



    }
}

void SngRankView::onGrabTouchBegan(bool bHitDialog)
{

}

void SngRankView::onMenuItemCloseSelected(cocos2d::Ref *)
{
			//setVisible(false);
	doLookUp() ;

}

void SngRankView::doLookUp()
{
	unsigned uReward = 0;
	if (m_maxRole == 6)
	{
		uReward = 2;
	}
	else if (m_maxRole == 9)
	{
		uReward =3;
	}


	setVisible(false) ;


	if (m_curRank<=uReward || g_globalSngTableInfo.m_isGameOver)
	{
		PopTipWithBt1("游戏结束，请返回大厅", "确定", [this](unsigned uBt){
			if (uBt == 0)
			{
				SendClientRequest(ClientRequest::kCallLevelTable) ;
			}
		});

	}

}

void SngRankView::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
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
			
    }
}