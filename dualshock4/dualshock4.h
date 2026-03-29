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
    DS4_INIT_SUCCESFUL,
    DS4_INIT_BTSTACK_INIT_FAILED,
    DS4_INIT_BLUEPAD_INIT_FAILED,
    DS4_INIT_COMMAND_TASK_FAILED,
    DS4_INIT_FAILED
} ds4_init_e;

typedef enum
{
    DS4_COMMAND_SEND_FAIL,
    DS4_COMMAND_SEND_SUCCES
} ds4_command_send_e;

// Callable functions

void ds4Set_dataToESPQueu();
void ds4Set_controllerComandsQueue();
ds4_init_e ds4_init(void);
void ds4_run_loop(void);

/**
 * @brief Sends a string message to commands task
 *
 * @return Error code
 */
ds4_command_send_e ds4SendMessage(const char *message);
// void ds4_autoconnect_mode();
// void ds4_disconnect();
// ds4_connection_error_e ds4_search_and_connect(void); // Connect to controller, report error
// ds4_data_t ds4_recieve_data(void);
// ds4_connection_error_e ds4_disconnect(void);

#endif // DUALSHOCK4_H
