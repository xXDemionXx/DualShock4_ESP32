#include "dualshock4.h"

#include "dualshock4_initializer.h"

/**
 * @brief Sets up everything needed for ds4 before we can connect.
 *
 * @return With what state did the initialization exit.
 */
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
    ds4_platform_handle ds4_platform = get_ds4_platform();

    return DS4_INIT_SUCCESFUL;
}
