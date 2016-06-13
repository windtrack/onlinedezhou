#include "LineLabel.h"
#include "Util.h"

USING_NS_CC;

void LineLabel::SetVAlignment(TextVAlignment vAlignment)
{
	if (m_vAlignment != vAlignment)
	{
		m_vAlignment = vAlignment;
		auto szBound = getContentSize();
		for (auto label : m_vecLabels)
		{
			ResetLablePosY(szBound.height, label);
		}
	}
}

void LineLabel::Appand(int iFontSize, const std::string& strFontName, const std::string& str, const cocos2d::Color4B& clr)
{
	Label *pLabel = CreateLabel(strFontName, iFontSize, str);
	if (pLabel)
	{
		pLabel->setTextColor(clr);
		bool bAdjustH = false;
		float fOff = 0;
		auto szLabel = pLabel->getContentSize();
		auto szBound = getContentSize();
		if (szBound.height < szLabel.height)
		{
			float fOff = (szLabel.height - szBound.height) / 2;
			for (auto item : m_vecLabels)
			{
				item->setPositionY(item->getPositionY() + fOff);
			}
			szBound.height = szLabel.height;	
		}
		szBound.width += szLabel.width;
		setContentSize(szBound);

		pLabel->setPositionX(szBound.width - szLabel.width / 2);
		ResetLablePosY(szBound.height, pLabel);
		m_vecLabels.push_back(pLabel);
		addChild(pLabel);
	}
}

void LineLabel::resetLable()
{
	this->removeAllChildren() ;
	this->setContentSize(Size(0,this->getContentSize().height)) ;
}
bool LineLabel::init()
{
	if (!Layer::init())
	{
		return false;
	}
	ignoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5, 0.5));
	setContentSize(Size(0, 0));
	m_vAlignment = TextVAlignment::CENTER;
	return true;
}

void LineLabel::ResetLablePosY(float fBoundH, cocos2d::Label *pLabel)
{
	auto szLabel = pLabel->getContentSize();
	if (m_vAlignment == TextVAlignment::TOP)
	{
		pLabel->setPositionY(fBoundH - szLabel.height / 2);
	}
	else if (m_vAlignment == TextVAlignment::CENTER)
	{
		pLabel->setPositionY(fBoundH / 2);
	}
	else if (m_vAlignment == TextVAlignment::BOTTOM)
	{
		pLabel->setPositionY(szLabel.height / 2);
	}
}