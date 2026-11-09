LOCAL_PATH := $(call my-dir)

# -------------------------------------------------------------
# libgenlock (includes Qualcomm Camera compatibility symbols)
# -------------------------------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE := libgenlock
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES := camera_compat_symbols.c
LOCAL_SHARED_LIBRARIES := libc
include $(BUILD_SHARED_LIBRARY)

# -------------------------------------------------------------
# libcamera (Bridge to camera.msm7x27a.so)
# -------------------------------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE := libcamera
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES := libcamera_shim.cpp
LOCAL_SHARED_LIBRARIES := \
    libutils \
    libui \
    libcamera_client \
    libcutils \
    liblog \
    libdl \
    libgenlock

LOCAL_C_INCLUDES += \
    frameworks/base/include/camera

include $(BUILD_SHARED_LIBRARY)
