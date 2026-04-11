#include "btns.h"
#include "btn_events.h"
#include "stdint.h"
#include <string.h>

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
    btn_states_s btn_states;
    btn_event_settings_s btn_event_settings;
} btn_t;

static btn_t buttons[DS4_NUM_OF_BUTTONS] = {0};

void ds4SetButtonEvent(btn_e button, const btn_event_bits_t event, void (*trigger_func)(void *), void *argv)
{

    buttons[button].btn_event_settings.set_events |= event;
    buttons[button].btn_event_settings.trigger_func = trigger_func;
    buttons[button].btn_event_settings.argv = argv;

    // Cleanup from before if something in states was left
    memset(&buttons[button].btn_states, 0, sizeof(buttons[button].btn_states));
}
