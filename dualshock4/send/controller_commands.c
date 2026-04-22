#include "btstack_defines.h"
#include "controller_behaviour.h"
#include "controller_commands.h"

void ds4_lightbar_callback(void *p_parameter)
{
    ds4_command_t command = *(ds4_command_t *)p_parameter;
    ds4_device_handle d = command.device;

    if (!d){
        ((ds4_command_t *)p_parameter)->status = DS4_COMMAND_STATUS_FAILED_SEND;
        return;
    }

    // If the controller disconnected in the meantime
    if (!uni_bt_conn_is_connected(&d->conn)){
        ((ds4_command_t *)p_parameter)->status = DS4_COMMAND_STATUS_FAILED_SEND;
        return;
    }

    ds4_command_lightbar_t data = command.data.lightbar;

    if (d->report_parser.set_lightbar_color)
        d->report_parser.set_lightbar_color(
            d,
            data.R,
            data.G,
            data.B
        );
    
    // Successfully sent
    ((ds4_command_t *)p_parameter)->status = DS4_COMMAND_STATUS_AVAILABLE;
}

void ds4_rumble_callback(void *p_parameter)
{
    ds4_command_t command = *(ds4_command_t *)p_parameter;
    ds4_device_handle d = command.device;

    if (!d){
        ((ds4_command_t *)p_parameter)->status = DS4_COMMAND_STATUS_FAILED_SEND;
        return;
    }

    // If the controller disconnected in the meantime
    if (!uni_bt_conn_is_connected(&d->conn)){
        ((ds4_command_t *)p_parameter)->status = DS4_COMMAND_STATUS_FAILED_SEND;
        return;
    }

    ds4_command_rumble_t data = command.data.rumble;

    if (d->report_parser.play_dual_rumble != NULL)
        d->report_parser.play_dual_rumble(
            d,
            data.start_delay,
            data.duration,
            data.magnitude_weak,
            data.magnitude_strong
        );

    // Successfully sent
    ((ds4_command_t *)p_parameter)->status = DS4_COMMAND_STATUS_AVAILABLE;
}
