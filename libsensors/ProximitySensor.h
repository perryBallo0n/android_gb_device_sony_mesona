#ifndef ANDROID_PROXIMITY_SENSOR_H
#define ANDROID_PROXIMITY_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "sensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

struct input_event;

class ProximitySensor : public SensorBase {
    int mEnabled;
    InputEventCircularReader mInputReader;
    sensors_event_t mPendingEvent;
    bool mHasPendingEvent;
    char input_sysfs_path[PATH_MAX];
    int input_sysfs_path_len;

    int setInitialState();

public:
            ProximitySensor();
    virtual ~ProximitySensor();

    virtual int enable(int32_t handle, int enabled);
    virtual bool hasPendingEvents() const;
    virtual int readEvents(sensors_event_t* data, int count);
};

#endif
