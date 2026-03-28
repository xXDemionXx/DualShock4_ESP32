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
    // Then set that struct for uni platform
    uni_platform_set_custom(get_ds4_platform());

    // Init Bluepad32 with no arguments
    if (uni_init(0 /* argc */, NULL /* argv */) != 0)
        return DS4_INIT_BLUEPAD_INIT_FAILED;

    return DS4_INIT_SUCCESFUL;
}

void ds4Set_controllerComandsQueue()
{
    // ds4_platform_handle ds4 = get_ds4_platform();
    // ds4->
}

void ds4_run_loop(void)
{
    // Does not return.
    btstack_run_loop_execute();
}
