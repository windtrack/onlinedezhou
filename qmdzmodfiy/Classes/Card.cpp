#include "Card.h"
#include "FrameSprite.h"

USING_NS_CC;

namespace{

	enum CardColor
	{
		Diamond,
		Club,
		Heart,
		Spade,
		Joker
	};

	inline CardColor GetCardColor(unsigned char uCardData)
	{
		const unsigned char uClrMask = 0xf0;
		return (CardColor)((uCardData & uClrMask) >> 4);
	}

	inline unsigned char GetCardValue(unsigned char uCardData)
	{
		const unsigned char uValueMask = 0x0f;
		return uCardData & uValueMask;
	}

	inline bool IsValidCard(CardColor cardColor, unsigned char uCardValue)
	{
		if ((cardColor < Joker && uCardValue > 0 && uCardValue < 14) ||
			(cardColor == Joker && uCardValue > 0 && uCardValue < 3))
		{
			return true;
		}
		return false;
	}

	inline bool IsValidCard(unsigned char uCardData)
	{
		return IsValidCard(GetCardColor(uCardData), GetCardValue(uCardData));
	}

	inline unsigned char MakeCardData(CardColor cardColor, unsigned char uCardValue)
	{
		if (IsValidCard(cardColor, uCardValue))
		{
			return (((unsigned char)(cardColor) << 4) | uCardValue);
		}
		return InvalidCard;
	}
}

unsigned GetCardFrameIdx(unsigned char cbCardData)
{
	if (!IsValidCard(cbCardData))
	{
		return 42;
	}
	unsigned char cbValue = GetCardValue(cbCardData);
	unsigned char cbColor = GetCardColor(cbCardData);

	return cbColor * 13 + cbValue - 1;
}

void ResetCardData(FrameSprite*pSpriteCard, unsigned char cbCardData)
{
	pSpriteCard->SetFrameIdx(GetCardFrameIdx(cbCardData));
}

void PlayAnimateOpenCard(FrameSprite *pCardSprite, unsigned char cbCardData, std::function<void()> funCBOpened)
{
	auto pActSetBack = CallFunc::create([pCardSprite](){ResetCardData(pCardSprite, InvalidCard);});
	auto pActShow = Show::create();
	auto pActRotate1 = RotateBy::create(0.15f, Vec3(0, -90, 0));
	auto pActSetValue = CallFunc::create([pCardSprite,cbCardData](){ResetCardData(pCardSprite, cbCardData); pCardSprite->setRotation3D(Vec3(0, -270, 0));});
	auto pActRotate2 = RotateBy::create(0.15f, Vec3(0, -90, 0));
	if (funCBOpened)
	{
		pCardSprite->runAction(Sequence::create(pActSetBack, pActShow, DelayTime::create(0.1f), pActRotate1, pActSetValue, pActRotate2, CallFunc::create(funCBOpened), nullptr));
	}
	else
	{
		pCardSprite->runAction(Sequence::create(pActSetBack, pActShow, DelayTime::create(0.1f), pActRotate1, pActSetValue, pActRotate2, nullptr));
	}	
}

void PlayAnimateOpenHandCard(FrameSprite *pCardSprite, unsigned char cbCardData, unsigned uCardIdx)
{
	static float s_fDegree[] = {-2.0f, 2.0f};
	float fDegree = 0; // s_fDegree[uCardIdx];
	auto pActSetBack = CallFunc::create([pCardSprite, fDegree](){ResetCardData(pCardSprite, InvalidCard); pCardSprite->setRotation3D(Vec3(0, 0, fDegree));});
	auto pActShow = Show::create();
	auto pActRotate1 = RotateBy::create(0.2f, Vec3(0, -90, fDegree));
	auto pActSetValue = CallFunc::create([pCardSprite,cbCardData, fDegree](){ResetCardData(pCardSprite, cbCardData); pCardSprite->setRotation3D(Vec3(0, -270, fDegree));});
	auto pActRotate2 = RotateBy::create(0.2f, Vec3(0, -90, fDegree));
	pCardSprite->runAction(Sequence::create(pActSetBack, pActShow, pActRotate1, pActSetValue, pActRotate2, nullptr));
}

/*
void ResetCardData(cocos2d::Sprite *pCardSprite, unsigned char cbCardData)
{
	Size szCard = pCardSprite->getContentSize();
	float fW = szCard.width;
	float fH = szCard.height;

	if (!IsValidCard(cbCardData))
	{
		cbCardData = InvalidCard;
	}
	unsigned char cbValue = GetCardValue(cbCardData);
	unsigned char cbColor = GetCardColor(cbCardData);

	pCardSprite->setTextureRect(Rect((cbValue - 1) * fW, cbColor * fH, fW, fH));
}

void PlayAnimateOpenCard(cocos2d::Sprite *pCardSprite, unsigned char cbCardData)
{
	auto pActSetBack = CallFunc::create([pCardSprite](){ResetCardData(pCardSprite, InvalidCard);});
	auto pActShow = Show::create();
	auto pActRotate1 = RotateBy::create(0.15f, Vec3(0, -90, 0));
	auto pActSetValue = CallFunc::create([pCardSprite,cbCardData](){ResetCardData(pCardSprite, cbCardData); pCardSprite->setRotation3D(Vec3(0, -270, 0));});
	auto pActRotate2 = RotateBy::create(0.15f, Vec3(0, -90, 0));
	pCardSprite->runAction(Sequence::create(pActSetBack, pActShow, DelayTime::create(0.1f), pActRotate1, pActSetValue, pActRotate2, nullptr));
}

void PlayAnimateOpenHandCard(cocos2d::Sprite *pCardSprite, unsigned char cbCardData, unsigned uCardIdx)
{
	static float s_fDegree[] = {-2.0f, 2.0f};
	float fDegree = s_fDegree[uCardIdx];
	auto pActSetBack = CallFunc::create([pCardSprite, fDegree](){ResetCardData(pCardSprite, InvalidCard); pCardSprite->setRotation3D(Vec3(0, 0, fDegree));});
	auto pActShow = Show::create();
	auto pActRotate1 = RotateBy::create(0.2f, Vec3(0, -90, fDegree));
	auto pActSetValue = CallFunc::create([pCardSprite,cbCardData, fDegree](){ResetCardData(pCardSprite, cbCardData); pCardSprite->setRotation3D(Vec3(0, -270, fDegree));});
	auto pActRotate2 = RotateBy::create(0.2f, Vec3(0, -90, fDegree));
	pCardSprite->runAction(Sequence::create(pActSetBack, pActShow, pActRotate1, pActSetValue, pActRotate2, nullptr));
}

CardFactory::CardFactory(const std::string& strCardTexture)
	: m_strCardTexture(strCardTexture)
	, m_fCardWidth(1)
	, m_fCardHeight(1) 
{
	auto pCard = Sprite::create(strCardTexture);
	if (pCard)
	{
		Size sz = pCard->getContentSize();
		m_fCardWidth = sz.width / 13;
		m_fCardHeight = sz.height / 5;
	}
}

cocos2d::Sprite *CardFactory::CreateCard(unsigned char uCardData)
{
	return Sprite::create(m_strCardTexture, CalcCardRect(uCardData));
}


cocos2d::Rect CardFactory::CalcCardRect(unsigned char cbCardData)
{
	if (!IsValidCard(cbCardData))
	{
		cbCardData = InvalidCard;
	}
	unsigned char cbValue = GetCardValue(cbCardData);
	unsigned char cbColor = GetCardColor(cbCardData);
	return cocos2d::Rect((cbValue - 1) * m_fCardWidth, cbColor * m_fCardHeight, m_fCardWidth, m_fCardHeight);
}
*/