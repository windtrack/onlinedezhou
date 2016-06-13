#include "QuickInviteList.h"
#include "Util.h"
#include "HttpSprite.h"
#include "GlobalUserData.h"
#include "FrameSprite.h"
#include "FrameSpriteButton.h"
#include "MenuItemSpriteWithLabel.h"

USING_NS_CC;

enum SrcollView	//	设置默认的滚动视图的大小
{
	ListScrollW = 362,
	ListScrollH = 408
};

enum Layer
{
	ListLayerW = 368,
	ListLayerH = 91
};

enum FriendType			//好友类型
{
	EATTENT,			//	我关注的
	EDLADY,		        //	德女郎
	EINVITE				//  邀请
};

bool QuickInviteList::init()
{
	if (!GameDialog::init("invitefriends/bg.png", false))
	{
		return false;
	}

	//德女郎
	m_nDLady = MenuItemSpriteWithLabel::create("invitefriends/dlady.png",3,CC_CALLBACK_1(QuickInviteList::menuClickDLady,this));
	m_nDLady->setPosition(67,444);
	m_nDLady->setEnabled(false);
    auto labelLady = CreateLabelMSYHBD(24, "德女郎");
    labelLady->setPosition(m_nDLady->getContentSize().width/2, m_nDLady->getContentSize().height/2);
    m_nDLady->SetLabel(labelLady);
    m_nDLady->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
    m_nDLady->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));

	//我关注的
	m_nAttent = MenuItemSpriteWithLabel::create("invitefriends/attention.png",3,CC_CALLBACK_1(QuickInviteList::menuClickAttent,this));
	m_nAttent->setPosition(187,444);
    auto labelAttent = CreateLabelMSYHBD(24, "我的关注");
    labelAttent->setPosition(m_nAttent->getContentSize().width/2, m_nAttent->getContentSize().height/2);
    m_nAttent->SetLabel(labelAttent);
    m_nAttent->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
    m_nAttent->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));

	//邀请我的
	m_nInvite = MenuItemSpriteWithLabel::create("invitefriends/invite.png",3,CC_CALLBACK_1(QuickInviteList::menuClickInvite,this));
	m_nInvite->setPosition(307,444);
    m_pSpriteInviteFlag = CreateNode1AndPosAddTo<Sprite>("inviteflag.png", 108, 44, m_nInvite, 0);
    m_pSpriteInviteFlag->setVisible(false);
    auto labelInvite = CreateLabelMSYHBD(24, "邀请我的");
    labelInvite->setPosition(m_nInvite->getContentSize().width/2, m_nInvite->getContentSize().height/2);
    m_nInvite->SetLabel(labelInvite);
    m_nInvite->SetEnabledColor(Color4B(0x37,0xa4,0x5c,0xff));
    m_nInvite->SetDisabledColor(Color4B(0xff,0xff,0xff,0xff));

	auto menu = Menu::create(m_nAttent,m_nDLady,m_nInvite,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,2);

	//防止点击穿透
	auto left = MenuItemSpriteWithLabel::create("invitefriends/dlady.png",3,CC_CALLBACK_1(QuickInviteList::menuClickLeft,this));
	left->setPosition(67,444);
	auto center = MenuItemSpriteWithLabel::create("invitefriends/attention.png",3,CC_CALLBACK_1(QuickInviteList::menuClickCenter,this));
	center->setPosition(187,444);
	auto right = MenuItemSpriteWithLabel::create("invitefriends/invite.png",3,CC_CALLBACK_1(QuickInviteList::menuClickRight,this));
	right->setPosition(307,444);

	menu = Menu::create(left,center,right,nullptr);
	menu->setPosition(Vec2::ZERO);
	addChild(menu,1);

	//滚动框
	for(int i=0; i<3; i++){
		m_pScrollView[i] = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ListScrollW,ListScrollH),6,5,this,0);
		m_pScrollView[i]->setContentSize(Size(ListScrollW,ListScrollH));
		m_pScrollView[i]->setDirection(extension::ScrollView::Direction::VERTICAL);
	}

	UpdateView(0);
	UpdateView(1);
	UpdateView(2);
	m_pScrollView[0]->setVisible(false);
	m_pScrollView[2]->setVisible(false);

	return true;
}

//我的关注
void QuickInviteList::menuClickAttent(Ref* pSender)
{
	m_pScrollView[0]->setVisible(true);
	m_pScrollView[1]->setVisible(false);
	m_pScrollView[2]->setVisible(false);
	m_nAttent->setEnabled(false);
	m_nDLady->setEnabled(true);
	m_nInvite->setEnabled(true);
}

//德女郎
void QuickInviteList::menuClickDLady(Ref* pSender)
{
	m_pScrollView[0]->setVisible(false);
	m_pScrollView[1]->setVisible(true);
	m_pScrollView[2]->setVisible(false);
	m_nAttent->setEnabled(true);
	m_nDLady->setEnabled(false);
	m_nInvite->setEnabled(true);
}

//邀请我的
void QuickInviteList::menuClickInvite(Ref* pSender)
{
	m_pScrollView[0]->setVisible(false);
	m_pScrollView[1]->setVisible(false);
	m_pScrollView[2]->setVisible(true);
	m_nAttent->setEnabled(true);
	m_nDLady->setEnabled(true);
	m_nInvite->setEnabled(false);
    m_pSpriteInviteFlag->setVisible(false);
}

//防按钮点击穿透
void QuickInviteList::menuClickLeft(Ref* pSender){}
void QuickInviteList::menuClickCenter(Ref* pSender){}
void QuickInviteList::menuClickRight(Ref* pSender){}

//加入
void QuickInviteList::menuClickJiaRu(unsigned uType,unsigned uAreaID, unsigned uTableID, bool bHasPassWord, unsigned uTableType,unsigned uUserID)
{
	Json::Value param;
	param["rid"] = uAreaID;
	param["tid"] = uTableID;
	param["ifInvite"] = (uType == 2 ? 1 : 0);
	param["haspwd"] = bHasPassWord;
    param["userid"] = uUserID;
	param["tabletype"] = uTableType;
	SendClientRequest(ClientRequest::kJoinTable, param);
}

//邀请
void QuickInviteList::menuClickYaoQing(unsigned uType,unsigned uUID)
{
	Json::Value param;
	param["uid"] = uUID;
	SendClientRequest(ClientRequest::kInvitePlayer, param);	
}
//忽略
void QuickInviteList::menuClickHuLue(unsigned uType, unsigned userID)
{
    for (auto iter = g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].begin(); iter != g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].end(); ++iter)
    {
        if (iter->m_uUserID == userID)
        {
            g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].erase(iter);
            break;
        }
    }
    UpdateView(uType);
}

void QuickInviteList::UpdateView(unsigned uType)
{
    //0：我关注的 1：德女郎
	for(auto item:m_vecFollowListInfoNew[uType])
	{
		item->m_pLayer->setVisible(false);
		m_vecFollowListInfoOld[uType].push_back(item);
	}
	m_vecFollowListInfoNew[uType].clear();
	unsigned uCount = g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType].size();
	unsigned fRealH = uCount * ListLayerH;
	if(fRealH < ListScrollH)
	{
		fRealH = ListScrollH;
	}
	float fX =0,fY = fRealH - ListLayerH;
	for(auto& followList:g_globalQuickFollowInfo.m_vecQuickFollowInfo[uType])
	{
		auto item = GetQuickInviteListItem(uType, followList);
		UpdateItem(item, followList, uType);
		item->m_pLayer->setPosition(fX, fY);
		item->m_pLayer->setVisible(true); 
		m_vecFollowListInfoNew[uType].push_back(item);
		fY -= ListLayerH;     
	}
	float temp = fRealH - ListScrollH;
	//每次都显示滚动框的最上面
	m_pScrollView[uType]->setContentOffset(Vec2(0,-temp), false);
	m_pScrollView[uType]->setContentSize(Size(ListScrollW,fRealH));
}

//获取列表内容
QuickInviteList::QuickInviteListItem *QuickInviteList::GetQuickInviteListItem(unsigned uType, const QuickFollowInfo& userInfo)
{
	QuickInviteListItem *item = nullptr;
	if(m_vecFollowListInfoOld[uType].empty())
	{
		auto layer = Layer::create();
		layer->setContentSize(Size(ListLayerW,ListLayerH));
		layer->setPosition(0,0);
		m_pScrollView[uType]->addChild(layer);
		//头像
		auto pTouXiang = CreateUserHeadSprite("defaulticon.png", "invitefriends/iconclipmask.png", layer, Vec2(7,12), "invitefriends/iconround.png");
		//昵称
		auto labelName = CreateLabelMSYHAndAnchorPosClrAddTo(24,"",Vec2(0,0),84,58,Color4B::WHITE,layer,0);
		//大小盲
		auto labelBlind = CreateLabelMSYHAndAnchorPosClrAddTo(18,"",Vec2(0.5f, 0.5f),281,71,Color4B(0x18,0x5d,0x5d,0xff),layer,0);
		//加入
		auto JiaRuBg = FrameSpriteButton::createWithVertFrameSprite("invitefriends/jiaru.png",false,true);
		JiaRuBg->setPosition(283,36);
		layer->addChild(JiaRuBg);
		auto label = CreateLabelMSYHBD(24,"观战");
		label->setPosition(0,0);
		JiaRuBg->addChild(label);
		//邀请
		AutoDelayEnabledFrameSpriteButton *YaoQingBg = nullptr;
		if(uType == 0){
			YaoQingBg = createButton("invitefriends/yaoqing.png",true,60,Color4B(0,0,0,0),Color4B(0,0,0,178));
			YaoQingBg->setPosition(148,36);
			layer->addChild(YaoQingBg);
			label = CreateLabelMSYHBD(24,"邀请");
			label->setPosition(0,0);
			YaoQingBg->addChild(label);
		}
		// 置灰的邀请
		auto pZhiHuiYaoQingBg = FrameSpriteButton::createWithVertFrameSprite("invitefriends/jiaru.png",false,true);
		pZhiHuiYaoQingBg->setPosition(148,36);
		layer->addChild(pZhiHuiYaoQingBg);
		auto YaoQinglabel = CreateLabelMSYHBD(24,"邀请");
		YaoQinglabel->setPosition(0,0);
		pZhiHuiYaoQingBg->addChild(YaoQinglabel);
		pZhiHuiYaoQingBg->SetEnable(false);
		pZhiHuiYaoQingBg->setVisible(false);

        //加入
        auto hulueBt = FrameSpriteButton::createWithVertFrameSprite("invitefriends/yaoqing.png",false,true);
        hulueBt->setPosition(148,36);
        layer->addChild(hulueBt);
        auto labelHulue = CreateLabelMSYHBD(24,"忽略");
        label->setPosition(0,0);
        hulueBt->addChild(labelHulue);

		//下划线
		auto underline = Sprite::create("invitefriends/underline.png");
		underline->setPosition(ListLayerW / 2,0);
		layer->addChild(underline);
		if(uType == 1)
		{
			auto level = FrameSprite::create("invitefriends/level.png",5,1);
			level->setPosition(20,75);
			layer->addChild(level,1);
		}

		auto pItem = new QuickInviteListItem;
		pItem->m_pLayer = layer;
		pItem->m_pSpriteTouXiang = pTouXiang;
		pItem->m_pLabelName = labelName;
		pItem->m_pLabelBlind = labelBlind;
		pItem->m_pBtJiaRu = JiaRuBg;
		pItem->m_pBtYaoQing = YaoQingBg;
		pItem->m_pBtZhiHuiYaoQing = pZhiHuiYaoQingBg;
		pItem->m_pUnderline = underline;
        pItem->m_pBtHuLue = hulueBt;
		item = pItem;
	}else{
		item=m_vecFollowListInfoOld[uType].back();
		m_vecFollowListInfoOld[uType].pop_back();
	}
	item->m_uUID = userInfo.m_uUserID;
	//加入回调
	item->m_pBtJiaRu->SetHitCB(std::bind(&QuickInviteList::menuClickJiaRu, this, uType, userInfo.m_uAreaID, userInfo.m_uTableID, userInfo.m_bHasPassword, userInfo.m_uTableType,userInfo.m_uUserID));
	//邀请回调
	if(uType == 0){
		item->m_pBtYaoQing->SetHitCB(std::bind(&QuickInviteList::menuClickYaoQing, this, uType,userInfo.m_uUserID));
	}
    //忽略回调
    if (uType == 2)
        item->m_pBtHuLue->SetHitCB(std::bind(&QuickInviteList::menuClickHuLue, this, uType, userInfo.m_uUserID));
	return item;
}

//更新列表
void QuickInviteList::UpdateItem(QuickInviteList::QuickInviteListItem *item, const QuickFollowInfo& followInfo,unsigned uType)
{
	SetIconUrl(item->m_pSpriteTouXiang, followInfo.m_strIconUrl);
	//SetLabelString(item->m_pLabelName,followInfo.m_strNickName);
	char str[128];
	sprintf(str,"(%s/%s)",FormatCash(followInfo.m_dLittleBlind).c_str(),FormatCash(followInfo.m_dBigBlind).c_str());
	SetLabelString(item->m_pLabelBlind,str);
    SetLabelStringWithBoundWidth(item->m_pLabelName, followInfo.m_strNickName, 281- 84 - item->m_pLabelBlind->getContentSize().width/2);
    item->m_pBtHuLue->setVisible(uType == 2 ? true : false);
	bool bSameTable = false;
	if (followInfo.m_uTableID == g_globalTableConfig.m_uTableID && followInfo.m_uAreaID == g_globalTableConfig.m_uRoomID)
	{
		bSameTable = true;
	}
	if (bSameTable)
	{
		item->m_pBtJiaRu->SetEnable(false);
	}
	else
	{
		if(followInfo.m_bOnLine)
		{
			item->m_pBtJiaRu->SetEnable(true);
			item->m_pBtJiaRu->SetHitCB(std::bind(&QuickInviteList::menuClickJiaRu, this, uType, followInfo.m_uAreaID, followInfo.m_uTableID, followInfo.m_bHasPassword,followInfo.m_uTableType, followInfo.m_uUserID));
		}
		else
		{
			item->m_pBtJiaRu->SetEnable(false);
		}
	}

	if (item->m_pBtYaoQing)
	{
		if (bSameTable || !followInfo.m_bOnLine)
		{
			item->m_pBtYaoQing->setVisible(false);
			item->m_pBtZhiHuiYaoQing->setVisible(true);
		}
		else
		{
			item->m_pBtYaoQing->setVisible(true);
			item->m_pBtZhiHuiYaoQing->setVisible(false);
		}
		if(followInfo.m_uTableID == g_globalTableConfig.m_uTableID && followInfo.m_uAreaID == g_globalTableConfig.m_uRoomID)
		{
			item->m_pBtYaoQing->setVisible(false);
			item->m_pBtZhiHuiYaoQing->setVisible(true);
		}
	}
}

//更改对应记录内容
void QuickInviteList::UpdateItemView(unsigned uType, const QuickFollowInfo& userInfo)
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
void QuickInviteList::AppadnItemView(unsigned uType, const QuickFollowInfo& userInfo)
{
	auto &vecFollow = m_vecFollowListInfoNew[uType];
	auto item = GetQuickInviteListItem(uType, userInfo);
	vecFollow.push_back(item);
	item->m_pLayer->setVisible(true);
	UpdateItem(item, userInfo, uType);
	auto sz = m_pScrollView[uType]->getContentSize();
	float fRealH = vecFollow.size() * ListLayerH;
	if(fRealH < ListScrollH){
		m_pScrollView[uType]->setContentSize(Size(ListScrollW,ListScrollH));
		m_pScrollView[uType]->setContentOffset(Vec2(0,0),false);
		float fX = 0,fY = ListScrollH - fRealH;
		for(auto followList:vecFollow)
		{
			followList->m_pLayer->setPosition(fX, fY);
			fY += ListLayerH;     
		}
	}else{
		m_pScrollView[uType]->setContentSize(Size(ListScrollW,fRealH));
		sz = m_pScrollView[uType]->getContentSize();
		float fY = 0;
		for(auto followList:m_vecFollowListInfoNew[uType])
		{
			followList->m_pLayer->setPositionY(fY);
			fY += ListLayerH;     
		}
		float temp = fRealH - ListScrollH;
		m_pScrollView[uType]->setContentOffset(Vec2(0,-temp), false);
	}	
}

//替换数据
void QuickInviteList::ReplaceAndTopItemView(unsigned uType, const QuickFollowInfo& userInfo)
{
	//auto &vecFollow = m_vecFollowListInfoNew[uType];
	/*auto item = GetQuickInviteListItem(uType, userInfo);
	UpdateItem(item, userInfo, uType);*/
	UpdateView(uType);
	/*for(int i=0; i<vecFollow.size(); i++)
	{
		if(item->m_uUID == vecFollow[i]->m_uUID)
		{
			vecFollow.erase(vecFollow.begin()+i);
			vecFollow.push_back(item);
			auto sz = m_pScrollView[uType]->getContentSize();
			float fX = 0, fY = sz.height-ListLayerH;
			for(int j=vecFollow.size()-1; j>=0; j--)
			{
				vecFollow[j]->m_pLayer->setPosition(fX,fY);
				fY -= ListLayerH;
			}
			break;
		}
	}*/
}

//删除记录
void QuickInviteList::RemoveItemView(unsigned uType, unsigned uUserID)
{
	auto &vecFollow = m_vecFollowListInfoNew[uType];
	for(int i=0; i<vecFollow.size(); i++)
	{
		if(vecFollow[i]->m_uUID == uUserID)
		{
			unsigned uItemH = 78;
			vecFollow[i]->m_pLayer->setVisible(false);
			vecFollow.erase(vecFollow.begin() + i);
			auto sz = m_pScrollView[uType]->getContentSize();
			float fRealH = vecFollow.size() * ListLayerH;
			if(fRealH < ListScrollH){
				m_pScrollView[uType]->setContentSize(Size(ListScrollW,ListScrollH));
				m_pScrollView[uType]->setContentOffset(Vec2(0,0),false);
				float fX = 0,fY = ListScrollH - fRealH;
				for(auto followList:vecFollow)
				{
					followList->m_pLayer->setPosition(fX, fY);
					fY += ListLayerH;     
				}
			}else{
				float fX = 0,fY = 0;
				m_pScrollView[uType]->setContentSize(Size(ListScrollW,fRealH));
				for(auto followList:vecFollow)
				{
					followList->m_pLayer->setPosition(fX, fY);
					fY += ListLayerH;     
				}
				float temp = fRealH - ListScrollH;
				m_pScrollView[uType]->setContentOffset(Vec2(0,-temp), false);
			}
		}
	}
}

void QuickInviteList::ShowInviteFlag(bool flag)
{
    m_pSpriteInviteFlag->setVisible(flag);
}

void QuickInviteList::setVisible(bool bVisible)
{
    if (bVisible)
    {
		/*UpdateView(0);
		UpdateView(1);
		UpdateView(2);*/
        auto actScale = Spawn::create(ScaleTo::create(0.2, 1.0), FadeIn::create(0.2), nullptr);
        runAction(Sequence::create(CallFunc::create([this](){setOpacity(0);setScale(0.618);GameDialog::setVisible(true);}), actScale, nullptr));
    }
    else
    {
        auto actScale = Spawn::create(ScaleTo::create(0.2, 0.618), FadeOut::create(0.2), nullptr);
        runAction(Sequence::create(actScale, CallFunc::create([this](){GameDialog::setVisible(false);}), nullptr));
    }
}