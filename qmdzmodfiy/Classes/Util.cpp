#include "Util.h"
#include <sstream>
#include "external/ConvertUTF/ConvertUTF.h"
#include "FrameSprite.h"
#include "HttpSprite.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"
USING_NS_CC;
#define GAME_RESOURCE_BASE_URL "http://static.wepoker.cn/"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
std::string gb23122utf8(const char* gb2312)    
{  
    int len = MultiByteToWideChar(0, 0, gb2312, -1, NULL, 0);    
    wchar_t* wstr = new wchar_t[len+1];    
    memset(wstr, 0, len+1);    
    MultiByteToWideChar(0, 0, gb2312, -1, wstr, len);    
    len = WideCharToMultiByte(65001, 0, wstr, -1, NULL, 0, NULL, NULL);    
    char* str = new char[len+1];
    memset(str, 0, len+1);    
    WideCharToMultiByte(65001, 0, wstr, -1, str, len, NULL, NULL);   
	std::string strRet(str);
    if(wstr) delete[] wstr;  
	if (str) delete [] str;
    return strRet;
}
#endif

cocos2d::Label *CreateLabel(const std::string& strFont, int iSize, const std::string& str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	std::string strUtf8;
	auto pStr = str.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);
	if (bUtf8 && uL > 0)
	{
		strUtf8 = str;
	}
	else
	{
		strUtf8 = gb23122utf8(pStr);
	}
	return cocos2d::Label::createWithTTF(strUtf8, strFont, iSize);
#else
    auto pLabel = cocos2d::Label::createWithTTF(str, strFont, iSize);
    return pLabel;
#endif
}

Label *CreateLabel(const std::string& strFont, const std::string& strSysFont, int iSize, const std::string& str, TextHAlignment hAlignment, TextVAlignment vAlignment)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	std::string strUtf8;
	auto pStr = str.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);
	if (bUtf8 && uL > 0)
	{
		strUtf8 = str;
	}
	else
	{
		strUtf8 = gb23122utf8(pStr);
	}
	auto pLabel = cocos2d::Label::createWithTTF(strUtf8, strFont, iSize, Size::ZERO, hAlignment, vAlignment);
	if (pLabel == nullptr)
	{
		pLabel = cocos2d::Label::createWithSystemFont(strUtf8, strSysFont, iSize, Size::ZERO, hAlignment, vAlignment);
	}
	return pLabel;
#else
    auto pLabel = cocos2d::Label::createWithTTF(str, strFont, iSize, Size::ZERO, hAlignment, vAlignment);
    if (pLabel == nullptr)
    {
        pLabel = cocos2d::Label::createWithSystemFont(str, strSysFont, iSize, Size::ZERO, hAlignment, vAlignment);
    }
    return pLabel;
#endif
}

Label *CreateLabelMSYH(int iSize, const std::string& str, TextHAlignment hAlignment, TextVAlignment vAlignment)
{
	return CreateLabel("fzlt.ttf", "msyh", iSize, str, hAlignment, vAlignment);
}

Label *CreateLabelMSYHBD(int iSize, const std::string& str, TextHAlignment hAlignment, TextVAlignment vAlignment)
{
	return CreateLabel("fzltbd.ttf", "msyhbd", iSize, str, hAlignment, vAlignment);
}

void SetLabelString(cocos2d::Label *pLabel, const std::string& strTxt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto pStr = strTxt.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);

	
	if (bUtf8 && uL > 0)
	{
		pLabel->setString(strTxt);
	}
	else
	{
		std::string strUtf8 = gb23122utf8(pStr);
		pLabel->setString(strUtf8);
	}
#else
    pLabel->setString(strTxt);
#endif
}

void SetLabelTTFString(cocos2d::LabelTTF *pLabel, const std::string& strTxt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto pStr = strTxt.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);


	if (bUtf8 && uL > 0)
	{
		pLabel->setString(strTxt);
	}
	else
	{
		std::string strUtf8 = gb23122utf8(pStr);
		pLabel->setString(strUtf8);
	}
#else
	pLabel->setString(strTxt);
#endif
}


void setUITextString(cocos2d::ui::Text *pLabel, const std::string& strTxt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto pStr = strTxt.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);


	if (bUtf8 && uL > 0)
	{
		pLabel->setString(strTxt);
	}
	else
	{
		std::string strUtf8 = gb23122utf8(pStr);
		pLabel->setString(strUtf8);
	}
#else
	pLabel->setString(strTxt);
#endif
}


void SetLabelStringWithBoundWidth(cocos2d::Label *pLabel, const std::string& strTxt, unsigned uBoundWidth)
{
	std::string strUtf8;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto pStr = strTxt.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);

	if (bUtf8 && uL > 0)
	{
		strUtf8 = strTxt;
		pLabel->setString(strTxt);
	}
	else
	{
		strUtf8 = gb23122utf8(pStr);
		pLabel->setString(strUtf8);
	}
#else
    strUtf8 = strTxt;
    pLabel->setString(strTxt);
#endif

	unsigned uIdx = 0;
	std::vector<unsigned> vecCharIdx;
	std::vector<unsigned> vecCharBytes;
	while (uIdx < strUtf8.size())
	{		
		vecCharIdx.push_back(uIdx);
		unsigned uBytes = getNumBytesForUTF8(strUtf8[uIdx]);
		vecCharBytes.push_back(uBytes);
		uIdx += uBytes;
	}
	
	unsigned uCurCount = vecCharIdx.size();
	cocos2d::Size szLabel = pLabel->getContentSize();
	while (szLabel.width > uBoundWidth)
	{
		strUtf8.erase(vecCharIdx[uCurCount-1], vecCharBytes[uCurCount-1]);
		--uCurCount;
		std::string str = strUtf8;
		str += "...";
		pLabel->setString(str);
		szLabel = pLabel->getContentSize();
	}
}



void SetUITextStringWithBoundWidth(cocos2d::ui::Text *pLabel, const std::string& strTxt, unsigned uBoundWidth)
{
	std::string strUtf8;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto pStr = strTxt.c_str();
	const UTF8*pUtf8Str = (const UTF8*)pStr;
	const UTF8*pUtf8End = (const UTF8*)(pStr + strlen(pStr));
	unsigned uL = getUTF8StringLength(pUtf8Str);
	bool bUtf8 = isLegalUTF8Sequence(pUtf8Str, pUtf8End);

	if (bUtf8 && uL > 0)
	{
		strUtf8 = strTxt;
		pLabel->setString(strTxt);
	}
	else
	{
		strUtf8 = gb23122utf8(pStr);
		pLabel->setString(strUtf8);
	}
#else
	strUtf8 = strTxt;
	pLabel->setString(strTxt);
#endif

	unsigned uIdx = 0;
	std::vector<unsigned> vecCharIdx;
	std::vector<unsigned> vecCharBytes;
	while (uIdx < strUtf8.size())
	{		
		vecCharIdx.push_back(uIdx);
		unsigned uBytes = getNumBytesForUTF8(strUtf8[uIdx]);
		vecCharBytes.push_back(uBytes);
		uIdx += uBytes;
	}

	unsigned uCurCount = vecCharIdx.size();
	cocos2d::Size szLabel = pLabel->getContentSize();
	while (szLabel.width > uBoundWidth)
	{
		strUtf8.erase(vecCharIdx[uCurCount-1], vecCharBytes[uCurCount-1]);
		--uCurCount;
		std::string str = strUtf8;
		str += "...";
		pLabel->setString(str);
		szLabel = pLabel->getContentSize();
	}
}


cocos2d::Sprite *CreateFrameSprite(const std::string& strAnimateFile, unsigned uXFrames, unsigned uYFrames, unsigned uIdx /*= 0*/)
{
	//auto pSprite = cocos2d::Sprite::create(strAnimateFile);
	//cocos2d::Size sizeImg = pSprite->getContentSize();
	//double dFrameW = sizeImg.width / uXFrames;
	//double dFrameH = sizeImg.height / uYFrames;
	//pSprite->setTextureRect(cocos2d::Rect((uIdx % uXFrames) * dFrameW, (uIdx / uXFrames * dFrameH), dFrameW, dFrameH));
	//return pSprite;

	return FrameSprite::create(strAnimateFile, uXFrames, uYFrames, uIdx);
}

cocos2d::Animate *CreateAnimate(const std::string& strAnimateFile, unsigned uXFrames, unsigned uYFrames, unsigned uAllFrame, double dIntervalInSec /*= 0.1f*/, bool bRestoreOriginalFrame /*= true*/)
{
	//auto pSprite = cocos2d::Sprite::create(strAnimateFile);
	//cocos2d::Size sizeImg = pSprite->getContentSize();
	//double dFrameW = sizeImg.width / uXFrames;
	//double dFrameH = sizeImg.height / uYFrames;
	FrameSprite *pFrameSprite = FrameSprite::create(strAnimateFile, uXFrames, uYFrames);
	
	if (uAllFrame > uXFrames * uYFrames)
	{
		uAllFrame = uXFrames * uYFrames;
	}
	if (uAllFrame > 1)
	{
		auto animation = Animation::create();
		for (unsigned i = 0; i < uAllFrame; ++i)
		{
			auto spriteFrame = pFrameSprite->GetSpriteFrame(i);// SpriteFrame::create(strAnimateFile, Rect((i % uXFrames) * dFrameW, (i / uXFrames * dFrameH), dFrameW, dFrameH));
			animation->addSpriteFrame(spriteFrame);
		}
		animation->setDelayPerUnit(dIntervalInSec);
		animation->setRestoreOriginalFrame(bRestoreOriginalFrame);

		return Animate::create(animation);
	}
	return nullptr;
}

void PlayAnimation(cocos2d::Sprite *pSprite, const std::string& strAnimateFile, unsigned uXFrames, unsigned uYFrames, unsigned uAllFrame, double dIntervalInSec /*= 0.1f*/, bool bRestoreOriginalFrame /*= true*/,  unsigned uRepeatTime/* = 0xffffffff*/, bool bHideWhenFinish /*= true*/)
{
	if (bHideWhenFinish)
	{
		pSprite->runAction(cocos2d::Sequence::create(Show::create(), Repeat::create(CreateAnimate(strAnimateFile, uXFrames, uYFrames, uAllFrame, dIntervalInSec, bRestoreOriginalFrame), uRepeatTime), Hide::create(), nullptr));
	}
	else
	{
		pSprite->runAction(cocos2d::Sequence::create(Show::create(), Repeat::create(CreateAnimate(strAnimateFile, uXFrames, uYFrames, uAllFrame, dIntervalInSec, bRestoreOriginalFrame), uRepeatTime), nullptr));
	}
}

Label *CreateLabelMSYHAndAnchorPosClrAddTo(int iFontSize, const std::string& str, const Vec2& anchor, float fX, float fY, const Color4B& clr, Node*pParent, int iZOrder)
{
	auto label = CreateLabelMSYH(iFontSize, str);
	label->setAnchorPoint(anchor);
	label->setPosition(Vec2(fX, fY));
	label->setTextColor(clr);
	pParent->addChild(label, iZOrder);
	return label;
}

Label *CreateLabelMSYHBDAndAnchorPosClrAddTo(int iFontSize, const std::string& str, const Vec2& anchor, float fX, float fY, const Color4B& clr,  Node*pParent, int iZOrder)
{
	auto label = CreateLabelMSYHBD(iFontSize, str);
	label->setAnchorPoint(anchor);
	label->setPosition(Vec2(fX, fY));
	label->setTextColor(clr);
	pParent->addChild(label, iZOrder);
	return label;
}

HttpSprite *CreateUserHeadSprite(const std::string& strDefaultSprite, 
								 const std::string& strClipMask, 
								 Node *pNodeAddTo, 
								 const Vec2& ptAddTo,
								 const std::string& strSpriteCover)
{
	auto pClipMask = Sprite::create(strClipMask);
	auto sz = pClipMask->getContentSize();
	pClipMask->setPosition(sz.width / 2, sz.height / 2);
	pClipMask->retain();

	auto pClip = CreateNodeAndPosAddTo<ClippingNode>(ptAddTo.x, ptAddTo.y, pNodeAddTo, 0);
	pClip->setContentSize(sz);

	auto pHeadIcon = CreateNode2AndPosAddTo<HttpSprite>(strDefaultSprite, sz, 0, 0, pClip, 0);
	CreateNode1AndPosAddTo<Sprite>(strSpriteCover, ptAddTo.x + sz.width / 2, ptAddTo.y + sz.height / 2, pNodeAddTo, 0);
	pClip->setStencil(pClipMask);
	pClip->setAlphaThreshold(0.5);

	return pHeadIcon;
}

bool SetHttpSpriteURL(HttpSprite* pHttpSprite, const std::string& strNewUrl)
{


	if (strNewUrl.length() > 7 && 
		strNewUrl[0] == 'h' &&
		strNewUrl[1] == 't' &&
		strNewUrl[2] == 't' &&
		strNewUrl[3] == 'p' &&
		strNewUrl[4] == ':' &&
		strNewUrl[5] == '//' &&
		strNewUrl[6] == '//')
	{
		pHttpSprite->ResetSourceUrl(strNewUrl);
		return true;
	}
	else if (strNewUrl.length() > 0)
	{


		//std::string strUrl = "http://192.168.1.201:81/" + strNewUrl;
		std::string strUrl = GAME_RESOURCE_BASE_URL + strNewUrl;
		pHttpSprite->ResetSourceUrl(strUrl);
		return true;
	}
	//pHttpSprite->ResetSourceUrl("hall/xingxiang2.png");


	return false;
}

#define GAME_RESOURCE_ICON_PREFIX "/xtuserlogo/200x200/"
bool SetIconUrl(HttpSprite* pHttpSprite, const std::string& strNewUrl)
{
	if (strNewUrl.length() > 0)
	{
		return SetHttpSpriteURL(pHttpSprite, GAME_RESOURCE_ICON_PREFIX + strNewUrl + "?1=1");
	}
	else
	{
		 pHttpSprite->ResetSourceUrl("defaulticon.png");
		 return true ;
	}

	return false;
}
bool SetLocalIconUrl(HttpSprite* pHttpSprite, const std::string& strNewUrl)
{
	if (strNewUrl.length() > 0)
	{
		pHttpSprite->ResetToLocalSprite(strNewUrl);
		return true ;
	}

	return false;
}

#define GAME_RESOURCE_FAMILYICON_PREFIX "/texas/f/"
bool SetFamilyIconUrl(HttpSprite* pHttpSprite, const std::string& strNewUrl)
{
	if (strNewUrl.length() > 0)
	{
		return SetHttpSpriteURL(pHttpSprite, GAME_RESOURCE_FAMILYICON_PREFIX + strNewUrl + "?1=1");
	}
	return false;
}


void PlayBackGroundSound(const std::string& strSound, bool bLoop)
{
	auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	if (!pAudioEngine->isBackgroundMusicPlaying())
	{
		pAudioEngine->playBackgroundMusic(strSound.c_str(), bLoop);
	}
	
}

void StopBackGroundSound(bool bReleaseData)
{
	auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	pAudioEngine->stopBackgroundMusic(bReleaseData);
}

unsigned PlayEffectSound(const std::string& strSound, bool bLoop)
{
	if (!GameData::getInstance()->getSoundOn())
	{
		return 0;
	}

	auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	return pAudioEngine->playEffect(strSound.c_str(), bLoop);
}

std::string ConvertUrl2LocalName(const std::string& strUrl)
{	
	char szBuf[1024];
	strncpy(szBuf, strUrl.c_str(), 1024);
	unsigned uIdx = 0;
	while (szBuf[uIdx] != 0)
	{
		if ((szBuf[uIdx] >= 'a' && szBuf[uIdx] <= 'z') ||
			(szBuf[uIdx] >= 'A' && szBuf[uIdx] <= 'Z') ||
			(szBuf[uIdx] >= '0' && szBuf[uIdx] <= '9') ||
			szBuf[uIdx] == '.')
		{
		}
		else
		{
			szBuf[uIdx] = '_';
		}
		++uIdx;
	}
	std::string strSrcUrl(szBuf);
	return strSrcUrl;
}

std::string ConvertUrl2LocalFullPath(const std::string& strUrl)
{
	FileUtils *pFileUtils = FileUtils::getInstance();
	std::string strWritableDir = pFileUtils->getWritablePath();
	std::string strFullPath = strWritableDir + ConvertUrl2LocalName(strUrl);
	return strFullPath;
}

bool IsFileExist(const std::string& strFilePath)
{
	FileUtils *pFileUtils = FileUtils::getInstance();
	return pFileUtils->isFileExist(strFilePath);
}

std::string do_fraction(long double value, int decplaces /*=1*/)
{
	std::stringstream sstream;
	int prec = std::numeric_limits<long double>::digits10; // 18
	sstream.precision(prec); //覆盖默认精度
	sstream << value;
	std::string str= sstream.str(); //从流中取出字符串
	char DECIMAL_POINT = '.';
	std::size_t n = str.find(DECIMAL_POINT);
	if ((n!=std::string::npos) //有小数点吗？
		&& (str.size() > n + decplaces)) //后面至少还有decplaces位吗？
	{
		str[n + decplaces] = '\0';//覆盖第一个多余的数
	}
	return std::string(str.c_str());
}

int UnicodeToUtf8(unsigned long unic, unsigned char szOutput[8])  
{  
	if ( unic <= 0x0000007F )  
	{  
		// * U-00000000 - U-0000007F:  0xxxxxxx  
		szOutput[0]     = (unic & 0x7F);  
		return 1;  
	}  
	else if ( unic >= 0x00000080 && unic <= 0x000007FF )  
	{  
		// * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
		szOutput[1] = (unic & 0x3F) | 0x80;  
		szOutput[0]     = ((unic >> 6) & 0x1F) | 0xC0;  
		return 2;  
	}  
	else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )  
	{  
		// * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
		szOutput[2] = (unic & 0x3F) | 0x80;  
		szOutput[1] = ((unic >>  6) & 0x3F) | 0x80;  
		szOutput[0]     = ((unic >> 12) & 0x0F) | 0xE0;  
		return 3;  
	}  
	else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )  
	{  
		// * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
		szOutput[3] = (unic & 0x3F) | 0x80;  
		szOutput[2] = ((unic >>  6) & 0x3F) | 0x80;  
		szOutput[1] = ((unic >> 12) & 0x3F) | 0x80;  
		szOutput[0]     = ((unic >> 18) & 0x07) | 0xF0;  
		return 4;  
	}  
	else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )  
	{  
		// * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
		szOutput[4] = (unic & 0x3F) | 0x80;  
		szOutput[3] = ((unic >>  6) & 0x3F) | 0x80;  
		szOutput[2] = ((unic >> 12) & 0x3F) | 0x80;  
		szOutput[1] = ((unic >> 18) & 0x3F) | 0x80;  
		szOutput[0]     = ((unic >> 24) & 0x03) | 0xF8;  
		return 5;  
	}  
	else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )  
	{  
		// * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
		szOutput[5] = (unic & 0x3F) | 0x80;  
		szOutput[4] = ((unic >>  6) & 0x3F) | 0x80;  
		szOutput[3] = ((unic >> 12) & 0x3F) | 0x80;  
		szOutput[2] = ((unic >> 18) & 0x3F) | 0x80;  
		szOutput[1] = ((unic >> 24) & 0x3F) | 0x80;  
		szOutput[0]     = ((unic >> 30) & 0x01) | 0xFC;  
		return 6;  
	}  

	return 0;  
}

void Replain2UTF8(const char *str, int len, std::string& out)
{
	char szUnicode[16];
	unsigned char szUtf8[8];
	unsigned uUnicode;
	int i = 0;
	for (; i < (len - 1);)
	{
		if('\\'==str[i] && 'u' == str[i+1])
		{
			memset(szUnicode, 0, sizeof(szUnicode));
			strncpy(szUnicode, &str[i+2], 4);
			sscanf(szUnicode, "%x", &uUnicode);
			int n = UnicodeToUtf8(uUnicode, szUtf8);
			for (int j = 0; j < n; ++j)
			{
				out.push_back(szUtf8[j]);
			}
			i += 6;
		}
		else if ('\\'==str[i] && '\"' == str[i+1])
		{
			out.push_back('\"');
			i+=2;
		}
		else if ('\"'==str[i] && '{' == str[i+1])
		{
			out.push_back('{');
			i+=2;
		}
		else if ('}'==str[i] && '\"' == str[i+1])
		{
			out.push_back('}');
			i+=2;
		}
		else
		{
			out.push_back(str[i]);
			i++;
		}
	}
	if (i < len)
	{
		out.push_back(str[i]);
	}
	out.push_back(0);
}

std::string FormatCash(double value)
{
	std::string strRet;
    auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
        if (abs(dValueFormating) >= dLow && abs(dValueFormating) < dHigh)
        {
            double dHigh = abs(dValueFormating) / dLow;
            double dRest = abs(dValueFormating) - dLow * unsigned(dHigh);
	/*		if (dHigh>=10)
			{
				dHigh = (int )dHigh ;
			}*/
            strRet = do_fraction(dHigh, (dRest >= dLow / 100.0f) ? 3 : 0);
            if (dValueFormating < 0)
                strRet = "-" + strRet;
            strRet += strUnit;
            return true;
        }
        return false;
    };
    if (pFunFormat(value, 10000.0f, 100000000.0f, "万", strRet))
    {
    }
    else if (pFunFormat(value, 100000000.0f, 1000000000000.0f, "亿", strRet))
    {
    }
    else if (pFunFormat(value, 1000000000000.0f, 10000000000000000.0f, "兆", strRet))
    {
    }
    else
    {
        strRet = do_fraction(value);
    }
	return strRet;
}
std::string FormatCash_miss(double value)
{
	std::string strRet;
    auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
        if (abs(dValueFormating) >= dLow && abs(dValueFormating) < dHigh)
        {
            double dHigh = abs(dValueFormating) / dLow;
            double dRest = abs(dValueFormating) - dLow * unsigned(dHigh);
			if (dHigh>=10)
			{
				dHigh = (int )dHigh ;
			}
            strRet = do_fraction(dHigh, (dRest >= dLow / 100.0f) ? 3 : 0);
            if (dValueFormating < 0)
                strRet = "-" + strRet;
            strRet += strUnit;
            return true;
        }
        return false;
    };
    if (pFunFormat(value, 10000.0f, 100000000.0f, "万", strRet))
    {
    }
    else if (pFunFormat(value, 100000000.0f, 1000000000000.0f, "亿", strRet))
    {
    }
    else if (pFunFormat(value, 1000000000000.0f, 10000000000000000.0f, "兆", strRet))
    {
    }
    else
    {
        strRet = do_fraction(value);
    }
	return strRet;
}
std::string FormatCash0(double value)
{
    std::string strRet;
    auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
        if (abs(dValueFormating) >= dLow && abs(dValueFormating) < dHigh)
        {
            double dHigh = abs(dValueFormating) / dLow;
            double dRest = abs(dValueFormating) - dLow * unsigned(dHigh);
            strRet = do_fraction(dHigh, 0);
            if (dValueFormating < 0)
                strRet = "-" + strRet;
            strRet += strUnit;
            return true;
        }
        return false;
    };
    if (pFunFormat(value, 10000.0f, 100000000.0f, "万", strRet))
    {
    }
    else if (pFunFormat(value, 100000000.0f, 1000000000000.0f, "亿", strRet))
    {
    }
    else if (pFunFormat(value, 1000000000000.0f, 10000000000000000.0f, "兆", strRet))
    {
    }
    else
    {
        strRet = do_fraction(value);
    }
    return strRet;
}

std::string FormatCash1(double value)
{
	std::string strRet;
	auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
		if (dValueFormating >= dLow && dValueFormating < dHigh)
		{
			double dHigh = dValueFormating / dLow;
			double dRest = dValueFormating - dLow * unsigned(dHigh);
			strRet = do_fraction(dHigh, 0);
			strRet += strUnit;
			return true;
		}
		return false;
	};
	if (pFunFormat(value, 1000.0f, 10000.0f, "K", strRet))
	{
	}
	else if (pFunFormat(value, 10000.0f, 100000000.0f, "万", strRet))
	{
	}
	else if (pFunFormat(value, 100000000.0f, 1000000000000.0f, "亿", strRet))
	{
	}
	else if (pFunFormat(value, 1000000000000.0f, 10000000000000000.0f, "兆", strRet))
	{
	}
	else
	{
		strRet = do_fraction(value);
	}
	return strRet;
}
std::string FormatCash2(double value)
{
	std::string strRet;
	auto pFunFormat = [](double dValueFormating, double dLow, double dHigh, const std::string& strUnit, std::string& strRet){
		if (dValueFormating >= dLow && dValueFormating < dHigh)
		{
			double dHigh = dValueFormating / dLow;
			double dRest = dValueFormating - dLow * unsigned(dHigh);
			strRet = do_fraction(dHigh, (dRest >= dLow / 100.0f) ? 2 : 0);
			strRet += strUnit;
			return true;
		}
		return false;
	};
	if (pFunFormat(value, 1000.0f, 10000.0f, "K", strRet))
	{
	}
	else if (pFunFormat(value, 10000.0f, 100000000.0f, "万", strRet))
	{
	}
	else if (pFunFormat(value, 100000000.0f, 1000000000000.0f, "亿", strRet))
	{
	}
	else if (pFunFormat(value, 1000000000000.0f, 10000000000000000.0f, "兆", strRet))
	{
	}
	else
	{
		strRet = do_fraction(value);
	}
	return strRet;
}

std::string FormatCash3(double value)
{
	std::string strRet;
	strRet = do_fraction(abs(value));
	size_t len = strRet.length();
	for(int index =(int) len-4; index > 0; index -= 4)
		strRet.insert(index, ",");
    if (value < 0)
        strRet = "-" + strRet;
	return strRet;
}

// 对Label的多文本内容进行自动的换行
std::string FormatAutoLine(std::string value)
{
	int size = value.length();
	int pos = value.find(".");
	for(int lenx = 0; lenx < size - 1; lenx++)
	{
		std::string str2 = value.substr(pos,1); 
		if(str2.compare(".") == 0)//从位置0开始查找字符i在当前字符串的位置
		{
			value.insert(pos - 1,"\n");
			pos += 2;
		}
		if(pos < size - 1)
			pos++;
	} 
	//std::cout << value << std::endl;
	return value;
}


std::string FormatCash4(double value)
{
    if (abs(value) >= 0 && abs(value) < 10000)
    {
        return FormatCash0(value);
    }
    else if(abs(value) >= 10000 && abs(value) <= 1000000)
    {
        return FormatCash(value);
    }
    else if (abs(value) > 1000000)
    {
        return FormatCash_miss(value);
    }

	return FormatCash(value);
	//if (abs(value) >= 0 && abs(value) < 10000)
	//{
	//	return FormatCash0(value);
	//}
	//else if(abs(value) >= 10000 && abs(value) <= 1000000)
	//{
	//	return FormatCash(value);
	//}
	//else if (abs(value) > 1000000 && abs(value) <= 100000000)
	//{
	//	return FormatCash0(value);
	//}
	//else if (abs(value) > 100000000)
	//{
	//	return FormatCash(value);
	//}
	//return FormatCash(value);
}
bool doubleIsInt(double input)
{
    const double eps = 1e-10;
    if (input - (double)((int)input) < eps)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double getUpInt(double input, double multi)
{
    int tmp = input/multi;
    return (tmp+1)*multi;
}

int getStrByteNumOfUTF8(std::string strSrc, int UTF8Num)
{
    unsigned uIdx = 0;
    std::vector<unsigned> vecCharBytes;
    while (uIdx < strSrc.size())
    {		
        unsigned uBytes = getNumBytesForUTF8(strSrc[uIdx]);
        vecCharBytes.push_back(uBytes);
        uIdx += uBytes;
    }
    unsigned sz = vecCharBytes.size();
    unsigned ret = 0;
    if (sz > UTF8Num)
    {
        sz = UTF8Num;
    }

    for (int i=0; i< sz; i++)
    {
        ret = ret + vecCharBytes[i];
    }

    return ret;
}

long getSystemTime()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv,NULL);
	timep = tv.tv_sec;
#endif

	return (long)timep;
}

long getCurrentTime()
{
    struct timeval tv;    
    gettimeofday(&tv,NULL);    
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int stringSplit(const std::string& str, std::vector<std::string>& ret_, std::string sep)
{
    if (str.empty())
    {
        return 0;
    }
    
    std::string tmp;
    std::string::size_type pos_begin = str.find_first_not_of(sep);
    std::string::size_type comma_pos = 0;
    
    while (pos_begin != std::string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != std::string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }
        
        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}