#include "TextureNumber.h"
#include "Util.h"

USING_NS_CC;

bool TextureNumber::init(const std::string& strNumTexture, const std::string& strDotTexture, const std::string& strUnitTexture, bool bHasPreFlag)
{
	if (!Layer::init())
    {
        return false;
    }

	m_bNeedFormat = true;
	m_bHasPreFlag = bHasPreFlag;
	m_strNumTexture = strNumTexture;
	m_strDotTexture = strDotTexture;
	m_strUnitTexture = strUnitTexture;
	m_hAlign = TextureNumberHAlignment::Center;
	m_vAlign = TextureNumberVAlignment::Center;

	auto texture = Sprite::create(m_strNumTexture);
	auto sz = texture->getContentSize();
	m_fNumFrameW = sz.width / 12.0f;
	m_fNumFrameH = sz.height;

	texture = Sprite::create(m_strUnitTexture);
	sz = texture->getContentSize();
	m_fUnitFrameW = sz.width / 3.0f;
	m_fUnitFrameH = sz.height;

	texture = Sprite::create(m_strDotTexture);
	sz = texture->getContentSize();
	m_fDotFrameW = sz.width;
	m_fDotFrameH = sz.height;

	return true;
}

bool TextureNumber::init(const std::string& strNumTexture, bool bHasPreFlag)
{
	if (!Layer::init())
    {
        return false;
    }
	m_bNeedFormat = false;
	m_bHasPreFlag = bHasPreFlag;
	m_strNumTexture = strNumTexture;
	m_hAlign = TextureNumberHAlignment::Center;
	m_vAlign = TextureNumberVAlignment::Center;

	auto texture = Sprite::create(m_strNumTexture);
	auto sz = texture->getContentSize();
	m_fNumFrameW = sz.width / 12.0f;
	m_fNumFrameH = sz.height;
	return true;
}

TextureNumber *TextureNumber::create(const std::string& strNumTexture, const std::string& strDotTexture, const std::string& strUnitTexture, bool bHasPreFlag)
{
	TextureNumber *pRet = new TextureNumber();
	if (pRet && pRet->init(strNumTexture, strDotTexture, strUnitTexture, bHasPreFlag)) 
    { 
        pRet->autorelease(); 
        return pRet;
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
    } 
    return pRet; 
}

TextureNumber *TextureNumber::create(const std::string& strNumTexture, bool bHasPreFlag)
{
	TextureNumber *pRet = new TextureNumber();
	if (pRet && pRet->init(strNumTexture, bHasPreFlag)) 
    { 
        pRet->autorelease(); 
        return pRet;
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
    } 
    return pRet; 
}

void TextureNumber::SetHAlignment(TextureNumberHAlignment hAlign)
{
	if (m_hAlign != hAlign)
	{		
		Size sz = getContentSize();
		if ((m_hAlign == TextureNumberHAlignment::Center && hAlign == TextureNumberHAlignment::Left) || 
			(m_hAlign == TextureNumberHAlignment::Right && hAlign == TextureNumberHAlignment::Center))
		{
			float fHOffset = sz.width / 2;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x + fHOffset, pt.y);
			}
		}
		else if ((m_hAlign == TextureNumberHAlignment::Center && hAlign == TextureNumberHAlignment::Right) || 
			(m_hAlign == TextureNumberHAlignment::Left && hAlign == TextureNumberHAlignment::Center))
		{
			float fHOffset = sz.width / 2;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x - fHOffset, pt.y);
			}
		}
		else if (m_hAlign == TextureNumberHAlignment::Left && hAlign == TextureNumberHAlignment::Right)
		{
			float fHOffset = sz.width;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x - fHOffset, pt.y);
			}
		}
		else if (m_hAlign == TextureNumberHAlignment::Right && hAlign == TextureNumberHAlignment::Left)
		{
			float fHOffset = sz.width;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x + fHOffset, pt.y);
			}
		}
		m_hAlign = hAlign;
	}
}

void TextureNumber::SetVAlignment(TextureNumberVAlignment vAlign)
{
	if (m_vAlign != vAlign)
	{		
		Size sz = getContentSize();
		if ((m_vAlign == TextureNumberVAlignment::Center && vAlign == TextureNumberVAlignment::Bottom) || 
			(m_vAlign == TextureNumberVAlignment::Top && vAlign == TextureNumberVAlignment::Center))
		{
			float fOffset = sz.height / 2;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x, pt.y + fOffset);
			}
		}
		else if ((m_vAlign == TextureNumberVAlignment::Center && vAlign == TextureNumberVAlignment::Top) || 
			(m_vAlign == TextureNumberVAlignment::Bottom && vAlign == TextureNumberVAlignment::Center))
		{
			float fOffset = sz.height / 2;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x, pt.y - fOffset);
			}
		}
		else if (m_vAlign == TextureNumberVAlignment::Bottom && vAlign == TextureNumberVAlignment::Top)
		{
			float fOffset = sz.height;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x, pt.y - fOffset);
			}
		}
		else if (m_vAlign == TextureNumberVAlignment::Top && vAlign == TextureNumberVAlignment::Bottom)
		{
			float fOffset = sz.height;
			for (auto item : m_vecSpriteNumbers)
			{
				Vec2 pt = item->getPosition();
				item->setPosition(pt.x, pt.y + fOffset);
			}
		}
		m_vAlign = vAlign;
	}
}

void TextureNumber::UpdateNumber(double dNumber)
{
	if (fabs(m_dNumber - dNumber) >= 0.5f)
	{
		m_dNumber = dNumber;
		
		std::string strRet;
		if (m_bNeedFormat)
		{
			auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
				if (dValueFormating >= dLow && dValueFormating < dHigh)
				{
					double dHigh = dValueFormating / dLow;
					double dRest = dValueFormating - dLow * unsigned(dHigh);
					strRet = do_fraction(dHigh, (dRest >= dLow / 100.0f) ? 3 : 0);
					strRet += strUnit;
					return true;
				}
				return false;
			};
			if (pFunFormat(dNumber, 10000.0f, 100000000.0f, "W", strRet))
			{
			}
			else if (pFunFormat(dNumber, 100000000.0f, 1000000000000.0f, "Y", strRet))
			{
			}
			else if (pFunFormat(dNumber, 1000000000000.0f, 10000000000000000.0f, "Z", strRet))
			{
			}
			else
			{
				strRet = do_fraction(dNumber);
			}
		}
		else
		{
			strRet = do_fraction(dNumber);
		}


		std::vector<unsigned> vecIdxs;
		if (m_bHasPreFlag)
		{
			if (m_dNumber > 0)
			{
				vecIdxs.push_back(10);
			}
			else if (m_dNumber < 0)
			{
				vecIdxs.push_back(11);
			}
		}
		for (unsigned i = 0; i < strRet.size(); ++i)
		{
			char c = strRet[i];
			if (c >= '0' && c <= '9')
			{
				vecIdxs.push_back(c - '0');
			}
			else if (c == '.')
			{
				vecIdxs.push_back(12);
			}
			else if (c == 'W') // 万
			{				
				vecIdxs.push_back(13);
			}
			else if (c == 'Y') // 亿
			{
				vecIdxs.push_back(14);
			}
			else if (c == 'Z') // 兆
			{
				vecIdxs.push_back(15);
			}
			else
			{
				continue;
			}
		}

		m_vecNumbers = vecIdxs;
		ReAdjustFrame();
	}
}

void TextureNumber::UpdateZeroNumber(double dNumber, int size)
{
	if (fabs(m_dNumber - dNumber) >= 0.5f)
	{
		m_dNumber = dNumber;

		std::string strRet;
		if (m_bNeedFormat)
		{
			auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
				if (dValueFormating >= dLow && dValueFormating < dHigh)
				{
					double dHigh = dValueFormating / dLow;
					double dRest = dValueFormating - dLow * unsigned(dHigh);
					strRet = do_fraction(dHigh, (dRest >= dLow / 100.0f) ? 3 : 0);
					strRet += strUnit;
					return true;
				}
				return false;
			};
			if (pFunFormat(dNumber, 10000.0f, 100000000.0f, "W", strRet))
			{
			}
			else if (pFunFormat(dNumber, 100000000.0f, 1000000000000.0f, "Y", strRet))
			{
			}
			else if (pFunFormat(dNumber, 1000000000000.0f, 10000000000000000.0f, "Z", strRet))
			{
			}
			else
			{
				strRet = do_fraction(dNumber);
			}
		}
		else
		{
			strRet = do_fraction(dNumber);
		}

		size_t len = strRet.length();
		if(strRet.length() < size)
		{
			for(int index =(int) 0; index + len < size; index++)
				strRet.insert(index, "0");
		}
		std::vector<unsigned> vecIdxs;
		if (m_bHasPreFlag)
		{
			if (m_dNumber > 0)
			{
				vecIdxs.push_back(10);
			}
			else if (m_dNumber < 0)
			{
				vecIdxs.push_back(11);
			}
		}
		for (unsigned i = 0; i < strRet.size(); ++i)
		{
			char c = strRet[i];
			if (c >= '0' && c <= '9')
			{
				vecIdxs.push_back(c - '0');
			}
			else if (c == '.')
			{
				vecIdxs.push_back(12);
			}
			else if (c == 'W') // 万
			{				
				vecIdxs.push_back(13);
			}
			else if (c == 'Y') // 亿
			{
				vecIdxs.push_back(14);
			}
			else if (c == 'Z') // 兆
			{
				vecIdxs.push_back(15);
			}
			else
			{
				continue;
			}
		}

		m_vecNumbers = vecIdxs;
		ReAdjustFrame();
	}
}

void TextureNumber::ReAdjustFrame()
{
	for (auto item : m_vecSpriteNumbers)
	{
		removeChild(item);
	}
	m_vecSpriteNumbers.clear();

	float fAllW = 0;
	float fAllH = m_fNumFrameH;

	for (auto item : m_vecNumbers)
	{
		if (item < 12)
		{
			fAllW += m_fNumFrameW;
		}
		else if (item == 12)
		{
			fAllW += m_fDotFrameW;
		}
		else
		{
			fAllW += m_fUnitFrameW;
		}
	}
	setContentSize(Size(fAllW, fAllH));


	float fX = 0, fY = 0;
	if (m_hAlign == TextureNumberHAlignment::Left)
	{
	}
	else if (m_hAlign == TextureNumberHAlignment::Center)
	{
		fX -= (fAllW / 2);
	}
	else
	{
		fX -= fAllW;
	}

	if (m_vAlign == TextureNumberVAlignment::Top)
	{
		fY -= (fAllH / 2);
	}
	else if (m_vAlign == TextureNumberVAlignment::Center)
	{
	}
	else
	{
		fY += (fAllH / 2);
	}

	for (int i = 0; i < m_vecNumbers.size(); ++i)
	{
		unsigned uNum = m_vecNumbers[i];
		Sprite *pSprite = nullptr;
		double dW = 0;
		if (uNum < 12)
		{
			pSprite = Sprite::create(m_strNumTexture, Rect(m_fNumFrameW * uNum, 0, m_fNumFrameW, m_fNumFrameH));
			dW = m_fNumFrameW;
		}
		else if (uNum == 12)
		{
			pSprite = Sprite::create(m_strDotTexture);
			dW = m_fDotFrameW;
		}
		else
		{
			uNum -= 13;
			pSprite = Sprite::create(m_strUnitTexture, Rect(m_fUnitFrameW * uNum, 0, m_fUnitFrameW, m_fUnitFrameH));
			dW = m_fUnitFrameW;
		}		
		pSprite->setPosition(fX + dW / 2, fY);
		fX += dW;
		addChild(pSprite);
		m_vecSpriteNumbers.push_back(pSprite);
	}
}