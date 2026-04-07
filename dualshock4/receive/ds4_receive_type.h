#ifndef DS4_RECEIVE_TYPES_H
#define DS4_RECEIVE_TYPES_H

#include <stdint.h>

/**
 * @brief A struct used to read data from the controller when polling
 *
 * @note This struct uses bit fields and is not portable due to compiller specific behavioure regarding bit fields
 */
typedef struct
{
    struct
    {
        uint32_t U : 1;
        uint32_t D : 1;
        uint32_t R : 1;
        uint32_t L : 1;
        uint32_t : 28; // Protocol padding
    } dpad;

    struct
    {
        int32_t x;
        int32_t y;
    } ljoy;

    struct
    {
        int32_t x;
        int32_t y;
    } rjoy;

    int32_t L2;
    int32_t R2;

    struct
    {
        uint8_t cross : 1;
        uint8_t circle : 1;
        uint8_t square : 1;
        uint8_t triangle : 1;
        uint8_t L1 : 1;
        uint8_t R1 : 1;
        uint8_t : 2; // Protocol padding
        uint8_t L3 : 1;
        uint8_t R3 : 1;
        uint8_t : 6; // Protocol padding
        uint16_t PS_button : 1;
        uint16_t share : 1;
        uint16_t options : 1;
        uint16_t : 13; // Protocol padding
    } buttons;

    struct // Note - not sure this is the ordering of coordinates, needs testing
    {
        int32_t x;
        int32_t y;
        int32_t z;
    } gyro;

    struct // Note - not sure this is the ordering of coordinates, needs testing
    {
        int32_t x;
        int32_t y;
        int32_t z;
    } accel;

    uint8_t battery; // Range: 0-255

} ds4_data_t;

#endif
