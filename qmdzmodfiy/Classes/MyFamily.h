//#ifndef _MYFAMILY_H_
//#define  _MYFAMILY_H_
//#include "cocos2d.h"
//#include "GameDialog.h"
//#include "UICommon.h"
//
//class HttpSprite;
//class LayerButton;
//class FrameSpriteButton;
//class MenuItemSpriteWithLabel;
//class GameHall;
//class TouchGrabber;
//class FrameSprite;
//class GameFamily;
//
//class ChooseFamilyIcon;
//class ContributionRankList ;
//class DataLoading ;
//struct GlobalMyFamilyInfo;
//class MyFamily : public GameDialog,public cocos2d::ui::EditBoxDelegate
//{
//
//public:
//	
//	static MyFamily* creatWithLayer(GameFamily* layer) ;
//	bool initWithLayer(GameFamily* layer) ;
//
//
//	CREATE_FUNC(MyFamily) ;
//	bool init() ;
//	void initLayer() ;
//	
//
//
//	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
//	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
//	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
//	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
//
//
//
//
//	//bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
//
//	void updateFamilyInfo( ) ;
//	void updateAdminList() ;
//	void updateTop5RankList() ;
//	void updateRankIconShow() ;
//	void updateRoleRankList() ;
//
//	void setVisible(bool bVisible) ;
//	void setOnVisible(bool bVisible) ;
//
//	void setGetReward(long time) ;
//
//
//	void onGrabTouchBegan(bool bHitDialog) ;
//
//	void cleanPower() ;
//
//
//	void updateNotice(std::string notice) ;
//	//void SetOnVieble(bool bVisible) ;
//private:
//	void initShow() ;
//	virtual void onMenuItemCloseSelected(cocos2d::Ref * ref);
//
//	void onButtonCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type) ;
//
//	void callAdminCallBack(cocos2d::Ref *pItem,cocos2d::ui::TouchEventType type) ;
//
//	void updateIcon() ;
//
//
//	void showEditAdminLayer(bool flag) ;
//
//	void setShowDataLoading(bool flag) ;
//public:
//	GameFamily* s_pLayer ;
//
//	DataLoading* s_dataLoading ;
//
//	ChooseFamilyIcon* s_ChooseIconLayer ;
//	ContributionRankList* s_ContributionLayer ;
//
//
//	cocos2d::Node* s_uiroot ;
//	cocos2d::ui::Layout* s_logicRoot ;
//
//
//	cocos2d::ui::Layout* s_nothingRoleTips ;
//	cocos2d::ui::Layout* s_RoleShow ;
//	cocos2d::ui::Layout* s_TimeLayer ;
//
//	cocos2d::ui::Text* s_familyName;
//	cocos2d::ui::Text* s_fangilyID ;
//
//	cocos2d::ui::Text* s_TextGetFuli ;
//
//	HttpSprite* s_FamilyIcon;
//	cocos2d::ui::Text* s_curLevel;
//	cocos2d::ui::Text* s_nextLevel;
//	cocos2d::ui::Text* s_curFanli;
//	cocos2d::ui::Text* s_netFanLi;
//	cocos2d::ui::Button* s_buttonGetFuli;
//	cocos2d::ui::Button* s_buttonTips;
//	cocos2d::ui::Button* s_button_exit;
//	cocos2d::ui::Button* s_button_GameFamily;
//	cocos2d::ui::Button* s_buttonInvite;
//	cocos2d::ui::Button* s_buttonEdit;
//	cocos2d::ui::Button* s_buttonEditGongGao;
//
//	cocos2d::ui::Button* buttonRank;
//
//	cocos2d::ui::Text* s_day;
//	cocos2d::ui::Text* s_hours;
//	cocos2d::ui::Text* s_hoursf;
//
//	cocos2d::ui::EditBox* s_editBoxGongGao ;
//
//	HttpSprite* s_spriteZuZhang;
//	HttpSprite* s_spriteFuZuZHang0;
//	HttpSprite* s_spriteFuZuZHang1;
//
//	cocos2d::ui::Text* s_familyGongGao;
//	cocos2d::ui::Text* s_curRole;
//	cocos2d::ui::Text* s_maxRole;
//
//	cocos2d::ui::Button* s_buttonIconEdit;
//
//	cocos2d::ui::LoadingBar* s_levelPercent ;
//	cocos2d::ui::Text* s_textLevelPercent;
//
//	cocos2d::Node* s_nodeFanLiTips ;
//	cocos2d::ui::Text* s_textFanliTips;
//
//
//
//	cocos2d::ui::Layout* m_layerTipsEditIcon ;
//
//	//管理人员头像
//	cocos2d::ui::Layout* m_LayerAdmin;
//
//	cocos2d::ui::Button* m_buttonAdmin[3];
//	cocos2d::ui::Button* m_buttonAdminBG[3];//头像框子
//	cocos2d::ui::Button* m_buttonAdminBGNull[3];//头像框子
//	HttpSprite* m_spriteAdmin[3];
//
//
//
//	//贡献排行五人
//	cocos2d::ui::ImageView* m_imgRankList[5];
//	HttpSprite* m_spriteRankList[5];
//		
//	std::string m_strNotice ;
//
//	cocos2d::Sprite* spriteStar[4] ;
//
//	cocos2d::Sprite* m_sprite_curLevel ;
//
//	cocos2d::Sprite* m_sprite_nextLevel ;
//	//std::vector<FamilyRoleBaseInfo> m_oldValue ;
//private:
//	int m_curAdminCount;//管理员数量
//
//
//};
//
//
//
//#endif
//
