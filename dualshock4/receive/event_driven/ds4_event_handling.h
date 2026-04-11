#ifndef DS4_EVENT_HANDLING_H
#define DS4_EVENT_HANDLING_H

#include "ds4_receive_type.h"

#include "freertos/FreeRTOS.h"
//#include "freertos/queue.h"

/**
 * @brief Sends controller data to the event handler
 *
 * @param data Pointer to the data that the event handler will evaluate
 * @return Returns pdTRUE if data was sent succesfully and errQUEUE_FULL if queue was full
 */
BaseType_t ds4_send_data_to_event_handler(ds4_data_t *data);
#endif
