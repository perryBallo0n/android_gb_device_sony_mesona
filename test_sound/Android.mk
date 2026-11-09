LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := test_sound.c
LOCAL_MODULE := test_sound
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -std=c99 -Wall
LOCAL_SHARED_LIBRARIES := libc libm
include $(BUILD_EXECUTABLE)
