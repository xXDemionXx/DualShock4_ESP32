// This is the API that you use to use the controller


#ifndef DUALSHOCK4_H
#define DUALSHOCK4_H

// includes
//#include <uni.h>
//

// Types

// Callable functions

ds4_init_e ds4_init(void);
ds4_connection_error_e ds4_search_and_connect(void);    // Connect to controller, report error
ds4_data_t ds4_recieve_data(void);
ds4_connection_error_e ds4_disconnect(void);


#endif // DUALSHOCK4_H
