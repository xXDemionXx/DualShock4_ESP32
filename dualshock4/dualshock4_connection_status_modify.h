#ifndef DUALSHOCK4_CONNECTION_STATUS_MODIFY_H
#define DUALSHOCK4_CONNECTION_STATUS_MODIFY_H

#include "dualshock4_connection_status.h"
#include <stdatomic.h>

/**
 * @brief Sets the connection status using atomic instructions
 * 
 * Use this function in API calls, not access_ds4_connection_status()
 */
void set_ds4_connection_status(ds4_connection_status_e);

/**
 * @brief Returns the pointer to the connection status variable
 */
_Atomic ds4_connection_status_e *access_ds4_connection_status(void);

void store_ds4_connection_status_pointer(_Atomic ds4_connection_status_e *);


/**
 * @brief Use this function when initializing the library
 */
void ds4_init_connection_status(void);

#endif  // DUALSHOCK4_CONNECTION_STATUS_MODIFY_H

