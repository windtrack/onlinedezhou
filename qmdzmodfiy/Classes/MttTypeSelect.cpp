#include "MttTypeSelect.h"
#include "Util.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"
#include "GlobalUserData.h"
#include "GrabTouch.h"
#include "FrameSprite.h"
#include "GameData.h"
#include "GameMttHall.h"
USING_NS_CC ;


enum 
{
	TableWidth = 378 ,
	TableHeight = 72 ,
};




//enum ButtonSize
//{
//	B_Width = 437,
//	B_Height = 70 ,
//};
bool MttTypeSelect::init()
{
	if(!GameDialog::init())
	{
		return false ;
	}
	
	Size sz = Size(TableWidth,400) ;
	//this->setContentSize(Size(GameData::getInstance()->getGameWidth(),GameData::getInstance()->getGameHeight())) ;
	this->setContentSize(Size(TableWidth,400)) ;
	m_nodePos = Node::create() ;
	m_nodePos->setContentSize(Size(TableWidth,400)) ;
	m_nodePos->ignoreAnchorPointForPosition(false) ;
	m_nodePos->setAnchorPoint(Point(0,1)) ;
	this->addChild(m_nodePos);
	m_nodePos->setPosition(0,sz.height) ;

	m_spriteRealBG = Scale9Sprite::create("gamemtt/mtt_sslxbackground.png",Rect(0,0,378,82),Rect(10,10,378-20,82-20));
	m_spriteRealBG->setAnchorPoint(Point(0.5,1)) ;
	m_nodePos->addChild(m_spriteRealBG); 

	sizeListView = Size(TableWidth,50) ;
	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,0,-20,m_nodePos,0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->ignoreAnchorPointForPosition(false) ;
	m_pScrollView->setAnchorPoint(Point(0.5,1)) ;
	m_pScrollView->setPosition(sizeListView.width/2,sizeListView.height) ;


	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);


	m_spriteBG = Scale9Sprite::create("gamemtt/mtt_sslxbackground.png",Rect(0,0,378,82),Rect(10,10,378-20,82-20));
	m_spriteBG->setAnchorPoint(Point(0.5,1)) ;
	m_pScrollView->addChild(m_spriteBG);

	initTypeButton() ;
	return true ;
}

void MttTypeSelect::initTypeButton()
{
	m_curTypeCount = g_globalMttHallInfo.m_allTypeCount ;

	m_curTypeCount = g_globalMttHallInfo.m_mttTypeName.size();

	for (int i=0;i<m_curTypeCount;i++)
	{
		ccMenuCallback menuCB = CC_CALLBACK_1(MttTypeSelect::onGameTypeSelect, this);

		auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("gamemtt/mtt_sslxchioce2.png", 1, 2, sizeListView.width / 2, 0, m_pScrollView, 0);
		auto touchGrabber = TouchGrabberEx<DTouchHitProcer>::create(bgItem, false);
		touchGrabber->retain();
		touchGrabber->GetTouchProcer().SetHitCB(
			[bgItem](bool bSel, Node*, TouchGrabber*){
				bgItem->SetFrameIdx(bSel?1:0);
		}
		);

		bgItem->setContentSize(Size(bgItem->getContentSize().width,bgItem->getContentSize().height)) ;
		auto btDetail = LayerButton::create(false, true);
		bgItem->addChild(btDetail);
		btDetail->setContentSize(Size(TableWidth,TableHeight-5)) ;
		btDetail->SetHitCB(menuCB);
		btDetail->setTag(g_globalMttHallInfo.m_mttTypeName.at(i).type) ;

		auto labelName = CreateLabelMSYH(26,g_globalMttHallInfo.m_mttTypeName.at(i).name.c_str());
		SetLabelString(labelName,g_globalMttHallInfo.m_mttTypeName.at(i).name.c_str());
		labelName->setPosition(bgItem->getContentSize().width/2,TableHeight/2) ;
		labelName->ignoreAnchorPointForPosition(false) ;
		labelName->setAnchorPoint(Point(0.5,0.5)) ;
		bgItem->addChild(labelName) ;
		bgItem->setPosition(TableWidth/2,TableHeight/2);
		bgItem->setVisible(false) ;
		m_buttonType.push_back(bgItem);

		Sprite* spriteLine = Sprite::create("gamemtt/mtt_sslxfgx.png");
		spriteLine->setAnchorPoint(Point(0.5,0)) ;
		spriteLine->setPosition(bgItem->getContentSize().width/2,0) ;
		bgItem->addChild(spriteLine) ;

	}

}

void MttTypeSelect::onGameTypeSelect(cocos2d::Ref* pItem)
{
	LayerButton* button = (LayerButton*)pItem ;
	int tag = button->getTag() ;

	for (int i=0;i<m_curTypeCount;i++)
	{
		if (tag == i)
		{
			this->setVisible(false) ;
			//发送消息

			auto pf = (GameMttHall*)this->getParent() ;
			pf->showMttTypeSelect(false) ;

			pf->updateMatchViewByGameType(tag);

		}
	}
}

//  Match_Type_All_0=1,//全部
//	Match_Type_GameMoney_1=2,//打金赛
//	Match_Type_FULI_2=4,//福利赛
//	Match_Type_Qbi_3=8,//q币
//	Match_Type_SHIWU_4=16,//实物
//	Match_Type_WEPT_5=32,//门票赛
void MttTypeSelect::updateView()
{
	//m_spriteRealBG->setPosition()
	//unsigned value = g_globalMttHallInfo.m_mttGameType ;
	m_editBox.clear();

	resetAllButton() ;
	
	for(auto type:g_globalMttHallInfo.m_mttTableType)
	{
		FrameSprite* s_lbt = m_buttonType.at(type);
		int tag = s_lbt->getTag() ;
		m_editBox.push_back(s_lbt);
	}
	

	unsigned uCount = m_editBox.size();
	unsigned uItemH = TableHeight;
	float fRealH = uCount * uItemH;

	float fX = sizeListView.width / 2;
	float fY = fRealH - uItemH / 2 ;
	for (int i=0;i<m_editBox.size();i++)
	{
		FrameSprite* s_lbt = m_editBox.at(i);

		int tag = s_lbt->getTag() ;
		s_lbt->setVisible(true);
		s_lbt->setPositionX(TableWidth/2);
		s_lbt->setPositionY(fY);
		fY-=(TableHeight);
	}

	float bgHeight ;

	if (fRealH>380)
	{
		bgHeight = 380;
	}
	else
	{
		bgHeight = fRealH ;
	}


	m_spriteBG->setPreferredSize(Size(TableWidth,bgHeight+14)) ;
	m_spriteBG->setPosition(TableWidth/2,fRealH-14) ;

	m_spriteRealBG->setPreferredSize(Size(TableWidth,bgHeight+14)) ;
	m_spriteRealBG->setPosition(TableWidth/2,400) ;

	float temp ;
	if (fRealH>400)
	{
		temp = fRealH-400+14;
	}
	else
	{
		temp = 0 ;
	}
	m_curoffsetY = temp ;
	sizeListView.height = bgHeight ;
	//每次都显示滚动框的最上面
	m_pScrollView->setContentSize(Size(TableWidth,fRealH));
	m_pScrollView->setViewSize(Size(TableWidth,fRealH));
	m_pScrollView->setContentOffset(Vec2(0,-temp), false);
	m_pScrollView->setPosition(TableWidth/2,400-8) ;
}


void MttTypeSelect::resetAllButton()
{
	for (int i=0;i<m_curTypeCount;i++)
	{
		FrameSprite* button = m_buttonType.at(i);
		button->setVisible(false) ;
	}
}

void MttTypeSelect::doMoveIn()
{
	updateView() ;
	m_pScrollView->setContentOffset(Vec2(0,-m_curoffsetY), false);
	m_pScrollView->setViewSize(Size(TableWidth,sizeListView.height));
	m_pScrollView->setPosition(TableWidth/2,400) ;
	float curX;
	float curY ;
	float moveTime = 0.3f;
	for (int i=0;i<m_editBox.size();i++)
	{
		FrameSprite* button = m_editBox.at(i);

		curX = button->getPositionX() ;
		curY = button->getPositionY()+m_curoffsetY ;

		button->setPositionY(curY+400) ;
		button->runAction(MoveTo::create(moveTime, Vec2(curX, curY)));
	}
	m_spriteBG->setVisible(true) ;
	m_spriteRealBG->setVisible(false) ;

	curX = m_spriteBG->getPositionX() ;
	curY = m_spriteBG->getPositionY() ;
	m_spriteBG->setPositionY(curY+400) ;

	auto bgMoveAction = [this](){

		for (int i=0;i<m_editBox.size();i++)
		{
			FrameSprite* button = m_editBox.at(i);

			float curX = button->getPositionX() ;
			float curY = button->getPositionY()-m_curoffsetY ;
			button->runAction(MoveTo::create(0.1f, Vec2(curX, curY)));
		}


		m_pScrollView->setContentOffset(Vec2(0,-m_curoffsetY), false);
		m_pScrollView->setViewSize(Size(TableWidth,sizeListView.height));
		m_pScrollView->setPosition(TableWidth/2,400-8) ;
		m_spriteRealBG->setVisible(true) ;
		m_spriteBG->setVisible(false) ;
	} ;

	m_spriteBG->runAction(Sequence::create(MoveTo::create(moveTime, Vec2(curX, curY)),CallFunc::create(bgMoveAction),NULL));


}

void MttTypeSelect::setVisible(bool visible)
{
	GameDialog::setVisible(visible) ;


}

void MttTypeSelect::onGrabTouchBegan(bool bHitDialog)
{
	if (bHitDialog)
	{
		
	}
	else
	{
		auto pf = (GameMttHall*)this->getParent() ;
		pf->showMttTypeSelect(false) ;
	}
	//GameDialog::setVisible(false) ;
}