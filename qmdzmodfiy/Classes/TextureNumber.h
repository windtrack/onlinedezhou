#ifndef __TEXTURENUMBER_H__
#define __TEXTURENUMBER_H__

#include "cocos2d.h"

enum class TextureNumberHAlignment
{
	Left,
	Center,
	Right,
};

enum class TextureNumberVAlignment
{
	Top,
	Center,
	Bottom,
};

class TextureNumber : public cocos2d::Layer
{
public:
	virtual bool init(const std::string& strNumTexture, const std::string& strDotTexture, const std::string& strUnitTexture, bool bHasPreFlag);
	virtual bool init(const std::string& strNumTexture, bool bHasPreFlag);

	static TextureNumber *create(const std::string& strNumTexture, const std::string& strDotTexture, const std::string& strUnitTexture, bool bHasPreFlag);
	static TextureNumber *create(const std::string& strNumTexture, bool bHasPreFlag);

	void SetHAlignment(TextureNumberHAlignment hAlign);
	void SetVAlignment(TextureNumberVAlignment vAlign);

	void UpdateNumber(double dNumber);

	// 数字前面插入0
	void UpdateZeroNumber(double dNumber, int size);

private:
	void ReAdjustFrame();

private:
	bool m_bHasPreFlag;
	bool m_bNeedFormat;
	double m_dNumber;
	std::string m_strNumTexture, m_strDotTexture, m_strUnitTexture;
	TextureNumberHAlignment m_hAlign;
	TextureNumberVAlignment m_vAlign;
	float m_fNumFrameW, m_fNumFrameH, m_fUnitFrameW, m_fUnitFrameH, m_fDotFrameW, m_fDotFrameH;
	std::vector<cocos2d::Sprite*> m_vecSpriteNumbers;
	std::vector<unsigned> m_vecNumbers;
};


#endif