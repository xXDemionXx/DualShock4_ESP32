#include "event_handling.h"
#include "ds4_btns.h"
#include "ds4_btn_events.h"
#include "ds4_receive_type.h"
#include "ds4_event_handling_init.h"
#include "btn_config.h"
#include "ds4_btn_events.h"
#include "btn_event_masks.h"
#include "freertos/queue.h"
#include <stdint.h>
#include <string.h>

// Defines

#define KB_TO_B(x) x * 1024
#define DS4_BTN_EVENT_TASK_SIZE 3 // In kB
#define DS4_BTN_EVENT_TASK_NAME "DS4 buttons event handler"
#define DS4_BTN_EVENT_TASK_PRIORITY 10
#define DS4_BTN_EVENT_MASK_FROM_EVENT(event) (1 << event)

/**
 * @brief Holds the event settings of the button
 */
typedef struct
{
    btn_event_bits_t set_events; ///> Fill this variable with ds4_btn_event_masks_e events that you want to use
    // uint16_t debounce_on_time;
    // uint16_t debounce_off_time;
    void (*trigger_func[DS4_NUM_OF_BTN_EVENTS])(void *);
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

/**
 * @brief Task that checks if button has events set for checking, reeds their states and calls the event checker.
 */
static void event_handler_task(void *p_parameter);

/**
 * @brief Check all the events on the button set for monitoring, calls the trigger function if event found.
 */
static void event_checker(btn_t *btn);

static inline ds4_btn_event_e event_check_btn_press(uint8_t state, uint8_t prev_state);
static inline ds4_btn_event_e event_check_btn_release(uint8_t state, uint8_t prev_state);
static inline void trigger_event_function(uint8_t found_event, btn_event_settings_s *settings);

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
    {
        vTaskSuspend(task_handle);
        // Reset the button states
        for (uint8_t current_button = 0; current_button < DS4_NUM_OF_BUTTONS; current_button++)
        {
            buttons[current_button].states.current_state = DS4_BTN_STATE_RELEASED;
            buttons[current_button].states.prev_state = DS4_BTN_STATE_RELEASED;
        }
    }
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
    if (pdPASS != xTaskCreate(&event_handler_task, DS4_BTN_EVENT_TASK_NAME, KB_TO_B(DS4_BTN_EVENT_TASK_SIZE), &queue_handle, DS4_BTN_EVENT_TASK_PRIORITY, &task_handle))
    {
        return DS4_INIT_EVENT_TASK_FAILED;
    }
    ds4_suspend_buttons_event_handler(); // Do not start the task immediately, start it when a controller connects

    return DS4_INIT_EVENT_SUCCES;
}

void ds4SetButtonEvent(btn_e button, const ds4_btn_event_e event, void (*trigger_func)(void *), void *argv)
{

    buttons[button].event_settings.set_events |= DS4_BTN_EVENT_MASK_FROM_EVENT(event);
    buttons[button].event_settings.trigger_func[event] = trigger_func;
    buttons[button].event_settings.argv = argv;

    // Cleanup from before if something in states was left
    memset(&buttons[button].states, 0, sizeof(buttons[button].states));
}

// Private functions

static void event_handler_task(void *p_parameter)
{
    ds4_data_t data;
    uint8_t current_button = 0;
    for (;;)
    {
        if (pdTRUE == xQueueReceive(queue_handle, &data, portMAX_DELAY))
        {
            for (; current_button < DS4_NUM_OF_BUTTONS; current_button++)
            {
                // Only check buttons that have events set on them
                if (buttons[current_button].event_settings.set_events != 0)
                {
                    // Get current state
                    buttons[current_button].states.current_state =
                        DS4_EXTRACT_BTN_STATE(data, current_button);

                    // Call event checker
                    event_checker(&buttons[current_button]);

                    // Update previous state
                    buttons[current_button].states.prev_state =
                        buttons[current_button].states.current_state;
                }
            }
            current_button = 0;
        }
    }
}

void event_checker(btn_t *btn)
{
    btn_event_bits_t found_event_MASK;
    uint8_t found_event = DS4_BTN_EVENT_NO_EVENT;
    uint8_t current_state = btn->states.current_state;
    uint8_t prev_state = btn->states.prev_state;
    for (uint8_t i = 0; i < DS4_NUM_OF_BTN_EVENTS; i++)
    {
        found_event_MASK = DS4_BTN_EVENT_MASK_FROM_EVENT(i) & btn->event_settings.set_events;
        switch (found_event_MASK)
        {
        case (DS4_BTN_PRESS_MASK):
            found_event = event_check_btn_press(current_state, prev_state);
            break;
        case (DS4_BTN_RELEASE_MASK):
            found_event = event_check_btn_release(current_state, prev_state);
            break;
        case (DS4_BTN_NO_EVENT_MASK):
        default:
            // Continue to the next iteration if that event isn't being monitored
            continue;
        }
        if (found_event != DS4_BTN_EVENT_NO_EVENT)
            trigger_event_function(found_event, &btn->event_settings);
    }
}

static inline void trigger_event_function(uint8_t found_event, btn_event_settings_s *settings)
{
    // Call the function indexed by the event type in the settings
    settings->trigger_func[found_event](settings->argv);
}

static inline ds4_btn_event_e event_check_btn_press(uint8_t state, uint8_t prev_state)
{
    if (prev_state == DS4_BTN_STATE_RELEASED && state == DS4_BTN_STATE_PRESSED)
        return DS4_BTN_EVENT_PRESS;
    else
    {
        return DS4_BTN_EVENT_NO_EVENT;
    }
}

inline static ds4_btn_event_e event_check_btn_release(uint8_t state, uint8_t prev_state)
{
    if (prev_state == DS4_BTN_STATE_PRESSED && state == DS4_BTN_STATE_RELEASED)
        return DS4_BTN_EVENT_RELEASE;
    else
        return DS4_BTN_EVENT_NO_EVENT;
}
