/* Qualcomm Camera Compatibility Shim for Android 2.3 Gingerbread */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/ioctl.h>
#include <utils/Log.h>
#include <camera/CameraHardwareInterface.h>

#define LOG_TAG "libcamera_shim"

extern "C" {

/* Genlock buffer stubs */
int genlock_lock_buffer(void* hnd, int timeout) { return 0; }
int genlock_unlock_buffer(void* hnd) { return 0; }

/* CameraParameters method stubs */
void _ZN7android16CameraParameters15setTouchIndexAfEii(void* self, int x, int y) {}
void _ZN7android16CameraParameters16setTouchIndexAecEii(void* self, int x, int y) {}
void _ZN7android16CameraParameters18setPreviewFpsRangeEii(void* self, int min, int max) {}
void _ZN7android16CameraParameters23setPreviewFrameRateModeEPKc(void* self, const char* m) {}
void _ZNK7android16CameraParameters18getFocusAreaCenterEPiS1_(void* self, int* x, int* y) { if (x) *x = 0; if (y) *y = 0; }
const char* _ZNK7android16CameraParameters23getPreviewFrameRateModeEv(void* self) { return "auto"; }

/* CameraParameters string constants */
const char _ZN7android16CameraParameters10DENOISE_ONE[] = "denoise-on";
const char _ZN7android16CameraParameters10HDR_ENABLEE[] = "hdr-enable";
const char _ZN7android16CameraParameters10MCE_ENABLEE[] = "mce-enable";
const char _ZN7android16CameraParameters11DENOISE_OFFE[] = "denoise-off";
const char _ZN7android16CameraParameters11HDR_DISABLEE[] = "hdr-disable";
const char _ZN7android16CameraParameters11KEY_DENOISEE[] = "denoise";
const char _ZN7android16CameraParameters11MCE_DISABLEE[] = "mce-disable";
const char _ZN7android16CameraParameters12KEY_CONTRASTE[] = "contrast";
const char _ZN7android16CameraParameters12KEY_ISO_MODEE[] = "iso";
const char _ZN7android16CameraParameters12VIDEO_HFR_2XE[] = "video-hfr-2x";
const char _ZN7android16CameraParameters12VIDEO_HFR_3XE[] = "video-hfr-3x";
const char _ZN7android16CameraParameters12VIDEO_HFR_4XE[] = "video-hfr-4x";
const char _ZN7android16CameraParameters13KEY_HISTOGRAME[] = "histogram";
const char _ZN7android16CameraParameters13KEY_LENSSHADEE[] = "lensshade";
const char _ZN7android16CameraParameters13KEY_SHARPNESSE[] = "sharpness";
const char _ZN7android16CameraParameters13SCENE_MODE_ARE[] = "scene-mode-ar";
const char _ZN7android16CameraParameters13VIDEO_HFR_OFFE[] = "video-hfr-off";
const char _ZN7android16CameraParameters14KEY_GPS_STATUSE[] = "gps-status";
const char _ZN7android16CameraParameters14KEY_SATURATIONE[] = "saturation";
const char _ZN7android16CameraParameters14KEY_VIDEO_SIZEE[] = "video-size";
const char _ZN7android16CameraParameters15KEY_CAMERA_MODEE[] = "camera-mode";
const char _ZN7android16CameraParameters15KEY_FOCUS_AREASE[] = "focus-areas";
const char _ZN7android16CameraParameters15SCENE_DETECT_ONE[] = "scene-detect-on";
const char _ZN7android16CameraParameters15SEMC_METRY_SPOTE[] = "spot";
const char _ZN7android16CameraParameters15TOUCH_AF_AEC_ONE[] = "touch-af-aec-on";
const char _ZN7android16CameraParameters16HISTOGRAM_ENABLEE[] = "histogram-enable";
const char _ZN7android16CameraParameters16KEY_MAX_CONTRASTE[] = "max-contrast";
const char _ZN7android16CameraParameters16KEY_SCENE_DETECTE[] = "scene-detect";
const char _ZN7android16CameraParameters16KEY_TOUCH_AF_AECE[] = "touch-af-aec";
const char _ZN7android16CameraParameters16LENSSHADE_ENABLEE[] = "lensshade-enable";
const char _ZN7android16CameraParameters16PIXEL_FORMAT_RAWE[] = "raw";
const char _ZN7android16CameraParameters16SCENE_DETECT_OFFE[] = "scene-detect-off";
const char _ZN7android16CameraParameters16SEMC_METRY_FRAMEE[] = "frame";
const char _ZN7android16CameraParameters16TOUCH_AF_AEC_OFFE[] = "touch-af-aec-off";
const char _ZN7android16CameraParameters17FACE_DETECTION_ONE[] = "face-detection-on";
const char _ZN7android16CameraParameters17HISTOGRAM_DISABLEE[] = "histogram-disable";
const char _ZN7android16CameraParameters17KEY_AUTO_EXPOSUREE[] = "auto-exposure";
const char _ZN7android16CameraParameters17KEY_EXIF_DATETIMEE[] = "exif-datetime";
const char _ZN7android16CameraParameters17KEY_MAX_SHARPNESSE[] = "max-sharpness";
const char _ZN7android16CameraParameters17LENSSHADE_DISABLEE[] = "lensshade-disable";
const char _ZN7android16CameraParameters17PIXEL_FORMAT_YV12E[] = "yv12";
const char _ZN7android16CameraParameters17SEMC_METRY_CENTERE[] = "center";
const char _ZN7android16CameraParameters18FACE_DETECTION_OFFE[] = "face-detection-off";
const char _ZN7android16CameraParameters18KEY_FACE_DETECTIONE[] = "face-detection";
const char _ZN7android16CameraParameters18KEY_MAX_SATURATIONE[] = "max-saturation";
const char _ZN7android16CameraParameters18KEY_METERING_AREASE[] = "metering-areas";
const char _ZN7android16CameraParameters18KEY_RECORDING_HINTE[] = "recording-hint";
const char _ZN7android16CameraParameters18SCENE_MODE_FLOWERSE[] = "flowers";
const char _ZN7android16CameraParameters19KEY_SEMC_METRY_MODEE[] = "semc-metry-mode";
const char _ZN7android16CameraParameters20KEY_GPS_ALTITUDE_REFE[] = "gps-altitude-ref";
const char _ZN7android16CameraParameters20KEY_GPS_LATITUDE_REFE[] = "gps-latitude-ref";
const char _ZN7android16CameraParameters20KEY_REDEYE_REDUCTIONE[] = "redeye-reduction";
const char _ZN7android16CameraParameters20PIXEL_FORMAT_YUV420PE[] = "yuv420p";
const char _ZN7android16CameraParameters20SCENE_MODE_BACKLIGHTE[] = "backlight";
const char _ZN7android16CameraParameters21KEY_GPS_LONGITUDE_REFE[] = "gps-longitude-ref";
const char _ZN7android16CameraParameters22EX_SCENE_MODE_DOCUMENTE[] = "document";
const char _ZN7android16CameraParameters22KEY_SELECTABLE_ZONE_AFE[] = "selectable-zone-af";
const char _ZN7android16CameraParameters23AUTO_EXPOSURE_FRAME_AVGE[] = "frame-average";
const char _ZN7android16CameraParameters23KEY_MAX_NUM_FOCUS_AREASE[] = "max-num-focus-areas";
const char _ZN7android16CameraParameters23KEY_SUPPORTED_HFR_SIZESE[] = "supported-hfr-sizes";
const char _ZN7android16CameraParameters23KEY_SUPPORTED_ISO_MODESE[] = "supported-iso-modes";
const char _ZN7android16CameraParameters23KEY_SUPPORTED_ZSL_MODESE[] = "supported-zsl-modes";
const char _ZN7android16CameraParameters23REDEYE_REDUCTION_ENABLEE[] = "enable";
const char _ZN7android16CameraParameters23SELECTABLE_ZONE_AF_AUTOE[] = "auto";
const char _ZN7android16CameraParameters24REDEYE_REDUCTION_DISABLEE[] = "disable";
const char _ZN7android16CameraParameters25KEY_SKIN_TONE_ENHANCEMENTE[] = "skinToneEnhancement";
const char _ZN7android16CameraParameters25KEY_SUPPORTED_VIDEO_SIZESE[] = "video-size-values";
const char _ZN7android16CameraParameters25KEY_VIDEO_HIGH_FRAME_RATEE[] = "video-hfr";
const char _ZN7android16CameraParameters26KEY_MAX_NUM_METERING_AREASE[] = "max-num-metering-areas";
const char _ZN7android16CameraParameters26KEY_SUPPORTED_TOUCH_AF_AECE[] = "supported-touch-af-aec";
const char _ZN7android16CameraParameters27AUTO_EXPOSURE_SPOT_METERINGE[] = "spot-metering";
const char _ZN7android16CameraParameters27KEY_SUPPORTED_AUTO_EXPOSUREE[] = "auto-exposure-values";
const char _ZN7android16CameraParameters28KEY_MEMORY_COLOR_ENHANCEMENTE[] = "mce";
const char _ZN7android16CameraParameters28PIXEL_FORMAT_YUV420SP_ADRENOE[] = "yuv420sp-adreno";
const char _ZN7android16CameraParameters28SKIN_TONE_ENHANCEMENT_ENABLEE[] = "enable";
const char _ZN7android16CameraParameters29AUTO_EXPOSURE_CENTER_WEIGHTEDE[] = "center-weighted";
const char _ZN7android16CameraParameters29FOCUS_MODE_CONTINUOUS_PICTUREE[] = "continuous-picture";
const char _ZN7android16CameraParameters29KEY_SUPPORTED_HISTOGRAM_MODESE[] = "histogram-values";
const char _ZN7android16CameraParameters29KEY_SUPPORTED_LENSSHADE_MODESE[] = "lensshade-values";
const char _ZN7android16CameraParameters29SKIN_TONE_ENHANCEMENT_DISABLEE[] = "disable";
const char _ZN7android16CameraParameters30KEY_HIGH_DYNAMIC_RANGE_IMAGINGE[] = "hdr";
const char _ZN7android16CameraParameters31KEY_EX_SUPPORTED_METERING_MODESE[] = "metering-values";
const char _ZN7android16CameraParameters31KEY_SUPPORTED_HDR_IMAGING_MODESE[] = "hdr-values";
const char _ZN7android16CameraParameters32KEY_PREVIEW_FRAME_RATE_AUTO_MODEE[] = "auto";
const char _ZN7android16CameraParameters32SELECTABLE_ZONE_AF_FRAME_AVERAGEE[] = "frame-average";
const char _ZN7android16CameraParameters32SELECTABLE_ZONE_AF_SPOT_METERINGE[] = "spot-metering";
const char _ZN7android16CameraParameters33KEY_PREVIEW_FRAME_RATE_FIXED_MODEE[] = "fixed";
const char _ZN7android16CameraParameters34SELECTABLE_ZONE_AF_CENTER_WEIGHTEDE[] = "center-weighted";
const char _ZN7android16CameraParameters36KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEOE[] = "preferred-preview-size-for-video";
const char _ZN7android16CameraParameters37KEY_SUPPORTED_MEM_COLOR_ENHANCE_MODESE[] = "mce-values";
const char _ZN7android16CameraParameters6ZSL_ONE[] = "on";
const char _ZN7android16CameraParameters7ISO_100E[] = "ISO100";
const char _ZN7android16CameraParameters7ISO_200E[] = "ISO200";
const char _ZN7android16CameraParameters7ISO_400E[] = "ISO400";
const char _ZN7android16CameraParameters7ISO_800E[] = "ISO800";
const char _ZN7android16CameraParameters7ISO_HJRE[] = "ISO_HJR";
const char _ZN7android16CameraParameters7KEY_ZSLE[] = "zsl";
const char _ZN7android16CameraParameters7ZSL_OFFE[] = "off";
const char _ZN7android16CameraParameters8ISO_1600E[] = "ISO1600";
const char _ZN7android16CameraParameters8ISO_AUTOE[] = "auto";

/* IOCTL Interceptor for Qualcomm Camera Server */
static int (*s_real_ioctl)(int, int, ...) = NULL;
int ioctl(int fd, int request, ...) {
    void* arg = NULL;
    va_list ap;
    va_start(ap, request);
    arg = va_arg(ap, void*);
    va_end(ap);

    if (!s_real_ioctl) {
        void* libc = dlopen("libc.so", RTLD_NOW);
        s_real_ioctl = (int (*)(int, int, ...))dlsym(libc, "ioctl");
    }

    if ((unsigned int)request == 0x80046d24) { /* MSM_CAM_IOCTL_GET_CAMERA_INFO */
        LOGI("ioctl hook: intercepted MSM_CAM_IOCTL_GET_CAMERA_INFO (0x80046d24)!");
        struct cam_info {
            int num_cameras;
            uint8_t has_3d_support[5];
            uint8_t is_internal_cam[5];
            uint8_t pad[2];
            uint32_t s_mount_angle[5];
            const char *video_dev_name[5];
            int sensor_type[5];
            int hw_version[5];
        };
        struct cam_info *info = (struct cam_info *)arg;
        if (info) {
            memset(info, 0, sizeof(*info));
            info->num_cameras = 1;
            info->has_3d_support[0] = 0;
            info->is_internal_cam[0] = 0; /* back camera */
            info->s_mount_angle[0] = 90;
            info->video_dev_name[0] = "/dev/video0";
            LOGI("ioctl hook: returned num_cameras=1, mount_angle=90");
            return 0;
        }
    }
    return s_real_ioctl(fd, request, arg);
}

/* Forwarding to Qualcomm camera.msm7x27a.so */
/* Note: Qualcomm HAL_openCameraHardware returns raw CameraHardwareInterface* in r0, takes cameraId in r0 */
typedef android::CameraHardwareInterface* (*qcom_open_raw_t)(int);
typedef void (*qcom_info_t)(int, struct android::CameraInfo*);
typedef int (*qcom_num_t)();

static void* g_hal_handle = NULL;
static qcom_open_raw_t g_open_fn = NULL;
static qcom_info_t g_info_fn = NULL;
static qcom_num_t g_num_fn = NULL;

static void ensure_hal_loaded() {
    if (g_hal_handle) return;
    LOGI("Loading /system/lib/hw/camera.msm7x27a.so ...");
    g_hal_handle = dlopen("/system/lib/hw/camera.msm7x27a.so", RTLD_NOW | RTLD_GLOBAL);
    if (!g_hal_handle) {
        LOGE("Failed to dlopen /system/lib/hw/camera.msm7x27a.so: %s", dlerror());
        return;
    }
    g_open_fn = (qcom_open_raw_t)dlsym(g_hal_handle, "HAL_openCameraHardware");
    g_info_fn = (qcom_info_t)dlsym(g_hal_handle, "HAL_getCameraInfo");
    g_num_fn = (qcom_num_t)dlsym(g_hal_handle, "HAL_getNumberOfCameras");
    LOGI("Qualcomm HAL loaded! open=%p, info=%p, num=%p", g_open_fn, g_info_fn, g_num_fn);
}

int HAL_getNumberOfCameras() {
    ensure_hal_loaded();
    if (g_num_fn) {
        int n = g_num_fn();
        if (n > 0) return n;
    }
    return 1;
}

void HAL_getCameraInfo(int cameraId, struct android::CameraInfo* cameraInfo) {
    if (cameraInfo) {
        cameraInfo->facing = 0; /* CAMERA_FACING_BACK */
        cameraInfo->orientation = 90;
    }
    ensure_hal_loaded();
    if (g_info_fn && cameraInfo) {
        struct android::CameraInfo temp;
        temp.facing = 0;
        temp.orientation = 90;
        g_info_fn(cameraId, &temp);
        if (temp.orientation != 0) {
            *cameraInfo = temp;
        }
    }
}

android::sp<android::CameraHardwareInterface> HAL_openCameraHardware(int cameraId) {
    ensure_hal_loaded();
    if (g_open_fn) {
        LOGI("Forwarding HAL_openCameraHardware(%d) to Qualcomm HAL (raw ptr call)", cameraId);
        android::CameraHardwareInterface* raw_hw = g_open_fn(cameraId);
        LOGI("Qualcomm HAL returned raw pointer: %p", raw_hw);
        if (raw_hw) {
            return android::sp<android::CameraHardwareInterface>(raw_hw);
        }
        LOGE("Qualcomm HAL returned NULL pointer!");
        return NULL;
    }
    LOGE("g_open_fn is NULL! Returning empty sp");
    return NULL;
}

} /* extern "C" */
