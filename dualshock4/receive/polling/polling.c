#include "ds4_polling.h"
#include "ds4_receive_type.h"
#include <string.h>

static ds4_data_t *data_p = NULL;

void ds4SetPollingStruct(ds4_data_t *p)
{
    data_p = p;
}

ds4_polling_send_e ds4_polling_send(ds4_data_t *p)
{
    if (data_p == NULL)
        return DS4_POLLING_SEND_FAIL;
    else
    {
        memcpy(data_p, p, sizeof(ds4_data_t));
        return DS4_POLLING_SEND_SUCCESS;
    }
}
