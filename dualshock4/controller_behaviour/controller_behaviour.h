#ifndef CONTROLLER_BEHAVIOUR_H
#define CONTROLLER_BEHAVIOUR_H

#include <btstack_port_esp32.h>
#include "sdkconfig.h" // We need to see CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#include "uni_hid_device.h"
#include "platform/uni_platform.h"

// Only custom platform is allowed
#ifndef CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#error "DS4 library requires Bluepad32 platform set to CUSTOM"
#endif

#ifndef CONFIG_BT_ENABLED
#error "DS4 library requires Bluetooth to be enabled"
#endif

#define DS4_DEVICE_IDX 0 // Only one device at any time allowed so the ID

// Types
typedef struct uni_platform ds4_platform;
typedef struct uni_platform *ds4_platform_handle;
typedef uni_hid_device_t ds4_device;
typedef uni_hid_device_t *ds4_device_handle;
typedef struct platform_instance_s
{
    uni_gamepad_seat_t gamepad_seat; // which "seat" is being used
} platform_instance_t;

// Function prototypes
ds4_platform_handle get_ds4_platform(void);

#endif // CONTROLLER_BEHAVIOUR_H
