#ifndef DS4_BTNS_H
#define DS4_BTNS_H

typedef enum
{
    DS4_BTN_X,
    DS4_BTN_CIRCLE,
    DS4_BTN_SQUARE,
    DS4_BTN_TRIANGLE,
    DS4_BTN_DPAD_UP,
    DS4_BTN_DPAD_DOWN,
    DS4_BTN_DPAD_LEFT,
    DS4_BTN_DPAD_RIGHT,
    DS4_BTN_L1,
    DS4_BTN_R1,
    DS4_BTN_L3,
    DS4_BTN_R3,
    DS4_BTN_OPTIONS,
    DS4_BTN_SHARE,
    DS4_BTN_PS,

    // Do not change the last enum member
    DS4_NUM_OF_BUTTONS
} btn_e;

/**
 * @brief This enum connects the data representation and the physical state of the button
 */
typedef enum
{
    DS4_BTN_STATE_PRESSED = 1,
    DS4_BTN_STATE_RELEASED = 0
} ds4_btn_position_e;

#endif // DS4_BTNS_H
