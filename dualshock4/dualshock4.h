// This is the API that you use to use the controller

#ifndef DUALSHOCK4_H
#define DUALSHOCK4_H

// includes
#include <stdlib.h>
#include <string.h>
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include <btstack_stdio_esp32.h>
#include <hci_dump.h>
#include <hci_dump_embedded_stdout.h>
#include <uni.h>
#include "sdkconfig.h" // We need to see CONFIG_BLUEPAD32_PLATFORM_CUSTOM
//

#ifndef CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#error "Must use BLUEPAD32_PLATFORM_CUSTOM"
#endif

// Defines
#define DUALSHOCK4_NAME "dualshock4" // Name of the controller (prefixes it's messages to the console)

// Types

// Callable functions

ds4_init_e ds4_init(void);
ds4_connection_error_e ds4_search_and_connect(void); // Connect to controller, report error
ds4_data_t ds4_recieve_data(void);
ds4_connection_error_e ds4_disconnect(void);

#endif // DUALSHOCK4_H
