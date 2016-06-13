#include "MenuItemScale9SpriteWithLabel.h"
#include "FrameSprite.h"
#include "MenuButtonFilePath.h"
USING_NS_CC;






MenuItemScale9SpriteWithLabel *MenuItemScale9SpriteWithLabel::create(const std::string& strImage, const std::string& strSImage,const std::string& strEImage,
																	 Rect rectOut,
																	 Rect rectIn,
																	 cocos2d::Size contentSize, const ccMenuCallback& callback)
{
	auto pRet = new MenuItemScale9SpriteWithLabel();
	if (pRet->init(strImage,strSImage, strEImage,rectOut,rectIn,contentSize,callback))
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
/************************************************************************/
/* Ë÷Òý´´½¨                                                                     */
/************************************************************************/
MenuItemScale9SpriteWithLabel *MenuItemScale9SpriteWithLabel::create(unsigned index,cocos2d::Size contentSize,const cocos2d::ccMenuCallback& callback)
{
	auto pRet = new MenuItemScale9SpriteWithLabel();
	if (pRet->init(sButtonPath[index][0],sButtonPath[index][1], sButtonPath[index][2],sBtRect[index][0],sBtRect[index][1],contentSize,callback))
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



bool MenuItemScale9SpriteWithLabel::init(const std::string& strImage,const std::string& strSImage,const std::string& strEImage,
										 Rect rectOut,
										 Rect rectIn,
										 cocos2d::Size contentSize,const ccMenuCallback& callback)
{
	


	Scale9Sprite *vecPSprite0 = Scale9Sprite::create(strImage,rectOut,rectIn);
	Scale9Sprite *vecPSprite1  = Scale9Sprite::create(strSImage,rectOut,rectIn);
	Scale9Sprite *vecPSprite2  = Scale9Sprite::create(strEImage,rectOut,rectIn);


	vecPSprite0->setPreferredSize(contentSize) ;
	vecPSprite1->setPreferredSize(contentSize) ;
	vecPSprite2->setPreferredSize(contentSize) ;

	vecPSprite.push_back(vecPSprite0);
	vecPSprite.push_back(vecPSprite1);
	vecPSprite.push_back(vecPSprite2);
	
	return MenuItemSprite::initWithNormalSprite(vecPSprite0, vecPSprite1, vecPSprite2, callback);

}



void MenuItemScale9SpriteWithLabel::setButtonSize( Size contentSize)
{
	for (int i=0;i<vecPSprite.size();i++)
	{
		Scale9Sprite* sprite = vecPSprite.at(i) ;
		sprite->setPreferredSize(contentSize);
	}
	this->setContentSize(contentSize);
}

void MenuItemScale9SpriteWithLabel::SetDisabledColor(const cocos2d::Color4B& color)
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

void MenuItemScale9SpriteWithLabel::SetEnabledColor(const cocos2d::Color4B& color)
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

void MenuItemScale9SpriteWithLabel::SetLabel(cocos2d::Label* pLabel)
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

cocos2d::Label *MenuItemScale9SpriteWithLabel::GetLabel()
{
	return m_pLabel;
}

void MenuItemScale9SpriteWithLabel::selected()
{
	MenuItemSprite::selected();
}

void MenuItemScale9SpriteWithLabel::unselected()
{
	MenuItemSprite::unselected();

}

void MenuItemScale9SpriteWithLabel::setEnabled(bool bEnabled)
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

MenuItemScale9SpriteWithLabel::MenuItemScale9SpriteWithLabel()
	: m_pLabel(nullptr)
	, m_enabledColor(Color4B::WHITE)
	, m_disabledColor(Color4B::GRAY)
{
}
