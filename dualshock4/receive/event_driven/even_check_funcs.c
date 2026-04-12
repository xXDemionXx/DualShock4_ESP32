#include "event_check_funcs.h"
#include "btn_events.h"
#include "btns.h"

// // Helper macros
// #define CHECK_BIT_IN_WORD(bit, word)    ((1 << bit) & word)

// Private function prototypes
static inline ds4_btn_event_e event_check_btn_press(uint8_t state, uint8_t prev_state);
static inline ds4_btn_event_e event_check_btn_release(uint8_t state, uint8_t prev_state);
// static ds4_btn_event_e event_check_btn_hold(uint8_t state, uint8_t prev_state);  // Might implement in the future
static void event_handler(uint8_t found_event);


// Public function
static void event_checker(uint8_t state, uint8_t prev_state, btn_event_bits_t set_events)
{
    btn_event_bits_t found_event_MASK;
    uint8_t found_event;
    for (uint8_t i = 0; i < DS4_NUM_OF_BTN_EVENTS; i++)
    {
        found_event_MASK = (1 << i) & set_events;
        switch (found_event_MASK)
        {
        case (DS4_BTN_PRESS_MASK):
            found_event = event_check_btn_press(state, prev_state);
            break;
        case (DS4_BTN_RELEASE_MASK):
            event_check_btn_press();
            break;
        // case (DS4_BTN_HOLD_MASK):    // Might implement in the future
        //     event_check_btn_press();
        //     break;
        case (DS4_BTN_HOLD_MASK):
        default:
            // Do nothing if that event isn't set for monitoring
            break;
        }
        if(found_event != DS4_BTN_EVENT_NO_EVENT)
            event_handler(found_event);
    }
}

// Private functions

static void event_handler(uint8_t found_event)
{
    switch(found_event){
        case(DS4_BTN_EVENT_PRESS):
            break;
        case(DS4_BTN_EVENT_RELEASE):
            break;
    }
}

static inline ds4_btn_event_e event_check_btn_press(uint8_t state, uint8_t prev_state)
{
    if (prev_state == DS4_BTN_STATE_RELEASED && state == DS4_BTN_STATE_PRESSED)
        return DS4_BTN_EVENT_PRESS;
    else
        return DS4_BTN_EVENT_NO_EVENT;
}

inline static ds4_btn_event_e event_check_btn_release(uint8_t state, uint8_t prev_state){
    if (prev_state == DS4_BTN_STATE_PRESSED && state == DS4_BTN_STATE_RELEASED)
        return DS4_BTN_EVENT_RELEASE;
    else
        return DS4_BTN_EVENT_NO_EVENT;
}
