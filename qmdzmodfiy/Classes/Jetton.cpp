#include "Jetton.h"
#include "Util.h"

USING_NS_CC;

void ResetJetton(cocos2d::Sprite *pJetton, unsigned uJettonIdx)
{
	Size sz = pJetton->getContentSize();
	pJetton->setTextureRect(Rect(uJettonIdx * sz.width, 0, sz.width, sz.height));
}

JettonFactory::JettonFactory(const std::string& strJettonTexture /*= "jetton.png"*/)
	: m_strTexture(strJettonTexture)
{
	auto pSprite = Sprite::create(m_strTexture);
	if (pSprite)
	{
		Size sz = pSprite->getContentSize();
		m_fItemWidth = sz.width / 10;
		m_fItemHeight = sz.height;
	}
}
cocos2d::Sprite *JettonFactory::CreateJetton(unsigned uJettonIdx)
{
	return Sprite::create(m_strTexture, Rect(uJettonIdx *m_fItemWidth, 0, m_fItemWidth, m_fItemHeight));
}

void JettonFactory::ResetJetton(cocos2d::Sprite *pJetton, unsigned uJettonIdx)
{
	pJetton->setTextureRect(Rect(uJettonIdx *m_fItemWidth, 0, m_fItemWidth, m_fItemHeight));
}

bool SidePot::init(FuncCalcJettonIdx funcCalcJettonIdx)
{
	if (!Layer::init())
	{
		return false;
	}
	m_funcCalcJettonIdx = funcCalcJettonIdx;
	m_pJettonFactory = new JettonFactory;
	m_pSpriteBgJettonPot = Sprite::create("bgjettonpot.png");
	Size sz = m_pSpriteBgJettonPot->getContentSize();
	setContentSize(sz);
	m_pSpriteBgJettonPot->setPosition(sz.width / 2, sz.height / 2);
	addChild(m_pSpriteBgJettonPot);

	m_pLabelPotValue = CreateLabelMSYH(20);
	m_pLabelPotValue->setPosition(70, sz.height / 2);
	m_pLabelPotValue->setTextColor(Color4B(255, 255, 0, 255));
	addChild(m_pLabelPotValue);

	return true;
}
SidePot *SidePot::create(FuncCalcJettonIdx funcCalcJettonIdx)
{
	auto pRet = new SidePot;
	if (pRet->init(funcCalcJettonIdx))
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
double SidePot::GetPotValue() const
{
	return m_dPotValue;
}

void SidePot::UpdatePotValue(double dPotValue)
{
	m_dPotValue = dPotValue;
	std::vector<int> vecJettonIdx;
	m_funcCalcJettonIdx(dPotValue, vecJettonIdx);
	if (vecJettonIdx.empty())
	{
		for (auto pJetton : m_vecPJetton)
		{
			pJetton->setVisible(false);
		}
		SetLabelString(m_pLabelPotValue, "");
		setVisible(false);
	}
	else
	{
		while (m_vecPJetton.size() < vecJettonIdx.size())
		{
			auto pJetton = m_pJettonFactory->CreateJetton(0);
			pJetton->setPosition(15, 13 + m_vecPJetton.size() * 1.3);
			addChild(pJetton);
			m_vecPJetton.push_back(pJetton);
		}

		for (unsigned i = 0; i < m_vecPJetton.size(); ++i)
		{
			if (i < vecJettonIdx.size())
			{
				m_pJettonFactory->ResetJetton(m_vecPJetton[i], vecJettonIdx[i]);
				m_vecPJetton[i]->setVisible(true);
			}
			else
			{
				m_vecPJetton[i]->setVisible(false);
			}
		}
		
		SetLabelString(m_pLabelPotValue, FormatCash(dPotValue));
		setVisible(true);
	}
}

SidePot::SidePot()
	: m_pJettonFactory(nullptr)
	, m_dPotValue(0)
{
}

SidePot::~SidePot()
{
	delete m_pJettonFactory;
	m_pJettonFactory = nullptr;
}