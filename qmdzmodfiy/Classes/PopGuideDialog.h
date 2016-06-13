#ifndef _POPGUIDEDIALOG_H__
#define _POPGUIDEDIALOG_H__

#include "GameDialog.h"
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;

class NewerTrain : public GameLayer
{
public:
    static NewerTrain* create(unsigned index);
    //CREATE_FUNC(NewerTrain);
    bool init(unsigned index);
    void SetFunCallBack(std::function<void()> func1, std::function<void()> func2, std::function<void()> func3, std::function<void()> func4);
	void setVisible(bool bVisible) ;
protected:
    void MenuBtnSelect(Ref* pSend);
    void MenuLabelSelect(Ref* pSend);
    std::function<void()> funNewerTrainBtn;
    std::function<void()> funNewerTrainLabel;
    std::function<void()> funStartGameBtn;
    std::function<void()> funStartGameLabel;
private:
    MenuItemScale9SpriteWithLabel  *m_BtnTrain;
    unsigned                  m_index;
};

class TrainTips : public GameLayer
{
public:
    CREATE_FUNC(TrainTips);
    virtual bool init();
    void UpdateTipsData(unsigned index);
    void UpdateBottomTips(std::string srtContent);
    void SetFunCallBack(std::function<void()> func1, std::function<void()> func2, std::function<void()> func3);
protected:
    void OnMenuSelectCardType(Ref *pSend);
    void OnMenuSelectDrawWard(Ref *pSend);
    std::function<void()> funDuiZi;
    std::function<void()> funLiangDui;
    std::function<void()> funHuLu;
private:
    MenuItemScale9SpriteWithLabel  *m_btnCardType[3];      //牌型操作
    MenuItemScale9SpriteWithLabel  *m_btnDrawWard;         //领取奖励
    cocos2d::Label           *m_labelBtn[3];
    cocos2d::Label           *m_labelTopText;
    cocos2d::Label           *m_labelBotomText;
    cocos2d::Label           *m_labelTips;
    unsigned                  m_index;
    //cocos2d::Sprite          *m_CardTypeEffect;
};


#endif