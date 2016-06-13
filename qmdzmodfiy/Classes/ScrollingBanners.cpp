#include "ScrollingBanners.h"
#include "MoveLabel.h"
#include "GameData.h"

USING_NS_CC ;


ScrollingBanners* ScrollingBanners::creatScrollingBanners()
{


	auto m_scrollbanners = new (std::nothrow)ScrollingBanners();
	if (m_scrollbanners && m_scrollbanners->initWithColor(Color4B(0x00,0x00,0x00,0x99),GameData::getInstance()->getGameWidth(), 30)) {
		m_scrollbanners->autorelease();
		return m_scrollbanners;
	} else {
		CC_SAFE_DELETE(m_scrollbanners);
		return nullptr;
	}

}


void ScrollingBanners::callScrollBannersMove()
{

	if (!GameData::getInstance()->hasMoveLable())
	{
		return ;
	}
	if (!m_isScrollTextMove)
	{
		m_moveLableCount++ ;
		this->setVisible(true) ;
		MoveLabel* m_curLineLabel = GameData::getInstance()->getOneMoveLable() ;
		m_curLineLabel->setVisible(true) ;
		m_isScrollTextMove = true ;

		m_curLineLabel->setTag(10) ;
		float m_MaxScrollTextWidth = GameData::getInstance()->getGameWidth() ;
		m_curLineLabel->setPositionX(m_MaxScrollTextWidth+m_curLineLabel->getContentSize().width/2+20) ;
		m_curLineLabel->setPositionY(this->getContentSize().height/2) ;
		this->addChild(m_curLineLabel) ;
		GameData::getInstance()->removeGameTips(m_curLineLabel) ;
		m_curLineLabel->doMove(std::bind(&ScrollingBanners::callNextScrollText, this),std::bind(&ScrollingBanners::callScrollTextMoveEnd, this)) ;

	}

}

/************************************************************************/
/* 通知下一个滚动提示                                                                     */
/************************************************************************/
void ScrollingBanners::callNextScrollText()
{
	m_isScrollTextMove = false ;
	if (!GameData::getInstance()->hasMoveLable())
	{
		return ;
	}
	callScrollBannersMove() ;
}

void ScrollingBanners::callScrollTextMoveEnd()
{
	m_moveLableCount-- ;
	if (!GameData::getInstance()->hasMoveLable()&&m_moveLableCount==0)
	{
		this->setVisible(false) ;
	}

}
/************************************************************************/
/* 重置滚动提示                                                                     */
/************************************************************************/
void ScrollingBanners::reSetScorlllBanners()
{
	this->removeAllChildren() ;
	this->setVisible(false) ;
	m_isScrollTextMove = false ;
	m_moveLableCount = 0 ;
}

void ScrollingBanners::checkVisible()
{
	this->setVisible(m_isScrollTextMove) ;
}