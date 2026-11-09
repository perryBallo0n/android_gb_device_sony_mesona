LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := test_cam.c
LOCAL_MODULE := test_cam
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -std=c99 -Wall
LOCAL_SHARED_LIBRARIES := libc libm
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := test_cam_hw.cpp
LOCAL_MODULE := test_cam_hw
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := libc libutils libui libcamera_client libdl
LOCAL_C_INCLUDES += frameworks/base/include/camera
include $(BUILD_EXECUTABLE)
