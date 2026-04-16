#include "btstack_defines.h"
#include "dualshock4_initializer.h"
#include "controller_commands.h"

void ds4_lightbar_callback(void *p_parameter)
{
    ds4_command_t command = *(ds4_command_t *)p_parameter;
    ds4_device_handle d = command.device;

    if (!d)
        return;

    // If the controller disconnected in the meantime
    if (!uni_bt_conn_is_connected(&d->conn))
        return;

    ds4_command_lightbar_t data = command.data.lightbar;

    if (d->report_parser.set_lightbar_color)
        d->report_parser.set_lightbar_color(
            d,
            data.R,
            data.G,
            data.B
        );
}

void ds4_rumble_callback(void *p_parameter)
{
    ds4_command_t command = *(ds4_command_t *)p_parameter;
    ds4_device_handle d = command.device;

    if (!d)
        return;

    // If the controller disconnected in the meantime
    if (!uni_bt_conn_is_connected(&d->conn))
        return;

    ds4_command_rumble_t data = command.data.rumble;

    if (d->report_parser.play_dual_rumble != NULL)
        d->report_parser.play_dual_rumble(
            d,
            data.start_delay,
            data.duration,
            data.magnitude_weak,
            data.magnitude_strong
        );
}
