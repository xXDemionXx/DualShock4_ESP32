#ifndef DUALSHOCK4_DEFAULT_FUNCS_H
#define DUALSHOCK4_DEFAULT_FUNCS_H

// Includes

#include "dualshock4_initializer.h"
#include "uni.h"


// Defines

#define DUALSHOCK4_DEFAULT_NAME "dualshock4" // Name of the controller (prefixes it's messages to the console)
#define DS4_DEFAULT_SEAT    GAMEPAD_SEAT_A


// Function prototypes

/**
 * @brief Do something for controller during init
 *
 * Currently we aren't doing anything special.
 *
 * @param argc Number of arguments (UNUSED)
 * @param argv Vector of arguments (UNUSED)
 */
void ds4_on_init(int argc, const char **argv);

/**
 * @brief Do something for controller after init
 *
 * Currently we aren't doing anything special.
 */
void ds4_on_init_complete(void);

/**
 * @brief Filter out the devices we don't want.
 *
 * Filter everything that isn't a DS4 controller.
 *
 * @param addr the Bluetooth address
 * @param name could be NULL, could be zero-length, or might contain the name
 * @param cod Class of Device. See "uni_bt_defines.h" for possible values
 * @param rssi Received Signal Strength Indicator (RSSI) measured in dBms. The higher (255) the better.
 * @return UNI error code, see uni_error.h
 */
uni_error_t ds4_on_device_discovered(bd_addr_t addr, const char *name, uint16_t cod, uint8_t rssi);

/**
 * @brief What to do when a device connects
 *
 * Currently nothing except logging.
 *
 * @param d connected device handle
 */
void ds4_on_device_connected(uni_hid_device_t *d);

/**
 * @brief What to do when a device disconnects
 *
 * Currently nothing except logging.
 *
 * @param d disconnected device handle
 */
void ds4_on_device_disconnected(uni_hid_device_t *d);

/**
 * @brief What to do when a device is ready for communication
 *
 * Currently nothing except logging and setting seat of instance.
 *
 * @param d ready device handle
 * @return UNI error code, see uni_error.h
 */
uni_error_t ds4_on_device_ready(uni_hid_device_t *d);

/**
 * @brief What to do on OOB events
 *
 * On DS4 OOB events are the pressing of system button
 * and bluetooth scanning.
 *
 * @param event event that triggered
 * @param data data of the event
 */
void default_ds4_platform_on_oob_event(uni_platform_oob_event_t event, void *data);

/**
 * @brief What to do when controller data arrives
 *
 * Currently just logging
 *
 * @param d device that sent the data
 * @param ctl controllers data
 */
void default_ds4_platform_on_controller_data(uni_hid_device_t *d, uni_controller_t *ctl);

/**
 * @brief Gets the property of controller
 *
 * Needs further research
 */
const uni_property_t *default_ds4_platform_get_property(uni_property_idx_t idx);

#endif // DUALSHOCK4_DEFAULT_FUNCS_H
