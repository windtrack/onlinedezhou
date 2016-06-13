#ifndef _GAMEHELP_H__
#define _GAMEHELP_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"
#include "ui/CocosGUI.h"

class MenuItemSpriteWithLabel;

class GameHelp : public GameDialog
{
public:
    CREATE_FUNC(GameHelp);
protected:
    virtual bool init();
    void LoadGongNengJian();
    void LoadWenTi();
    void MenuItemCallback(cocos2d::Ref *pSend, unsigned idx);
    void CaoZuoMenuCallback(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type);
private:
    cocos2d::Label                     *m_LabelWenti[8];
    MenuItemSpriteWithLabel            *m_pHelpBtn[5];
    cocos2d::ui::Layout                *m_pLayoutHelp[5];
    cocos2d::ui::Button                *m_pBtnOpt[2];
    cocos2d::ui::ImageView             *m_pImageCaoZuo[2];
    cocos2d::extension::ScrollView     *m_pScrollGongNeng;
    cocos2d::extension::ScrollView     *m_pScrollWenTi;
};

#endif