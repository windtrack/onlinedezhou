#ifndef __JETTON_H__
#define __JETTON_H__

#include "cocos2d.h"
#include <functional>

void ResetJetton(cocos2d::Sprite *pJetton, unsigned uJettonIdx);

class JettonFactory
{
public:
	JettonFactory(const std::string& strJettonTexture = "jetton.png");
	cocos2d::Sprite *CreateJetton(unsigned uJettonIdx);
	void ResetJetton(cocos2d::Sprite *pJetton, unsigned uJettonIdx);
private:
	std::string m_strTexture;
	float m_fItemWidth, m_fItemHeight;
};

typedef std::function<void(double,std::vector<int>&)> FuncCalcJettonIdx;

class SidePot : public cocos2d::Layer
{
public:
	virtual bool init(FuncCalcJettonIdx funcCalcJettonIdx);  
    static SidePot *create(FuncCalcJettonIdx funcCalcJettonIdx);

	double GetPotValue() const;
	void UpdatePotValue(double dPotValue);

private:
	SidePot();
	~SidePot();

	JettonFactory *m_pJettonFactory;
	FuncCalcJettonIdx m_funcCalcJettonIdx;
	cocos2d::Sprite *m_pSpriteBgJettonPot;
	cocos2d::Label *m_pLabelPotValue;
	double m_dPotValue;
	std::vector<cocos2d::Sprite*> m_vecPJetton;
};




#endif