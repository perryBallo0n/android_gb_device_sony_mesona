LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sensors.msm7x27a
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -DLOG_TAG=\"Sensors\"
LOCAL_SRC_FILES := \
    sensors.cpp \
    SensorBase.cpp \
    AccelSensor.cpp \
    ProximitySensor.cpp \
    InputEventReader.cpp

LOCAL_SHARED_LIBRARIES := liblog libcutils libdl
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
