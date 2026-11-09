LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := test_wifi.c
LOCAL_MODULE := test_wifi
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -std=c99 -Wall
LOCAL_SHARED_LIBRARIES := libc
include $(BUILD_EXECUTABLE)
