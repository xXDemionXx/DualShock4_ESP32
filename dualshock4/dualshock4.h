// This is the API that you use to use the controller

#ifndef DUALSHOCK4_H
#define DUALSHOCK4_H

#include "dualshock4_initializer.h"
#include "dualshock4_task_commands.h"

// Defines

// Types

/**
 * @brief Describes how the initialization process went.
 */
typedef enum
{
    DS4_INIT_SUCCES,
    DS4_INIT_BTSTACK_INIT_FAILED,
    DS4_INIT_BLUEPAD_INIT_FAILED,
    DS4_INIT_COMMAND_TASK_FAILED,
    DS4_INIT_FAILED
} ds4_init_e;

typedef enum
{
    DS4_COMMAND_SEND_FAIL_NO_CONTROLLER,
    DS4_COMMAND_SEND_FAIL_QUEUE,
    DS4_COMMAND_SEND_SUCCES
} ds4_command_send_e;

// Callable functions

ds4_init_e ds4_init(void);
void ds4_run_loop(void);

/**
 * @brief Sends a string message to commands task
 *
 * @return Error code
 */
ds4_command_send_e ds4SendMessage(const char *message);

/**
 * @brief Set the RGB value of the lightbar
 *
 * @return Error code
 */
ds4_command_send_e ds4SetLightbar(uint8_t R, uint8_t G, uint8_t B);

/**
 * @brief Returns the connection status
 *
 * @return Connection status
 */
ds4_connection_status_e ds4GetConnectionStatus(void);
// void ds4_autoconnect_mode();
// void ds4_disconnect();
// ds4_connection_error_e ds4_search_and_connect(void); // Connect to controller, report error
// ds4_data_t ds4_recieve_data(void);
// ds4_connection_error_e ds4_disconnect(void);

#endif // DUALSHOCK4_H
