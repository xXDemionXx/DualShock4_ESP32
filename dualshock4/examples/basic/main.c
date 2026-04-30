// This example shows how to setup the basic polling aproach.
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ds4.h" // API header

#define PRINT_DELAY_MS 2000
#define MIN_CHECK_DELAY_MS 100
const char *TAG = "Polling demo";

ds4_data_t data = {0}; // Make a struct that will be used for polling

static void print_data(const ds4_data_t *data); // Helper for printing all of the members of ds4_data_t

int app_main(void)
{
    ds4Init(); // Before using any ds4 functions you need to initialize the library

    ds4SetPollingStruct(&data); // Pass the pointer to the struct you want to receive controller data

    for (;;)
    {
        if (ds4GetConnectionStatus() == DS4_READY)
        {
            // If the controller isn't connected the data will all allways be 0
            // but you should always check if the controller is connected before
            // you do anything with the data because that is the only way to know
            // if the data actually is 0 or it is unknown.
            print_data(&data);

            // Your application logic can replace the print function or it can run in a different task
            //
            // application_logic();
            //
        }
        else
        {
            ESP_LOGI(TAG, "Controller not connected, waiting for connection...\n");
            while (ds4GetConnectionStatus() != DS4_READY) // Just wait if controller isn't connected
                vTaskDelay(pdMS_TO_TICKS(MIN_CHECK_DELAY_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY_MS));
    }
}

static void print_data(const ds4_data_t *data)
{
    if (!data)
    {
        ESP_LOGE(TAG, "NULL pointer passed to print_data()\n");
        return;
    }

    ESP_LOGI(TAG,
             "\n==========================================================================\n"
             "Buttons right: [X: %u, O: %u, □: %u, △: %u]\n"
             "Buttons front: [L1: %u R1: %u L3: %u R3: %u]\n"
             "Buttons other: [PS: %u, Share: %u, Options:%u]\n"
             "DPAD[U:%u D:%u L:%u R:%u]\n"
             "L2: %4ld R2: %4ld\n"
             "L joystick (%4ld, %4ld)\n"
             "R joystick (%4ld, %4ld)\n"
             "Gyro: [x: %ld, y: %ld, z: %ld]"
             "Acel: [x: %ld, y: %ld, z: %ld]\n"
             "Battery: %3u/255\n"
             "==========================================================================\n",
             data->buttons.cross, data->buttons.circle, data->buttons.square, data->buttons.triangle,
             data->buttons.L1, data->buttons.R1, data->buttons.L3, data->buttons.R3,
             data->buttons.PS_button, data->buttons.share, data->buttons.options,
             data->dpad.U, data->dpad.D, data->dpad.L, data->dpad.R,
             (long)data->L2, (long)data->R2,
             (long)data->ljoy.x, (long)data->ljoy.y,
             (long)data->rjoy.x, (long)data->rjoy.y,
             (long)data->gyro.x, (long)data->gyro.y, (long)data->gyro.z,
             (long)data->accel.x, (long)data->accel.y, (long)data->accel.z,
             data->battery);
}
