#include <nvs.h>
#include "result_out.h"
#include "case_interface.h"
#include "command_monitor.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_partition.h"

#define TAG "system_control"

// firmware version
#define SOFT_VERSION "1.1.0"

#define GPIO_POWER            GPIO_NUM_19
#define GPIO_POWER_SEL        GPIO_SEL_19

#define GPIO_LED_R            GPIO_NUM_33
#define GPIO_LED_R_SEL        GPIO_SEL_33

void start_reset_system(void)
{
    out_print_result(RESULT_RST_LEVEL, SUCCESS, "software reset success!");
    fflush(stdout);
    esp_restart();
}

void start_poweroff_system(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = GPIO_POWER_SEL;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;

    out_print_result(RESULT_OFF_LEVEL, SUCCESS, "software power off success!");
    fflush(stdout);
    
    gpio_config(&io_conf);
    gpio_set_level(GPIO_POWER, 0);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void start_test_fail(void)
{
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = GPIO_LED_R_SEL;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;

    gpio_config(&io_conf);

    gpio_set_level(GPIO_LED_R, 1);
    out_print_result(RESULT_FAIL_LEVEL, SUCCESS, "software power off success!");

    while(1)
    {
        gpio_set_level(GPIO_LED_R, 1);
        vTaskDelay(150 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_LED_R, 0);
        vTaskDelay(150 / portTICK_PERIOD_MS);
    }
}

void start_quit1_case(void)
{
    out_print_result(RESULT_QUIT1_LEVEL, SUCCESS, "quit all of tests!");
    return;
}

void start_quit2_case(void)
{
    out_print_result(RESULT_QUIT2_LEVEL, SUCCESS, "quit all of tests!");
    return;
}

char* get_param(char* src, char ch)
{
    char* temp1_param = NULL;
    char* temp2_param = NULL;
    char* dst = NULL;
    int len = 0;

    temp1_param = strchr(src, '"');
    if (temp1_param == NULL)
    {
        return NULL;
    }
    temp2_param = strchr(temp1_param + 1, '"');
    if (temp2_param == NULL)
    {
        return NULL;
    }
    len = (unsigned int)temp2_param - (unsigned int)temp1_param;
    dst = (char *) malloc(len * sizeof(char));
    if (dst == NULL)
    {
        return NULL;
    }
    memcpy(dst, temp1_param + 1, len - 1);
    dst[len - 1] = '\0';

    return dst;
}

void start_profile_case(char *params)
{
    // command format: prod_test profile "fc" "pk" "ak" "sk"
    char* temp1_param = NULL;
    char* temp2_param = NULL;
    char* fc = NULL;
    char* pk = NULL;
    char* ak = NULL;
    char* sk = NULL;
    char* profile_str = NULL;
    int res = 1;
    int i = 0;

    for (i = 0; i < 1; i++)
    {
        //printf("params:%s\n", params);
        fc = get_param(params, '"');
        if (fc == NULL)
        {
            break;
        }
        //printf("fc:%s\n", fc);
        temp1_param = strchr(params, '"');
        temp1_param = strchr(temp1_param + 1, '"');
        
        pk = get_param(temp1_param + 1, '"');
        if (pk == NULL)
        {
            break;
        }
        //printf("pk:%s\n", pk);
        temp1_param = strchr(temp1_param + 1, '"');
        temp1_param = strchr(temp1_param + 1, '"');

        ak = get_param(temp1_param + 1, '"');
        if (ak == NULL)
        {
            break;
        }
        //printf("ak:%s\n", ak);
        temp1_param = strchr(temp1_param + 1, '"');
        temp1_param = strchr(temp1_param + 1, '"');

        sk = get_param(temp1_param + 1, '"');
        if (sk == NULL)
        {
            break;
        }
        //printf("sk:%s\n", sk);

        char fc_header[] = "{\"fc\":\"";
        char pk_header[] = "\",\"pk\":\"";
        char ak_header[] = "\",\"ak\":\"";
        char sk_header[] = "\",\"sk\":\"";
        char tail[] = "\"}\0a";
        int len = strlen(fc) + strlen(pk) + strlen(ak) + strlen(sk) + 
        strlen(fc_header) + strlen(pk_header) + strlen(ak_header) + strlen(sk_header) + strlen(tail) + 1;

        profile_str = (char *)malloc(len * sizeof(char));
        strcpy(profile_str, fc_header);
        strcat(profile_str, fc);
        strcat(profile_str, pk_header);
        strcat(profile_str, pk);
        strcat(profile_str, ak_header);
        strcat(profile_str, ak);
        strcat(profile_str, sk_header);
        strcat(profile_str, sk);
        strcat(profile_str, tail);

        esp_partition_t *partition = NULL;
        partition = (esp_partition_t *)esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                                                PROFILE_PARTITION, "profile");
        if (partition == NULL) {
            ESP_LOGE("Partition", "Can not find dsp-partition");
            break;
        }
        ESP_LOGI(TAG, "%d: type[0x%x]", __LINE__, partition->type);
        ESP_LOGI(TAG, "%d: subtype[0x%x]", __LINE__, partition->subtype);
        ESP_LOGI(TAG, "%d: address:0x%x", __LINE__, partition->address);
        ESP_LOGI(TAG, "%d: size:0x%x", __LINE__, partition->size);
        ESP_LOGI(TAG, "%d: label:%s", __LINE__,  partition->label);

        esp_err_t flash_ret = esp_partition_erase_range(partition, 0, 4096);
        if (ESP_OK != flash_ret) {
            printf("flash erase_ret %x\n", flash_ret);
            out_print_result(RESULT_PROFILE_LEVEL, FAILED, "flash partition erase error");
            break;
        }

        flash_ret = esp_partition_write(partition, 0, profile_str, len + 1);
        if (ESP_OK != flash_ret) {
            printf("flash write_ret %x\n", flash_ret);
            out_print_result(RESULT_PROFILE_LEVEL, FAILED, "flash partition write error");
            break;
        }
        res = 0;
    }

    if (fc != NULL)
    {
        free(fc);
    }
    if (pk != NULL)
    {
        free(pk);
    }
    if (ak != NULL)
    {
        free(ak);
    }
    if (sk != NULL)
    {
        free(sk);
    }
    if (profile_str != NULL)
    {
        free(profile_str);
    }

    if (res == 0)
    {
        out_print_result(RESULT_PROFILE_LEVEL, SUCCESS, "profile pass!");
    }
    else
    {
        out_print_result(RESULT_PROFILE_LEVEL, FAILED, "param error!");
    }
    return;
}

void profile_read()
{
    char *buf = NULL;
    esp_partition_t *partition = NULL;
    partition = (esp_partition_t *)esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                                            PROFILE_PARTITION, "profile");
    if (partition == NULL) {
        ESP_LOGE("Partition", "Can not find dsp-partition");
        return NULL;
    }
    ESP_LOGI(TAG, "%d: type[0x%x]", __LINE__, partition->type);
    ESP_LOGI(TAG, "%d: subtype[0x%x]", __LINE__, partition->subtype);
    ESP_LOGI(TAG, "%d: address:0x%x", __LINE__, partition->address);
    ESP_LOGI(TAG, "%d: size:0x%x", __LINE__, partition->size);
    ESP_LOGI(TAG, "%d: label:%s", __LINE__,  partition->label);

    buf = malloc(partition->size);
    esp_err_t flash_ret = esp_partition_read(partition, 0, buf, partition->size);
    if (flash_ret != ESP_OK) {
        ESP_LOGW(TAG, "Read profile failed and use default profile.");
        free(buf);
        return ESP_ERR_NOT_FOUND;
    }

    printf("profile read as:%s\n", buf);
    out_print_result(RESULT_PROFILE_LEVEL, SUCCESS, "read profile pass!");
    return;
}

void start_version_case(void)
{
    out_print_result(RESULT_VERSION_LEVEL, SUCCESS, SOFT_VERSION);
    return;
}
