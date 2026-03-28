// This is the API that you use to use the controller

#ifndef DUALSHOCK4_H
#define DUALSHOCK4_H

#include "dualshock4_initializer.h"

#ifndef CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#error "Must use Blupad32 \"custom\" platform in sdkconfig!"
#endif

#ifndef CONFIG_BT_ENABLED
#error "Enable Bluetooth in sdkconfig!"
#endif

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

ds4_init_e ds4_init(void);
void ds4_run_loop(void);
// ds4_connection_error_e ds4_search_and_connect(void); // Connect to controller, report error
// ds4_data_t ds4_recieve_data(void);
// ds4_connection_error_e ds4_disconnect(void);

#endif // DUALSHOCK4_H
