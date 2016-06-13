#include "PrivilegesRewards.h"
#include "MenuItemSpriteWithLabel.h"
#include "MenuItemScale9SpriteWithLabel.h"
#include "GlobalUserData.h"
#include "GameCommon.h"
#include "HttpSprite.h"
#include "Util.h"
#include "GrabKeyboard.h"
#include "GameData.h"
#include "GameTipsManager.h"
#include "GlobalUserData.h"
USING_NS_CC;

enum 
{
    ScrollViewW = 600,
    ScrollViewH = 220
};

//---------------------------------------------------------------------------------------------------------------
void PrivilegesRewards::onKeyEscPressed()
{
}
//---------------------------------------------------------------------------------------------------------------
bool PrivilegesRewards::init()
{
	if(!GameDialog::init("userinfo/bgdialog0.png", false))
	{
		return false;
	}
	// 创建背景
	//auto pBgSprite = CreateNode1AndPosAddTo<Sprite>("userinfo/bgdialog0.png", GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight()/ 2, this, 1);
	// 顶上的绿色标题
	auto title = CreateNode1AndPosAddTo<Sprite>("privilegesrewards/title.png", 345, 402, this, 0);

	// 领取按钮
	//auto pRewardButton = MenuItemSpriteWithLabel::create("privilegesrewards/btreward.png", 3, CC_CALLBACK_1(PrivilegesRewards::RewardButton, this));
    auto pRewardButton = MenuItemScale9SpriteWithLabel::create(0, Size(212, 66), CC_CALLBACK_1(PrivilegesRewards::RewardButton, this));
	pRewardButton->setAnchorPoint(Vec2(0.5, 0.5));
	pRewardButton->setPosition(345, 66);
	pRewardButton->setVisible(true);
	auto pPrivilegesButton = Menu::create(pRewardButton,nullptr);
	pPrivilegesButton->setPosition(Vec2::ZERO);
	this->addChild(pPrivilegesButton);

	auto pQucikReward = CreateLabelMSYHBD(28,"立即领取");
	pRewardButton->SetLabel(pQucikReward);
	pRewardButton->SetEnabledColor(cocos2d::Color4B::WHITE);
	pRewardButton->SetDisabledColor(cocos2d::Color4B::WHITE);

	// 领取内容
	m_pRewardContent = CreateLabelMSYHAndAnchorPosClrAddTo(28, "",Vec2(0.5, 0.5), 340, 266, Color4B::WHITE, this, 0);
	m_pRewardContent->setLineHeight(25);
	UpdatePrivilegesRewards();

	return true;
}

void PrivilegesRewards::setVisible(bool bVisible)
{
	GameDialog::setVisible(bVisible) ;
	if (bVisible)
	{
        SendClientRequest(ClientRequest::kShowHallBgLayer);
		this->setScale(0.7f) ;
		ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
		CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
		this->runAction(easeBounceOut);
	}
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
    }

}
//---------------------------------------------------------------------------------------------------------------
// 更新特权奖励
void PrivilegesRewards::UpdatePrivilegesRewards()
{
	// 当前总桌数
	char szCurrenContent[100] = {0};
	sprintf(szCurrenContent, "尊敬的%s用户,恭喜获得特权奖励\n\n%s德州币。您的%s还有%d天到期",g_globalMyData.m_strVipName.c_str(),
	FormatCash(g_globalMyData.m_dVipAward).c_str(),g_globalMyData.m_strVipName.c_str(),g_globalMyData.m_uVipValidDay);
	SetLabelString(m_pRewardContent, szCurrenContent);
}

//---------------------------------------------------------------------------------------------------------------
void PrivilegesRewards::RewardButton(Ref *pSender)
{
	GameTipsManager::getInstance()->hideTips() ;
	this->setVisible(false);
	SendClientRequest(ClientRequest::kDrawVipLoginAward);
}

void PrivilegesRewards::onGrabTouchBegan(bool bHitDialog)
{

}



//----------------------------------------------------德女郎----------------------------------------------------
void WaveGirlRewards::onKeyEscPressed()
{
}
//---------------------------------------------------------------------------------------------------------------
bool WaveGirlRewards::init()
{
	if(!GameDialog::init("userinfo/bgdialog0.png", false))
	{
		return false;
	}
	// 创建背景
	//auto pBgSprite = CreateNode1AndPosAddTo<Sprite>("userinfo/bgdialog0.png", GameData::getInstance()->getGameWidth() / 2, GameData::getInstance()->getGameHeight()/ 2, this, 1);
	// 顶上的绿色标题
	auto title = CreateNode1AndPosAddTo<Sprite>("privilegesrewards/wavegirltitle.png", 345, 402, this, 0);

	// 领取按钮
	//auto pRewardButton = MenuItemSpriteWithLabel::create("privilegesrewards/btreward.png", 3, CC_CALLBACK_1(WaveGirlRewards::RewardButton, this));
    auto pRewardButton = MenuItemScale9SpriteWithLabel::create(0, Size(212, 66), CC_CALLBACK_1(WaveGirlRewards::RewardButton, this));
	pRewardButton->setAnchorPoint(Vec2(0.5, 0.5));
	pRewardButton->setPosition(345, 66);
	pRewardButton->setVisible(true);
	auto pPrivilegesButton = Menu::create(pRewardButton,nullptr);
	pPrivilegesButton->setPosition(Vec2::ZERO);
	this->addChild(pPrivilegesButton);

	auto pQucikReward = CreateLabelMSYHBD(28,"立即领取");
	pRewardButton->SetLabel(pQucikReward);
	pRewardButton->SetEnabledColor(cocos2d::Color4B::WHITE);
	pRewardButton->SetDisabledColor(cocos2d::Color4B::WHITE);

	// 领取内容
	m_pRewardContent = CreateLabelMSYHAndAnchorPosClrAddTo(28, "",Vec2(0.5, 0.5), 340, 256, Color4B::WHITE, this, 0);
	m_pRewardContent->setLineHeight(25);
	UpdateWaveGirlRewards();

	return true;
}
//---------------------------------------------------------------------------------------------------------------
// 更新特权奖励
void WaveGirlRewards::UpdateWaveGirlRewards()
{
	// 当前总桌数
	char szCurrenContent[100] = {0};
	sprintf(szCurrenContent, "尊敬的德女郎,恭喜获得奖励%s德州币",do_fraction(g_globalMyData.m_WaveGrilAward).c_str());
	SetLabelString(m_pRewardContent, szCurrenContent);
}

//---------------------------------------------------------------------------------------------------------------
void WaveGirlRewards::RewardButton(Ref *pSender)
{
	this->setVisible(false);
	SendClientRequest(ClientRequest::kDrawWaveGrilLoginAward);
	GameTipsManager::getInstance()->hideTips();
}

void WaveGirlRewards::setVisible(bool bVisible)
{
    GameDialog::setVisible(bVisible) ;
    if (bVisible)
    {
        SendClientRequest(ClientRequest::kShowHallBgLayer);
        this->setScale(0.7f) ;
        ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
        CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
        this->runAction(easeBounceOut);
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
    }

}

void WaveGirlRewards::onGrabTouchBegan(bool bHitDialog)
{

}



//----------------------------------------------------活动奖励----------------------------------------------------
void ActivityRewards::onKeyEscPressed()
{
}
//---------------------------------------------------------------------------------------------------------------
bool ActivityRewards::init()
{
    if(!GameDialog::init("userinfo/bgdialog0.png", false))
    {
        return false;
    }
    auto bgSz = this->getContentSize();
    // 创建背景
    auto title = CreateNode1AndPosAddTo<Sprite>("dialog/reward_title.png", 345, 402, this, 0);

    // 领取按钮
    auto pRewardButton = MenuItemScale9SpriteWithLabel::create(0, Size(212, 66), CC_CALLBACK_1(ActivityRewards::RewardButton, this));
    pRewardButton->setAnchorPoint(Vec2(0.5, 0.5));
    pRewardButton->setPosition(345, 66);
    pRewardButton->setVisible(true);
    auto pPrivilegesButton = Menu::create(pRewardButton,nullptr);
    pPrivilegesButton->setPosition(Vec2::ZERO);
    this->addChild(pPrivilegesButton);

    auto pQucikReward = CreateLabelMSYHBD(28, "确定");
    pRewardButton->SetLabel(pQucikReward);
    pRewardButton->SetEnabledColor(cocos2d::Color4B::WHITE);
    pRewardButton->SetDisabledColor(cocos2d::Color4B::WHITE);

    // 领取内容
    m_pRewardContent = CreateLabelMSYHBDAndAnchorPosClrAddTo(28, "",Vec2(0.5, 0.5), bgSz.width/2, 330, Color4B::WHITE, this, 0);
    m_pRewardContent->setLineHeight(25);

    m_scrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollViewW, ScrollViewH), 0, 0, this, 0);
    m_scrollView->setContentSize(Size(ScrollViewW, ScrollViewH));
    m_scrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_scrollView->ignoreAnchorPointForPosition(false);
    m_scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    m_scrollView->setPosition(bgSz.width/2, bgSz.height/2+10);

    return true;
}
//---------------------------------------------------------------------------------------------------------------
// 更新活动奖励
void ActivityRewards::UpdateActivityRewards()
{
    for (auto item : m_vecRewardItemNew)
    {
        item->m_layerBg->setVisible(false);
        m_vecRewardItemOld.push_back(item);
    }
    m_vecRewardItemNew.clear();

    SetLabelString(m_pRewardContent, g_globalActRewardInfo.m_strTitle);
    unsigned itemSz = g_globalActRewardInfo.m_vecReward.size();

    float fX=0, fY=0;
    float oneItemW = 200;
    float totalItemW = oneItemW*itemSz;
    if (itemSz == 1)//奖励小于三的时候要居中显示
    {
        fX = 200;
    }
    else if (itemSz == 2)
    {
        fX = 100;
    }
    else if (itemSz >2)
    {
        fX = 0;
    }
    if (totalItemW < ScrollViewW)
    {
        totalItemW = ScrollViewW;
        m_scrollView->setBounceable(false);
    }
    else
        m_scrollView->setBounceable(true);
    for (auto rewardInfo : g_globalActRewardInfo.m_vecReward)
    {
        auto item = GetActivityItem(rewardInfo);
        UpdateRewardItem(item, rewardInfo);
        item->m_layerBg->setVisible(true);
        item->m_layerBg->setPosition(fX, fY);
        m_vecRewardItemNew.push_back(item);
        fX += oneItemW;
    }

    m_scrollView->setContentOffset(Vec2(0, 0), false);
    m_scrollView->setContentSize(Size(totalItemW, ScrollViewH));
}

//---------------------------------------------------------------------------------------------------------------
void ActivityRewards::RewardButton(Ref *pSender)
{
    this->setVisible(false);
    //Json::Value rewardArray;
    //for (auto itemInfo : g_globalActRewardInfo.m_vecReward)
    //{
    //    Json::Value param;
    //    param["rewardid"] = itemInfo.m_uActivityID;
    //    rewardArray["rewardarr"].append(param);
    //}
    //SendClientRequest(ClientRequest::kPopActReward, "");
    //GameTipsManager::getInstance()->hideTips();
}

void ActivityRewards::setVisible(bool bVisible)
{
    GameDialog::setVisible(bVisible) ;
    if (bVisible)
    {
        SendClientRequest(ClientRequest::kShowHallBgLayer);
        this->setScale(0.7f) ;
        ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
        CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
        this->runAction(easeBounceOut);
    }
    else
    {
        SendClientRequest(ClientRequest::kOptHallBgLayer);
    }

}

void ActivityRewards::onGrabTouchBegan(bool bHitDialog)
{

}

ActivityRewards::ActivityItem* ActivityRewards::GetActivityItem(ActReward rewardInfo)
{
    ActivityItem* activityItem = nullptr;
    if (m_vecRewardItemOld.empty())
    {
        //奖励背景
        auto layerBg = Layer::create();
        layerBg->setContentSize(Size(200, 220));
        m_scrollView->addChild(layerBg);
        //奖励图片
        auto  httpIconSprite = CreateNode2AndPosAddTo<HttpSprite>("sigin_default.png", Size(200,200), 0, 0, layerBg, 0);
        httpIconSprite->ignoreAnchorPointForPosition(false);
        httpIconSprite->setAnchorPoint(Vec2(0.5, 0.5));
        httpIconSprite->setPosition(100, 110);
        //奖励描述
        auto labelDes = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0.5,0), 100, 10, Color4B::WHITE, layerBg, 0);
        
        auto pItem = new ActivityItem;
        pItem->m_spriteReward = httpIconSprite;
        pItem->m_labelDes = labelDes;
        pItem->m_layerBg = layerBg;
        activityItem = pItem;

    }
    else
    {
        activityItem = m_vecRewardItemOld.back();
        m_vecRewardItemOld.pop_back();
    }

    return activityItem;
}

void ActivityRewards::UpdateRewardItem(ActivityItem *item, ActReward rewardInfo)
{
    SetLabelString(item->m_labelDes, rewardInfo.m_strDes);
    SetHttpSpriteURL(item->m_spriteReward, rewardInfo.m_strURL);
    item->m_spriteReward->setScale(0.7);
}