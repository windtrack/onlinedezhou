#ifndef __PLATFORMSPEC_H__
#define __PLATFORMSPEC_H__

#include <string>
#include "cocos2d.h"
#include "jsonrpc/json/value.h"
USING_NS_CC;

unsigned GetPlatformType();
std::string getVersionName();
std::string GetMobileConditionCode();
std::string GetMobileDeviceName();

void Login3rd(unsigned uType);
void MobilePay(unsigned uPayType, const std::string& strPayID, double dPrice, const std::string& strName);
void vibrate(int times);
bool checkNetworkState();

void playVideo(const char* path, int x, int y, int w, int h);
void stopVideo();
int checkVideoVisible();
void innerSetVideoVisible(bool visible);
void setVideoVisible(bool visible);
void setVideoSoundEnable(bool bEnable);
void setVideoToolsVisible(bool bVisible);
void bringVideoToFront();
void setVideoMute(bool mute);
void callCustomService();
void getImageFromCamera();
void getImageFromGallery();

void uploadHeaderImageCallback();

void callGameBilling(std::string strJson) ;
void call3rdExit() ;

void call3rdLogin() ;
void call3rdLogout() ;



void getImageFromCamera() ;//调用拍照
void getImageFromGallery() ;//调用相册
void uploadHeaderImage();

void callDoUpdate() ;

void callSoundRecorder() ;
void callStopRecorder() ;

void showGameLog(std::string  str) ;
void showGameLogByJson(Json::Value param) ;

void callShare() ;

#endif