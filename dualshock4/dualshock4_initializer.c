#include "dualshock4_initializer.h"
#include "dualshock4_connection_status_modify.h"
#include "ds4_polling.h"
#include "uni.h"

#ifdef CONFIG_DS4_MODE_EVENT
#include "ds4_event_handling.h"
#endif

// Private defines

#define DUALSHOCK4_DEFAULT_NAME "dualshock4" // Name of the controller (prefixes it's messages to the console)
#define DS4_DEFAULT_SEAT GAMEPAD_SEAT_A

// Private functions

/**
 * @brief Do something for controller during init
 *
 * Currently we aren't doing anything special.
 *
 * @param argc Number of arguments (UNUSED)
 * @param argv Vector of arguments (UNUSED)
 */
void ds4_on_init(int argc, const char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    logi(DUALSHOCK4_DEFAULT_NAME ": init()\n");

    // Invert the controls if needed
#if 0
    uni_gamepad_mappings_t mappings = GAMEPAD_DEFAULT_MAPPINGS;

    // Inverted axis with inverted Y in RY.
    mappings.axis_x = UNI_GAMEPAD_MAPPINGS_AXIS_RX;
    mappings.axis_y = UNI_GAMEPAD_MAPPINGS_AXIS_RY;
    mappings.axis_ry_inverted = true;
    mappings.axis_rx = UNI_GAMEPAD_MAPPINGS_AXIS_X;
    mappings.axis_ry = UNI_GAMEPAD_MAPPINGS_AXIS_Y;

    // Invert A & B
    mappings.button_a = UNI_GAMEPAD_MAPPINGS_BUTTON_B;
    mappings.button_b = UNI_GAMEPAD_MAPPINGS_BUTTON_A;

    uni_gamepad_set_mappings(&mappings);
#endif
}

/**
 * @brief Do something for controller after init
 *
 * Currently we aren't doing anything special.
 */
void ds4_on_init_complete(void)
{
    logi(DUALSHOCK4_DEFAULT_NAME ": on_init_complete()\n");

    uni_bt_del_keys_unsafe();

#ifdef CONFIG_DS4_AUTOCONNECT_MODE
    uni_bt_start_scanning_and_autoconnect_unsafe();
    uni_bt_allow_incoming_connections(true);
#endif

#ifdef CONFIG_DS4_CONNECT_TO_SPECIFIC_MAC
    uni_bt_allow_incoming_connections(true);
#endif
}

/**
 * @brief Filter out the devices we don't want.
 *
 * Filter everything that isn't a DS4 controller.
 *
 * @param addr the Bluetooth address
 * @param name could be NULL, could be zero-length, or might contain the name
 * @param cod Class of Device. See "uni_bt_defines.h" for possible values
 * @param rssi Received Signal Strength Indicator (RSSI) measured in dBms. The higher (255) the better.
 * @return UNI error code, see uni_error.h
 */
uni_error_t ds4_on_device_discovered(bd_addr_t addr, const char *name, uint16_t cod, uint8_t rssi)
{
    // DualShock 4 is a gamepad so filter out anything that isn't a game pad.
    if (((cod & UNI_BT_COD_MINOR_MASK) & UNI_BT_COD_MINOR_GAMEPAD) != UNI_BT_COD_MINOR_GAMEPAD)
    {
        logi("Ignoring device - Not a gamepad\n");
        return UNI_ERROR_IGNORE_DEVICE;
    }

    return UNI_ERROR_SUCCESS;
}

/**
 * @brief What to do when a device connects
 *
 * Currently nothing except logging.
 *
 * @param d connected device handle
 */
void ds4_on_device_connected(uni_hid_device_t *d)
{
    set_ds4_connection_status(DS4_CONNECTED);

#ifdef CONFIG_DS4_AUTOCONNECT_MODE
    uni_bt_stop_scanning_unsafe();
    uni_bt_allow_incoming_connections(false);
#endif

#ifdef CONFIG_DS4_CONNECT_TO_SPECIFIC_MAC
    uni_bt_allow_incoming_connections(false);
#endif

#ifdef CONFIG_DS4_MODE_EVENT
    ds4_resume_buttons_event_handler();
#endif

    logi(DUALSHOCK4_DEFAULT_NAME ": device connected: %p\n", d);
}

/**
 * @brief What to do when a device disconnects
 *
 * Currently nothing except logging.
 *
 * @param d disconnected device handle
 */
void ds4_on_device_disconnected(uni_hid_device_t *d)
{
    set_ds4_connection_status(DS4_DISCONNECTED);

#ifdef CONFIG_DS4_AUTOCONNECT_MODE
    uni_bt_start_scanning_and_autoconnect_unsafe();
    uni_bt_allow_incoming_connections(true);
#endif

#ifdef CONFIG_DS4_CONNECT_TO_SPECIFIC_MAC
    uni_bt_allow_incoming_connections(true);
#endif

#ifdef CONFIG_DS4_MODE_EVENT
    ds4_suspend_buttons_event_handler();
#endif

    logi(DUALSHOCK4_DEFAULT_NAME ": device disconnected: %p\n", d);
}

/**
 * @brief What to do when a device is ready for communication
 *
 * Currently nothing except logging and setting seat of instance.
 *
 * @param d ready device handle
 * @return UNI error code, see uni_error.h
 */
uni_error_t ds4_on_device_ready(uni_hid_device_t *d)
{
    set_ds4_connection_status(DS4_READY);

    logi(DUALSHOCK4_DEFAULT_NAME ": device ready - %p\n", d);

    platform_instance_t *ins = (platform_instance_t *)&d->platform_data[0];
    ins->gamepad_seat = DS4_DEFAULT_SEAT;

    return UNI_ERROR_SUCCESS;
}

/**
 * @brief What to do on OOB events
 *
 * On DS4 OOB events are the pressing of system button
 * and bluetooth scanning.
 *
 * @param event event that triggered
 * @param data data of the event
 */
void default_ds4_platform_on_oob_event(uni_platform_oob_event_t event, void *data)
{
    switch (event)
    {
    case UNI_PLATFORM_OOB_GAMEPAD_SYSTEM_BUTTON:
        logi(DUALSHOCK4_DEFAULT_NAME ": PS button");
        break;

    case UNI_PLATFORM_OOB_BLUETOOTH_ENABLED:
        logi(DUALSHOCK4_DEFAULT_NAME ": Bluetooth enabled: %d\n", (bool)(data));
        break;

    default:
        logi(DUALSHOCK4_DEFAULT_NAME ": unsupported event: 0x%04x\n", event);
        break;
    }
}

/**
 * @brief What to do when controller data arrives
 *
 * Currently just logging
 *
 * @param d device that sent the data
 * @param ctl controllers data
 */
void default_ds4_platform_on_controller_data(uni_hid_device_t *d, uni_controller_t *ctl)
{
    static uni_controller_t prev = {0};

    // Optimization to avoid processing the previous data so that the console
    // does not get spammed with a lot of logs, but remove it from your project.
    if (memcmp(&prev, ctl, sizeof(*ctl)) == 0)
    {
        return;
    }
    prev = *ctl;

    ds4_polling_send((ds4_data_t *)&ctl->gamepad);

#ifdef CONFIG_DS4_MODE_EVENT
    ds4_send_data_to_event_handler((ds4_data_t *)&ctl->gamepad);
#endif
}

/**
 * @brief Gets the property of controller
 *
 * Needs further research
 */
const uni_property_t *default_ds4_platform_get_property(uni_property_idx_t idx)
{
    ARG_UNUSED(idx);
    return NULL;
}

// Private variables

// This is the instance that holds all callbacks for DS4.
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

// Public functions

/**
 * @brief Returns the pointer to the structure that contains all callbacks
 */
ds4_platform_handle get_ds4_platform(void)
{
    return &ds4_plat;
}
