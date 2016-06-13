#ifndef __GAMEFRIENDLIST_H__
#define __GAMEFRIENDLIST_H__

#include "GameDialog.h"
#include "CCScrollView.h"
#include <vector>

class MenuItemSpriteWithLabel;
class HttpSprite;
struct FollowUserInfo;
class FrameSpriteButton;
class FrameScale9SpriteButton;
class LayerButton;


class GameFriendList : public GameDialog
{
public:
    CREATE_FUNC(GameFriendList);
    void UpdateFollowList(unsigned uType);
    virtual void setVisible(bool Visible) override;
protected:
    virtual bool init();
	virtual void onSetVisible(bool bVisible);

	void UpdateListView(unsigned uType);

	void Clear();
	void SwitchTo(unsigned uType);

    void OnMyLady(Ref* pSender);
    void OnMyAttention(Ref* pSender);
    void OnAttentMine(Ref* pSender);

    void OnPageTurnUp(unsigned);
    void OnPageTurnDown(unsigned);

    //好友列表
    struct MyListItem
    {
        unsigned             m_uUserID;
        cocos2d::Sprite      *m_pSpriteBgItem;
        cocos2d::Label       *m_pLableUserName;
        cocos2d::Label       *m_pLableProfit;
        cocos2d::Label       *m_pLabelBtnName;
        HttpSprite           *m_pHeadPic;
        FrameScale9SpriteButton    *m_pBtCancelAttention;
        LayerButton          *m_pBtDetail;
    };
    MyListItem *GetListItem(unsigned uUID, unsigned uType);
    void UpdateListItem(GameFriendList::MyListItem* item, const FollowUserInfo& followUserInfo, unsigned uType);

private:
    MenuItemSpriteWithLabel         *m_btMyLady;
    MenuItemSpriteWithLabel         *m_btMyAttention;
    MenuItemSpriteWithLabel         *m_btAttentMine;
    cocos2d::extension::ScrollView  *m_ScrollList;
    std::vector<MyListItem*>        m_vecMyListNew;
    std::vector<MyListItem*>        m_vecMyListOld;
    FrameSpriteButton               *m_btNextPage;
    FrameSpriteButton               *m_btPrePage;
	GameLayer *m_pLayerTop;
	unsigned m_uType;
};

#endif