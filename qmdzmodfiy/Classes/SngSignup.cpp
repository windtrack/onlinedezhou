#include "SngSignup.h"
#include "GlobalUserData.h"
#include "Util.h"
#include "Tools.h"
#include "GameFrame.h"
#include "GameData.h"
bool SngSignUp::init()
{
    if (!GameDialog::init("vipprivatenormal/bdefinebg.webp", true))
    {
        return false;
    }



	Size sz = this->getContentSize() ;
	Node* rootNode = CSLoader::createNode("res/ui/signup.csb");
	rootNode->ignoreAnchorPointForPosition(false);
	rootNode->setAnchorPoint(Point(0.5,0.5));
	rootNode->setPosition(sz.width/2 + 19,sz.height/2 + 19) ;

	//rootNode->setScale(0.5);
	this->addChild(rootNode,1);


	/*LayerColor  * bgLayer = LayerColor::create(Color4B(0x00, 0x00, 0x00, 0xaf), GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()) ;
	bgLayer->setAnchorPoint(Point(0.5,0.5)) ;
	bgLayer->ignoreAnchorPointForPosition(false) ;
	bgLayer->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(bgLayer,-1);*/

    m_pLayoutBg = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_signup_BG"));
    m_pSignup6p = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_signup_6p"));
    m_pSignup9p = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_signup_9p"));
    m_pNoTicket = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_no_ticket"));
    m_pHasTicket = static_cast<ui::Layout*>(Tools::getWidgetByName(rootNode, "Panel_has_ticket"));
    //按钮
    m_pBtnSignup = static_cast<ui::Button*>(Tools::getWidgetByName(m_pLayoutBg, "Button_signup"));
    m_pBtnCancel = static_cast<ui::Button*>(Tools::getWidgetByName(m_pLayoutBg, "Button_cancel"));
    m_pBtnSignup->addTouchEventListener(CC_CALLBACK_2(SngSignUp::OnMenuClick, this));
    m_pBtnCancel->addTouchEventListener(CC_CALLBACK_2(SngSignUp::OnMenuClick, this));
    //文本
    m_pTextAward6p[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pSignup6p, "Text_rank1_awardPoint"));
    m_pTextAward6p[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pSignup6p, "Text_rank2_awardPoint"));
    m_pTextMaster6p[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pSignup6p, "Text_rank1_masterPoint"));
    m_pTextMaster6p[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pSignup6p, "Text_rank2_masterPoint"));

    char buff[64] = {0};
    for (int i=0; i<3; i++)
    {
        sprintf(buff, "Text_rank%d_awardPoint", i+1);
        m_pTextAward9p[i] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pSignup9p, buff));
        sprintf(buff, "Text_rank%d_masterPoint", i+1);
        m_pTextMaster9p[i] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pSignup9p, buff));
    }

    m_pTextEntryFee[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pNoTicket, "Text_entry_fees_point"));
    m_pTextEntryFee[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pNoTicket, "Text_service_fees_point"));
    m_pTextEntryFee[2] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pNoTicket, "Text_totalgold_point"));
    m_pTextEntryTicket[0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pHasTicket, "Text_scene_field"));
    m_pTextEntryTicket[1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pHasTicket, "Text_need_ticket"));
    m_pTextEntryTicket[2] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pHasTicket, "Text_total_ticketNum"));

    return true;
}

void SngSignUp::UpdateView(GlobalSngTableInfo date)
{
	curTableInfo = date ;

    m_pLayoutBg->setVisible(true);
	m_tableId = date.m_tableID ;
    if (date.m_rewardCount == 2)
    {
        m_pSignup6p->setVisible(true);
        m_pSignup9p->setVisible(false);
        char buff[64] = {0};
        for (int i=0; i<2; i++)
        {
            setUITextString(m_pTextAward6p[i], FormatCash4(date.m_reward[i]));
            sprintf(buff, "%d", date.m_score[i]);
            setUITextString(m_pTextMaster6p[i], buff);
        }
        
        if (date.m_myTicket >= date.m_freeTicket && date.m_myTicket > 0)
        {
            m_pNoTicket->setVisible(false);
            m_pHasTicket->setVisible(true);

            setUITextString(m_pTextEntryTicket[0], date.m_tableName.c_str());
            sprintf(buff, "%d张", date.m_freeTicket);
            setUITextString(m_pTextEntryTicket[1], buff);
            sprintf(buff, "%d张", date.m_myTicket);
            setUITextString(m_pTextEntryTicket[2], buff);
        }
        else
        {
            m_pNoTicket->setVisible(true);
            m_pHasTicket->setVisible(false);

            setUITextString(m_pTextEntryFee[0], FormatCash4(date.m_entryFee));
            setUITextString(m_pTextEntryFee[1], FormatCash4(date.m_fee));
            setUITextString(m_pTextEntryFee[2], FormatCash4(g_globalMyData.GetCash()));
        }
    }
    else if (date.m_rewardCount == 3)
    {
        m_pSignup6p->setVisible(false);
        m_pSignup9p->setVisible(true);
        char buff[64] = {0};
        for (int i=0; i<3; i++)
        {
            setUITextString(m_pTextAward9p[i], FormatCash4(date.m_reward[i]));
            sprintf(buff, "%d", date.m_score[i]);
            setUITextString(m_pTextMaster9p[i], buff);
        }

        if (date.m_myTicket >= date.m_freeTicket && date.m_myTicket > 0)
        {
            m_pNoTicket->setVisible(false);
            m_pHasTicket->setVisible(true);

            setUITextString(m_pTextEntryTicket[0], date.m_tableName.c_str());
            sprintf(buff, "%d张", date.m_freeTicket);
            setUITextString(m_pTextEntryTicket[1], buff);
            sprintf(buff, "%d张", date.m_myTicket);
            setUITextString(m_pTextEntryTicket[2], buff);
        }
        else
        {
            m_pNoTicket->setVisible(true);
            m_pHasTicket->setVisible(false);

            setUITextString(m_pTextEntryFee[0], FormatCash4(date.m_entryFee));
            setUITextString(m_pTextEntryFee[1], FormatCash4(date.m_fee));
            setUITextString(m_pTextEntryFee[2], FormatCash4(g_globalMyData.GetCash()));
        }
    }
}

void SngSignUp::onSetVisible(bool bVisible)
{
    if (bVisible)
    {

    }
    else
    {
		 //SendClientRequest(ClientRequest::kOptSNGHallBgLayer);
		//g_pGameFrame->hideUserItemLayer() ;
    }
}

void SngSignUp::OnMenuClick(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == cocos2d::ui::Button::TouchEventType::ENDED)
    {
        if (pSender == m_pBtnSignup)
        {
			


			bool flag = false;

			int allcash = curTableInfo.m_fee +curTableInfo.m_entryFee ;
			if (g_globalMyData.GetCash()>=allcash)
			{
				flag = true ;
			}

			int count = curTableInfo.m_myTicket-1 ;
			if (count>=0)
			{
				flag = true ;
			}

			if (flag)
			{
	
				if(g_globalSngTableInfo.m_signupformGame)
				{
					SendClientRequest(ClientRequest::kCallLevelTableWithOutLevel);

				}
				g_globalSngTableInfo.m_signupformGame = false ;
				g_globalSngHallInfo.m_curJoinTable = curTableInfo ;

				g_globalSngTableInfo.m_playerCount = curTableInfo.m_maxPlayRoleCount ;
				g_globalSngTableInfo.m_roudIdex=0 ;
				g_globalSngTableInfo.m_mangzhuIndex=0 ;
				g_globalSngTableInfo.m_rank=1 ;
				g_globalSngTableInfo.m_isGameStart = false ;
				g_globalSngTableInfo.m_isSnglose=false ;
				g_globalSngTableInfo.m_tableId =curTableInfo.m_tableID ;
				g_globalSngTableInfo.m_isGameOver = false ;

				char buff[64] ;
				if (curTableInfo.m_index==0)
				{
					sprintf(buff,"菜鸟%d人场",curTableInfo.m_maxPlayRoleCount) ;
				}
				if (curTableInfo.m_index==1)
				{
					sprintf(buff,"普通%d人场",curTableInfo.m_maxPlayRoleCount) ;
				}
				if (curTableInfo.m_index==2)
				{
					sprintf(buff,"精英%d人场",curTableInfo.m_maxPlayRoleCount) ;
				}
				if (curTableInfo.m_index==3)
				{
					sprintf(buff,"卓越%d人场",curTableInfo.m_maxPlayRoleCount) ;
				}
				if (curTableInfo.m_index==4)
				{
					sprintf(buff,"大师%d人场",curTableInfo.m_maxPlayRoleCount) ;
				}
				if (curTableInfo.m_index==5)
				{
					sprintf(buff,"宗师%d人场",curTableInfo.m_maxPlayRoleCount) ;
				}
				g_globalSngTableInfo.m_tableName = buff ;

				g_globalSngTableInfo.m_rewardCount = curTableInfo.m_rewardCount ;

				g_globalSngTableInfo.m_sngReward.clear() ;

				for (int i=0;i<curTableInfo.m_rewardCount;i++)
				{
					SngReward rewardInfo ;
					rewardInfo.m_rankid = i ;
					rewardInfo.m_cashReward = curTableInfo.m_reward[i];
					rewardInfo.m_score = curTableInfo.m_score[i] ;
					g_globalSngTableInfo.m_sngReward.push_back(rewardInfo) ;
				}





				//SendClientRequest(ClientRequest::kCallPlayAgain);
				Json::Value param;
				//param["enterType"] = 0;
				param["id"] = m_tableId;
				SendClientRequest(ClientRequest::kEnterSngTable, param);
				g_pGameFrame->hideUserItemLayer() ;
				setVisible(false) ;







			}
			else
			{
				PopTipWithBt2("您的德州币不足，是否购买进入商城购买德州币！","取消","商城",[](unsigned ubt)
				{
					if(ubt == 1)
					{
						Json::Value param;
						param["idx"] = 0;
						SendClientRequest(ClientRequest::kPopMall, param);
					}
				});
			}











        }
        else if (pSender == m_pBtnCancel)
        {
			setVisible(false) ;
        }
    }
}


void SngSignUp::onGrabTouchBegan(bool bHitDialog)
{

}
void SngSignUp::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
	//GameDialog::setVisible(false) ;
    if (bVisible)
    {
		/*auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
		GameDialog::setVisible(true);
		/*	this->setScale(0.7f) ;
		ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
		this->runAction(easeBounceOut);*/
		Tools::callScaleAction(this) ;
    }
    else
    {
        SendClientRequest(ClientRequest::kOptSNGHallBgLayer);
		callHideAction() ;
		//GameDialog::setVisible(false) ;
		/*       auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));*/
    }
}

void SngSignUp::onMenuItemCloseSelected(cocos2d::Ref *)
{
	setVisible(false) ;
	//GameTipsManager::getInstance()->hideTips() ;
}
