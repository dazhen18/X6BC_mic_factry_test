deps_config := \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/app_trace/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/aws_iot/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/bt/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/driver/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/efuse/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esp32/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esp_adc_cal/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esp_event/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esp_http_client/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esp_http_server/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esp_https_ota/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/espcoredump/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/ethernet/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/fatfs/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/freemodbus/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/freertos/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/heap/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/libsodium/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/log/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/lwip/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/mbedtls/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/mdns/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/mqtt/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/nvs_flash/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/openssl/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/pthread/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/spi_flash/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/spiffs/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/tcpip_adapter/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/unity/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/vfs/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/wear_levelling/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/wifi_provisioning/Kconfig \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/app_update/Kconfig.projbuild \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/main/Kconfig.projbuild \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/mars/git/x6bc_gitlab/mic_test/x6_mic_factory_test/esp-gitee-tools/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
