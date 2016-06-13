#ifndef __FULLSCREENADS_H__
#define __FULLSCREENADS_H__

#include "cocos2d.h"
#include "GameDialog.h"

class HttpSprite;
class MenuItemScale9SpriteWithLabel;

class FullScreenAds : public GameLayer
{
public:
    CREATE_FUNC(FullScreenAds);
    void updateFullAdsInfo();
    virtual void setVisible(bool bVisible);
protected:
    virtual bool init();
    void menuCallBackClose(cocos2d::Ref* pSend);//关闭按钮回调
    void menuCallBackDetail(cocos2d::Ref* pSend);//查看详情按钮回调
private:
    HttpSprite                       *m_httpAdsBg;      //全屏广告背景
    cocos2d::Label                   *m_labelBtnName;   //按钮名称
    MenuItemScale9SpriteWithLabel    *m_pOptBtn;        //操作按钮
};
#endif
