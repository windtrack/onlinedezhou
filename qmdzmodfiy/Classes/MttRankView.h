
#ifndef _MttRankView_H_

#define _MttRankView_H_
#include "cocos2d.h"
#include "GameDialog.h"

struct MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class RankTexture;
class HttpSprite;
class MTTRankViewFirst;
class MttRankView :public GameDialog
{
public:

	CREATE_FUNC(MttRankView) ;
	bool init() ;

	void updateMttRankView( unsigned uRankId,std::string desc,std::string icon,std::string name,unsigned cash,unsigned score) ;
    void setVisible(bool Visible) ;
	
protected:
	void onMenuItemBottomSelect(cocos2d::Ref*pMenuItem);

	void GetReword();
	void SetAllSpriteVisible(bool bVisible);

	void onGrabTouchBegan(bool bHitDialog) ;

	void doLookUp() ;

private:
	unsigned m_uMaster;
	double m_dReword;
	cocos2d::Sprite          *m_pWinCup;//奖杯
	RankTexture              *m_pRankTexture;//排名
	cocos2d::Sprite          *m_pFailedExpress;//失败表情
	cocos2d::Label           *m_pLabelFailed;//失败说明
    cocos2d::Label           *m_pLabelWinDes;//获得名次的说明
	cocos2d::Label           *m_pLabelWin[3];//奖励标题
	cocos2d::Label           *m_pLabelReward[3];//实际奖励
	MenuItemScale9SpriteWithLabel  *m_pMenuItem[2];//返回大厅, 留下旁观, 再战一场
    MTTRankViewFirst         *m_pRankFirst;//第一名
	int m_curRank ;
	bool		    m_bShowItem;//是否显示物品
	bool		    m_bShowGold;//是否显示德州币 
	bool		    m_bShowScore;//是否显示大师分 
    bool            m_bIsWin;    //是否赢了

};
//第一名界面
class MTTRankViewFirst : public GameDialog
{
public:
    CREATE_FUNC(MTTRankViewFirst);
    bool init() ;
    void updateMTTRankViewFirst(unsigned uRankID, std::string strDes, std::string strIcon, std::string rewardName, unsigned cash, unsigned score);
    //void setVisible(bool Visible) ;
protected:
    void onMenuItemBottomSelect(cocos2d::Ref*pMenuItem);
private:
    MenuItemScale9SpriteWithLabel  *m_pMenuItem[3];//返回大厅, 告诉朋友,留下旁观
    cocos2d::Label                 *m_pLabelReward[3];//实物奖励，大师分，德州币
    cocos2d::Label                 *m_pLabelDes;//说明赢得比赛的具体情况
    HttpSprite                     *m_pSpriteReward;//实物奖励图片
    cocos2d::Sprite                *m_pSpritReward[3];
    cocos2d::Sprite                *m_pSpriteMan;
    cocos2d::Sprite                *m_pSpriteGirl;
    RankTexture                    *m_rankTexture;
};

#endif
