#ifndef __NEWBIEGUIDE_MANAGER_H__
#define __NEWBIEGUIDE_MANAGER_H__

#include "cocos2d.h"
#include <functional>
#include "jsonrpc/json/reader.h"

#define NewbieMgr NewbieGuideManager::getInstance()

struct NewbieStep
{
	int id;
	int type;
	int cmdType;
	std::string desc;
	Json::Value value;
};

class NewbieGuideManager
{
public:
	static NewbieGuideManager* getInstance();
	void init();

	void loadConfigs();

	NewbieStep* getNewbieStep(int step);
public:
	int m_curInTableStep;
private:
	static NewbieGuideManager* m_sNewbieMgr;
	std::map<int, NewbieStep*> m_allInTableSteps;
	
};


#endif // __NEWBIEGUIDE_MANAGER_H__