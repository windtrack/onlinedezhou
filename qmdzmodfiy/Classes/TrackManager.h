#ifndef __TRACK_MANAGER_H__
#define __TRACK_MANAGER_H__

#include "cocos2d.h"

#define TrackMgr TrackManager::getInstance()

class TrackManager
{
public:
	static TrackManager* getInstance();

	TrackManager();
	virtual ~TrackManager();
    //MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL
    void MobclickcppStartWithAppKeyAndChannel();
    void ApplicationWillEnterForeground();
    void ApplicationDidEnterBackground();
    void MobClickEnd();
	void init();
	void event(const char* eventId, const char* label);
    void event(const char* eventId, const char* type, const char* name,double price);
	void event(const char* eventId, const char* type, int counter, const char* units);
    void onEventValue();

	void setEnabled(bool enabled);
    void SetPreTime(long time); //上次计算时的时间
    long GetPreTime();
    std::string GetIntervalName(int time);
private:
	static TrackManager* mInstance;

	bool mEnabled;
    long mSysTime;
};

#endif // __TRACK_MANAGER_H__