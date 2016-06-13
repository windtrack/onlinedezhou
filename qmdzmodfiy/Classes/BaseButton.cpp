#include "BaseButton.h"
#include "Util.h"
#include "FrameSprite.h"

USING_NS_CC;
//////////////////////////////////////////////////////////////////////////////////////////////////
LableBackGround::LableBackGround(const std::string& str, const std::string& strFont, int iFontSize)
	: m_btViewIdx(BtViewNormal)
{	
	m_pLabel = CreateLabel(strFont,iFontSize, str);
	m_pLabel->retain();

	m_clr[0] = Color4B::WHITE;
	m_clr[1] = Color4B::BLACK;
	m_clr[2] = Color4B::GRAY;
}
LableBackGround::~LableBackGround()
{
	m_pLabel->release();
}

Node *LableBackGround::GetNode()
{
	return m_pLabel;
}

void LableBackGround::Activate(BtViewIdx btViewIdx)
{
	m_btViewIdx = btViewIdx;
	m_pLabel->setTextColor(m_clr[btViewIdx]);
}

BtViewIdx LableBackGround::Activated()
{
	return m_btViewIdx;
}

void LableBackGround::HitFinished()
{
}

void LableBackGround::SetStatusColor(cocos2d::Color4B clr[3])
{
	m_clr[0] = clr[0];
	m_clr[1] = clr[1];
	m_clr[2] = clr[2];
}
void LableBackGround::SetStatuColor(BtViewIdx btViewIdx, const cocos2d::Color4B& clr)
{
	m_clr[btViewIdx] = clr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
FrameSpriteBackGround::FrameSpriteBackGround(const std::string& strFrameSprite, bool bVert)
	: m_btViewIdx(BtViewNormal)
{
	if (bVert)
	{
		m_pFrameSprite = FrameSprite::create(strFrameSprite, 1, 3);
	}
	else
	{
		m_pFrameSprite = FrameSprite::create(strFrameSprite, 3, 1);
	}
	m_pFrameSprite->retain();
}

FrameSpriteBackGround::~FrameSpriteBackGround()
{
	m_pFrameSprite->release();
}

Node *FrameSpriteBackGround::GetNode()
{
	return m_pFrameSprite;
}

void FrameSpriteBackGround::Activate(BtViewIdx btViewIdx)
{
	m_btViewIdx = btViewIdx;
	m_pFrameSprite->SetFrameIdx(m_btViewIdx);
}

BtViewIdx FrameSpriteBackGround::Activated()
{
	return m_btViewIdx;
}

void FrameSpriteBackGround::HitFinished()
{
}

////////////////////////////////////////////////////////////////////////////////////////
AutoDelayEnabledFrameSpriteBackGround::AutoDelayEnabledFrameSpriteBackGround(const std::string& strFrameSprite, bool bVert, float fDelaySec, const Color4B& clrLeft, const Color4B& clrRight)
	: FrameSpriteBackGround(strFrameSprite, bVert)
	, m_fDelaySec(fDelaySec)
	, m_clrLeft(clrLeft)
	, m_clrRight(clrRight)
{
	m_pDrawNode = DrawNode::create();
	auto bg = GetNode();
	auto sz = bg->getContentSize();
	m_pDrawNode->setContentSize(sz);
	bg->addChild(m_pDrawNode);
}
AutoDelayEnabledFrameSpriteBackGround::~AutoDelayEnabledFrameSpriteBackGround()
{
}
void AutoDelayEnabledFrameSpriteBackGround::Activate(BtViewIdx btViewIdx)
{
	auto old = Activated();
	FrameSpriteBackGround::Activate(btViewIdx);
	//if (btViewIdx == BtViewNormal)
	//{
		GetNode()->stopAllActions();
		m_pDrawNode->clear();
	//}
}
void AutoDelayEnabledFrameSpriteBackGround::HitFinished()
{
	Activate(BtViewDisable);
	float fInterval = 0.1f;
	auto pNode = GetNode();
	auto sz = pNode->getContentSize();
	m_clockBeg = clock();
	m_clockAll = CLOCKS_PER_SEC * m_fDelaySec;
	GetNode()->runAction(RepeatForever::create(Sequence::create(DelayTime::create(fInterval), 
		CallFunc::create([this, sz](){
			clock_t clockCur = clock();
			clock_t clkInteval = clockCur - m_clockBeg;
			float fX = sz.width * clkInteval / m_clockAll;

			if (fX > sz.width)
			{
				Activate(BtViewNormal);
			}
			else
			{
				m_pDrawNode->clear();
				static Vec2 pt[4];
				pt[0].x = pt[0].y = 0;
				pt[1].x = fX;
				pt[1].y = 0;
				pt[2].x = fX;
				pt[2].y = sz.height;
				pt[3].x = 0;
				pt[3].y = sz.height;
				m_pDrawNode->drawPolygon(pt, 4, m_clrLeft, 0, m_clrLeft);
				pt[0].x = fX;
				pt[0].y = 0;
				pt[1].x = sz.width;
				pt[1].y = 0;
				pt[2].x = sz.width;
				pt[2].y = sz.height;
				pt[3].x = fX;
				pt[3].y = sz.height;
				m_pDrawNode->drawPolygon(pt, 4, m_clrRight, 0, m_clrRight);
			}
	}), nullptr)));
}
