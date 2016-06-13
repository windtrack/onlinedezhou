#ifndef __RANKTEXTURE_H__
#define __RANKTEXTURE_H__
#include "cocos2d.h"

class RankTexture :public cocos2d::Layer
{
public:
    CREATE_FUNC(RankTexture);
    void Appand(std::string strImg);//增加拼接图片
    void Appand(unsigned uType, unsigned uRank);//根据类型使用uRank循环添加拼接图片
    void resetTexTure();
protected:
    virtual bool init();
private:
    std::vector<cocos2d::Sprite*> m_vecSprites;
};
#endif