#include "TextTip.h"
#include "Util.h"
#include "GameCommon.h"
#include "GameData.h"
USING_NS_CC;

bool TextTip::init()
{
	if (!Layer::init())
    {
        return false;
    }

	setAnchorPoint(Vec2(0, 0));

	m_pSpriteBgTip = Sprite::create("bgtip.png");
	m_pSpriteBgTip->setPosition(503 / 2, 44 / 2);
	addChild(m_pSpriteBgTip);

	m_pLabelTip = CreateLabelMSYH(20, "", TextHAlignment::LEFT, TextVAlignment::CENTER); // Label::create("", "arial.ttf", 20);
	m_pLabelTip->setAnchorPoint(Vec2(0, 0));
	m_pLabelTip->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	m_pLabelTip->setTextColor(Color4B(255, 255, 255, 255));
	m_pLabelTip->setPosition(10, 15);
	m_pLabelTip->setContentSize(Size(503, 44));
	m_pLabelTip->setMaxLineWidth(503);
	addChild(m_pLabelTip);

	setVisible(false);

	return true;
}

void TextTip::AddTip(const std::string& strTip)
{
	if (m_deqStrTip.empty())
	{
		m_pLabelTip->setString(strTip);
		stopAllActions();
		setScale(1.0f);
		setVisible(false);
		runAction(Sequence::create(ScaleTo::create(0.01f, 1.0f, 0.1f, 1.0f), Show::create(), ScaleTo::create(0.1f, 1.0f), DelayTime::create(3.0f), CallFunc::create(std::bind(&TextTip::OnActionCheckNextTip, this)), nullptr)); // ::create(std::bind(&TextTip::OnActionCheckNextTip, this)), nullptr));
	}
	m_deqStrTip.push_back(strTip);
}

void TextTip::OnActionCheckNextTip()
{
	m_deqStrTip.pop_front();
	if (m_deqStrTip.empty())
	{
		runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f, 0.0f, 1.0f), Hide::create(), nullptr));
	}
	else
	{
		m_pLabelTip->setString(m_deqStrTip.front());
		runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create(std::bind(&TextTip::OnActionCheckNextTip, this)), nullptr));
	}
}

TextTip::TextTip()
{
}

TextTip::~TextTip()
{
}


bool TextTip2::init()
{
	if (!Layer::init())
    {
        return false;
    }

	setContentSize(Size(GameData::getInstance()->getGameWidth(), 40));
	setAnchorPoint(Vec2(0.5, 0.5));

	m_pBg = LayerColor::create(Color4B(0, 0, 0, 200));
	m_pBg->setContentSize(Size(GameData::getInstance()->getGameWidth(), 40));
	addChild(m_pBg);

	m_pLabelTip = CreateLabelMSYH(30,""); // Label::create("", "arial.ttf", 20);
	m_pLabelTip->setAnchorPoint(Vec2(0.5, 0.5));
	m_pLabelTip->setPosition(GameData::getInstance()->getGameWidth()/2, 20);
	m_pLabelTip->setContentSize(Size(GameData::getInstance()->getGameWidth(), 40));
	m_pLabelTip->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	m_pLabelTip->setTextColor(Color4B(255, 255, 255, 255));
	addChild(m_pLabelTip);

	setVisible(false);

	return true;
}

void TextTip2::AddTip(const std::string& strTip)
{
	if (m_deqStrTip.empty())
	{
		// m_pLabelTip->setString(strTip);
		SetLabelString(m_pLabelTip, strTip);
		stopAllActions();
		setScale(1.0f);
		setVisible(false);
		runAction(Sequence::create(ScaleTo::create(0.01f, 1.0f, 0.1f, 1.0f), Show::create(), ScaleTo::create(0.1f, 1.0f), DelayTime::create(3.0f), CallFunc::create(std::bind(&TextTip2::OnActionCheckNextTip, this)), nullptr)); // ::create(std::bind(&TextTip::OnActionCheckNextTip, this)), nullptr));
	}
	m_deqStrTip.push_back(strTip);
}

void TextTip2::AddTip(const std::string& strTip, float delayTime)
{
    if (m_deqStrTip.empty())
    {
        // m_pLabelTip->setString(strTip);
        SetLabelString(m_pLabelTip, strTip);
        stopAllActions();
        setScale(1.0f);
        setVisible(false);
        runAction(Sequence::create(ScaleTo::create(0.01f, 1.0f, 0.1f, 1.0f), Show::create(), ScaleTo::create(0.1f, 1.0f), DelayTime::create(delayTime), CallFunc::create(std::bind(&TextTip2::OnActionCheckNextTip, this)), nullptr)); // ::create(std::bind(&TextTip::OnActionCheckNextTip, this)), nullptr));

    }
    m_deqStrTip.push_back(strTip);
}

void TextTip2::OnActionCheckNextTip()
{
	m_deqStrTip.pop_front();
	if (m_deqStrTip.empty())
	{
		runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f, 0.0f, 1.0f), Hide::create(), nullptr));
	}
	else
	{
		m_pLabelTip->setString(m_deqStrTip.front());
		runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create(std::bind(&TextTip2::OnActionCheckNextTip, this)), nullptr));
	}
}

TextTip2::TextTip2()
{
}

TextTip2::~TextTip2()
{
}