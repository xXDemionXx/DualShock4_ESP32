#include "dualshock4_initializer.h"
#include "dualshock4_default_funcs.h"
#include "dualshock4_connection_status_modify.h"
#include "ds4_polling.h"

#ifdef CONFIG_DS4_MODE_EVENT
#include "ds4_event_handling.h"
#endif


// Public functions

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
    //    uni_bt_service_set_enabled(true);
}

void ds4_on_init_complete(void)
{
    logi(DUALSHOCK4_DEFAULT_NAME ": on_init_complete()\n");

    // Safe to call "unsafe" functions since they are called from BT thread

    // Start scanning
    // uni_bt_start_scanning_and_autoconnect_unsafe();
    // uni_bt_allow_incoming_connections(true);
}

uni_error_t ds4_on_device_discovered(bd_addr_t addr, const char *name, uint16_t cod, uint8_t rssi)
{
    // DualShock 4 is a gamepad so filter out anything that isn't a game pad.
    if (((cod & UNI_BT_COD_MINOR_MASK) & UNI_BT_COD_MINOR_GAMEPAD) != UNI_BT_COD_MINOR_GAMEPAD)
    {
        logi("Ignoring device - Not a DualShock 4 gamepad\n");
        return UNI_ERROR_IGNORE_DEVICE;
    }

    return UNI_ERROR_SUCCESS;
}

void ds4_on_device_connected(uni_hid_device_t *d)
{
    set_ds4_connection_status(DS4_CONNECTED);

#ifdef CONFIG_DS4_MODE_EVENT
    ds4_resume_buttons_event_handler();
#endif

    logi(DUALSHOCK4_DEFAULT_NAME ": device connected: %p\n", d);
}

void ds4_on_device_disconnected(uni_hid_device_t *d)
{
    set_ds4_connection_status(DS4_DISCONNECTED);

#ifdef CONFIG_DS4_MODE_EVENT
    ds4_suspend_buttons_event_handler();
#endif

    logi(DUALSHOCK4_DEFAULT_NAME ": device disconnected: %p\n", d);
}

uni_error_t ds4_on_device_ready(uni_hid_device_t *d)
{
    set_ds4_connection_status(DS4_READY);

    logi(DUALSHOCK4_DEFAULT_NAME ": device ready - %p\n", d);

    platform_instance_t *ins = (platform_instance_t *)&d->platform_data[0];
    ins->gamepad_seat = DS4_DEFAULT_SEAT;

    return UNI_ERROR_SUCCESS;
}

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

const uni_property_t *default_ds4_platform_get_property(uni_property_idx_t idx)
{
    ARG_UNUSED(idx);
    return NULL;
}
