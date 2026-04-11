#include "btn_events.h"
#include "ds4_event_handling.h"
#include "freertos/queue.h"
#include "ds4_receive_type.h"
#include "ds4_event_handling_init.h"
#include "ds4_btn_config.h"
#include "stdint.h"
#include <string.h>

// Helper macro that extracts a certain bit from controller data when given which byte and bit it is on
#define DS4_EXTRACT_BTN_STATE(data_p, bit_NUM, byte_NUM)\
        (*(((char *)&data_p) + byte_NUM) << bit_NUM) & DS4_BTN_PRESSED

/**
 * @brief Holds the event settings of the button
 */
typedef struct
{
    btn_event_bits_t set_events; ///> Fill this variable with ds4_btn_event_e events that you want to use
    // uint16_t debounce_on_time;
    // uint16_t debounce_off_time;
    void (*trigger_func)(void *);
    void *argv; ///> Arguments vector that will be passed to the trigger_func
} btn_event_settings_s;

typedef struct
{
    uint8_t current_state;
    uint8_t prev_state;
    // uint16_t on_state_timer;
    // uint16_t off_state_timer;
} btn_states_s;

/**
 * @brief Holds the button event settings and states
 */
typedef struct
{
    btn_states_s states;
    btn_event_settings_s event_settings;
} btn_t;

// Private variables
static btn_t buttons[DS4_NUM_OF_BUTTONS];
static QueueHandle_t queue_handle = NULL;
static TaskHandle_t task_handle = NULL;

// Private function prototypes
static void event_checker_task(void *p_parameter);

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
    if (pdPASS != xTaskCreate(&event_checker_task, "DS4 buttons event handler", 1024 * 3, &queue_handle, 6, &task_handle))
    {
        return DS4_INIT_EVENT_TASK_FAILED;
    }
    ds4_suspend_buttons_event_handler(); // Do not start the task immediately, start it when a controller connects

    return DS4_INIT_EVENT_SUCCES;
}

void ds4SetButtonEvent(btn_e button, const btn_event_bits_t event, void (*trigger_func)(void *), void *argv)
{

    buttons[button].event_settings.set_events |= event;
    buttons[button].event_settings.trigger_func = trigger_func;
    buttons[button].event_settings.argv = argv;

    // Cleanup from before if something in states was left
    memset(&buttons[button].states, 0, sizeof(buttons[button].states));
}

// Private functions

static void event_checker_task(void *p_parameter)
{
    ds4_data_t data;
    uint8_t current_button = 0;
    for (;;)
    {
        if (pdTRUE == xQueueReceive(queue_handle, &data, portMAX_DELAY))
        {
            for(; current_button < DS4_NUM_OF_BUTTONS; current_button++){

                if(buttons[current_button].event_settings.set_events != 0){
                    buttons[current_button].states.current_state =
                    DS4_EXTRACT_BTN_STATE(data, ds4_button_configs[current_button].bit_NUM, ds4_button_configs[current_button].byte_NUM);
                }
            }
            current_button = 0;
        }
    }
}
