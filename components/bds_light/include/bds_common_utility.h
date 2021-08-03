//
// Created by ligaofeng on 19-2-11.
//

#ifndef LIGHT_BDSPEECH_COMMON_UTILITY_H
#define LIGHT_BDSPEECH_COMMON_UTILITY_H

#include "stdint.h"

unsigned long long bds_get_current_time();

uint64_t bds_get_current_time_ms();

int bds_generate_uuid(char* uuid_out);

int bds_get_sdk_version(char* out_version, int len);

int bds_create_sign(char* psrc, int slen, uint32_t *sign);

uint32_t bds_rand32();

/**
 *  0:E  1:W  2:I 3:D 4:V
 * @param level
 */
void bds_set_log_level(int level);

void bds_set_log_callback(void(*log_listener)(const char* format, ...));

#define DURATION_MS(n)  ((n)/portTICK_PERIOD_MS)

#endif //LIGHT_BDSPEECH_COMMON_UTILITY_H
