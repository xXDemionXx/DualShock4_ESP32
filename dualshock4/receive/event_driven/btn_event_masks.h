#ifndef BTN_EVENT_MASKS_H
#define BTN_EVENT_MASKS_H

#include <stdint.h>

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

#endif  // BTN_EVENT_MASKS_H
