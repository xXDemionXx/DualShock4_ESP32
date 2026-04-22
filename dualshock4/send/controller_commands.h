#ifndef CONTROLLER_COMMANDS_H
#define CONTROLLER_COMMANDS_H

#include "controller_behaviour.h"
#include <stdint.h>

// Public types

typedef enum
{
    DS4_COMMAND_LIGHTBAR,
    DS4_COMMAND_RUMBLE,
    //
    DS4_NUM_OF_COMMAND_TYPES
} ds4_command_types_e;

typedef enum
{
    DS4_COMMAND_STATUS_AVAILABLE = 0,
    DS4_COMMAND_STATUS_UNAVAILABLE,
    DS4_COMMAND_STATUS_FAILED_SEND
} ds4_command_execution_status;

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} ds4_command_lightbar_t;

typedef struct
{
    uint8_t magnitude_weak;
    uint8_t magnitude_strong;
    uint16_t duration;
    uint16_t start_delay;
} ds4_command_rumble_t;

typedef union
{
    ds4_command_lightbar_t lightbar;
    ds4_command_rumble_t rumble;
} ds4_command_data_t;

typedef struct
{
    ds4_command_execution_status status; ///>
    ds4_device_handle device;            ///> Handle of the device to which the command will be sent
    ds4_command_data_t data;             ///> The payload
} ds4_command_t;

// Public functions

/**
 * @brief Set the lightbar (use only as an argument for btstack's callback registration)
 *
 * @param p_parameter Pointer to ds4_command_t parameters with ds4_command_lightbar_t data
 */
void ds4_lightbar_callback(void *p_parameter);

/**
 * @brief Set the rumble (use only as an argument for btstack's callback registration)
 *
 * @param p_parameter Pointer to ds4_command_t parameters with ds4_command_rumble_t data
 */
void ds4_rumble_callback(void *p_parameter);

#endif // CONTROLLER_COMMANDS_H
