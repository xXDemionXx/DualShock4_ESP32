#ifndef DUALSHOCK4_TASK_COMMANDS_H
#define DUALSHOCK4_TASK_COMMANDS_H

// Includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "stdint.h"

// Defines
#define NUM_TO_KB(x) x * 1024
#define DS4_COMMAND_TASK_SIZE 5                         // In kB
#define DS4_COMMAND_TASK_PRIORITY 5
#define DS4_COMMAND_TASK_QUEUE_SIZE 3                   // How many commands can fit
#define DS4_COMMAND_TASK_MAX_TEST_WRITE_STRING_SIZE 20  // In chars

// Types
typedef enum
{
    DS4_COMMAND_TASK_INIT_SUCCES,
    DS4_COMMAND_TASK_INIT_FAILED_QUEUE,
    DS4_COMMAND_TASK_INIT_FAILED_TASK,
    DS4_COMMAND_TASK_INIT_STARTING_FAILURE
} ds4_command_task_init_error_e;

typedef struct
{
    ds4_command_task_init_error_e error_code;
    QueueHandle_t queue_handle;
} ds4_command_task_init_return_t;

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} ds4_command_change_lightbar;

typedef struct
{
    char string[DS4_COMMAND_TASK_MAX_TEST_WRITE_STRING_SIZE];
} ds4_command_test_write;

typedef union
{
    ds4_command_change_lightbar lightbar_command;
    ds4_command_test_write test_write_command;
} ds4_command_types_u;

typedef enum
{
    DS4_COMMAND_TYPE_CHANGE_LIGHTBAR,
    DS4_COMMAND_TYPE_RUMBLE,
    DS4_COMMAND_TYPE_TEST_WRITE
} ds4_command_type_e;

typedef struct
{
    uint8_t command_indicator; // Use ds4_command_type_e for this
    ds4_command_types_u data;
} ds4_command_s;

// Function prototypes

/**
 * @brief Initializes the commands task, returns an error code and queue handle.
 *
 * @return Struct {error code, queue handle}
 */
ds4_command_task_init_return_t ds4_init_commands_task(void);

#endif
