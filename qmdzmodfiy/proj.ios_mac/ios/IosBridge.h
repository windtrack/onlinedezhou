//
//  IosBridge.h
//  Poker
//
//  Created by apple on 15/5/1.
//
//

#ifndef Poker_IosBridge_h
#define Poker_IosBridge_h

#include<stdio.h>

#define IOS_BRIDGE IosBridge::getInstance()

typedef void (*tecentLogin)(const char *openid ,const char *token );
typedef void (*iosMobilePay)(int payState);

class IosBridge
{
public:
    static IosBridge* getInstance();
    
    void getQQLogin(tecentLogin callback);
    void getWeChatLogin(tecentLogin callback);
    void getAlipay(const char *tradeID, double price, iosMobilePay payback);
    void getUPPay(const char *tradeID, double price, iosMobilePay payback);
    
    const char* GetVersionName();
    const char* GetUUID();
    const char* GetDeviceName();
    
    bool checkNetworkState();
    
    void vibrate();
    
private:
    static IosBridge* instance;
};

#endif
