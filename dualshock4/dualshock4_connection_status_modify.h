#ifndef DUALSHOCK4_CONNECTION_STATUS_MODIFY_H
#define DUALSHOCK4_CONNECTION_STATUS_MODIFY_H

typedef enum
{
    DS4_READY,
    DS4_CONNECTED,
    DS4_DISCONNECTING,
    DS4_DISCONNECTED
} ds4_connection_status_e;

/**
 * @brief Sets the connection status
 */
void set_ds4_connection_status(ds4_connection_status_e status);

ds4_connection_status_e ds4GetConnectionStatus(void);

#endif // DUALSHOCK4_CONNECTION_STATUS_MODIFY_H
