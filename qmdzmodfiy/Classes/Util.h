#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"
#include "UICommon.h"
#define REG_TOUCH_ONEBYONE_BME(mfuncOnTouchBegan, mfuncOnTouchMove, mfuncOnTouchEnded, target) if (true) { \
	EventListenerTouchOneByOne *pTouchLisenter = EventListenerTouchOneByOne::create(); \
	pTouchLisenter->onTouchBegan = CC_CALLBACK_2(mfuncOnTouchBegan, target);	\
	pTouchLisenter->onTouchMoved = CC_CALLBACK_2(mfuncOnTouchMove, target);	\
	pTouchLisenter->onTouchEnded = CC_CALLBACK_2(mfuncOnTouchEnded, target);	\
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchLisenter, target);}	

#define REG_TOUCH_ONEBYONE_SWALLOW_BME(mfuncOnTouchBegan, mfuncOnTouchMove, mfuncOnTouchEnded, target) if (true) { \
	EventListenerTouchOneByOne *pTouchLisenter = EventListenerTouchOneByOne::create(); \
	pTouchLisenter->setSwallowTouches(true);	\
	pTouchLisenter->onTouchBegan = CC_CALLBACK_2(mfuncOnTouchBegan, target);	\
	pTouchLisenter->onTouchMoved = CC_CALLBACK_2(mfuncOnTouchMove, target);	\
	pTouchLisenter->onTouchEnded = CC_CALLBACK_2(mfuncOnTouchEnded, target);	\
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchLisenter, target);}	

#define REG_TOUCH_ONEBYONE_BE(mfuncOnTouchBegan, mfuncOnTouchEnded, target) if (true) { \
	EventListenerTouchOneByOne *pTouchLisenter = EventListenerTouchOneByOne::create(); \
	pTouchLisenter->onTouchBegan = CC_CALLBACK_2(mfuncOnTouchBegan, target);	\
	pTouchLisenter->onTouchEnded = CC_CALLBACK_2(mfuncOnTouchEnded, target);	\
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchLisenter, target);}	

#define REG_TOUCH_ONEBYONE_SWALLOW_BE(mfuncOnTouchBegan, mfuncOnTouchEnded, target) if (true) { \
	EventListenerTouchOneByOne *pTouchLisenter = EventListenerTouchOneByOne::create(); \
	pTouchLisenter->setSwallowTouches(true);	\
	pTouchLisenter->onTouchBegan = CC_CALLBACK_2(mfuncOnTouchBegan, target);	\
	pTouchLisenter->onTouchEnded = CC_CALLBACK_2(mfuncOnTouchEnded, target);	\
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchLisenter, target);}	

#define REG_TOUCH_ONEBYONE_B(mfuncOnTouchBegan, target) if (true) { \
	EventListenerTouchOneByOne *pTouchLisenter = EventListenerTouchOneByOne::create(); \
	pTouchLisenter->onTouchBegan = CC_CALLBACK_2(mfuncOnTouchBegan, target);	\
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchLisenter, target);}	

#define REG_TOUCH_ONEBYONE_SWALLOW_B(mfuncOnTouchBegan, target) if (true) { \
	EventListenerTouchOneByOne *pTouchLisenter = EventListenerTouchOneByOne::create(); \
	pTouchLisenter->setSwallowTouches(true);	\
	pTouchLisenter->onTouchBegan = CC_CALLBACK_2(mfuncOnTouchBegan, target);	\
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pTouchLisenter, target);}	


#define ADD_NODE_CENTER(pNode, zOrder) if (true) {\
	cocos2d::Size sz = this->getContentSize(); \
	pNode->setPosition(Vec2(sz.width / 2, sz.height / 2)); \
	addChild(pNode, zOrder);	\
	}

cocos2d::Label *CreateLabel(const std::string& strFont, int iSize, const std::string& str);
cocos2d::Label *CreateLabel(const std::string& strFont, const std::string& strSysFont, int iSize, const std::string& str, cocos2d::TextHAlignment hAlignment, cocos2d::TextVAlignment vAlignment);
cocos2d::Label *CreateLabelMSYH(int iSize, const std::string& str = "", cocos2d::TextHAlignment hAlignment = cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment vAlignment = cocos2d::TextVAlignment::CENTER);
cocos2d::Label *CreateLabelMSYHBD(int iSize, const std::string& str = "", cocos2d::TextHAlignment hAlignment = cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment vAlignment = cocos2d::TextVAlignment::CENTER);

void SetLabelString(cocos2d::Label *pLabel, const std::string& strTxt);
void SetLabelTTFString(cocos2d::LabelTTF *pLabel, const std::string& strTxt);
void setUITextString(cocos2d::ui::Text *pLabel, const std::string& strTxt) ;
void SetLabelStringWithBoundWidth(cocos2d::Label *pLabel, const std::string& strTxt, unsigned uBoundWidth);
void SetUITextStringWithBoundWidth(cocos2d::ui::Text *pLabel, const std::string& strTxt, unsigned uBoundWidth);
cocos2d::Sprite *CreateFrameSprite(const std::string& strAnimateFile, unsigned uXFrames, unsigned uYFrames, unsigned uIdx = 0);
cocos2d::Animate *CreateAnimate(const std::string& strAnimateFile, unsigned uXFrames, unsigned uYFrames, unsigned uAllFrame, double dIntervalInSec = 0.1f, bool bRestoreOriginalFrame = true);
void PlayAnimation(cocos2d::Sprite *pSprite, const std::string& strAnimateFile, unsigned uXFrames, unsigned uYFrames, unsigned uAllFrame, double dIntervalInSec = 0.1f, bool bRestoreOriginalFrame = true, unsigned uRepeatTime = 0xffffffff, bool bHideWhenFinish = true);

template <typename NodeType>
NodeType *CreateNodeAndPosAddTo(float fX, float fY, cocos2d::Node*pParent, int iZOrder)
{
	auto pNode = NodeType::create();
	pNode->setPosition(cocos2d::Vec2(fX, fY));
	pParent->addChild(pNode, iZOrder);
	return pNode;
}

template <typename NodeType, typename T1>
NodeType *CreateNode1AndPosAddTo(const T1& t1, float fX, float fY, cocos2d::Node*pParent, int iZOrder)
{
	auto pNode = NodeType::create(t1);
	pNode->setPosition(cocos2d::Vec2(fX, fY));
	pParent->addChild(pNode, iZOrder);
	return pNode;
}

template <typename NodeType, typename T1, typename T2>
NodeType *CreateNode2AndPosAddTo(const T1& t1, const T2& t2, float fX, float fY, cocos2d::Node*pParent, int iZOrder)
{
	auto pNode = NodeType::create(t1, t2);
	pNode->setPosition(cocos2d::Vec2(fX, fY));
	pParent->addChild(pNode, iZOrder);
	return pNode;
}

template <typename NodeType, typename T1, typename T2, typename T3>
NodeType *CreateNode3AndPosAddTo(const T1& t1, const T2& t2, const T3& t3, float fX, float fY, cocos2d::Node*pParent, int iZOrder)
{
	auto pNode = NodeType::create(t1, t2, t3);
	pNode->setPosition(cocos2d::Vec2(fX, fY));
	pParent->addChild(pNode, iZOrder);
	return pNode;
}

template <typename NodeType, typename T1, typename T2, typename T3, typename T4>
NodeType *CreateNode4AndPosAddTo(const T1& t1, const T2& t2, const T3& t3, const T4& t4, float fX, float fY, cocos2d::Node*pParent, int iZOrder)
{
	auto pNode = NodeType::create(t1, t2, t3, t4);
	pNode->setPosition(cocos2d::Vec2(fX, fY));
	pParent->addChild(pNode, iZOrder);
	return pNode;
}

cocos2d::Label *CreateLabelMSYHAndAnchorPosClrAddTo(int iFontSize, const std::string& str, const cocos2d::Vec2& anchor, float fX, float fY, const cocos2d::Color4B& clr, cocos2d::Node*pParent, int iZOrder);
cocos2d::Label *CreateLabelMSYHBDAndAnchorPosClrAddTo(int iFontSize, const std::string& str, const cocos2d::Vec2& anchor, float fX, float fY, const cocos2d::Color4B& clr,  cocos2d::Node*pParent, int iZOrder);

class HttpSprite;
HttpSprite *CreateUserHeadSprite(const std::string& strDefaultSprite, const std::string& strClipMask, 
								 cocos2d::Node *pNodeAddTo, const cocos2d::Vec2& ptAddTo,
								 const std::string& strSpriteCover);




//#define GAME_RESOURCE_BASE_URL "http://192.168.1.201:8001/"

bool SetHttpSpriteURL(HttpSprite* pHttpSprite, const std::string& strNewUrl);
bool SetIconUrl(HttpSprite* pHttpSprite, const std::string& strNewUrl);
bool SetLocalIconUrl(HttpSprite* pHttpSprite, const std::string& strNewUrl);
bool SetFamilyIconUrl(HttpSprite* pHttpSprite, const std::string& strNewUrl);

void PlayBackGroundSound(const std::string& strSound, bool bLoop = false);
void StopBackGroundSound(bool bReleaseData);
unsigned PlayEffectSound(const std::string& strSound, bool bLoop = false);

std::string ConvertUrl2LocalName(const std::string& strUrl);
std::string ConvertUrl2LocalFullPath(const std::string& strUrl);
bool IsFileExist(const std::string& strFilePath);

// 将浮点数据用小数点格式化到string
std::string do_fraction(long double value, int decplaces = 0);

// 将utf8字符串中的\u23a4之类的unicode字符串转换成utf8编码
void Replain2UTF8(const char *str, int len, std::string& out);

// 将强类型枚举对象转换到对应的整数值
template <typename Enumeration>
auto as_integer(Enumeration const value)
-> typename std::underlying_type<Enumeration>::type
{
return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

// 将10000-》1万 21200->2.12万  主要后缀万 亿 兆
std::string FormatCash(double value);
//将10000-》1万 21200->2万  主要后缀万 亿 兆
std::string FormatCash0(double value);
// 将10000-》1万 2100->2千  主要后缀千 万 亿 兆
std::string FormatCash1(double value);
// 将10000-》1万 2100->2.1千  主要后缀千 万 亿 兆
std::string FormatCash2(double value);

std::string FormatCash_miss(double value) ;
// 将所有纯数字的金钱转换并加上,(逗号)
std::string FormatCash3(double value);

std::string FormatCash4(double value);

std::string FormatAutoLine(std::string value);

//判断double类型的是否为整
bool doubleIsInt(double input);
//double类型向上去multi倍数
double getUpInt(double input, double multi);
//返回指定长度UTF8字符串的字节数
int getStrByteNumOfUTF8(std::string strSrc, int UTF8Num);

// 返回当前的时间，以秒为单位。
long getSystemTime();
//返回当前时间，以毫秒为单位
long getCurrentTime();

int stringSplit(const std::string& str, std::vector<std::string>& ret_, std::string sep);
#endif