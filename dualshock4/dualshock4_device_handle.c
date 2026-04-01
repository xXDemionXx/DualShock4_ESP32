#include "dualshock4_device_handle.h"

// The device handle, only ever one used.
static ds4_device_handle *ds4_p = NULL;

void ds4_set_device_handle(ds4_device_handle *p)
{
    ds4_p = p;
}

void ds4_pass_device_handle(ds4_device_handle device)
{
    *ds4_p = device;
}
