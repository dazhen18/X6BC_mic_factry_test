//
// Created by ligaofeng on 19-11-11.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include "rom/uart.h"
#include "esp_heap_caps.h"
#include "result_out.h"

void out_print_result(const char *test_case, int code, char* error_string) {
    char *result = (code == SUCCESS) ? "success" : "error";
    printf("%s %s %s\n", test_case, result, error_string);
}
#define RECORDER_HEAD 40
int g_idx = 0;
void out_print_data(const char *test_case, unsigned char *data, int length) {
//    printf("%s record %s\n", test_case, data);
    char head[RECORDER_HEAD];
    snprintf(head, RECORDER_HEAD, "%srecord %d %d ", test_case, length, g_idx++);
    unsigned char* proto_data = NULL;
    proto_data = heap_caps_malloc(length + RECORDER_HEAD + 1, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (proto_data == NULL)
    {
        printf("\n[cgf] Failed to malloc memory\n");
        return;
    }
    memcpy(proto_data, head, RECORDER_HEAD);
    memcpy(proto_data + RECORDER_HEAD, data, length);
    proto_data[RECORDER_HEAD + length] = '\n';
    fwrite(proto_data, length + RECORDER_HEAD + 1, 1, stdout);
    free(proto_data);
}

int calc_cksum(unsigned char *data)
{
    return 0;
}

void data_out_exec(const char *test_case, unsigned char* data, int length)
{
    printf("data out start\n");
    int pcm_length = 0;
    unsigned char out_slip[UART_OUT_SIZE];
    int cksum = 0;
    while (1) {
        if (length < UART_OUT_SIZE) {
            out_print_data(test_case, data, length);
            break;
        }
        if ((pcm_length + UART_OUT_SIZE) > length) {
            break;
        }
        memcpy(out_slip, data + pcm_length, UART_OUT_SIZE);
        out_print_data(test_case, out_slip, UART_OUT_SIZE);
        pcm_length += UART_OUT_SIZE;
        // end
        if (pcm_length >= length) {
            break;
        }
        
        vTaskDelay(6 / portTICK_PERIOD_MS);
    }
}

void out_print_data_ctl(const char *test_case) {
    g_idx = 0;
    printf("idx %d\n", g_idx);
    printf("%srecord_end\n", test_case);
}
