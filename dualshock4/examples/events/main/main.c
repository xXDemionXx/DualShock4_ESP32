// This example shows how to use events on buttons.
// To use events in sdkconfig "DualShock4 operation mode" must be set to "Event-based mode"
// Events can be registered anywhere with ds4SetButtonEvent(). (it can even be used before ds4Init)
//
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ds4.h" // API header

#ifndef CONFIG_DS4_MODE_EVENT
#error "Enable \"Event-based mode\" for DualShock4 in sdkconfig"
#endif

#define MIN_CHECK_DELAY_MS 100

static ds4_data_t data = {0}; // Make a struct that will be used for polling
static int count = 0;

static void user_task(void *p_parameter);
static void count_up(void *argv);
static void count_down(void *argv);
void set_lightbar(void *argv);

int app_main(void)
{
    ds4Init(); // Before using any ds4 functions you need to initialize the library

    ds4SetPollingStruct(&data); // Pass the pointer to the struct you want to receive controller data

    // Add an avent to a buttom, pass it a function and a pointer to data that the function needs
    ds4SetButtonEvent(DS4_BTN_DPAD_UP, DS4_BTN_EVENT_PRESS, &count_up, &count);
    ds4SetButtonEvent(DS4_BTN_DPAD_DOWN, DS4_BTN_EVENT_PRESS, &count_down, &count);

    // You can use a press event or a release event
    ds4SetButtonEvent(DS4_BTN_X, DS4_BTN_EVENT_PRESS, &set_lightbar, (uint8_t[3]){0, 0, 255});
    ds4SetButtonEvent(DS4_BTN_SQUARE, DS4_BTN_EVENT_RELEASE, &set_lightbar, (uint8_t[3]){178, 86, 184});
    ds4SetButtonEvent(DS4_BTN_CIRCLE, DS4_BTN_EVENT_PRESS, &set_lightbar, (uint8_t[3]){255, 0, 0});
    ds4SetButtonEvent(DS4_BTN_TRIANGLE, DS4_BTN_EVENT_RELEASE, &set_lightbar, (uint8_t[3]){0, 255, 0});

    // You can even attach the disconnect function to a button
    ds4SetButtonEvent(DS4_BTN_PS, DS4_BTN_EVENT_PRESS, (void (*)(void *))&ds4Disconnect, NULL);

    if (pdPASS != xTaskCreate(&user_task, "user_task", 1024 * 5, NULL, 3, NULL))
    {
        ESP_LOGE("app_main", "User task creation failed");
        ESP_ERROR_CHECK(ESP_FAIL);
    }

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void user_task(void *p_parameter)
{

    int prev_count = count;

    for (;;)
    {
        if (count != prev_count)
        {
            ESP_LOGI("user_task", "Count = %d", count);
            prev_count = count;
        }
        if (data.R2 > 500) // Press R2
        {
            // Data from polling can be used anywhere like normal, events don't affect polling
            ESP_LOGI("user_task", "R2 pressed = %lu", data.R2);
        }
        vTaskDelay(pdMS_TO_TICKS(MIN_CHECK_DELAY_MS));
    }
}

static void count_up(void *argv)
{
    int *cnt = (int *)argv;
    (*cnt)++;
    ESP_LOGI("count_up", "SQUARE button pressed");
}

static void count_down(void *argv)
{
    int *cnt = (int *)argv;
    (*cnt)--;
    ESP_LOGI("count_down", "TRIANGLE button pressed");
}

void set_lightbar(void *argv)
{
    ds4SetLightbar(*((uint8_t *)argv), *((uint8_t *)argv + 1), *((uint8_t *)argv + 2));
}
