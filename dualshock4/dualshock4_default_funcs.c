#include "dualshock4_initializer.h"

// Function prototypes
// static void trigger_event_on_gamepad(uni_hid_device_t* d); Unimplemented yet
platform_instance_t *default_get_ds4_platform_instance(uni_hid_device_t *d);

/**
 * @brief Do something for controller during init
 *
 * Currently we aren't doing anything special in
 * initialization.
 *
 * @param[in] argc Number of arguments (UNUSED)
 * @param[in] argv Vector of arguments (UNUSED)
 */
void default_ds4_platform_init(int argc, const char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    logi(DUALSHOCK4_NAME ": init()\n");

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
    //    uni_bt_service_set_enabled(true);
}

/**
 * @brief Do something for controller after init
 *
 * Currently we aren't doing anything special.
 */
void default_ds4_platform_init_on_init_complete(void)
{
    logi(DUALSHOCK4_NAME ": on_init_complete()\n");

    // Safe to call "unsafe" functions since they are called from BT thread

    // Start scanning
    uni_bt_start_scanning_and_autoconnect_unsafe();
    uni_bt_allow_incoming_connections(true);

    // Based on runtime condition, you can delete or list the stored BT keys.
    if (1)
        uni_bt_del_keys_unsafe();
    else
        uni_bt_list_keys_unsafe();
}

/**
 * @brief Filter out the devices we don't want.
 *
 * Filter everything that isn't a DS4 controller.
 *
 * @param[in] addr the Bluetooth address
 * @param[in] name could be NULL, could be zero-length, or might contain the name
 * @param[in] cod Class of Device. See "uni_bt_defines.h" for possible values
 * @param[in] rssi Received Signal Strength Indicator (RSSI) measured in dBms. The higher (255) the better.
 * @return UNI error code, see uni_error.h
 */
uni_error_t default_ds4_platform_on_device_discovered(bd_addr_t addr, const char *name, uint16_t cod, uint8_t rssi)
{
    // DualShock 4 is a gamepad so filter out anything that isn't a game pad.
    if (((cod & UNI_BT_COD_MINOR_MASK) & UNI_BT_COD_MINOR_GAMEPAD) != UNI_BT_COD_MINOR_GAMEPAD)
    {
        logi("Ignoring device - Not a DualShock 4 gamepad\n");
        return UNI_ERROR_IGNORE_DEVICE;
    }

    return UNI_ERROR_SUCCESS;
}

/**
 * @brief What to do when a device connects
 *
 * Currently nothing except logging.
 *
 * @param[in] d connected device handle
 */
void default_ds4_platform_on_device_connected(uni_hid_device_t *d)
{
    logi(DUALSHOCK4_NAME ": device connected: %p\n", d);
}

/**
 * @brief What to do when a device disconnects
 *
 * Currently nothing except logging.
 *
 * @param[in] d disconnected device handle
 */
void default_ds4_platform_on_device_disconnected(uni_hid_device_t *d)
{
    logi(DUALSHOCK4_NAME ": device disconnected: %p\n", d);
}

/**
 * @brief What to do when a device is ready for communication
 *
 * Currently nothing except logging and setting seat of instance.
 *
 * @param[in] d ready device handle
 * @return UNI error code, see uni_error.h
 */
uni_error_t default_ds4_platform_on_device_ready(uni_hid_device_t *d)
{
    logi(DUALSHOCK4_NAME ": device ready: %p\n", d);
    platform_instance_t *ins = default_get_ds4_platform_instance(d);
    ins->gamepad_seat = GAMEPAD_SEAT_A;

    d->report_parser.set_lightbar_color(d, 200, 200, 0);

    return UNI_ERROR_SUCCESS;
}

/**
 * @brief What to do on OOB events
 *
 * On DS4 OOB events are the pressing of system button
 * and bluetooth scanning.
 *
 * @param[in] event event that triggered
 * @param[in] data data of the event
 */
void default_ds4_platform_on_oob_event(uni_platform_oob_event_t event, void *data)
{

    // Need to test all events on DS4, for now just log them
    logi(DUALSHOCK4_NAME ": on_device_oob_event(): %d\n", event);

    switch (event)
    {
    case UNI_PLATFORM_OOB_GAMEPAD_SYSTEM_BUTTON:
        break;

    case UNI_PLATFORM_OOB_BLUETOOTH_ENABLED:
        logi("custom: Bluetooth enabled: %d\n", (bool)(data));
        break;

    default:
        logi("DS4_platform_on_oob_event: unsupported event: 0x%04x\n", event);
        break;
    }
}

/**
 * @brief What to do when controller data arrives
 *
 * Currently just logging
 *
 * @param[in] d device that sent the data
 * @param[in] ctl controllers data
 */
void default_ds4_platform_on_controller_data(uni_hid_device_t *d, uni_controller_t *ctl)
{
    // static uint8_t leds = 0;
    // static uint8_t enabled = true;
    static uni_controller_t prev = {0};
    // uni_gamepad_t* gp;

    // Optimization to avoid processing the previous data so that the console
    // does not get spammed with a lot of logs, but remove it from your project.
    if (memcmp(&prev, ctl, sizeof(*ctl)) == 0)
    {
        return;
    }
    prev = *ctl;
    // Print device Id before dumping gamepad.
    // This could be very CPU intensive and might crash the ESP32.
    // Remove these 2 lines in production code.
    // logi("(%p), id=%d, \n", d, uni_hid_device_get_idx_for_instance(d));
    // uni_controller_dump(ctl);

    // switch (ctl->klass) {
    //     case UNI_CONTROLLER_CLASS_GAMEPAD:
    //         gp = &ctl->gamepad;

    //         // Debugging
    //         // Axis ry: control rumble
    //         if ((gp->buttons & BUTTON_A) && d->report_parser.play_dual_rumble != NULL) {
    //             d->report_parser.play_dual_rumble(d, 0 /* delayed start ms */, 250 /* duration ms */,
    //                                               255 /* weak magnitude */, 0 /* strong magnitude */);
    //         }
    //         // Buttons: Control LEDs On/Off
    //         if ((gp->buttons & BUTTON_B) && d->report_parser.set_player_leds != NULL) {
    //             d->report_parser.set_player_leds(d, leds++ & 0x0f);
    //         }
    //         // Axis: control RGB color
    //         if ((gp->buttons & BUTTON_X) && d->report_parser.set_lightbar_color != NULL) {
    //             uint8_t r = (gp->axis_x * 256) / 512;
    //             uint8_t g = (gp->axis_y * 256) / 512;
    //             uint8_t b = (gp->axis_rx * 256) / 512;
    //             d->report_parser.set_lightbar_color(d, r, g, b);
    //         }

    //         // Toggle Bluetooth connections
    //         if ((gp->buttons & BUTTON_SHOULDER_L) && enabled) {
    //             logi("*** Stop scanning\n");
    //             uni_bt_stop_scanning_safe();
    //             enabled = false;
    //         }
    //         if ((gp->buttons & BUTTON_SHOULDER_R) && !enabled) {
    //             logi("*** Start scanning\n");
    //             uni_bt_start_scanning_and_autoconnect_safe();
    //             enabled = true;
    //         }
    //         break;
    //     default:
    //         break;
    // }
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

platform_instance_t *default_get_ds4_platform_instance(uni_hid_device_t *d)
{
    return (platform_instance_t *)&d->platform_data[0];
}
