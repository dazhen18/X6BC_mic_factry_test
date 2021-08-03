#include <nvs.h>
#include "result_out.h"
#include "case_interface.h"
#include "command_monitor.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define TAG "sn_case"

#define NVS_DEVICE_SN_NAMESPACE "names_bd_device"
#define NVS_DEVICE_SN_KEY "bd_device_sn"

static esp_err_t _get_sn_case(char* outSn, size_t* length) {
    nvs_handle deviceNvsHandle;

    esp_err_t ret = nvs_open(NVS_DEVICE_SN_NAMESPACE, NVS_READONLY, &deviceNvsHandle);
    if (ret != ESP_OK) {
        return ret;
    }

    ret = nvs_get_str(deviceNvsHandle, NVS_DEVICE_SN_KEY, outSn, length);

    nvs_close(deviceNvsHandle);

    return ret;
}

static esp_err_t _set_sn_case(const char* inputSn) {
    nvs_handle deviceNvsHandle;

    printf("start to write sn: %s, key: %s, ns: %s\n", inputSn, NVS_DEVICE_SN_KEY, NVS_DEVICE_SN_NAMESPACE);

    esp_err_t ret = nvs_open(NVS_DEVICE_SN_NAMESPACE, NVS_READWRITE, &deviceNvsHandle);
    if (ret != ESP_OK) {
        printf("open device ns: %s error %d\n", NVS_DEVICE_SN_NAMESPACE, ret);
        return ret;
    }

    ret = nvs_set_str(deviceNvsHandle, NVS_DEVICE_SN_KEY, inputSn);

    printf("write sn ret: %d\n", ret);

    nvs_close(deviceNvsHandle);

    return ret;

}

int get_sn_case(char* deviceSn, size_t* length) {
    esp_err_t ret = _get_sn_case(deviceSn, length);
    
    if (ret == ESP_OK) {
        out_print_result(RESULT_GET_SN, SUCCESS, deviceSn);
    } else {
        snprintf(deviceSn, MAX_DEVICE_SN_LENGTH, "%d", ret);
        out_print_result(RESULT_GET_SN, FAILED, deviceSn);
    }

    return ret;
}

int set_sn_case(const char* cmdline) {
    char deviceSn[MAX_DEVICE_SN_LENGTH + 1] = {'\0'};

    size_t preLen = strlen(CMD_SET_SN) + 1;
    size_t inputSnLen = strlen(cmdline) - preLen;
    if (inputSnLen > 0 && inputSnLen <= MAX_DEVICE_SN_LENGTH) {
        memcpy(deviceSn, cmdline + preLen, inputSnLen);
    } else {
        out_print_result(RESULT_SET_SN, FAILED, "sn length should be between 1 and 31");
        return ESP_ERR_NVS_INVALID_LENGTH;
    }
    
    esp_err_t ret = _set_sn_case(deviceSn);
    
    if (ret == ESP_OK) {
        out_print_result(RESULT_SET_SN, SUCCESS, deviceSn);
    } else {
        snprintf(deviceSn, MAX_DEVICE_SN_LENGTH, "%d", ret);
        out_print_result(RESULT_SET_SN, FAILED, deviceSn);
    }

    return ret;
}
