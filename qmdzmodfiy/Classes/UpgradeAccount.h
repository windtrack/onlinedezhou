#ifndef __UPGRADEACCOUNT_H__
#define __UPGRADEACCOUNT_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "ui/CocosGUI.h"
#include "BaseButton.h"

class LineLabel;
class MenuItemScale9SpriteWithLabel;
class UserProtocol;
class HttpSprite;

class UpgradeTips : public GameDialog
{
public:
    CREATE_FUNC(UpgradeTips);
    void updateTips(unsigned uType);
protected:
    virtual bool init();
    void OnMenuItemSelect(cocos2d::Ref*);
private:
    cocos2d::Label      *m_labelTips;    //普通提示
    LineLabel           *m_lineLabelTips;//拼接的提示
    cocos2d::Label      *m_labelAddTips; //与拼接的消息组合使用（否则换行麻烦）
    cocos2d::Label      *m_labelBtnName; //按钮文字信息

    cocos2d::Layer      *m_layerReward;     //升级账号奖励界面
    cocos2d::Sprite     *m_rewardTitle;     //奖励标题
    cocos2d::Sprite     *m_spriteRewardBg[3]; //升级奖励背景
    HttpSprite          *m_httpSpriteReward[3];//升级奖励
    cocos2d::Label      *m_labelReward[3];

    MenuItemScale9SpriteWithLabel  *m_menuItem;//操作按钮
    unsigned             m_uTipsType;
    unsigned             m_uTestIndex;
};

class UpgradeAccount : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(UpgradeAccount);
protected:
    virtual bool init();
    void popUseProtocol();
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    void menuItemCallback(cocos2d::Ref* pSend, cocos2d::ui::Button::TouchEventType type);
    void getSecurityCodeReg();
private:
    AutoDelayEnabledFrameSpriteButton  *m_baseBtn; //发送短信按钮
    UserProtocol                *m_pLicenceViewDlg;//用户协议
    cocos2d::ui::EditBox        *m_edit0058Account;//已有0058账号
    cocos2d::ui::EditBox        *m_edit0058Password;
    cocos2d::ui::EditBox        *m_editRegPhoneNum;//注册账号
    cocos2d::ui::EditBox        *m_editSecurityCode;
    cocos2d::ui::EditBox        *m_editRegPassword;
    //ui界面控件
    cocos2d::ui::Layout         *m_layoutBindAccount;//账号绑定层
    cocos2d::ui::Layout         *m_layout0058Account;//0058账号登录层
    cocos2d::ui::Layout         *m_layoutRegAccount; //注册账号层
    cocos2d::ui::Button         *m_btnAccountBind[4];//微信绑定 qq绑定 0058账号绑定 微博绑定
    cocos2d::ui::Button         *m_btnQuickReg;      //立即注册
    cocos2d::ui::Button         *m_btnQuickBind;     //立即绑定
    cocos2d::ui::Button         *m_btnRegAccount[3]; //查看用户协议 接受协议并注册 注册界面的返回按钮
    cocos2d::ui::ImageView      *m_imgEditboxBg[5];  //编辑框背景
    cocos2d::ui::ImageView      *m_imgInputFlag[4];  //输入框判错标记
};
#endif