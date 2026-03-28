idf_component_register(SRCS "dualshock4_initializer.c" "dualshock4.c" "dualshock4_default_funcs.c" "dualshock4_task_commands.c"
                       INCLUDE_DIRS "."
                       REQUIRES bluepad32 btstack)
