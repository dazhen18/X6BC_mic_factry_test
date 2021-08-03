#!/usr/bin bash

if [[ -z $1 ]]
then
    port="/dev/tty.usbserial-110"
else
    port=$1
fi

python $IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port ${port} --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect \
    0x10000  ./build/bds_product_tool.bin 
