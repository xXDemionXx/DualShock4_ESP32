#include "btn_events.h"
#include "ds4_event_handling.h"
#include "freertos/queue.h"
#include "ds4_receive_type.h"
#include "ds4_event_handling_init.h"

static QueueHandle_t queue_handle = NULL;
static TaskHandle_t task_handle = NULL;

static void event_handler(void *p_parameter);

// Public functions

BaseType_t ds4_send_data_to_event_handler(ds4_data_t *data)
{
    if (queue_handle != NULL)
    {
        return xQueueOverwrite(queue_handle, data);
    }
    return pdFAIL; // Noninitialized queue
}

void ds4_suspend_buttons_event_handler()
{
    if (task_handle != NULL)
        vTaskSuspend(task_handle);
}

void ds4_resume_buttons_event_handler()
{
    if (task_handle != NULL)
        vTaskResume(task_handle);
}

ds4_event_handling_init_e ds4_init_buttons_event_handler()
{
    // Create queue for receiving current button data
    queue_handle = xQueueCreate(1, sizeof(ds4_data_t));
    if (queue_handle == NULL)
    {
        return DS4_INIT_EVENT_QUEUE_FAILED;
    }

    // Create the event handler task
    if (pdPASS != xTaskCreate(&event_handler, "DS4 buttons event handler", 1024 * 3, &queue_handle, 6, &task_handle))
    {
        return DS4_INIT_EVENT_TASK_FAILED;
    }
    ds4_suspend_buttons_event_handler();    // Do not start the task immediately, start it when a controller connects
    
    return DS4_INIT_EVENT_SUCCES;
}

// Private functions

static void event_handler(void *p_parameter)
{
}
