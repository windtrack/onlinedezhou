#include "NewbieGuideManager.h"
#include "tinyxml2/tinyxml2.h"

USING_NS_CC;
using namespace tinyxml2;


NewbieGuideManager* NewbieGuideManager::m_sNewbieMgr = nullptr;

NewbieGuideManager* NewbieGuideManager::getInstance()
{
    CCLOG("newbieguide0000");
	if (m_sNewbieMgr == nullptr) {
		m_sNewbieMgr = new NewbieGuideManager();
		m_sNewbieMgr->init();
	}

	return m_sNewbieMgr;
}

void NewbieGuideManager::init()
{
	m_curInTableStep = 1;
}

void NewbieGuideManager::loadConfigs()
{
	if (m_allInTableSteps.size() > 0)
		return;

	tinyxml2::XMLDocument doc;
	//std::string file_path = FileUtils::getInstance()->fullPathForFilename("newbieguide.xml");
    //CCLOG("newbieguide11111 %s", file_path.c_str());
    doc.Parse(FileUtils::getInstance()->getStringFromFile("newbieguide.xml").c_str());
	//doc.LoadFile(file_path.c_str());
	auto rootElement = doc.RootElement();//Root  
	auto xnInTable = rootElement->FirstChildElement("intable");
	auto xnStep = xnInTable->FirstChildElement();
	while (xnStep != nullptr) {
		auto ns = new NewbieStep();
		ns->id = xnStep->IntAttribute("id");
		ns->type = xnStep->IntAttribute("type");
		ns->desc = xnStep->Attribute("desc");
		ns->cmdType = xnStep->IntAttribute("cmdType");
		auto xnData = xnStep->FirstChildElement("data");
		auto value = xnData->GetText();
		if (value != nullptr) {
			Json::Reader reader;
			reader.parse(value, ns->value);
		}
		m_allInTableSteps.insert(std::make_pair(ns->id, ns));
		xnStep = xnStep->NextSiblingElement();
	}
}

NewbieStep* NewbieGuideManager::getNewbieStep(int step)
{
	return m_allInTableSteps[step];
}
