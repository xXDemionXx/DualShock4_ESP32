#ifndef BTN_CONFIG_H
#define BTN_CONFIG_H

#include "ds4_btns.h"
#include <stdint.h>

typedef struct
{
    const uint8_t byte_NUM; ///> Controller sends an array of bytes containing all the data, on which byte is the state
    const uint8_t bit_NUM;  ///> Controller sends an array of bytes containing all the data, on which bit is the state
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
    DS4_BIT_IN_DATA_DPAD_RIGHT_BTN = 2,
    DS4_BIT_IN_DATA_DPAD_LEFT_BTN = 3,
    DS4_BIT_IN_DATA_L1_BTN = 228,
    DS4_BIT_IN_DATA_R1_BTN = 229,
    DS4_BIT_IN_DATA_L3_BTN = 232,
    DS4_BIT_IN_DATA_R3_BTN = 233,
    DS4_BIT_IN_DATA_OPTIONS_BTN = 242,
    DS4_BIT_IN_DATA_SHARE_BTN = 241,
    DS4_BIT_IN_DATA_PS_BTN = 240,
};

// Helper macros for filling the ds4_button_configs struct
#define DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(x) x / 8
#define DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(x) x % 8

// Used for filling the settings
const ds4_button_config_t ds4_button_configs[DS4_NUM_OF_BUTTONS] = {
    [DS4_BTN_X] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_X_BTN),
               .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_X_BTN)},

    [DS4_BTN_CIRCLE] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_CIRCLE_BTN),
                    .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_CIRCLE_BTN)},

    [DS4_BTN_SQUARE] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_SQUARE_BTN),
                    .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_SQUARE_BTN)},

    [DS4_BTN_TRIANGLE] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_TRIANGLE_BTN),
                      .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_TRIANGLE_BTN)},

    [DS4_BTN_DPAD_UP] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_UP_BTN),
                     .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_UP_BTN)},

    [DS4_BTN_DPAD_DOWN] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_DOWN_BTN),
                       .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_DOWN_BTN)},

    [DS4_BTN_DPAD_LEFT] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_LEFT_BTN),
                       .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_LEFT_BTN)},

    [DS4_BTN_DPAD_RIGHT] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_DPAD_RIGHT_BTN),
                        .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_DPAD_RIGHT_BTN)},

    [DS4_BTN_L1] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_L1_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_L1_BTN)},

    [DS4_BTN_R1] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_R1_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_R1_BTN)},

    [DS4_BTN_L3] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_L3_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_L3_BTN)},

    [DS4_BTN_R3] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_R3_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_R3_BTN)},

    [DS4_BTN_OPTIONS] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_OPTIONS_BTN),
                     .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_OPTIONS_BTN)},

    [DS4_BTN_SHARE] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_SHARE_BTN),
                   .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_SHARE_BTN)},

    [DS4_BTN_PS] = {.byte_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BYTE_NUM(DS4_BIT_IN_DATA_PS_BTN),
                .bit_NUM = DS4_NUM_OF_BIT_IN_DATA_TO_BIT_NUM(DS4_BIT_IN_DATA_PS_BTN)},
};

// Helper macro that extracts a certain bit from controller data when given which byte and bit it is on
#define DS4_EXTRACT_BTN_STATE(data, btn_number)                   \
    (*(((char *)&data) + ds4_button_configs[btn_number].byte_NUM) \
     >> ds4_button_configs[btn_number].bit_NUM) &                 \
        DS4_BTN_STATE_PRESSED

#endif // BTN_CONFIG_H
