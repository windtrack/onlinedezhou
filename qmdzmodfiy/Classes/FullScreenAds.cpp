#include "FullScreenAds.h"
#include "HttpSprite.h"
#include "MenuItemFrameSprite.h"
#include "GameTipsManager.h"
#include "GameData.h"
#include "Util.h"

USING_NS_CC;

bool FullScreenAds::init()
{
    if (!GameLayer::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight()))
    {
        return false;
    }

    m_httpAdsBg = HttpSprite::create("logobg.png", Size(1136,640));
    m_httpAdsBg->setPosition(0,0);
    this->addChild(m_httpAdsBg);
    /*m_pOptBtn = MenuItemScale9SpriteWithLabel::create(0, Size(234, 74), CC_CALLBACK_1(FullScreenAds::menuCallBack, this));
    m_pOptBtn->setPosition(GameData::getInstance()->getGameWidth()/2, 150);
    m_labelBtnName = CreateLabelMSYHBD(28, "立即参与");
    m_pOptBtn->SetLabel(m_labelBtnName);
    auto menu = Menu::create(m_pOptBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);*/
    auto pMenuItemLeave = MenuItemFrameSprite::create("playback/btback.png", 2, CC_CALLBACK_1(FullScreenAds::menuCallBackClose, this));
    pMenuItemLeave->setPosition(49, GameData::getInstance()->getGameHeight()-40);
    auto menu = Menu::create(pMenuItemLeave, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void FullScreenAds::setVisible(bool bVisible)
{
    GameLayer::setVisible(bVisible) ;
    if (bVisible)
    {
        this->setScale(0.5f) ;
        ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
        CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
        this->runAction(easeBounceOut);
    }
}

void FullScreenAds::updateFullAdsInfo()
{
    SetIconUrl(m_httpAdsBg, "");
}

void FullScreenAds::menuCallBackClose(cocos2d::Ref* pSend)
{
    this->setVisible(false);
    GameTipsManager::getInstance()->hideTips() ;
    removeFromParentAndCleanup(true);
}

void FullScreenAds::menuCallBackDetail(cocos2d::Ref* pSend)
{

}