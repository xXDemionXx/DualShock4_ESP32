#ifndef DUALSHOCK4_CONNECTION_STATUS_H
#define DUALSHOCK4_CONNECTION_STATUS_H

#include <stdatomic.h>

typedef enum
{
    DS4_READY,
    DS4_CONNECTED,
    DS4_DISCONNECTING,
    DS4_DISCONNECTED
} ds4_connection_status_e;

/**
 * @brief Returns the connection status
 *
 * Possible states are: disconnected, connected, ready.
 * Usually you should ignore connected status and only
 * interact with the controller when it is ready.
 */
ds4_connection_status_e ds4GetConnectionStatus(void);

#endif // DUALSHOCK4_CONNECTION_STATUS_H
