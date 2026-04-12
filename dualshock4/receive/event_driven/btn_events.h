#ifndef BTN_EVENTS_H
#define BTN_EVENTS_H

#include "btns.h"
#include <stdint.h>

// Helper 
#define DS4_BTN_EVENT_MASK_FROM_EVENT(event)    (1 << event)

/**
 * @brief Enum that holds all the possible event types for a button
 * 
 * If you add a new event, make sure to add an apropreate bit mask in ds4_btn_event_masks_e
 */
typedef enum{
    DS4_BTN_EVENT_PRESS,
    DS4_BTN_EVENT_RELEASE,
    // DS4_BTN_EVENT_HOLD,  // Might implement in the future
    //
    DS4_NUM_OF_BTN_EVENTS,
    //
    DS4_BTN_EVENT_NO_EVENT = 0xFF   // Sentinal value used to indicate no event was found
} ds4_btn_event_e;

/**
 * @brief Enum that holds the bit positions of each event type in the buttons set events.
 * 
 * If you add a new event, make sure the mask correlates with the bit position of ds4_btn_event_e
 */
typedef enum{
    DS4_BTN_NO_EVENT_MASK   = 0x0,
    DS4_BTN_PRESS_MASK      = 0x1,
    DS4_BTN_RELEASE_MASK    = 0x2,
    // DS4_BTN_HOLD_MASK       = 0x4,   // Might implement in the future
    //
    DS4_POSSIBLE_EVENT_MASKS // Which values bellow contain event bits
} ds4_btn_event_masks_e;

#if DS4_POSSIBLE_EVENT_MASKS <= 0xFF
typedef uint8_t btn_event_bits_t;
#elif DS4_POSSIBLE_EVENT_MASKS <= 0xFFFF
typedef uint16_t btn_event_bits_t;
#else
typedef uint32_t btn_event_bits_t;
#endif

void ds4SetButtonEvent(btn_e button, const ds4_btn_event_e event, void (*trigger_func)(void *), void *argv);
#endif  // BTN_EVENTS_H
