$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full.mk)
# Vendor blobs
$(call inherit-product-if-exists, vendor/sony/mesona/mesona-vendor.mk)
DEVICE_PACKAGE_OVERLAYS += device/sony/mesona/overlay
PRODUCT_NAME := full_mesona
PRODUCT_DEVICE := mesona
PRODUCT_BRAND := Sony
PRODUCT_MODEL := Xperia Miro
PRODUCT_MANUFACTURER := Sony
PRODUCT_LOCALES += hdpi mdpi
PRODUCT_AAPT_CONFIG := normal hdpi mdpi nodpi
PRODUCT_AAPT_PREF_CONFIG := mdpi
# Display
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.lcd_density=160 \
    ro.sf.hwrotation=0
# Memory tuning for 512MB RAM
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapsize=48m \
    dalvik.vm.heapgrowthlimit=32m \
    ro.config.low_ram=true
# Wi-Fi
PRODUCT_COPY_FILES += \
    device/sony/mesona/prebuilt/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf

# Key layouts
PRODUCT_COPY_FILES += \
    device/sony/mesona/prebuilt/cyttsp_i2c_tma340.kl:system/usr/keylayout/cyttsp_i2c_tma340.kl
# Permissions
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/base/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/base/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml
PRODUCT_COPY_FILES += device/sony/mesona/kernel:kernel

# Root init and ueventd files
PRODUCT_COPY_FILES +=     device/sony/mesona/init.qcom.rc:root/init.qcom.rc     device/sony/mesona/init.qcom.rc:root/init.mesona.rc     device/sony/mesona/ueventd.qcom.rc:root/ueventd.qcom.rc     device/sony/mesona/ueventd.qcom.rc:root/ueventd.mesona.rc device/sony/mesona/ueventd.qcom.rc:root/ueventd.qct.rc

# Root debugging
ADDITIONAL_DEFAULT_PROPERTIES += \
    ro.secure=0 \
    ro.allow.mock.location=1 \
    persist.service.adb.enable=1
PRODUCT_PACKAGES += gralloc.msm7x27a

PRODUCT_COPY_FILES +=     device/sony/mesona/adreno_config.txt:root/adreno_config.txt     device/sony/mesona/adreno_config.txt:system/etc/adreno_config.txt

# Vold config
PRODUCT_COPY_FILES += device/sony/mesona/vold.fstab:system/etc/vold.fstab

PRODUCT_COPY_FILES += device/sony/mesona/prebuilt/rmt_storage:system/bin/rmt_storage

PRODUCT_PACKAGES += lights.msm7x27a boot_rainbow
PRODUCT_COPY_FILES += device/sony/mesona/prebuilt/initlogo.rle:root/initlogo.rle
PRODUCT_COPY_FILES += device/sony/mesona/prebuilt/initlogo.rle:root/logo.rle

# Live Wallpapers & RenderScript
PRODUCT_PACKAGES += \
    LiveWallpapers \
    LiveWallpapersPicker \
    MagicSmokeWallpapers \
    VisualizationWallpapers \
    librs_jni \
    libRS

PRODUCT_COPY_FILES += \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml

# Sensors
PRODUCT_PACKAGES +=     sensors.msm7x27a

# GPS & RPC
PRODUCT_PACKAGES +=     gps.msm7x27a     librpc

PRODUCT_COPY_FILES +=     device/common/gps/gps.conf_US_SUPL:system/etc/gps.conf
