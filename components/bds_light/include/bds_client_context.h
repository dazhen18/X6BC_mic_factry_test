/*
 * bds_client_context.h
 *
 *  Created on: Jan 22, 2019
 *      Author: liweigao
 *
 *      set or get some system environment value
 */

#ifndef MAIN_CLIENT_BDS_CLIENT_CONTEXT_H_
#define MAIN_CLIENT_BDS_CLIENT_CONTEXT_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define BYTE1  __attribute__((packed, aligned(1)))

// Define interface return value.
#define RET_OK                                   0
#define RET_BAD_PARAM                           -1
#define RET_BAD_STATE                           -2
#define RET_BAD_OPERATE                         -3
#define RET_UNHANDLED                           -4

// Define interface error code.
#define ERROR_BDSC_INVALID_RESOURCE             -1000
#define ERROR_BDSC_ASR_START_FAILED             -2000
#define ERROR_BDSC_ASR_CANCEL_FAILED            -2001
#define ERROR_BDSC_ASR_NET_ERROR                -2002  
#define ERROR_BDSC_ASR_HD_SERVER_ERROR          -2003
#define ERROR_BDSC_ASR_TYPE_NOT_RSP             -2004
#define ERROR_BDSC_RECORDER_START_FAILED        -3000
#define ERROR_BDSC_RECORDER_READ_FAILED         -3001
#define ERROR_BDSC_EVENTUPLOAD_START_FAILED     -4000
#define ERROR_BDSC_EVENTUPLOAD_CANCEL_FAILED    -4001
#define ERROR_BDSC_EVENTUPLOAD_ENGINE_BUSY      -4002
#define ERROR_BDSC_EVENTUPLOAD_NET_ERROR        -4003  
#define ERROR_BDSC_EVENTUPLOAD_HD_SERVER_ERROR  -4004
#define ERROR_BDSC_EVENTUPLOAD_TYPE_NOT_RSP     -4005
#define ERROR_BDSC_PUSH_NET_ERROR               -4006
#define ERROR_BDSC_WAKEUP_START_FAILED          -5000
#define ERROR_BDSC_WAKEUP_STOP_FAILED           -5001
#define ERROR_BDSC_LINK_START_INVALID           -6000
#define ERROR_BDSC_LINK_STOP_INVALID            -6001
#define ERROR_BDSC_LINK_CONNECT_FAILED          -6002
#define ERROR_BDSC_LINK_IO_BREAK                -6003
#define ERROR_BDSC_LINK_RESPONSE_TIMEOUT        -6004
#define ERROR_BDSC_LINK_FINISH_TIMEOUT          -6005
#define ERROR_BDSC_MIC_SIGNAL_AM_ALL_ZERO       -7000
#define ERROR_BDSC_MIC_SIGNAL_AM_CLIPPING       -7001
#define ERROR_BDSC_MIC_SIGNAL_UNIFORM           -7002
#define ERROR_BDSC_MIC_SIGNAL_INCONSISTENCY     -7003
#define ERROR_BDSC_DSP_RUNTIME_ERROR            -7004
#define ERROR_BDSC_TTS_START_FAILED             -8000
#define ERROR_BDSC_TTS_CANCEL_FAILED            -8001
#define ERROR_BDSC_TTS_NET_ERROR                -8002
#define ERROR_BDSC_TTS_RESPONSE_TIMEOUT         -8003

#define SN_LENGTH                               37
#define KEY_LENGTH                              32
#define CUID_LENGTH                             37
#define UUID_LENGTH                             37
#define APP_LENGTH                              64
#define WAKEUP_WORDS_LENGTH                     32
#define URI_LENGTH                              128
#define SCHEME_LENGTH                           5
#define HOST_LENGTH                             64
#define PORT_LENGTH                             5
#define PATH_LENGTH                             64
#define PARAM_LENGTH                            64

#define FLAG_DEFAULT                            0
#define FLAG_TAIL                               1

#define KEY_ASR_MODE_STATE                      "asr_mode_state"
#define KEY_WS_HEAD_HOST                        "head_host"
#define KEY_SELF_WAKEUP_RESTRAIN                "self_wakeup_restrain"
#define KEY_OFFLINE_DIRECTIVE_DURATION          "offline_directive_duration"
#define KEY_IDX                                 "idx"
#define KEY_TTS_HOLD_WAKE                       "tts_hold_wake"
#define KEY_TTS_KEY_SP                          "tts_key_sp"
#define KEY_SN                                  "sn"
#define KEY_PER                                 "per"
#define KEY_TTS_TOTAL_TIME                      "tts_total_time"
#define KEY_SPK                                 "spk"
#define KEY_SADDR                               "saddr"
#define KEY_LENGTH_STR                          "length"
#define KEY_NIGHT_MODE                          "night_mode"
#define KEY_DBG_FIRST_WP                        "dbg_first_wp"   
#define KEY_DBG_AUDIO_SPEED                     "dbg_audio_speed"

#define KEY_HOST                                "host"
#define KEY_PORT                                "port"
#define KEY_CUID                                "cuid"
#define KEY_PID                                 "pid"
#define KEY_KEY                                 "key"
#define KEY_ENGINE_URI                          "engine_uri"
#define KEY_NET_LOG_LEVEL                       "qnet_log_level"
#define KEY_WAKEUP_WORDS                        "wakeup_words"
#define KEY_PDT                                 "pdt"
#define KEY_APPID                               "appid"
#define KEY_APPKEY                              "appkey"
#define KEY_SN                                  "sn"
#define KEY_TEX                                 "tex"
#define KEY_SPD                                 "spd"
#define KEY_PIT                                 "pit"
#define KEY_VOL                                 "vol"
#define KEY_PER                                 "per"
#define KEY_AUE                                 "aue"
#define KEY_RATE                                "rate"
#define KEY_XML                                 "xml"
#define KEY_PATH                                "path"
#define KEY_LAN                                 "lan"
#define KEY_MODE                                "mode"
#define KEY_PAM                                 "pam"

typedef struct {
    char sn[SN_LENGTH];
    int32_t flag;
    uint16_t buffer_length;
    uint16_t real_length;
    uint8_t buffer[];
} bdsc_audio_t;

typedef struct {
    int32_t code;
    uint16_t info_length;
    char info[];
} bdsc_error_t;

typedef struct {

} bds_client_context_t;

typedef struct bds_sn {
    char sn[SN_LENGTH];
}BYTE1 bds_sn_t;

bdsc_audio_t* bdsc_audio_create(char* sn, int32_t flag, uint16_t buffer_length, const uint8_t *buffer);

void bdsc_audio_destroy(bdsc_audio_t *audio);

bdsc_error_t* bdsc_error_create(int32_t code, uint16_t info_length, char *info);

void bdsc_error_destroy(bdsc_error_t *error);

#endif /* MAIN_CLIENT_BDS_CLIENT_CONTEXT_H_ */
