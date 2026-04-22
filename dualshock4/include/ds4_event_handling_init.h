#ifndef DS4_EVENT_HANDLING_INIT_H
#define DS4_EVENT_HANDLING_INIT_H

typedef enum
{
    DS4_INIT_EVENT_SUCCES,
    DS4_INIT_EVENT_QUEUE_FAILED,
    DS4_INIT_EVENT_TASK_FAILED,
} ds4_event_handling_init_e;

/**
 * @brief Initializes the button event handling task
 */
ds4_event_handling_init_e ds4_init_buttons_event_handler();

#endif // DS4_EVENT_HANDLING_INIT_H
