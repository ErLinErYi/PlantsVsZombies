LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_curl_static
LOCAL_MODULE_FILENAME := curl
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libcurl.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/android
LOCAL_STATIC_LIBRARIES += cocos_ssl_static
LOCAL_STATIC_LIBRARIES += cocos_crypto_static
include $(PREBUILT_STATIC_LIBRARY)
