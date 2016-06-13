#ifndef __USERINFO_H__
#define __USERINFO_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "CCScrollView.h"
#include "GameDialog.h"
class LayerButton;
class TextureNumber;
class MobileBind;
class AnimationManager;
class DataLoading ;

struct UserDetailInfo
{
	// 基本信息
	unsigned m_uUID;
	std::string m_strName;
	std::string m_strIcon;
	bool m_bBoy; // girl
	double m_dCash;
    double m_dDiamond;

	int m_iVipDengJi;
	int m_iMeiLiZhi;

	unsigned m_uJingYanDengJi;
	unsigned m_uJingYanZongZhi;
	unsigned m_uJingYanJinDu;
	
	// 房间里查看视图信息
	unsigned m_uPaiJu;
	double m_fRuJuLv;
	double m_fShengLv;
	double m_fTanPaiLv;

	std::string m_strBeiZhu;
	int m_iBiaoJi;
	std::string m_strBiaoJi;

	bool m_bYiGuanZhu;
	bool m_bYiPingBi;

	double m_fJiJinDu;
	double m_fZaiJiaZhu;
	double m_fFanQianXiaZhu;
	double m_fChiXuXiaZhu;

	// 主播信息
	bool m_bZhuBo;
	unsigned m_uZhuBoFangJian;
	std::string m_strShengRi;
	unsigned m_uShenGao;
	unsigned m_uTiZhong;
	std::string m_strYiShuZhao[4];

	// 常规场盈利排名信息
	double m_fZhouYinLi;
	double m_fRiDanJu;
	unsigned m_uZongJuShu;
	unsigned m_uZongShengLv;
	int m_iYinLiPaiMing;
	int m_iDanJuPaiMing;
	unsigned char m_cbMaxCardData[5];
	
	// SNG盈利排名信息
	//unsigned m_uSNGBiSaiCiShu;
	//unsigned m_uSNGHuoJiangCiShu;
	//unsigned m_uSNGZhouDaShiFen;
	//int m_iSNGDaShiPaiMing;
	//unsigned m_uSNGJiangBei[3];

	// MTT盈利排名信息
	//unsigned m_uMTTBiSaiCiShu;
	//unsigned m_uMTTHuoJiangCiShu;
	//unsigned m_uMTTZhouDaShiFen;
	//int m_iMTTDaShiPaiMing;
	//int m_iZuiHaoChengJi;

	// 成就
	unsigned m_uChengJiu[4];
	unsigned m_uChengJiuStep[2];

	//俱乐部
	std::string m_familyName;
	unsigned m_familyID ;
	unsigned m_familyLV;
	bool m_hasJoinFamily ;
};


struct UserSngInfo
{
	unsigned m_uSNGBiSaiCiShu;
	unsigned m_uSNGHuoJiangCiShu;
	unsigned m_uSNGZhouDaShiFen;
	int m_iSNGDaShiPaiMing;
	unsigned m_uSNGJiangBei[3];
};

struct UserMTTInfo
{
	unsigned m_uMTTBiSaiCiShu;
	unsigned m_uMTTHuoJiangCiShu;
	unsigned m_uMTTZhouDaShiFen;
	int m_iMTTDaShiPaiMing;
	int m_iZuiHaoChengJi;
};

class HttpSprite;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class FrameSprite;
class MenuItemFrameSprite;
class ChatBox;

class UserInfoView : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(UserInfoView);

	void UpdateUserInfo(const UserDetailInfo& usrInfo, bool bLookOn, bool bMySelf,bool bOwner=false);
	

	void SetToolPrice(double dToolPrice);
	// void AppandExpress(unsigned uType, unsigned uIdx);

	void SetGuanZhuResult(unsigned uTUID, int iGuanZhuType, int iRet);
	void SetPingBiResult(unsigned uTUID, int iPingBiType, int iRet);
	void SetBiaoJiOrBeiZhuResult(unsigned uTUID, const std::string& strBeiZhu, int iBiaoJi, const std::string& strBiaoJi);

	virtual void onSetVisible(bool bVisible) override;

	void onGrabTouchBegan(bool bHitDialog) ;

	void setMoveUp(bool flag);
	inline bool getMoveUp() {return m_hasMoveUp ;} ;
protected:
	void initData() ;

	void setShowDataLoading(bool flag) ;
	// Layer
	virtual bool init() override;	
	// EditBoxDelegate
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;



	void onMenuItemSelected(cocos2d::Ref *pItem);
	void onMenuItemBiaoJiSelected(cocos2d::Ref *pItem);
	void onMenuCallBackPaijuFamily(cocos2d::Ref *pItem);
	void ShowTool(bool bShow);
	void ShowExpress(bool bShow);

private:
	bool m_hasMoveUp ;
	DataLoading* s_dataLoading ;
	unsigned m_uFamilyID;
	unsigned m_uUserFamilyID;
	UserDetailInfo m_UsrInfo;
	//std::deque<unsigned> m_deqExpress;
	std::vector<FrameSprite*> m_vecQuickExpress;
	std::vector<LayerButton*> m_vecBtQuickExpress;
	/*std::vector<TouchGrabber*> m_vecExpressTouchGrabber;
	std::vector<TouchGrabber*> m_vecToolTouchGrabber;*/

	HttpSprite *m_pIcon;
	FrameSprite *m_pSpriteSex;
	cocos2d::Label *m_pLabelName;
	FrameSprite *m_pSpriteVIPDengJi;

	cocos2d::Label *m_pLabelBeiZhu;
	FrameSprite *m_pSpriteBeiZhu;
	cocos2d::ui::EditBox *m_pEdtBeiZhu;
	cocos2d::Sprite *spriteLine ;
	cocos2d::Sprite *spriteLineSmall ;

	cocos2d::Label *m_pLabelJingYanDengJi1;
	cocos2d::ProgressTimer *m_pProgressJingYanDengJi;
	cocos2d::Label *m_pLabelJingYanDengJi2;

	cocos2d::Label *m_pLabelBiaoJi;
	MenuItemSpriteWithLabel *m_pMenuItemBiaoJi;
	FrameSprite *m_pSpriteBiaoJi;

    cocos2d::Sprite *m_pSpriteDiamond;
	cocos2d::Label *m_pLabelLeBi;
    cocos2d::Label *m_pLabelDiamond;
	cocos2d::Label *m_pLabelMeiLi;
	cocos2d::Label *m_pLabelUID;
	cocos2d::Label *m_pLabelPaiJu;
	cocos2d::Label *m_pLabelRuJuLv;
	cocos2d::Label *m_pLabelShengLv;
	cocos2d::Label *m_pLabelTanPaiLv;
	
	cocos2d::Label *m_pLabelGetVIP;
	cocos2d::Label *m_pLabelVIP[4];
	MenuItemSpriteWithLabel *m_pMenuItemGetVIP;
	cocos2d::Label *m_pLabelJiJinDu;
	cocos2d::Label *m_pLabelFanQianJiaZhu;
	cocos2d::Label *m_pLabelZaiJiaZhu;
	cocos2d::Label *m_pLabelChiXuXiaZhu;

	cocos2d::Label *m_pLabelToolPrice;
	cocos2d::Label *m_pLabelQuickExpress;
	cocos2d::Label *m_pLabelLookOnTip;

	cocos2d::extension::ScrollView *m_pScrollViewTool;
	cocos2d::extension::ScrollView *m_pScrollViewQuickExpress;

	MenuItemScale9SpriteWithLabel *m_pMenuItemAt;
	MenuItemScale9SpriteWithLabel *m_pMenuItemGuanZhu;
	MenuItemScale9SpriteWithLabel *m_pMenuItemQuXiaoGuanZhu;
	MenuItemScale9SpriteWithLabel *m_pMenuItemPingBi;
	MenuItemScale9SpriteWithLabel *m_pMenuItemQuXiaoPingBi;
	MenuItemScale9SpriteWithLabel *m_pMenuItemZhuBoDangAn;

	MenuItemScale9SpriteWithLabel *m_pMenuItemPushPlayer;
	MenuItemSpriteWithLabel *m_pMenuItemFamilyButton[4];
	cocos2d::Label *m_pMenuItemFamilyLabel[4];

	// cocos2d::Sprite *m_pSpriteBgDialogPingBi;
	GameDialog *m_pDlgPingBi;
	cocos2d::Label *m_pLabelPingBiConfirm;
	cocos2d::Label *m_pLabelPingBiDescribe;
	MenuItemScale9SpriteWithLabel *m_MenuItemPingBiOk;
	MenuItemScale9SpriteWithLabel *m_MenuItemPingBiCancel;
	cocos2d::Menu *m_pMenuDialogPingBi;
	//TouchGrabber *m_pTouchGrabberDialogPingBi;

	// cocos2d::Sprite *m_pSpriteBgDialogPingBiShiBai;
	GameDialog *m_pDlgPingBiShiBai;
	cocos2d::Label *m_pLabelPingBiShiBaiConfirm;
	cocos2d::Label *m_pLabelPingBiShiBaiDescribe;
	MenuItemSpriteWithLabel *m_MenuItemKaiTongVIP1;
	cocos2d::Menu *m_pMenuDialogPingBiShiBai;
	//TouchGrabber *m_pTouchGrabberDialogPingBiShiBai;

	// cocos2d::Sprite *m_pSpriteBgDialogKaiTongVIP;
	GameDialog *m_pDlgKaiTongVIP;
	cocos2d::Label *m_pLabelKaiTongVIPDescribe;
	MenuItemSpriteWithLabel *m_MenuItemKaiTongVIP2;
	cocos2d::Menu *m_pMenuDialogKaiTongVIP;
	//TouchGrabber *m_pTouchGrabberDialogKaiTongVIP;

	// cocos2d::Sprite *m_pSpriteBgDialogBiaoJi;
	GameLayer *m_pDlgBiaoJi;
	//TouchGrabber *m_pTouchGrabberDialogBiaoJi;
	cocos2d::ui::EditBox *m_pEdtBiaoJi;
	MenuItemScale9SpriteWithLabel *m_MenuItemBiaoJiOk;
	MenuItemScale9SpriteWithLabel *m_MenuItemBiaoJiCancel;
	cocos2d::Menu *m_pMenuBiaoJi;
	FrameSprite *m_pSpriteBiaoJiItem[8];
	cocos2d::Label *m_pLabelBiaoJiTuPianMiaoShu[8];
	MenuItemSpriteWithLabel *m_MenuItemBiaoJiItem[8];

	GameDialog *m_pSpriteBgTouSu;
};

class UserInfoHallView : public GameDialog
{
public:
	static UserInfoHallView *create();
	void UpdateUserInfo(const UserDetailInfo& usrInfo, bool bMySelf);
	void UpdateUserSngInfo(const UserSngInfo& usrInfo) ;
	void UpdateUserMTTInfo(const UserMTTInfo& usrInfo) ;
	void Preload(bool bMySelf);
    virtual void setVisible(bool bVisible) override;




protected:
	void initUserInfoHallViewData(bool bVisible) ;

	virtual bool init(const std::string& strBg);

	void onMenuItemSelected(cocos2d::Ref* pItem);
	void onMenuCallBackFamily(cocos2d::Ref* pItem);
	void PopMobileBind();

	void OnMyDataChanged(unsigned uDataType);
	void setShowDataLoading(bool flag) ;
protected:

	
	virtual void onSetVisible(bool bVisible) override;
	DataLoading* s_dataLoading ;
	unsigned m_uTUID;
	unsigned m_uFamilyID;

	unsigned m_uid ;
	unsigned m_fid ;
	bool m_hasJoinFamily ;
	// 基本信息
	cocos2d::ClippingNode *m_pClipTouXiang;
	HttpSprite *m_pSpriteTouXiang;
	// cocos2d::Sprite *m_pSpriteBianJiTouXiang;
	MenuItemSpriteWithLabel *m_pMenuItemBianJiTouXiang;
	MenuItemFrameSprite *m_pMenuItemBindPhone;
	FrameSprite *m_pSpriteXingBie;
	cocos2d::Label *m_pLabelNiChen;
	FrameSprite *m_pSpriteVIP;

	cocos2d::Label *m_pLabelID;
	MenuItemSpriteWithLabel *m_pMenuItemKaiQiVIP;
	MenuItemSpriteWithLabel *m_pMenuItemJoinFamily;
	cocos2d::Label *m_pLabelMeiLi;
	cocos2d::Label *m_pLabelLeBi;
    cocos2d::Label *m_pLabelDiamond;
    cocos2d::Sprite *m_pSpriteDiamond;
	cocos2d::Label *m_pLabelLV;
	cocos2d::Label *m_pLabelLV2;
	cocos2d::ProgressTimer *m_pProgressLV;

	MenuItemSpriteWithLabel *m_pMenuItemSwitch[3];
	FrameSprite *m_pMenuItemFamilySprite;
	MenuItemSpriteWithLabel *m_pMenuItemJoinButton[4];
	cocos2d::Label *m_pMenuItemFamilyLabel[4];
	cocos2d::Layer *m_pLayerBg[3];

	// 常规
	cocos2d::Label *m_pLabelZhouYingLi;
	cocos2d::Label *m_pLabelRiDanJu;
	cocos2d::Label *m_pLabelZongJuShu;
	cocos2d::Label *m_pLabelZongShengLv;
	cocos2d::Label *m_pLabelYingLiPaiMing;
	cocos2d::Label *m_pLabelDanJuPaiMing;
	FrameSprite *m_pSpriteZuiDaPaiXing[5];

	// SNG
	cocos2d::Label *m_pLabelSNGCanSaiCiShu;
	cocos2d::Label *m_pLabelSNGHuoJiangCiShu;
	cocos2d::Label *m_pLabelSNGZhouDaShiFen;
	cocos2d::Label *m_pLabelSNGDaShiPaiMing;
	TextureNumber *m_pNumSNGJiangBei[3];

	// MTT
	cocos2d::Label *m_pLabelMTTCanSaiCiShu;
	cocos2d::Label *m_pLabelMTTJiangLiCiShu;
	cocos2d::Label *m_pLabelMTTZhouDaShiFen;
	cocos2d::Label *m_pLabelMTTDaShiPaiMing;
	cocos2d::Label *m_pLabelMTTZuiHaoChengJi;
	cocos2d::Label *m_pLabelMTTBest;
	// 成就
	FrameSprite *m_pSpriteChengJiu[4];
	cocos2d::Label *m_pLabelChengJiu;
	MenuItemSpriteWithLabel *m_pMenuItemChengJiu;
	MenuItemScale9SpriteWithLabel *m_pMenuItemPaiJuJiLu;
	MenuItemScale9SpriteWithLabel *m_pMenuItemWoDeDaoJu;

private:
	MobileBind *m_pMobileBind;
    bool m_bAlreadyOpen;
};


#endif