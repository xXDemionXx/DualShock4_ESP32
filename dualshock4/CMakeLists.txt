set(srcs
         "ds4.c"
         "controller_behaviour/controller_behaviour.c" 
         "receive/polling/polling.c"
         "send/controller_commands.c"
        )

set(includes 
            "."
            "include"
            )

set(priv_includes
            "receive/polling"
            "controller_behaviour"
            "send"
            )

if(CONFIG_DS4_MODE_EVENT)
    list(APPEND srcs    "receive/event_driven/event_handling.c")
    list(APPEND priv_includes   "receive/event_driven")
endif()

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${includes}
                       PRIV_INCLUDE_DIRS ${priv_includes}
                       REQUIRED_IDF_TARGETS esp32
                       PRIV_REQUIRES bluepad32 btstack)
