#ifndef __CARD_H__
#define __CARD_H__


#include "cocos2d.h"

enum 
{
	InvalidCard = 0x41
};

class FrameSprite;

unsigned GetCardFrameIdx(unsigned char cbCardData);
void ResetCardData(FrameSprite*pSpriteCard, unsigned char cbCardData);
// 翻牌显示动画
void PlayAnimateOpenCard(FrameSprite *pCardSprite, unsigned char cbCardData, std::function<void()> funCBOpened = nullptr);
// 翻牌显示手上牌动画
void PlayAnimateOpenHandCard(FrameSprite *pCardSprite, unsigned char cbCardData, unsigned uCardIdx);

//// 重新设置牌数据
//void ResetCardData(cocos2d::Sprite *pCardSprite, unsigned char cbCardData);
//
//// 翻牌显示动画
//void PlayAnimateOpenCard(cocos2d::Sprite *pCardSprite, unsigned char cbCardData);
//
//// 翻牌显示手上牌动画
//void PlayAnimateOpenHandCard(cocos2d::Sprite *pCardSprite, unsigned char cbCardData, unsigned uCardIdx);
//
//class CardFactory
//{
//public:
//	CardFactory(const std::string& strCardTexture = "publiccard.png");
//	cocos2d::Sprite *CreateCard(unsigned char uCardData = InvalidCard);
//
//private:
//	cocos2d::Rect CalcCardRect(unsigned char uCardData);
//
//	std::string m_strCardTexture;
//	float m_fCardWidth, m_fCardHeight;
//};



#endif