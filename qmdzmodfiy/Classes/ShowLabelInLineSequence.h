#ifndef __SHOWLABELINLINESEQUENCE_H__
#define __SHOWLABELINLINESEQUENCE_H__

#include "cocos2d.h"
class AnimationManager;
class ShowLabelInLineSequence : public cocos2d::Layer
{
public:
	static ShowLabelInLineSequence *create(const std::string& strBg, const std::string& strFont, float fFontSize, int iLineMaxWidth);
	static ShowLabelInLineSequence *create(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth);
    static ShowLabelInLineSequence *create(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth, unsigned uShowLineNum);
	void SetText(const std::string& str);
	void SetTextColor(const cocos2d::Color4B& clr);
	void SetHAlignment(cocos2d::TextHAlignment halign);
	void SetLastLineFinishedCB(std::function<void(ShowLabelInLineSequence*)>);
    void MultLineSetText(const std::string& str);
protected:
	virtual bool init(const std::string& strBg, const std::string& strFont, float fFontSize, int iLineMaxWidth);
	virtual bool init(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth,unsigned uShowLineNum=1);
    //virtual bool init(const std::string& strBg, cocos2d::Label *pLabel, int iLineMaxWidth, unsigned uShowLineNum);
	void onScheduleChangeLine(float fDelta);
    void onScheduleChangeMultLine(float fDelta);
    void ResetClipPosAndSz(int iLine);
private:
	std::function<void(ShowLabelInLineSequence*)> m_funLstLineFinishedCB;
	cocos2d::Label *m_pLabel;
	cocos2d::ClippingNode *m_pClip;
	cocos2d::DrawNode *m_pDrawNodeStencil;
	int m_iCurLine;
    unsigned m_uShowLineNum;
    bool m_bVisible;
};

class TableTip : public cocos2d::Layer
{
public:
	static TableTip *create(const std::string& strBg, const std::string& strFont, int iFontSize, unsigned uMaxLineWidth);
	void UserChat(const std::string& strNickName, const std::string& strChat);
	void UserExpress(const std::string& strNickName, unsigned uExpressIdx);
	// 打赏提示 领取奖励 自动购买补充筹码提示 玩家离开 设置补充筹码成功提示 玩家站起旁观提示
	void SystemTip(const std::string& strTip);
    void SystemTip(const std::string& strTip, float fDelay);

protected:
	virtual bool init(const std::string& strBg, const std::string& strFont, int iFontSize, unsigned uMaxLineWidth);

	cocos2d::ClippingNode *m_pClip;
	cocos2d::Sprite *m_pSpriteBG;
	cocos2d::DrawNode *m_pClipStencil;
	float m_fLabelFontH;
	cocos2d::Label *m_pLabel;
	cocos2d::Sprite *m_pSpriteUsrCharIcon;
	cocos2d::Sprite *m_pSpriteExpress;
		AnimationManager* m_biaoqingAni[2];

};


#endif