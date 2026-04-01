#include "dualshock4_connection_status.h"
#include "dualshock4_connection_status_modify.h"

static _Atomic ds4_connection_status_e ds4_connection_status = DS4_DISCONNECTED;

inline ds4_connection_status_e ds4GetConnectionStatus(void)
{
    return atomic_load(&ds4_connection_status);
}

_Atomic ds4_connection_status_e *access_ds4_connection_status(void)
{
    return &ds4_connection_status;
}
