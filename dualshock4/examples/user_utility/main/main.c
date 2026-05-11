// This example prints out the MAC address set as your bluetooth MAC.
// If "Bluetooth MAC Address Mode" is set to "Use custom Bluetooth MAC"
// you will just read the address that you wrote.
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ds4.h" // API header

#define MIN_CHECK_DELAY_MS 100
const char *TAG = "User utility demo";

char MAC_buffer[18];

int app_main(void)
{
    ds4Init(); // Before using any ds4 functions you need to initialize the library

    ds4GetUserAddress(MAC_buffer);

    ESP_LOGI(TAG, "MAC address of ESP32 - \"%s\"", MAC_buffer);

    for (;;)
    {
        if (ds4GetConnectionStatus() == DS4_READY)
        {
            ESP_LOGI(TAG, "Disconnecting controller in 5 seconds:");
            ESP_LOGI(TAG, "5");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG, "4");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG, "3");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG, "2");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ESP_LOGI(TAG, "1");
            vTaskDelay(pdMS_TO_TICKS(1000));
            ds4Disconnect();
            ESP_LOGI(TAG, "Controller disconnected");
        }
        else
        {
            ESP_LOGI(TAG, "Controller not connected, waiting for connection...\n");
            while (ds4GetConnectionStatus() != DS4_READY) // Just wait if controller isn't connected
                vTaskDelay(pdMS_TO_TICKS(MIN_CHECK_DELAY_MS));
        }
    }
}
