#include "ChatBox.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "HitTouchGrabLayer.h"
#include "HttpSprite.h"
#include "GameCommon.h"
#include "PlatformSpec.h"
#include "GlobalUserData.h"

USING_NS_CC;

void ChatBox::AppandWord(const std::string& strIcon, const std::string& strWord, unsigned uUID,  bool bIconLeft)
{
	auto label = CreateLabelMSYH(24, "");
	label->setTextColor(Color4B(255, 255, 255, 255));
	label->setWidth(250);
	label->setLineBreakWithoutSpace(true);
	SetLabelString(label, strWord);
	label->updateContent();
	auto sz = label->getContentSize();

	float fY = m_fScrollViewWordStartY;
	if (fY <= 0)
	{
		fY = 0;
	}
	if (sz.height >= 30) // 超过一行
	{
		if (sz.height <= 84)
		{
			label->setHorizontalAlignment(TextHAlignment::LEFT);
			label->setVerticalAlignment(TextVAlignment::CENTER);
			label->setAnchorPoint(Vec2(0, 0.5));
			label->setPosition((bIconLeft ? 98 : 98), fY + 84 / 2 + 5);
		}
		else
		{
			label->setHorizontalAlignment(TextHAlignment::LEFT);
			label->setVerticalAlignment(TextVAlignment::BOTTOM);
			label->setAnchorPoint(Vec2::ZERO);
			label->setPosition((bIconLeft ? 98 : 98), fY + 5);
		}
	}
	else
	{
		label->setHorizontalAlignment(bIconLeft ? TextHAlignment::LEFT : TextHAlignment::RIGHT);
		label->setVerticalAlignment(TextVAlignment::CENTER);
		label->setAnchorPoint(bIconLeft ? Vec2(0, 0.5) : Vec2(1, 0.5));
		label->setPosition((bIconLeft ? 98 : 358), fY + 84 / 2 + 5);
	}

	Size szBg(250, 84);
	if (sz.height >= 84)
	{
		szBg.height = sz.height;
	}

	auto icon = HttpSprite::create("defaulticon.png", Size(58, 58)); 
	SetIconUrl(icon, strIcon);
	float fx = bIconLeft ? 32 : 358;
	icon->setPosition(fx, fY + szBg.height - 46 - (84 - 46) / 2);
	auto btpressicon = HitTouchGrabLayer::create();
	btpressicon->setContentSize(icon->getContentSize());
	btpressicon->setPosition(icon->getPosition());
	btpressicon->SetTouchDiscardCondition([](Node*node, Touch*touch)->bool{Vec2 offset = touch->getDelta(); return (offset.x * offset.x + offset.y * offset.y) >= 8;});
	btpressicon->SetHitCB(nullptr, nullptr, [](Node*node,bool bSelected){});
	auto btpresslabel = HitTouchGrabLayer::create();
	btpresslabel->setContentSize(szBg);
	btpresslabel->setPosition(bIconLeft ? 58 : 14, fY + 0);
	btpresslabel->SetTouchDiscardCondition([](Node*node, Touch*touch)->bool{Vec2 offset = touch->getDelta(); return (offset.x * offset.x + offset.y * offset.y) >= 8;});
	btpresslabel->SetHitCB(nullptr, nullptr, 
		[this](Node*node,bool bSelected)
	{
		if (bSelected)
		{
			for (int i = 0; i < m_vecBtPressWords.size(); ++i)
			{
				if (node == m_vecBtPressWords[i])
				{
					m_pEdtChat->setText(m_vecLabelWords[i]->getString().c_str());
				}
			}
		}
	});
	btpresslabel->SetTouchEnable(!m_pBtWord->isEnabled());
	
	m_fScrollViewWordStartY -= szBg.height;
	auto szold = m_pScrollViewWord->getContentSize();
	double dContentH = (494-m_fScrollViewWordStartY-84);
	if (dContentH > szold.height)
	{
		float fOffY = dContentH - szold.height;
		m_pScrollViewWord->setContentSize(Size(szold.width, dContentH));
		for (auto item : m_vecLabelWords)
		{
			item->setPositionY(item->getPositionY() + fOffY);
		}
		for (auto item : m_vecIconOfWords)
		{
			item->setPositionY(item->getPositionY() + fOffY);
		}
		for (auto item : m_vecBtPressWords)
		{
			item->setPositionY(item->getPositionY() + fOffY);
		}
		for (auto item : m_vecBtPressIcons)
		{
			item->setPositionY(item->getPositionY() + fOffY);
		}
	}
	m_pScrollViewWord->addChild(label);
	m_pScrollViewWord->addChild(icon);
	m_pScrollViewWord->addChild(btpressicon);
	m_pScrollViewWord->addChild(btpresslabel);
	
	m_vecLabelWords.push_back(label);
	m_vecIconOfWords.push_back(icon);
	m_vecBtPressWords.push_back(btpresslabel);
	m_vecBtPressIcons.push_back(btpressicon);
	m_vecWordUIDs.push_back(uUID);
}

void ChatBox::AppandUser(const std::string& strIcon, const std::string& strName, unsigned uUID)
{
	// 取消选中区域
	m_pLayerPressAt->setVisible(false);

	const float fH = 67;

	float fNeedH = (m_vecAtUIDs.size() + 1) * fH;
	auto szCur = m_pScrollViewAt->getContentSize();
	float fY;
	if (szCur.height < fNeedH)
	{
		float fOffset = fNeedH - szCur.height;
		m_pScrollViewAt->setContentSize(Size(szCur.width, fNeedH));
		m_pLayerPressAt->setPositionY(m_pLayerPressAt->getPositionY() + fOffset);
		for (unsigned i = 0; i < m_vecAtUIDs.size(); ++i)
		{
			m_vecLabelAt[i]->setPositionY(m_vecLabelAt[i]->getPositionY() + fOffset);
			m_vecIconAt[i]->setPositionY(m_vecIconAt[i]->getPositionY() + fOffset);
			m_vecBtPressAt[i]->setPositionY(m_vecBtPressAt[i]->getPositionY() + fOffset);
		}
		fY = 0;
	}
	else
	{
		float fCurH = m_vecAtUIDs.size() * fH;
		fY = 494 - fH - fCurH;
	}

	// 点击区域
	auto presslayer = HitTouchGrabLayer::create();
	presslayer->setContentSize(Size(szCur.width, fH));
	presslayer->SetTouchDiscardCondition([](Node*node, Touch*touch)->bool{Vec2 offset = touch->getDelta(); return (offset.x * offset.x + offset.y * offset.y) >= 8;});
	presslayer->SetHitCB(
		[this](Node*node){m_pLayerPressAt->setPosition(node->getPosition());m_pLayerPressAt->setVisible(true);},
		[this](Node*node){m_pLayerPressAt->setVisible(false);},
		[this](Node*node,bool bSelected){
			if (bSelected)
			{
				for (unsigned i = 0; i < m_vecAtUIDs.size(); ++i)
				{
					if (node == m_vecBtPressAt[i])
					{
						std::string strName = m_vecLabelAt[i]->getString();
						std::string strNew("@");
						strNew += strName;
						//std::string strOld = m_pEdtChat->getText();
						//strNew += strOld;
						m_pEdtChat->setText(strNew.c_str());
                        m_IsAtOthers = true;
						break;
					}
				}
			}
	});
	presslayer->SetTouchEnable(!m_pBtAt->isVisible());
	// 分割线
	auto spriteSepLine = Sprite::create("chatbox/lineseplong.png");
	spriteSepLine->setPosition(szCur.width / 2, 1);
	presslayer->addChild(spriteSepLine);
	presslayer->setPosition(0, fY);
	m_pScrollViewAt->addChild(presslayer);
	m_vecBtPressAt.push_back(presslayer);

	// 头像
	auto spriteIcon = HttpSprite::create("defaulticon.png", Size(56, 56));
	SetIconUrl(spriteIcon, strIcon);
	spriteIcon->setPosition(87, fY + 4);
	m_pScrollViewAt->addChild(spriteIcon);
	m_vecIconAt.push_back(spriteIcon);

	// 昵称
	auto labelName = CreateLabelMSYH(20+5, "", TextHAlignment::LEFT, TextVAlignment::CENTER);
	labelName->setTextColor(Color4B::WHITE);
	SetLabelString(labelName, strName);
	labelName->setAnchorPoint(Vec2(0, 0.5));
	labelName->setPosition(148, fY + 32);
	m_pScrollViewAt->addChild(labelName);
	m_vecLabelAt.push_back(labelName);

	// 用户ID
	m_vecAtUIDs.push_back(uUID);
}

void ChatBox::RemoveUser(unsigned uUID)
{
	// 取消选中区域
	m_pLayerPressAt->setVisible(false);

	const float fH = 67;
	unsigned uLine = m_vecAtUIDs.size();
	for (unsigned i = 0; i < m_vecAtUIDs.size(); ++i)
	{
		if (m_vecAtUIDs[i] == uUID)
		{
			uLine = i;
			break;
		}
	}

	if (uLine < m_vecAtUIDs.size())
	{
		auto sz = m_pScrollViewAt->getContentSize();
		if (sz.height <= 495)
		{
			for (unsigned i = uLine + 1; i < m_vecAtUIDs.size(); i++)
			{
				m_vecLabelAt[i]->setPositionY(m_vecLabelAt[i]->getPositionY() + fH);
				m_vecIconAt[i]->setPositionY(m_vecIconAt[i]->getPositionY() + fH);
				m_vecBtPressAt[i]->setPositionY(m_vecBtPressAt[i]->getPositionY() + fH);
			}
			m_pScrollViewAt->setContentSize(Size(sz.width, 494));
		}
		else
		{
			for (unsigned i = 0; i < uLine; i++)
			{
				m_vecLabelAt[i]->setPositionY(m_vecLabelAt[i]->getPositionY() - fH);
				m_vecIconAt[i]->setPositionY(m_vecIconAt[i]->getPositionY() - fH);
				m_vecBtPressAt[i]->setPositionY(m_vecBtPressAt[i]->getPositionY() - fH);
			}
			m_pScrollViewAt->setContentSize(Size(sz.width, sz.height - fH));
		}
		
		m_vecAtUIDs.erase(m_vecAtUIDs.begin() + uLine);
		m_vecLabelAt[uLine]->setVisible(false);
		m_vecIconAt[uLine]->setVisible(false);
		m_vecBtPressAt[uLine]->setVisible(false);
		m_pScrollViewAt->removeChild(m_vecLabelAt[uLine]);
		m_pScrollViewAt->removeChild(m_vecIconAt[uLine]);
		m_pScrollViewAt->removeChild(m_vecBtPressAt[uLine]);
		m_vecLabelAt.erase(m_vecLabelAt.begin() + uLine);
		m_vecIconAt.erase(m_vecIconAt.begin() + uLine);
		m_vecBtPressAt.erase(m_vecBtPressAt.begin() + uLine);
	}
}


void ChatBox::onMenuItemExpressCommonSelected(cocos2d::Ref *pItem)
{
	for (unsigned i = 0; i < m_vecBtExpressCommon.size(); i++)
	{
		if (pItem == m_vecBtExpressCommon.at(i))
		{
			SendExpress(i, true);
			if (m_funCBHide)
			{
				m_funCBHide();
			}
			break;
		}
	}
}

void ChatBox::onMenuItemExpressNobleSelected(cocos2d::Ref *pItem)
{
	for (unsigned i = 0; i < m_vecBtExpressNoble.size(); i++)
	{
		if (pItem == m_vecBtExpressNoble.at(i))
		{
			SendExpress(i, false);
			if (m_funCBHide)
			{
				m_funCBHide();
			}
			break;
		}
	}
}

void ChatBox::onMenuItemSelected(cocos2d::Ref *pItem)
{
	if (pItem == m_pBtExpress)
	{
        m_IsExpress = true;
		m_radioGroupLeftSwitch.Activate(RadioItemIcon);
	}
	else if (pItem == m_pBtLine)
	{
        m_IsExpress = false;
		m_radioGroupLeftSwitch.Activate(RadioItemLine);
		m_pBtExpressCommon->setVisible(false);
		m_pBtExpressNoble->setVisible(false);
	}
	else if (pItem == m_pBtExpressCommon)
	{
		m_radioGroupIconSwitch.Activate(RadioItemIconCommon);
	}
	else if (pItem == m_pBtExpressNoble)
	{
		m_radioGroupIconSwitch.Activate(RadioItemIconNoble);
	}
	else if (pItem == m_pBtWord)
	{
        m_IsExpress = false;
		m_radioGroupLeftSwitch.Activate(RadioItemWord);
		m_pBtExpressCommon->setVisible(false);
		m_pBtExpressNoble->setVisible(false);
	}
	else if (pItem == m_pBtSend)
	{
		SendChat();
	}
	else if (pItem == m_pBtAt)
	{
		//m_pEdtChat->setPositionX(m_pEdtChat->getPositionX() + 60);
        m_pEdtChat->setPositionX(80);
		m_pBtAt->setVisible(false);
		m_pBtBack->setVisible(true);
		m_pBtExpressCommon->setVisible(false);
		m_pBtExpressNoble->setVisible(false);
		m_pSpriteBgEditLong->setVisible(false);
		m_pSpriteBgEditShort->setVisible(true);
		m_pScrollViewAt->setVisible(true);
		for (auto item : m_vecBtPressAt)
		{
			item->SetTouchEnable(true);
		}
		m_pLayerBgAt->setVisible(true);
		m_pLayerBgAt->SetTouchEnable(true);

		m_pBtExpress->setVisible(false);
		m_pBtWord->setVisible(false);
		m_pBtLine->setVisible(false);
        if (m_IsExpress)
        {
            m_pBtExpressCommon->setVisible(false);
            m_pBtExpressNoble->setVisible(false);
        }
	}
	else if (pItem == m_pBtBack)
	{
		//m_pEdtChat->setPositionX(m_pEdtChat->getPositionX() - 45);
        m_pEdtChat->setPositionX(20);
		m_pBtAt->setVisible(true);
		m_pBtBack->setVisible(false);
		m_pSpriteBgEditLong->setVisible(true);
		m_pSpriteBgEditShort->setVisible(false);
		m_pScrollViewAt->setVisible(false);
		for (auto item : m_vecBtPressAt)
		{
			item->SetTouchEnable(false);
		}
		m_pLayerBgAt->setVisible(false);
		m_pLayerBgAt->SetTouchEnable(false);

		m_pBtExpress->setVisible(true);
		m_pBtWord->setVisible(true);
		m_pBtLine->setVisible(true);
        if (m_IsExpress)
        {
            m_pBtExpressCommon->setVisible(true);
            m_pBtExpressNoble->setVisible(true);
        }
		else
		{
			m_pBtExpressCommon->setVisible(false);
			m_pBtExpressNoble->setVisible(false);
		}
	}
}

// Layer
bool ChatBox::init()
{
	if (!GameDialog::init("chatbox/bg.png", false))
	{
		return false;
	}

	GameDialog::setVisible(false);

	setCascadeOpacityEnabled(true);

	m_pSpriteBgEditLong = Sprite::create("chatbox/bgeditlong.png");
	m_pSpriteBgEditLong->setVisible(true);
	m_pSpriteBgEditLong->setPosition(189, 45);
	addChild(m_pSpriteBgEditLong);

	m_pSpriteBgEditShort = Sprite::create("chatbox/bgeditshort.png");
	m_pSpriteBgEditShort->setVisible(false);
	m_pSpriteBgEditShort->setPosition(224, 45);
	addChild(m_pSpriteBgEditShort);

	m_pBtExpress = MenuItemSpriteWithLabel::create("chatbox/bticon.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtExpress->setPosition(Vec2(50, 507));

	m_pBtLine = MenuItemSpriteWithLabel::create("chatbox/btline.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtLine->setPosition(Vec2(50, 342.3));

	m_pBtWord = MenuItemSpriteWithLabel::create("chatbox/btword.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtWord->setPosition(Vec2(50, 176));

	m_pBtAt = MenuItemSpriteWithLabel::create("chatbox/btat.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtAt->setPosition(Vec2(335, 45));

	m_pBtBack = MenuItemSpriteWithLabel::create("chatbox/btback.png", 3, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtBack->setPosition(Vec2(41, 47));
	m_pBtBack->setVisible(false);

	m_pBtSend = MenuItemSpriteWithLabel::create("chatbox/btsend.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtSend->setPosition(Vec2(437, 47));
	auto pLabel = CreateLabelMSYHBD(26, "");
	SetLabelString(pLabel, "发送");
	m_pBtSend->SetEnabledColor(Color4B(0xff, 0xff, 0xff, 0xff));
	m_pBtSend->SetDisabledColor(Color4B(0, 0, 0, 0xff));
	m_pBtSend->SetLabel(pLabel);

	m_pEdtChat = ui::EditBox::create(Size(230, 45), ui::Scale9Sprite::create("chatbox/bgedit.png"));
	m_pEdtChat->setPosition(Vec2(20, 25));
	m_pEdtChat->setAnchorPoint(Vec2(0, 0));
	m_pEdtChat->setFont("fzlt.ttf", 24);
	m_pEdtChat->setFontColor(Color3B::BLACK);
	m_pEdtChat->setMaxLength(30);
	m_pEdtChat->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pEdtChat->setDelegate(this);
	addChild(m_pEdtChat, 1);

	m_pBtExpressCommon = MenuItemSpriteWithLabel::create("chatbox/bticoncommon.png", 3, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtExpressCommon->setPosition(Vec2(214, 559));
	pLabel = CreateLabelMSYHBD(24, "");
	SetLabelString(pLabel, "基本");
	m_pBtExpressCommon->SetEnabledColor(Color4B(0xec, 0xc7, 0x87, 0xff));
	m_pBtExpressCommon->SetDisabledColor(Color4B(0, 0, 0, 0xff));
	m_pBtExpressCommon->SetLabel(pLabel);
	m_pBtExpressNoble = MenuItemSpriteWithLabel::create("chatbox/bticonnoble.png", 3, CC_CALLBACK_1(ChatBox::onMenuItemSelected, this));
	m_pBtExpressNoble->setPosition(Vec2(379, 559));
	pLabel = CreateLabelMSYHBD(24, "");
	SetLabelString(pLabel, "贵族");
	m_pBtExpressNoble->SetEnabledColor(Color4B(0xec, 0xc7, 0x87, 0xff));
	m_pBtExpressNoble->SetDisabledColor(Color4B(0, 0, 0, 0xff));
	m_pBtExpressNoble->SetLabel(pLabel);

	auto pMenu = Menu::create(m_pBtExpress, m_pBtLine, m_pBtWord, m_pBtAt, m_pBtBack, m_pBtSend, m_pBtExpressCommon, m_pBtExpressNoble, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu, 1);

	for (int i = 0; i < 16; ++i)
	{
		auto menuItem = MenuItemSpriteWithLabel::create("chatbox/bticonpress.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemExpressCommonSelected, this));
		menuItem->setPosition(138 + (i % 4) * 105, 474 - (i / 4) * 105);
		menuItem->setVisible(false);
		m_vecBtExpressCommon.pushBack(menuItem);
	}
	for (int i = 0; i < 16; ++i)
	{
		auto menuItem = MenuItemSpriteWithLabel::create("chatbox/bticonpress.png", 2, CC_CALLBACK_1(ChatBox::onMenuItemExpressNobleSelected, this));
		menuItem->setPosition(138 + (i % 4) * 105, 474 - (i / 4) * 105);
		menuItem->setVisible(false);
		m_vecBtExpressNoble.pushBack(menuItem);
	}

	pMenu = Menu::createWithArray(m_vecBtExpressCommon);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu);

	pMenu = Menu::createWithArray(m_vecBtExpressNoble);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu);

	m_pSpriteExpressCommon = Sprite::create("express/allexpress.png");
	m_pSpriteExpressCommon->setPosition(295, 317);
	m_pSpriteExpressCommon->setVisible(false);
	addChild(m_pSpriteExpressCommon);

	m_pSpriteExpressNoble = Sprite::create("express/allexpressnoble.png");
	m_pSpriteExpressNoble->setPosition(295, 317);
	m_pSpriteExpressNoble->setVisible(false);
	addChild(m_pSpriteExpressNoble);

	m_pScrollViewLine = extension::ScrollView::create(Size(400, 490)); // 328 67
	static const unsigned s_uLineCount = 18;
	static const char *s_szLines[s_uLineCount] = {
		"快点下注吧，时间宝贵",
		"各位爷，让看看牌再加钱吧",
		"我的宝剑已经饥渴难耐了",
		"莫偷鸡！偷鸡必被抓",
		"你牌技这么好，地球人知道吗？",
		"冲动是魔鬼，冷静！",
		"很高兴能和大家一起打牌",
		"赢钱了别走，留下你的姓名",
		"打诚信德州，不偷不抢",
		"有埋伏？不要轻举妄动",
		"这手牌打得不错，赢得漂亮",
		"难道你看穿我的底牌了吗？",
		"我加注了，有胆子跟吗？",
		"青山不改绿水长流，改日再战",
		"难道这就是失败的滋味？",
		"冤家牌，没办法",
		"嘿嘿，我又赢了，感谢大家！",
		"你真是一个天生的演员",
	};
	m_pScrollViewLine->setContentSize(Size(400, 83 * s_uLineCount));
	m_pScrollViewLine->setDirection(extension::ScrollView::Direction::VERTICAL);
	float fx = 20, fy = 55;
	auto bghitline = LayerColor::create(Color4B(9, 43, 21, 255));
	bghitline->setContentSize(Size(400, 83));
	bghitline->setVisible(false);
	m_pScrollViewLine->addChild(bghitline);
	for (int i = 0; i < s_uLineCount; i++)
	{
		auto pLabel = CreateLabelMSYH(28, "", TextHAlignment::LEFT, TextVAlignment::CENTER);
		SetLabelString(pLabel, s_szLines[s_uLineCount-i-1]);
		pLabel->setPosition(fx, fy - 12);
		pLabel->setAnchorPoint(Vec2(0, 0.5));
		m_pScrollViewLine->addChild(pLabel, 1);
		auto pLine = Sprite::create("chatbox/linesep.png");
		pLine->setPosition(194, fy - 53);		
		m_pScrollViewLine->addChild(pLine, 1);

		auto pBt = HitTouchGrabLayer::create();
		pBt->setContentSize(Size(328, 67));
		pBt->setPosition(0, fy - 53.5);
		m_vecBtLine.push_back(pBt);
		m_pScrollViewLine->addChild(pBt, 1);
		pBt->SetTouchDiscardCondition([](Node*node, Touch*touch)->bool{Vec2 offset = touch->getDelta(); return (offset.x * offset.x + offset.y * offset.y) >= 8;});
		
		const char **pQuickChat = s_szLines;
		unsigned uQuickChatCount = s_uLineCount;
		pBt->SetHitCB(
			[bghitline](Node*node){bghitline->setPosition(node->getPosition());bghitline->setVisible(true);},
			[bghitline](Node*node){bghitline->setVisible(false);},
			[this, pQuickChat, uQuickChatCount](Node*node,bool bSelected){
				if (bSelected)
				{
					for (int i = 0; i < m_vecBtLine.size(); i++)
					{
						if (node == m_vecBtLine[i])
						{
                            std::string strTmp = "";
                            if (m_IsAtOthers)
                            {
                                strTmp = m_pEdtChat->getText();
                            }
							SendChat(strTmp + pQuickChat[uQuickChatCount - 1 - i]);
							if (m_funCBHide)
							{
								m_funCBHide();
							}
                            m_IsAtOthers = false;
						}
					}
				}
		});
		pBt->SetTouchEnable(false);
		fy += 83;
	}

	m_pScrollViewLine->setVisible(false);
	m_pScrollViewLine->setPosition(94, 93);
	addChild(m_pScrollViewLine);

	m_pScrollViewWord = extension::ScrollView::create(Size(487, 494));
	m_pScrollViewWord->setVisible(false);
	m_pScrollViewWord->setPosition(74, 93);
	m_pScrollViewWord->setContentSize(Size(487, 494));
	m_pScrollViewWord->setDirection(extension::ScrollView::Direction::VERTICAL);
	addChild(m_pScrollViewWord);
	m_fScrollViewWordStartY = 494 - 84;

	auto funcIconActiveCommon = [this](bool bActive)
	{
		m_pBtExpressCommon->setVisible(true);
		m_pBtExpressNoble->setVisible(true);
		m_pBtExpressCommon->setEnabled(!bActive);
		m_pBtExpressNoble->setEnabled(bActive);
		m_pSpriteExpressCommon->setVisible(bActive);
		m_pSpriteExpressNoble->setVisible(!bActive);
		for (auto item : m_vecBtExpressCommon)
		{
			item->setVisible(bActive);
		}
		for (auto item : m_vecBtExpressNoble)
		{
			item->setVisible(!bActive);
		}
	};
	auto funcIconActiveNoble = [this](bool bActive)
	{
		bActive = !bActive;
		m_pBtExpressCommon->setVisible(true);
		m_pBtExpressNoble->setVisible(true);
		m_pBtExpressCommon->setEnabled(!bActive);
		m_pBtExpressNoble->setEnabled(bActive);
		m_pSpriteExpressCommon->setVisible(bActive);
		m_pSpriteExpressNoble->setVisible(!bActive);
		for (auto item : m_vecBtExpressCommon)
		{
			item->setVisible(bActive);
		}
		for (auto item : m_vecBtExpressNoble)
		{
			item->setVisible(!bActive);
		}
	};
	m_radioGroupIconSwitch.AddGroupItem(RadioGroupItem(funcIconActiveCommon));
	m_radioGroupIconSwitch.AddGroupItem(RadioGroupItem(funcIconActiveNoble));
	m_radioGroupIconSwitch.Activate(RadioItemIconCommon);


	auto funcLeftActiveIcon = [this](bool bActive)
	{
		m_pBtExpress->setEnabled(!bActive);
		if (bActive)
		{
			m_pBtExpressCommon->setVisible(true);
			m_pBtExpressNoble->setVisible(true);
			bool bCurCommon = !(m_pBtExpressCommon->isEnabled());
			m_pSpriteExpressCommon->setVisible(bCurCommon);
			m_pSpriteExpressNoble->setVisible(!bCurCommon);
			for (auto item : m_vecBtExpressCommon)
			{
				item->setVisible(bCurCommon);
			}
			for (auto item : m_vecBtExpressNoble)
			{
				item->setVisible(!bCurCommon);
			}
		}
		else
		{
			m_pBtExpressCommon->setVisible(false);
			m_pBtExpressNoble->setVisible(false);
			m_pSpriteExpressCommon->setVisible(false);
			m_pSpriteExpressNoble->setVisible(false);
			for (auto item : m_vecBtExpressCommon)
			{
				item->setVisible(false);
			}
			for (auto item : m_vecBtExpressNoble)
			{
				item->setVisible(false);
			}
		}
	};
	auto funcLeftActiveLine = [this](bool bActive)
	{
		m_pBtLine->setEnabled(!bActive);
		m_pScrollViewLine->setVisible(bActive);
		for (auto item : m_vecBtLine)
		{
			item->SetTouchEnable(bActive);
		}
	};
	auto funcLeftActiveWord = [this](bool bActive)
	{
		m_pBtWord->setEnabled(!bActive);
		m_pScrollViewWord->setVisible(bActive);
		for (auto item : m_vecBtPressWords)
		{
			item->SetTouchEnable(bActive);
		}
		for (auto item : m_vecBtPressIcons)
		{
			item->SetTouchEnable(bActive);
		}
	};

	m_radioGroupLeftSwitch.AddGroupItem(RadioGroupItem(funcLeftActiveIcon));
	m_radioGroupLeftSwitch.AddGroupItem(RadioGroupItem(funcLeftActiveLine));
	m_radioGroupLeftSwitch.AddGroupItem(RadioGroupItem(funcLeftActiveWord));
	m_radioGroupLeftSwitch.Activate(RadioItemIcon);

	// 10 80 400 402
	// 7 75 405 413 
	m_pLayerBgAt = TouchGrabLayer::create();
	m_pLayerBgAt->SetTouchSwallow(true);
	m_pLayerBgAt->SetTouchBeganCB([](Node*node, Touch*touch, Event*unused_event)->bool{return TouchHitNode(touch, node);});
	m_pLayerBgAt->setContentSize(Size(487, 494));
	m_pLayerBgAt->setPosition(12, 94);
	m_pLayerBgAt->setVisible(false);
	m_pLayerBgAt->SetTouchEnable(false);
	auto bgcolor = LayerColor::create(Color4B(0, 0, 0, 230));
	bgcolor->setContentSize(Size(487, 494));
	m_pLayerBgAt->addChild(bgcolor);
	addChild(m_pLayerBgAt);

	m_pScrollViewAt = extension::ScrollView::create(Size(487, 494));
	m_pScrollViewAt->setContentSize(Size(487, 494));
	m_pScrollViewAt->setPosition(12, 94);
	m_pScrollViewAt->setVisible(false);
	m_pScrollViewAt->setDirection(extension::ScrollView::Direction::VERTICAL);
	addChild(m_pScrollViewAt);

	m_pLayerPressAt = LayerColor::create(Color4B(9, 43, 21, 255));
	m_pLayerPressAt->setContentSize(Size(487, 67));
	m_pLayerPressAt->setPosition(0, 0);
	m_pLayerPressAt->setVisible(false);
	m_pScrollViewAt->addChild(m_pLayerPressAt);
    m_IsExpress = true;

	return true;
}

void ChatBox::setVisible(bool bVisible)
{
	this->setAnchorPoint(Vec2(0, 0));
	if (bVisible)
	{
		auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0.618);GameDialog::setVisible(true);}), actScale, nullptr));
	}
	else
	{
		auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
		runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));
        if (m_pLayerBgAt->isVisible())
            onMenuItemSelected(m_pBtBack);
	}
}

void ChatBox::onSetVisible(bool bVisible)
{
	if (!bVisible)
	{
		for (auto item : m_vecBtLine)
		{
			item->SetTouchEnable(false);
		}
		for (auto item : m_vecBtPressWords)
		{
			item->SetTouchEnable(false);
		}
		for (auto item : m_vecBtPressIcons)
		{
			item->SetTouchEnable(false);
		}
		for (auto item : m_vecBtPressAt)
		{
			item->SetTouchEnable(false);
		}
	}
	else
	{
		for (auto item : m_vecBtLine)
		{
			item->SetTouchEnable(!m_pBtLine->isEnabled());
		}
		for (auto item : m_vecBtPressWords)
		{
			item->SetTouchEnable(!m_pBtWord->isEnabled());
		}
		for (auto item : m_vecBtPressIcons)
		{
			item->SetTouchEnable(!m_pBtWord->isEnabled());
		}
		for (auto item : m_vecBtPressAt)
		{
			item->SetTouchEnable(!m_pBtAt->isVisible());
		}
	}
}

// EditBoxDelegate
void ChatBox::editBoxReturn(ui::EditBox* editBox)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (nullptr != editBox && false == editBox->isRetPressed())
        //setVisible(false);
        return;
#endif
    
	SendChat();
}

void ChatBox::SendChat(const std::string& strChat)
{
	Json::Value param;

	param["text"] = strChat;
	SendClientRequest(ClientRequest::kPublicChat, param);
	setVisible(false);
}

void ChatBox::SendChat()
{
	std::string strChat(m_pEdtChat->getText());
	if (!strChat.empty())
	{
		SendChat(strChat);
		m_pEdtChat->setText("");
        g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskChat);
	}
}

void ChatBox::SendExpress(unsigned uIdx, bool bCommon)
{
	Json::Value param;
	param["expressidx"] = uIdx;
	param["expresstype"] = bCommon ? 0 : 1;
	SendClientRequest(ClientRequest::kPublicExpress, param);
	setVisible(false);
}

void ChatBox::SetEditText(std::string strName)
{
    m_pEdtChat->setText(strName.c_str());
    m_IsAtOthers = true;
}

void ChatBox::resetAt()
{
	while (m_vecAtUIDs.size() > 0) {
		unsigned uid = m_vecAtUIDs.at(m_vecAtUIDs.size() - 1);
		RemoveUser(uid);
	}
    m_pEdtChat->setText("");

}