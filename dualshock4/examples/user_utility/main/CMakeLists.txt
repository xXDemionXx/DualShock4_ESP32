set(srcs "main.c")
set(requires "dualshock4")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
