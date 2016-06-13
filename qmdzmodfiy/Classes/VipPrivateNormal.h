#ifndef  __VIPPRIVATENORMAL_H__
#define  __VIPPRIVATENORMAL_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"
#include "UICommon.h"

class PrivateRoomRule ;
class SelfDefinition;
class SelfFund ;

class LayerButton ;


struct DefineRoomInfo;

class VipPrivateNormal : public GameDialog,public cocos2d::extension::ScrollViewDelegate,public cocos2d::ui::EditBoxDelegate
{
public:
	void updateRoomView(unsigned type) ;
	void showRoomView(bool showAction) ;
	CREATE_FUNC(VipPrivateNormal) ;

	void updateJoinRommBtShow(bool flag) ;

	void updateCurPrivateRoomInfo(DefineRoomInfo pDefineOpenRoomInfo) ;

	void callUpdateRoomName(std::string  name) ;

	void updateFundView(int index) ;
protected:
	virtual bool init();
	virtual void onSetVisible(bool bVisible) override;
	virtual void onKeyEscPressed() override;
	void OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type);
    void UpdateCash(unsigned cashflag);


	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	virtual void editBoxEditingDidBegin(EditBox* editBox) ;

	void callAction() ;//入场动画

	void doJoinRoom(const DefineRoomInfo &data) ;//根据id进入房间

	void setJoinRoomButtonEnable(bool flag) ; //进入房间按钮状态控制

	void creatDemoRoom(); //生成一个测试房间

	void showCreatRoomLayer(bool flag) ;
	/*void showSelfFundLayer(bool flag) ;*/
	void showPrivateRoomRule(bool flag) ;

	void doRoomTypeSelect(unsigned index) ;

	struct DefineRoomItem
	{
		cocos2d::Sprite                         *m_spriteTableBg; //背景按钮
		LayerButton								*m_pBtDetail ;
		cocos2d::Sprite                         *m_spriteHuman; //玩家
		cocos2d::Sprite                         *m_spriteEye; //旁观
		cocos2d::Sprite                         *m_spriteAnti; //必下
		cocos2d::Sprite                         *m_spriteState;//锁
		cocos2d::Sprite                         *m_spriteRoomidBG;//房间id背景
		cocos2d::Sprite                         *m_spriteRoomMine;//我的房间

		cocos2d::Label                          *m_labelBlind; //大小盲
		cocos2d::Label                          *m_labelPlayersNum;//玩家人数
		cocos2d::Label                          *m_labelLookOnNum;//旁观人数
		cocos2d::Label                          *m_labelRoomscreat;//房间名称
		cocos2d::Label                          *m_labelRoomOwnerName;//房间名称

		cocos2d::Label                          *m_labelRoomstate;//房间状态
		unsigned								m_RoomID;
		bool									m_ismine;

		bool                                     m_bIsBuild;
		unsigned								m_state; //0 未开始  1进行中
	};
	DefineRoomItem* GetDefineRoomItem(unsigned uDeskId);
	void UpdateDefineRoomItem(VipPrivateNormal::DefineRoomItem* item, const DefineRoomInfo &data);

private:
	SelfDefinition							*m_selfDefinition ;
	SelfFund								*m_selfFund ;
	PrivateRoomRule							*m_privateRoomRule ;
private:
	std::vector<DefineRoomInfo> m_DefineRoom[2];
	cocos2d::ui::Layout						*m_pLayoutShow;//背景

	cocos2d::ui::Layout						*m_pSpriteBgTable;//背景
	cocos2d::ui::Layout						*m_pLayoutTop;//上层
	cocos2d::Sprite							*m_pVipTimeSprite;//时间图片

	cocos2d::ui::Button						*m_pBtBack; //返回
	cocos2d::ui::Button						*m_pBtEnterMall;//商城
	cocos2d::ui::Button						*m_pBtEnterClient;//客服
	cocos2d::ui::Button						*m_pBtHelp;//帮助
	cocos2d::ui::Button						*m_pBtSelfFund;//我的基金
	cocos2d::ui::Button						*m_pBtClubFund;//我的基金
	cocos2d::ui::Button						*m_btnCreatRoom;//我要开房
	cocos2d::ui::Button						*m_pBtJoinRoom;//进入房间

	cocos2d::ui::Button						*m_btPerosonalRoom;//个人房
	cocos2d::ui::Button						*m_btClubRoomRoom;//俱乐部房
	
	cocos2d::ui::Text						*m_textPersonal ;	
	cocos2d::ui::Text						*m_textClub ;	

	cocos2d::ui::Text						*m_textCach ;	
	cocos2d::ui::ImageView					*m_imageLineUp ;
	
	cocos2d::extension::ScrollView          *m_scrollView;


	cocos2d::ui::ImageView					*m_imgeditbox;

	float                                   m_dOffSet;
	std::vector<DefineRoomItem*>            m_vecDefineRoomNew;
	std::vector<DefineRoomItem*>            m_vecDefineRoomOld;

	cocos2d::Size							m_sizeView ;


	std::vector<DefineRoomInfo> m_showBySerch;

	//搜索相关
	cocos2d::ui::Text						*m_textSerach ;	
	cocos2d::ui::Text						*m_textJoinRoom ;	
	//cocos2d::ui::Button						*m_pBtReset;
	//cocos2d::ui::Button						*m_pBtRefsh;
	//cocos2d::ui::ImageView					*m_pBtSerch;
	cocos2d::ui::EditBox					*m_EditSerch;


	cocos2d::ui::Text						*m_textNulltips ;	
	cocos2d::ui::Text						*m_textNulltipsClub ;	

	unsigned								m_curTypeIndex ;
};
#endif //__VIPPRIVATENORMAL_H__