#ifndef _PERSONALINFORMATION_H__
#define _PERSONALINFORMATION_H__

#include "GameDialog.h"
#include "CCScrollView.h"
#include "vector"
#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class HttpSprite;
class FrameSprite;
class LayerButton;

struct AvatatItem
{
    HttpSprite          *m_httpSprite;
    LayerButton         *m_layerBt;
};

class PersonalInfomation : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(PersonalInfomation);

    void MyAvatar(cocos2d::Ref* pSender);
    void PersonalInfo(cocos2d::Ref* pSender);

    void UpdatePersonalInfo();
    void SelectAvatar(unsigned  uAvatarID, unsigned uIdx);

    void TakePhoto(cocos2d::Ref* pSender);
    void SelectFromAlbum(cocos2d::Ref* pSender);

    void ExpandGender();
    void SelectMan(cocos2d::Ref* pSender);
    void SelectWoman(cocos2d::Ref* pSender);
    
    void OnMyDataChanged(unsigned uDataType);
protected:
    virtual bool init();
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    struct HeadItem
    {
        HttpSprite                  *m_httpSpriteHead;   //人物头像
        cocos2d::ClippingNode       *m_clipNode;         //遮罩
        cocos2d::Sprite             *m_spriteFrame;     //头像边框
        cocos2d::Sprite             *m_spriteMask;      //遮罩底
        LayerButton                 *m_btnLayer;       //点击区域
    };
    HeadItem* GetHeadItem(unsigned fX, unsigned uSkinId);
    void updateHeadItem(PersonalInfomation::HeadItem* headShop, unsigned uSkinId);

private:
    MenuItemSpriteWithLabel                     *m_btMyAvatar;                  //我的头像
    MenuItemSpriteWithLabel                     *m_btPersonalInfo;              //个人资料
    MenuItemScale9SpriteWithLabel               *m_btTakePhoto;                 //拍照
    MenuItemScale9SpriteWithLabel               *m_btSelectFromAlbum;           //从相册选择
    MenuItemSpriteWithLabel                     *m_btSelectMan;                 //选择女
    MenuItemSpriteWithLabel                     *m_btSelectWoman;               //选择男
    cocos2d::Label                              *m_lableWarn;                   //警告
    cocos2d::extension::ScrollView              *m_ScrollMyAvatar;              //头像滚动视图
    FrameSprite                                 *m_SpriteNickName;              //昵称
    FrameSprite                                 *m_SpriteGender;                //性别
    FrameSprite                                 *m_SpriteQQNum;                 //QQ号
    FrameSprite                                 *m_SpriteEmail;                 //邮箱
    FrameSprite                                 *m_SpriteUsing;                 //使用头像
    GameDialog                                  *m_ExpandGender;                //性别弹框
    cocos2d::Sprite                             *m_SpriteMan;                   //男
    cocos2d::Sprite                             *m_SpriteWoman;                 //女
    cocos2d::ui::EditBox                 *m_editNickName;                //昵称
    cocos2d::ui::EditBox                 *m_editQQNum;                   //QQ号
    cocos2d::ui::EditBox                 *m_editEmail;                   //邮箱
    std::vector<AvatatItem>                      m_vecAvatarItem;
    std::vector<HeadItem*>                       m_vecHeadItemNew;
    std::vector<HeadItem*>                       m_vecHeadItemOld;
};

#endif