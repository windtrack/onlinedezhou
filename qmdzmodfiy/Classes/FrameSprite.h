#ifndef __FRAMESPRITE_H__
#define __FRAMESPRITE_H__

#include "cocos2d.h"

// 请注意，texture2d是保存在texturecache中的，删除缓存，可能导致错误
class FrameSprite : public cocos2d::Sprite
{
public:
	static FrameSprite *create(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx = 0);
	
	cocos2d::Sprite *GetSprite(unsigned uFrameIdx);
	cocos2d::SpriteFrame *GetSpriteFrame(unsigned uFrameIdx);

	void SetFrameIdx(unsigned uFrameIdx);
	unsigned GetFrameIdx();

protected:
	bool init(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx);

	cocos2d::Rect CalcFrameRect(unsigned uFrameIdx);

private:
	unsigned m_uXFrames, m_uYFrames;
	unsigned m_uFrameIdx;
	cocos2d::Texture2D *m_pTexture2D;
	cocos2d::Size m_szImg;
	cocos2d::Size m_szFrame;
};



#endif