LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := lanmo-rtc
LOCAL_SRC_FILES := ../../../BMGamingSDK/libs/Android/$(TARGET_ARCH_ABI)/libmaple-rtc-sdk-jni.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

#LOCAL_CPP_EXTENSION := .cxx .cpp .cc .mm

FILE_LIST := hellocpp/main.cpp

FILE_LIST += $(wildcard $(CLASSES_PATH)/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(CLASSES_PATH)
LOCAL_C_INCLUDES += ../../../cocos2d/cocos/platform/android/jni


LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_SHARED_LIBRARIES := lanmo-rtc


include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
#$(call import-module,libs/extensions)
#$(call import-module,external/Box2D)
#$(call import-module,external/chipmunk)
