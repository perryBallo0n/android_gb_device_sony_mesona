#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

typedef unsigned long long __u64;
typedef long long __s64;

#include <linux/videodev2.h>

int main(int argc, char **argv) {
    printf("1. Opening server /dev/video100...\n");
    int sfd = open("/dev/video100", O_RDWR);
    if (sfd < 0) {
        printf("Failed to open /dev/video100: %s (errno=%d)\n", strerror(errno), errno);
        return 1;
    }
    printf("Successfully opened server /dev/video100! fd=%d\n", sfd);

    printf("2. Opening camera /dev/video0...\n");
    int cfd = open("/dev/video0", O_RDWR);
    if (cfd < 0) {
        printf("Failed to open /dev/video0: %s (errno=%d)\n", strerror(errno), errno);
        close(sfd);
        return 1;
    }
    printf("Successfully opened camera /dev/video0! fd=%d\n", cfd);

    struct v4l2_capability cap;
    memset(&cap, 0, sizeof(cap));
    if (ioctl(cfd, VIDIOC_QUERYCAP, &cap) < 0) {
        printf("VIDIOC_QUERYCAP on /dev/video0 failed: %s (errno=%d)\n", strerror(errno), errno);
    } else {
        printf("Capabilities on /dev/video0:\n");
        printf("  Driver: %s\n  Card: %s\n  Bus info: %s\n  Version: %u.%u.%u\n  Capabilities: 0x%08x\n",
            cap.driver, cap.card, cap.bus_info,
            (cap.version >> 16) & 0xFF, (cap.version >> 8) & 0xFF, cap.version & 0xFF,
            cap.capabilities);
    }

    close(cfd);
    close(sfd);
    return 0;
}
