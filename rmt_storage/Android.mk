LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := rmt_storage.c
LOCAL_MODULE := rmt_storage
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -std=c99 -Wall
LOCAL_SHARED_LIBRARIES := libc libcutils
include $(BUILD_EXECUTABLE)
