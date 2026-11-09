LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := boot_rainbow
LOCAL_SRC_FILES := boot_rainbow.c
LOCAL_SHARED_LIBRARIES := libcutils libc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
include $(BUILD_EXECUTABLE)
