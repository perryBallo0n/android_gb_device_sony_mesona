#ifndef ANDROID_SENSORS_H
#define ANDROID_SENSORS_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/input.h>
#include <hardware/hardware.h>
#include <hardware/sensors.h>

__BEGIN_DECLS

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define ID_A (0)
#define ID_P (1)

__END_DECLS

#endif
