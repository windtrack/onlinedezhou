#ifndef _UPDATEINFO_H_
#define _UPDATEINFO_H_

#include "cocos2d.h"
#include "GameDialog.h"
#include "UICommon.h"
class LineLabel ;
class UpdateInfo :public GameLayer
{
public:
	CREATE_FUNC(UpdateInfo);

	void updateInfo() ;
	void updateTileVersion(std::string version) ;
protected:
	bool init() ;
private:
	cocos2d::extension::ScrollView* m_pScrollView ;
	std::vector<LineLabel*>		m_vecAutoLineLabel;

	Size sizeListView ;

	cocos2d::Sprite *m_spriteFlowerLeft ;
	cocos2d::Sprite *m_spriteFlowerRight ;
	cocos2d::Label *m_labelTitle ;
	int m_curLineCount ;
};

#endif