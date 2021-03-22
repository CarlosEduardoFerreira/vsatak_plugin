LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := MainBridgeCPP
LOCAL_SRC_FILES := libs
LOCAL_EXPORT_CPP_INCLUDES := src/main/cpp/main-bridge.cpp
include $(PREBUILT_SHARED_LIBRARY)

LOCAL_SHARED_LIBRARIES := main-bridge
include $(BUILD_SHARED_LIBRARY)