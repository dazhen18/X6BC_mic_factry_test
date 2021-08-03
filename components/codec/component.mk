#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
CFLAGS += -mfix-esp32-psram-cache-issue

COMPONENT_ADD_INCLUDEDIRS := include

COMPONENT_ADD_INCLUDEDIRS += utility \
                             TAS5805M \
                             es7243e

COMPONENT_SRCDIRS += utility \
                     TAS5805M \
                     es7243e
