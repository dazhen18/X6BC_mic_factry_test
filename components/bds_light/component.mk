#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
CFLAGS += -mfix-esp32-psram-cache-issue

COMPONENT_SRCDIRS := .
COMPONENT_ADD_INCLUDEDIRS := include
ifdef CONFIG_CUPID_BOARD_V2
COMPONENT_ADD_LDFLAGS += -L$(COMPONENT_PATH)/lib -lbds_light_sdk_cupid
else
COMPONENT_ADD_LDFLAGS += -L$(COMPONENT_PATH)/lib -lbds_light_sdk
endif