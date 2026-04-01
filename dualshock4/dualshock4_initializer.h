#ifndef DUALSHOCK4_INITIALIZER_H
#define DUALSHOCK4_INITIALIZER_H

// includes
#include <stdlib.h>
#include <string.h>
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include <btstack_stdio_esp32.h>
#include <hci_dump.h>
#include <hci_dump_embedded_stdout.h>
#include <uni.h>
#include "dualshock4_connection_status.h"
#include "sdkconfig.h" // We need to see CONFIG_BLUEPAD32_PLATFORM_CUSTOM
//

// Only custom platform is allowed
#ifndef CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#error "Must use BLUEPAD32_PLATFORM_CUSTOM"
#endif

#ifndef CONFIG_BT_ENABLED
#error "Enable Bluetooth in sdkconfig!"
#endif

// Types
typedef struct uni_platform ds4_platform;
typedef struct uni_platform *ds4_platform_handle;
typedef struct uni_hid_device_t ds4_device;
typedef struct uni_hid_device_t *ds4_device_handle;
typedef struct platform_instance_s
{
    uni_gamepad_seat_t gamepad_seat; // which "seat" is being used
} platform_instance_t;

// Function prototypes
ds4_platform_handle get_ds4_platform(void);

#endif // DUALSHOCK4_INITIALIZER_H
