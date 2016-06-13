#include "MenuItemSpriteWithLabel.h"
#include "FrameSprite.h"

USING_NS_CC;

bool MenuItemSpriteWithLabel::init(const std::string& strImage, unsigned uFrames, const ccMenuCallback& callback)
{
	auto pFrameSprite = FrameSprite::create(strImage, 1, uFrames);
	if (pFrameSprite)
	{
		//std::vector<Sprite*> vecPSprite;
		for (auto i = 0; i < uFrames; i++)
		{
			vecPSprite.push_back(pFrameSprite->GetSprite(i));
		}
		while (vecPSprite.size() < 3)
		{
			vecPSprite.push_back(pFrameSprite->GetSprite(uFrames - 1));
		}
		return MenuItemSprite::initWithNormalSprite(vecPSprite[0], vecPSprite[1], vecPSprite[2], callback);

	}
	return false;
}

MenuItemSpriteWithLabel *MenuItemSpriteWithLabel::create(const std::string& strImage, unsigned uFrames, const ccMenuCallback& callback)
{
	auto pRet = new MenuItemSpriteWithLabel();
	if (pRet->init(strImage, uFrames, callback))
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

void MenuItemSpriteWithLabel::setButtonSize( Size contentSize)
{
	for (int i=0;i<vecPSprite.size();i++)
	{
		Sprite* sprite = vecPSprite.at(i) ;
		Size size = sprite->getContentSize() ;
		sprite->setScale(contentSize.width/size.width,contentSize.height/size.height) ;
		sprite->setContentSize(contentSize);
	}
	this->setContentSize(contentSize);
}

void MenuItemSpriteWithLabel::SetDisabledColor(const cocos2d::Color4B& color)
{
	if (m_disabledColor != color)
	{
		m_disabledColor = color;
		if (!isEnabled() && m_pLabel)
		{
			m_pLabel->setTextColor(color);
		}
	}
}

void MenuItemSpriteWithLabel::SetEnabledColor(const cocos2d::Color4B& color)
{
	if (m_enabledColor != color)
	{
		m_enabledColor = color;
		if (isEnabled() && m_pLabel)
		{
			m_pLabel->setTextColor(color);
		}
	}
}

void MenuItemSpriteWithLabel::SetLabel(cocos2d::Label* pLabel)
{
	if (m_pLabel != pLabel)
	{
		if (pLabel)
		{
			pLabel->setTextColor(isEnabled() ? m_enabledColor : m_disabledColor);
			auto sz = getContentSize();
			pLabel->setPosition(sz.width / 2, sz.height / 2);
			pLabel->setHorizontalAlignment(TextHAlignment::CENTER);
			pLabel->setVerticalAlignment(TextVAlignment::CENTER);
			addChild(pLabel, 0);
		}

		if (m_pLabel)
		{
			removeChild(m_pLabel);
		}

		m_pLabel = pLabel;
	}
}

cocos2d::Label *MenuItemSpriteWithLabel::GetLabel()
{
	return m_pLabel;
}

void MenuItemSpriteWithLabel::selected()
{
	MenuItemSprite::selected();
}

void MenuItemSpriteWithLabel::unselected()
{
	MenuItemSprite::unselected();
	
}

void MenuItemSpriteWithLabel::setEnabled(bool bEnabled)
{
	if( _enabled != bEnabled ) 
    {
		MenuItemSprite::setEnabled(bEnabled);
		if (m_pLabel)
		{
			m_pLabel->setTextColor(_enabled ? m_enabledColor : m_disabledColor);
		}
	}
}

MenuItemSpriteWithLabel::MenuItemSpriteWithLabel()
	: m_pLabel(nullptr)
	, m_enabledColor(Color4B::WHITE)
	, m_disabledColor(Color4B::GRAY)
{
}