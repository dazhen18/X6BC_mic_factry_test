/*
 * bds_client_command.h
 *
 *  Created on: Jan 22, 2019
 *      Author: liweigao
 */

#ifndef MAIN_CLIENT_BDS_CLIENT_COMMAND_H_
#define MAIN_CLIENT_BDS_CLIENT_COMMAND_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bds_client_params.h"

typedef enum {
    CMD_ASR_START = 100,
    CMD_ASR_CANCEL,
    CMD_ASR_EXTERN_DATA,
    CMD_ASR_CONFIG,
    CMD_ASR_STOP,
    CMD_WAKEUP_START = 200,
    CMD_WAKEUP_STOP,
    CMD_EVENTUPLOAD_START = 300,
    CMD_EVENTUPLOAD_CANCEL,
    CMD_EVENTUPLOAD_DATA,
    CMD_LINK_START = 500,
    CMD_LINK_STOP,
    CMD_RECORDER_START = 600,
    CMD_RECORDER_STOP,
    CMD_DYNAMIC_CONFIG = 700,
    CMD_TTS_HEADER = 701,
    CMD_TTS_START = 800,
    CMD_TTS_CANCEL
} bdsc_cmd_key_t;

typedef struct {
    bdsc_cmd_key_t key;
    void *content;
    uint16_t content_length;
} bds_client_command_t;

typedef struct {
    char sn[SN_LENGTH];
    int32_t flag;
    uint16_t buffer_length;
    uint8_t buffer[];
} bdsc_cmd_data_t;

bdsc_cmd_data_t* bdsc_cmd_data_create(int32_t flag, uint16_t buffer_length, uint8_t *buffer, char* sn);

void bdsc_cmd_data_destroy(bdsc_cmd_data_t *data);

bool bdsc_deepcopy_command(bds_client_command_t *dst, bds_client_command_t *src);

void bdsc_deepdestroy_command(bds_client_command_t *command);

#endif /* MAIN_CLIENT_BDS_CLIENT_COMMAND_H_ */
