LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := bullet_static
LOCAL_MODULE_FILENAME := bullet_static
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libLinearMath.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../include/bullet
LOCAL_STATIC_LIBRARIES += cocos_bulletcollision_static
LOCAL_STATIC_LIBRARIES += cocos_bulletdynamics_static
LOCAL_STATIC_LIBRARIES += cocos_bulletmultithreaded_static
# LOCAL_STATIC_LIBRARIES += cocos_linearmath_static
LOCAL_STATIC_LIBRARIES += cocos_minicl_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_bulletcollision_static
LOCAL_MODULE_FILENAME := bulletcollision
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libBulletCollision.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_STATIC_LIBRARIES += cocos_linearmath_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_bulletdynamics_static
LOCAL_MODULE_FILENAME := bulletdynamics
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libBulletDynamics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_STATIC_LIBRARIES += cocos_bulletcollision_static
LOCAL_STATIC_LIBRARIES += cocos_linearmath_static
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_bulletmultithreaded_static
LOCAL_MODULE_FILENAME := bulletmultithreaded
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libBulletMultiThreaded.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_linearmath_static
LOCAL_MODULE_FILENAME := linearmath
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libLinearMath.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_minicl_static
LOCAL_MODULE_FILENAME := minicl
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libMiniCL.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)
