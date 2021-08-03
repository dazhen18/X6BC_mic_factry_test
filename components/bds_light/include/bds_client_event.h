/*
 * bds_client_event.h
 *
 *  Created on: Jan 22, 2019
 *      Author: liweigao
 */

#ifndef MAIN_CLIENT_BDS_CLIENT_EVENT_H_
#define MAIN_CLIENT_BDS_CLIENT_EVENT_H_

#include <stdbool.h>

#include "bds_client_context.h"

typedef enum {
    EVENT_ASR_BEGIN = 1000,
    EVENT_ASR_RESULT, // msg received through sdk.
    EVENT_ASR_EXTERN_DATA, // msg received through sdk.
    EVENT_ASR_TTS_DATA, // msg received through sdk.
    EVENT_ASR_END,
    EVENT_ASR_CANCEL,
    EVENT_ASR_ERROR,
#ifdef CONFIG_USING_WEBAPI
    EVENT_ASR_WEBAPI_AUDIO,  // _WEBAPI_xxx msg should upload through webapi.
    EVENT_ASR_WEBAPI_START,
    EVENT_ASR_WEBAPI_FINISH,
    EVENT_ASR_WEBAPI_CANCEL,
#endif // CONFIG_USING_WEBAPI
    EVENT_WAKEUP_TRIGGER = 2000,
    EVENT_WAKEUP_ERROR,
    EVENT_WAKEUP_OFFLINE_DIRECTIVE,
    EVENT_EVENTUPLOAD_BEGIN = 3000,
    EVENT_EVENTUPLOAD_END,
    EVENT_EVENTUPLOAD_DATA,
    EVENT_EVENTUPLOAD_TTS,
    EVENT_EVENTUPLOAD_CANCEL,
    EVENT_EVENTUPLOAD_ERROR,
    EVENT_PUSH_DATA = 4000,
    EVENT_PUSH_ERROR,
    EVENT_LINK_CONNECTED = 5000,
    EVENT_LINK_DISCONNECTED,
    EVENT_LINK_ERROR,
    EVENT_RECORDER_DATA = 6000,
    EVENT_RECORDER_ERROR,
    EVENT_SDK_START_COMPLETED = 7000,
    EVENT_DSP_FATAL_ERROR = 8000,
    EVENT_DSP_LOAD_FAILED = 8001,
    EVENT_TTS_BEGIN = 9000,
    EVENT_TTS_END,
    EVENT_TTS_RESULT,
    EVENT_TTS_CANCEL,
    EVENT_TTS_ERROR,
} bdsc_event_key_t;

// content may be basetype or struct, can custom
typedef struct {
    bdsc_event_key_t key;
    void *content;
    uint16_t content_length;
} bds_client_event_t;

typedef struct {
    char sn[SN_LENGTH];
    int32_t code;
    uint16_t info_length;
    char info[];
} bdsc_event_error_t;

typedef struct {
    char sn[SN_LENGTH];
    int16_t idx;
    uint16_t buffer_length;
    uint8_t buffer[];
} bdsc_event_data_t;

typedef struct {
    char sn[SN_LENGTH];
} bdsc_event_process_t;

typedef struct {
    int status;
    uint16_t dci_length;
    uint8_t dci_buffer[];
} bdsc_event_wakeup_t;

bdsc_event_error_t* bdsc_event_error_create(char *sn,
        int32_t code, uint16_t info_length, char *info);

void bdsc_event_error_destroy(bdsc_event_error_t *error);

bdsc_event_data_t* bdsc_event_data_create(char *sn,
        int16_t idx, uint16_t buffer_length, uint8_t *buffer);

void bdsc_event_data_destroy(bdsc_event_data_t *data);

bdsc_event_wakeup_t*  bdsc_wakeup_data_create(int status, uint16_t buffer_length, uint8_t *buffer);

void bdsc_wakeup_data_destroy(bdsc_event_wakeup_t *data);

bool bdsc_deepcopy_event(bds_client_event_t *dst, bds_client_event_t *src);

void bdsc_deepdestroy_event(bds_client_event_t *event);

#endif /* MAIN_CLIENT_BDS_CLIENT_EVENT_H_ */
