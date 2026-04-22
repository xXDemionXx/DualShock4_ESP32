#ifndef DS4_BTN_EVENTS_H
#define DS4_BTN_EVENTS_H

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

#endif  // BTN_EVENTS_H
