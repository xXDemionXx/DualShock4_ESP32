set(srcs
         "dualshock4_initializer.c" 
         "dualshock4.c"
         "dualshock4_default_funcs.c"
         "dualshock4_task_commands.c"
         "dualshock4_connection_status.c"
         "dualshock4_connection_status_modify.c"
         "dualshock4_device_handle.c"
         "receive/polling/polling.c"
         )

set(includes
         "."
         "receive"
         "receive/polling"
        )

if(CONFIG_DS4_MODE_EVENT)
    list(APPEND srcs "receive/event_driven/events.c")
    list(APPEND includes "receive/event_driven")
endif()

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES bluepad32 btstack)
