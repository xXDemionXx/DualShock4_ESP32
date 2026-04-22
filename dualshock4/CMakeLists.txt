set(srcs
         "dualshock4.c"
         "dualshock4_initializer.c" 
         "receive/polling/polling.c"
         "send/controller_commands.c"
        )

set(includes
         "."
         "receive"
         "receive/polling"
         "send"
        )

if(CONFIG_DS4_MODE_EVENT)
    list(APPEND srcs    "receive/event_driven/ds4_event_handling.c")
    list(APPEND includes "receive/event_driven")
endif()

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       REQUIRES bluepad32 btstack)
