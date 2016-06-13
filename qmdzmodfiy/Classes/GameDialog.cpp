#include "GameDialog.h"
#include "GrabTouch.h"
#include "Util.h"
#include "GrabKeyboard.h"
#include "MenuItemSpriteWithLabel.h"
#include "GameCommon.h"
#include "Tools.h"
USING_NS_CC;

static std::map<GameDialog*, int> m_allDialogs;
static bool g_IsOpenDialogSound = false;

GameDialog *GameDialog::create(const std::string& strBgDialog, bool bHasClose)
{
	auto pRet = new GameDialog;
	if (pRet->init(strBgDialog, bHasClose))
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
GameDialog *GameDialog::create(unsigned uDialogW, unsigned uDialogH, bool bHasClose)
{
	auto pRet = new GameDialog;
	if (pRet->init(uDialogW, uDialogH, bHasClose))
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
void GameDialog::setVisible(bool bVisible)
{
	if (bVisible) {
		m_allDialogs[this] = 1;
	} else {
		m_allDialogs.erase(this);
	}
	bool isVisible = this->isVisible();
	if (isVisible != bVisible)
	{
		bool bRunning = isRunning();
		if (bRunning)
		{
			if (m_pTouchGrabberDialog)
			{
				m_pTouchGrabberDialog->SetTouchEnable(bVisible);
			}
			if (bVisible)
			{
				RegistEscPressCb(this, std::bind(&GameDialog::onKeyEscPressed, this));
			}
			else
			{
				UnRegistEscPressCb(this);
			}
		}
		onSetVisible(bVisible);
		Layer::setVisible(bVisible);

		if (bRunning)
		{
			if (m_cbSetVisible)
			{
				m_cbSetVisible(bVisible);
			}			
		}
	}
}


void GameDialog::setOnVisible(bool bVisible)
{
	bool isVisible = this->isVisible();
	if (isVisible != bVisible)
	{
		bool bRunning = isRunning();
		if (bRunning)
		{
			if (m_pTouchGrabberDialog)
			{
				m_pTouchGrabberDialog->SetTouchEnable(bVisible);
			}
			if (bVisible)
			{
				RegistEscPressCb(this, std::bind(&GameDialog::onKeyEscPressed, this));
			}
			else
			{
				UnRegistEscPressCb(this);
			}
		}
		onSetVisible(bVisible);
		Layer::setVisible(bVisible);
		if (bRunning)
		{
			if (m_cbSetVisible)
			{
				m_cbSetVisible(bVisible);
			}			
		}
	}
}

void GameDialog::SetVisibleCB(std::function<void(bool)> cbSetVisible)
{
	m_cbSetVisible = cbSetVisible;
}

bool GameDialog::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_pTouchGrabberDialog = nullptr;

	return true ;
}

bool GameDialog::init(const std::string& strBgDialog, bool bHasClose)
{
	if (!Layer::init())
	{
		return false;
	}
	auto bg = Sprite::create(strBgDialog);
	auto sz = bg->getContentSize();
	bg->setPosition(sz.width / 2, sz.height / 2);
	setContentSize(sz);
	addChild(bg);
	m_pTouchGrabberDialog = nullptr;





	if (bHasClose)
	{
		m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(GameDialog::onMenuItemCloseSelected, this));
		m_btMenuItemClose->setPosition(sz.width - 25, sz.height - 38);
		auto menu = Menu::create(m_btMenuItemClose, nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu, 100);
	}
	return true;
}
bool GameDialog::init(unsigned uDialogW, unsigned uDialogH, bool bHasClose)
{
	if (!Layer::init())
	{
		return false;
	}
	setContentSize(Size(uDialogW, uDialogH));
	m_pTouchGrabberDialog = nullptr;

	if (bHasClose)
	{
		m_btMenuItemClose = MenuItemSpriteWithLabel::create("dialog/btclose.png", 3, CC_CALLBACK_1(GameDialog::onMenuItemCloseSelected, this));
		m_btMenuItemClose->setPosition(uDialogW - 15, uDialogH - 18);
		auto menu = Menu::create(m_btMenuItemClose, nullptr);
		menu->setPosition(Vec2::ZERO);
		addChild(menu, 100);
	}
	return true;
}
void GameDialog::onMenuItemCloseSelected(cocos2d::Ref *)
{
	setVisible(false);
}

void GameDialog::setTouchEnabled(bool value)
{
	if (m_pTouchGrabberDialog!=nullptr)
	{
		m_pTouchGrabberDialog->SetTouchEnable(value);
	}
}

void GameDialog::onEnter()
{
	Layer::onEnter();
	bool bVisible = isVisible();
	if (bVisible)
	{
		RegistEscPressCb(this, std::bind(&GameDialog::onKeyEscPressed, this));
	}
	//m_pTouchGrabberDialog = nullptr;
	if (m_pTouchGrabberDialog == nullptr)
	{
		auto touchGrabber = TouchGrabberEx<DTouchDialogProcer>::create(this, true);
		touchGrabber->GetTouchProcer().SetAlwaysRetHit(true);
		touchGrabber->GetTouchProcer().SetCB(std::bind(&GameDialog::onGrabTouchBegan, this, std::placeholders::_1));
		m_pTouchGrabberDialog = touchGrabber;
		m_pTouchGrabberDialog->retain();
	}
	m_pTouchGrabberDialog->SetTouchEnable(bVisible);


}
void GameDialog::onExit()
{
	if (m_pTouchGrabberDialog)
	{
		m_pTouchGrabberDialog->SetTouchEnable(false);
		m_pTouchGrabberDialog->release();
		m_pTouchGrabberDialog = nullptr;
	}

	UnRegistEscPressCb(this);
	Layer::onExit();
}
void GameDialog::onGrabTouchBegan(bool bHitDialog)
{
	if (bHitDialog)
	{

	}
	else
	{
		setVisible(false);
	}
}
void GameDialog::onSetVisible(bool bVisible)
{
	//	PlayVisibleSound(bVisible);
}
void GameDialog::onKeyEscPressed()
{
	setVisible(false);
}

void GameDialog::callShowAction()
{
	GameDialog::setVisible(true) ;
	this->setScale(0.85f) ;
	ActionInterval* scaleTo = ScaleTo::create(0.3f,1.0f) ;
	CCActionInterval * easeBounceOut = CCEaseBackOut ::create(scaleTo);
	this->setCascadeOpacityEnabled(true) ;
	this->setOpacity(0);
	ActionInterval * fadein = FadeTo::create(0.3f,255) ;
	this->runAction(Spawn::create(easeBounceOut,fadein,NULL)) ;
}

void GameDialog::callHideAction()
{
	
	if (!this->isVisible())
	{
		return ;
	}

	ActionInterval* scaleTo = ScaleTo::create(0.2f,0.93f) ;
	CCActionInterval * easeBounceOut = CCEaseBackIn ::create(scaleTo);
	this->setCascadeOpacityEnabled(true) ;
	this->setOpacity(255);
	ActionInterval * fadeout = FadeTo::create(0.2f,80) ;

	//auto fun = funcation ;
	auto callback = [this](){
		GameDialog::setVisible(false) ; ;
	};

	this->runAction(Sequence::create(Spawn::create(scaleTo,fadeout,NULL),CallFunc::create(callback),NULL)) ;
}

void GameDialog::hideLayer()
{
	GameDialog::setVisible(false) ;
}

void GameDialog::PlayVisibleSound(bool bVisible)
{
	if (bVisible)
	{
		PlayEffectSound("sound/dialog.mp3");
        g_IsOpenDialogSound = true;
	}
	else
	{
        if (g_IsOpenDialogSound == false)
            return;
		PlayEffectSound("sound/return.mp3");
        g_IsOpenDialogSound = false;
	}
}

MenuItemSpriteWithLabel* GameDialog::GetMenuItemClose()
{
	if (m_btMenuItemClose != nullptr)
	{
		return m_btMenuItemClose;
	}
	else
	{
		return nullptr;
	}
}

GameLayer *GameLayer::create(const std::string& strBgLayer, bool bTouchCoverAll, bool bGrabEscKey)
{
	auto ret = new GameLayer;
	if (ret->init(strBgLayer, bTouchCoverAll))
	{
		ret->autorelease();
	}
	else
	{
		delete ret;
		ret = nullptr;
	}
	return ret;
}

GameLayer *GameLayer::create(unsigned uDialogW, unsigned uDialogH, bool bTouchCoverAll, bool bGrabEscKey)
{
	auto ret = new GameLayer;
	if (ret->init(uDialogW, uDialogH, bTouchCoverAll, bGrabEscKey))
	{
		ret->autorelease();
	}
	else
	{
		delete ret;
		ret = nullptr;
	}
	return ret;
}

GameLayer *GameLayer::create(unsigned uDialogW, unsigned uDialogH, const cocos2d::Color4B& clrBgLayer, bool bTouchCoverAll, bool bGrabEscKey)
{
	auto ret = new GameLayer;
	if (ret->init(uDialogW, uDialogH, clrBgLayer, bTouchCoverAll, bGrabEscKey))
	{
		ret->autorelease();
	}
	else
	{
		delete ret;
		ret = nullptr;
	}
	return ret;
}

void GameLayer::setVisible(bool bVisible)
{
	bool isVisible = this->isVisible();
	if (isVisible != bVisible)
	{
		bool bRunning = isRunning();
		if (bRunning)
		{
			if (m_pTouchGrabber)
			{
				m_pTouchGrabber->SetTouchEnable(bVisible);
			}
			if (m_bGrabEscKey)
			{
				if (bVisible)
				{
					RegistEscPressCb(this, std::bind(&GameLayer::onKeyEscPressed, this));
				}
				else
				{
					UnRegistEscPressCb(this);
				}
			}

		}
		Layer::setVisible(bVisible);
	}
}


void GameLayer::SetEscKeyReleasedCB(std::function<void()> cbEscKeyReleased)
{
	m_cbEscKeyReleased = cbEscKeyReleased;
}

bool GameLayer::init(const std::string& strBgDialog, bool bTouchCoverAll, bool bGrabEscKey)
{
	if (!Layer::init())
	{
		return false;
	}
	auto bg = Sprite::create(strBgDialog);
	auto sz = bg->getContentSize();
	setContentSize(sz);
	bg->setPosition(sz.width / 2, sz.height / 2);
	addChild(bg);
	m_bGrabEscKey = bGrabEscKey;
	m_bTouchCoverAll = bTouchCoverAll;
	m_pTouchGrabber = nullptr;
	return true;
}

bool GameLayer::init(unsigned uDialogW, unsigned uDialogH, bool bTouchCoverAll, bool bGrabEscKey)
{
	if (!Layer::init())
	{
		return false;
	}
	setContentSize(Size(uDialogW, uDialogH));
	m_bGrabEscKey = bGrabEscKey;
	m_bTouchCoverAll = bTouchCoverAll;
	m_pTouchGrabber = nullptr;

	//Color4B clrBgLayer(0xff,0xff,0xff,0xff) ;
	//auto drawing = DrawNode::create();
	//Vec2 ptRect[] = {Vec2(0, 0), Vec2(uDialogW, 0), Vec2(uDialogW, uDialogH), Vec2(0, uDialogH)};
	//drawing->drawPolygon(ptRect, 4, Color4F(clrBgLayer.r / 255.0f, clrBgLayer.g / 255.0f, clrBgLayer.b / 255.0f, clrBgLayer.a / 255.0f), 0, Color4F());
	//addChild(drawing);

	return true;
}

bool GameLayer::init(unsigned uDialogW, unsigned uDialogH, const cocos2d::Color4B& clrBgLayer, bool bTouchCoverAll, bool bGrabEscKey)
{
	if (!Layer::init())
	{
		return false;
	}

	setContentSize(Size(uDialogW, uDialogH));

	auto drawing = DrawNode::create();
	Vec2 ptRect[] = {Vec2(0, 0), Vec2(uDialogW, 0), Vec2(uDialogW, uDialogH), Vec2(0, uDialogH)};
	drawing->drawPolygon(ptRect, 4, Color4F(clrBgLayer.r / 255.0f, clrBgLayer.g / 255.0f, clrBgLayer.b / 255.0f, clrBgLayer.a / 255.0f), 0, Color4F());
	addChild(drawing);
	
	m_bGrabEscKey = bGrabEscKey;
	m_bTouchCoverAll = bTouchCoverAll;
	m_pTouchGrabber = nullptr;

	return true;
}

void GameLayer::onEnter()
{
	Layer::onEnter();
	bool bVisible = isVisible();
	if (bVisible && m_bGrabEscKey)
	{
		RegistEscPressCb(this, std::bind(&GameLayer::onKeyEscPressed, this));
	}
	if (m_pTouchGrabber == nullptr)
	{
		if (m_bTouchCoverAll)
		{
			m_pTouchGrabber = CreateTouchGrabberWithStaticProcer<STouchEmptyProcer<true>>(this, true);// touchGrabber;
		}
		else
		{
			m_pTouchGrabber = CreateTouchGrabberWithStaticProcer<STouchHitProcer<false>>(this, true);// touchGrabber;
		}		
		m_pTouchGrabber->retain();
	}
	m_pTouchGrabber->SetTouchEnable(bVisible);
}

void GameLayer::onExit()
{
	if (m_pTouchGrabber)
	{
		m_pTouchGrabber->SetTouchEnable(false);
		m_pTouchGrabber->release();
		m_pTouchGrabber = nullptr;
	}
	if (m_bGrabEscKey)
	{
		UnRegistEscPressCb(this);
	}
	Layer::onExit();
}

void GameLayer::onKeyEscPressed()
{
	if (m_cbEscKeyReleased)
	{
		m_cbEscKeyReleased();
	}
}

std::map<GameDialog*, int>& GameDialog::getAllDialogs()
{
	return m_allDialogs;
}

void GameDialog::cleanAllDialogs()
{
	m_allDialogs.clear();
}

bool GameDialog::getDialogVisible()
{
	for (auto dlg : m_allDialogs)
	{
		GameDialog* gDlg = dlg.first;
		if (gDlg != nullptr && gDlg->isVisible())
			return true;
	}

	return false;
}

 bool GameDialog::getDialogEmpty()
 {
	 int size = m_allDialogs.size();
	 return size <= 0;
 }