#include "DoorMenu.h"
#include "MenuItemSpriteWithLabel.h"
#include "Util.h"
#include "GameCommon.h"
#include "GlobalUserData.h"
#include "GameData.h"
USING_NS_CC;

DoorMenu* DoorMenu::creatWithLayer(cocos2d::Node* node)
{
	DoorMenu* doorMenu = new DoorMenu() ;
	if (doorMenu && doorMenu->initWithLayer(node))
	{
		doorMenu->autorelease();
		return doorMenu;
	}
	else
	{
		CC_SAFE_DELETE(doorMenu) ;
		return NULL;
	}


}

bool DoorMenu::initWithLayer(cocos2d::Node* node)
{
	do 
	{
		if (!GameDialog::init("doormenu/bgmenu.png", false))
		{
			return false;
		}
		// setVisible(false);
		m_nodePos = node ;
	m_pMenuItemReturn = MenuItemSpriteWithLabel::create("doormenu/btmenuret.png", 2, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	m_pMenuItemChangeTable = MenuItemSpriteWithLabel::create("doormenu/btmenuchangetable.png", 3, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	m_pMenuItemStandUp = MenuItemSpriteWithLabel::create("doormenu/btmenustandup.png", 3, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	m_pMenuItemShowCardType = MenuItemSpriteWithLabel::create("doormenu/btmenushowcardtype.png", 2, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	m_pMenuItemTaskSet = MenuItemSpriteWithLabel::create("doormenu/btmenutaskset.png", 2, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	m_pMenuItemReturn->setPosition(105, 295);
	m_pMenuItemChangeTable->setPosition(105, 232);
	m_pMenuItemStandUp->setPosition(105, 168);
	m_pMenuItemShowCardType->setPosition(105, 106);
	m_pMenuItemTaskSet->setPosition(105, 45);
	auto pMenu = Menu::create(m_pMenuItemChangeTable, m_pMenuItemReturn, m_pMenuItemStandUp, m_pMenuItemShowCardType, m_pMenuItemTaskSet, nullptr);
	pMenu->setPosition(Vec2::ZERO);
	addChild(pMenu);



		return true ;
	} while (0);

	return false ;
}




bool DoorMenu::init()
{
	//if (!GameDialog::init("doormenu/bgmenu.png", false))
	//{
	//	return false;
	//}
	//// setVisible(false);

	//m_pMenuItemReturn = MenuItemSpriteWithLabel::create("doormenu/btmenuret.webp", 2, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	//m_pMenuItemChangeTable = MenuItemSpriteWithLabel::create("doormenu/btmenuchangetable.webp", 3, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	//m_pMenuItemStandUp = MenuItemSpriteWithLabel::create("doormenu/btmenustandup.webp", 3, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	//m_pMenuItemShowCardType = MenuItemSpriteWithLabel::create("doormenu/btmenushowcardtype.webp", 2, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	//m_pMenuItemTaskSet = MenuItemSpriteWithLabel::create("doormenu/btmenutaskset.webp", 2, CC_CALLBACK_1(DoorMenu::onMenuItemSelected, this));
	//m_pMenuItemReturn->setPosition(95, 224);
	//m_pMenuItemChangeTable->setPosition(95, 173);
	//m_pMenuItemStandUp->setPosition(95, 124);
	//m_pMenuItemShowCardType->setPosition(95, 78);
	//m_pMenuItemTaskSet->setPosition(95, 30);
	//auto pMenu = Menu::create(m_pMenuItemReturn, m_pMenuItemChangeTable, m_pMenuItemStandUp, m_pMenuItemShowCardType, m_pMenuItemTaskSet, nullptr);
	//pMenu->setPosition(Vec2::ZERO);
	//addChild(pMenu);

	return true;
}

void DoorMenu::onGrabTouchBegan(bool bHit)
{
	if (bHit)
	{

	}
	else
	{
		Hide();
	}
}

void DoorMenu::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		m_pMenuItemStandUp->setEnabled(g_globalTableConfig.GetTableMode() == TablePlaying);
        m_pMenuItemChangeTable->setEnabled(!(g_globalGameSerialization.IsInGameNow()));

		runAction(Sequence::create(Place::create(Vec2(6, GameData::getInstance()->getGameHeight())), MoveTo::create(0.2f, Vec2(6, m_nodePos->getPositionY()-this->getContentSize().height+30)), nullptr));
	}
}
void DoorMenu::onKeyEscPressed()
{
	Hide();
}

void DoorMenu::onMenuItemSelected(cocos2d::Ref *pItem)
{
	Hide();
	if (pItem == m_pMenuItemReturn)
	{
		SendClientRequest(ClientRequest::kLeaveTable);
	}
	else if (pItem == m_pMenuItemChangeTable)
	{
		if (g_pGameFrame->getTableType() == PrivateTable)
		{
			PopTipWithBt1("当前不可换桌","确定",nullptr) ;
			return ;
		}

		SendClientRequest(ClientRequest::kChangeTable);
	}
	else if (pItem == m_pMenuItemStandUp)
	{
		Json::Value param;
		param["needconfirm"] = true;
		SendClientRequest(ClientRequest::kStandUp, param);
	}
	else if (pItem == m_pMenuItemShowCardType)
	{
		if (m_funCBShowCardType)
		{
			m_funCBShowCardType();
            g_globalGameSerialization.AddAtomicAction(AtomicAction::TaskCardIntro);
		}
	}
	else if (pItem == m_pMenuItemTaskSet)
	{
		Json::Value param;
		param["needconfirm"] = false;
		SendClientRequest(ClientRequest::kPopGameSet, param);
	}
}

void DoorMenu::Hide()
{
	if (getPositionY() <= m_nodePos->getPositionY())
	{
		runAction(Sequence::create(MoveBy::create(0.1f, Vec2(0, GameData::getInstance()->getGameHeight()/2)), Hide::create(), nullptr));
	}
}

bool CardTypeView::init()
{
	if (!GameDialog::init("doormenu/bgcardtype.png", false))
	{
		return false;
	}
	return true;
}

void CardTypeView::onGrabTouchBegan(bool bHit)
{
	Hide();
}

void CardTypeView::onSetVisible(bool bVisible)
{
	if (bVisible)
	{
		auto sz = getContentSize();
		runAction(Sequence::create(Place::create(Vec2(-sz.width, GameData::getInstance()->getGameHeight()/2 + 42)), MoveTo::create(0.2f, Vec2(8, GameData::getInstance()->getGameHeight()/2 + 42)), nullptr));
	}
}
void CardTypeView::onKeyEscPressed()
{
	Hide();
}


void CardTypeView::Hide()
{
	auto sz = getContentSize();
	runAction(Sequence::create(MoveBy::create(0.2f, Vec2(-3 - sz.width, 0)), Hide::create(), nullptr));
}