#ifndef DS4_POLLING_H
#define DS4_POLLING_H

#include "ds4_receive_type.h"

/*! \brief Was the data for polling sent correctly */
typedef enum
{
    DS4_POLLING_SEND_SUCCESS, /*!&lt; Success */
    DS4_POLLING_SEND_FAIL     /*!&lt; Pointer not set */
} ds4_polling_send_e;

/**
 * @brief Pass the pointer to your ds4_data_t variable that you can use for polling
 *
 * Access your variable like you are accessing a normal struct. The polling aproach will
 * not guarante that you allways read the freshest data, just that incoming controller
 * data will continuously be saved into the variable given by the pointer.
 */
void ds4SetPollingStruct(ds4_data_t *p);

/**
 * @brief Sends the data for polling
 *
 * Copy the data from *p to the location specified by the pointer set
 * by ds4SetPollingStruct(ds4_data_t *p).
 */
ds4_polling_send_e ds4_polling_send(ds4_data_t *p);

#endif // DS4_POLLING_H
