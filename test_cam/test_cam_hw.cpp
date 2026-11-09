#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <camera/CameraHardwareInterface.h>

using namespace android;

typedef sp<CameraHardwareInterface> (*open_fn_t)(int);
typedef int (*num_fn_t)();
typedef void (*info_fn_t)(int, struct CameraInfo*);

int main(int argc, char** argv) {
    printf("Opening /system/lib/libcamera.so ...\n");
    void* handle = dlopen("/system/lib/libcamera.so", RTLD_NOW);
    if (!handle) {
        printf("dlopen error: %s\n", dlerror());
        return 1;
    }

    num_fn_t num_fn = (num_fn_t)dlsym(handle, "HAL_getNumberOfCameras");
    if (num_fn) {
        printf("HAL_getNumberOfCameras returned: %d\n", num_fn());
    }

    info_fn_t info_fn = (info_fn_t)dlsym(handle, "HAL_getCameraInfo");
    if (info_fn) {
        struct CameraInfo info;
        info.facing = -1;
        info.orientation = -1;
        info_fn(0, &info);
        printf("HAL_getCameraInfo(0): facing=%d, orientation=%d\n", info.facing, info.orientation);
    }

    open_fn_t open_fn = (open_fn_t)dlsym(handle, "HAL_openCameraHardware");
    if (!open_fn) {
        printf("Failed to find HAL_openCameraHardware: %s\n", dlerror());
        return 1;
    }

    printf("Calling HAL_openCameraHardware(0)...\n");
    sp<CameraHardwareInterface> hw = open_fn(0);
    if (hw == NULL) {
        printf("HAL_openCameraHardware(0) returned NULL!\n");
        return 1;
    }
    printf("SUCCESS! Got CameraHardwareInterface pointer: %p\n", hw.get());

    printf("Getting parameters...\n");
    CameraParameters params = hw->getParameters();
    const char* str = params.flatten().string();
    printf("Parameters:\n%s\n", str ? str : "(null)");

    return 0;
}
