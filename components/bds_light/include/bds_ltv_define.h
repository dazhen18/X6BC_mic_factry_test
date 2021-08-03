//
// Created by ligaofeng on 19-12-9.
//
#ifndef ESP_SDK_CUPID_BDS_LTV_DEFINE_H
#define ESP_SDK_CUPID_BDS_LTV_DEFINE_H

#include <stdint.h>
#include <stdbool.h>
enum WSASRRequestType {
    ASR_REQ_TYPE_PARAM = 0x0,
    ASR_REQ_TYPE_AUDIO = 0x1,
    ASR_REQ_TYPE_THIRD = 0x2,
    ASR_REQ_TYPE_LAST = 0x3,
    ASR_REQ_TYPE_CANCEL = 0x4,
    ASR_REQ_TYPE_REALTIME = 0x5,
    ASR_REQ_TYPE_TTS = 0x6,
    ASR_REQ_TYPE_FINAL_AUDIO = 0x7,
    ASR_REQ_TYPE_WAKEUP_UPLOAD = 0x8,
    ASR_REQ_TYPE_NULL = 0x9,  //heart beat
    ASR_REQ_TYPE_PROXY_ERR = 0xa,
    //ASR_REQ_TYPE_FINAL_THIRD    = 0xb; //used in dcs
    ASR_REQ_TYPE_TTS_AUDIO = 0xd,  //tts wakeup audio
    ASR_REQ_TYPE_DCI = 0xe,  //some control message
    ASR_REQ_TYPE_SDK_CLOSE = 0xf,  //sdk主动断开
    ASR_REQ_TYPE_SUSPECTED_AUDIO = 0x10, //疑似唤醒音频
    ASR_REQ_TYPE_ORIGINAL_AUDIO = 0x11, //原始音频
    ASR_REQ_TYPE_SIGNAL_DEBUG = 0x13 //信号debug
};

//need convert to uint8 while using in protocol header
enum WSASRResponseType {
    ASR_RSP_TYPE_MIDDLE_RES = 0xf0,
    ASR_RSP_TYPE_FINAL_RES = 0xf1,
    ASR_RSP_TYPE_THIRD_RES = 0xf2,
    ASR_RSP_TYPE_LAST_RES = 0xf3,
    ASR_RSP_TYPE_NULL = 0xf4,  //heart beat
    ASR_RSP_TYPE_NLU_RES = 0xf5,
    ASR_RSP_TYPE_WAKE_RES = 0xf6,
    ASR_RSP_TYPE_TTS_RES = 0xf7,
    ASR_RSP_TYPE_WAKE_UPLOAD_RES = 0xf8,
    ASR_RSP_TYPE_VP_RES = 0xf9,
    ASR_RSP_TYPE_ASR_EOF = 0xfc
};

enum EventRequestType {
    EVENT_REQ_TYPE_PARAM = 0x00,
    EVENT_REQ_TYPE_THIRD = 0x0a,
    EVENT_REQ_TYPE_THIRD_LAST = 0x0b,
    EVENT_REQ_TYPE_END
};

enum EventResponseType {
    EVENT_RSP_TYPE_THIRD = 0xf2,
    EVENT_RSP_TYPE_END = 0xf3,
    EVENT_RSP_TYPE_TTS = 0xf7
};

enum TTSRequestType {
    TTS_REQ_TYPE_PARAMS = 0x01,
    TTS_REQ_TYPE_CANCLE = 0x0e
};

enum TTSResponseType {
    TTS_RSP_TYPE_DATA = 0x50,
    TTS_RSP_TYPE_END = 0x5f
};

#define PACKAGE_HEAD_SIZE  8
#define HEAD_LENGTH_SIZE   4
#define HEAD_VERSION_SIZE  1
#define HEAD_REMAIN_SIZE   2
#define HEAD_TYPE          1

#define HEAD_LENGTH_OFFSET  0
#define HEAD_VERSION_OFFSET 4
#define HEAD_REMAIN_OFFSET  5
#define HEAD_TYPE_OFFSET    7
#define HEAD_BODY_OFFSET    PACKAGE_HEAD_SIZE

typedef struct LTVHead
{
    uint32_t length;
    uint8_t version;
    uint16_t remain : 12;
    uint16_t idx : 4;
    uint8_t type;
} LTVHead_t;

typedef struct LTVBody
{
    uint8_t *m_body;
} LTVBody_t;

typedef struct LTVPackage
{
    LTVHead_t head;
    LTVBody_t body;
} LTVPackage;

char* print_package_type(int type);

bool ltv_str_value(int type);

#endif //ESP_SDK_CUPID_BDS_LTV_DEFINE_H
