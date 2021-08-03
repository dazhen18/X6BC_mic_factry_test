#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "command_monitor.h"

#define tag  "MAIN"

extern int es7243e_init();

void app_main(void)
{
    printf("------------------> start <--------------------- \n");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

#if CONFIG_CUPID_BOARD_V2
    // Analog mic tie to ES7243 to dsp chip.
    es7243e_init();
#endif

    monitor_init();
    while (1)
    {
        // test memory
        vTaskDelay(50000 / portTICK_PERIOD_MS);
    }

}
