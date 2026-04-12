#ifndef DS4_BTN_EVENT_HANDLING_SETTINGS_H
#define DS4_BTN_EVENT_HANDLING_SETTINGS_H

// xTaskCreate(&event_handler_task, "DS4 buttons event handler", 1024 * 3, &queue_handle, 6, &task_handle))
#define NUM_TO_KB(x) x * 1024
#define DS4_BTN_EVENT_TASK_SIZE 3 // In kB
#define DS4_BTN_EVENT_TASK_NAME "DS4 buttons event handler"
#define DS4_BTN_EVENT_TASK_PRIORITY 7

#endif // DS4_EVENT_HANDLING_SETTINGS_H
