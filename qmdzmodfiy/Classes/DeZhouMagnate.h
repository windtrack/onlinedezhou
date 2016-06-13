#ifndef _DEZHOUMAGNATE_H__
#define _DEZHOUMAGNATE_H__

#include "cocos2d.h"
//#include "CCScrollView.h"
#include "extensions/GUI/CCScrollView/CCScrollView.h"
//#include "ui/UIPageView.h"
#include "GameDialog.h"
//#include "ui/UIEditBox/UIEditBox.h"
#include "UICommon.h"
class HttpSprite;
class FrameSpriteButton;
class MenuItemScale9SpriteWithLabel;
class MenuItemSpriteWithLabel;
struct DeZhouMagnateInfo;

class DeZhouMagnate : public cocos2d::Layer, public cocos2d::extension::ScrollViewDelegate
{
public:
    static DeZhouMagnate* create(cocos2d::Node *node);
    void ReceiveDataAndUpdate();
    void SetLayerVisable(bool visable);
    void UpdatePageView();
    unsigned        m_uCurPageNum;
protected:
    bool init(cocos2d::Node *node);
    void UpdataMagnateView(unsigned pageNum);
    void pageViewEvent(cocos2d::Ref* pSender, cocos2d::ui::PageView::EventType type);
    void OnPageLeft(cocos2d::Ref* pSender);
    void OnPageRight(cocos2d::Ref* pSender);

    void OnQuickJoin(cocos2d::Ref* pSender);
    void OnSearch(cocos2d::Ref* pSender);
    void OnUpdate(cocos2d::Ref* pSender);
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
    struct MagnateItem
    {
        FrameSpriteButton                   *m_spriteBgItem;      //背景按钮
        cocos2d::Sprite                     *m_spritePlayerBg[3]; //玩家底框
        cocos2d::Sprite                     *m_spriteHuman;       //人物图标
        cocos2d::Sprite                     *m_spriteCamera;      //摄像头图标
        cocos2d::Sprite                     *m_spriteEye;         //眼睛图标
        cocos2d::Sprite                     *m_spriteClassify[3]; //当前桌类型（必下、贵族、单挑）
        cocos2d::Sprite                     *m_spriteNormalDesk;  //普通桌背景
        cocos2d::Sprite                     *m_spriteVipDesk;     //会员桌子背景
        cocos2d::Label                      *m_labelMinTake;      //多少万带入
        cocos2d::Label                      *m_labelCurPlayer[2]; //当前桌玩家数量
        cocos2d::Label                      *m_labelUserName[3];  //玩家名称
        cocos2d::Label                      *m_labelOnlookNum;    //旁观人数
        HttpSprite                          *m_httpUserHead[2];   //头像
    };
    MagnateItem* GetMagnateItem(DeZhouMagnateInfo data);
    void UpdataMagnateItem(DeZhouMagnate::MagnateItem *item, const DeZhouMagnateInfo &data);

private:
    cocos2d::ui::PageView                   *m_pageView;
    cocos2d::extension::ScrollView          *m_ScrollFlip;
	cocos2d::ui::ScrollView *m_pView;
    std::vector<MagnateItem*>                m_vecMagnateNew;
    std::vector<MagnateItem*>                m_vecMagnateOld;
    MenuItemSpriteWithLabel                 *m_btPageLeft;
    MenuItemSpriteWithLabel                 *m_btPageRight;
    std::vector<std::string>                 m_vecStrRegin;
    cocos2d::Sprite                         *m_spriteSegregate;
    cocos2d::Label                          *m_labelCurPagePlayers;
    cocos2d::Label                          *m_labelCurPageDesks;
    GameLayer                               *m_layerTop;
    GameLayer                               *m_layerBottom;
    unsigned                                 m_uMaxPageNum;
    float                                    m_dOffSet;
};

class EnterVipRoom : public GameLayer,public cocos2d::ui::EditBoxDelegate
{
public:
    //CREATE_FUNC(EnterVipRoom);
    static EnterVipRoom* create(unsigned uDeskID);
    bool init(unsigned uDeskID);
	void onMenuItemCloseSelected(cocos2d::Ref* pSend);
protected:
    void EnterRoom(cocos2d::Ref* pSend);
    void CancelEnter(cocos2d::Ref* pSend);
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
private:
    cocos2d::ui::EditBox              *m_editPassWord;
    MenuItemScale9SpriteWithLabel           *m_btEnterRoom;
    MenuItemScale9SpriteWithLabel           *m_btCancelRoom;
	MenuItemSpriteWithLabel			  *m_btMenuItemClose;
    unsigned                                  m_uDeskID;
    std::string                               m_strPassword;
};

class EnterVipRoomEx : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(EnterVipRoomEx);
    bool init();
	void SetHitCB(std::function<void(const std::string&)> cbBt);

protected:
    void EnterRoom(cocos2d::Ref* pSend);
    void CancelEnter(cocos2d::Ref* pSend);
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
private:
    cocos2d::ui::EditBox *m_editPassWord;
    MenuItemScale9SpriteWithLabel     *m_btEnterRoom;
    MenuItemScale9SpriteWithLabel     *m_btCancelRoom;
    std::string                  m_strPassword;
	std::function<void(const std::string&)> m_cbBt;
};

#endif