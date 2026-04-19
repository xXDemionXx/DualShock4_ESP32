#include "dualshock4.h"
#include "dualshock4_initializer.h"
#include "dualshock4_connection_status_modify.h"
#include "controller_commands.h"
#include "btstack_defines.h"
#include "ds4_event_handling.h"
#include "dualshock4_connection_status_modify.h"
#include "bt/uni_bt_conn.h"


// Local defines
#define DS4_COMMAND_TASK_SEND_TIME 100 // In ms
//

// Private variables
static btstack_context_callback_registration_t callback_registration;
static ds4_command_t commands[DS4_NUM_OF_COMMAND_TYPES] = {0}; // An array that is used for storing data of each command type

ds4_init_e ds4_init(void)
{
    // If you enable HCI Dump better to disable "Bluepad32 USB Console" from "idf.py menuconfig".

    // Don't use BTstack buffered UART. It conflicts with the console.
#ifdef CONFIG_ESP_CONSOLE_UART
#ifndef CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
    btstack_stdio_init();
#endif // CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
#endif // CONFIG_ESP_CONSOLE_UART

    // Configure BTstack for ESP32 VHCI Controller
    if (btstack_init() != 0)
        return DS4_INIT_BTSTACK_INIT_FAILED;

    // Get pointer to a struct containing all functions for the platform
    // Then set that struct for uni platform
    uni_platform_set_custom(get_ds4_platform());

    // Init Bluepad32 with no arguments
    if (uni_init(0 /* argc */, NULL /* argv */) != 0)
        return DS4_INIT_BLUEPAD_INIT_FAILED;

    // Needed for global connection status checking
    ds4_init_connection_status();

#ifdef CONFIG_DS4_MODE_EVENT
    ds4_event_handling_init_e buttons_event_handler_init_error;
    buttons_event_handler_init_error = ds4_init_buttons_event_handler();
    if (buttons_event_handler_init_error != DS4_INIT_EVENT_SUCCES)
        return DS4_INIT_BUTTONS_EVENT_HANDLER_TASK_FAILED;
#endif

    return DS4_INIT_SUCCES;
}

ds4_command_send_e ds4SetLightbar(uint8_t R, uint8_t G, uint8_t B)
{
    ds4_device_handle ds4 = uni_hid_device_get_instance_for_idx(DS4_DEVICE_IDX);

    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY) || (ds4 == NULL))
        return DS4_COMMAND_SEND_FAIL_NO_CONTROLLER;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_LIGHTBAR].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return DS4_COMMAND_SEND_FAIL_LAST_COMMAND_NOT_SENT;

    commands[DS4_COMMAND_LIGHTBAR].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_LIGHTBAR].device = ds4;
    commands[DS4_COMMAND_LIGHTBAR].data.lightbar.R = R;
    commands[DS4_COMMAND_LIGHTBAR].data.lightbar.G = G;
    commands[DS4_COMMAND_LIGHTBAR].data.lightbar.B = B;

    callback_registration.callback = &ds4_lightbar_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_LIGHTBAR]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return DS4_COMMAND_SEND_SUCCES;
}

ds4_command_send_e ds4PlayRumble(uint8_t magnitude, uint16_t duration, uint16_t start_delay)
{
    ds4_device_handle ds4 = uni_hid_device_get_instance_for_idx(DS4_DEVICE_IDX);

    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY) || (ds4 == NULL))
        return DS4_COMMAND_SEND_FAIL_NO_CONTROLLER;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return DS4_COMMAND_SEND_FAIL_LAST_COMMAND_NOT_SENT;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].device = ds4;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return DS4_COMMAND_SEND_SUCCES;
}

ds4_command_send_e ds4PlayRumbleWeak(uint8_t magnitude, uint16_t duration, uint16_t start_delay)
{
    ds4_device_handle ds4 = uni_hid_device_get_instance_for_idx(DS4_DEVICE_IDX);

    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY) || (ds4 == NULL))
        return DS4_COMMAND_SEND_FAIL_NO_CONTROLLER;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return DS4_COMMAND_SEND_FAIL_LAST_COMMAND_NOT_SENT;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].device = ds4;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = 0;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return DS4_COMMAND_SEND_SUCCES;
}

ds4_command_send_e ds4PlayRumbleStrong(uint8_t magnitude, uint16_t duration, uint16_t start_delay)
{
    ds4_device_handle ds4 = uni_hid_device_get_instance_for_idx(DS4_DEVICE_IDX);

    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY) || (ds4 == NULL))
        return DS4_COMMAND_SEND_FAIL_NO_CONTROLLER;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return DS4_COMMAND_SEND_FAIL_LAST_COMMAND_NOT_SENT;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].device = ds4;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = 0;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return DS4_COMMAND_SEND_SUCCES;
}

ds4_command_send_e ds4PlayRumbleSpecific(uint8_t magnitude_weak, uint8_t magnitude_strong, uint16_t duration, uint16_t start_delay)
{
    ds4_device_handle ds4 = uni_hid_device_get_instance_for_idx(DS4_DEVICE_IDX);

    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY) || (ds4 == NULL))
        return DS4_COMMAND_SEND_FAIL_NO_CONTROLLER;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return DS4_COMMAND_SEND_FAIL_LAST_COMMAND_NOT_SENT;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].device = ds4;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = magnitude_weak;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = magnitude_strong;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return DS4_COMMAND_SEND_SUCCES;
}

void ds4Disconnect(void)
{
    if (ds4GetConnectionStatus() == DS4_READY)
    {
        set_ds4_connection_status(DS4_DISCONNECTING);
        uni_bt_disconnect_device_safe(DS4_DEVICE_IDX);
    }
    else
        ESP_LOGI("SFDS", "sdada");
}

// Private functions

void ds4_run_loop(void)
{
    // Does not return.
    btstack_run_loop_execute();
}
