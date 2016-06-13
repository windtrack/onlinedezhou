#include "TrackManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    #include "MobClickCpp.h"
#endif

TrackManager* TrackManager::mInstance = nullptr;

TrackManager* TrackManager::getInstance()
{
	if (mInstance == nullptr) {
		mInstance = new TrackManager();
	}

	return mInstance;
}

TrackManager::TrackManager() :
mEnabled(true)
{

}

TrackManager::~TrackManager()
{

}

void TrackManager::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

void TrackManager::MobclickcppStartWithAppKeyAndChannel()
{
    if (!mEnabled)
        return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("5541d68667e58e060d000545", "Default");
#endif
}
void TrackManager::ApplicationWillEnterForeground()
{
    if (!mEnabled)
        return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umeng::MobClickCpp::applicationWillEnterForeground();
#endif
}

void TrackManager::ApplicationDidEnterBackground()
{
    if (!mEnabled)
        return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umeng::MobClickCpp::applicationDidEnterBackground();
#endif
}

void TrackManager::MobClickEnd()
{
    if (!mEnabled)
        return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umeng::MobClickCpp::end();
#endif
}

void TrackManager::init()
{
	if (!mEnabled)
		return;

	// initialize
}
std::string TrackManager::GetIntervalName(int time)
{
    if (time < 300)
    {
        if (time < 30)
            return "0-30";
        else if (time < 60)
            return "30-60";
        else if (time < 120)
            return "1-2";
        else if (time < 180)
            return "2-3";
        else if (time < 300)
            return "3-5";
    }
    else
        time = time/60;

    if (time<10)
    {
        return "5-10";
    }
    else if (time >= 10)
    {
        char buff[64] = {0};
        int temp = time/10;
        if (time%10 == 0)
        {
            sprintf(buff, "%d-%d", (temp-1)*10, temp*10);
        }
        else
        {
            sprintf(buff, "%d-%d", temp*10, (temp+1)*10);
        }
        return buff;
    }
        
}

void TrackManager::event(const char* eventId, const char* type, int counter, const char* units)
{
	if (!mEnabled)
		return;
    char interval[64];
    if (counter != 0)
        sprintf(interval, "%s%d%s", type, counter, units);
    else
        sprintf(interval, "%s%s", type, units);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umeng::eventDict dict;
    //dict["type"] = type;
    dict["interval"] = interval;
    umeng::MobClickCpp::event(eventId, &dict, counter);
#endif
}

void TrackManager::event(const char* eventId, const char* type, const char* name, double price)
{
    if (!mEnabled)
        return;
    char goodsPrice[64];
    sprintf(goodsPrice, "%.0f元", price);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umeng::eventDict dict;
    dict["PayType"] = type;
    dict["GoodsName"] = name;
    dict["GoodsPrice"] = goodsPrice;
    umeng::MobClickCpp::event(eventId, &dict);
#endif
}

void TrackManager::event(const char* eventId, const char* label)
{
	if (!mEnabled)
		return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    umeng::MobClickCpp::event(eventId, label);
#endif
}

void TrackManager::SetPreTime(long time)
{
    mSysTime = time;
}

long TrackManager::GetPreTime()
{
    return mSysTime;
}