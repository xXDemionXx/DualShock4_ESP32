#include "dualshock4.h"

#include "dualshock4_initializer.h"
#include "dualshock4_task_commands.h"
#include "dualshock4_connection_status_modify.h"


// Variables
static QueueHandle_t commands_queue_handle = NULL;
static QueueHandle_t receive_queue_handle = NULL;

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

    // Needed for global connection status checking
    ds4_init_connection_status();

    ds4_command_task_init_return_t command_task_init_return;
    command_task_init_return = ds4_init_commands_task();
    if (command_task_init_return.error_code != DS4_COMMAND_TASK_INIT_SUCCES)
        return DS4_INIT_COMMAND_TASK_FAILED;
    else
        commands_queue_handle = command_task_init_return.queue_handle;

    // ds4_command_task_init_error_e ds4_commaand_task_init_error;
    //  ds4_commaand_task_init_error = ds4_commaand_task_init(commands_queue_handle)

    return DS4_INIT_SUCCES;
}

// ds4_connection_status_e ds4GetConnectionStatus(void){
//     return atomic_load(access_ds4_connection_status());
// }


ds4_command_send_e ds4SendMessage(const char *message)
{
    ds4_command_s command;
    command.command_indicator = DS4_COMMAND_TYPE_TEST_WRITE;
    strcpy((char *)&command.data.test_write_command.string, message);
    if (pdTRUE == xQueueSend(commands_queue_handle, &command, 100 / portTICK_PERIOD_MS))
    {
        return DS4_COMMAND_SEND_SUCCES;
    }
    else
    {
        return DS4_COMMAND_SEND_FAIL;
    }
}

void ds4_run_loop(void)
{
    // Does not return.
    btstack_run_loop_execute();
}
