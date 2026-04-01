# set(srcs
#          "dualshock4_initializer.c" 
#          "dualshock4.c"
#          "dualshock4_default_funcs.c"
#          "dualshock4_task_commands.c")

# set(includes
#          "."
#          "tasks/commands/"
#         )

# idf_component_register(SRCS ${srcs}
#                        INCLUDE_DIRS ${includes}
#                        REQUIRES bluepad32 btstack)

set(srcs
         "dualshock4_initializer.c" 
         "dualshock4.c"
         "dualshock4_default_funcs.c"
         "dualshock4_task_commands.c"
         "dualshock4_connection_status.c"
         "dualshock4_connection_status_modify.c")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS .
                       REQUIRES bluepad32 btstack)
