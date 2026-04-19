#include "ds4_init.h"
#include "ds4_event_handling_init.h"
#include "dualshock4_initializer.h"
#include "dualshock4_connection_status_modify.h"
#include "btstack_defines.h"
#include "bt/uni_bt_conn.h"
//#include "dualshock4_initializer.h"

#include "platform/uni_platform.h"
#include "uni_btstack_version_compat.h"
#include "uni_config.h"
#include "uni_console.h"
#include "uni_hid_device.h"
#include "uni_log.h"
#include "uni_property.h"
#include "uni_version.h"
#include "uni_virtual_device.h"

// Private function prototypes
void ds4_bluepad32_init(void);

ds4_init_e ds4Init(void)
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
    
    // Init Bluepad32
    ds4_bluepad32_init();

    // // Init Bluepad32 with no arguments
    // if (uni_init(0 /* argc */, NULL /* argv */) != 0)
    //     return DS4_INIT_BLUEPAD_INIT_FAILED;

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

void ds4_bluepad32_init(void){

    // Get pointer to a struct containing all functions for the platform
    // Then set that struct for uni platform
    uni_platform_set_custom(get_ds4_platform());

    // Disable stdout buffering
    setbuf(stdout, NULL);

    loge("Bluepad32 (C) 2016-2025 Ricardo Quesada and contributors.\n");
    loge("Version: v" UNI_VERSION_STRING "\n");

    // Honoring BTstack license
    loge("BTstack: Copyright (C) 2017 BlueKitchen GmbH.\n");
    loge("Version: v" BTSTACK_VERSION_STRING "\n");

    uni_property_init();
    uni_platform_init(0, NULL); // Initialize controller, do not pass anything
    uni_hid_device_setup();

    // Continue with bluetooth setup.
    uni_bt_setup();
    uni_bt_allowlist_init();
    // uni_virtual_device_init();
}
