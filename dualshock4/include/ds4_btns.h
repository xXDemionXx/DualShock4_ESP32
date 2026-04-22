#ifndef DS4_BTNS_H
#define DS4_BTNS_H

typedef enum
{
    X_BTN,
    CIRCLE_BTN,
    SQUARE_BTN,
    TRIANGLE_BTN,
    DPAD_UP_BTN,
    DPAD_DOWN_BTN,
    DPAD_LEFT_BTN,
    DPAD_RIGHT_BTN,
    L1_BTN,
    R1_BTN,
    L3_BTN,
    R3_BTN,
    OPTIONS_BTN,
    SHARE_BTN,
    PS_BTN,

    // Do not change the last enum meber
    DS4_NUM_OF_BUTTONS
} btn_e;

/**
 * @brief This enum connects the data representation and the physical truth
 */
typedef enum
{
    DS4_BTN_STATE_PRESSED = 1,
    DS4_BTN_STATE_RELEASED = 0
} ds4_btn_position_e;

#endif // BTN_EVENT_STRUCT_H
