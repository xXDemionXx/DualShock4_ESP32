#ifndef DS4_EVENT_HANDLING_H
#define DS4_EVENT_HANDLING_H

#include "ds4_receive_type.h"

#include "freertos/FreeRTOS.h"
//#include "freertos/queue.h"

/**
 * @brief Sends controller data to the event handler
 *
 * @param data Pointer to the data that the event handler will evaluate
 * @return Returns pdPASS if data was sent succesfully and pdFAIL if queue wasn't initialized
 */
BaseType_t ds4_send_data_to_event_handler(ds4_data_t *data);

/**
 * @brief Use to stop the event handler when the controller isn't connected
 */
void ds4_suspend_buttons_event_handler();

/**
 * @brief Use to start the event handler when the controller is connected
 */
void ds4_resume_buttons_event_handler();

#endif
