#include "OpenUpVip.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"
#include "MenuItemSpriteWithLabel.h"
#include "GrabKeyboard.h"
#include "GameCommon.h"
#include "Util.h"
#include "GameData.h"

USING_NS_CC;

bool OpenUpVip::init()
{
    
    if (!GameLayer::init(GameData::getInstance()->getGameWidth(), GameData::getInstance()->getGameHeight(), Color4B(0,0,0,150)))
    {
        return false;
    }
    auto spriteBg = CreateNode1AndPosAddTo<Sprite>("quickshop/bgquickshop.png", GAME_WIDTH / 2, GAME_HEIGHT / 2, this, 1);
    auto spriteTitle = CreateNode1AndPosAddTo<Sprite>("vipdredge/title.png", 308, 470, spriteBg, 0);
    auto spriteVipRank = CreateNode1AndPosAddTo<Sprite>("vipdredge/titlerank.png", 308, 390, spriteBg, 1);
    auto spriteRank = CreateNode3AndPosAddTo<FrameSprite>("vipdredge/viptupe.png", 6, 1, 263, 38, spriteVipRank, 1);
    spriteRank->SetFrameIdx(g_globalMyData.GetVipLevel() - 1);

    auto m_btClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(OpenUpVip::OnClose, this));
    m_btClose->setPosition(590, 457);
    auto menuPageRight = Menu::create(m_btClose, nullptr);
    menuPageRight->setPosition(Vec2::ZERO);
    spriteBg->addChild(menuPageRight);

    std::string strItem[8] = {"立即获赠德州币：","立即获赠经验：","每日登录返还：","幸运值：",
        "贵族表情包：","贵族魔法表情：","牌局记录：","私密开放："};
    for (int i=0; i<8; i++)
    {
        m_labelItemName[i] = CreateLabelMSYHAndAnchorPosClrAddTo(24, strItem[i], Vec2(1,0),321, 309-i*40, Color4B(0xff,0xff,0xff,0xff), spriteBg, 1);
        m_labelItemContent[i] = CreateLabelMSYHAndAnchorPosClrAddTo(24, "", Vec2(0,0),327, 309-i*40, Color4B(0xff,0xd2,0x00,0xff), spriteBg, 1);
    }
    return true;
}

void OpenUpVip::OnClose(cocos2d::Ref* pSender)
{
    this->setVisible(false);
}

 void OpenUpVip::onKeyEscPressed()
 {
     setVisible(false);
}

 void OpenUpVip::Update()
 {
	 char buff[20] = {0};
	 sprintf(buff, "%.0f", g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_dAddLeBi);
	 SetLabelString(m_labelItemContent[0], buff);
	 sprintf(buff, "%d点", g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_uAddJingYan);
	 SetLabelString(m_labelItemContent[1], buff);
	 sprintf(buff, "%.0f德州币", g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_dDengLuFanHuan);
	 SetLabelString(m_labelItemContent[2], buff);
	 sprintf(buff, "+%d点", g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_uAddMeiLi);
	 SetLabelString(m_labelItemContent[3], buff);
	 if (g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenGuiZuBiaoQing)
	 {
		 SetLabelString(m_labelItemContent[4], "开启");
	 }
	 else
	 {
		 SetLabelString(m_labelItemContent[4], "-");
	 }
	 if (g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenGuiZuMoFaBiaoQing)
	 {
		 SetLabelString(m_labelItemContent[5], "开启");
	 }
	 else
	 {
		 SetLabelString(m_labelItemContent[5], "-");
	 }
	 sprintf(buff, "%d手", g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_uJiLuPaiJu);
	 SetLabelString(m_labelItemContent[6], buff);

	 if (g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenSiMiKaiFang && g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenZiDingYiKaiFang)
	 {
		 SetLabelString(m_labelItemContent[7], "开启&可自定义");
	 }
	 else if (g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenSiMiKaiFang && !g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenZiDingYiKaiFang)
	 {
		 SetLabelString(m_labelItemContent[7], "开启");
	 }
	 else if (!g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenSiMiKaiFang && g_globalMallInfo.m_vipInfo[g_globalMyData.GetVipLevel() - 1].m_bOpenZiDingYiKaiFang)
	 {
		 SetLabelString(m_labelItemContent[7], "可自定义");
	 }
	 else
	 {
		 SetLabelString(m_labelItemContent[7], "-");
	 }
 }