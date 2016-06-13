LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_CPPFLAGS += -fexceptions
LOCAL_MODULE := libjsonrpc_static
LOCAL_MODULE_FILENAME := libjsonrpc
LOCAL_SRC_FILES := \
  client.cpp \
  clientconnector.cpp \
  errors.cpp \
  exception.cpp \
  procedure.cpp \
  rpcprotocolclient.cpp \
  rpcprotocolserver.cpp \
  server.cpp \
  serverconnector.cpp \
  specificationparser.cpp \
  specificationwriter.cpp \
  connectors/httpclient.cpp \
  connectors/httpserver.cpp \
  connectors/mongoose.c \
  json/json_reader.cpp \
  json/json_value.cpp \
  json/json_writer.cpp
LOCAL_C_INCLUDES := \
  $(LOCAL_PATH)/ \
  $(LOCAL_PATH)/../curl/include/android
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/
include $(BUILD_STATIC_LIBRARY)