#include "GuideTips.h"
#include "GameTipsManager.h"
#include "tinyxml2/tinyxml2.h"
#include "GlobalUserData.h"

USING_NS_CC;


cocos2d::ClippingNode* CreateClippingMask(std::string strMask, Vec2 pos, Node* bottomBoard, std::string strMask1, Vec2 pos1)
{
    auto stencil = Node::create();
    auto stencilSprite = Sprite::create(strMask);
    stencilSprite->setPosition(pos);
    if (strMask1 != "")//创建第二个
    {
        auto stencilSprite1 = Sprite::create(strMask1);
        stencilSprite1->setPosition(pos1);
        stencil->addChild(stencilSprite1);
    }
    stencil->addChild(stencilSprite);
    auto pClip = ClippingNode::create(stencil);
    pClip->setPosition(Vec2(0, 0));
    pClip->setInverted(true);
    pClip->setAlphaThreshold(0.5);
    pClip->addChild(bottomBoard);
    return pClip;
}

GuideTips* GuideTips::create(std::string strBg, std::string strText)
{
    auto pRet = new GuideTips;
    if (pRet->init(strBg, strText))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }

    return pRet;
}

bool GuideTips::init(std::string strBg, std::string strText)
{
    
    if (!Layer::init())
    {
        return false;
    }
    m_uTipsID = 0;
    auto frameSz = Director::getInstance()->getVisibleSize();
    m_layerColorBg = LayerColor::create(Color4B(0,0,0,120), frameSz.width, frameSz.height);
    m_layerColorBg->setPosition(0, 0);
    this->addChild(m_layerColorBg);

    m_bgSprite = Sprite::create(strBg);
    m_bgSprite->setPosition(frameSz.width/2, frameSz.height/2);
    this->addChild(m_bgSprite);

    m_flagSprite = Sprite::create(strText);
    m_flagSprite->setPosition(m_bgSprite->getContentSize().width/2, m_bgSprite->getContentSize().height/2);
    this->addChild(m_flagSprite);

    m_listenerTouch = EventListenerTouchOneByOne::create();
    m_listenerTouch->setSwallowTouches(true);

    m_listenerTouch->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event){
    auto  target = static_cast<Node*>(event->getCurrentTarget());
    auto pos = target->convertToNodeSpace(touch->getLocation());
    Size sz = target->getContentSize();
    Rect rt(0, 0, sz.width, sz.height);
    if (rt.containsPoint(pos))
    {
        this->setVisible(false);
        m_listenerTouch->setEnabled(false);
        Json::Value param;
        param["guideid"] = m_uTipsID;
        SendClientRequest(ClientRequest::kModifyGuideTipsInfo, param);
        if (GameTipsManager::getInstance()->m_verTips.size() > 0)
            GameTipsManager::getInstance()->hideTips();
        return true;
    }

    m_listenerTouch->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event){
        this->removeFromParentAndCleanup(true);
    };
    
    return false;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listenerTouch, m_layerColorBg);

    return true;
}

void GuideTips::ResetBgSprite(std::string strBg)
{
    m_bgSprite->removeAllChildrenWithCleanup(true);
    m_bgSprite->setTexture(strBg);
}

void GuideTips::setBgPosition(cocos2d::Vec2 vec)
{
    m_bgSprite->setPosition(vec);
}

void GuideTips::setFlagPosition(cocos2d::Vec2 vec)
{
    m_flagSprite->setPosition(vec);
}

void GuideTips::addFlagSpriteAndPos(std::string strFlag, Vec2 pos)
{
    auto flagSprite = Sprite::create(strFlag);
    this->addChild(flagSprite);
    flagSprite->setPosition(pos);
}

void GuideTips::setVisible(bool bVisible)
{
    Layer::setVisible(bVisible);
    m_listenerTouch->setEnabled(bVisible);
}

void GuideTips::setTipsID(unsigned uID)
{
    m_uTipsID = uID;
}

ReadGuideInfo* ReadGuideInfo::m_ReadGuideInfo = nullptr;
ReadGuideInfo* ReadGuideInfo::getInstance()
{
    if (m_ReadGuideInfo == nullptr)
    {
        m_ReadGuideInfo = new ReadGuideInfo;
    }
    return m_ReadGuideInfo;
}

ReadGuideInfo::ReadGuideInfo()
{
    /*for (int i=0; i<12; i++)
    {
        unsigned status = 1;
        m_vecGuideStatus.push_back(status);
    }*/
}

void ReadGuideInfo::getAllGuideInfo(unsigned status)
{
    //m_vecGuideStatus[id] = status;
    m_vecGuideStatus.push_back(status);
}

bool ReadGuideInfo::readGuideInfo(unsigned id)
{
    /*tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError xmlRet = xmlDoc.Parse(FileUtils::getInstance()->getStringFromFile("guideinfo.xml").c_str());
    if (xmlRet > 0)
    {
        return;
    }
    auto xmlRoot = xmlDoc.RootElement();
    auto xmlScene = xmlRoot->FirstChildElement("scene");
    auto xmlData = xmlScene->FirstChildElement();
    while(xmlData != nullptr)
    {
        GuideItemInfo info;
        info.id = xmlData->IntAttribute("id");
        info.status = xmlData->IntAttribute("status");
        xmlData = xmlData->NextSiblingElement();
        m_vecGuideInfo.push_back(info);
    }*/

    /*bool status = false;
    char buff[32] ;
    sprintf(buff, "guideTips_%d_%d", g_globalMyData.m_iUserId, id) ;
    status = UserDefault::getInstance()->getBoolForKey(buff, false) ;

    UserDefault::getInstance()->flush();*/

    //return status;
    return m_vecGuideStatus[id];
}


void ReadGuideInfo::ModifyGuideInfo(unsigned id, unsigned status)
{
    /*char buff[32] ;
    sprintf(buff, "guideTips_%d_%d", g_globalMyData.m_iUserId, id) ;
    UserDefault::getInstance()->setBoolForKey(buff, status);
    UserDefault::getInstance()->flush();*/

    m_vecGuideStatus[id] = status;
    /*tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError xmlRet = xmlDoc.Parse(FileUtils::getInstance()->getStringFromFile("guideinfo.xml").c_str());
    if (xmlRet > 0)
    {
        
        return;
    }
    auto xmlRoot = xmlDoc.RootElement();
    auto xmlScene = xmlRoot->FirstChildElement("scene");
    auto xmlData = xmlScene->FirstChildElement();
    while(xmlData != nullptr)
    {
        if (xmlData->IntAttribute("id") == id)
        {
            xmlData->SetAttribute("status", status);
            xmlDoc.SaveFile("guideinfo.xml");
        }
        xmlData = xmlData->NextSiblingElement();
    }
    for (auto &item : m_vecGuideInfo)
    {
        if (id == item.id)
        {
            item.status = status;
        }
    }*/
}

void ReadGuideInfo::clearOldGuideInfo()
{
    m_vecGuideStatus.clear();
}