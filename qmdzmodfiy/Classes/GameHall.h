#ifndef __GAMEHALL_H__
#define __GAMEHALL_H__

#include "cocos2d.h"
#include "CCScrollView.h"
#include "UICommon.h"
#include "MoveLabel.h"

class GameLayer;
class MenuItemSpriteWithLabel;
class TouchGrabber;
class HttpSprite;
class FrameSprite;
class GameSet;
class UserInfoHallView;
class TaskSet;
struct UserDetailInfo;
class GameFriendList;
class LayerButton;
struct QuickFollowInfo;
class RankList;
class GameActivity;
class MessageList;
class QuickFollowList;
class AchievementUi;
class PersonalInfomation;
class PrivilegesRewards;
class WaveGirlRewards;
class AcivateRewards;
class DailySign;
class ChooseFamilyIcon;
class EditFamilyRole ;
class ChangeViceShaikh ;
class InviteFriends ;
class MobileBind ;
class MobileCheck;
class DealApplyFamily ;
class FamilyInfoTips ;
class NewerTrain;
class UserItem ;
class DailySignReward;
class SngOnline ;
class ScrollingBanners ;
class AnimationManager;
class GameClubHall ;
class SelfClub ;
class AllotFund ;
class GameDialog;
class FriendsSpread;
struct UserSngInfo ; 
struct UserMTTInfo ;

struct FamilyRoleBaseInfo ;
struct MessageInfo;
class GameHall : public cocos2d::Layer
{
public:
	CREATE_FUNC(GameHall);

	//void UpdateMyInfo();
	//void UpdateFriendInfo(unsigned uUID, const std::string& strName, const std::string& strIcon, unsigned uStatus);
	//void UpdateZhuBoInfo(unsigned uUID, const std::string& strName, const std::string& strIcon, unsigned uStatus);
	void UpdateMissionInfo();
	void UpdateFollowList(unsigned uType);
	void UpdateRankList(unsigned uRankType, unsigned uWinType);
	void PopUserDetailInfo(unsigned uTID);
	void PopUserAchievementInfo(unsigned uTID);
	void PopPersonalInfo();


	void callPopPersonalInfo(Ref* pSender);
	void UpdateAchievementInfo();
	void UpdateUserInfo(const UserDetailInfo& userInfo);
	void UpdateUserSngInfo(const UserSngInfo& userInfo);
	void UpdateUserMttInfo(const UserMTTInfo& userInfo);
	//void updateUserSngInfo() ;


	void UpdateQuickFollowList(unsigned uType);
	void OperateQuickFollowList(unsigned uOpType, unsigned uType, const QuickFollowInfo& quickFollowInfo);

	void UpdateActivityInfo();

	void UpdateMessageInfo(unsigned uType);

	void SetEnterGameResult(bool bSuccess);

	void ShowAnimateEntering();
	virtual void setVisible(bool bVisible);
    
    void HideShowAll();

	void hideBtSkin() ;

    void ShowLayerBg(bool bVisible);

	//创建俱乐部 成功
	void cretFamilySuccess() ;
	//显示当前俱乐部
	void callUpdateTargetFamilyInfo() ;
	//显示俱乐部排行
	void callUpdateFamilyRankList();
	//显示我加入的俱乐部
	void callUpdateSelfJoinClubList();
	//更新我的俱乐部
	void callUpdateMyFamily() ;

	//更新族长数据
	void callUpdateAdminList() ;
	//更新Tos排行信息
	void callUpdateTop5RankList() ;
	//更新无权限成员列表
	void callUpdateNoPowerRoleList() ;
	//更新好友列表
	void callUpdateFriendsList() ;
	//提出玩家
	void callPushOutRole(unsigned uid) ;
	//更新排行列表可更新数据
	void callUpdateRanlistItem(const FamilyRoleBaseInfo& baseInfo) ;
	//领取奖励
	void callGetReward(long time) ;
	//更新消息列表已处理
	void callUpdateMessageFamily(const  MessageInfo& info) ;



	void showChooseFamilyIcon(bool flag) ;




	void showEditFamilyRole(bool flag,unsigned u_uid =0,std::string uName="",unsigned  controFlags=0) ;

	void showChangeShaikh(bool flag) ;
	void showInviteFriends(bool flag) ;
	void showMobileBind(bool flag) ;

	void showMobileCheck(bool flag,unsigned id) ;

	void showDealApplyJoinFamily(bool flag,unsigned fid ,unsigned uid, unsigned mid,std::string uname,std::string uicon) ;

	void showFamilyInfoTips(bool flag) ;

	void updateShowNewMessage(bool fag) ;

	void checkNewMessage() ;

	void callCleanPower() ;

	void callUpdateFamilyNotice(std::string sNotice) ;

	void removeInviteFriendsView(std::string uid) ;

	void callUpdateMobileCheckPhone(std::string  phone) ;

	void callHideMessageLoading() ;

	bool isMyFamilyVisible() ;
	 void PopDailySignReward(bool isCash);
	void showUserItem(bool flag) ;//显示我的道具

	void updateUserItem() ;//更新道具列表

	void doHideUserItemLayer() ;

	void showGameActivity() ;//显示活动界面


	void reSetScorlllTips() ;

	void callScrollTextMove() ;

	void showDailySign(bool flag) ;//显示签到界面

    void showBottomView(unsigned uType);

	void creatTipsLayer() ;

    unsigned getTaskStatus();

	void showGameClub(bool flag) ;
	void showSelfClub(bool flag,unsigned fid =0) ;
	void showAllotFund(bool flag,unsigned fid,unsigned uid,unsigned mid,std::string uname) ;

	void updateSelfClubRoleFundInfo(double clubFund,unsigned uid,double uFund) ;

	void updateSelfClubView() ;

    void popMoreCallBack(Ref *pSend);

    void popMoreBottomItems();//点击更多后的弹窗

    void popBindSpreadResult(bool bSuccess);

    void updateSpreadList();
protected:
	virtual bool init() override;

	void onMenuItemBottomSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type);
	void onMenuItemTouXiangSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type);

	void onMenuMatchItemSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type);

	void onGameTypeSelect(cocos2d::Ref*pMenuItem,cocos2d::ui::TouchEventType type) ;

	void OnUpdateMyData(unsigned uDataType);

	void showBtSkin() ;//显示皮肤按钮
	void setShowBtSkinBig() ;
	void setShowBtSkillSmall() ;

	void creatSngOnlineTips() ;
	void creatActivityLayer() ;
	void creatSiginLayer() ;
	void creatTeachLayer() ;
	void creatGetVipRewardLayer() ;
    void createDelaerTips();
	void creatWaveGirlReward() ;

    //创建活动
    void createChristmas();
    void createSpringFestival();

	void showMathcLayer(bool flag) ;

private:
	float m_fXingXiangHeight;
	float m_fXingXiangScaleLowHeight;
	float m_fXingXiangScaleTopHeight;
	GameLayer *m_pHallTouchLayer;
	cocos2d::Sprite *m_newMessageTips ;


	HttpSprite *m_pHttpSpriteBG;
	QuickFollowList *m_pQuickFriend;

	cocos2d::ui::ImageView* imgScrollList;

	HttpSprite *m_pHttpSpriteXingXiang;//头像
	LayerButton *m_pBtXingXiang;//形象
	cocos2d::Layer *m_pLayerHitXingXiang;

	cocos2d::ui::Layout *m_pSpriteTable;// 桌子
	//cocos2d::Sprite *m_pSpriteTable;// 桌子
	cocos2d::ui::ImageView* m_pBtSkin ;//换装按钮


	cocos2d::ui::Button *m_pMenuItemBottom[8];
	//MenuItemSpriteWithLabel *m_pMenuItemBottom[8];
    MenuItemSpriteWithLabel *m_pMenuMoreItem[2];//大厅底部的更多弹窗里面的两个按钮
	cocos2d::ui::Button *m_pSpriteEnterGame[4];//对应的按钮
	cocos2d::Sprite *m_pSpriteAniEnterGame[4];//按钮对应的动画
	cocos2d::Action *m_pAniEnterGame[4];//对应的动作
	AnimationManager *m_aniPrivateRoom ;

	//TouchGrabber *m_pTouchGrabberEnterGame[4];//对应的触摸区域


	//cocos2d::Sprite *m_pSpriteEnterGame[4];//对应的按钮
	//cocos2d::Sprite *m_pSpriteAniEnterGame[4];//按钮对应的动画
	//cocos2d::Action *m_pAniEnterGame[4];//对应的动作
	//TouchGrabber *m_pTouchGrabberEnterGame[4];//对应的触摸区域
    ImageView* img_roleBody;;
	cocos2d::ui::Text *m_pLabelName;
	cocos2d::ui::Text *m_pLabelCash;   //当前的总金币
    cocos2d::ui::Text *m_pLabelDiamond;//当前的总钻数
	cocos2d::ui::ImageView *m_pSprite$;
    cocos2d::ui::ImageView *m_pSpriteDiamond;
    cocos2d::ui::ImageView *m_pImageCashBg;     //当前金币背景
    cocos2d::ui::ImageView *m_pImageDiamondBg;  //当前钻石背景

    cocos2d::ui::Layout   *m_pLayoutTouXiang;
	cocos2d::ClippingNode *m_pClipTouXiang;
	FrameSprite *m_pVipSign;
	cocos2d::ui::ImageView* imgHead ;
	HttpSprite *m_pSpriteTouXiang;
	cocos2d::ui::Button *m_pMenuItemHitTouXiang;
	//cocos2d::MenuItemImage *m_pMenuItemHitTouXiang;
	//cocos2d::ui::ImageView *m_pSpriteIconRound;
	//cocos2d::ui::ImageView *m_pSpriteIconRound;
    LayerColor  *m_layerColorBg;
	GameSet *m_pGameSet;
	UserInfoHallView *m_pUserInfo;
	TaskSet *m_pTaskSet;
	GameFriendList *m_pFriendList;
	RankList *m_pRankList;
	GameActivity *m_pGameActivity;
	MessageList *m_pMessageList;
	AchievementUi *m_pAchievementDlg;
	PersonalInfomation *m_pPersonalInfomation;
	PrivilegesRewards  *m_pPrivilegesRewards;
	NewerTrain         *m_pNewerTrain;
	WaveGirlRewards *m_pWaveGrilSign;
	DailySign *m_pDailySign;
    FriendsSpread *m_pFriendSpread;


	GameClubHall  *m_GameClubHall ;
	SelfClub	  *m_selfClub ;

	 ChooseFamilyIcon *m_chooseFamilyIcon;
	 EditFamilyRole* m_editFamilyRole ;
	 ChangeViceShaikh* m_changeViceShaikh ;
	 InviteFriends* m_inviteFriends ;

	 DealApplyFamily* m_dealApplyFamily ;
	 //FamilyInfoTips* m_familyInfoTips ;

	 MobileBind *m_pMobileBind;
	 MobileCheck *m_mobileCheck ;
     DailySignReward *m_DailySignReward;

	 UserItem *m_userItem ;
	 AllotFund								*m_allotFund ;
	 //提示相关
	 cocos2d::Sprite* m_spriteNewTips ;//新功能的相关提示
	 cocos2d::ui::ImageView* m_spriteSngOnline ;
	 SngOnline* m_tipsSngOnline ;

     cocos2d::Sprite    *m_pSpriteTaskFlag;//任务提示

	 //滚动提示
	 bool m_isScrollTextMove ;
	 //cocos2d::LayerColor* m_moveTipsLayer ;
	 ScrollingBanners* m_moveTipsLayer ;
	 int m_moveLableCount ;//当前显示中的跑马灯数量
     bool m_bPerfectEnter; //是否为从加载到登陆一级大厅

     //圣诞活动雪景
     //AnimationManager           *m_aniChristmasTree;
     AnimationManager           *m_aniChristmasTree;
     cocos2d::ui::ImageView     *m_christmasShow[5];// 好友列表， 桌子左， 桌子右， 任务头像， 场次按钮
	 cocos2d::ParticleSnow		*m_snow ;
     //新春活动
     AnimationManager           *m_aniSpringFestival[2];



	 //赛市场
	 cocos2d::ui::Layout		*m_layout_match;
	 cocos2d::ui::Layout		*m_layout_matchColorBG;
	 cocos2d::ui::Button		*m_button_sngMatch;
	 cocos2d::ui::Button		*m_button_mttMatch;
	 cocos2d::ui::Button		*m_button_matchTouch;

	 AnimationManager			*m_aniMttMatch ;
	 AnimationManager			*m_aniSngMatch ;

     GameDialog                 *m_popMoreItems;//推广小弹窗

	
};


#endif