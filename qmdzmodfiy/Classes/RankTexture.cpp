#include "RankTexture.h"

USING_NS_CC;

bool RankTexture::init()
{
    if (!Layer::init())
    {
        return false;
    }

    ignoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2(0.5, 0.5));
    setContentSize(Size(0, 0));

    return true;
}

void RankTexture::Appand(std::string strImg)
{
    Sprite *sprite = Sprite::create(strImg);
    if (sprite)
    {

        auto spriteSz = sprite->getContentSize();
        auto frameSz = this->getContentSize();
        sprite->setPositionY(spriteSz.height/2);
        sprite->setPositionX(frameSz.width + spriteSz.width / 2);
        frameSz.width += spriteSz.width;
        this->setContentSize(frameSz);
        m_vecSprites.push_back(sprite);
        this->addChild(sprite);
    }
}

void RankTexture::Appand(unsigned uType, unsigned uRank)
{
    char buff[32] = {0};
    std::vector<unsigned> vecRank;
    while(uRank >= 10)
    {
        unsigned rank = uRank%10;
        vecRank.push_back(rank);
        uRank /= 10;
    }
    vecRank.push_back(uRank);
    std::vector<unsigned>::reverse_iterator iter;
    for (iter=vecRank.rbegin(); iter!=vecRank.rend(); iter++)
    {
        if (uType == 0)
        {
            sprintf(buff, "ranktexture/failed%d.png", *iter);
        }
        else if (uType == 1)
        {
            sprintf(buff, "ranktexture/winrank%d.png", *iter);
        }
        else if (uType == 2)
        {
            sprintf(buff, "ranktexture/share_n%d.png", *iter);
        }
        Appand(buff);
    }
}

void RankTexture::resetTexTure()
{
    this->removeAllChildren() ;
    this->setContentSize(Size(0,this->getContentSize().height));
}