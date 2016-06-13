#ifndef _SELFFUND_H_
#define _SELFFUND_H_
#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
#define  FundViewSize 4
struct SingleFundData ;
class SelfFund : public GameDialog ,public cocos2d::extension::ScrollViewDelegate
{
public:
	CREATE_FUNC(SelfFund) ;
	bool init() ;
	void setVisible(bool bVisible) ;

	void callView(int index) ;

	void updateView(int index) ;

	void setCurFundType(unsigned type) ;

	void updateData() ;
protected:
	struct LineFundData
	{
		cocos2d::Layer							*m_spriteTableBg; //背景
		cocos2d::LayerColor                     *m_spriteColorBg; //背景
		cocos2d::Sprite							*m_spritePersonal ; //个人房
		cocos2d::Sprite							*m_spriteClub ;//私密房
		cocos2d::Label                          *m_labelDate; //日期
		cocos2d::Label                          *m_labelRoomName;//房间名
		cocos2d::Label                          *m_labelCash;//基金
		unsigned								m_index; //
	};
	LineFundData* GetLineFundDataItem(unsigned uIndex);
	void UpdateFundDataView(SelfFund::LineFundData* item, const SingleFundData &data,int index);

private:
	void OnMenuItemSelect(cocos2d::Ref *pSend, cocos2d::ui::Button::TouchEventType type);
private:

	unsigned								m_curFundType ;
	unsigned								m_curIndex ;
	cocos2d::Node							*s_uiroot ;//逻辑
	cocos2d::ui::Layout						*m_layout_btController[2] ;

	cocos2d::ui::Button						*m_btController[FundViewSize] ; //三个按钮
	cocos2d::ui::Button						*m_btControllerPerosonal[2][FundViewSize] ; //三个按钮
	//cocos2d::ui::Button						*m_btControllerClub[2][FundViewSize] ; //三个按钮

	cocos2d::ui::Text						*m_textbtController[FundViewSize];
	cocos2d::ui::Text						*m_textbtControllerPersonal[2][FundViewSize];
	//cocos2d::ui::Text						*m_textbtControllerClub[FundViewSize];

	cocos2d::ui::Layout						*m_layerFund[FundViewSize] ;//对应的上层显示

	cocos2d::ui::Layout						*m_layerView[FundViewSize] ;//三个滚动父节点
	cocos2d::extension::ScrollView          *m_scrollView[FundViewSize];//三个滚动层

	cocos2d::ui::Text						*m_textFundAllCash[FundViewSize];
	cocos2d::ui::Text						*m_textFundPersonalCash[FundViewSize];
	cocos2d::ui::Text						*m_textFundClubCash[FundViewSize];

	cocos2d::Size							m_sizeView ;

	cocos2d::ui::Text						*m_textNoFundInfo;

	std::vector<LineFundData*>            m_vecLineFundNew;
	std::vector<LineFundData*>            m_vecLineFundOld;

	cocos2d::ui::Text						*m_text_fundAll;
};

#endif