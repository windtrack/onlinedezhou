#ifndef _MATCHRECORD_H__
#define _MATCHRECORD_H__

#include "GameDialog.h"
#include "CCScrollView.h"
#include <vector>
#include <string.h>
#include "Card.h"

class FrameSpriteButton;
class FrameScale9SpriteButton;
class MenuItemSpriteWithLabel;
class FrameSprite;
struct GameRecordInfo;

class GameRecord : public GameDialog
{
public:
    CREATE_FUNC(GameRecord);

    void UpdataRecordData();               //更新服务端数据请求
    void menuTimeSort(cocos2d::Ref* pSender);                               //时间排序
    void menuProfitSort(cocos2d::Ref* pSender);                             //盈利排序
    
    void UpdataRecordView(unsigned sortMethod, unsigned pageOffset);        //更新记录视图
    std::string CalculateTime(double gameTime);                             //时间计算

    void OnPageTurnUp();                                                    //上翻
    void OnPageTurnDown();                                                  //下翻
protected:
    virtual bool init();

    struct RecordItem
    {
        unsigned             m_uRecordID;
        cocos2d::Sprite      *m_pSpriteBgItem;
        cocos2d::Label       *m_pLableRecordNum;
        cocos2d::Label       *m_pLableRoomName;
        cocos2d::Label       *m_pLableProfit;
        cocos2d::Label       *m_pLableGameTime;
        FrameScale9SpriteButton    *m_pSpriteBtn;
        FrameSprite          *m_pSpriteHandCardL;
        FrameSprite          *m_pSpriteHandCardR;
    };
    RecordItem*  GetRecordItem(std::string strRecordID);
    void UpdateRecordItem(GameRecord::RecordItem *item, const GameRecordInfo& recordInfo);

private:
    cocos2d::extension::ScrollView           *m_ScrollView;
    MenuItemSpriteWithLabel                  *m_btTimeSort;
    MenuItemSpriteWithLabel                  *m_btProfitSort;
    FrameSpriteButton                        *m_btPrePage;
    FrameSpriteButton                        *m_btNextPage;
    std::vector<RecordItem*>                  m_vecRecordNew;
    std::vector<RecordItem*>                  m_vecRecordOld;
    std::vector<GameRecordInfo>               m_vecAllRecordData;
    cocos2d::Label                           *m_labelIsVIP;
    unsigned                                  m_uCurPage;
    unsigned                                  m_uSortType;
};
#endif