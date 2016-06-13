#ifndef __MTTMATCHSTATE_H__
#define __MTTMATCHSTATE_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
#include "GlobalUserData.h"
struct GameMttTableInfo;
struct MttTableDes;
struct MttBlind;
struct MttReward;
struct MttRankInTable;
struct SingleFloatReward;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class FrameSprite;
class HttpSprite;
class LineLabel;
class FrameSpriteButton;
class DataLoading;

class MTTMatchState : public GameDialog,public cocos2d::extension::ScrollViewDelegate
{
public:
    CREATE_FUNC(MTTMatchState);
    void UpdateMTTMatchStateInfo(unsigned index);//更新所有,并指定当前停留在哪个界面
    void UpdateGaiShuInfo(const GameMttTableInfo& tableInfo);   //更新概述
    void UpdatePaiMingInfo();  //跟新排名
    void UpdateMangZhuInfo();  //更新盲注
    void UpdateJiangLiInfo(std::vector<MttReward> vecReward, std::string strTableName);  //更新奖励
	void UpdateFuDongInfo();   //更新浮动赛数据
    //void UpdateFuDongJiangli(std::vector<MttReward> vecReward, std::string strTableName);//更新浮动赛每一项的奖励列表
    void getGameTableType(int type){m_iGameTableType = type;}
    bool curTableDataIsNULL(){return m_bCurDataIsNULL;}
    unsigned getCurSelectIndex(){return m_uCurSelectIndex;}
    bool getVisibleState(){return m_bIsVisible;}
    virtual void setVisible(bool bVisible) override;
protected:
    virtual bool init();
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    void OnMenuItemSelected(cocos2d::Ref*, unsigned uType);
    void OnMenuSignUp(cocos2d::Ref*);//报名、退赛按钮回调
    void onGrabTouchBegan(bool bHitDialog);
    void showDataLoading(bool bVisible);
    //概述结构
    struct GaishuItem
    {
        //精灵及label顺序都是从上到下
        cocos2d::Sprite    *m_GaishuSptite[8];
        LineLabel          *m_GaiShuLine[8];
        cocos2d::Label     *m_labelTD[2];//标题和延迟
    };
    void GetGaiShuItem();//获取概述结构

    //排名结构
    struct PaiMingItem 
    {
        FrameSprite         *m_spriteRankFlag;
        HttpSprite          *m_httpSpriteIcon;
        cocos2d::Sprite     *m_spriteCoin;
        cocos2d::Label      *m_labelRankId;
        cocos2d::Label      *m_labelPlayerName;
        cocos2d::Label      *m_labelCash;
        cocos2d::ui::Scale9Sprite     *m_spriteItemBg;
    };
    PaiMingItem* GetPaiMingItem(); //获取一个排名项
    void UpdatePaiMingItem(MTTMatchState::PaiMingItem* item, const MttRankInTable& mttRank);//更新一个排名项

    //盲注结构
    struct MangZhuItem
    {
        cocos2d::Label      *m_labelLevel;
        cocos2d::Label      *m_labelBlind;
        cocos2d::Label      *m_labelAnti;
        cocos2d::Label      *m_labelTime;
        cocos2d::Sprite     *m_spriteAddOn;
        cocos2d::Sprite     *m_spriteRebuy;
        cocos2d::Sprite     *m_spriteLine;
        cocos2d::Sprite     *m_spriteCurBlind;
    };
    MangZhuItem* GetMangZhuItem();//获取一个盲注项
    void UpdateMangZhuItem(MTTMatchState::MangZhuItem* item, const MttBlind& mttBlind);//更新一个盲注项

    //奖励结构
    struct JiangLiItem
    {
        cocos2d::Label      *m_labelRankId[2];//前三名的字体不同
        cocos2d::Label      *m_labelReward;
        cocos2d::Label      *m_labelScore;
        cocos2d::Sprite     *m_spriteRewardCup[5];
        cocos2d::Sprite     *m_spriteLine;
        cocos2d::Sprite     *m_spriteGold;
    };
    JiangLiItem* GetJiangLiItem();//获取一个奖励项
    JiangLiItem* GetFuDongJiangLiItem();//获取一个浮动奖励项
    void UpdateJiangLiItem(MTTMatchState::JiangLiItem* item, const MttReward& mttReward);//更新一个奖励项

    //浮动赛结构
    struct FuDongItem
    {
        FrameSpriteButton   *m_spriteButton;
        cocos2d::Sprite     *m_spriteCurSel;
        cocos2d::Sprite     *m_spriteBtnBg;
        cocos2d::Label      *m_labelPlayerNum;
		unsigned			m_floatIndex ;
    };
    FuDongItem* GetFuDongItem(std::vector<MttReward> vecReward, std::string tableName);//获取浮动赛的一个奖励阶段
    void UpdateFuDongItem(MTTMatchState::FuDongItem* item, const SingleFloatReward& mttReward);
private:
    cocos2d::ui::Scale9Sprite            *m_pScale9ContentBg;//滚动视图背景
    MenuItemSpriteWithLabel              *m_pMenuItemType[4];//操作栏按钮
    cocos2d::extension::ScrollView       *m_pScrollViewType[5];
    GaishuItem                           *m_pGaishuItem;//概述就一项，定义成员变量保存
    std::vector<PaiMingItem*>             m_vecPaiMingNew;
    std::vector<PaiMingItem*>             m_vecPaiMingOld;
    cocos2d::Label                       *m_labelPaiMingDes;//排名没有数据时的提示
    cocos2d::Label                       *m_labelFuDong[2];//浮动赛 0:当前奖池总额文本、1:具体奖池金额
	cocos2d::Sprite                      *m_pSpriteFuDongGold;//浮动 奖池金币图标
    cocos2d::ui::Scale9Sprite            *m_pSpriteListTitle;//列表表头
    cocos2d::Sprite                      *m_pSpriteBlindBg;//当前盲注选中背景
    std::vector<MangZhuItem*>             m_vecMangZhuNew;
    std::vector<MangZhuItem*>             m_vecMangZhuOld;
    cocos2d::Label                       *m_labelMTTTableName;//MTT牌桌名
    cocos2d::Label                       *m_labelCurPaiming;//当前排名
    cocos2d::Label                       *m_labelMangZhuTitle[4];//盲注列表表头名称
    cocos2d::Label                       *m_labelJiangLiTitle[3];//奖励列表表头名称
    std::vector<JiangLiItem*>             m_vecJiangLiNew;
    std::vector<JiangLiItem*>             m_vecJiangLiOld;
    MenuItemScale9SpriteWithLabel        *m_menuSignUp;//报名按钮
    MenuItemScale9SpriteWithLabel        *m_menuCalcelSign;//退赛
    MenuItemScale9SpriteWithLabel        *m_menuEnterGame;//进入
    DataLoading                          *s_dataLoading ;
    std::vector<FuDongItem*>              m_vecFuDongNew;
    std::vector<FuDongItem*>              m_vecFuDongOld;
    std::vector<JiangLiItem*>             m_vecFuDongJiangLiNew;
    std::vector<JiangLiItem*>             m_vecFuDongJiangLiOld;
    unsigned                              m_uCurSelectIndex;
    unsigned                              m_uSizePaiMing;//排名列表大小
    unsigned                              m_uCurMyRankId;//当前排名
    unsigned                              m_uCurPageIndex;//排名列表 当前页
    unsigned                              m_uTotalOldItem;//排名列表翻页前的总数
    unsigned                              m_uTableIdOld;//保存上一个桌子ID
    int                                   m_iGameTableType;
    float                                 m_dBlindOffset;//盲注视图当前偏移
    int                                   m_iLastTime;//倒计时时间
    bool                                  m_bSignUp;//是否已经报名，判断显示报名还是退赛按钮
    bool                                  m_bRequestData;//是否请求数据
    bool                                  m_bCurDataIsNULL;//当前数据为空的时候显示加载界面
    bool                                  m_bIsVisible;//当前界面的显示状态
};
#endif