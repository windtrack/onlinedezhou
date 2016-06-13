#ifndef __GAMESET_H__
#define __GAMESET_H__

#include "cocos2d.h"
#include "GameDialog.h"
#include "CCScrollView.h"

class TouchGrabber;
class FrameSprite;
class MenuItemSpriteWithLabel;
class MenuItemScale9SpriteWithLabel;
class FangChenMiDlg;
class GameHelp;
class GameAbout;

class GameSet : public GameDialog
{
public:
	CREATE_FUNC(GameSet);

	void UpdateSet();

	void PopDlgBangZhu();
	void PopDlgXinShowJiaoCheng();
	void PopDlgFangChenMi();
	void PopDlgPaiMianXuanZe();
	void PopDlgPaiZhuoXuanZe();
	void ZhuXiao(Ref* pSender);
	void PopGuanYu();
	void ZhuXiaoShowSetVisible(bool bIfShow);
	void OnMyDataChanged(unsigned uDataType);
    virtual void setVisible(bool bVisible) override;
protected:
	virtual bool init();
	virtual void onSetVisible(bool bVisible) override;

private:
	cocos2d::extension::ScrollView *m_pScrollView;
	cocos2d::Label *m_pLabelAccount;
	TouchGrabber *m_pTouchGrabberZhuXiao;
	TouchGrabber *m_pTouchGrabberYinYue;
	TouchGrabber *m_pTouchGrabberYinXiao;
	TouchGrabber *m_pTouchGrabberVibrate;
	TouchGrabber *m_pTouchGrabberJinZhiPangGuan;
	TouchGrabber *m_pTouchGrabberJinZhiTongZhuo;
	TouchGrabber *m_pTouchGrabberJuJueYaoQing;
	TouchGrabber *m_pTouchGrabberPaiZhuoXuanZe;
	TouchGrabber *m_pTouchGrabberPaiMianXuanZe;
	TouchGrabber *m_pTouchGrabberFangChenMi;
	TouchGrabber *m_pTouchGrabberGuanYu;
	TouchGrabber *m_pTouchGrabberXinShowJiaoCheng;
	TouchGrabber *m_pTouchGrabberBangZhu;

	FrameSprite *m_pSpriteYinYue;
	FrameSprite *m_pSpriteYinXiao;
	FrameSprite *m_pSpriteVibrate;
	MenuItemScale9SpriteWithLabel *m_pBtZhuXiao;
	/*FrameSprite *m_pSpriteJinZhiPangGuan;*/
	FrameSprite *m_pSpriteJinZhiTongZhuo;
	FrameSprite *m_pSpriteJuJueYaoQing;

	FangChenMiDlg *m_pFangChenMiDlg;
	GameAbout *m_pGameAbout;
    GameHelp  *m_pGameHelp;
};

#endif