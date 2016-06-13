#include "CustomerServices.h"
#include "UICommon.h"
#include "GameCommon.h"
#include "Tools.h"
#include "PlatformSpec.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool CustomerServices::init()
{
    if (!GameDialog::init())
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("res/ui/Services.csb");
    Size frameSize = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(frameSize);
    ui::Helper::doLayout(rootNode);
    addChild(rootNode);

    m_btBack = static_cast<ui::Button*>(Tools::getWidgetByName(rootNode, "Button_Back"));
    m_btBack->addTouchEventListener(CC_CALLBACK_2(CustomerServices::OnMenuItemSelect, this));

    m_btOnlineServices = static_cast<ui::Button*>(Tools::getWidgetByName(rootNode, "Button_Online"));
    m_btOnlineServices->addTouchEventListener(CC_CALLBACK_2(CustomerServices::OnMenuItemSelect, this));

    m_btPhoneServices = static_cast<ui::Button*>(Tools::getWidgetByName(rootNode, "Button_Phone"));
    m_btPhoneServices->addTouchEventListener(CC_CALLBACK_2(CustomerServices::OnMenuItemSelect, this));

    m_btVipChannel = static_cast<ui::Button*>(Tools::getWidgetByName(rootNode, "Button_Vip"));
    m_btVipChannel->addTouchEventListener(CC_CALLBACK_2(CustomerServices::OnMenuItemSelect, this));

    return true;
}

void CustomerServices::OnMenuItemSelect(cocos2d::Ref* pSend, cocos2d::ui::Button::TouchEventType type)
{
    if (type == Button::TouchEventType::BEGAN)
    {
        if (pSend == m_btOnlineServices)
        {
            m_btOnlineServices->setScale(1.1);
        }
        else if (pSend == m_btPhoneServices)
        {
            m_btPhoneServices->setScale(1.1);
        }
        else if (pSend == m_btVipChannel)
        {
            m_btVipChannel->setScale(1.1);
        }
    }
    else if (type == Button::TouchEventType::MOVED)
    {
        if (pSend == m_btOnlineServices)
        {
            m_btOnlineServices->setScale(1);
        }
        else if (pSend == m_btPhoneServices)
        {
            m_btPhoneServices->setScale(1);
        }
        else if (pSend == m_btVipChannel)
        {
            m_btVipChannel->setScale(1);
        }
    }
    else if (type == Button::TouchEventType::ENDED)
    {
        if (pSend == m_btBack)
        {
            setVisible(false);
        }
        else if (pSend == m_btOnlineServices)
        {
            m_btOnlineServices->setScale(1);
            PopTipWithBt1("在线客服功能暂未开放，敬请期待ing~", "确定");
        }
        else if (pSend == m_btPhoneServices)
        {
            m_btPhoneServices->setScale(1);
			callCustomService();
        }
        else if (pSend == m_btVipChannel)
        {
            m_btVipChannel->setScale(1);
            PopTipWithBt1("VIP专属通道功能暂未开放，敬请期待ing~", "确定");
        }
    }
}

void CustomerServices::setVisible(bool bVisible)
{
    PlayVisibleSound(bVisible);
    GameDialog::setVisible(bVisible);
}