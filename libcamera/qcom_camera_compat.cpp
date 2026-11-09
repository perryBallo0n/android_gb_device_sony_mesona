#include <camera/CameraParameters.h>
#include <stdio.h>
#include <string.h>

extern "C" {
int genlock_lock_buffer(void* hnd, int timeout) { return 0; }
int genlock_unlock_buffer(void* hnd) { return 0; }
}

namespace android {
const char CameraParameters::DENOISE_ON[] = "denoise-on";
const char CameraParameters::HDR_ENABLE[] = "hdr-enable";
const char CameraParameters::MCE_ENABLE[] = "mce-enable";
const char CameraParameters::DENOISE_OFF[] = "denoise-off";
const char CameraParameters::HDR_DISABLE[] = "hdr-disable";
const char CameraParameters::KEY_DENOISE[] = "key-denoise";
const char CameraParameters::MCE_DISABLE[] = "mce-disable";
const char CameraParameters::KEY_CONTRAST[] = "key-contrast";
const char CameraParameters::KEY_ISO_MODE[] = "key-iso-mode";
const char CameraParameters::VIDEO_HFR_2X[] = "video-hfr-2x";
const char CameraParameters::VIDEO_HFR_3X[] = "video-hfr-3x";
const char CameraParameters::VIDEO_HFR_4X[] = "video-hfr-4x";
const char CameraParameters::KEY_HISTOGRAM[] = "key-histogram";
const char CameraParameters::KEY_LENSSHADE[] = "key-lensshade";
const char CameraParameters::KEY_SHARPNESS[] = "key-sharpness";
const char CameraParameters::SCENE_MODE_AR[] = "scene-mode-ar";
const char CameraParameters::VIDEO_HFR_OFF[] = "video-hfr-off";
const char CameraParameters::KEY_GPS_STATUS[] = "key-gps-status";
const char CameraParameters::KEY_SATURATION[] = "key-saturation";
const char CameraParameters::KEY_VIDEO_SIZE[] = "key-video-size";
const char CameraParameters::KEY_CAMERA_MODE[] = "key-camera-mode";
const char CameraParameters::KEY_FOCUS_AREAS[] = "key-focus-areas";
const char CameraParameters::SCENE_DETECT_ON[] = "scene-detect-on";
const char CameraParameters::SEMC_METRY_SPOT[] = "semc-metry-spot";
const char CameraParameters::TOUCH_AF_AEC_ON[] = "touch-af-aec-on";
void CameraParameters::setTouchIndexAf(int x, int y) { char b[32]; snprintf(b, sizeof(b), "%dx%d", x, y); set("touch-af", b); }
const char CameraParameters::HISTOGRAM_ENABLE[] = "histogram-enable";
const char CameraParameters::KEY_MAX_CONTRAST[] = "key-max-contrast";
const char CameraParameters::KEY_SCENE_DETECT[] = "key-scene-detect";
const char CameraParameters::KEY_TOUCH_AF_AEC[] = "key-touch-af-aec";
const char CameraParameters::LENSSHADE_ENABLE[] = "lensshade-enable";
const char CameraParameters::PIXEL_FORMAT_RAW[] = "pixel-format-raw";
const char CameraParameters::SCENE_DETECT_OFF[] = "scene-detect-off";
const char CameraParameters::SEMC_METRY_FRAME[] = "semc-metry-frame";
const char CameraParameters::TOUCH_AF_AEC_OFF[] = "touch-af-aec-off";
void CameraParameters::setTouchIndexAec(int x, int y) { char b[32]; snprintf(b, sizeof(b), "%dx%d", x, y); set("touch-aec", b); }
const char CameraParameters::FACE_DETECTION_ON[] = "face-detection-on";
const char CameraParameters::HISTOGRAM_DISABLE[] = "histogram-disable";
const char CameraParameters::KEY_AUTO_EXPOSURE[] = "key-auto-exposure";
const char CameraParameters::KEY_EXIF_DATETIME[] = "key-exif-datetime";
const char CameraParameters::KEY_MAX_SHARPNESS[] = "key-max-sharpness";
const char CameraParameters::LENSSHADE_DISABLE[] = "lensshade-disable";
const char CameraParameters::PIXEL_FORMAT_YV12[] = "pixel-format-yv12";
const char CameraParameters::SEMC_METRY_CENTER[] = "semc-metry-center";
const char CameraParameters::FACE_DETECTION_OFF[] = "face-detection-off";
const char CameraParameters::KEY_FACE_DETECTION[] = "key-face-detection";
const char CameraParameters::KEY_MAX_SATURATION[] = "key-max-saturation";
const char CameraParameters::KEY_METERING_AREAS[] = "key-metering-areas";
const char CameraParameters::KEY_RECORDING_HINT[] = "key-recording-hint";
const char CameraParameters::SCENE_MODE_FLOWERS[] = "scene-mode-flowers";
void CameraParameters::setPreviewFpsRange(int min, int max) { char b[32]; snprintf(b, sizeof(b), "%d,%d", min, max); set(KEY_PREVIEW_FPS_RANGE, b); }
const char CameraParameters::KEY_SEMC_METRY_MODE[] = "key-semc-metry-mode";
const char CameraParameters::KEY_GPS_ALTITUDE_REF[] = "key-gps-altitude-ref";
const char CameraParameters::KEY_GPS_LATITUDE_REF[] = "key-gps-latitude-ref";
const char CameraParameters::KEY_REDEYE_REDUCTION[] = "key-redeye-reduction";
const char CameraParameters::PIXEL_FORMAT_YUV420P[] = "pixel-format-yuv420p";
const char CameraParameters::SCENE_MODE_BACKLIGHT[] = "scene-mode-backlight";
const char CameraParameters::KEY_GPS_LONGITUDE_REF[] = "key-gps-longitude-ref";
const char CameraParameters::EX_SCENE_MODE_DOCUMENT[] = "ex-scene-mode-document";
const char CameraParameters::KEY_SELECTABLE_ZONE_AF[] = "key-selectable-zone-af";
const char CameraParameters::AUTO_EXPOSURE_FRAME_AVG[] = "auto-exposure-frame-avg";
const char CameraParameters::KEY_MAX_NUM_FOCUS_AREAS[] = "key-max-num-focus-areas";
const char CameraParameters::KEY_SUPPORTED_HFR_SIZES[] = "key-supported-hfr-sizes";
const char CameraParameters::KEY_SUPPORTED_ISO_MODES[] = "key-supported-iso-modes";
const char CameraParameters::KEY_SUPPORTED_ZSL_MODES[] = "key-supported-zsl-modes";
const char CameraParameters::REDEYE_REDUCTION_ENABLE[] = "redeye-reduction-enable";
const char CameraParameters::SELECTABLE_ZONE_AF_AUTO[] = "selectable-zone-af-auto";
void CameraParameters::setPreviewFrameRateMode(const char *m) { set("preview-frame-rate-mode", m); }
const char CameraParameters::REDEYE_REDUCTION_DISABLE[] = "redeye-reduction-disable";
const char CameraParameters::KEY_SKIN_TONE_ENHANCEMENT[] = "key-skin-tone-enhancement";
const char CameraParameters::KEY_SUPPORTED_VIDEO_SIZES[] = "key-supported-video-sizes";
const char CameraParameters::KEY_VIDEO_HIGH_FRAME_RATE[] = "key-video-high-frame-rate";
const char CameraParameters::KEY_MAX_NUM_METERING_AREAS[] = "key-max-num-metering-areas";
const char CameraParameters::KEY_SUPPORTED_TOUCH_AF_AEC[] = "key-supported-touch-af-aec";
const char CameraParameters::AUTO_EXPOSURE_SPOT_METERING[] = "auto-exposure-spot-metering";
const char CameraParameters::KEY_SUPPORTED_AUTO_EXPOSURE[] = "key-supported-auto-exposure";
const char CameraParameters::KEY_MEMORY_COLOR_ENHANCEMENT[] = "key-memory-color-enhancement";
const char CameraParameters::PIXEL_FORMAT_YUV420SP_ADRENO[] = "pixel-format-yuv420sp-adreno";
const char CameraParameters::SKIN_TONE_ENHANCEMENT_ENABLE[] = "skin-tone-enhancement-enable";
const char CameraParameters::AUTO_EXPOSURE_CENTER_WEIGHTED[] = "auto-exposure-center-weighted";
const char CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE[] = "focus-mode-continuous-picture";
const char CameraParameters::KEY_SUPPORTED_HISTOGRAM_MODES[] = "key-supported-histogram-modes";
const char CameraParameters::KEY_SUPPORTED_LENSSHADE_MODES[] = "key-supported-lensshade-modes";
const char CameraParameters::SKIN_TONE_ENHANCEMENT_DISABLE[] = "skin-tone-enhancement-disable";
const char CameraParameters::KEY_HIGH_DYNAMIC_RANGE_IMAGING[] = "key-high-dynamic-range-imaging";
const char CameraParameters::KEY_EX_SUPPORTED_METERING_MODES[] = "key-ex-supported-metering-modes";
const char CameraParameters::KEY_SUPPORTED_HDR_IMAGING_MODES[] = "key-supported-hdr-imaging-modes";
const char CameraParameters::KEY_PREVIEW_FRAME_RATE_AUTO_MODE[] = "key-preview-frame-rate-auto-mode";
const char CameraParameters::SELECTABLE_ZONE_AF_FRAME_AVERAGE[] = "selectable-zone-af-frame-average";
const char CameraParameters::SELECTABLE_ZONE_AF_SPOT_METERING[] = "selectable-zone-af-spot-metering";
const char CameraParameters::KEY_PREVIEW_FRAME_RATE_FIXED_MODE[] = "key-preview-frame-rate-fixed-mode";
const char CameraParameters::SELECTABLE_ZONE_AF_CENTER_WEIGHTED[] = "selectable-zone-af-center-weighted";
const char CameraParameters::KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO[] = "key-preferred-preview-size-for-video";
const char CameraParameters::KEY_SUPPORTED_MEM_COLOR_ENHANCE_MODES[] = "key-supported-mem-color-enhance-modes";
const char CameraParameters::ZSL_ON[] = "zsl-on";
const char CameraParameters::ISO_100[] = "iso-100";
const char CameraParameters::ISO_200[] = "iso-200";
const char CameraParameters::ISO_400[] = "iso-400";
const char CameraParameters::ISO_800[] = "iso-800";
const char CameraParameters::ISO_HJR[] = "iso-hjr";
const char CameraParameters::KEY_ZSL[] = "key-zsl";
const char CameraParameters::ZSL_OFF[] = "zsl-off";
const char CameraParameters::ISO_1600[] = "iso-1600";
const char CameraParameters::ISO_AUTO[] = "iso-auto";
void CameraParameters::getFocusAreaCenter(int *x, int *y) const { if (x) *x = 0; if (y) *y = 0; }
const char *CameraParameters::getPreviewFrameRateMode() const { return get("preview-frame-rate-mode"); }
} // namespace android
