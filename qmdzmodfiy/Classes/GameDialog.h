#ifndef __GAMEDIALOG_H__
#define __GAMEDIALOG_H__

#include "cocos2d.h"
class TouchGrabber;
class MenuItemSpriteWithLabel;
// 父亲层setVisible(false)的时候，务必将该dialog也setVisible(false),否则会抢夺touch消息
class GameDialog : public cocos2d::Layer
{
public:
	static GameDialog *create(const std::string& strBgDialog, bool bHasClose);
	static GameDialog *create(unsigned uDialogW, unsigned uDialogH, bool bHasClose);

	virtual void setVisible(bool bVisible) override;
	void SetVisibleCB(std::function<void(bool)> cbSetVisible);// running下有效
	MenuItemSpriteWithLabel* GetMenuItemClose(); 
	virtual void setOnVisible(bool bVisible) ;
	static std::map<GameDialog*, int>& getAllDialogs();
	static void cleanAllDialogs();
	static bool getDialogVisible();
	static bool getDialogEmpty();

	bool init() ;
	bool init(const std::string& strBgDialog, bool bHasClose);
	bool init(unsigned uDialogW, unsigned uDialogH, bool bHasClose);

	virtual void onMenuItemCloseSelected(cocos2d::Ref *);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void onGrabTouchBegan(bool bHitDialog);
	virtual void onSetVisible(bool bVisible);
	virtual void onKeyEscPressed();

	void PlayVisibleSound(bool bVisible);

	void setTouchEnabled(bool value) ;

	void callHideAction() ;
	void callShowAction() ;
	void hideLayer() ;
protected:
	//cocos2d::Sprite *m_pSpriteBgDialog;
	TouchGrabber *m_pTouchGrabberDialog;
	std::function<void(bool)> m_cbSetVisible;
	MenuItemSpriteWithLabel *m_btMenuItemClose;
};

// 捕获touch事件并Swallow住(bTouchCoverAll true全屏捕获 false在contenSize区域捕获)，注册键盘返回事件,默认啥都不做
class GameLayer : public cocos2d::Layer
{
public:
	static GameLayer *create(const std::string& strBgLayer, bool bTouchCoverAll = true, bool bGrabEscKey = true);
	static GameLayer *create(unsigned uDialogW, unsigned uDialogH, bool bTouchCoverAll = true, bool bGrabEscKey = true);
	static GameLayer *create(unsigned uDialogW, unsigned uDialogH, const cocos2d::Color4B& clrBgLayer, bool bTouchCoverAll = true, bool bGrabEscKey = true);

	virtual void setVisible(bool bVisible) override;
	void SetEscKeyReleasedCB(std::function<void()> cbEscKeyReleased); // return true继续注册捕获esc事件， false从队列移除注册

protected:

	bool init(const std::string& strBgDialog, bool bTouchCoverAll = true, bool bGrabEscKey = true);
	bool init(unsigned uDialogW, unsigned uDialogH, bool bTouchCoverAll = true, bool bGrabEscKey = true);
	bool init(unsigned uDialogW, unsigned uDialogH, const cocos2d::Color4B& clrBgLayer, bool bTouchCoverAll = true, bool bGrabEscKey = true);

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void onKeyEscPressed();

protected:	
	bool m_bTouchCoverAll;
	bool m_bGrabEscKey;
	std::function<void()> m_cbEscKeyReleased;
	TouchGrabber *m_pTouchGrabber;
};


#endif