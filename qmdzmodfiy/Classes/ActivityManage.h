#ifndef __ACTIVITYMANAGE_H__
#define __ACTIVITYMANAGE_H__
#define  ActManage ActivityManage::getInstance()
#include "Util.h"
//元旦   new_year       //春节   spring_festival
//情人节 valentine_day  //端午节 dragon_boat
//七夕节 star_festival  //中秋节 mid_autumn
//国庆节 national_day   //圣诞节 Christmas
class ActivityManage 
{
public:
    static ActivityManage* getInstance();
    void initActivityState();
    inline bool getNewYearState(){return m_bNewYear;};
    inline bool getSpringFestivalState(){return m_bSpringFestival;};
    inline bool getValentineDatState(){return m_bValentineDat;};
    inline bool getDragonBoatState(){return m_bDragonBoat;};
    inline bool getSartFestivalState(){return m_bSartFestival;};
    inline bool getMidAutumuState(){return m_bMidAutumu;};
    inline bool getNationalDayState(){return m_bNationalDay;};
    inline bool getChristmasState(){return m_bChristmas;};

    inline std::string getNewYearURL(){return m_strNewYearURL;};
    inline std::string getSpringFestivalURL(){return m_strSpringFestivalURL;};
    inline std::string getValentineDatURL(){return m_strValentineDatURL;};
    inline std::string getDragonBoatURL(){return m_strDragonBoatURL;};
    inline std::string getSartFestivalURL(){return m_strSartFestivalURL;};
    inline std::string getMidAutumuURL(){return m_strMidAutumuURL;};
    inline std::string getNationalDayURL(){return m_strNationalDayURL;};
    inline std::string getChristmasURL(){return m_strChristmasURL;};
protected:
private:
    static  ActivityManage*  instance;
    bool    m_bNewYear;
    bool    m_bSpringFestival;
    bool    m_bValentineDat;
    bool    m_bDragonBoat;
    bool    m_bSartFestival;
    bool    m_bMidAutumu;
    bool    m_bNationalDay;
    bool    m_bChristmas;
    std::string m_strNewYearURL;
    std::string m_strSpringFestivalURL;
    std::string m_strValentineDatURL;
    std::string m_strDragonBoatURL;
    std::string m_strSartFestivalURL;
    std::string m_strMidAutumuURL;
    std::string m_strNationalDayURL;
    std::string m_strChristmasURL;
};

#endif