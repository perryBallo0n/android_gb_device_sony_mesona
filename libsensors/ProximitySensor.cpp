#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "ProximitySensor.h"

ProximitySensor::ProximitySensor()
    : SensorBase(NULL, "proximity"),
      mEnabled(0),
      mInputReader(4),
      mHasPendingEvent(false),
      input_sysfs_path_len(0)
{
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_P;
    mPendingEvent.type = SENSOR_TYPE_PROXIMITY;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    if (data_fd >= 0) {
        strcpy(input_sysfs_path, "/sys/class/input/");
        strcat(input_sysfs_path, input_name);
        strcat(input_sysfs_path, "/device/");
        input_sysfs_path_len = strlen(input_sysfs_path);
    }
}

ProximitySensor::~ProximitySensor() {
    if (mEnabled) {
        enable(0, 0);
    }
}

int ProximitySensor::setInitialState() {
    struct input_absinfo absinfo;
    if (data_fd >= 0 && !ioctl(data_fd, EVIOCGABS(ABS_X), &absinfo)) {
        mHasPendingEvent = true;
        mPendingEvent.distance = (absinfo.value == 0) ? 0.0f : 5.0f;
    } else {
        mHasPendingEvent = true;
        mPendingEvent.distance = 5.0f;
    }
    return 0;
}

int ProximitySensor::enable(int32_t, int en) {
    int flags = en ? 1 : 0;
    if (flags != mEnabled) {
        if (input_sysfs_path_len > 0) {
            strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
            int fd = open(input_sysfs_path, O_RDWR);
            if (fd >= 0) {
                write(fd, flags ? "1" : "0", 1);
                close(fd);
                mEnabled = flags;
                if (flags) {
                    setInitialState();
                }
                return 0;
            }
        }
        return -1;
    }
    return 0;
}

bool ProximitySensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

int ProximitySensor::readEvents(sensors_event_t* data, int count) {
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
        if (type == EV_ABS && event->code == ABS_X) {
            mPendingEvent.distance = (event->value == 0) ? 0.0f : 5.0f;
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
