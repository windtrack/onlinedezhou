#ifndef __MESSAGELIST_H__
#define __MESSAGELIST_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"
#include "BaseButton.h"
#include "UICommon.h"

class DataLoading ;

class FrameSprite;
class MenuItemSpriteWithLabel;
class HttpSprite;
class FrameSpriteButton;
class FrameScale9SpriteButton;
class MenuItemFrameSprite;
class LayerButton;
class LineLabel ;

struct MessageInfo;

class MessageList : public GameDialog,public cocos2d::extension::ScrollViewDelegate
{
public:
	CREATE_FUNC(MessageList);
	void UpdateView(unsigned uMessageType); // 0邮件消息 1系统消息 2俱乐部消息
    virtual void setVisible(bool Visible) override;


	void updateMessageFamily(const MessageInfo& info) ;



	void updateShowFamilyMessageTips(bool show) ;

	void showDealApplyFamily(unsigned fid,unsigned uid, unsigned mid,std::string name,std::string uicon) ;

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) ;

	void hideMessageLoading() ;
protected:
	virtual bool init();
	virtual void onSetVisible(bool bVisible) override;

	// 回调函数
	void ActivateTo(Ref* pSender, unsigned uMessageType);
	void ClearScrollView(unsigned uMessageType);
	 std::string CalculateTime(double gameTime);                             //时间计算
	struct ListItem
	{
		cocos2d::Sprite		*m_pBgItem;				// 滚动视图背景图片
		cocos2d::Sprite		*m_pSpriteIcon;			// 邮件左精灵图片
		cocos2d::Sprite		*m_pSpriteIcon1;		// 系统左精灵图片
		cocos2d::Label		*m_pLabelContent;		// 显示的文本内容
		//cocos2d::Label		*m_pLebeCount;			// 显示乐币的数量
		cocos2d::Label		*m_pLabelDate;			// 显示的系统时间
		cocos2d::Label		*m_pLabelDay;			// 显示的邮件日期
		cocos2d::Label		*m_pLabelActivate;		// 显示的活动消息日期
		cocos2d::Sprite		*m_pMessageButton;		// 右精灵按钮图片
		LineLabel			*m_messageLabel ;
        LineLabel           *m_labelReward;

		cocos2d::ui::Text *m_pTextContent ;
		//俱乐部新加

		LayerButton *m_btHeadIcon;
		unsigned			m_cash ;
		unsigned			m_eventType ; //消息类型
		HttpSprite			*m_httpIcon ;  //俱乐部或者个人
		FrameScale9SpriteButton *m_btDeal; //处理按钮
		cocos2d::Label		*m_pLabelTime;		// 时间
		cocos2d::Label      *m_labeldealText ;
		//俱乐部消息新家接口
		unsigned m_familyid;
		std::string m_familyName;
		std::string m_familyIcon;

		unsigned m_userid;
		std::string m_userName;
		std::string m_userIcon;

		unsigned m_hasDeal;//是否已经处理

		unsigned m_UID ;

		double m_strLastTime;

		cocos2d::Label		*m_labelBtUname;	
		cocos2d::Label		*m_labelBtFname;

	};

	ListItem* GetListItem(unsigned uMessageType,unsigned eventType);	// 设置滚动视图的位置
	void UpdateListItemView(ListItem *pListItem, const MessageInfo& messgeInfo);	// 更新滚动视图的数据


	//void callShowUserInfo(unsigned uid) ;
	//void callShowFamilyInfo(unsigned fid) ;

	void autoLoadNewItem() ;

	void setShowDataLoading(bool flag) ;
protected:

	DataLoading* s_dataLoading ;

	bool m_bHasRequest[4];
	MenuItemSpriteWithLabel				*m_pBt[4];					// 0邮件按钮1系统按钮
	cocos2d::extension::ScrollView		*m_pScrollView[4];			// 0邮件的滚动视图1系统的滚动视图
	std::vector<ListItem*>				m_vecViewListItemOld[4];	// 0代表邮件1代表系统消息Old
	std::vector<ListItem*>				m_vecViewListItemNew[4];	// 0代表邮件1代表系统消息New
	cocos2d::Label						*nMessageNamelabel;
	cocos2d::Label						*m_pLabelMessageDay;		// 邮件的时间
	cocos2d::Label						*m_pLabelMessageTime;		// 系统的时间
	cocos2d::Label						*m_pLabelMessageActivateTime;// 活动消息的时间
	unsigned							m_uMessageTypeID;			// 记录消息的ID
	cocos2d::Sprite						*m_pSpriteImage;			// 显示邮件的右图片
	cocos2d::Sprite						*m_pSpriteImage1;			// 显示的右图片
	cocos2d::Sprite						*m_pSpriteImage2;			// 显示的右图片
	cocos2d::Sprite						*m_pSpriteImage3;			// 显示的右图片


	GameLayer                               *m_layerTop;
	GameLayer                               *m_layerBottom;

	cocos2d::Sprite                      *m_newFamilyMessageTips ;

	int loadIndex ;
	int curSize ;
	int preSize ;
private:
	unsigned							m_uMessageType;				// 消息的类型
};


#endif

