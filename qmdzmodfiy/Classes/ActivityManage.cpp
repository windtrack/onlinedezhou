#include "ActivityManage.h"
#include "GlobalUserData.h"

ActivityManage* ActivityManage::instance = nullptr;

ActivityManage* ActivityManage::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ActivityManage();
        instance->initActivityState();
    }
    return instance;
}

void ActivityManage::initActivityState()
{
    m_bNewYear = false;
    m_bSpringFestival = false;
    m_bValentineDat = false;
    m_bDragonBoat = false;
    m_bSartFestival = false;
    m_bMidAutumu = false;
    m_bNationalDay = false;
    m_bChristmas = false;
    for (auto actItem : g_globalMyData.m_vecActivityState)
    {
        if (actItem.m_activityName == "new_year" && actItem.m_state)
        {
            m_bNewYear = true;
            m_strNewYearURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "spring_festival" && actItem.m_state)
        {
            m_bSpringFestival = true;
            m_strSpringFestivalURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "valentine_day" && actItem.m_state)
        {
            m_bValentineDat = true;
            m_strValentineDatURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "dragon_boat" && actItem.m_state)
        {
            m_bDragonBoat = true;
            m_strDragonBoatURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "star_festival" && actItem.m_state)
        {
            m_bSartFestival = true;
            m_strSartFestivalURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "mid_autumn" && actItem.m_state)
        {
            m_bMidAutumu = true;
            m_strMidAutumuURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "national_day" && actItem.m_state)
        {
            m_bNationalDay = true;
            m_strNationalDayURL = actItem.m_strURL;
        }
        else if (actItem.m_activityName == "christmas" && actItem.m_state)
        {
            m_bChristmas = true;
            m_strChristmasURL = actItem.m_strURL;
        }
    }
}