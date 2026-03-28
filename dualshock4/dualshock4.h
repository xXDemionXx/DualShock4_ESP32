// This is the API that you use to use the controller

#ifndef DUALSHOCK4_H
#define DUALSHOCK4_H

#include "dualshock4_initializer.h"

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
    DS4_INIT_FAILED
} ds4_init_e;

// Callable functions

void ds4Set_dataToESPQueu();
void ds4Set_controllerComandsQueue();
ds4_init_e ds4_init(void);
void ds4_run_loop(void);
// void ds4_autoconnect_mode();
// void ds4_disconnect();
// ds4_connection_error_e ds4_search_and_connect(void); // Connect to controller, report error
// ds4_data_t ds4_recieve_data(void);
// ds4_connection_error_e ds4_disconnect(void);

#endif // DUALSHOCK4_H
