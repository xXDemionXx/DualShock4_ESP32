// This example shows two controller fetures:
//  - lightbar
//  - rumble
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ds4.h" // API header

#define MIN_CHECK_DELAY_MS 100
const char *TAG = "Controller commands demo";

enum color_indexes
{
    R = 0,
    G = 1,
    B = 2
};

static const char pink[3] = {178, 86, 184};
static const char yellow[3] = {255, 243, 0};
static const char green[3] = {0, 255, 0};
static const char blue[3] = {0, 0, 255};

int app_main(void)
{
    ds4Init(); // Before using any ds4 functions you need to initialize the library

    for (;;)
    {
        if (ds4GetConnectionStatus() == DS4_READY)
        {
            ds4SetLightbar(pink[R], pink[G], pink[B]);
            ESP_LOGI(TAG, "Changed lightbar color to PINK");
            ds4PlayRumble(255, 1000, 0);
            ESP_LOGI(TAG, "Play rumble with BOTH motors at intensity 255/255, for 1 second starting imedietly\n");
            vTaskDelay(pdMS_TO_TICKS(3000));
            ds4SetLightbar(yellow[R], yellow[G], yellow[B]);
            ESP_LOGI(TAG, "Changed lightbar color to YELLOW");
            ds4PlayRumbleWeak(100, 1000, 2000);
            ESP_LOGI(TAG, "Play rumble with the WEAK motors at intensity 100/255, for 1 second after 2 seconds\n");
            vTaskDelay(pdMS_TO_TICKS(3000));
            ds4SetLightbar(green[R], green[G], green[B]);
            ESP_LOGI(TAG, "Changed lightbar color to GREEN");
            ds4PlayRumbleStrong(200, 2000, 0);
            ESP_LOGI(TAG, "Play rumble with the STRONG motors at intensity 200/255, for 2 seconds starting imedietly\n");
            vTaskDelay(pdMS_TO_TICKS(3000));
            ds4SetLightbar(blue[R], blue[G], blue[B]);
            ESP_LOGI(TAG, "Changed lightbar color to BLUE");
            ds4PlayRumbleSpecific(200, 100, 1000, 500);
            ESP_LOGI(TAG, "Play rumble with the BOTH motors (but specify each motors intensity)\nIntensity weak = 200/255, intensity strong = 100/255, for 1 second after 0.5 seconds\n");
            vTaskDelay(pdMS_TO_TICKS(3000));
        }
        else
        {
            ESP_LOGI(TAG, "Controller not connected, waiting for connection...\n");
            while (ds4GetConnectionStatus() != DS4_READY) // Just wait if controller isn't connected
                vTaskDelay(pdMS_TO_TICKS(MIN_CHECK_DELAY_MS));
        }
    }
}
