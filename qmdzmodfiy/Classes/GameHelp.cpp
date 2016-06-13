#include "GameHelp.h"
#include "UICommon.h"
#include "Tools.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"

USING_NS_CC;

enum SrcollView	//	设置默认的滚动视图的大小
{
    ScrollW = 694,
    ScrollH = 400
};

bool GameHelp::init()
{
    if (!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }

    auto title = CreateNode1AndPosAddTo<Sprite>("dialog/help_title.png", 470, 568, this, 0);

    auto rootNode = CSLoader::createNode("res/ui/HelpUI.csb");
    ////Size frameSize = Director::getInstance()->getVisibleSize();
    ////rootNode->setContentSize(frameSize);
    ui::Helper::doLayout(rootNode);
    rootNode->setPosition(211, 70);
    addChild(rootNode);

    m_pLayoutHelp[0] = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_wanfajieshao"));
    m_pLayoutHelp[1] = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_caozuojieshao"));
    m_pLayoutHelp[2] = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_paixingjieshao"));
    m_pLayoutHelp[3] = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_gongnengjieshao"));
    m_pLayoutHelp[4] = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_changjianwenti"));

    m_pBtnOpt[0] = static_cast<Button*>(Tools::getWidgetByName(rootNode, "Button_caozuo1"));
    m_pBtnOpt[0]->addTouchEventListener(CC_CALLBACK_2(GameHelp::CaoZuoMenuCallback, this));
    m_pBtnOpt[1] = static_cast<Button*>(Tools::getWidgetByName(rootNode, "Button_caozuo2"));
    m_pBtnOpt[1]->addTouchEventListener(CC_CALLBACK_2(GameHelp::CaoZuoMenuCallback, this));
    m_pImageCaoZuo[0] = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "Image_caozuo1"));
    m_pImageCaoZuo[1] = static_cast<ImageView*>(Tools::getWidgetByName(rootNode, "Image_caozuo2"));

    m_pScrollGongNeng = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollW, ScrollH), 0, 14, m_pLayoutHelp[3], 0);
    m_pScrollGongNeng->setContentSize(Size(ScrollW, ScrollH));
    m_pScrollGongNeng->setDirection(extension::ScrollView::Direction::VERTICAL);

    m_pScrollWenTi = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollW - 40, ScrollH), 20, 14, m_pLayoutHelp[4], 0);
    m_pScrollWenTi->setContentSize(Size(ScrollW - 40, ScrollH));
    m_pScrollWenTi->setDirection(extension::ScrollView::Direction::VERTICAL);

    std::string btnStr[5] = {"玩法介绍", "操作介绍", "牌型介绍", "功能键介绍", "常见问题"};
    for (int i=0; i<5; i++)
    {
        m_pHelpBtn[i] = MenuItemSpriteWithLabel::create("messagelist/messagebutton.png", 3, CC_CALLBACK_1(GameHelp::MenuItemCallback, this, i));
        m_pHelpBtn[i]->setAnchorPoint(Vec2(0,0));
        m_pHelpBtn[i]->setPosition(24, 430 - 68 * i);
        auto helpMenu = Menu::create(m_pHelpBtn[i],nullptr);
        helpMenu->setPosition(Vec2::ZERO);

        auto btnLabel = CreateLabelMSYHBD(26,btnStr[i]);
        m_pHelpBtn[i]->SetLabel(btnLabel);
        m_pHelpBtn[i]->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
        m_pHelpBtn[i]->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

        addChild(helpMenu);
    }

    m_pHelpBtn[0]->setEnabled(false);
    m_pLayoutHelp[1]->setVisible(false);
    m_pLayoutHelp[2]->setVisible(false);
    m_pLayoutHelp[3]->setVisible(false);
    m_pLayoutHelp[4]->setVisible(false);
    LoadWenTi();
    LoadGongNengJian();

    return true;
}

void GameHelp::MenuItemCallback(cocos2d::Ref *pSend, unsigned idx)
{
    for (int i=0; i<5; i++)
    {
        if (idx == i)
        {
            m_pHelpBtn[i]->setEnabled(false);
            m_pLayoutHelp[i]->setVisible(true);
            if (idx == 1)
            {
                m_pBtnOpt[0]->setBright(false);
                m_pBtnOpt[1]->setBright(true);
                m_pImageCaoZuo[0]->setVisible(true);
                m_pImageCaoZuo[1]->setVisible(false);
            }
        }
        else
        {
            m_pHelpBtn[i]->setEnabled(true);
            m_pLayoutHelp[i]->setVisible(false);
        }
    }
    
}

void GameHelp::CaoZuoMenuCallback(cocos2d::Ref *pSend, ui::Button::TouchEventType type)
{
    if (type == ui::Button::TouchEventType::ENDED)
    {
        if (pSend == m_pBtnOpt[0])
        {
            m_pBtnOpt[0]->setBright(false);
            m_pBtnOpt[1]->setBright(true);
            m_pImageCaoZuo[0]->setVisible(true);
            m_pImageCaoZuo[1]->setVisible(false);
        }
        else if (pSend == m_pBtnOpt[1])
        {
            m_pBtnOpt[1]->setBright(false);
            m_pBtnOpt[0]->setBright(true);
            m_pImageCaoZuo[0]->setVisible(false);
            m_pImageCaoZuo[1]->setVisible(true);
        }
    }
}

void GameHelp::LoadGongNengJian()
{
    std::string strPath[7] = {"help_btn1", "help_btn1", "help_btn1", "help_btn1", "help_btn2", "help_btn2", "help_btn2"};
    std::string strBtName[7] = {"弃牌", "跟注", "加注", "All in", "让或弃", "弃牌", "跟任何注"};
    std::string strContent[7] = {"牌不好，就选择弃牌，不参与这次对局", "如果觉得牌还可以，就可以下注",
                                 "如果觉得牌优势明显，就可以下注", "如果对你的牌有信心，就All in（全下）吧",
                                 "首选让牌，如果需要下注则选择弃牌","自动弃牌","自动选择跟任何注"};
    float OneItemH = 46.0;
    float Interval = 24.0;
    float AllItemH = (OneItemH + Interval) * 7 - Interval;
    float fX = 129, fY = AllItemH - OneItemH/2;
    float offSet = (float)ScrollH - AllItemH;
    m_pScrollGongNeng->setContentOffset(Vec2(0, offSet), false);
    for (int i = 0; i<7; i++)
    {
        strPath[i] = "dialog/" + strPath[i] + ".png";
        auto spriteBtImg = CreateNode1AndPosAddTo<Sprite>(strPath[i], fX, fY , m_pScrollGongNeng, 1);
        auto sz = spriteBtImg->getContentSize();
        CreateLabelMSYHAndAnchorPosClrAddTo(26 , strBtName[i], Vec2(0, 0.5), sz.width/2 - 21, sz.height/2, Color4B(0xff,0xff,0xff,0xff), spriteBtImg, 1);
        CreateLabelMSYHAndAnchorPosClrAddTo(21 , strContent[i], Vec2(0, 0.5), sz.width + 15, sz.height/2, Color4B(0x8c,0xfd,0xc6,0xff), spriteBtImg, 1);
        fY = fY - OneItemH - Interval;
    }
    m_pScrollGongNeng->setContentSize(Size(ScrollW, AllItemH));
}

void GameHelp::LoadWenTi()
{
    float fY = 0.0;
    float labelH[8] = {0};
    std::string strQuestion[8];
    float interval[8] = {10,10,10,10,10,10,10,10};
    strQuestion[0] = "一、登录、账户相关";
    strQuestion[1] = "Q：用手机登录与网页登录有啥区别?\nA：您好，手机登录与网页登录游戏没有任何区别，并且手机登录的用户与网页登录的用户数据是相同的，只是二者不能同时登录。\nQ：正在游戏时提示“您的帐号已在其他设备上登录”是怎么回事?\nA：您好，现在全民德州一个帐号只能同时在一个设备上登录，如果出现这种情况请您检查是否是您同时在多台设备上或分别在移动端和网页端都登录了帐号，如果不是请您检查是否有其他人知道您的账户密码登录了帐号。如果您发现账户密码有被盗的可能请及时更换密码。\nQ：账号被盗怎么办?\nA：如果发现账号被盗，请在第一时间修改个人密码，如有必要，请修改其他涉及个人账号安全的信息。";
    strQuestion[2] = "二、德州币(德州币)相关";
    strQuestion[3] = "A：您可以在“奖励”里通过完成一些任务获取免费德州币(德州币)，当然也可以在牌局里通过高超的技术赢取别人手中的德州币(德州币)。您还可以在商城通过购买道具获赠大量德州币(德州币)。\nQ：iPhone版，网页版和Android版德州币(德州币)是否互通?\nA：您好，我们的三个版本数据全是互通的，您可以使用任一平台进行游戏\nQ：为什么对局完后德州币(德州币)数目跟输赢有出入?\nA：您好，系统会在玩家每次对局中收取一定数量的台子费，台子费将直接从您的德州币(德州币)账户里扣除;还有一种情况是当您在桌上输完所有筹码时，系统会自动用您的剩余德州币(德州币)帮您补充筹码。\nQ：为什么我购买不了德州币(德州币)?\nA：请检查您的网络连接是否正常或您的账户余额是否充足。您也可以联系我们的客服。\nQ：如何赠送别人德州币(德州币)?\nA：在牌局中点击欲送的玩家，选择您要赠送的宝石（只有宝石才是德州币），点击赠送即可。\nQ：奖励的德州币(德州币)每天都能领吗?\nA：奖励的德州币(德州币)以每日任务形式出现，每天刷新一次，每天都能领。\nQ：德州币(德州币)能换成乐币(秀场货币)吗?\nA：您好，不能。但乐币可兑换成德州币";
    strQuestion[4] = "三、游戏相关";
    strQuestion[5] = "Q：为什么我老输，你们系统有配牌吗?\nA：您好，发牌是完全随机的。\nQ：等级如何提升?\nA：您在不同牌局中参与一次将获得不同经验，胜负经验不同，经验足够多时将升级\nQ：经常输，能教教我德州扑克怎么玩吗?\nA：不限注德州扑克玩的就是斗智斗勇，德州扑克游戏规则就是高手的天下。在这里给在不限注德州扑克里的玩家一些建议，希望能对您有多帮助。\n德州扑克玩太多手起手牌：您应该有20-30%的查看翻牌比率。就是说，在第一位置，手中持有AJ时弃牌、中间位置持有KJ时弃牌，靠后位置持有QJ时弃牌。\n德州扑克牌桌的选择：只玩您占有优势的游戏。在牌桌前坐下来时，您至少需要有两三个实力较弱的玩家。\n德州扑克中“与玩家斗智斗勇”：一定要快速分析对手情况，谁玩次牌、谁在遇到进攻时弃牌、谁在拿牌时下注、谁在手中持有弱牌和拿牌时跟大注、虚张声势会对谁起作用、谁会虚张声势等。\n德州扑克中“押注还是弃牌”：弃牌或下注/加注(在您占优势的情况下)。如果没有好的理由(如为了套住对手)，您应该避免跟注。\n德州扑克中认真对待大部分大赌注和加注：特别是在转牌和河牌时尤应如此，因为大部分玩家不会虚张声势。\n不限注德州扑克中的常见错误\n德州扑克在失利时抓住好牌不放手，因此在一手牌上输掉全部筹码。\n面对对方下注时，德州扑克玩家手持弱牌跟注。\n德州扑克玩家玩太多的起手牌。\n德州扑克玩家手中持有最棒的一手牌时，在翻牌前未加注(给持有潜力牌、第一个下注的玩家施加压力)，而在翻牌后随他们走得太远。\n相对底池赌金下注过多/不足(德州扑克为赢取少量赌金冒险过多/未保护手中的牌)。\nQ：游戏过程中我All in后掉线，最后会结算吗?\nA：您好，您掉线后系统会自动帮您“让或弃”。如果您已经All in，会自动进入最后的结算按照正常逻辑进行比牌，如果您的牌最大会赢得底池，德州币(德州币)会自动进入到您的账户里。\nQ：为什么我的牌比其他玩家的大还输了?\nA：您可能有时候会看到，自己的牌是最大的，但是赢取的德州币(德州币)却没有其他用户多：这是因为德州里会根据您投入的德州币(德州币)将底池分成几个池。池底分配方法：由于玩家筹码不同，池底可能由主池和边池构成，有时候有多个边池。主池由allin全下的玩家和其他玩家下注组成，边池由超出allin玩家下注的筹码部分组成。 比牌之后开始分配池底，最大成牌的玩家赢取主池，如果玩家下注的筹码参与了边池，就同时赢取边池，如果没有参与边池，边池由剩余玩家再次比牌分配。如果玩家之间存在成牌大小相等，则平分池底(不能平分的单数筹码由前面的牌手获得)。\nQ：A2345是顺子吗?\nA：A2345是最小的顺子。\nQ：正在玩的时候掉线，损失了XXX，能补偿吗?\nA：您好，当您正在游戏时掉线，系统会帮你自动做“让或弃”的动作，即如果有玩家加注会帮您弃牌。请您在一个良好的网络情况下游戏。\nQ：德州币(德州币)多了还能去低分区玩吗?\nA：您好，可以的，您的德州币多了就不要去低分区蹂躏低手了：)。";
    strQuestion[6] = "四、其他问题";
    strQuestion[7] = "Q：为什么我玩游戏非常卡?\nA：如果游戏过程中响应过慢，请查看网络连接是否正常或者检查手机内存是否占用过高。\nQ：如何关掉声音?\nA：在设置里可以关掉声音。\nQ：如果换帐号登录?\nA：在设置里注销帐号重新登录。";
    for (int i=0; i<8; i++)
    {
        int fontSz = 0;
        if (i%2 == 0)
        {
            fontSz = 19;
        }
        else
        {
            fontSz = 18;
        }
        m_LabelWenti[i] = CreateLabelMSYHAndAnchorPosClrAddTo(fontSz, strQuestion[i], Vec2(0, 0.5), 0, 0, Color4B(0x79,0xd5,0x96,0xff), m_pScrollWenTi, 0);
        m_LabelWenti[i]->setHorizontalAlignment(TextHAlignment::LEFT);
        m_LabelWenti[i]->setLineHeight(30);
        m_LabelWenti[i]->setWidth(ScrollW - 40);
        m_LabelWenti[i]->setLineBreakWithoutSpace(true);
        Size sz = m_LabelWenti[i]->getContentSize();
        labelH[i] = sz.height;
        fY = fY + sz.height + interval[i];
    }

    float uHeight = fY;
    float AddLabelH = 0;
    float offset = ScrollH - uHeight;
    m_pScrollWenTi->setContentOffset(Vec2(0, offset), false);
    for (int j=0; j<8; j++)
    {
        m_LabelWenti[j]->setPosition(0, uHeight - labelH[j]/2 - AddLabelH);
        AddLabelH = AddLabelH + labelH[j] + interval[j];
    }
    m_pScrollWenTi->setContentSize(Size(ScrollW - 40, uHeight));
}