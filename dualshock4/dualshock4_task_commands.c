#include "dualshock4_task_commands.h"

static void ds4_commands_task(void *p_parameter);

static QueueHandle_t commands_queue_handle = NULL;

ds4_command_task_init_error_e ds4_init_commands_task(QueueHandle_t commands_queue_handle)
{
    // Create the command queue which the command task will use to receive commands for the controller
    commands_queue_handle = xQueueCreate(DS4_COMMAND_TASK_QUEUE_SIZE, sizeof(ds4_command_s));

    if (commands_queue_handle == NULL)
        return DS4_COMMAND_TASK_INIT_FAILED_QUEUE;

    // Create the commands task
    if (pdPASS != xTaskCreate(&ds4_commands_task, "DS4 commands", DS4_COMMAND_TASK_SIZE, &commands_queue_handle, DS4_COMMAND_TASK_PRIORITY, NULL))
        return DS4_COMMAND_TASK_INIT_FAILED_TASK;

    return DS4_COMMAND_TASK_INIT_SUCCES;
}

static const char *TAG = "queue_example";
//---------------------------- PRIVATE FUNCTIONS ------------------------------
static void ds4_commands_task(void *p_parameter)
{
    QueueHandle_t commands_queue_handle = *(QueueHandle_t *)p_parameter;

    ds4_command_s command_packet;

    for (;;)
    {
        //
        if (pdTRUE == xQueueReceive(commands_queue_handle, &command_packet, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "NNNNNNNNN");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
