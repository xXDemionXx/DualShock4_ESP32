#include "dualshock4_initializer.h"
#include "dualshock4_default_funcs.h"

// This is the global instance that holds all callbacks for DS4.
static ds4_platform ds4_plat = {
    .name = DUALSHOCK4_DEFAULT_NAME,
    .init = ds4_on_init,
    .on_init_complete = ds4_on_init_complete,
    .on_device_discovered = ds4_on_device_discovered,
    .on_device_connected = ds4_on_device_connected,
    .on_device_disconnected = ds4_on_device_disconnected,
    .on_device_ready = ds4_on_device_ready,
    .on_oob_event = default_ds4_platform_on_oob_event,
    .on_controller_data = default_ds4_platform_on_controller_data,
    .get_property = default_ds4_platform_get_property,
};

/**
 * @brief Returns the pointer to the structure that contains all callbacks
 */
ds4_platform_handle get_ds4_platform(void)
{
    return &ds4_plat;
}
