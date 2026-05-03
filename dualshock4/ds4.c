#include "ds4.h"
#include "controller_behaviour.h"
#include "connection_status_modify.h"
#include "controller_commands.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "uni_version.h"
#include "uni_log.h"
#include "bt/uni_bt.h"
#include "bt/uni_bt_defines.h"
#include "bt/uni_bt_allowlist.h"
#include "bt/uni_bt_setup.h"
#include <stdbool.h>
#ifdef CONFIG_DS4_MODE_EVENT
#include "event_handling.h"
#include "ds4_event_handling_init.h"
#endif

// Defines

#define KB_TO_B(x) x * 1024
#define DS4_INIT_TASK_SIZE 5 // In kB
#define DS4_INIT_TASK_NAME "DS4 init"
#define DS4_INIT_TASK_PRIORITY 10
#ifdef CONFIG_BT_BLUEDROID_PINNED_TO_CORE
#define DS4_INIT_TASK_CORE CONFIG_BT_BLUEDROID_PINNED_TO_CORE
#else
#define DS4_INIT_TASK_CORE 0
#endif

// Private constants

static const char *TAG_DS4_Init = "DS4_Init";

// Private variables

static btstack_context_callback_registration_t callback_registration;
static ds4_command_t commands[DS4_NUM_OF_COMMAND_TYPES] = {0}; // An array that is used for storing data of each command type

// Private function prototypes

static void ds4_init_task(void *p_parameter);
static bool ds4_bluepad32_init(void);
#if defined(CONFIG_DS4_CONNECT_TO_SPECIFIC_MAC) || defined(CONFIG_DS4_BT_MAC_CUSTOM)
static bool string_to_MAC(const char *MAC_string, bd_addr_t *MAC);
#endif

// Public functions

void ds4Init()
{
    if (pdPASS != xTaskCreatePinnedToCore(&ds4_init_task, DS4_INIT_TASK_NAME, KB_TO_B(DS4_INIT_TASK_SIZE), NULL, DS4_INIT_TASK_PRIORITY, NULL, DS4_INIT_TASK_CORE))
    {
        ESP_LOGE(TAG_DS4_Init, "Init task creation failed.");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
}

void ds4Disconnect(void)
{
    if (ds4GetConnectionStatus() == DS4_READY)
    {
        set_ds4_connection_status(DS4_DISCONNECTING);
        uni_bt_disconnect_device_safe(DS4_DEVICE_IDX);
    }
}

bool ds4SetLightbar(uint8_t R, uint8_t G, uint8_t B)
{
    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY))
        return false;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_LIGHTBAR].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return false;

    commands[DS4_COMMAND_LIGHTBAR].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_LIGHTBAR].data.lightbar.R = R;
    commands[DS4_COMMAND_LIGHTBAR].data.lightbar.G = G;
    commands[DS4_COMMAND_LIGHTBAR].data.lightbar.B = B;

    callback_registration.callback = &ds4_lightbar_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_LIGHTBAR]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return true;
}

bool ds4PlayRumble(uint8_t magnitude, uint16_t duration, uint16_t start_delay)
{
    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY))
        return false;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return false;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return true;
}

bool ds4PlayRumbleWeak(uint8_t magnitude, uint16_t duration, uint16_t start_delay)
{
    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY))
        return false;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return false;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = 0;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return true;
}

bool ds4PlayRumbleStrong(uint8_t magnitude, uint16_t duration, uint16_t start_delay)
{
    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY))
        return false;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return false;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = 0;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = magnitude;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return true;
}

bool ds4PlayRumbleSpecific(uint8_t magnitude_weak, uint8_t magnitude_strong, uint16_t duration, uint16_t start_delay)
{
    // Controller not connected or no device handle
    if ((ds4GetConnectionStatus() != DS4_READY))
        return false;

    // The callback sets the command status to AVAILABLE when it is done sending
    if (commands[DS4_COMMAND_RUMBLE].status == DS4_COMMAND_STATUS_UNAVAILABLE)
        return false;

    commands[DS4_COMMAND_RUMBLE].status = DS4_COMMAND_STATUS_UNAVAILABLE; // Take the command spot
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_weak = magnitude_weak;
    commands[DS4_COMMAND_RUMBLE].data.rumble.magnitude_strong = magnitude_strong;
    commands[DS4_COMMAND_RUMBLE].data.rumble.duration = duration;
    commands[DS4_COMMAND_RUMBLE].data.rumble.start_delay = start_delay;

    callback_registration.callback = &ds4_rumble_callback;
    callback_registration.context = (void *)(&commands[DS4_COMMAND_RUMBLE]);
    btstack_run_loop_execute_on_main_thread(&callback_registration);

    return true;
}

void ds4GetUserAddress(char buffer[18])
{
    bd_addr_t addr;
    gap_local_bd_addr(addr);
    snprintf(buffer, 18, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
}

// Private functions

// We need to use a seperate task that initializes Btstack and runs it's loop
static void ds4_init_task(void *p_parameter)
{
    // If you enable HCI Dump better to disable "Bluepad32 USB Console" from "idf.py menuconfig".

    // Don't use BTstack buffered UART. It conflicts with the console.
#ifdef CONFIG_ESP_CONSOLE_UART
#ifndef CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
    btstack_stdio_init();
#endif // CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
#endif // CONFIG_ESP_CONSOLE_UART

#ifdef CONFIG_DS4_BT_MAC_CUSTOM
    // If user provides a MAC address use it for Bluetooth
    bd_addr_t addr;
    if (string_to_MAC(CONFIG_DS4_BT_CUSTOM_MAC_STRING, &addr) == true)
        esp_iface_mac_addr_set(addr, ESP_MAC_BT);
    else
    {
        ESP_LOGE(TAG_DS4_Init,
                 "Invalid Bluetooth MAC string provided\nExpected formating - \"XX:XX:XX:XX:XX:XX\" where XX is HEX value in range from 00 to FF");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
#endif

    btstack_init();

    ds4_bluepad32_init();

#ifdef CONFIG_DS4_MODE_EVENT
    if (ds4_init_buttons_event_handler() != DS4_INIT_EVENT_SUCCES)
    {
        ESP_LOGE(TAG_DS4_Init, "Buttons event handler init fail.");
        ESP_ERROR_CHECK(ESP_FAIL);
    }
#endif

    // run loop needs to run in the same task context as btstack_init()
    btstack_run_loop_execute(); // Does not return
}

#if defined(CONFIG_DS4_CONNECT_TO_SPECIFIC_MAC) || defined(CONFIG_DS4_BT_MAC_CUSTOM)
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
#endif

// Returns true if successful
static bool ds4_bluepad32_init(void)
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
        return false;

#ifdef CONFIG_DS4_CONNECT_TO_SPECIFIC_MAC
    uni_bt_allowlist_init();
    bd_addr_t addr;
    if (string_to_MAC(CONFIG_DS4_CONTROLLER_MAC_STRING, &addr) == true)
    {
        // If the address isn't on the list remove the previous and add this one
        if (uni_bt_allowlist_is_allowed_addr(addr) != true)
        {
            uni_bt_allowlist_remove_all();
            esp_iface_mac_addr_set(addr, ESP_MAC_BT);
        }
    }
    else
    {
        ESP_LOGE(TAG_DS4_Init,
                 "Invalid controller MAC string provided\nExpected formating - \"XX:XX:XX:XX:XX:XX\" where XX is HEX value in range from 00 to FF");
        ESP_ERROR_CHECK(ESP_FAIL);
        return false;
    }
    uni_bt_allowlist_add_addr(addr);
    uni_bt_allowlist_set_enabled(true);
#endif

    // uni_virtual_device_init();   // No virtual device (touchpad) for now

    return true;
}
