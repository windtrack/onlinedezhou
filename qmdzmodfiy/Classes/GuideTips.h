#ifndef __GUIDETIPS_H__
#define __GUIDETIPS_H__

#include "cocos2d.h"

//    <data id="1" status="0" des="德女郎引导"/>
//    <data id="2" status="0" des="常规场引导"/>
//    <data id="3" status="0" des="贵族开房引导"/>
//    <data id="4" status="0" des="添加关注引导"/>
//    <data id="5" status="0" des="好友功能引导"/>
//    <data id="6" status="0" des="视频6人场切换"/>
//    <data id="7" status="0" des="SNG场引导"/>
struct GuideItemInfo
{
    int id;
    bool status;
};

enum
{
    DelaerGuide=0,//大厅德女郎列表
    NormalSceneGuide,//普通场导航栏
    VipDefineRoomGuide,//VIP自定义开放
    AddAttentionGuide,//游戏中加关注
    FriendListGuide,//游戏中好友列表
    SwitchvideoOr6Guide,//切换视频场和六人场
    SNGGuide,//SNG帮助
    MTTHelp,//MTT帮助
    MTTSign,//MTT操作导航栏
    MTTFilter,//MTT赛事筛选
    MTTDescription,//MTT具体场次信息
    MTTPageUpAndDown//MTT上下翻页
};

#define GuideInfo ReadGuideInfo::getInstance()

//1.strMask:遮罩模板
//2.pos    :模板坐标
//3.bottom..:底板
cocos2d::ClippingNode* CreateClippingMask(std::string strMask, cocos2d::Vec2 pos, cocos2d::Node* bottomBoard, std::string strMask1="", cocos2d::Vec2 pos1=cocos2d::Vec2(0,0));

class GuideTips : public cocos2d::Layer
{
public:
    //static GuideTips* getInstance();
    static GuideTips* create(std::string strBg, std::string strFlag);
    bool init(std::string strBg, std::string strText);
    void setBgPosition(cocos2d::Vec2 vec);
    void setFlagPosition(cocos2d::Vec2 vec);
    void ResetBgSprite(std::string strBg);
    void addFlagSpriteAndPos(std::string strFlag, cocos2d::Vec2 pos);
    void setVisible(bool bVisible);
    void setTipsID(unsigned uID);
protected:

private:
    //static GuideTips*       m_GuideTips;
    cocos2d::EventListenerTouchOneByOne *m_listenerTouch;
    cocos2d::LayerColor*    m_layerColorBg;
    cocos2d::Sprite*        m_bgSprite;
    cocos2d::Sprite*        m_flagSprite;
    unsigned                m_uTipsID;
};

class ReadGuideInfo
{
public:
    static ReadGuideInfo* getInstance();
    ReadGuideInfo();
    //从服务端获取每个引导界面的状态
    void getAllGuideInfo(unsigned status);
    //获取引导信息
    bool readGuideInfo(unsigned id);
    //引导后修改状态
    void ModifyGuideInfo(unsigned id, unsigned status = 1);
    //清理vector
    void clearOldGuideInfo();
    std::vector<GuideItemInfo> m_vecGuideInfo;
private:
    static ReadGuideInfo* m_ReadGuideInfo;
    std::vector<unsigned> m_vecGuideStatus;
};

#endif