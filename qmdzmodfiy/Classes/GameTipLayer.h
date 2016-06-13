#ifndef __GAMETIPLAYER_H__
#define __GAMETIPLAYER_H__

#include "cocos2d.h"
#include <functional>
#include "MenuItemFrameSprite.h"
#include "MenuItemScale9Sprite.h"
class TouchGrabber;

struct GameTipLytBt1Cfg
{
	static cocos2d::Color4B GetBgColor()
	{
		return cocos2d::Color4B(0, 0, 0, 128);
	}
	static std::string GetBgSprite()
	{
		return "userinfo/bgdialog0.png";
	}
	static unsigned GetButtonCount()
	{
		return 1;
	}
	static cocos2d::Vec2 GetButtonPos(unsigned uBtIdx)
	{
		static cocos2d::Vec2 s_ptBt[] = {
			cocos2d::Vec2(339, 57)
		};
		return s_ptBt[uBtIdx];
	}
	//static std::string GetButtonBg(unsigned uBtIdx)
	//{
	//	return "userinfo/btred.png";
	//}
	static float GetButtonFontSize(unsigned uBtIdx)
	{
		return 30;
	}
	static std::string GetButtonFont(unsigned uBtIdx)
	{
		return "fzltbd.ttf";
	}
	static cocos2d::Color4B GetButtonTextColor(unsigned uBtIdx)
	{
		return cocos2d::Color4B::WHITE;
	}
	static std::string GetContentFont()
	{
		return "fzlt.ttf";
	}
	static float GetContentFontSize()
	{
		return 30;
	}
	static cocos2d::Color4B GetContentTextColor()
	{
		return cocos2d::Color4B::WHITE;
	}
	static float GetContentTextMaxWidth()
	{
		return 510;
	}
	static cocos2d::Vec2 GetContentTextCenter()
	{
		return cocos2d::Vec2(339, 245);
	}
};

struct GameTipLytBt2Cfg
{
	static cocos2d::Color4B GetBgColor()
	{
		return cocos2d::Color4B(0, 0, 0, 128);
	}
	static std::string GetBgSprite()
	{
		return "userinfo/bgdialog0.png";
	}
	static unsigned GetButtonCount()
	{
		return 2;
	}
	static cocos2d::Vec2 GetButtonPos(unsigned uBtIdx)
	{
		static cocos2d::Vec2 s_ptBt[] = {
			cocos2d::Vec2(220, 57),
			cocos2d::Vec2(460, 57)
		};
		return s_ptBt[uBtIdx];
	}
	//static std::string GetButtonBg(unsigned uBtIdx)
	//{
	//	return "userinfo/btred.png";
	//}
	static float GetButtonFontSize(unsigned uBtIdx)
	{
		return 30;
	}
	static std::string GetButtonFont(unsigned uBtIdx)
	{
		return "fzltbd.ttf";
	}
	static cocos2d::Color4B GetButtonTextColor(unsigned uBtIdx)
	{
		return cocos2d::Color4B::WHITE;
	}
	static std::string GetContentFont()
	{
		return "fzlt.ttf";
	}
	static float GetContentFontSize()
	{
		return 30;
	}
	static cocos2d::Color4B GetContentTextColor()
	{
		return cocos2d::Color4B::WHITE;
	}
	static float GetContentTextMaxWidth()
	{
		return 510;
	}
	static cocos2d::Vec2 GetContentTextCenter()
	{
		return cocos2d::Vec2(339, 245);
	}
};

// 仅仅接受按钮的点击后隐藏，不支持返回键或者点击其他空白区域隐藏
class GameTipLayer : public cocos2d::LayerColor
{
public:
	template <typename LayoutCfg>
	static GameTipLayer *create()
	{
		auto pRet = new GameTipLayer;
		if (pRet && pRet->init<LayoutCfg>())
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

	void SetBtCB(std::function<void(unsigned)> cbBtHit);
	cocos2d::Label *GetContentLabel();
	cocos2d::Label *GetButtonLabel(unsigned uIdx);

	virtual void setVisible(bool bVisible) override;
protected:
	template <typename LayoutCfg>
	bool init()
	{
		if (!cocos2d::LayerColor::initWithColor(LayoutCfg::GetBgColor()))
		{
			return false;
		}
		auto sz = getContentSize();
		auto bgSprite = cocos2d::Sprite::create(LayoutCfg::GetBgSprite());
		bgSprite->setPosition(sz.width / 2, sz.height / 2);
		addChild(bgSprite);
		m_pLabelContent = cocos2d::Label::createWithTTF("", LayoutCfg::GetContentFont(), LayoutCfg::GetContentFontSize());
		m_pLabelContent->setTextColor(LayoutCfg::GetContentTextColor());
		m_pLabelContent->setMaxLineWidth(LayoutCfg::GetContentTextMaxWidth());
		m_pLabelContent->setPosition(LayoutCfg::GetContentTextCenter());
		bgSprite->addChild(m_pLabelContent);

		auto pMenu = cocos2d::Menu::create(nullptr);
		pMenu->setPosition(cocos2d::Vec2::ZERO);
		unsigned uBtCount = LayoutCfg::GetButtonCount();
		for (unsigned u = 0; u < uBtCount; ++u)
		{
			//auto menuItem = MenuItemFrameSprite::create(LayoutCfg::GetButtonBg(u), 3, CC_CALLBACK_1(GameTipLayer::onMenuItemSelect, this));
            auto menuItem = MenuItemScale9Sprite::create(0, Size(203, 70), CC_CALLBACK_1(GameTipLayer::onMenuItemSelect, this));
			auto szBt = menuItem->getContentSize();
			auto label = cocos2d::Label::createWithTTF("", LayoutCfg::GetButtonFont(u), LayoutCfg::GetButtonFontSize(u));
			label->setTextColor(LayoutCfg::GetButtonTextColor(u));
			label->setPosition(szBt.width / 2, szBt.height / 2);
			menuItem->addChild(label);
			menuItem->setPosition(LayoutCfg::GetButtonPos(u));
			pMenu->addChild(menuItem);
			m_vecBt.push_back(menuItem);
			m_vecLabelBt.push_back(label);
		}
		bgSprite->addChild(pMenu);
		m_pTouchGrabber = nullptr;
		return true;
	}

	virtual void onEnter() override;
	virtual void onExit() override;

	void onKeyEscPressed();
	void onMenuItemSelect(cocos2d::Ref*pMenuItem);

protected:
	TouchGrabber *m_pTouchGrabber;
	std::function<void(unsigned)> m_cbBtHit;
	cocos2d::Label *m_pLabelContent;
	std::vector<cocos2d::Label*> m_vecLabelBt;
	std::vector<MenuItemScale9Sprite*> m_vecBt;
};


#endif