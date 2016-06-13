LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


include $(CLEAR_VARS)
LOCAL_MODULE := libentryexpro.so
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libentryexpro.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcocklogic.so
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libcocklogic.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libtnet-1.1.5.so
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libtnet-1.1.5.so
include $(PREBUILT_SHARED_LIBRARY)

#ÐÂÀËÎ¢²©µÇÂ¼
include $(CLEAR_VARS)
LOCAL_MODULE := libweibosdkcore.so
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libweibosdkcore.so
include $(PREBUILT_SHARED_LIBRARY)

#ÐÅ¸ë
include $(CLEAR_VARS)
LOCAL_MODULE := libtpnsSecurity.so
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libtpnsSecurity.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libtpnsWatchdog.so
LOCAL_SRC_FILES := prebuilt/$(TARGET_ARCH_ABI)/libtpnsWatchdog.so
include $(PREBUILT_SHARED_LIBRARY)



include $(CLEAR_VARS)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := hellocpp/main.cpp  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)  
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)  
  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d/cocos/audio/include \

LOCAL_WHOLE_STATIC_LIBRARIES += libjsonrpc_static
LOCAL_SHARED_LIBRARIES := mobclickcpp_shared

LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,jsonrpc)
$(call import-module,libmobclickcpp)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
