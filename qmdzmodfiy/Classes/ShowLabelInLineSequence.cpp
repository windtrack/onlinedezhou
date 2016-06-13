#include "ShowLabelInLineSequence.h"
#include "Util.h"
#include "AnimationManager.h"
USING_NS_CC;

// static 
ShowLabelInLineSequence * ShowLabelInLineSequence::create(const std::string& strBg, const std::string& strFont, float fFontSize, int iLineMaxWidth)
{
	ShowLabelInLineSequence *pRet = new ShowLabelInLineSequence;
	if (pRet->init(strBg, strFont, fFontSize, iLineMaxWidth))
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

// static 
ShowLabelInLineSequence *ShowLabelInLineSequence::create(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth)
{
	ShowLabelInLineSequence *pRet = new ShowLabelInLineSequence;
	if (pRet->init(strBg, pLabel, iLineMaxWidth))
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

ShowLabelInLineSequence *ShowLabelInLineSequence::create(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth, unsigned uShowLineNum)
{
    ShowLabelInLineSequence *pRet = new ShowLabelInLineSequence;
    if (pRet->init(strBg, pLabel, iLineMaxWidth, uShowLineNum))
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

void ShowLabelInLineSequence::SetText(const std::string& str)
{
	unschedule(schedule_selector(ShowLabelInLineSequence::onScheduleChangeLine));
	auto sz = m_pClip->getContentSize();
	m_pLabel->setPosition(sz.width / 2, sz.height);
	SetLabelString(m_pLabel, str);
	// m_pLabel->setString(str);
	m_iCurLine = 0;
	int iLines = m_pLabel->getStringNumLines();
	schedule(schedule_selector(ShowLabelInLineSequence::onScheduleChangeLine), 2, kRepeatForever, 2.0f);
}

void ShowLabelInLineSequence::MultLineSetText(const std::string& str)
{
    if (m_bVisible)
        return;
    SetLabelString(m_pLabel, str);
    int iLines = m_pLabel->getStringNumLines();
    ResetClipPosAndSz(iLines);
    unschedule(schedule_selector(ShowLabelInLineSequence::onScheduleChangeMultLine));
    auto sz = m_pClip->getContentSize();
    m_pLabel->setPosition(sz.width / 2, sz.height);
    m_iCurLine = 0;
    schedule(schedule_selector(ShowLabelInLineSequence::onScheduleChangeMultLine), 2, kRepeatForever, 2.0f);
    m_bVisible = true;
}

void ShowLabelInLineSequence::SetTextColor(const cocos2d::Color4B& clr)
{
	m_pLabel->setTextColor(clr);
}

void ShowLabelInLineSequence::SetHAlignment(cocos2d::TextHAlignment halign)
{
	m_pLabel->setHorizontalAlignment(halign);
}


void ShowLabelInLineSequence::SetLastLineFinishedCB(std::function<void(ShowLabelInLineSequence*)> lstLineFinishedCB)
{
	m_funLstLineFinishedCB = lstLineFinishedCB;
}

bool ShowLabelInLineSequence::init(const std::string& strBg, const std::string& strFont, float fFontSize, int iLineMaxWidth)
{
	return init(strBg, Label::create("", strFont, fFontSize), iLineMaxWidth);
}

bool ShowLabelInLineSequence::init(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth,unsigned uShowLineNum)
{
	if (!Layer::init())
	{
		return false;
	}
    m_uShowLineNum = uShowLineNum;
	m_pLabel = pLabel; 
	m_pLabel->setWidth(iLineMaxWidth);
	m_pLabel->setLineBreakWithoutSpace(true);
	m_pLabel->setAnchorPoint(Vec2(0.5, 1.0));
	float fLineHeight = m_pLabel->getLineHeight();
	m_pLabel->setPosition(iLineMaxWidth / 2, fLineHeight);

	auto szClip = Size(iLineMaxWidth, fLineHeight*m_uShowLineNum);
	Sprite *bg = nullptr;
	if (strBg.length() > 0)
	{
		bg = Sprite::create(strBg);
	}
	if (bg)
	{
		auto sz = bg->getContentSize();
		setContentSize(sz);
		bg->setPosition(sz.width / 2, sz.height / 2);
		addChild(bg);
	}
	else
	{
		setContentSize(szClip);
	}

	auto sz = getContentSize();
	m_pClip = ClippingNode::create();
	m_pClip->setContentSize(szClip);
	m_pClip->addChild(m_pLabel);

	m_pDrawNodeStencil = DrawNode::create();
	m_pDrawNodeStencil->setContentSize(szClip);
	Vec2 ptRect[4];
	ptRect[0] = Vec2(0, 0);
	ptRect[1] = Vec2(szClip.width, 0);
	ptRect[2] = Vec2(szClip.width, szClip.height);
	ptRect[3] = Vec2(0, szClip.height);
	m_pDrawNodeStencil->drawPolygon(ptRect, 4, Color4F::BLACK, 0, Color4F::BLACK);
	m_pDrawNodeStencil->retain();
	m_pClip->setStencil(m_pDrawNodeStencil);
	m_pClip->setPosition((sz.width - szClip.width) / 2, (sz.height - szClip.height) / 2);
	addChild(m_pClip);
    m_bVisible = false;

	return true;
}

void ShowLabelInLineSequence::ResetClipPosAndSz(int iLine)
{
    Size szClip;
    if (iLine == 1)
        szClip = Size(m_pLabel->getWidth(), m_pLabel->getLineHeight());
    else 
        szClip = Size(m_pLabel->getWidth(), m_pLabel->getLineHeight()*m_uShowLineNum);
    m_pClip->setContentSize(szClip);
    m_pDrawNodeStencil->setContentSize(szClip);
    Vec2 ptRect[4];
    auto sz = getContentSize();
    ptRect[0] = Vec2(0, 0);
    ptRect[1] = Vec2(szClip.width, 0);
    ptRect[2] = Vec2(szClip.width, szClip.height);
    ptRect[3] = Vec2(0, szClip.height);
    m_pDrawNodeStencil->drawPolygon(ptRect, 4, Color4F::BLACK, 0, Color4F::BLACK);
    m_pClip->setStencil(m_pDrawNodeStencil);
    m_pClip->setPosition((sz.width - szClip.width) / 2, (sz.height - szClip.height) / 2);
}

void ShowLabelInLineSequence::onScheduleChangeLine(float fDelta)
{
	int iAllLine = m_pLabel->getStringNumLines();
	if (m_iCurLine >= 0 && m_iCurLine < (iAllLine - 1))
	{
		m_pLabel->runAction(MoveBy::create(0.2f, Vec2(0, m_pLabel->getLineHeight())));
		++m_iCurLine;
	}
	else if (m_iCurLine == (iAllLine - 1))
	{
		unschedule(schedule_selector(ShowLabelInLineSequence::onScheduleChangeLine));
		m_iCurLine = -1;
		if (m_funLstLineFinishedCB)
		{
			m_funLstLineFinishedCB(this);
		}
	}
}

void ShowLabelInLineSequence::onScheduleChangeMultLine(float fDelta)
{
    int iAllLine = m_pLabel->getStringNumLines();
    if (m_iCurLine >= 0 && m_iCurLine < iAllLine - m_uShowLineNum && iAllLine > m_uShowLineNum)
    {
        m_pLabel->runAction(MoveBy::create(0.2f, Vec2(0, m_pLabel->getLineHeight())));
        ++m_iCurLine;
    }
    else if (m_iCurLine == iAllLine - m_uShowLineNum || 1 == iAllLine )
    {
        unschedule(schedule_selector(ShowLabelInLineSequence::onScheduleChangeMultLine));
        m_iCurLine = -1;
        if (m_funLstLineFinishedCB)
        {
            m_funLstLineFinishedCB(this);
        }
        m_bVisible = false;
    }
}

// static
TableTip *TableTip::create(const std::string& strBg, const std::string& strFont, int iFontSize, unsigned uMaxLineWidth)
{
	auto pRet = new TableTip;
	if (pRet->init(strBg, strFont, iFontSize, uMaxLineWidth))
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

bool TableTip::init(const std::string& strBg, const std::string& strFont, int iFontSize, unsigned uMaxLineWidth)
{
	if (!Layer::init())
	{
		return false;
	}

	m_pSpriteBG = Sprite::create(strBg);
	auto sz = m_pSpriteBG->getContentSize();
	m_pSpriteBG->setPosition(sz.width / 2, sz.height / 2);
	setContentSize(sz);
	
	m_pClipStencil = DrawNode::create();
	m_pClipStencil->setContentSize(sz);
	Vec2 ptRect[4];
	ptRect[0] = Vec2(0, 0);
	ptRect[1] = Vec2(sz.width, 0);
	ptRect[2] = Vec2(sz.width, sz.height);
	ptRect[3] = Vec2(0, sz.height);
	m_pClipStencil->drawPolygon(ptRect, 4, Color4F::BLACK, 0, Color4F::BLACK);
	m_pClipStencil->retain();

	m_pClip = ClippingNode::create(m_pClipStencil);
	m_pClip->setContentSize(sz);
	addChild(m_pClip);

	m_pClip->addChild(m_pSpriteBG);

	m_pLabel = Label::create("", strFont, iFontSize);
	m_pLabel->setString("0123456789abcdefghijklmnABCDEFGHIJKLMNOPQRSTUVWXYZ");
	auto szLabel = m_pLabel->getContentSize();
	m_fLabelFontH = szLabel.height;
	m_pLabel->setString("");
	m_pLabel->setAnchorPoint(Vec2(0.5, 1.0));
	m_pLabel->setMaxLineWidth(uMaxLineWidth);
	m_pLabel->setLineHeight(sz.height);
	m_pLabel->setLineBreakWithoutSpace(true);
	m_pLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	m_pLabel->setTextColor(Color4B::WHITE);
	m_pLabel->setVisible(false);
	m_pClip->addChild(m_pLabel);

	m_pSpriteUsrCharIcon = Sprite::create("tabletip/usricon.png");
	m_pSpriteUsrCharIcon->setVisible(false);
	m_pClip->addChild(m_pSpriteUsrCharIcon);

	m_pSpriteExpress = Sprite::create("tabletip/bgexpress.png");
	m_pSpriteExpress->setVisible(false);
	m_pClip->addChild(m_pSpriteExpress);




	m_biaoqingAni[0] = AnimationManager::creatArmature("putongbiaoqing");
	m_biaoqingAni[1] = AnimationManager::creatArmature("guzubiaoqing");
	//m_biaoqingAni[0]->setPosition(Vec2(UserViewW / 2, UserViewH / 2));
	m_biaoqingAni[0]->setVisible(false);
	//m_biaoqingAni[1]->setPosition(Vec2(UserViewW / 2, UserViewH / 2));
	m_biaoqingAni[1]->setVisible(false);

	m_pSpriteExpress->addChild(m_biaoqingAni[0]) ;
	m_pSpriteExpress->addChild(m_biaoqingAni[1]) ;

	return true;
}

void TableTip::UserChat(const std::string& strNickName, const std::string& strChat)
{
	m_pClip->stopAllActions();
	m_pClip->setVisible(false);
	m_pSpriteBG->stopAllActions();
//	m_pSpriteBG->setVisible(false);
	m_pLabel->stopAllActions();
	m_pLabel->setVisible(false);
	m_pSpriteUsrCharIcon->stopAllActions();
	m_pSpriteUsrCharIcon->setVisible(false);
	m_pSpriteExpress->stopAllActions();
	m_pSpriteExpress->setVisible(false);
	auto sz = m_pClip->getContentSize();
	m_pClip->runAction(Sequence::create(Place::create(Vec2(0, sz.height)), Show::create(), MoveBy::create(0.4f, Vec2(0, -sz.height)), nullptr));

	std::string str = strNickName;
	str += ":";
	str += strChat;
	SetLabelString(m_pLabel, str);
	m_pLabel->updateContent();

	int iLines = m_pLabel->getStringNumLines();
	auto szLabel = m_pLabel->getContentSize();
	auto szIcon = m_pSpriteUsrCharIcon->getContentSize();
	float fShowW = szLabel.width + szIcon.width;
	float fX = (sz.width - fShowW) / 2;
	m_pSpriteUsrCharIcon->setPosition(fX + szIcon.width / 2, sz.height / 2);
	m_pSpriteUsrCharIcon->setVisible(true);
	fX += szIcon.width;
	m_pLabel->setPosition(fX + szLabel.width / 2, sz.height / 2 + m_fLabelFontH / 2);
	m_pLabel->setVisible(true);

	if (iLines > 1)
	{
		m_pSpriteUsrCharIcon->runAction(Sequence::create(DelayTime::create(2.4f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr));
		auto actShowNextLine = Repeat::create(Sequence::create(DelayTime::create(2.4f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr), iLines - 1);
		m_pLabel->runAction(Sequence::create(actShowNextLine, DelayTime::create(2.4f), CallFunc::create([this, sz](){m_pClip->runAction(MoveBy::create(0.4f, Vec2(0, sz.height)));}), nullptr));
	}
	else
	{
		m_pClip->runAction(Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr)); 
	}
}

void TableTip::UserExpress(const std::string& strNickName, unsigned uExpressIdx)
{
	m_pClip->stopAllActions();
	m_pClip->setVisible(false);
	m_pSpriteBG->stopAllActions();
//	m_pSpriteBG->setVisible(false);
	m_pLabel->stopAllActions();
	m_pLabel->setVisible(false);
	m_pSpriteUsrCharIcon->stopAllActions();
	m_pSpriteUsrCharIcon->setVisible(false);
	m_pSpriteExpress->stopAllActions();
	m_pSpriteExpress->setVisible(false);
	auto sz = m_pClip->getContentSize();
	m_pClip->runAction(Sequence::create(Place::create(Vec2(0, sz.height)), Show::create(), MoveBy::create(0.4f, Vec2(0, -sz.height)), nullptr));

	std::string str = strNickName;
	str += ":";
	SetLabelString(m_pLabel, str);

//	auto sz = getContentSize();
	auto szLabel = m_pLabel->getContentSize();
	auto szIcon = m_pSpriteUsrCharIcon->getContentSize();
	auto szExpress = m_pSpriteExpress->getContentSize();
	float fShowW = szLabel.width + szIcon.width + 85/*szExpress.width*/;
	float fX = (sz.width - fShowW) / 2;
	m_pSpriteUsrCharIcon->setPosition(fX + szIcon.width / 2, sz.height / 2);
	m_pSpriteUsrCharIcon->setVisible(true);
	fX += szIcon.width;
	m_pLabel->setPosition(fX + szLabel.width / 2, sz.height / 2 + m_fLabelFontH / 2);
	m_pLabel->setVisible(true);
	fX += szLabel.width;
	m_pSpriteExpress->setPosition(fX + /*szExpress.width*/85 / 2, sz.height / 2);
	m_pSpriteExpress->setVisible(true);

	//auto ani = CreateAnimate(szFile, s_uXFrames[uExpressIdx], s_uYFrames[uExpressIdx], s_uAllFrames[uExpressIdx], s_fInterval[uExpressIdx]);
	m_pSpriteExpress->stopAllActions();
	m_pSpriteExpress->runAction(Sequence::create(Show::create(), nullptr));



	m_biaoqingAni[0]->stopAni();
	m_biaoqingAni[1]->stopAni();


	int putongAni[2][16]={
		{
			0,9,14,8,11,10,12,2,1,7,3,15,4,5,6,13
		},
		{
			2,14,12,10,1,9,4,3,5,8,11,13,15,7,0,6
			},
	} ;

	m_biaoqingAni[uExpressIdx/16]->setPositionX(m_biaoqingAni[uExpressIdx/16]->getContentSize().width/2+10) ;
	m_biaoqingAni[uExpressIdx/16]->setPositionY( sz.height / 2-10) ;
	m_biaoqingAni[uExpressIdx/16]->playOnceByIndex(putongAni[uExpressIdx/16][uExpressIdx%16]);


	m_pClip->runAction(Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr)); 




}

// 打赏提示 领取奖励 自动购买补充筹码提示 玩家离开 设置补充筹码成功提示 玩家站起旁观提示
void TableTip::SystemTip(const std::string& strTip)
{
	m_pClip->stopAllActions();
	m_pClip->setVisible(false);
	m_pSpriteBG->stopAllActions();
//	m_pSpriteBG->setVisible(false);
	m_pLabel->stopAllActions();
	m_pLabel->setVisible(false);
	m_pSpriteUsrCharIcon->stopAllActions();
	m_pSpriteUsrCharIcon->setVisible(false);
	m_pSpriteExpress->stopAllActions();
	m_pSpriteExpress->setVisible(false);
	auto sz = m_pClip->getContentSize();
	m_pClip->runAction(Sequence::create(Place::create(Vec2(0, sz.height)), Show::create(), MoveBy::create(0.4f, Vec2(0, -sz.height)), nullptr));

	SetLabelString(m_pLabel, strTip);

//	auto sz = getContentSize();
	auto szLabel = m_pLabel->getContentSize();
	m_pLabel->setPosition(sz.width / 2, sz.height / 2 + m_fLabelFontH / 2);
	m_pLabel->setVisible(true);
	int iLines = m_pLabel->getStringNumLines();
	if (iLines > 1)
	{
		auto actShowNextLine = Repeat::create(Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr), iLines - 1);
		m_pLabel->runAction(Sequence::create(actShowNextLine, DelayTime::create(2.0f), CallFunc::create([this, sz](){m_pClip->runAction(MoveBy::create(0.4f, Vec2(0, sz.height)));}), nullptr));
	}
	else
	{
		m_pClip->runAction(Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr)); 
	}
}

void TableTip::SystemTip(const std::string& strTip, float fDelay)
{
    m_pClip->stopAllActions();
    m_pClip->setVisible(false);
    m_pSpriteBG->stopAllActions();
    //	m_pSpriteBG->setVisible(false);
    m_pLabel->stopAllActions();
    m_pLabel->setVisible(false);
    m_pSpriteUsrCharIcon->stopAllActions();
    m_pSpriteUsrCharIcon->setVisible(false);
    m_pSpriteExpress->stopAllActions();
    m_pSpriteExpress->setVisible(false);
    auto sz = m_pClip->getContentSize();
    m_pClip->runAction(Sequence::create(Place::create(Vec2(0, sz.height)), Show::create(), MoveBy::create(0.4f, Vec2(0, -sz.height)), nullptr));

    SetLabelString(m_pLabel, strTip);

    //	auto sz = getContentSize();
    auto szLabel = m_pLabel->getContentSize();
    m_pLabel->setPosition(sz.width / 2, sz.height / 2 + m_fLabelFontH / 2);
    m_pLabel->setVisible(true);
    int iLines = m_pLabel->getStringNumLines();
    if (iLines > 1)
    {
        auto actShowNextLine = Repeat::create(Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr), iLines - 1);
        m_pLabel->runAction(Sequence::create(actShowNextLine, DelayTime::create(2.0f), CallFunc::create([this, sz](){m_pClip->runAction(MoveBy::create(0.4f, Vec2(0, sz.height)));}), nullptr));
    }
    else
    {
        m_pClip->runAction(Sequence::create(DelayTime::create(2.0f), MoveBy::create(0.4f, Vec2(0, sz.height)), nullptr)); 
    }
}