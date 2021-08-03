#if [[ -z $1 ]]
#then
#    port="/dev/tty.usbserial-110"
#else
#    port=$1
#fi

python $IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port COM3 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect \
    0x1000   ./build/bootloader/bootloader.bin \
    0x8000   ./build/partitions.bin \
    0xd000   ./build/ota_data_initial.bin \
    0x10000  ./build/bds_product_tool.bin \
    0x210000 ./files/DU1906_slave_v1.5.5.D.bin
