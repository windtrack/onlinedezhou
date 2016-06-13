#include "QuickFollowList.h"
#include "FrameSprite.h"
#include "Util.h"
#include "MenuItemSpriteWithLabel.h"
#include "FrameSpriteButton.h"
#include "GlobalUserData.h"
#include "HttpSprite.h"
#include <vector>
#include "Tools.h"
USING_NS_CC;

enum SrcollView	//	设置默认的滚动视图的大小
{
	ListScrollW = 356,
	ListScrollH = 409
	//ListScrollW = 286,
	//ListScrollH = 364
};

enum BgItem
{
	ListBgItemW = 256,
	ListBgItemH = 73
};
enum FriendType			//好友类型
{
	EATTENT,			//	我关注的
	EDLADY		        //	德女郎
};

QuickFollowList* QuickFollowList::creatWithLayer(cocos2d::ui::ImageView* view)
{
	QuickFollowList* quick = new QuickFollowList() ;
	if (quick&&quick->initWithLayer(view))
	{
		quick->autorelease();
		return quick ;
	}
	else
	{
		CC_SAFE_DELETE(quick) ;
		return nullptr ;
	}


}

bool QuickFollowList::initWithLayer(cocos2d::ui::ImageView* view)
{
	do 
	{


	auto bg =  static_cast<ImageView*>(Tools::getWidgetByName(view,"Image_liebiao"));

	//关注
	m_nAttent = static_cast<Button*>(Tools::getWidgetByName(bg,"Button_guanzhu"));
	m_nAttent->addTouchEventListener(this,toucheventselector(QuickFollowList::menuClickAttent));
	m_nAttent->setEnabled(true);
	m_nAttent->setBright(false);
	//德女郎
	m_nDLady = static_cast<Button*>(Tools::getWidgetByName(bg,"Button_denvlang"));
	m_nDLady->addTouchEventListener(this,toucheventselector(QuickFollowList::menuClickDLady));
	m_nDLady->setEnabled(false);
	m_nDLady->setBright(true);


	Button* touchup = static_cast<Button*>(Tools::getWidgetByName(bg,"Button_touchup"));
	touchup->addTouchEventListener(this,toucheventselector(QuickFollowList::menuCTTouch));


	Button* touchDown = static_cast<Button*>(Tools::getWidgetByName(bg,"Button_touchdown"));
	touchDown->addTouchEventListener(this,toucheventselector(QuickFollowList::menuCTTouch));


	auto lvPanel = static_cast<Layout*>(Tools::getWidgetByName(bg, "Panel_listview"));

	sizeListView.width = ListScrollW;
	sizeListView.height = lvPanel->getContentSize().height;

	for(int i=1; i >= 0; i--){
		m_pScrollView[i] = CreateNode1AndPosAddTo<extension::ScrollView>(sizeListView,0,4,lvPanel,0);
		m_pScrollView[i]->setContentSize(sizeListView);
		//m_pScrollView[i]->setContentSize(Size(ListScrollW,ListScrollH));
		m_pScrollView[i]->setBounceable(true);
		m_pScrollView[i]->setSwallowsTouches(false) ;
		m_pScrollView[i]->setDirection(extension::ScrollView::Direction::VERTICAL);
		m_pScrollView[i]->setDelegate(this);//看这里！！
	}

	UpdateView(0);
	UpdateView(1);
	m_pScrollView[0]->setVisible(false);


	isClickButton= false ;
		return true ;
	} while (0);
	return false ;
}

void QuickFollowList::menuCTTouch(Ref* pSender,cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		isClickButton = true ;
		return ;
	}
	if (type == TOUCH_EVENT_CANCELED)
	{
		isClickButton = false ;
		return ;
	}
	if (type == TOUCH_EVENT_ENDED)
	{
		isClickButton= false ;
		return ;
	}
}

bool QuickFollowList::init()
{
	if ( !Layer::init() )
    {
        return false;
    }

	return true;
}

//关注
void QuickFollowList::menuClickAttent(Ref* pSender,cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		isClickButton = true ;
		return ;
	}
	if (type == TOUCH_EVENT_CANCELED)
	{
		isClickButton = false ;
		return ;
	}
	if (type == TOUCH_EVENT_ENDED)
	{
		isClickButton= false ;
		m_pScrollView[0]->setVisible(true);
		m_pScrollView[1]->setVisible(false);

		m_nAttent->setBright(true) ;
		m_nAttent->setEnabled(false);
		m_nDLady->setBright(false);
		m_nDLady->setEnabled(true);
	}

}
//德女郎
void QuickFollowList::menuClickDLady(Ref* pSender,cocos2d::ui::TouchEventType type)
{
	if (type == TOUCH_EVENT_BEGAN)
	{
		isClickButton = true ;
		return ;
	}
	if (type == TOUCH_EVENT_CANCELED)
	{
		isClickButton = false ;
		return ;
	}
	if (type == TOUCH_EVENT_ENDED)
	{
		isClickButton= false ;
		m_pScrollView[0]->setVisible(false);
		m_pScrollView[1]->setVisible(true);
		m_nAttent->setBright(false);
		m_nAttent->setEnabled(true);
		m_nDLady->setBright(true);
		m_nDLady->setEnabled(false);
	}

}

//防止按钮点击穿透
void QuickFollowList::menuClickLeft(Ref* pSender){}
void QuickFollowList::menuClickRight(Ref* pSender){}

//加入
void QuickFollowList::menuClickJiaRu(unsigned uAreaID, unsigned uTableID, bool bHasPassword,unsigned uTableType, unsigned uUserID)
{
	if (isClickButton)
	{
		return ;
	}
	Json::Value param;
	param["rid"] = uAreaID;
	param["tid"] = uTableID;
	param["ifInvite"] = 0;
	param["haspwd"] = bHasPassword;
	param["tabletype"] = uTableType;
    param["userid"] = uUserID;
	SendClientRequest(ClientRequest::kJoinTable, param);
}

//点击背景
void QuickFollowList::menuClickBgItem(unsigned uUID)
{
	if (isClickButton)
	{
		return ;
	}
	Json::Value param;
	param["tid"] = uUID;
	SendClientRequest(ClientRequest::kPopUserDetailInfo, param);
}



//获取好友列表
//QuickFollowList::QuickFollowListItem *QuickFollowList::GetQuickFollowListItem(unsigned m_uUID, unsigned uAreaID, unsigned uTableID,bool bHasPassWord, unsigned uType,unsigned uTableType)
QuickFollowList::QuickFollowListItem *QuickFollowList::GetQuickFollowListItem(QuickFollowInfo followList,unsigned uType)
{//auto item = GetQuickFollowListItem(followList.m_uUserID, followList.m_uAreaID, followList.m_uTableID, followList.m_bHasPassword, uType,followList.m_uTableType);
	QuickFollowListItem *item = nullptr;
	if(m_vecFollowListInfoOld[uType].empty())
	{
		auto bgItem = FrameSprite::create("quickfriend/bgfriend.png",1,1);
		bgItem->setPosition(sizeListView.width / 2, 0);
		auto btDetail = LayerButton::create(false, true);
		bgItem->addChild(btDetail);
		m_pScrollView[uType]->addChild(bgItem);
		//头像
		auto pTouXiang = CreateUserHeadSprite("defaulticon.png", "quickfriend/friendiconclipmask.png", bgItem, Vec2(8,5), "quickfriend/friendiconround.png");
		//昵称
		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(22,"",Vec2(0,0),85,39,Color4B::WHITE,bgItem,0);
		//游戏状态
		auto labelStatus = CreateLabelMSYHAndAnchorPosClrAddTo(20,"",Vec2(0,0),85,12,Color4B(0xff,0xd2,0x00,0xff),bgItem,0);
		//auto labelStatus = CreateLabelMSYHAndAnchorPosClrAddTo(20,"",Vec2(0,0),85,12,Color4B(0xfc,0xcd,0x03,0xff),bgItem,0);
		//加入按钮
		auto JiaRuBg = FrameSpriteButton::createWithVertFrameSprite("quickfriend/join.png",false,true);
		JiaRuBg->setPosition(290,37);
		bgItem->addChild(JiaRuBg);

		//德女郎等级
		if(uType == 1)
		{
			auto level = FrameSprite::create("quickfriend/level.png",5,1);
			level->setPosition(10,60);
			bgItem->addChild(level,1);
		}

		auto pItem = new QuickFollowListItem;
		pItem->m_pSpriteBgItem = bgItem;
		pItem->m_pBtDetail = btDetail;
		pItem->m_pSpriteTouXiang = pTouXiang;
		pItem->m_pLabelUp = labelName;
		pItem->m_pLabelDown = labelStatus;
		pItem->m_pBtJiaRu = JiaRuBg;
		item = pItem;
	}else{
		item=m_vecFollowListInfoOld[uType].back();
		m_vecFollowListInfoOld[uType].pop_back();
	}
	item->m_uUID = followList.m_uUserID;
	//加入回调
	item->m_pBtJiaRu->SetHitCB(std::bind(&QuickFollowList::menuClickJiaRu, this, followList.m_uAreaID, followList.m_uTableID, followList.m_bHasPassword,followList.m_uTableType, followList.m_uUserID));
	//背景回调
	item->m_pBtDetail->SetHitCB(std::bind(&QuickFollowList::menuClickBgItem, this, followList.m_uUserID));
	return item;
}

//更新列表里面的内容
void QuickFollowList::UpdateItem(QuickFollowList::QuickFollowListItem *item, const QuickFollowInfo& followInfo,unsigned uType)
{
	SetIconUrl(item->m_pSpriteTouXiang, followInfo.m_strIconUrl);
	SetLabelStringWithBoundWidth(item->m_pLabelUp,followInfo.m_strNickName, 155);
	if(followInfo.m_bOnLine){
		char str[128];
		sprintf(str,"正在%s/%s",FormatCash(followInfo.m_dLittleBlind).c_str(),FormatCash(followInfo.m_dBigBlind).c_str());
		SetLabelString(item->m_pLabelDown, str);
		item->m_pBtJiaRu->setVisible(true);
		item->m_pBtDetail->setContentSize(Size(ListBgItemW - 62,ListBgItemH));
		item->m_pBtJiaRu->SetHitCB(std::bind(&QuickFollowList::menuClickJiaRu, this, followInfo.m_uAreaID, followInfo.m_uTableID, followInfo.m_bHasPassword,followInfo.m_uTableType, followInfo.m_uUserID));
	}else if(!followInfo.m_bOnLine){
		SetLabelString(item->m_pLabelDown,"正在闲逛...");
		item->m_pBtJiaRu->setVisible(false);
		item->m_pBtDetail->setContentSize(Size(ListBgItemW,ListBgItemH));
	}
}

//改变对应的记录
void QuickFollowList::UpdateItemView(unsigned uType, const QuickFollowInfo& userInfo)
{
	for(auto item:m_vecFollowListInfoNew[uType])
	{
		if(item->m_uUID == userInfo.m_uUserID)
		{
			UpdateItem(item, userInfo, uType);
		}
	}
}

//追加记录
void QuickFollowList::AppadnItemView(unsigned uType, const QuickFollowInfo& userInfo)
{
	/*g_globalQuickFollowInfo.m_vecQuickFollowInfo->push_back(userInfo);
	callNextPage(uType) ;*/

	//auto item = GetQuickFollowListItem(userInfo.m_uUserID, userInfo.m_uAreaID, userInfo.m_uTableID,userInfo.m_bHasPassword, uType,userInfo.m_uTableType);
    auto item = GetQuickFollowListItem(userInfo, uType);
	m_vecFollowListInfoNew[uType].push_back(item);
	item->m_pSpriteBgItem->setVisible(true);
	UpdateItem(item, userInfo, uType);
	unsigned uItemH = 78;
	auto sz = m_pScrollView[uType]->getContentSize();
	float fRealH = m_vecFollowListInfoNew[uType].size() * uItemH;
	if(fRealH < sizeListView.height){
		m_pScrollView[uType]->setContentSize(sizeListView);
		m_pScrollView[uType]->setContentOffset(Vec2(0,0));
		float fX = sizeListView.width / 2,fY = sizeListView.height - fRealH;
		for(auto followList:m_vecFollowListInfoNew[uType])
		{
			followList->m_pSpriteBgItem->setPosition(fX, fY + ListBgItemH / 2);
			fY += uItemH;     
		}
	}else{
		m_pScrollView[uType]->setContentSize(Size(sizeListView.width,fRealH));
		sz = m_pScrollView[uType]->getContentSize();
		float fY = 5 + ListBgItemH / 2;
		for(auto followList:m_vecFollowListInfoNew[uType])
		{
			followList->m_pSpriteBgItem->setPositionY(fY);
			fY += uItemH;     
		}
		float temp = fRealH - sizeListView.height;
		m_pScrollView[uType]->setContentOffset(Vec2(0,-temp),false);
	}
}

//删除记录
void QuickFollowList:: RemoveItemView(unsigned uType, unsigned uUserID)
{
	for(int i=0; i<m_vecFollowListInfoNew[uType].size(); i++)
	{
		if(m_vecFollowListInfoNew[uType][i]->m_uUID == uUserID)
		{
			unsigned uItemH = 78;
			m_vecFollowListInfoNew[uType][i]->m_pSpriteBgItem->setVisible(false);
			m_vecFollowListInfoNew[uType].erase(m_vecFollowListInfoNew[uType].begin() + i);
			auto sz = m_pScrollView[uType]->getContentSize();
			float fRealH = m_vecFollowListInfoNew[uType].size() * uItemH;
			if(fRealH < sizeListView.height){
				m_pScrollView[uType]->setContentSize(sizeListView);
				m_pScrollView[uType]->setContentOffset(Vec2(0,0));
				float fX = sizeListView.width / 2,fY = sizeListView.height - fRealH;
				for(auto followList:m_vecFollowListInfoNew[uType])
				{
					followList->m_pSpriteBgItem->setPosition(fX, fY + ListBgItemH / 2);
					fY += uItemH;     
				}
				break;
			}else{
				float fX = sizeListView.width / 2,fY = 0;
				m_pScrollView[uType]->setContentSize(Size(sizeListView.width,fRealH));
				for(auto followList:m_vecFollowListInfoNew[uType])
				{
					followList->m_pSpriteBgItem->setPosition(fX, fY + ListBgItemH / 2);
					fY += uItemH;     
				}
				float temp = fRealH - sizeListView.height;
				m_pScrollView[uType]->setContentOffset(Vec2(0,-temp),false);
				break;
			}
		}
	}
}




//初始化好友列表
void QuickFollowList::UpdateView(unsigned uType)
{
	//log("update view %d", uType);
	for(auto item:m_vecFollowListInfoNew[uType])
	{
		item->m_pSpriteBgItem->setVisible(false);
		m_vecFollowListInfoOld[uType].push_back(item);
	}
	m_vecFollowListInfoNew[uType].clear();

	unsigned uCount = g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].size();

	m_curPage[uType] =0 ;

	callNextPage(uType) ;

}


void QuickFollowList::callNextPage(int uType)
{
	unsigned uCount = g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].size();
	m_maxPage[uType] = (uCount%MAX_PAGECOUNT==0)?(uCount/MAX_PAGECOUNT):(uCount/MAX_PAGECOUNT)+1 ;
	if (m_curPage[uType]+1 > m_maxPage[uType])
	{
		return ;
	}

	int nextPage = m_curPage[uType]+1 ;

	int curMaxCount =  nextPage*MAX_PAGECOUNT>=uCount?uCount: nextPage*MAX_PAGECOUNT ;
	int preCount = m_curPage[uType]*MAX_PAGECOUNT>=uCount?uCount: m_curPage[uType]*MAX_PAGECOUNT ;
	preCount = m_vecFollowListInfoNew[uType].size() ;


	unsigned ListBgItemH = 76;
	unsigned uItemH = 81;
	unsigned fRealH = curMaxCount * uItemH;

	if(fRealH < sizeListView.height)
	{
		fRealH = sizeListView.height;
	}
	float fX = sizeListView.width / 2;
	float fY = fRealH - uItemH / 2 - 5;

	for (int i = 0; i <curMaxCount; i++)
	{
		if (i<preCount&&i<m_vecFollowListInfoNew[uType].size())
		{
			QuickFollowListItem *item = m_vecFollowListInfoNew[uType][i];
			item->m_pSpriteBgItem->setPosition(fX, fY);
			item->m_pSpriteBgItem->setVisible(true); 		
		}
		else
		{
			QuickFollowInfo followList = g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].at(i) ;
			//auto item = GetQuickFollowListItem(followList.m_uUserID, followList.m_uAreaID, followList.m_uTableID, followList.m_bHasPassword, uType,followList.m_uTableType);
            auto item = GetQuickFollowListItem(followList, uType);
			UpdateItem(item, followList, uType);
			item->m_pSpriteBgItem->setPosition(fX, fY);
			item->m_pSpriteBgItem->setVisible(true); 
			m_vecFollowListInfoNew[uType].push_back(item);
			
		}
			fY -= uItemH;  
	}

																																										

	float temp = fRealH - sizeListView.height;
	//每次都显示滚动框的最上面

	if (m_curPage[uType]==0)
	{
			m_pScrollView[uType]->setContentOffset(Vec2(0,-temp), false);
	}
	else
	{
		float height = uItemH*MAX_PAGECOUNT ;
		float offesty = m_pScrollView[uType]->getContentOffset().y;
		float py = -offesty+height;
		m_pScrollView[uType]->setContentOffset(Vec2(0,-py), false);
	}

	m_pScrollView[uType]->setContentSize(Size(sizeListView.width,fRealH));
	m_curPage[uType]++ ;

}



void QuickFollowList::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

	int type =0 ;
	for (int i=0; i<2;i++)
	{
		if (view == m_pScrollView[i])
		{
			type = i ;
		}
	}

	auto offSet = view->getContentOffset();
	auto vSzie = view->getContentSize() ;

	if (offSet.y > 5)
	{
		unsigned uCount = g_globalQuickFollowInfo.m_vecQuickFollowInfo[type].size();
		m_maxPage[type] = (uCount%MAX_PAGECOUNT==0)?(uCount/MAX_PAGECOUNT):(uCount/MAX_PAGECOUNT)+1 ;
		if (m_curPage[type]+1 > m_maxPage[type])
		{
			return ;
		}
		//view->setTouchEnabled(false) ;
		callNextPage(type) ;
		//view->setTouchEnabled(true) ;
	}

}