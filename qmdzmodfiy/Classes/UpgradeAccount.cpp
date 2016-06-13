#include "UpgradeAccount.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "MenuItemSpriteWithLabel.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include "RegisterView.h"
#include "LineLabel.h"
#include "Tools.h"
#include "Util.h"

USING_NS_CC;

bool UpgradeTips::init()
{
    if (!GameDialog::init("userinfo/bgdialog0.png", true))
    {
        return false;
    }
    //普通提示
    auto tipsSz = this->getContentSize();
    //m_labelTips = CreateLabelMSYH(28, "", cocos2d::TextHAlignment::LEFT);
    m_labelTips = CreateLabelMSYHAndAnchorPosClrAddTo(28, "", Vec2(0, 0.5), 65, 235, Color4B::WHITE, this, 0);
    m_labelTips->setLineBreakWithoutSpace(true);
    m_labelTips->setMaxLineWidth(550);
    //拼接的提示
    m_lineLabelTips = LineLabel::create();
    m_lineLabelTips->ignoreAnchorPointForPosition(false);
    m_lineLabelTips->setPosition(tipsSz.width/2, 235);
    this->addChild(m_lineLabelTips);
    m_labelAddTips = CreateLabelMSYHAndAnchorPosClrAddTo(28, "请重新登录后领取奖励！", Vec2(0.5, 0.5), tipsSz.width/2, 195, Color4B::WHITE, this, 0);
    //操作按钮
    m_labelBtnName = CreateLabelMSYHBD(30, "立即升级", TextHAlignment::CENTER);
    m_menuItem = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(UpgradeTips::OnMenuItemSelect, this));
    m_menuItem->setPosition(tipsSz.width/2, 55);
    m_menuItem->SetLabel(m_labelBtnName);
    auto menu = Menu::create(m_menuItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    //升级账号奖励层
    m_rewardTitle = Sprite::create("vipdredge/title.png");
    m_rewardTitle->setPosition(tipsSz.width/2, tipsSz.height+10);
    this->addChild(m_rewardTitle);
    m_layerReward = Layer::create();
    m_layerReward->setContentSize(Size(tipsSz.width, 200));
    m_layerReward->setPosition(0, 110);
    this->addChild(m_layerReward);
    auto labelTitle = LineLabel::create();
    labelTitle->Appand(28, "fzlt.ttf", "获得了", Color4B::WHITE);
    labelTitle->Appand(28, "fzlt.ttf", "升级账号奖励", Color4B(0xff,0xd2,0x00,0xff));
    labelTitle->setPosition(tipsSz.width/2, 180);
    m_layerReward->addChild(labelTitle);
    for (int i=0; i<3; i++)
    {
        m_spriteRewardBg[i] = CreateNode1AndPosAddTo<Sprite>("login/upgraderewardbg.png", (tipsSz.width/4)*(i+1), 80, m_layerReward, 0);
        auto bgSize = m_spriteRewardBg[i]->getContentSize();
        m_httpSpriteReward[i] = CreateNode1AndPosAddTo<HttpSprite>("login/gj_money-1.png", bgSize.width/2, bgSize.height/2, m_spriteRewardBg[i], 0);
        auto httpSpriteSz = m_httpSpriteReward[i]->getContentSize();
        m_httpSpriteReward[i]->setPosition((bgSize.width-httpSpriteSz.width)/2, (bgSize.height-httpSpriteSz.height)/2);
        m_labelReward[i] = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5,0.5),bgSize.width/2, 20, Color4B(0xff,0xd2,0x00,0xff),m_spriteRewardBg[i], 0);
    }
    m_uTipsType = -1;

    m_uTestIndex = 0;
    g_globalUpgradeAccount.m_strChannelName = "中视账号";
    for (int i=0; i<2; i++)
    {
        AccountBindReward reward;
        reward.m_strDes = "德州币 X 1000";
        reward.m_strIconURL = "static/img/game/texas/ticket/gj_money-1.png";
        g_globalUpgradeAccount.m_vecBindReward.push_back(reward);
    }

    return true;
}

void UpgradeTips::updateTips(unsigned uType)
{
    m_uTipsType = uType;
    m_lineLabelTips->setVisible(false);
    m_labelAddTips->setVisible(false);
    m_labelTips->setVisible(false);
    m_rewardTitle->setVisible(false);
    m_layerReward->setVisible(false);
    if (uType >=0 && uType <=4)//uType:0-4 不同情况下弹出账号升级的提示
    {
        const char* strTips[]= {"德州币不足？升级账号可以获得大量德州币，赶快行动吧！",
            "您当前德州币数量较多，为保证您的账号安全，请立即升级账号！",
            "一个人游戏太寂寞，赶快升级账号与美女帅哥互动吧！",
            "恭喜您充值成功，为更有效的保护您的账号安全，请立即升级账号！",
            "当前账号安全等级过低，建议您立即升级账号，成功后还可获得大量德州币奖励！"}; 
        m_labelTips->setVisible(true);
        SetLabelString(m_labelTips, strTips[uType]);
        SetLabelString(m_labelBtnName, "立即升级");
    }
    else if (uType == 5)//升级账号后游客登录提示用正式账号登录
    {
        char* strTips = "您已经成为正式用户了，请使用绑定的第三方账号登录哦！";
        m_labelTips->setVisible(true);
        SetLabelString(m_labelTips, strTips);
        SetLabelString(m_labelBtnName, "确定");
    }
    else if (uType == 6)//绑定账号成功后的提示
    {
        m_lineLabelTips->resetLable();
        m_lineLabelTips->Appand(28, "fzlt.ttf", "您已成功绑定 ", Color4B(0xff, 0xff, 0xff, 0xff));
        m_lineLabelTips->Appand(28, "fzlt.ttf", g_globalUpgradeAccount.m_strChannelName, Color4B(0xff, 0xd2, 0x00, 0xff));
        m_lineLabelTips->setVisible(true);
        m_labelAddTips->setVisible(true);
        SetLabelString(m_labelAddTips, "请重新登录后领取奖励！");
        SetLabelString(m_labelBtnName, "确定");
    }
    else if (uType == 7)//升级账号奖励
    {
        auto tipsSz = this->getContentSize();
        m_rewardTitle->setVisible(true);
        m_layerReward->setVisible(true);
        m_spriteRewardBg[0]->setVisible(true);
        m_spriteRewardBg[1]->setVisible(true);
        m_spriteRewardBg[2]->setVisible(true);
        unsigned rewardSz = g_globalUpgradeAccount.m_vecBindReward.size();
        if (rewardSz == 1)
        {
            m_spriteRewardBg[0]->setPositionX(tipsSz.width/2);
            m_spriteRewardBg[1]->setVisible(false);
            m_spriteRewardBg[2]->setVisible(false);
        }
        else if (rewardSz == 2)
        {
            m_spriteRewardBg[0]->setPositionX(tipsSz.width*3/8 - 20);
            m_spriteRewardBg[1]->setPositionX(tipsSz.width*5/8 + 20);
            m_spriteRewardBg[2]->setVisible(false);
        }
        else if (rewardSz == 3)
        {
            m_spriteRewardBg[0]->setPositionX(tipsSz.width*2/8 - 20);
            m_spriteRewardBg[1]->setPositionX(tipsSz.width*4/8);
            m_spriteRewardBg[2]->setPositionX(tipsSz.width*6/8 + 20);
        }
        for (int i=0; i<rewardSz; i++)
        {
            SetHttpSpriteURL(m_httpSpriteReward[i], g_globalUpgradeAccount.m_vecBindReward[i].m_strIconURL);
            SetLabelString(m_labelReward[i], g_globalUpgradeAccount.m_vecBindReward[i].m_strDes);
        }
    }
}

void UpgradeTips::OnMenuItemSelect(Ref* pSend)
{
    if (m_uTipsType >= 0 && m_uTipsType <= 4)//不同情况下弹出账号升级的提示->弹出账号升级界面
    {

    }
    else if (m_uTipsType == 5)//升级账号后游客登录提示用正式账号登录->直接隐藏
    {

    }
    else if (m_uTipsType == 6)//绑定账号成功后的提示->直接隐藏
    {

    }
    else if (m_uTipsType == 7)//绑定成功后奖励按钮->直接隐藏
    {

    }
    if (m_uTestIndex <= 7)
    {
        updateTips(m_uTestIndex);
    }
    else 
    {
        m_uTestIndex = 0;
        updateTips(0);
    }
    m_uTestIndex += 1;
}

bool UpgradeAccount::init()
{
    if (!GameDialog::init("vipprivatenormal/bdefinebg.webp", true))
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("res/ui/upgradeaccount.csb");
    Size frameSize = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(frameSize);
    ui::Helper::doLayout(rootNode);
    addChild(rootNode);
    ////账号绑定层  ,0058账号登录层  ,注册账号层
    m_layoutBindAccount = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_UpgradeAccount"));
    m_layout0058Account = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_0058Account"));
    m_layoutRegAccount = static_cast<Layout*>(Tools::getWidgetByName(rootNode, "Panel_RegisterAccount"));
    m_layout0058Account->setVisible(false);
    m_layoutRegAccount->setVisible(false);
    //按钮组合
    const char* strBtnName1[] = {"Button_weixin_bind", "Button_qq_bind", "Button_account_bind", "Button_weibo_bind"};
    for (int i=0; i<4; i++)
    {
        m_btnAccountBind[i] = static_cast<Button*>(Tools::getWidgetByName(m_layoutBindAccount, strBtnName1[i]));
        m_btnAccountBind[i]->addTouchEventListener(CC_CALLBACK_2(UpgradeAccount::menuItemCallback, this));
        m_btnAccountBind[i]->setTag(i+1);
    }

    m_btnQuickReg = static_cast<Button*>(Tools::getWidgetByName(m_layout0058Account, "Button_quick_reg"));
    m_btnQuickReg->addTouchEventListener(CC_CALLBACK_2(UpgradeAccount::menuItemCallback, this));
    m_btnQuickReg->setTag(5);
    m_btnQuickBind = static_cast<Button*>(Tools::getWidgetByName(m_layout0058Account, "Button_quick_bind"));
    m_btnQuickBind->addTouchEventListener(CC_CALLBACK_2(UpgradeAccount::menuItemCallback, this));
    m_btnQuickBind->setTag(6);

    const char* strBtnName2[] = {"Button_read_protocol", "Button_confirm_reg", "Button_btn_back"};
    for (int i=0; i<3; i++)
    {
        m_btnRegAccount[i] = static_cast<Button*>(Tools::getWidgetByName(m_layoutRegAccount, strBtnName2[i]));
        m_btnRegAccount[i]->addTouchEventListener(CC_CALLBACK_2(UpgradeAccount::menuItemCallback, this));
        m_btnRegAccount[i]->setTag(i+7);
    }
    //编辑框组合
    const char* editboxName[] = {"Image_editaccount_bg","Image_editpassword_bg","Image_phonenum_bg","Image_security_bg","Image_password_bg"};
    for (int i=0; i<5; i++)
    {
        if (i < 2)
            m_imgEditboxBg[i] = static_cast<ImageView*>(Tools::getWidgetByName(m_layout0058Account, editboxName[i]));
        else
            m_imgEditboxBg[i] = static_cast<ImageView*>(Tools::getWidgetByName(m_layoutRegAccount, editboxName[i]));
    }
    auto editbgSz = m_imgEditboxBg[0]->getContentSize();
    m_edit0058Account = CreateNode2AndPosAddTo<ui::EditBox>(Size(editbgSz.width-20, editbgSz.height), ui::Scale9Sprite::create("res/ui/bgedit.png"), editbgSz.width/2+10, editbgSz.height/2, m_imgEditboxBg[0], 0);
    m_edit0058Account->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_edit0058Account->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_edit0058Account->setFont("fzlt.ttf", 22);
    m_edit0058Account->setFontColor(Color3B::BLACK);
    m_edit0058Account->setPlaceHolder("请输入6-20位字母或数字");
    m_edit0058Account->setDelegate(this);

    m_edit0058Password = CreateNode2AndPosAddTo<ui::EditBox>(Size(editbgSz.width-20, editbgSz.height), ui::Scale9Sprite::create("res/ui/bgedit.png"), editbgSz.width/2+10, editbgSz.height/2, m_imgEditboxBg[1], 0);
    m_edit0058Password->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_edit0058Password->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_edit0058Password->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    m_edit0058Password->setFont("fzlt.ttf", 22);
    m_edit0058Password->setFontColor(Color3B::BLACK);
    m_edit0058Password->setPlaceHolder("请输入至少6位密码");
    m_edit0058Password->setDelegate(this);

    editbgSz = m_imgEditboxBg[2]->getContentSize();
    m_editRegPhoneNum = CreateNode2AndPosAddTo<ui::EditBox>(editbgSz, ui::Scale9Sprite::create("res/ui/bgedit.png"), editbgSz.width/2, editbgSz.height/2, m_imgEditboxBg[2], 0);
    m_editRegPhoneNum->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editRegPhoneNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editRegPhoneNum->setFont("fzlt.ttf", 22);
    m_editRegPhoneNum->setFontColor(Color3B::BLACK);
    m_editRegPhoneNum->setPlaceHolder("请输入手机号");
    m_editRegPhoneNum->setDelegate(this);

    m_editRegPassword = CreateNode2AndPosAddTo<ui::EditBox>(editbgSz, ui::Scale9Sprite::create("res/ui/bgedit.png"), editbgSz.width/2, editbgSz.height/2, m_imgEditboxBg[4], 0);
    m_editRegPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editRegPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editRegPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
    m_editRegPassword->setFont("fzlt.ttf", 22);
    m_editRegPassword->setFontColor(Color3B::BLACK);
    m_editRegPassword->setPlaceHolder("支持6-24位字符");
    m_editRegPassword->setDelegate(this);

    editbgSz = m_imgEditboxBg[3]->getContentSize();
    m_editSecurityCode = CreateNode2AndPosAddTo<ui::EditBox>(Size(editbgSz.width-65, editbgSz.height), ui::Scale9Sprite::create("res/ui/bgedit.png"), editbgSz.width/2-32, editbgSz.height/2, m_imgEditboxBg[3], 0);
    m_editSecurityCode->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_editSecurityCode->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_editSecurityCode->setFont("fzlt.ttf", 22);
    m_editSecurityCode->setFontColor(Color3B::BLACK);
    m_editSecurityCode->setPlaceHolder("请输入验证码");
    m_editSecurityCode->setDelegate(this);

    m_baseBtn = createButton("res/ui/securitycode.png", true, 60, Color4B(0, 0, 0, 0), Color4B(14, 85, 38, 80));
    m_baseBtn->setPosition(editbgSz.width, editbgSz.height/2);
    m_baseBtn->SetHitCB(std::bind(&UpgradeAccount::getSecurityCodeReg, this));
    m_imgEditboxBg[3]->addChild(m_baseBtn);

    const char* flagName[] = {"Image_phone_right","Image_phone_wrong","Image_code_right","Image_code_wrong"};
    for (int i=0; i<4; i++)
    {
        m_imgInputFlag[i] = static_cast<ui::ImageView*>(Tools::getWidgetByName(m_layoutRegAccount, flagName[i]));
        m_imgInputFlag[i]->setVisible(false);
    }

    m_pLicenceViewDlg = nullptr;

    return true;
}

void UpgradeAccount::getSecurityCodeReg()
{

}

void UpgradeAccount::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    if (editBox == m_edit0058Account)
    {

    }
    else if (editBox == m_edit0058Password)
    {

    }
    else if (editBox == m_editRegPhoneNum)
    {

    }
    else if (editBox == m_editSecurityCode)
    {

    }
    else if (editBox == m_editRegPassword)
    {

    }
}

void UpgradeAccount::menuItemCallback(cocos2d::Ref* pSend, cocos2d::ui::Button::TouchEventType type)
{
    //1:微信绑定 2:qq绑定 3:0058账号绑定 4:微博绑定 5:立即注册 6:立即绑定 7:查看用户协议 8:接受协议并注册 9:注册界面的返回按钮
    if (type == ui::Button::TouchEventType::ENDED)
    {
        auto btnItem = (Button*)pSend;
        unsigned tagIndex = btnItem->getTag();
        this->GetMenuItemClose()->setVisible(true);
        switch(tagIndex)
        {
        case 1:
            {

            }break;
        case 2:
            {

            }break;
        case 3:
            {
                m_layoutBindAccount->setVisible(false);
                m_layout0058Account->setVisible(true);
            }break;
        case 4:
            {

            }break;
        case 5:
            {
                m_layoutBindAccount->setVisible(false);
                m_layout0058Account->setVisible(false);
                m_layoutRegAccount->setVisible(true);
                this->GetMenuItemClose()->setVisible(false);
            }break;
        case 6:
            {

            }break;
        case 7:
            {
                popUseProtocol();
            }break;
        case 8:
            {

            }break;
        case 9:
            {
                m_layoutBindAccount->setVisible(false);
                m_layout0058Account->setVisible(true);
                m_layoutRegAccount->setVisible(false);
            }break;
        }
    }
}

void UpgradeAccount::popUseProtocol()
{
    if (m_pLicenceViewDlg == nullptr)
    {
        m_pLicenceViewDlg = UserProtocol::create();
        m_pLicenceViewDlg->setPosition(0, 0);
        m_pLicenceViewDlg->setVisible(false);
        addChild(m_pLicenceViewDlg);
    }
    m_pLicenceViewDlg->setVisible(true);
}