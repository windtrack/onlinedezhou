#ifndef _FRIENDSSPREAD_H__ 
#define _FRIENDSSPREAD_H__

#include "GameDialog.h"
#include "cocos2d.h"
#include "CCScrollView.h"
#include "ui/UIEditBox/UIEditBox.h"

class MenuItemSpriteWithLabel;
class LayerButton;
class HttpSprite;
class LineLabel;
struct FollowUserInfo;

class FriendsSpread : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(FriendsSpread);
    void updateSpreadInfo();//更新推广员
    void updateListView();  //更新我的推广
    virtual void setVisible(bool Visible) override;
    void popBindResult(bool bSuccess);
    bool getVisible();
protected:
    virtual bool init();

    void OnSpreadPerson(Ref *pSend);
    void OnMySpread(Ref *pSend);
    void OnBindSpread(Ref *pSend);
    void OnBindResult(Ref *pSend);
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;

    //我的推广列表
    struct MyListItem
    {
        unsigned                    m_uUserID;
        cocos2d::ui::Scale9Sprite   *m_pSpriteBgItem;
        cocos2d::Label              *m_pLableUserName;
        cocos2d::Label              *m_pLableUserID;
        HttpSprite                  *m_pHeadPic;
        LayerButton                 *m_pBtDetail;
    };

    MyListItem *GetListItem(unsigned uUID);
    void UpdateListItem(FriendsSpread::MyListItem* item, const FollowUserInfo& userInfo);

private:
    std::vector<MyListItem*>        m_vecMyListNew;
    std::vector<MyListItem*>        m_vecMyListOld;
    cocos2d::extension::ScrollView  *m_ScrollList;
    MenuItemSpriteWithLabel         *m_menuSpreadPerson;
    MenuItemSpriteWithLabel         *m_menuMySpread;
    GameLayer *m_pLayerTop;
    cocos2d::Layer                  *m_layerNotBind;    //绑定前的层
    cocos2d::Layer                  *m_layerAlreayBind; //绑定后的层
    cocos2d::Layer                  *m_layerSpreadList; //我的推广列表
    cocos2d::ui::EditBox            *m_editSpreadID;    //推广员ID编辑框
    cocos2d::Label                  *m_labelSpreadID;   //推广员ID
    cocos2d::Label                  *m_labelSpreadName; //推广员昵称 
    cocos2d::Label                  *m_labelNoSpread;   //我的推广为0的描述
    cocos2d::Label                  *m_labelEditRetFaild;//编辑框格式不合法提示
    cocos2d::Label                  *m_labelTips;        //提示信息
    cocos2d::Label                  *m_labelBtnName;     //按钮名称
    cocos2d::ui::Scale9Sprite       *m_pScale9ContentBg;//滚动视图背景
    HttpSprite                      *m_pHttpSpreadHead; //推广员头像
    HttpSprite                      *m_pHttpMyHead;     //我的头像
    LineLabel                       *m_pLineLabelMyID;  //我的推广ID
    LineLabel                       *m_pLineLabelNum;   //我的推广人数
    bool                             m_bIsVisible;      //是否显示

    GameDialog                      *m_pFaildDialog;
};

#endif