#include "dualshock4_connection_status.h"
#include "dualshock4_connection_status_modify.h"
#include <stdatomic.h>

static _Atomic ds4_connection_status_e ds4_connection_status = DS4_DISCONNECTED;

inline ds4_connection_status_e ds4GetConnectionStatus(void)
{
    return atomic_load(&ds4_connection_status);
}

void set_ds4_connection_status(ds4_connection_status_e status)
{
    //if (status == DS4_READY || DS4_CONNECTED || DS4_DISCONNECTED) // Valid statuses
        atomic_store(&ds4_connection_status, status);
}
