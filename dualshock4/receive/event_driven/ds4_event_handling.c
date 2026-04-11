#include "btn_events.h"
#include "ds4_event_handling.h"
#include "freertos/queue.h"
#include "ds4_receive_type.h"

static QueueHandle_t queue_handle = NULL;

static void event_handler(void *p_parameter);

// Public functions

BaseType_t ds4_send_data_to_event_handler(ds4_data_t *data)
{
    if (queue_handle != NULL){
        return xQueueOverwrite(queue_handle, data);
    }
    return -1;  // Noninitialized queue
}

void ds4_suspend_buttons_event_handler()
{
}

void ds4_resume_buttons_event_handler()
{
}

void ds4_init_buttons_event_handler()
{
    queue_handle = xQueueCreate(1, sizeof(ds4_data_t));

    if (queue_handle == NULL)
    {
    }

    // Create the commands task
    if (pdPASS != xTaskCreate(&event_handler, "DS4 buttons event handler", 1024 * 3, &queue_handle, 6, NULL))
    {
    }
}

// Private functions

static void event_handler(void *p_parameter)
{
}
