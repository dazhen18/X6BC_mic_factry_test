//
// Created by ligaofeng on 19-11-11.
//

#ifndef ESP32_PRODUCT_TOOL_RESULT_OUT_H
#define ESP32_PRODUCT_TOOL_RESULT_OUT_H

#define SUCCESS 0
#define FAILED 1
#define UART_OUT_SIZE           (512)

void out_print_result(const char *test_case, int code, char* error_string);

void out_print_data(const char *test_case, unsigned char *data, int length);

void out_print_data_ctl(const char *test_case);

void data_out_exec(const char *test_case, unsigned char* data, int length);

#endif //ESP32_PRODUCT_TOOL_RESULT_OUT_H
