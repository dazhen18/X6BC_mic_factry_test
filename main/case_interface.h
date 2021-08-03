//
// Created by ligaofeng on 19-11-8.
//

#ifndef ESP32_PRODUCT_TOOL_TEST_CASE_INTERFACE_H
#define ESP32_PRODUCT_TOOL_TEST_CASE_INTERFACE_H

#define CASE_NAME_PLAYER_RECORDER "player_recorder"

#define CASE_NAME_BLUETOOTH "bluetooth"

#include <stddef.h>
#include "bds_client.h"

int start_dsp_check_case(bds_client_handle_t handle);
int start_dsp_intr(void);

int start_player_recorder_case(int type, const char* cmdline);

void test_led_red();
void test_led_green();
void test_led_blue();
void test_led_end();

void test_led_redx();
void test_led_greenx();
void test_led_bluex();
void test_led_endx();

void test_led_read();
void led_red_on();


int start_nolinear_case(const char* cmdline);


int get_sn_case(char* deviceSn, size_t* length);

int set_sn_case(const char* deviceSn);


void start_reset_system(void);
void start_poweroff_system(void);
void start_test_fail(void);
void start_quit1_case(void);
void start_quit2_case(void);

void start_profile_case(char *params);
void profile_read(void);

void start_version_case(void);

void start_mic_seal_case(char *params);

#endif //ESP32_PRODUCT_TOOL_TEST_CASE_INTERFACE_H
