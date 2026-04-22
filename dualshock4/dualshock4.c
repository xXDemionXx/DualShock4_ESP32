#include "dualshock4.h"
#include "dualshock4_initializer.h"
#include "dualshock4_connection_status_modify.h"
#include "controller_commands.h"
#include "btstack_defines.h"
#include "ds4_event_handling.h"
#include "dualshock4_connection_status_modify.h"
#include "bt/uni_bt_conn.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "uni_version.h"
#include <string.h>


// Private variables

static btstack_context_callback_registration_t callback_registration;
static ds4_command_t commands[DS4_NUM_OF_COMMAND_TYPES] = {0}; // An array that is used for storing data of each command type


// Private function prototypes

static bool string_to_MAC(const char *MAC_string, bd_addr_t *MAC);
static ds4_init_e ds4_bluepad32_init(void);


// Public functions

ds4_init_e ds4Init(const char *MAC)
{
    // If you enable HCI Dump better to disable "Bluepad32 USB Console" from "idf.py menuconfig".

    // Don't use BTstack buffered UART. It conflicts with the console.
#ifdef CONFIG_ESP_CONSOLE_UART
#ifndef CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
    btstack_stdio_init();
#endif // CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
#endif // CONFIG_ESP_CONSOLE_UART

    if (MAC != NULL)    // If user provides a MAC address use it for Bluetooth
    {
        bd_addr_t addr;
        if (string_to_MAC(MAC, &addr) == true)
            esp_iface_mac_addr_set(addr, ESP_MAC_BT);
        else
        {
            ESP_LOGE("DS4_init",
                     "Invalid MAC string provided\nExpected formating - \"XX:XX:XX:XX:XX:XX\" where XX is HEX value in range from 00 to FF");
            return DS4_INIT_BAD_MAC;
        }
    }   // If user passes NULL, the default Bluetooth MAC will be used

    // Configure BTstack for ESP32 VHCI Controller
    if (btstack_init() != 0)
        return DS4_INIT_BTSTACK_INIT_FAILED;

    // Init Bluepad32
    if (ds4_bluepad32_init() != 0)
        return DS4_INIT_BLUEPAD_INIT_FAILED;

    // uni_bt_allowlist_remove_all();

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
}

void ds4GetUserAddress(char buffer[18])
{
    bd_addr_t addr;
    gap_local_bd_addr(addr);
    snprintf(buffer, 18, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
}

void ds4SetUserAddress(const char *MAC_string)
{
}

bool ds4AllowDevice(const char *MAC_string)
{
    bd_addr_t addr;
    if (string_to_MAC(MAC_string, &addr))
    {
        if (uni_bt_allowlist_add_addr(addr) == true)
        {
            uni_bt_allowlist_set_enabled(true); // If not allready enabled
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void ds4UnallowDevices(void)
{
    uni_bt_allowlist_remove_all();
    uni_bt_allowlist_set_enabled(false);
}

void ds4_run_loop(void)
{
    // Does not return.
    btstack_run_loop_execute();
}

// Private functions

// MAC is the return array
// Returns false if formating error
static bool string_to_MAC(const char *MAC_string, bd_addr_t *MAC)
{
    char current_char;
    char hex_buff[3] = {'0', '0', '\0'};
    uint8_t hex_idx = 0;
    uint8_t MAC_idx = 0;
    char *endptr; // Used for checking if strtol() found invalid characters

    if (strlen(MAC_string) != 17) // Invalid string length
        return false;

    for (uint8_t i = 0; i < 17; i++)
    {
        current_char = MAC_string[i];

        if (current_char == ':')
        {
            if (
                // Checks if ':' is in an unexpected place (XX:XX:XX:XX:XX:XX)
                (i != 2) &&
                (i != 5) &&
                (i != 8) &&
                (i != 11) &&
                (i != 14))
            {
                return false;
            }
            else
            {
                hex_idx = 0; // Reset for the next number
                (*MAC)[MAC_idx] = (uint8_t)strtol(hex_buff, &endptr, 16);
                MAC_idx++;
                if (*endptr != '\0')
                    return false; // Invalid character for HEX
            }
        }
        else
        {
            if (hex_idx > 1)
                return false;

            hex_buff[hex_idx] = current_char;
            hex_idx++;
        }
    }

    // The last number is gotten outside of the loop
    (*MAC)[MAC_idx] = (uint8_t)strtol(hex_buff, &endptr, 16);
    if (*endptr != '\0')
        return false; // Invalid character for HEX

    return true;
}

static ds4_init_e ds4_bluepad32_init(void)
{

    // Get pointer to a struct containing all functions for the platform
    // Then set that struct for uni platform
    uni_platform_set_custom(get_ds4_platform());

    // Disable stdout buffering
    setbuf(stdout, NULL);

    // Honoring Bluepad32 license
    loge("Bluepad32 (C) 2016-2025 Ricardo Quesada and contributors.\n");
    loge("Version: v" UNI_VERSION_STRING "\n");

    // Honoring BTstack license
    loge("BTstack: Copyright (C) 2017 BlueKitchen GmbH.\n");
    loge("Version: v" BTSTACK_VERSION_STRING "\n");

    uni_property_init();
    uni_platform_init(0, NULL); // Initialize controller, do not pass anything
    uni_hid_device_setup();

    // Continue with bluetooth setup.
    if (uni_bt_setup() != 0)
        return DS4_INIT_BLUEPAD_INIT_FAILED;
    uni_bt_allowlist_init();
    uni_bt_allowlist_set_enabled(false); // This ensures that the user is explicitly required to call ds4AllowDevice()
    // uni_virtual_device_init();   // No virtual device (touchpad) for now

    return 0;
}
