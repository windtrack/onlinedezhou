#include "UpdateInfo.h"
#include "Util.h"
#include "GlobalUserData.h"
#include "LineLabel.h"
#include "GameData.h"

enum 
{
	UpdateInfo_WIDTH = 25*22 ,
	UpdateInfo_HEIGHT = 45
};

USING_NS_CC;
bool UpdateInfo::init()
{
	if (!GameLayer::init("userinfo/bgdialog0.png", false))
	{
		return false;
	}
	Size bgSize = this->getContentSize() ;

	sizeListView = Size(26*22,UpdateInfo_HEIGHT*5) ;
	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,65+22,77,this,0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);


	m_labelTitle  = CreateLabelMSYHBD(30, "版本更新内容");
	m_labelTitle->setTextColor(Color4B(0xff,0xd2,0x00,255));
	m_labelTitle->setPosition(bgSize.width/2,342) ;
	this->addChild(m_labelTitle);


	m_spriteFlowerLeft = Sprite::create("update/titleflower.png") ;
	m_spriteFlowerLeft->setPositionY(342);
	this->addChild(m_spriteFlowerLeft);
	m_spriteFlowerRight = Sprite::createWithTexture(m_spriteFlowerLeft->getTexture()) ;
	m_spriteFlowerRight->setScaleX(-1) ;
	m_spriteFlowerRight->setPositionY(342);
	this->addChild(m_spriteFlowerRight);

	m_curLineCount = 0 ;

	//for (int i=0; i<10; i++)
	//{
	//	char buf[128] ;
	//	LineUpdateInfo lineInfo ;
	//	sprintf(buf,"%d 、经理开会四点多就快来粉红色立刻返回给开了家第三方和管理和收费",(i+1)) ;
	//	lineInfo.m_info = buf ;
	//	g_globaUpdateInfo.m_vecAllUpdateInfo.push_back(lineInfo) ;
	//}

	std::vector<std::string> mUpdateContent = GameData::getInstance()->getUpdateContent() ;
	for (int i=0; i<mUpdateContent.size(); i++)
	{
		LineUpdateInfo lineInfo ;
		lineInfo.m_info = mUpdateContent.at(i) ;
		g_globaUpdateInfo.m_vecAllUpdateInfo.push_back(lineInfo) ;
	}


	updateInfo() ;
	return true ;
}


void UpdateInfo::updateInfo()
{

	unsigned uItemH = UpdateInfo_HEIGHT;
	float fX = 5;
	m_vecAutoLineLabel.clear() ;
	for(auto& followList:g_globaUpdateInfo.m_vecAllUpdateInfo)
	{
		std::string tips = followList.m_info ;
		LineLabel *mLineLable = LineLabel::create() ;
		mLineLable->Appand(22,"fzlt.ttf",tips,Color4B::WHITE) ;
		mLineLable->setAnchorPoint(Point(0,0.5)) ;
		m_pScrollView->addChild(mLineLable) ;
		m_vecAutoLineLabel.push_back(mLineLable);
		m_curLineCount++ ;
	}


	unsigned maxHeight = UpdateInfo_HEIGHT*m_curLineCount ;


	//每次都显示滚动框的最上面
	float fY = maxHeight - uItemH / 2 +10;
	for(int i=0; i<m_vecAutoLineLabel.size(); i++)
	{
		auto autoLine = m_vecAutoLineLabel.at(i) ;
		autoLine->setPositionX(fX);
		autoLine->setPositionY(fY) ;
		fY -= uItemH;  
	}

	if(maxHeight < sizeListView.height)
	{
		maxHeight = sizeListView.height;
	}
	float temp = maxHeight - sizeListView.height;
	m_pScrollView->setContentSize(Size(sizeListView.width,maxHeight));
	m_pScrollView->setContentOffset(Vec2(0,-temp), false);

}

void UpdateInfo::updateTileVersion(std::string version)
{
	char buf[64] ;
	sprintf(buf,"%s版本更新内容",version.c_str()) ;
	SetLabelString(m_labelTitle,buf) ;
	Size bgSize = this->getContentSize() ;
	m_spriteFlowerLeft->setPositionX(bgSize.width/2-m_labelTitle->getContentSize().width/2-25) ;
	m_spriteFlowerRight->setPositionX(bgSize.width/2+m_labelTitle->getContentSize().width/2+25) ;
}