#ifndef DS4_RECEIVE_TYPES_H
#define DS4_RECEIVE_TYPES_H

#include <stdint.h>

typedef struct
{
    uint32_t _reserved; // Page data from bluepad32, we don't need this
    struct
    {
        uint8_t U : 1;
        uint8_t D : 1;
        uint8_t R : 1;
        uint8_t L : 1;
    } dpad __attribute__((packed));

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
        uint32_t cross : 1;
        uint32_t circle : 1;
        uint32_t square : 1;
        uint32_t triangle : 1;
        uint32_t L1 : 1;
        uint32_t L2 : 1;
        uint32_t : 2;
        uint32_t R1 : 1;
        uint32_t R2 : 1;
        uint32_t : 6;
        uint32_t PS_button : 1;
        uint32_t share : 1;
        uint32_t options : 1;
        uint32_t : 13;
    } buttons __attribute__((packed));
    // uint16_t buttons;
    // uint8_t misc_buttons;

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

    uint8_t battery; // range: 0-255

} ds4_data_t __attribute__((packed));

// typedef struct {
//     // Usage Page: 0x01 (Generic Desktop Controls)
//     uint32_t page;
//     uint8_t dpad;
//     int32_t axis_x;
//     int32_t axis_y;
//     int32_t axis_rx;
//     int32_t axis_ry;

//     // Usage Page: 0x02 (Sim controls)
//     int32_t brake;
//     int32_t throttle;

//     // Usage Page: 0x09 (Button)
//     uint16_t buttons;

//     // Misc buttons (from 0x0c (Consumer) and others)
//     uint8_t misc_buttons;

//     int32_t gyro[3];
//     int32_t accel[3];
// } ds4_data_t;

#endif
