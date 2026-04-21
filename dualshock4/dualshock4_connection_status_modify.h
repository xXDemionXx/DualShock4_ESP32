#ifndef DUALSHOCK4_CONNECTION_STATUS_MODIFY_H
#define DUALSHOCK4_CONNECTION_STATUS_MODIFY_H

#include "dualshock4_connection_status.h"
#include <stdatomic.h>

/**
 * @brief Sets the connection status
 */
void set_ds4_connection_status(ds4_connection_status_e status);

#endif  // DUALSHOCK4_CONNECTION_STATUS_MODIFY_H

