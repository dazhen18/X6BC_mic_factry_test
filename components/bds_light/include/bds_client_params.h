/*
 * bds_client_params.h
 *
 *  Created on: Mar 8, 2019
 *      Author: liweigao
 */

#ifndef MAIN_CLIENT_BDS_CLIENT_PARAMS_H_
#define MAIN_CLIENT_BDS_CLIENT_PARAMS_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bds_client_context.h"

typedef enum PROTOCOL_TYPE {
    PROTOCOL_DEFAULT = 0x00,
    PROTOCOL_TLS = 0x01
} PROTOCOL_TYPE;

typedef enum RECORDER_TYPE {
    RECORDER_TYPE_PCM0 = 0x00,
    RECORDER_TYPE_PCM1 = 0x01
} RECORDER_TYPE;

typedef enum ASR_FORCE_MODE {
    ASR_FORCE_AUTO = 0,
    ASR_FORCE_OPUS,
    ASR_FORCE_FEATURE
} ASR_FORCE_MODE;

typedef enum LAUNCH_MODE {
    LAUNCH_LOAD_DSP = 0x00,
    LAUNCH_CHECK_DSP = 0x01
} LAUNCH_MODE_T;

typedef enum WAKEUP_NUM {
    WP_NUM_DEFAULT = 0x00,
    WP_NUM_ONE = 0x01,
    WP_NUM_TWO = 0x02
} WAKEUP_NUM;

typedef struct {
    LAUNCH_MODE_T launch_mode;
    int dsp_subtype;
} bds_client_params_t;

typedef struct {
    char sn[SN_LENGTH];
    uint32_t primary_pid;
    uint32_t assist_pid;
    char key[KEY_LENGTH];
    uint16_t audio_rate;
    char cuid[CUID_LENGTH];
    int wakeup_status;
    int voice_print;
    uint16_t pam_len;
    char pam[];
} bdsc_asr_params_t;

typedef struct {
    char sn[SN_LENGTH];
    uint32_t pid;
    char key[KEY_LENGTH];
    char cuid[CUID_LENGTH];
    uint16_t pam_len;
    char pam[];
} bdsc_eventupload_params_t;

typedef struct {
    int wakeup_num;
} bdsc_wp_params_t;

bdsc_asr_params_t *bdsc_asr_params_create(char *sn,
        uint32_t primary_pid, uint32_t assist_pid, char *key,
        uint16_t audio_rate, char *cuid, int wakeup_status,
        uint16_t pam_len, char *pam);

bdsc_asr_params_t *bdsc_asr_params_create_ext(char *sn,
        uint32_t primary_pid, uint32_t assist_pid, char *key,
        uint16_t audio_rate, char *cuid, int wakeup_status,
        uint16_t pam_len, char *pam, int voice_print);

void bdsc_asr_params_destroy(bdsc_asr_params_t *params);

bdsc_eventupload_params_t *bdsc_event_params_create(char *sn,
                                                    uint32_t pid, char *key, char *cuid, uint16_t pam_len, char *pam);

void bdsc_event_params_destroy(bdsc_eventupload_params_t *params);

#endif /* MAIN_CLIENT_BDS_CLIENT_PARAMS_H_ */
