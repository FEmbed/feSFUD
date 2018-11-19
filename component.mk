#
# Component Makefile
#
# flags for component's makefile.
CPPFLAGS += -DFE_USE_DRIVER_NAME=1

# port include has high priority than default ones.
COMPONENT_ADD_INCLUDEDIRS += port
COMPONENT_ADD_INCLUDEDIRS += SFUD/sfud/inc

COMPONENT_SRCDIRS += port
COMPONENT_SRCDIRS += SFUD/sfud/src
