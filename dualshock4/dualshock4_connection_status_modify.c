#include "dualshock4_connection_status_modify.h"
#include "dualshock4_connection_status.h"

static _Atomic ds4_connection_status_e *connection_status_p;

void set_ds4_connection_status(ds4_connection_status_e status)
{
    if (status == DS4_READY || DS4_CONNECTED || DS4_DISCONNECTED) // Valid statuses
        atomic_store(connection_status_p, status);
}

void store_ds4_connection_status_pointer(_Atomic ds4_connection_status_e *p)
{
    connection_status_p = p;
}

void ds4_init_connection_status(void){
    store_ds4_connection_status_pointer(access_ds4_connection_status);
}

