#ifndef _SNGMATHCHSTATE_H_
#define  _SNGMATHCHSTATE_H_

#include "cocos2d.h"
#include "UICommon.h"
#include "GameDialog.h"
#include "HttpSprite.h"
#include "FrameSprite.h"

struct SngRoleInfo;
struct SngShengmang ;
class SngMatchState :public GameDialog
{
public:
	CREATE_FUNC(SngMatchState) ;
	bool init() ;
	void setVisible(bool bVisible) ;



	void updateData() ;
	void updateBlindData() ;
	//void updateMangzhuData() ;

private:


	void updateQIanzhu() ;//更新前注

	void showMatchStateLayer(int index) ;

	void onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type) ;

	void updateReward(unsigned rewordNum) ;

	void updateCurBlindLevel(int index) ;

	void updateTableMenuTips() ;
	
public:
    void updateSngMatchRankList();
    void appendRankItem(SngRoleInfo& roleInfo);
    void clearRank();


	void updateBlindList() ;
protected:

	struct SngRoleShow
	{
		FrameSprite		    *m_pSpriteBGItem;		// 滚动视图行的背景
		HttpSprite			*m_pSpriteIcon;			// 玩家的头像
		FrameSprite			*m_pSpriteBgRank;		// 玩家的背景排名
		cocos2d::Sprite		*m_pSpriteRewordImage;	// 玩家的奖金精灵图片
		cocos2d::Label		*m_pLabelRank;			// 玩家文字的排名
		cocos2d::Label		*m_pLabelName;			// 玩家的称呼
		cocos2d::Label		*m_pLabelWinPot;		// 玩家获得的金额数量
		
	};

    SngRoleShow *GetRankItem();
    void updateRankItem(SngMatchState::SngRoleShow *roleShow, SngRoleInfo& roleInfo);



	struct SngBlindShow
	{
		cocos2d::Node			*m_pNode ;
		cocos2d::Sprite		    *m_pSpriteBGItem;		// 滚动视图行的背景
		cocos2d::Sprite		    *m_pSpriteBGlight;		// 滚动视图行的背景

		cocos2d::Label		*m_pLabelRank;			
		cocos2d::Label		*m_pLabelBlind;		
		cocos2d::Label		*m_pLabelForntBlind;		
		cocos2d::Label		*m_pLabelTime;		


	};
	SngBlindShow *GetBlindItem();
	void updateBlindItem(SngMatchState::SngBlindShow *roleShow, SngShengmang& roleInfo);




private:
	cocos2d::Node* m_uiRoot ;

	cocos2d::ui::Layout* m_matchLayer[3] ;
	cocos2d::ui::Button* m_matchButton[3] ;
	cocos2d::ui::Text* m_text_button[3] ;

	cocos2d::ui::Text* m_textRank ;
	cocos2d::ui::Text* m_textTableName ;

	cocos2d::ui::ListView* listviewMangzhu ;

	cocos2d::ui::ImageView* m_img_mangzhuBg[16] ;

	//排行
	cocos2d::extension::ScrollView* m_pScrollView ;
	cocos2d::Size sizeListView;
    std::vector<SngRoleShow *>      m_vecShowItem;
    std::vector<SngRoleInfo>      m_vecRoleInfo;
	//盲注
    cocos2d::ui::Layout*    m_pBlindLayer[16];
    cocos2d::ui::Text*      m_pBlindText[16][4];

	cocos2d::extension::ScrollView* m_pScrollViewBlind ;
	cocos2d::Size sizeBlindListView;
	std::vector<SngBlindShow *>      m_vecShowBlindItem;
	std::vector<SngShengmang>      m_vecBlindInfo;


	//奖励
    cocos2d::ui::ImageView* m_pImageReward[3];
    cocos2d::ui::Text*      m_pTextReward[3][2];

};


#endif

