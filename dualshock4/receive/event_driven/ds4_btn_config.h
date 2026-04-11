#ifndef BTN_DATA_MASKS_H
#define BTN_DATA_MASKS_H

#include "btns.h"
#include <stdint.h>

typedef struct
{
    const uint8_t byte_NUM;    ///> Controller sends an array of bytes containing all the data, on which byte is the state
    const uint8_t bit_NUM;     ///> Controller sends an array of bytes containing all the data, on which bit is the state
} ds4_button_config_t;

// Numbering starts from 0!!!
// Check this struct, need to confirm if everything is correct
enum
{
    DS4_BIT_IN_DATA_X_BTN = 224,
    DS4_BIT_IN_DATA_CIRCLE_BTN = 225,
    DS4_BIT_IN_DATA_SQUARE_BTN = 226,
    DS4_BIT_IN_DATA_TRIANGLE_BTN = 227,
    DS4_BIT_IN_DATA_DPAD_UP_BTN = 0,
    DS4_BIT_IN_DATA_DPAD_DOWN_BTN = 1,
    DS4_BIT_IN_DATA_DPAD_LEFT_BTN = 2,
    DS4_BIT_IN_DATA_DPAD_RIGHT_BTN = 3,
    DS4_BIT_IN_DATA_L1_BTN = 228,
    DS4_BIT_IN_DATA_R1_BTN = 229,
    DS4_BIT_IN_DATA_L3_BTN = 232,
    DS4_BIT_IN_DATA_R3_BTN = 233,
    DS4_BIT_IN_DATA_OPTIONS_BTN = 242,
    DS4_BIT_IN_DATA_SHARE_BTN = 241,
    DS4_BIT_IN_DATA_PS_BTN = 240,
};

// Helper macros
#define DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(x) x / 8
#define DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(x) x % 8

// Used for filling the settings
const ds4_button_config_t ds4_button_configs[DS4_NUM_OF_BUTTONS] = {
    [X_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_X_BTN),
               .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_X_BTN)},

    [CIRCLE_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_CIRCLE_BTN),
                    .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_CIRCLE_BTN)},

    [SQUARE_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_SQUARE_BTN),
                    .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_SQUARE_BTN)},

    [TRIANGLE_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_TRIANGLE_BTN),
                      .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_TRIANGLE_BTN)},

    [DPAD_UP_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_UP_BTN),
                     .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_UP_BTN)},

    [DPAD_DOWN_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_DOWN_BTN),
                       .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_DOWN_BTN)},

    [DPAD_LEFT_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_LEFT_BTN),
                       .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_LEFT_BTN)},

    [DPAD_RIGHT_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_RIGHT_BTN),
                        .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_RIGHT_BTN)},

    [L1_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_L1_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_L1_BTN)},

    [R1_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_R1_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_R1_BTN)},

    [L3_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_L3_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_L3_BTN)},

    [R3_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_R3_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_R3_BTN)},

    [OPTIONS_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_OPTIONS_BTN),
                     .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_OPTIONS_BTN)},

    [SHARE_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_SHARE_BTN),
                   .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_SHARE_BTN)},

    [PS_BTN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_PS_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_PS_BTN)},
};

#endif // BTN_DATA_MASKS_H
