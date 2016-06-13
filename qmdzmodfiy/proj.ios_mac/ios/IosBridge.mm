//
//  IosBridge.mm
//  Poker
//
//  Created by apple on 15/5/1.
//
//

#import "IosBridge.h"
#import <Foundation/Foundation.h>
#import "IosOperation.h"
#import "SSKeychain.h"
#import "Reachability.h"
#import <AudioToolbox/AudioToolbox.h>

IosBridge* IosBridge::instance = nullptr;

IosBridge* IosBridge::getInstance()
{
    if (!instance) {
        instance = new IosBridge();
    }
    return instance;
}

void IosBridge::getQQLogin(tecentLogin callback)
{
    IosOperation *_operation = [[IosOperation alloc] init];
    [_operation pullQQLogin:callback];
}

void IosBridge::getWeChatLogin(tecentLogin callback)
{
    IosOperation *_operation = [[IosOperation alloc] init];
    [_operation pullWeChatLogin:callback];
}

void IosBridge::getAlipay(const char *tradeID, double price, iosMobilePay payback)
{
    IosOperation *_operation = [[IosOperation alloc] init];
    [_operation pullAlipay:tradeID price:price callback:payback];
}

void IosBridge::getUPPay(const char *tradeID, double price, iosMobilePay payback)
{
    IosOperation *_operation = [[IosOperation alloc] init];
    [_operation pullUPPay:tradeID price:price callback:payback];
}

const char* IosBridge::GetVersionName()
{
    NSString* versionName = [NSString stringWithFormat:@"v%@", [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString*)kCFBundleVersionKey]];
    return [versionName UTF8String];
}

const char* IosBridge::GetUUID()
{
//    NSString* uuid = [[NSUUID UUID] UUIDString];
    NSString *appName=[[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString*)kCFBundleNameKey];
    
    NSString *strApplicationUUID = [SSKeychain passwordForService:appName account:@"incoding"];
    if (strApplicationUUID == nil)
    {
        strApplicationUUID  = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
        [SSKeychain setPassword:strApplicationUUID forService:appName account:@"incoding"];
    }
    
    return [strApplicationUUID UTF8String];
}

const char* IosBridge::GetDeviceName()
{
    NSString* deviceName = [[UIDevice currentDevice] name];
    return [deviceName UTF8String];
}

bool IosBridge::checkNetworkState()
{
    bool retval = false;
    Reachability *r = [Reachability reachabilityWithHostName:@"www.apple.com"];
    switch ([r currentReachabilityStatus]) {
        case NotReachable:
            // 没有网络连接
            retval = false;
            break;
        case ReachableViaWWAN:
            // 使用3G网络
            retval = true;
            break;
        case ReachableViaWiFi:
            // 使用WiFi网络
            retval = true;
            break;
    }
    
    return retval;
}

void IosBridge::vibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}


