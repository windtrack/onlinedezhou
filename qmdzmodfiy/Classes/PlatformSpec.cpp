#include "PlatformSpec.h"
#include "GlobalUserData.h"
#include "GameData.h"
#include "HttpUploader.h"
#include "Util.h"
#include "jsonrpc/json/reader.h"
#include "jsonrpc/json/value.h"
#include "jsonrpc/json/writer.h"


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "Util.h"
#include "json/reader.h"
#include "json/value.h"

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#include "IosBridge.h"

#endif

unsigned GetPlatformType()
{
	return CC_TARGET_PLATFORM;
}

std::string getVersionName()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity","getVersionName","()Ljava/lang/String;"))
	{
		jstring result = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		std::string strVersionName = JniHelper::jstring2string(result);
		return strVersionName;
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return IOS_BRIDGE->GetVersionName();
#endif
    
    return "unknown";
}

std::string GetMobileConditionCode()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity","getLocalMacAddress","()Ljava/lang/String;"))
	{
		jstring result = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
		std::string strMac = JniHelper::jstring2string(result);
		t.env->DeleteLocalRef(t.classID);
		return strMac;
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return IOS_BRIDGE->GetUUID();
#endif
    
    return "";
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define JAVAVM cocos2d::JniHelper::getJavaVM()
std::string jni_GetStaticField_string(const char* className, const char* fieldName)
{
	std::string strRes;
	do 
	{
		JNIEnv *pEnv = 0;
		
		if (JAVAVM->GetEnv((void**)&pEnv, JNI_VERSION_1_4) != JNI_OK)
		{
			break;
		}

		if (JAVAVM->AttachCurrentThread(&pEnv, 0) < 0)
		{
			break;
		}
		jclass classID = pEnv->FindClass(className);
		if (!classID)
		{
			break;
		}
		jfieldID fieldID = pEnv->GetStaticFieldID(classID, fieldName, "Ljava/lang/String;");
		if(!fieldID)
		{
			break;
		}
		jstring jstr = (jstring)pEnv->GetStaticObjectField(classID, fieldID);
		strRes = JniHelper::jstring2string(jstr);
	} while (0);
	return strRes;
}
#endif

std::string GetMobileDeviceName()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return jni_GetStaticField_string("android/os/Build", "BRAND");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return IOS_BRIDGE->GetDeviceName();
#endif
    return "";
}

// android jni native callback
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_LoginActivity_setQQLoginResult(JNIEnv *env, jobject thiz, jstring openid, jstring token)
	{
		char szOpenId[512];
		strcpy(szOpenId, env->GetStringUTFChars(openid, NULL));
		
		char szToken[512];
		strcpy(szToken, env->GetStringUTFChars(token, NULL));

		Json::Value param;
		param["cmd"] = NonUI_QQLoginResult;
		param["openid"] = szOpenId;
		param["token"] = szToken;
		SendNonUIMsg(param);
	}


	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_wxapi_WXEntryActivity_setWXLoginResult(JNIEnv *env, jobject thiz, jstring openid, jstring token)
	{
		char szOpenId[512];
		strcpy(szOpenId, env->GetStringUTFChars(openid, NULL));
		
		char szToken[512];
		strcpy(szToken, env->GetStringUTFChars(token, NULL));

		Json::Value param;
		param["cmd"] = NonUI_WXLoginResult;
		param["openid"] = szOpenId;
		param["token"] = szToken;
		SendNonUIMsg(param);
	}



	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_sina_SinaLoginLogout_setSinaLoginResult(JNIEnv *env, jobject thiz, jstring openid, jstring token)
	{
		char szOpenId[512];
		strcpy(szOpenId, env->GetStringUTFChars(openid, NULL));

		char szToken[512];
		strcpy(szToken, env->GetStringUTFChars(token, NULL));

		Json::Value param;
		param["cmd"] = NonUI_SinaLoginResult;
		param["openid"] = szOpenId;
		param["token"] = szToken;
		SendNonUIMsg(param);
	}

	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_set3rdLoginResult(JNIEnv *env, jobject thiz, jstring openid, jstring token)
	{
		char szOpenId[512];
		strcpy(szOpenId, env->GetStringUTFChars(openid, NULL));

		char szToken[512];
		strcpy(szToken, env->GetStringUTFChars(token, NULL));

		Json::Value param;
		param["cmd"] = NonUI_3rdLoginResult;
		param["openid"] = szOpenId;
		param["token"] = szToken;
		SendNonUIMsg(param);
	}


	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_UniPaying_setMobilePayResult(JNIEnv *env, jobject thiz, jint res)
	{
		Json::Value param;
		param["cmd"] = NonUI_MobilePayResult;
		param["res"] = 2;
		if (res == 1)
			param["res"] = 1;
		else if (res == 2)
			param["res"] = 2;
		else if (res == 3)
			param["res"] = 3;
		SendNonUIMsg(param);
	}
}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

void getQQLoginResult_iOS(const char *openid, const char *token)
{
    Json::Value param;
    param["cmd"] = NonUI_QQLoginResult;
    param["openid"] = openid;
    param["token"] = token;
    SendNonUIMsg(param);
}

void getWeChatLoginResult_iOS(const char *openid, const char *token)
{
    Json::Value param;
    param["cmd"] = NonUI_WXLoginResult;
    param["openid"] = openid;
    param["token"] = token;
    SendNonUIMsg(param);
}
void getSinaLoginResult_iOS(const char *openid, const char *token)
{
    Json::Value param;
    param["cmd"] = NonUI_SinaLoginResult;
    param["openid"] = openid;
    param["token"] = token;
    SendNonUIMsg(param);
}


void getPayResult_iOS(int res)
{
    Json::Value param;
    param["cmd"] = NonUI_MobilePayResult;
    param["res"] = res;
    SendNonUIMsg(param);
}

void checkPayResult_iOS(const char * res,const char * price,const char * orderid)
{
    Json::Value param;
    param["cmd"] = NonUI_AppStoreCheck;
    param["product"] = res;
    param["price"] = price;
    param["orderid"] = orderid;
    SendNonUIMsg(param);
}
#endif


void Login3rd(unsigned uType)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (uType == 0)
	{
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "QQ_Logon", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
		else
		{
		}
	}
	else if (uType == 1)
	{
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "WX_Logon", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
		else
		{
		}
	}
	else if (uType == 2)
	{
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "sina_Logon", "()V"))
		{
			t.env->CallStaticVoidMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
		else
		{
		}
	}

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    if (uType == 0) {
        IOS_BRIDGE->getQQLogin(getQQLoginResult_iOS);
    }
    else if (uType == 1) {
        IOS_BRIDGE->getWeChatLogin(getWeChatLoginResult_iOS);
    }
    else if (uType == 2) {
        IOS_BRIDGE->getSinaLogin(getSinaLoginResult_iOS);
    }
    
#endif
}


void MobilePay(unsigned uPayType, const std::string& strPayID, double dPrice, const std::string& name)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (uPayType == 0 || uPayType == 1 || uPayType == 2) //  0-银联 1-支付宝 2-微信
	{
		JniMethodInfo t;
		if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "Mobile_pay", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
		{
			jstring strOrderID = t.env->NewStringUTF(strPayID.c_str());
			char str[256] = {0};
			sprintf(str, "%.2f", dPrice);
			jstring strPrice = t.env->NewStringUTF(str);
			jstring strName = t.env->NewStringUTF(name.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, strOrderID, strPrice, strName, uPayType);
			t.env->DeleteLocalRef(strOrderID);
			t.env->DeleteLocalRef(strPrice);
			t.env->DeleteLocalRef(strName);
			t.env->DeleteLocalRef(t.classID);
		}
		else
		{
		}
	}
	else if (uPayType == 2) // 微信
	{

	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

    if (uPayType == 0) {
        IOS_BRIDGE->getUPPay(strPayID.c_str(), dPrice, getPayResult_iOS);
    }
    else if (uPayType == 1) {
        IOS_BRIDGE->getAlipay(strPayID.c_str(), dPrice, getPayResult_iOS);
    }
    else if (uPayType == 2) {
        IOS_BRIDGE->getWXPay(strPayID.c_str(), dPrice, getPayResult_iOS);
    }
    
#endif
    
}


// times: 毫秒
void vibrate(int times)
{
	if (false == DataMgr->getVibrate())
		return;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "Vibrate", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID,  times);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	IosBridge::getInstance()->vibrate();
#endif

}

bool checkNetworkState()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "checkNetworkState", "()Z"))
	{
		jboolean connected = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);

		return connected;
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	// TODO: check network state on ios device
    return IOS_BRIDGE->checkNetworkState();
#else
	return true;
#endif	
}

int videoVisibleFlag = 0;
void playVideo(const char* path, int x, int y, int w, int h)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "playVideo", "(Ljava/lang/String;II)V"))
	{
		jstring strPath = t.env->NewStringUTF(path);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, strPath, x, y);
		t.env->DeleteLocalRef(strPath);
		t.env->DeleteLocalRef(t.classID);
	}
	videoVisibleFlag = 1;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif

	
}

void stopVideo()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "stopVideo", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	videoVisibleFlag = 2;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
	
}

int checkVideoVisible()
{
	return videoVisibleFlag;
}

void setVideoVisible(bool visible)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	int visibleFlag = visible ? 1 : 2;
	if (visibleFlag == videoVisibleFlag)
		return;

	videoVisibleFlag = visibleFlag;
#endif
	innerSetVideoVisible(visible);
}

void innerSetVideoVisible(bool visible)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "setVideoVisible", "(Z)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, visible);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif

	
}

void setVideoSoundEnable(bool bEnable)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "setVideoMute", "(Z)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, !bEnable);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}
void setVideoToolsVisible(bool bVisible)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "setVideoToolsVisible", "(Z)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, bVisible);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}
void bringVideoToFront()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "bringVideoToFront", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}

void setVideoMute(bool mute)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "setVideoMute", "(Z)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, mute);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}

void callCustomService()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callCustomService", "(Ljava/lang/String;)V"))
	{
		jstring strNumber = t.env->NewStringUTF("4007700058");
		t.env->CallStaticVoidMethod(t.classID, t.methodID, strNumber);
		t.env->DeleteLocalRef(strNumber);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    IosBridge::getInstance()->dialCustomService();
#endif
}


extern int g_updateFlag;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_nativeNetworkChanged(JNIEnv*  env, jobject thiz, jint status) {
		/*EventCustom event("network_changed");
		char buffer[8];
		sprintf(buffer, "%d", status);
		event.setUserData(buffer);

		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);*/
		SendNetworkChangedMsg(status);
	}

	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_nativeUpdateChanged(JNIEnv*  env, jobject thiz) {
		g_updateFlag = 1;
		/*EventCustom event("update_changed");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);*/
	}

	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_showUpdateScene(JNIEnv*  env, jobject thiz) {
		g_updateFlag = 3;

		//char szOpenId[512];
		//strcpy(szOpenId, env->GetStringUTFChars(version, NULL));
		//setNewVersion() ;



		/*EventCustom event("update_changed");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);*/
	}
}
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_callPauseNetWork(JNIEnv*  env, jobject thiz, jboolean flag) {

		unsigned char b = flag;   
		printf("\n c-boolean: %lu  ", b);   
		if (b) {   
				GameData::getInstance()->setPauseNetWork(true) ;
		} else {   
				GameData::getInstance()->setPauseNetWork(false) ;
		} 
	}
}
#endif



void callGameBilling(std::string strJson)
{



#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callBilling", "(Ljava/lang/String;)V"))
	{
		//jint jprice= price ;
		jstring strOrderID = t.env->NewStringUTF(strJson.c_str());


		t.env->CallStaticVoidMethod(t.classID, t.methodID, strOrderID);
		t.env->DeleteLocalRef(strOrderID);
		t.env->DeleteLocalRef(t.classID);

	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

	   Json::Reader reader;
    
    Json::Value json_object;
    
    if (!reader.parse(strJson, json_object))
        return ;
    //    int sInt_att = JsonValueAdapter(json_object["index"]) ;
    //    std::string orderid =json_object["strTradeID"].asCString() ;
    //    callAppStorePay(sInt_att);
    
    //    text["uid"] = g_globalMyData.m_iUserId;
    //    text["uname"] = g_globalMyData.GetNickName();
    //    text["strTradeID"] =  strTradeID;
    //    text["itemName"] = strName;
    //    text["itemprice"] = str;
    //    text["itemid"] = itemid;
    //    text["index"] = index;
    
    int index = JsonValueAdapter(json_object["index"]) ;
    std::string orderid = json_object["strTradeID"].asCString() ;
    std::string price = json_object["itemprice"].asCString() ;
    //callAppStorePay(sInt_att);
    IOS_BRIDGE->showAppStorePay(index,price.c_str(),orderid.c_str(),getPayResult_iOS,checkPayResult_iOS);

#endif

}


/************************************************************************/
/* 调用第三方退出                                                                     */
/************************************************************************/
void call3rdExit()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "call3rdExit", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);

	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}


/************************************************************************/
/* 游戏结束                                                                     */
/************************************************************************/
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_callGameFinish(JNIEnv*  env, jobject thiz) {

		SendClientRequest(ClientRequest::CallGameFinished);
	}
}
#endif




/************************************************************************/
/* 调用第三方登录                                                                     */
/************************************************************************/
void call3rdLogin()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callShow3rdLogin", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}






/************************************************************************/
/* 调用第三方退出   如果有的话                                                                     */
/************************************************************************/
void call3rdLogout()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callShow3rdLogout", "()V"))
	{


		t.env->CallStaticVoidMethod(t.classID, t.methodID);

		t.env->DeleteLocalRef(t.classID);


	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}


/************************************************************************/
/* 返回登录界面                                                                     */
/************************************************************************/
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_callReturnToLogin(JNIEnv*  env, jobject thiz) {

		SendClientRequest(ClientRequest::CallReturnToLogin);
	}

}
#endif


/************************************************************************/
/* 调用拍照功能                                                                     */
/************************************************************************/
void getImageFromCamera()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callTakePhoto", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);

	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	IosBridge::getInstance()->getImageFromCamera(uploadHeaderImageCallback);
#endif

}

/************************************************************************/
/* 调用相册                                                                     */
/************************************************************************/
void getImageFromGallery()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callPhotoalbum", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	IosBridge::getInstance()->getImageFromGallery(uploadHeaderImageCallback);

#endif

}

/************************************************************************/
/* ios下上传图片回调                                                                     */
/************************************************************************/
void uploadHeaderImageCallback()
{
	SendClientRequest(ClientRequest::kCallUploadPhoto);
}


/************************************************************************/
/* android下上传照片的回调                                                                     */
/************************************************************************/
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_PhotoManagerActivity_callUploaderPhoto(JNIEnv*  env, jobject thiz) {
		SendClientRequest(ClientRequest::kCallUploadPhoto);
	}
}
#endif



/************************************************************************/
/* 游戏更新相关                                                                     */
/************************************************************************/
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_setNeedUpdate(JNIEnv*  env, jobject thiz,jboolean flag) {
		unsigned char b = flag;   
		printf("\n c-boolean: %lu  ", b);   
		if (b) {   
			GameData::getInstance()->setUpdateVersion(true) ;
		} else {   
			GameData::getInstance()->setUpdateVersion(false) ;
		} 
	}
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_setUpdateLoadingPro(JNIEnv*  env, jobject thiz,jint pro) {
		setUpdateLoadPro(pro) ;
	}
}
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_setNewVersion(JNIEnv*  env, jobject thiz,jstring version) {
		char szOpenId[64];
		strcpy(szOpenId, env->GetStringUTFChars(version, NULL));
		setNewVersion(szOpenId) ;
	}
}
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_setUpdateContent(JNIEnv*  env, jobject thiz,jstring content) {
		char szOpenId[2048];
		strcpy(szOpenId, env->GetStringUTFChars(content, NULL));
		setUpdateContent(szOpenId) ;
	}
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_SoundRecorder_callUploaderSoundRecorder(JNIEnv*  env, jobject thiz) {
		//SendClientRequest(ClientRequest::kCallUploadPhoto);
	}

}
extern "C" {
	JNIEXPORT void JNICALL Java_com_oo58_game_texaspoker_AppActivity_PushJson(JNIEnv*  env, jobject thiz,jstring content) {
			//SendClientRequest(ClientRequest::kCallUploadPhoto);
		std::string strMac = JniHelper::jstring2string(content);

		Json::Reader reader;

		Json::Value json_object;

		if (!reader.parse(strMac, json_object))
			return ;
		int sInt_att = JsonValueAdapter(json_object["Int_att"]) ;
		std::string String_att = json_object["String_att"].asString() ;
		showGameLog(String_att) ;


	}
}
#endif


void callDoUpdate()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/UpdateUtils", "doUpdate", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS


#endif
}

/************************************************************************/
/* 录音                                                                     */
/************************************************************************/
void callSoundRecorder()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callSoundRecorder", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	//IosBridge::getInstance()->getImageFromGallery(uploadHeaderImageCallback);

#endif
}

void callStopRecorder()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callStopRecorder", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	//IosBridge::getInstance()->getImageFromGallery(uploadHeaderImageCallback);

#endif
}

void showGameLog(std::string str)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "showLog", "(Ljava/lang/String;)V"))
	{
		//jint jprice= price ;
		jstring strOrderID = t.env->NewStringUTF(str.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, strOrderID);
		t.env->DeleteLocalRef(strOrderID);
		t.env->DeleteLocalRef(t.classID);

	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}

void showGameLogByJson(Json::Value param)
{
	//Json::Value text;
	//text["uid"] = g_globalMyData.m_iUserId;
	//text["uname"] = g_globalMyData.GetNickName();
	//text["strTradeID"] =  strTradeID;
	//text["itemName"] = strName;
	//text["itemprice"] = str;
	//text["itemid"] = itemid;
	Json::FastWriter fast_writer2;
	std::string strBody = fast_writer2.write(param);
	showGameLog(strBody) ;
}


void callShare()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t,"com/oo58/game/texaspoker/AppActivity", "callShare", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);

	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
}