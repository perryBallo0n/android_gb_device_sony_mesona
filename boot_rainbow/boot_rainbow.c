#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <cutils/properties.h>

int main(int argc, char **argv) {
    int fd;
    int h = 0;
    int test_mode;
    int test_count = 240;
    int step;
    char prop[PROPERTY_VALUE_MAX];
    char buf[256];

    fd = open("/sys/class/led/fih_led/control", O_WRONLY);
    if (fd < 0) {
        return 1;
    }

    test_mode = (argc > 1 && strcmp(argv[1], "--test") == 0);

    while (1) {
        int r = 0, g = 0, b = 0;
        int max_val = 500;
        int sector, rem, len;

        if (!test_mode) {
            property_get("sys.boot_completed", prop, "0");
            if (prop[0] == '1') {
                break;
            }
        } else {
            if (--test_count <= 0) break;
        }

        sector = h / 60;
        rem = (h % 60) * max_val / 60;

        switch (sector) {
            case 0: r = max_val; g = rem; b = 0; break;
            case 1: r = max_val - rem; g = max_val; b = 0; break;
            case 2: r = 0; g = max_val; b = rem; break;
            case 3: r = 0; g = max_val - rem; b = max_val; break;
            case 4: r = rem; g = 0; b = max_val; break;
            case 5: default: r = max_val; g = 0; b = max_val - rem; break;
        }

        len = snprintf(buf, sizeof(buf),
            "4 4 %d\n1 4 %d\n"
            "4 5 %d\n1 5 %d\n"
            "4 6 %d\n1 6 %d\n",
            r, r > 0 ? 1 : 0,
            g, g > 0 ? 1 : 0,
            b, b > 0 ? 1 : 0);

        write(fd, buf, len);

        h = (h + 3) % 360;
        usleep(35000);
    }

    // Smooth fade out to black
    for (step = 15; step >= 0; step--) {
        int r = 100 * step / 15;
        int g = 100 * step / 15;
        int b = 100 * step / 15;
        int len = snprintf(buf, sizeof(buf),
            "4 4 %d\n1 4 %d\n"
            "4 5 %d\n1 5 %d\n"
            "4 6 %d\n1 6 %d\n",
            r, r > 0 ? 1 : 0,
            g, g > 0 ? 1 : 0,
            b, b > 0 ? 1 : 0);
        write(fd, buf, len);
        usleep(25000);
    }

    {
        int len = snprintf(buf, sizeof(buf), "1 4 0\n1 5 0\n1 6 0\n");
        write(fd, buf, len);
    }
    close(fd);

    return 0;
}
