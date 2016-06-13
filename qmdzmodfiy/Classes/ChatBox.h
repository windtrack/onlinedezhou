#ifndef __CHATBOX__H__
#define __CHATBOX__H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "CCScrollView.h"
#include "CCTableView.h"
#include "CCTableViewCell.h"
#include "RadioGroup.h"
#include "GameDialog.h"

class MenuItemSpriteWithLabel;
class HitTouchGrabLayer;
class HttpSprite;
class TouchGrabLayer;

class ChatBox : public GameDialog, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(ChatBox);

	void AppandWord(const std::string& strIcon, const std::string& strWord, unsigned uUID, bool bIconLeft);

	void AppandUser(const std::string& strIcon, const std::string& strName, unsigned uUID);
	void RemoveUser(unsigned uUID);

	//void SetCBHide(std::function<void()> funCB)
	//{
	//	m_funCBHide = funCB;
	//}
	virtual void setVisible(bool bVisible) override;
	virtual void onSetVisible(bool bVisible) override;
    void SetEditText(std::string strName);

	void resetAt();
protected:
	void onMenuItemSelected(cocos2d::Ref *pItem);
	void onMenuItemExpressCommonSelected(cocos2d::Ref *pItem);
	void onMenuItemExpressNobleSelected(cocos2d::Ref *pItem);
	
	// Layer
	virtual bool init();

	// EditBoxDelegate
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

	void SendChat(const std::string& strChat);
	void SendChat();
	void SendExpress(unsigned uIdx, bool bCommon);	

private:
	std::function<void()> m_funCBHide;

	cocos2d::Sprite *m_pSpriteBgEditLong;
	cocos2d::Sprite *m_pSpriteBgEditShort;

	MenuItemSpriteWithLabel *m_pBtExpress;
	MenuItemSpriteWithLabel *m_pBtLine;
	MenuItemSpriteWithLabel *m_pBtWord;

	MenuItemSpriteWithLabel *m_pBtBack;
	MenuItemSpriteWithLabel *m_pBtAt;
	MenuItemSpriteWithLabel *m_pBtSend;
	cocos2d::ui::EditBox *m_pEdtChat;

	MenuItemSpriteWithLabel *m_pBtExpressCommon;
	MenuItemSpriteWithLabel *m_pBtExpressNoble;

	cocos2d::Sprite *m_pSpriteExpressCommon;
	cocos2d::Sprite *m_pSpriteExpressNoble;
	cocos2d::Vector<cocos2d::MenuItem*> m_vecBtExpressCommon;
	cocos2d::Vector<cocos2d::MenuItem*> m_vecBtExpressNoble;

	cocos2d::extension::ScrollView *m_pScrollViewLine;
	std::vector<HitTouchGrabLayer*> m_vecBtLine;

	float m_fScrollViewWordStartY;
	cocos2d::extension::ScrollView *m_pScrollViewWord;
	std::vector<cocos2d::Label*> m_vecLabelWords;
	std::vector<HttpSprite*> m_vecIconOfWords;
	std::vector<HitTouchGrabLayer*> m_vecBtPressWords;
	std::vector<HitTouchGrabLayer*> m_vecBtPressIcons;
	std::vector<unsigned> m_vecWordUIDs;

	TouchGrabLayer *m_pLayerBgAt;
	cocos2d::extension::ScrollView *m_pScrollViewAt;
	cocos2d::LayerColor *m_pLayerPressAt;
	std::vector<cocos2d::Label*> m_vecLabelAt;
	std::vector<HttpSprite*> m_vecIconAt;
	std::vector<HitTouchGrabLayer*> m_vecBtPressAt;
	std::vector<unsigned> m_vecAtUIDs;
    bool m_IsExpress;
    bool m_IsAtOthers;


	enum
	{
		RadioItemIcon,
		RadioItemLine,
		RadioItemWord,
	};

	enum
	{
		RadioItemIconCommon,
		RadioItemIconNoble
	};

	RadioGroup m_radioGroupLeftSwitch;
	RadioGroup m_radioGroupIconSwitch;

};

#endif