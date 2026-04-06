#ifndef CONTROLLER_DATA_TYPE_H
#define CONTROLLER_DATA_TYPE_H

#include <stdint.h>

typedef struct
{
    struct
    {
        uint32_t L : 1;
        uint32_t U : 1;
        uint32_t D : 1;
        uint32_t R : 1;
        uint32_t padding : 28;
    } dpad;

    struct
    {
        int32_t x;
        int32_t y;
    } Ljoy;

    struct
    {
        int32_t x;
        int32_t y;
    } Rjoy;
    int32_t L2;
    int32_t R2;
    uint16_t buttons;
    uint8_t misc_buttons;

    struct
    {
        int32_t x;
        int32_t y;
        int32_t z;
    } gyro;

    struct
    {
        int32_t x;
        int32_t y;
        int32_t z;
    } accel;

} ds4_data_t;

#endif
