#ifndef DUALSHOCK4_DEVICE_PASS_H
#define DUALSHOCK4_DEVICE_PASS_H

#include "dualshock4_initializer.h"

/**
 * @brief Set the ds4 handle
 *
 * This function exists solely as a utility
 * within the library.
 */
void ds4_set_device_handle(ds4_device_handle *device);

/**
 * @brief Passes the handle to dualshock4 API's variable.
 */
void ds4_pass_device_handle(ds4_device_handle device);

/**
 * @brief Passes the handle to dualshock4 API's variable.
 */
ds4_device_handle ds4_getd_evice_handle(void);

#endif // DUALSHOCK4_DEVICE_PASS_H
