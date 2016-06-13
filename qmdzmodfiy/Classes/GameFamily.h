//#ifndef _GAMEFAMILY_H_
//
//#define _GAMEFAMILY_H_
//
//#include "cocos2d.h"
//#include "GameDialog.h"
//#include "UICommon.h"
//
//class HttpSprite;
//class LayerButton;
//class FrameSpriteButton;
//class FrameScale9SpriteButton;
//class MenuItemSpriteWithLabel;
//class GameHall;
//class TouchGrabber;
//class FrameSprite;
//
//class MyFamily;
//class ShowFamily;
//
//class DataLoading ;
//struct FamilyInfo;
//struct GlobalMyFamilyInfo;
//
//class GameFamily : public GameDialog, public cocos2d::ui::EditBoxDelegate
//{
//public:
//	CREATE_FUNC(GameFamily) ;
//	bool init() ;
//	
//	void setVisible(bool bVisible);
//	void setOnVisible(bool bVisible);
//	void initFamilyShow() ;
//	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
//	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
//	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
//	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
//
//	void showMyFamily(bool flag);
//
//	void UpdateView();
//	void UpdateItemView( const FamilyInfo& userInfo);
//	void AppadnItemView( const FamilyInfo& userInfo);
//	void RemoveItemView( unsigned uUserID);
//
//	//void showTargetFamily(bool flag);
//
//	void updateRankList() ;
//	
//	void updateMyFamilyData() ;
//
//	void updateTargetFamilyData() ;
//
//	void updateMyFamilyRoleRankList() ;
//
//	void setCloseButtonShow(bool flag) ;
//private:
//
//
//	 void callScaleBig(cocos2d::Node* node) ;
//	 void callScaleSmall(cocos2d::Node* node) ;
//
//	void onFamilyMenuItemClick(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type);
//
//
//	//初始化俱乐部按钮文字与状态
//	void showFamilyLayer(int index) ;
//	//初始化俱乐部介绍中文字与状态
//	void showFamilyInfoLayer(int index) ;
//
//
//	virtual void onMenuItemCloseSelected(cocos2d::Ref * ref);
//
//	void onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type) ;
//
//
//	//防穿透
//	void menuClickLeft(Ref* pSender);
//	void menuClickRight(Ref* pSender);
//	//加入俱乐部
//	void menuClickJiaRu(unsigned ufamilyID);
//
//	//显示俱乐部信息
//	void menuClickBgItem(unsigned m_familyID);
//
//	//创建俱乐部
//	void menuClickCeatFamily(std::string name);
//
//	bool checkTextChineseOrNumberOrLetter(std::string str) ;
//
//	char* QXUtf82Unicode(const char* utf, size_t *unicode_number)  ;
//
//	void updateCreatFamilyLayer() ;
//
//	void updateBuyVip(unsigned uDataType) ;
//
//	void setShowDataLoading(bool flag) ;
//public:
//
//
//	struct FamilywListItem
//	{
//		FrameSprite *m_pSpriteBgItem;			    // 滚动视图行的背景	
//		LayerButton *m_pBtDetail;
//		HttpSprite *m_pSpriteFamilyTouXiang;	
//		cocos2d::Label* m_familyName;
//		cocos2d::Sprite *m_pSpriteStar;
//		cocos2d::Label* m_labellevel;
//
//		unsigned m_level; //id	
//		unsigned m_curRoleCount;
//		unsigned m_maxRoleCount;
//		cocos2d::Label* m_labelRoleCount;
//		cocos2d::Label* m_roleCount;
//		FrameScale9SpriteButton *m_pBtJiaRu;					//加入按钮
//		unsigned m_familyID; //id
//		cocos2d::Label* m_labelRankIndex;
//		unsigned m_familyRankID;//排名
//		FrameSprite* m_spriteRank;
//	};
//
//	//获取列表显示
//	FamilywListItem *GetFamilyListItem(unsigned m_familyID);
//	void UpdateItem(GameFamily::FamilywListItem *item, const FamilyInfo& followInfo);
//
//
//private:
//	DataLoading* s_dataLoading ;
//	//MyFamily* m_myFamily;
//	//ShowFamily* m_targetFamily;
//
//	//俱乐部试图相关
//	cocos2d::extension::ScrollView			*m_pScrollView;			
//	std::vector<FamilywListItem*>		m_vecFollowListInfoOld;		//原始数据
//	std::vector<FamilywListItem*>		m_vecFollowListInfoNew;
//	cocos2d::Size sizeListView;
//
//	TouchGrabber *m_pTouchGrabberBG;
//
//
//	cocos2d::Node* s_uiroot ;
//
//	cocos2d::ui::Layout* s_familybackgroundLayer ;//俱乐部公共底
//	cocos2d::ui::Layout* s_rankLayer ; //俱乐部排行
//	cocos2d::ui::Layout* s_myFamilyLayer ;//我的俱乐部
//	cocos2d::ui::Layout* s_myFamilyInof ;//俱乐部介绍
//	cocos2d::ui::Layout* s_creatFamilyLayer ;//创建俱乐部
//
//
//
//	//创建俱乐部界面判断
//	cocos2d::ui::Layout* s_cloudCreat ;
//	cocos2d::ui::Layout* s_vipLow ;
//	cocos2d::ui::Layout* s_hasFamily ;
//
//	cocos2d::ui::Button* s_buttonShop ;
//	cocos2d::ui::Button* s_buttonExit ;
//
//	//主按钮
//	cocos2d::ui::Button* s_button_myFamilyMenu[4]; 
//	cocos2d::ui::Text*   s_button_myFamilyMenuText[4]; 
//
//
//
//	cocos2d::ui::EditBox* s_editBoxCreatFamliy;//创建俱乐部输入
//	cocos2d::ui::Text*   s_editBoxFamilyName; 
//
//	cocos2d::ui::Layout* s_familyManager[4];
//
//
//	//我的俱乐部
//	cocos2d::ui::Button* buttomMyFamilyCreatFamily;
//	cocos2d::ui::Button* buttomMyFamilyJoinFamily;
//
//	cocos2d::ui::EditBox* s_editBoxSearch;//俱乐部搜索
//	//创建俱乐部
//	cocos2d::ui::Button* buttomCreatFamily;
//
//
//
//	std::string s_familyName ;
//
//	GameLayer                               *m_layerTop;
//	GameLayer                               *m_layerBottom;
//
//
//
//	cocos2d::Sprite* spriteStar[4] ;
//
//	int m_curLayerIndex ;
//
//	cocos2d::Sprite* spriteFamilyTips ;
//};
//
//
//
//#endif
