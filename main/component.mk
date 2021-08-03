#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
CFLAGS += -mfix-esp32-psram-cache-issue
COMPONENT_ADD_INCLUDEDIRS += . case

COMPONENT_SRCDIRS += . case

COMPONENT_EMBED_TXTFILES := ../product_tool_version.build \
    ./case/log-10s-mono.wav \
    ./case/1k-2s.wav
