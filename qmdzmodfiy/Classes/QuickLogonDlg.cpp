#include "QuickLogonDlg.h"
#include "MenuItemSpriteWithLabel.h"
#include "LogonLayerEx.h"
#include "GameCommon.h"
#include "PlatformSpec.h"
#include "Tools.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool QuickLogonDlg::initQuickDlg(LogonLayerEx* llEx)
{
    m_pMenuItemLoginByGuest = static_cast<Button*>(Tools::getWidgetByName(llEx,"bt_youkeLogin"));
    m_pMenuItemLoginByGuest->addTouchEventListener(CC_CALLBACK_2(QuickLogonDlg::menuClickQuickGame,this));
    m_pMenuItemLoginByGuest->setTag(0);

    m_pMenuItemLoginByWeiXin = static_cast<Button*>(Tools::getWidgetByName(this,"bt_weixinLogin"));
    m_pMenuItemLoginByWeiXin->addTouchEventListener(CC_CALLBACK_2(QuickLogonDlg::menuItemClick,this));
    m_pMenuItemLoginByWeiXin->setTag(1);

    m_pMenuItemLoginByQQ =  static_cast<Button*>(Tools::getWidgetByName(this,"bt_qqLogin"));
    m_pMenuItemLoginByQQ->addTouchEventListener(CC_CALLBACK_2(QuickLogonDlg::menuItemClick,this));
    m_pMenuItemLoginByQQ->setTag(2);

    m_pMenuItemLoginByAccount = static_cast<Button*>(Tools::getWidgetByName(this,"bt_zhanghaoLogin"));
    m_pMenuItemLoginByAccount->addTouchEventListener(CC_CALLBACK_2(QuickLogonDlg::menuItemClick,this));
    m_pMenuItemLoginByAccount->setTag(3);

    m_pMenuItemLoginByWeiBo = static_cast<Button*>(Tools::getWidgetByName(this,"bt_weiboLogin"));
    m_pMenuItemLoginByWeiBo->addTouchEventListener(CC_CALLBACK_2(QuickLogonDlg::menuItemClick,this));
    m_pMenuItemLoginByWeiBo->setTag(4);

    m_parent = llEx;

    return true;
}

//快速游戏
void QuickLogonDlg::menuClickQuickGame(cocos2d::Ref* pSender, cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        if (checkNetworkState())
        {
            SendClientRequest(ClientRequest::kGuestLogon);
            m_parent->ShowLoading();
        }
        else
        {
            PopTipWithBt1(NETWORK_DISCONNECT_STRING, "确定");
        }
    }
}

//登陆回调
void QuickLogonDlg::menuItemClick(cocos2d::Ref* pMenuItem, cocos2d::ui::Button::TouchEventType type)
{
    if (type == TouchEventType::ENDED)
    {
        auto item = (Button*)pMenuItem;
        if (checkNetworkState())
        {
            if(item->getTag() == 3)
            {
                m_parent->ShowAccountLogon();
            }
            else if(item->getTag() == 1)
            {
                Json::Value param;
                param["3rd"] = 1;
                SendClientRequest(ClientRequest::kLogonBy3rd, param);
            }
            else if(item->getTag() == 2)
            {
                Json::Value param;
                param["3rd"] = 0;
                SendClientRequest(ClientRequest::kLogonBy3rd, param);
            }
            else if(item->getTag() == 4)
            {
                Json::Value param;
                param["3rd"] = 2;
                SendClientRequest(ClientRequest::kLogonBy3rd, param);
				//PopTipWithBt1("微博登录功能暂未开放，敬请期待中~","确定");
				
            }			
        }
        else
        {
            PopTipWithBt1(NETWORK_DISCONNECT_STRING, "确定");
        }
    }
}