#include "PopGuideDialog.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "GlobalUserData.h"
#include "GameData.h"
#include "cocos-ext.h"
#include "Util.h"
#include "GameTipsManager.h"
#include "TrackManager.h"
USING_NS_CC_EXT;
USING_NS_CC;

NewerTrain* NewerTrain::create(unsigned index)//0:新手特训  1:开始对局
{
    auto pRet = new NewerTrain;
    if (pRet->init(index))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

bool NewerTrain::init(unsigned index)
{
    if (!GameLayer::init("userinfo/bgdialog0.png", true, true))
    {
        return false;
    }
    auto sz = this->getContentSize();
    std::string strContent = "       全民德州的玩法源于风靡全球的德州扑克。玩法简单，易学难精，不仅是技术的博弈，更是心理战术的较量。惊喜、刺激尽在其中！";
    if (index == 0)
    {
        auto label = CreateLabelMSYHAndAnchorPosClrAddTo(30, strContent, Vec2(0, 1), 50, 315, Color4B::WHITE, this, 0);
        label->setAlignment(TextHAlignment::LEFT);
        label->setMaxLineWidth(572);
        label->setLineBreakWithoutSpace(true);
    }
    else
    {
        strContent = "恭喜您完成了新手特训，对基本规则已经了解。德州扑克只有四个操作：";
        auto label1 = CreateLabelMSYHAndAnchorPosClrAddTo(30, strContent, Vec2(0, 1), 68, 295, Color4B::WHITE, this, 0);
        label1->setAlignment(TextHAlignment::LEFT);
        label1->setMaxLineWidth(550);
        label1->setLineBreakWithoutSpace(true);
        strContent = "                                                    让牌、跟注、加注和弃牌。";
        auto label2 = CreateLabelMSYHAndAnchorPosClrAddTo(30, strContent, Vec2(0, 1), 68, 260, Color4B(0xff,0xb2,0x15,0xff), this, 0);
        label2->setAlignment(TextHAlignment::LEFT);
        label2->setMaxLineWidth(550);
        label2->setLineBreakWithoutSpace(true);
    }

    //m_BtnTrain = MenuItemSpriteWithLabel::create("vipprivatenormal/enter.png", 3, CC_CALLBACK_1(NewerTrain::MenuBtnSelect, this));
    m_BtnTrain = MenuItemScale9SpriteWithLabel::create(0, Size(248, 70), CC_CALLBACK_1(NewerTrain::MenuBtnSelect, this));
    m_BtnTrain->setPosition(sz.width/2, 60);
    auto btSz = m_BtnTrain->getContentSize();
    std::string strBtn[2] = {"30秒新手特训", "开始对局"};
    CreateLabelMSYHBDAndAnchorPosClrAddTo(30, strBtn[index], Vec2(0.5, 0.5), btSz.width/2, btSz.height/2, Color4B::WHITE, m_BtnTrain, 0);
    auto menu = Menu::create(m_BtnTrain, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    auto labelSkipTrain = Label::createWithTTF("", "fzlt.ttf", 24);
    labelSkipTrain->setTextColor(Color4B(0xff,0xd2,0x00,0xff));
    std::string strLabel[2] = {"跳过教学>>","再来一遍>>"};
    SetLabelString(labelSkipTrain, strLabel[index]);
    auto menuSkipTrain = MenuItemLabel::create(labelSkipTrain, CC_CALLBACK_1(NewerTrain::MenuLabelSelect, this));
    menuSkipTrain->setPosition(550, 60);
    menu = Menu::create(menuSkipTrain, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    m_index = index;
    return true;
}

void NewerTrain::MenuBtnSelect(Ref *pSend)
{
    //0:新手特训  1:开始对局
    this->setVisible(false);
    if (m_index == 0)
    {
        TrackMgr->event("NewbieGuide", "开始特训");
        SendClientRequest(ClientRequest::kNewbieGuidLayer);
        //funNewerTrainBtn();
    }
    else if (m_index == 1)
    {
        TrackMgr->event("NewbieGuide", "完成特训");
        funStartGameBtn();
    }
}

void NewerTrain::setVisible(bool bVisible)
{
	GameLayer::setVisible(bVisible) ;
	if (bVisible)
	{
		this->setScale(0.7f) ;
		ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
		this->runAction(easeBounceOut);
	}

}


void NewerTrain::MenuLabelSelect(Ref *pSend)
{
    //0:跳过教学  1:再来一遍
    this->setVisible(false);
    if (m_index == 0)
    {
        TrackMgr->event("NewbieGuide", "跳过教学");
        SendClientRequest(ClientRequest::kJumpOverNewerGuide);
        if (g_globalMissionInfo.GetMissions(1)[0].strMissionIcon == "texas_guide" && g_globalMissionInfo.GetMissions(1)[0].uMissonStatus == 0)
        {
            Json::Value param;
            param["task_id"] = g_globalMissionInfo.GetMissions(1)[0].uMissionID;
            SendClientRequest(ClientRequest::kUpdateMission, param);
            Json::Value param1;
            param1["missionDrawID"] = g_globalMissionInfo.GetMissions(1)[0].uMissionDrawID;
            param1["rewardflag"] = 1;
            SendClientRequest(ClientRequest::kDrawAward, param1);
        }
		GameTipsManager::getInstance()->hideTips() ;
    }
    else if (m_index == 1)
    {
        TrackMgr->event("NewbieGuide", "再来一遍");
        funStartGameLabel();
    }
}

void NewerTrain::SetFunCallBack(std::function<void()> func1, std::function<void()> func2, std::function<void()> func3, std::function<void()> func4)
{
    funNewerTrainBtn = func1;
    funNewerTrainLabel = func2;
    funStartGameBtn = func3;
    funStartGameLabel = func4;
}

bool TrainTips::init()
{
    if (!GameLayer::init(376, 227, false, true))
    {
        return false;
    }

    auto tipsBg = Sprite::create("newbieguide/tipsbg.png");
    this->addChild(tipsBg);

    m_labelTopText = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0, 1), 25, 200, Color4B::WHITE, tipsBg, 0);
    m_labelTopText->setAlignment(TextHAlignment::LEFT);
    m_labelTopText->setMaxLineWidth(330);
    m_labelTopText->setLineBreakWithoutSpace(true);
    m_labelBotomText = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0, 1), 25, 125, Color4B(0xff,0xb2,0x15,0xff), tipsBg, 0);
    m_labelBotomText->setAlignment(TextHAlignment::LEFT);
    m_labelBotomText->setMaxLineWidth(330);
    m_labelBotomText->setLineBreakWithoutSpace(true);
    m_labelTips = CreateLabelMSYHAndAnchorPosClrAddTo(24, "点击屏幕继续", Vec2(0, 1), 202, 40, Color4B(0xff,0xb2,0x15,0xff), tipsBg, 0);
    m_labelTips->setVisible(false);

    for (int i=0; i<3; i++)
    {
        //m_btnCardType[i] = MenuItemSpriteWithLabel::create("newbieguide/btn.png", 2, CC_CALLBACK_1(TrainTips::OnMenuSelectCardType, this));
        m_btnCardType[i] = MenuItemScale9SpriteWithLabel::create(0, Size(104, 54), CC_CALLBACK_1(TrainTips::OnMenuSelectCardType, this));
        m_btnCardType[i]->setPosition(71+i*118, 41);
        auto btSz = m_btnCardType[i]->getContentSize();
        m_btnCardType[i]->setVisible(false);
        m_labelBtn[i] = CreateLabelMSYHBDAndAnchorPosClrAddTo(26, "", Vec2(0.5, 0.5), btSz.width/2, btSz.height/2, Color4B::WHITE, m_btnCardType[i], 0);
    }

    //m_btnDrawWard = MenuItemSpriteWithLabel::create("privilegesrewards/btreward.png", 3, CC_CALLBACK_1(TrainTips::OnMenuSelectDrawWard, this));
    m_btnDrawWard = MenuItemScale9SpriteWithLabel::create(0, Size(212, 66), CC_CALLBACK_1(TrainTips::OnMenuSelectDrawWard, this));
    m_btnDrawWard->setPosition(-30, 50);
    auto btSz = m_btnDrawWard->getContentSize();
    CreateLabelMSYHBDAndAnchorPosClrAddTo(26, "领取200德州币", Vec2(0.5, 0.5), btSz.width/2, btSz.height/2, Color4B::WHITE, m_btnDrawWard, 0);
    m_btnDrawWard->setVisible(false);
    auto menu = Menu::create(m_btnCardType[0], m_btnCardType[1], m_btnCardType[2], m_btnDrawWard, nullptr);
    menu->setPosition(Vec2::ZERO);
    tipsBg->addChild(menu);

    m_index = 1;
    return true;
}

void TrainTips::OnMenuSelectCardType(Ref *pSend)
{
    if (m_index == 3)
    {
        if (pSend == m_btnCardType[0])
            SetLabelString(m_labelBotomText, "同花是指5张牌一个花色，你的牌型不是哦");
        else if (pSend == m_btnCardType[1])
            SetLabelString(m_labelBotomText, "顺子是指5张牌点数相连，比如56789");
        else
        {
            SetLabelString(m_labelTopText, "恭喜您答对了，您的牌型是对子（一对A）");
            SetLabelString(m_labelBotomText, "");
            m_labelTips->setVisible(true);
            for (int i=0; i<3; i ++)
                m_btnCardType[i]->setVisible(false);
            funDuiZi();
        }
    }
    else if (m_index == 6)
    {
        if (pSend == m_btnCardType[0])
            SetLabelString(m_labelBotomText, "同花是指5张牌一个花色，你的牌型不是哦");
        else if (pSend == m_btnCardType[2])
            SetLabelString(m_labelBotomText, "三条是指最大牌型里有三张同样点数的牌");
        else
        {
            SetLabelString(m_labelTopText, "恭喜您答对了，您的牌型是两对（对A+对J）");
            SetLabelString(m_labelBotomText, "");
            m_labelTips->setVisible(true);
            for (int i=0; i<3; i ++)
                m_btnCardType[i]->setVisible(false);
            funLiangDui();
        }
    }
    else if (m_index == 8)
    {
        if (pSend == m_btnCardType[0])
            SetLabelString(m_labelBotomText, "同花顺是指有5张牌花色一样点数相连的牌");
        else if (pSend == m_btnCardType[2])
            SetLabelString(m_labelBotomText, "三条是指最大牌型里有三张同样点数的牌");
        else
        {
            SetLabelString(m_labelTopText, "恭喜您答对了，您的牌型是葫芦（三条J+一对A）");
            SetLabelString(m_labelBotomText, "");
            m_labelTips->setVisible(true);
            for (int i=0; i<3; i ++)
                m_btnCardType[i]->setVisible(false);
            funHuLu();
        }
    }
}

void TrainTips::OnMenuSelectDrawWard(Ref *pSend)
{

}

void TrainTips::UpdateTipsData(unsigned index)
{
    m_index = index;
    if (index > 11)
        return;
    m_labelTips->setVisible(false);
    const char *m_strTips[] = {
        "先了解下牌桌内的主要元素。游戏每一局都会给每个玩家发两张“手牌”，“手牌”只有玩家自己能看见。桌上还会发五张“公共牌”，“公共牌”所有玩家可见",
        "牌型大小从大到小如左图所示。您的牌型是由“手牌”+“公共牌”的任意三张组成的最大牌型。您现在的牌型是葫芦",
        "现在轮到您操作了，这个牌跟注是一个不错的选择",
        "您现在的牌型是？请点击下方的三个选项。",
        "这里请您选择操作，想想我们应该怎么做。",
        "当您或其他玩家弃牌后，您已下注的筹码不会退回，您也退出了本轮牌局。",
        "第四张公牌已发，您的牌型又发生了变化，您现在的牌型是？",
        "现在轮到您操作了，您的牌力还不错，可以加注也能跟注，这里我们先跟注看看吧。",
        "最后一张公牌已发，您的牌型已经确定，您现在的牌型是？",
        "轮到您操作了，葫芦已经是很大牌型了，您应该可以取得胜利，All in吧，赢光对手所有的筹码。",
        "最后一轮下注完，剩余玩家将亮牌比大小，这时可看见Tina的手牌，没有您的葫芦大，您赢了！",
        "最后，来了解下牌局中的其他功能吧，筹码不够了可随时增加，聊天、互动、视频让您的牌局不再寂寞！"
    };
    if (index == 0 || index == 1 || index == 5 || index == 10 || index == 11)
    {
        m_labelTips->setVisible(true);
    }
    SetLabelString(m_labelTopText, m_strTips[index]);
    SetLabelString(m_labelBotomText, "");
    if (index == 3 || index == 6 || index == 8)
    {
        for (int i=0; i<3; i ++)
            m_btnCardType[i]->setVisible(true);
        if (index == 3)
        {
            SetLabelString(m_labelBtn[0], "同花");
            SetLabelString(m_labelBtn[1], "顺子");
            SetLabelString(m_labelBtn[2], "对子");
        }
        else if (index == 6)
        {
            SetLabelString(m_labelBtn[0], "同花");
            SetLabelString(m_labelBtn[1], "两对");
            SetLabelString(m_labelBtn[2], "三条");
        }
        else if (index == 8)
        {
            SetLabelString(m_labelBtn[0], "同花顺");
            SetLabelString(m_labelBtn[1], "葫芦");
            SetLabelString(m_labelBtn[2], "三条");
        }
    }
    else
    {
        for (int i=0; i<3; i ++)
            m_btnCardType[i]->setVisible(false);
    }
}

void TrainTips::UpdateBottomTips(std::string strContent)
{
    SetLabelString(m_labelBotomText, strContent);
}

void TrainTips::SetFunCallBack(std::function<void()> func1, std::function<void()> func2, std::function<void()> func3)
{
    funDuiZi = func1;
    funLiangDui = func2;
    funHuLu = func3;
}
