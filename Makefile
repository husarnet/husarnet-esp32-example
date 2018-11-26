PROJECT_NAME := husarnet-esp32-example

MY_PATH := $(abspath $(dir $(firstword $(MAKEFILE_LIST))))

COMPONENT_DIRS := $(IDF_PATH)/components $(MY_PATH)

include $(IDF_PATH)/make/project.mk
