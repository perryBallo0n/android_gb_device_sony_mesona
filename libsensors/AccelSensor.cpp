#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "AccelSensor.h"

#define RANGE_A (2.0f * GRAVITY_EARTH)
#define CONVERT_A (GRAVITY_EARTH / 256.0f)

AccelSensor::AccelSensor()
    : SensorBase(NULL, "accelerometer"),
      mEnabled(0),
      mInputReader(4),
      mHasPendingEvent(false),
      input_sysfs_path_len(0)
{
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_A;
    mPendingEvent.type = SENSOR_TYPE_ACCELEROMETER;
    mPendingEvent.acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    if (data_fd >= 0) {
        strcpy(input_sysfs_path, "/sys/class/input/");
        strcat(input_sysfs_path, input_name);
        strcat(input_sysfs_path, "/device/");
        input_sysfs_path_len = strlen(input_sysfs_path);
    }
}

AccelSensor::~AccelSensor() {
    if (mEnabled) {
        enable(0, 0);
    }
}

int AccelSensor::enable(int32_t, int en) {
    int flags = en ? 1 : 0;
    if (flags != mEnabled) {
        if (input_sysfs_path_len > 0) {
            strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
            int fd = open(input_sysfs_path, O_RDWR);
            if (fd >= 0) {
                write(fd, flags ? "1" : "0", 1);
                close(fd);
                mEnabled = flags;
                return 0;
            }
        }
        return -1;
    }
    return 0;
}

int AccelSensor::setDelay(int32_t, int64_t ns) {
    int ms = ns / 1000000;
    if (ms < 10) ms = 10;
    if (input_sysfs_path_len > 0) {
        strcpy(&input_sysfs_path[input_sysfs_path_len], "delay");
        int fd = open(input_sysfs_path, O_RDWR);
        if (fd >= 0) {
            char buf[16];
            int len = sprintf(buf, "%d", ms);
            write(fd, buf, len);
            close(fd);
            return 0;
        }
    }
    return -1;
}

bool AccelSensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

int AccelSensor::readEvents(sensors_event_t* data, int count) {
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent) {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled ? 1 : 0;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_ABS) {
            if (event->code == ABS_X) {
                mPendingEvent.acceleration.x = event->value * CONVERT_A;
            } else if (event->code == ABS_Y) {
                mPendingEvent.acceleration.y = event->value * CONVERT_A;
            } else if (event->code == ABS_Z) {
                mPendingEvent.acceleration.z = event->value * CONVERT_A;
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        }
        mInputReader.next();
    }

    return numEventReceived;
}
