#include "SngMatchState.h"
#include "Tools.h"
#include "Util.h"
#include "GlobalUserData.h"

enum
{
	Layer_Rank=0,
	Layer_Mangzhu,
	Layer_Reward,
};
bool comp(const SngRoleInfo &a, const SngRoleInfo &b)
{
    return a.m_cash > b.m_cash;
}

bool SngMatchState::init()
{

	if(!GameDialog::init("dialog/bg.png",true))
	{
		return false;
	}

	Size sz = this->getContentSize() ;
	m_uiRoot = CSLoader::createNode("res/ui/matchstate.csb");
	m_uiRoot->ignoreAnchorPointForPosition(false);
	m_uiRoot->setAnchorPoint(Point(0.5,0.5));
	m_uiRoot->setPosition(sz.width/2,sz.height/2) ;
	this->addChild(m_uiRoot,1);

	std::string layerName[3] ={"Panel_rank", "Panel_mangzhu", "Panel_reward", } ; 

	for (int i=0; i<3 ;i++)
	{
		m_matchLayer[i] = static_cast<Layout*>(Tools::getWidgetByName(m_uiRoot,layerName[i])) ;
	}

	std::string buttonName[3] ={"Button_rank", "Button_mangzhu", "Button_reward", } ; 
	std::string buttonNameText[3] ={"Text_rank", "Text_mangzhu", "Text_reward", } ; 
	Layout* buttonLayer = static_cast<Layout*>(Tools::getWidgetByName(m_uiRoot,"Panel_button")) ;
	for (int i=0; i<3 ;i++)
	{
		m_matchButton[i] = static_cast<Button*>(Tools::getWidgetByName(buttonLayer,buttonName[i])) ;
		m_matchButton[i] ->addTouchEventListener(this,toucheventselector(SngMatchState::onButtonCallBack));

		m_text_button[i] = static_cast<Text*>(Tools::getWidgetByName(m_matchButton[i],buttonNameText[i])) ;

	}

	m_textRank = static_cast<Text*>(Tools::getWidgetByName(m_matchLayer[Layer_Rank],"Text_currank")) ;
	m_textTableName = static_cast<Text*>(Tools::getWidgetByName(buttonLayer,"Text_mathname")) ;


	std::string mangzhuBgName[16] ={"Image_mangzhuselect0", "Image_mangzhuselect0_11", "Image_mangzhuselect0_15", 
		"Image_mangzhuselect0_19" ,"Image_mangzhuselect0_23" ,"Image_mangzhuselect0_27" ,
		"Image_mangzhuselect0_31" ,"Image_mangzhuselect0_35" ,"Image_mangzhuselect0_39" ,
		"Image_mangzhuselect0_43" ,"Image_mangzhuselect0_47" ,"Image_mangzhuselect0_51" ,
		"Image_mangzhuselect0_55" ,"Image_mangzhuselect0_59" ,"Image_mangzhuselect0_63" ,
		"Image_mangzhuselect0_67" ,
	
	} ; 

    //获取盲注中需要的控件
    char buff[64] = {0};
    for (int i=0; i<16; i++)
    {
        sprintf(buff, "Panel_mangzhu%d", i);
        m_pBlindLayer[i] = static_cast<ui::Layout*>(Tools::getWidgetByName(m_matchLayer[1], buff));
        for (int j=0; j<4; j++)
        {
            sprintf(buff, "Text_mangzhu%d", j);
            m_pBlindText[i][j] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pBlindLayer[i], buff));
        }
    }
	
																																						

	listviewMangzhu =  static_cast<ListView*>(Tools::getWidgetByName(m_matchLayer[Layer_Mangzhu],"ListView_mangzhu")) ;
	for (int i=0; i<16; i++)
	{
		m_img_mangzhuBg[i] = static_cast<ImageView*>(Tools::getWidgetByName(listviewMangzhu,mangzhuBgName[i])) ;
		m_img_mangzhuBg[i]->setVisible(false) ;
	}
    //奖励控件
    for (int i=0; i<3; i++)
    {
        sprintf(buff, "Image%d", i);
        m_pImageReward[i] = static_cast<ui::ImageView*>(Tools::getWidgetByName(m_matchLayer[Layer_Reward], buff));
        m_pTextReward[i][0] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pImageReward[i], "Text_reward"));
        m_pTextReward[i][1] = static_cast<ui::Text*>(Tools::getWidgetByName(m_pImageReward[i], "Text_score"));
    }

	Node* joinFamilyNode = Tools::getWidgetByName(m_matchLayer[Layer_Rank],"Panel_rankview");
	sizeListView.width = joinFamilyNode->getContentSize().width+10;
	sizeListView.height = joinFamilyNode->getContentSize().height;
	float py = joinFamilyNode->getPositionY() ;
	joinFamilyNode->setPositionY(py) ;

	m_pScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,-10,+3,joinFamilyNode,0);
	m_pScrollView->setContentSize(sizeListView);
	m_pScrollView->setBounceable(true);
	m_pScrollView->setSwallowsTouches(false) ;
	m_pScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollView->setVisible(true);


	Node* viewBlind = Tools::getWidgetByName(m_matchLayer[Layer_Mangzhu],"Panel_blindview");
	
	sizeBlindListView =viewBlind->getContentSize() ;
	m_pScrollViewBlind = CreateNode1AndPosAddTo<extension::ScrollView>(sizeBlindListView,0,0,viewBlind,0);
	m_pScrollViewBlind->setContentSize(sizeBlindListView);
	m_pScrollViewBlind->setBounceable(true);
	m_pScrollViewBlind->setSwallowsTouches(false) ;
	m_pScrollViewBlind->setDirection(extension::ScrollView::Direction::VERTICAL);
	m_pScrollViewBlind->setVisible(true);

	//盲注界面
	//数据模拟
	//for (int i=0;i<16 ;i++)
	//{
	//	SngShengmang shengmang = g_globalSngTableInfo.m_sngShengmang.at(i) ;
	//	sprintf(buff, "%d", shengmang.m_level);
	//	setUITextString(m_pBlindText[i][0], buff);
	//	sprintf(buff, "%d/%d", shengmang.m_mangzhu, 2*shengmang.m_mangzhu);
	//	setUITextString(m_pBlindText[i][1], buff);
	//	sprintf(buff, "%d", shengmang.m_qiangzhu);
	//	setUITextString(m_pBlindText[i][2], buff);
	//	sprintf(buff, "%d", shengmang.m_time);
	//	setUITextString(m_pBlindText[i][3], buff);
	//}


    //SngRoleInfo roleInfo;
    //for (int i=0; i<9; i++)
    //{
    //    sprintf(buff, "Name = %d", i+1);
    //    roleInfo.m_cash = 10000 - i*400;
    //    roleInfo.m_charid = i+1;
    //    roleInfo.m_name = buff;
    //    roleInfo.m_rankid = i+1;
    //    appendRankItem(roleInfo);
    //}



		//showMatchStateLayer(Layer_Rank);
	return true ;
}

void SngMatchState::onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_ENDED)
	{
		for (int i=0;i<3 ;i++)
		{
			if (pItem == m_matchButton[i])
			{
				showMatchStateLayer(i) ;
			}
		}
	}
}

void SngMatchState::showMatchStateLayer(int index)
{


	for (int i=0;i<3; i++)
	{

			m_matchButton[i]->setBright(i!=index) ;
			m_text_button[i]->setTextColor(i!=index?Color4B(0x37,0xa4,0x5c,0xff):Color4B(0xff,0xff,0xff,0xff));
			m_matchLayer[i]->setVisible(i==index);
	}


	m_textRank->setVisible(index==Layer_Rank) ;
	m_textTableName->setVisible(index!=Layer_Rank) ;




	//if (index==Layer_Mangzhu)
	//{
	//	updateMangzhu(g_globalSngTableInfo.m_mangzhuIndex) ;
	//}
     if (index == Layer_Rank)
    {
        //updateRankList();
    }
    else if (index == Layer_Reward)
    {
        //updateMatchStateReward(g_globalSngTableInfo.m_rewardCount);
    }
}

void SngMatchState::setVisible(bool bVisible)
{
	
    PlayVisibleSound(bVisible);
	if (bVisible)
	{
		GameDialog::setVisible(bVisible) ;

		SendClientRequest(ClientRequest::kCallSngTableRankList) ;

		showMatchStateLayer(Layer_Rank) ;

		Tools::callScaleAction(this);
		//updateMangzhu(g_globalSngTableInfo.m_mangzhuIndex);
		/*      auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
		runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0);GameDialog::setVisible(true);}), actScale, nullptr));*/
	}
	else
	{
			GameDialog::setVisible(bVisible) ;
		//clearRank();
		//SendClientRequest(ClientRequest::)
        SendClientRequest(ClientRequest::kOptSNGHallBgLayer);
		//callHideAction() ;
        //auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
        //runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));
	}
}

void SngMatchState::updateReward(unsigned rewordNum)
{
    if (rewordNum == 2)
    {
        m_pImageReward[2]->setVisible(false);
    }
    else if (rewordNum == 3)
    {
        m_pImageReward[2]->setVisible(true);
    }
    char buff[32] = {0};
    for (int i=0; i<rewordNum; i++)
    {
        sprintf(buff, "%d", g_globalSngTableInfo.m_sngReward.at(i).m_cashReward);
        setUITextString(m_pTextReward[i][0], buff);
        sprintf(buff, "%d", g_globalSngTableInfo.m_sngReward.at(i).m_score);
        setUITextString(m_pTextReward[i][1], buff);
    }

}

void SngMatchState::updateCurBlindLevel(int index)
{

	for (int i=0 ;i<m_vecShowBlindItem.size(); i++)
	{
		SngBlindShow* blindShow = m_vecShowBlindItem.at(i) ;
		if (i == index)
		{
		blindShow->m_pSpriteBGItem->setVisible(true) ;
		unsigned uItemH = 43;
		unsigned fRealH = m_vecShowBlindItem.size() * uItemH;
		float temp = fRealH - sizeBlindListView.height ;

		float curTemp = temp -index*uItemH ;

		if ((temp-curTemp)<sizeBlindListView.height-uItemH)
		{
			curTemp = temp ;
		}

		if ((temp-curTemp)<0)
		{
			curTemp = fRealH ;
		}
		if (curTemp<0)
		{
			curTemp = 0 ;
		}

		m_pScrollViewBlind->setContentOffset(Vec2(0,-curTemp), false);
	
		}
		else
		{
			blindShow->m_pSpriteBGItem->setVisible(false) ;
		}
	}


}

void SngMatchState::updateSngMatchRankList()
{


	while (m_vecShowItem.size()!=0)
	{
		SngRoleShow* blind = m_vecShowItem.at(0) ;
		blind->m_pSpriteBGItem->removeFromParent() ;
		m_vecShowItem.erase(m_vecShowItem.begin());
	}


	m_vecShowItem.clear();


	//第一轮手动排序  让自己排在第一位
	if (g_globalSngTableInfo.m_roudIdex<=1)
	{
		std::vector<SngRoleInfo> m_oldValue ;

		for (int i=0; i<g_globalSngTableInfo.m_sngRoleInfo.size();i++)
		{
			SngRoleInfo info = g_globalSngTableInfo.m_sngRoleInfo.at(i) ;
			if (info.m_uid == g_globalMyData.m_iUserId)
			{
					m_oldValue.push_back(info) ;
					g_globalSngTableInfo.m_sngRoleInfo.erase(i+g_globalSngTableInfo.m_sngRoleInfo.begin()) ;
			
			}
		
		}
		for (int i=0; i<g_globalSngTableInfo.m_sngRoleInfo.size();i++)
		{
			SngRoleInfo info = g_globalSngTableInfo.m_sngRoleInfo.at(i) ;
			m_oldValue.push_back(info) ;
		}
		g_globalSngTableInfo.m_sngRoleInfo.clear() ;

		for(int i=0 ;i<m_oldValue.size(); i++)
		{
			g_globalSngTableInfo.m_sngRoleInfo.push_back(m_oldValue.at(i)) ;
		}

	}








	unsigned uCount = g_globalSngTableInfo.m_sngRoleInfo.size();
	unsigned ListBgItemH = 686;
	unsigned uItemH = 112;




	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2;
	float fY = fRealH - uItemH / 2 - 10;




	for(auto& followList:g_globalSngTableInfo.m_sngRoleInfo)
	{
		auto item = GetRankItem();
		updateRankItem(item, followList);
		item->m_pSpriteBGItem->setPosition(fX+5, fY);
		item->m_pSpriteBGItem->setVisible(true); 
		/*item->m_pSpriteBGItem->setVisible(true) ;*/
		m_vecShowItem.push_back(item);
		fY -= uItemH;     
	}
	float temp = fRealH - sizeListView.height;

	//每次都显示滚动框的最上面
	m_pScrollView->setContentOffset(Vec2(0,-temp), false);
	m_pScrollView->setContentSize(Size(sizeListView.width,fRealH));
	



}

void SngMatchState::appendRankItem(SngRoleInfo& roleInfo)
{
    unsigned uAllItemH = 0;
    auto rankItem = GetRankItem();
    m_vecShowItem.push_back(rankItem);
    unsigned uAllItemNum = m_vecShowItem.size();

    auto szItem = rankItem->m_pSpriteBGItem->getContentSize();
    unsigned uY = szItem.height/2 + 12;
    uAllItemH = uAllItemNum*(szItem.height + 10);

    updateRankItem(rankItem, roleInfo);
    
    if (uAllItemH < sizeListView.height)
    {
        uAllItemH = sizeListView.height;
    }

    double dOffset = sizeListView.height - uAllItemH;
    m_pScrollView->setContentOffset(Vec2(0, dOffset), false);
    unsigned num = 0;
    for (auto item : m_vecShowItem)
    {
        item->m_pSpriteBGItem->setPosition(sizeListView.width/2+5, uAllItemH - uY - (num)*(szItem.height + 10));
        num += 1;
    }
    m_pScrollView->setContentSize(Size(sizeListView.width, uAllItemH));

    m_vecRoleInfo.push_back(roleInfo);
}

SngMatchState::SngRoleShow* SngMatchState::GetRankItem()
{
    SngRoleShow *roleItem = nullptr;

    auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("family/fanlilyblock.png", 1, 2, sizeListView.width/2, 0, m_pScrollView, 0);

    auto httpSprite = CreateUserHeadSprite("defaulticon.png", "gamefriendlist/iconclipmask.png", bgItem, Vec2(10, 8), "gamefriendlist/iconround.png");

    auto spriteRank = CreateNode3AndPosAddTo<FrameSprite>("ranklist/ranklistreword.png", 1, 4, 20, 98, bgItem, 3);
    spriteRank->SetFrameIdx(0);

    auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0, 0), 130, 44, Color4B(0xff,0xff,0xff,0xff), bgItem, 0);

    auto labelCash = CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0, 0), 482, 44, Color4B(0xff,0xd2,0x00,0xff), bgItem, 0);

    auto sz = spriteRank->getContentSize();
    auto labelRank = CreateLabelMSYHAndAnchorPosClrAddTo(18, "", Vec2(0.5, 0.5), sz.width/2, sz.height/2, Color4B(0xff,0xd2,0x00,0xff), spriteRank, 0);

    auto pItem = new SngRoleShow;
    pItem->m_pSpriteBGItem = bgItem;
    pItem->m_pSpriteIcon = httpSprite;
    pItem->m_pSpriteBgRank = spriteRank;
    pItem->m_pLabelName = labelName;
    pItem->m_pLabelRank = labelRank;
    pItem->m_pLabelWinPot = labelCash;
    roleItem = pItem;

    return roleItem;

}

void SngMatchState::updateRankItem(SngMatchState::SngRoleShow *roleShow, SngRoleInfo& roleInfo)
{
    SetLabelString(roleShow->m_pLabelName, roleInfo.m_name);
    SetLabelString(roleShow->m_pLabelWinPot, FormatCash4(roleInfo.m_cash));
    char buff[32];
    if (roleInfo.m_rankid > 3)
    {
        roleShow->m_pSpriteBgRank->SetFrameIdx(3);
        sprintf(buff, "%d", roleInfo.m_rankid);
        SetLabelString(roleShow->m_pLabelRank, buff);
    }
    else
    {
        SetLabelString(roleShow->m_pLabelRank, "");
        roleShow->m_pSpriteBgRank->SetFrameIdx(roleInfo.m_rankid - 1);
    }
    SetIconUrl(roleShow->m_pSpriteIcon, roleInfo.m_icon);
}

void SngMatchState::clearRank()
{
	//m_pScrollView->removeAllChildren() ;
	//m_pScrollViewBlind->removeAllChildren() ;
 //   m_vecRoleInfo.clear();
 //   m_vecShowItem.clear();
	//m_vecShowBlindItem.clear() ;
}



void SngMatchState::updateTableMenuTips()
{
	setUITextString(m_textTableName,g_globalSngTableInfo.m_tableName.c_str()) ;
	char buff[64] ;
	sprintf(buff,"当前排名第%d名",g_globalSngTableInfo.m_rank) ;
	setUITextString(m_textRank,buff) ;
}

void SngMatchState::updateQIanzhu()
{
	for (int i=0;i<16 ;i++)
	{
		char buff[64] ;
		SngShengmang shengmang = g_globalSngTableInfo.m_sngShengmang.at(i) ;
		sprintf(buff, "%d", shengmang.m_qiangzhu);
		setUITextString(m_pBlindText[i][2], buff);

	}
}

void SngMatchState::updateData()
{
	//排行列表和前注
	updateTableMenuTips() ;
	//updateSngMatchRankList();
	//updateQIanzhu() ;
	//updateCurBlindLevel(rand()%g_globalSngTableInfo.m_sngShengmang.size()) ;
	updateCurBlindLevel(g_globalSngTableInfo.m_mangzhuIndex) ;
	//更新奖励
	updateReward(g_globalSngTableInfo.m_rewardCount);

	//updateBlindData() ;
}

void SngMatchState::updateBlindData()
{
	updateBlindList() ;

}

void SngMatchState::updateBlindList()
{


	while (m_vecShowBlindItem.size()!=0)
	{
			SngBlindShow* blind = m_vecShowBlindItem.at(0) ;
			blind->m_pNode->removeFromParent() ;
			m_vecShowBlindItem.erase(m_vecShowBlindItem.begin());
	}


	m_vecShowBlindItem.clear();
	unsigned uCount = g_globalSngTableInfo.m_sngShengmang.size();
	unsigned ListBgItemH = sizeBlindListView.width;
	unsigned uItemH = 43;
	unsigned fRealH = uCount * uItemH;
	if(fRealH < sizeBlindListView.height)
	{
		fRealH = sizeBlindListView.height;
	}
	float fX = sizeBlindListView.width / 2;
	float fY = fRealH - uItemH;




	for(auto& followList:g_globalSngTableInfo.m_sngShengmang)
	{
		auto item = GetBlindItem();
		updateBlindItem(item, followList);
		item->m_pNode->setPosition(0, fY);
		item->m_pNode->setVisible(true); 
		item->m_pSpriteBGItem->setVisible(false) ;
		m_vecShowBlindItem.push_back(item);
		fY -= uItemH;     
	}
	float temp = fRealH - sizeBlindListView.height;

	//每次都显示滚动框的最上面
	m_pScrollViewBlind->setContentOffset(Vec2(0,-temp), false);
	m_pScrollViewBlind->setContentSize(Size(sizeBlindListView.width,fRealH));




}

SngMatchState::SngBlindShow* SngMatchState::GetBlindItem()
{
	SngBlindShow *roleItem = nullptr;
	Node* bgNode = Node::create() ;
	bgNode->setContentSize(Size(671.00,43.00)) ;
	m_pScrollViewBlind->addChild(bgNode) ;

	auto bgItem = Sprite::create("gamesng/sng_table_background-8.png");

	//bgItem->setContentSize(Size(659.00,43.00)) ;
	bgItem->setPosition(sizeBlindListView.width/2,43.00/2) ;
	bgNode->addChild(bgItem) ;


	auto bgItemlight = Sprite::create("gamesng/sng_table_background-9.png");
	bgItemlight->setPosition(sizeBlindListView.width/2,0) ;
	bgNode->addChild(bgItemlight) ;

	auto labelrank = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5, 0.5), 50.84, 20.57, Color4B(0xff,0xff,0xff,0xff), bgNode, 0);

	auto labelBlind = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5, 0.5), 189.59, 20.57, Color4B(0xff,0xff,0xff,0xff), bgNode, 0);

	auto labelForntBlind = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5, 0.5), 387.89, 20.57, Color4B(0xff,0xff,0xff,0xff), bgNode, 0);

	auto labelTime = CreateLabelMSYHAndAnchorPosClrAddTo(20, "", Vec2(0.5, 0.5), 576.66, 20.57, Color4B(0xff,0xff,0xff,0xff), bgNode, 0);

	auto pItem = new SngBlindShow;
	pItem->m_pNode = bgNode;
	pItem->m_pSpriteBGItem = bgItem;
	pItem->m_pSpriteBGlight = bgItemlight;
	pItem->m_pLabelRank = labelrank;
	pItem->m_pLabelBlind = labelBlind;
	pItem->m_pLabelForntBlind = labelForntBlind;
	pItem->m_pLabelTime = labelTime;
	roleItem = pItem;

	return roleItem;
}

void SngMatchState::updateBlindItem(SngMatchState::SngBlindShow *roleShow, SngShengmang& roleInfo)
{


	char buff[64] ;
	sprintf(buff,"%d",roleInfo.m_level) ;
	SetLabelString(roleShow->m_pLabelRank, buff);
	SetLabelString(roleShow->m_pLabelBlind,roleInfo.m_mangzhu.c_str() );
	sprintf(buff,"%d",roleInfo.m_qiangzhu) ;
	SetLabelString(roleShow->m_pLabelForntBlind, buff);
	sprintf(buff,"%d秒",roleInfo.m_time) ;
	SetLabelString(roleShow->m_pLabelTime, buff);


}