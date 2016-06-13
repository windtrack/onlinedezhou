#include "GameRecord.h"
#include "MenuItemSpriteWithLabel.h"
#include "FrameSpriteButton.h"
#include "FrameScale9SpriteButton.h"
#include "FrameSprite.h"
#include "GlobalUserData.h"
#include "GameCommon.h"
#include "Util.h"
#include "stdio.h"

USING_NS_CC;

enum RecordSrcollView        //设置默认的滚动视图的大小
{
    ScrollW = 810,
    ScrollH = 378
};

void GameRecord::UpdataRecordData()
{
    unsigned count = 0;

    for (auto offSet : g_globalGameRecord.m_vecPageIdx[m_uSortType])
    {
        count++;
        if (count == m_uCurPage && m_uCurPage != 1)
        {
            UpdataRecordView(m_uSortType, offSet);
        }
        else if (count == 1 && m_uCurPage == 1)
        {
            UpdataRecordView(m_uSortType, 0);
        }
    }

    if (m_uSortType == 0)
    {
        m_btTimeSort->setEnabled(false);
        m_btProfitSort->setEnabled(true);
    }
    else if (m_uSortType == 1)
    {
        m_btProfitSort->setEnabled(false);
        m_btTimeSort->setEnabled(true);
    }
}

bool GameRecord::init()
{
    if (!GameDialog::init("dialog/bg.png", true))
    {
        return false;
    }

    auto title = CreateNode1AndPosAddTo<Sprite>("gamerecordlist/titlegamerecordlist.png", 470, 568, this, 0);

    //滚动视图
    m_ScrollView = CreateNode1AndPosAddTo<extension::ScrollView>(Size(ScrollW, ScrollH), 64, 30, this, 0);
    m_ScrollView->setContentSize(Size(ScrollW, ScrollH));
    m_ScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);

    //时间排序
    m_btTimeSort = MenuItemSpriteWithLabel::create("userinfo/btchangguichang.png", 3, CC_CALLBACK_1(GameRecord::menuTimeSort, this));
    m_btTimeSort->setAnchorPoint(Vec2(0.5, 0.5));
    m_btTimeSort->setPosition(375,484);
    auto menuTimeSort = Menu::create(m_btTimeSort, nullptr);
    menuTimeSort->setPosition(Vec2::ZERO);
    addChild(menuTimeSort);
    auto label = CreateLabelMSYHBD(26, "时间排序");
    m_btTimeSort->SetLabel(label);
    m_btTimeSort->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_btTimeSort->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

    //盈利排序
    m_btProfitSort = MenuItemSpriteWithLabel::create("userinfo/btmtt.png", 3, CC_CALLBACK_1(GameRecord::menuProfitSort, this));
    m_btProfitSort->setAnchorPoint(Vec2(0.5, 0.5));
    m_btProfitSort->setPosition(562, 484);
    auto menuProfitSort = Menu::create(m_btProfitSort, nullptr);
    menuProfitSort->setPosition(Vec2::ZERO);
    addChild(menuProfitSort);
    label = CreateLabelMSYHBD(26, "盈利排序");
    m_btProfitSort->SetLabel(label);
    m_btProfitSort->SetEnabledColor(cocos2d::Color4B(0x37,0xa4,0x5c,0xff));
    m_btProfitSort->SetDisabledColor(cocos2d::Color4B(0xff,0xff,0xff,0xff));

    m_labelIsVIP = CreateLabelMSYH(22, "");
	unsigned vipRecoder[6] = {200, 300, 500, 800, 1000, 2000};
	if (g_globalMyData.GetVipLevel() > 0)
    {
		char szText[100] = {0};
		sprintf(szText, "您是VIP,显示最近%d手牌局记录", vipRecoder[g_globalMyData.GetVipLevel() - 1]);
        SetLabelString(m_labelIsVIP, szText);
    }
    else
    {
        SetLabelString(m_labelIsVIP, "您不是VIP,只显示最近100手牌");
    }
    m_labelIsVIP->setAnchorPoint(Vec2(0.5, 0.5));
    m_labelIsVIP->setPosition(468, 433);
    m_labelIsVIP->setTextColor(cocos2d::Color4B(0x79,0xdc,0x98,0xff));
    addChild(m_labelIsVIP);

    //上一页  下一页
    m_btPrePage = FrameSpriteButton::createWithVertFrameSprite("commonbt/pageup.png", false, true);
    m_btNextPage = FrameSpriteButton::createWithVertFrameSprite("commonbt/pagedown.png", false, true); 
    m_btNextPage->retain();
    m_btPrePage->retain();
    m_ScrollView->addChild(m_btNextPage);
    m_ScrollView->addChild(m_btPrePage);

    //UpdataRecordData();
    m_uCurPage = 1;
    m_uSortType = 0;
    return true;
}

void GameRecord::menuTimeSort(cocos2d::Ref* pSender)
{
    m_btTimeSort->setEnabled(false);
    m_btProfitSort->setEnabled(true);

    m_uCurPage = 1;
    m_uSortType = 0;
    UpdataRecordView(0, 0);
}

void GameRecord::menuProfitSort(cocos2d::Ref* pSender)
{
    m_btProfitSort->setEnabled(false);
    m_btTimeSort->setEnabled(true);

    m_uCurPage = 1;
    m_uSortType = 1;
     if (g_globalGameRecord.m_vecPageIdx[1].size() == 0)
     {
         Json::Value param;
         param["page"] = 0;
         param["sortType"] = 2;
         param["countPerPage"] = GameRecordPerPage;
         SendClientRequest(ClientRequest::kGetGamePlayBackList, param);
     }
     else
     {
         UpdataRecordView(1, 0);
     }
}

void GameRecord::UpdataRecordView(unsigned sortMethod, unsigned pageOffset)
{
    bool bpageUp = false, bpageDown = false, bpageSmall = false;
    for (auto item : m_vecRecordNew)
    {
        item->m_pSpriteBgItem->setVisible(false);
        m_vecRecordOld.push_back(item);
    }
    m_vecRecordNew.clear();
    Size sz = m_btNextPage->getContentSize();

    unsigned uAllItem = 0;
    if (m_uCurPage == g_globalGameRecord.m_uAllPage)
    {
        uAllItem = g_globalGameRecord.m_vecRecord[sortMethod].size() - pageOffset;
    }
    else
    {
        uAllItem = GameRecordPerPage;
    }
    
    unsigned uItemH = 110;  //条目高度 + 条目间隙
    unsigned uAllItemH = uItemH*uAllItem;
    float fX = ScrollW / 2, fY = 55;

    if (m_uCurPage < g_globalGameRecord.m_uAllPage)
    {
        uAllItemH += sz.height;
        bpageDown = true;
    }
    if (m_uCurPage > 1)
    {
        uAllItemH += sz.height;
        bpageUp = true;
    }
    if (uAllItemH < ScrollH)
    {
        fY = ScrollH - uAllItemH + fY;
        uAllItemH = ScrollH;
        bpageSmall = true;
    }
    double offSet = ScrollH - (int)uAllItemH;
    fY = uAllItemH;
    m_ScrollView->setContentOffset(Vec2(0, offSet), false);

    if (bpageUp)
    {
        //当前页大于第一页，创建上一页按钮
        fY = fY - sz.height/2;
        m_btPrePage->setVisible(true);
        m_btPrePage->setPosition(fX, fY);
        m_btPrePage->SetHitCB(std::bind(&GameRecord::OnPageTurnUp, this));
        m_btPrePage->retain();
        fY = fY - uItemH / 2 - sz.height / 2;
    }
    else
    {
        fY = fY - uItemH / 2 - 5;
        m_btPrePage->setVisible(false);
        m_btPrePage->retain();
    }

    /*int count = 0;
    int onePageNum = 0;
    for (auto recordItem : g_globalGameRecord.m_vecRecord[sortMethod])
    {
        if (count >= pageOffset && count < pageOffset + GameRecordPerPage)
        {
            onePageNum ++;
            auto item = GetRecordItem(recordItem.m_strID);
            UpdateRecordItem(item, recordItem);
            item->m_pSpriteBgItem->setPosition(fX, fY);
            item->m_pSpriteBgItem->setVisible(true);
            m_vecRecordNew.push_back(item);
            fY -= uItemH;
        }
        if (onePageNum > 20)break;
        count ++;
    }*/
    int vecSz = g_globalGameRecord.m_vecRecord[sortMethod].size();
    for (int i=0; i<GameRecordPerPage; i++)
    {
        if (pageOffset + i >= vecSz)break;
        auto item = GetRecordItem(g_globalGameRecord.m_vecRecord[sortMethod][pageOffset+i].m_strID);
        UpdateRecordItem(item, g_globalGameRecord.m_vecRecord[sortMethod][pageOffset+i]);
        item->m_pSpriteBgItem->setPosition(fX, fY);
        item->m_pSpriteBgItem->setVisible(true);
        m_vecRecordNew.push_back(item);
        fY -= uItemH;
    }

    if (bpageDown)
    {
        fY = sz.height/2;
        //当前页小于总页数，创建下一页按钮
        m_btNextPage->setVisible(true);
        m_btNextPage->setPosition(fX, fY);
        m_btNextPage->SetHitCB(std::bind(&GameRecord::OnPageTurnDown, this));
        m_btNextPage->retain();
    }
    else
    {
        m_btNextPage->setVisible(false);
        m_btNextPage->retain();
    }

    m_ScrollView->setContentSize(Size(ScrollW, uAllItemH));
}

GameRecord::RecordItem* GameRecord::GetRecordItem(std::string strRecordID)
{
    RecordItem *recordItem = nullptr;
    if (m_vecRecordOld.empty())
    {
        //背景条目
        auto bgItem = CreateNode3AndPosAddTo<FrameSprite>("gamerecordlist/bgitem.png", 1, 2, 330, 46, m_ScrollView, 0);

        //第几手牌
        auto cardRecord =  CreateLabelMSYHAndAnchorPosClrAddTo(26, "", Vec2(0, 0), 119, 54, Color4B::WHITE, bgItem, 0);

        //房间名称
        auto labelRoomName =  CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0, 0), 122, 14, Color4B(0x50,0xbd,0x2e,0xff), bgItem, 0);

        //盈利
        auto label =  CreateLabelMSYHAndAnchorPosClrAddTo(26, "盈利", Vec2(0, 0), 422, 54, Color4B(0xff,0xd2,0x00,0xff), bgItem, 0);
        auto labelProfit =  CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0, 0), 480, 58, Color4B(0x79,0xdc,0x98,0xff), bgItem, 0);

        //游戏时间
        auto timeBg = CreateNode3AndPosAddTo<FrameSprite>("gamerecordlist/timebg.png", 1, 1, 621, 28, bgItem, 0);
        Size sz = timeBg->getContentSize();
        auto lableTime = CreateLabelMSYHAndAnchorPosClrAddTo(22, "", Vec2(0.5, 0.5), sz.width/2, sz.height/2, Color4B(0x79,0xdc,0x98,0xff), timeBg, 0);

        //回顾
        //auto btReview = FrameSpriteButton::createWithVertFrameSprite("commonbt/framebtn1.png", false, true);
        auto btReview = FrameScale9SpriteButton::createWithScale9Sprite(3, Size(114, 86), false, true);
        btReview->setPosition(739, 54);
        bgItem->addChild(btReview);
        CreateLabelMSYHBDAndAnchorPosClrAddTo(30, "回顾", Vec2(0.5, 0.5), 0, 0, Color4B::WHITE, btReview, 0);
        //手牌
        auto cardLeft = FrameSprite::create("handcard0.png", 13, 5);
        cardLeft->setPosition(50, 52);
		cardLeft->setScale(0.7);
        bgItem->addChild(cardLeft, 1);
        auto cardRight = FrameSprite::create("handcard1.png", 13, 5);
        cardRight->setPosition(81, 52);
		cardRight->setScale(0.7);
        bgItem->addChild(cardRight, 2);

        auto pItem = new RecordItem;
        pItem->m_pSpriteBgItem = bgItem;
        pItem->m_pLableRecordNum = cardRecord;
        pItem->m_pLableRoomName = labelRoomName;
        pItem->m_pLableProfit = labelProfit;
        pItem->m_pLableGameTime = lableTime;
        pItem->m_pSpriteBtn = btReview;
        pItem->m_pSpriteHandCardL = cardLeft;
        pItem->m_pSpriteHandCardR = cardRight;
        recordItem = pItem;
    }
    else
    {
        recordItem = m_vecRecordOld.back();
        m_vecRecordOld.pop_back();

    }
    recordItem->m_pSpriteBtn->SetHitCB([strRecordID](Ref*){
        Json::Value param;
        param["id"] = strRecordID;
        SendClientRequest(ClientRequest::kGetGamePlayBack, param);
    });

    return recordItem;
}

void GameRecord::UpdateRecordItem(GameRecord::RecordItem *item, const GameRecordInfo& recordInfo)
{
    //刷新第几手牌
    char record[128] = {0};
    sprintf(record, "第%d手牌", recordInfo.m_uIdx);
    SetLabelString(item->m_pLableRecordNum, record);

    //刷新房间名称
    //char roomName[128]={0};
	//sprintf(record,"%s",recordInfo.m_strTableName.c_str()) ;
    sprintf(record,"%s  ( %d人 )  :%.0f/%.0f   扑克牌:...", recordInfo.m_strTableName.c_str(), recordInfo.m_uMaxPlayer, recordInfo.m_dSmallBlind, recordInfo.m_dBigBlind);
    SetLabelStringWithBoundWidth(item->m_pLableRoomName, record, 420);

    //刷新盈利
    std::string profit = FormatCash4(recordInfo.m_dWinLost);
    SetLabelString(item->m_pLableProfit, profit);

    //刷新游戏时间
    std::string strTime = CalculateTime(recordInfo.m_dTime);
    SetLabelString(item->m_pLableGameTime, strTime);

    
    /*int cardLeft = recordInfo.m_cbHandCard[0];
    int cardRight = recordInfo.m_cbHandCard[1];
    if (cardLeft < 0)cardLeft = 0;
    if (cardRight < 0)cardRight = 0;*/
    item->m_pSpriteHandCardL->SetFrameIdx(GetCardFrameIdx(recordInfo.m_cbHandCard[0]));
    item->m_pSpriteHandCardR->SetFrameIdx(GetCardFrameIdx(recordInfo.m_cbHandCard[1]));
}

std::string GameRecord::CalculateTime(double gameTime)
{
    char strTime[64];
    time_t time_Now = time(NULL);
    int difftime = time_Now - (time_t)gameTime;
    if (difftime <= 0)
    {
        sprintf(strTime,"%d秒内", 5);
        return strTime;
    }
    unsigned minute = difftime/60;
    unsigned hour = difftime/60/60;
    unsigned day = difftime/60/60/24;
    unsigned week = difftime/60/60/24/7;
    unsigned month = difftime/60/60/24/7/4;
    unsigned year = difftime/60/60/24/7/4/12;

    if(year!=0){
        sprintf(strTime,"%d年前",year);
    }else if(month!=0){
        sprintf(strTime,"%d月前",month);
    }else if(week!=0){
        sprintf(strTime,"%d星期前",week);
    }else if(day!=0){
        sprintf(strTime,"%d天前",day);
    }else if(hour!=0){
        sprintf(strTime,"%d小时前",hour);
    }else if(minute!=0){
        sprintf(strTime,"%d分前",minute);
    }else{
        sprintf(strTime,"%d秒前",difftime);
    }

    return strTime;
}

void GameRecord::OnPageTurnUp()
{
    if (m_uCurPage > 1)
    {
        m_uCurPage -= 1;
    }
    unsigned count = 0;
    for (auto offSet : g_globalGameRecord.m_vecPageIdx[m_uSortType])
    {
        count++;
        if (count == m_uCurPage)
        {
            UpdataRecordView(m_uSortType, offSet);
        }
    }
}
void GameRecord::OnPageTurnDown()
{
    m_uCurPage +=1;
    unsigned count = 0;
    if (g_globalGameRecord.m_vecPageIdx[m_uSortType].size() < m_uCurPage)//下一页没有数据
    {
        Json::Value param;
        param["page"] = m_uCurPage-1;
        param["sortType"] = m_uSortType + 1;
        param["countPerPage"] = GameRecordPerPage;
        SendClientRequest(ClientRequest::kGetGamePlayBackList, param);
    }
    else//下一页有数据，不需要发请求，直接访问全局数据
    {
        for (auto offSet : g_globalGameRecord.m_vecPageIdx[m_uSortType])
        {
            count++;
            if (count == m_uCurPage)
            {
                UpdataRecordView(m_uSortType, offSet);
            }
        }
    }
}
