#include "dualshock4_task_commands.h"
#include "dualshock4_device_handle.h"

#include "dualshock4_task_commands_control.h"
#include "hci.h"
#include "esp_log.h"
#include "btstack_defines.h"

// Private variables
static QueueHandle_t commands_queue_handle = NULL;
static TaskHandle_t task_handle;
static const char *TAG = "ds4_command";
static btstack_context_callback_registration_t callback_registration;

// Private function prototypes
static void ds4_commands_task(void *p_parameter);
static void change_lightbar(void * p_parameter);
static void rumble(ds4_command_rumble command, ds4_device_handle d);
static void test_write(ds4_command_test_write);


ds4_command_task_init_return_t ds4_init_commands_task(void)
{
    ds4_command_task_init_return_t return_vals;
    return_vals.error_code = DS4_COMMAND_TASK_INIT_STARTING_FAILURE;

    // Create the command queue which the command task will use to receive commands for the controller
    commands_queue_handle = xQueueCreate(DS4_COMMAND_TASK_QUEUE_SIZE, sizeof(ds4_command_s));

    if (commands_queue_handle == NULL)
    {
        return_vals.error_code = DS4_COMMAND_TASK_INIT_FAILED_QUEUE;
        return return_vals;
    }
    // Queue was created succesfully, we can store the handle for returning
    return_vals.queue_handle = commands_queue_handle;

    // Create the commands task
    if (pdPASS != xTaskCreate(&ds4_commands_task, "DS4 commands", NUM_TO_KB(DS4_COMMAND_TASK_SIZE), &commands_queue_handle, DS4_COMMAND_TASK_PRIORITY, &task_handle))
    {
        ds4_suspend_command_task(); // Do not start the task immediately, start it when a controller connects
        return_vals.error_code = DS4_COMMAND_TASK_INIT_FAILED_TASK;
        return return_vals;
    }
    // Everything passed
    return_vals.error_code = DS4_COMMAND_TASK_INIT_SUCCES;
    return return_vals;
}

// Public functions
QueueHandle_t ds4_command_task_get_queue_handle(void)
{
    return commands_queue_handle;
}

void ds4_suspend_command_task()
{
    if (task_handle != NULL)
        vTaskSuspend(task_handle);
}

void ds4_resume_command_task()
{
    if (task_handle != NULL)
        vTaskResume(task_handle);
}

// Private functions

static void ds4_commands_task(void *p_parameter)
{
    // QueueHandle_t commands_queue_handle = *(QueueHandle_t *)p_parameter;
    // QueueHandle_t commands_queue_handle;

    ds4_command_s command_packet;

    for (;;)
    {
        if (pdTRUE == xQueueReceive(commands_queue_handle, &command_packet, portMAX_DELAY))
        {
            if(/*hci_is_packet_buffer_reserved() == false*/ true){
                switch (command_packet.command_indicator)
                {
                case (DS4_COMMAND_TYPE_LIGHTBAR):
                    ESP_LOGI(TAG, "Change lightbar");
                    callback_registration.callback = &change_lightbar;
                    callback_registration.context = (void*)(&command_packet);
                    btstack_run_loop_execute_on_main_thread(&callback_registration);
                    //change_lightbar(command_packet.data.lightbar_command, command_packet.device);
                    break;
                case (DS4_COMMAND_TYPE_RUMBLE):
                    rumble(command_packet.data.rumble_command, command_packet.device);
                    break;
                case (DS4_COMMAND_TYPE_TEST_WRITE):
                    test_write(command_packet.data.test_write_command);
                    break;
                default:
                    ESP_LOGI(TAG, "Unknown command for DS4");
                }
            }else{
                static uint16_t i=0;
                i++;
                ESP_LOGW(TAG, "HCI buffer full occurance num: %d", i);
            }
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

// Possible commands

// static void change_lightbar(ds4_command_change_lightbar command, ds4_device_handle d)
// {
//     ESP_LOGI(TAG, "Change lightbar");
//     if(((uni_hid_device_t*)d)->report_parser.set_lightbar_color != NULL)
//     ((uni_hid_device_t*)d)->report_parser.set_lightbar_color((uni_hid_device_t*)d, command.R, command.G, command.B);
// }

// static void change_lightbar(ds4_command_change_lightbar command, uni_hid_device_t *d)
// {
//     //ESP_LOGI(TAG, "Change lightbar");
//     if(((uni_hid_device_t*)d)->report_parser.set_lightbar_color != NULL)
//         ((uni_hid_device_t*)d)->report_parser.set_lightbar_color(d, command.R, command.G, command.B);
// }

static void change_lightbar(void * p_parameter)
{
    //ESP_LOGI(TAG, "Change lightbar");
    ds4_command_s command_packet = *(ds4_command_s *)p_parameter;

    if (!command_packet.device) return;
    // ESP_LOGI(TAG, "A");
    if (!uni_bt_conn_is_connected(&command_packet.device->conn)) return;
    // ESP_LOGI(TAG, "R-%d, G-%d, B-%d", command_packet.data.lightbar_command.R, command_packet.data.lightbar_command.G, command_packet.data.lightbar_command.B);

    if(command_packet.device->report_parser.set_lightbar_color != NULL)
        command_packet.device->report_parser.set_lightbar_color(command_packet.device, command_packet.data.lightbar_command.R, command_packet.data.lightbar_command.G, command_packet.data.lightbar_command.B);
}

static void rumble(ds4_command_rumble command, ds4_device_handle d)
{
    ESP_LOGI(TAG, "Rumble");
    ((uni_hid_device_t*)d)->report_parser.play_dual_rumble((uni_hid_device_t*)d, command.start_delay, command.duration, command.magnitude_weak, command.magnitude_strong);
}

static void test_write(ds4_command_test_write command)
{
    // For now nothing
    ESP_LOGI(TAG, "Received text: %s", command.string);
}
