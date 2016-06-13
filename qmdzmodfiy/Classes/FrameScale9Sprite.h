
#ifndef __FrameScale9Sprite_H__
#define __FrameScale9Sprite_H__

#include "cocos2d.h"
#include "UICommon.h"
// 请注意，texture2d是保存在texturecache中的，删除缓存，可能导致错误
class FrameScale9Sprite : public cocos2d::Node
{
public:
	//static FrameScale9Sprite *create(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx = 0);
	static FrameScale9Sprite* create(int index ,Size contentSize) ;

	//cocos2d::Sprite *GetSprite(unsigned uFrameIdx);
	//cocos2d::SpriteFrame *GetSpriteFrame(unsigned uFrameIdx);

	void SetFrameIdx(unsigned uFrameIdx);
	unsigned GetFrameIdx();

protected:
	//bool init(const std::string& strImgFile, unsigned uXFrames, unsigned uYFrames, unsigned uFrameIdx);
	bool init(int index ,Size contentSize) ;
	cocos2d::Rect CalcFrameRect(unsigned uFrameIdx);

private:
	unsigned m_uXFrames, m_uYFrames;
	unsigned m_uFrameIdx;
	cocos2d::Texture2D *m_pTexture2D;
	cocos2d::Size m_szImg;
	cocos2d::Size m_szFrame;

	std::vector<cocos2d::ui::Scale9Sprite*> vecPSprite;
};



#endif