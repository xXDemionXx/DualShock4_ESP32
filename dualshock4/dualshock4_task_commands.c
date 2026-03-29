#include "dualshock4_task_commands.h"

// Private variables
static QueueHandle_t commands_queue_handle = NULL;

// Private function prototypes
static void ds4_commands_task(void *p_parameter);
static void change_lightbar(void);
static void rumble(void);
static void test_write(ds4_command_test_write);

ds4_command_task_init_return_t ds4_init_commands_task(void)
{
    ds4_command_task_init_return_t return_vals;
    return_vals.error_code = DS4_COMMAND_TASK_INIT_STARTING_FAILURE;

    // Create the command queue which the command task will use to receive commands for the controller
    commands_queue_handle = xQueueCreate(DS4_COMMAND_TASK_QUEUE_SIZE, sizeof(ds4_command_s));

    if (commands_queue_handle == NULL)
    {
        return_vals.error_code = DS4_COMMAND_TASK_INIT_FAILED_QUEUE;
        return return_vals;
    }
    // Queue was created succesfully, we can store the handle for returning
    return_vals.queue_handle = commands_queue_handle;

    // Create the commands task
    if (pdPASS != xTaskCreate(&ds4_commands_task, "DS4 commands", DS4_COMMAND_TASK_SIZE, &commands_queue_handle, DS4_COMMAND_TASK_PRIORITY, NULL))
    {
        return_vals.error_code = DS4_COMMAND_TASK_INIT_FAILED_TASK;
        return return_vals;
    }
    // Everything passed
    return_vals.error_code = DS4_COMMAND_TASK_INIT_SUCCES;
    return return_vals;
}

static const char *TAG = "queue_example";

// Public functions
QueueHandle_t ds4_command_task_get_queue_handle(void)
{
    return commands_queue_handle;
}

// Private functions

static void ds4_commands_task(void *p_parameter)
{
    QueueHandle_t commands_queue_handle = *(QueueHandle_t *)p_parameter;
    ds4_command_s command_packet;

    for (;;)
    {
        if (pdTRUE == xQueueReceive(commands_queue_handle, &command_packet, portMAX_DELAY))
        {
            switch (command_packet.command_indicator)
            {
            case (DS4_COMMAND_TYPE_CHANGE_LIGHTBAR):
                change_lightbar();
                break;
            case (DS4_COMMAND_TYPE_RUMBLE):
                rumble();
                break;
            case (DS4_COMMAND_TYPE_TEST_WRITE):
                test_write(command_packet.data.test_write_command);
                break;
            default:
                ESP_LOGI(TAG, "Unknown command for DS4");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Possible commands

static void change_lightbar(void)
{
    // For now nothing
    ESP_LOGI(TAG, "Change lightbar");
}

static void rumble(void)
{
    // For now nothing
    ESP_LOGI(TAG, "Rumble");
}

static void test_write(ds4_command_test_write command)
{
    // For now nothing
    ESP_LOGI(TAG, "Received text: %s", command.string);
}
