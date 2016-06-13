#ifndef __LINELABEL_H__
#define __LINELABEL_H__

#include "cocos2d.h"
#include <vector>

class LineLabel : public cocos2d::Layer
{
public:
	CREATE_FUNC(LineLabel);

	void SetVAlignment(cocos2d::TextVAlignment vAlignment);
	void Appand(int iFontSize, const std::string& strFontName, const std::string& str, const cocos2d::Color4B& clr);
	void resetLable() ;
protected:
	virtual bool init();

	void ResetLablePosY(float fBoundH, cocos2d::Label *pLabel);



private:
	std::vector<cocos2d::Label*> m_vecLabels;
	cocos2d::TextVAlignment m_vAlignment;
};

#endif