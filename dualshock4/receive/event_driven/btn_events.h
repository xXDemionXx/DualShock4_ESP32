#ifndef BTN_EVENTS_H
#define BTN_EVENTS_H

#include "btns.h"

/**
 * @brief Enum that holds the bit positions of each event type in the buttons set events.
 */
typedef enum{
    DS4_BTN_NO_EVENT    = 0x0,
    DS4_BTN_PRESS       = 0x1,
    DS4_BTN_NO_RELEASE  = 0x2,
    DS4_BTN_HOLD        = 0x4,
    DS4_POSSIBLE_EVENTS // Which values bellow contain event bits
} ds4_btn_event_e;

#if DS4_POSSIBLE_EVENTS <= 0xFF
typedef uint8_t btn_event_bits_t;
#elif DS4_POSSIBLE_EVENTS <= 0xFFFF
typedef uint16_t btn_event_bits_t;
#else
typedef uint32_t btn_event_bits_t;
#endif

void ds4SetButtonEvent(btn_e button, const btn_event_bits_t event, void (*trigger_func)(void *), void *argv);
#endif  // BTN_EVENTS_H
