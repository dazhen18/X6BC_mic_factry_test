//
// Created by ligaofeng on 19-4-19.
//

#ifndef BDSN_CONNECT_PARAMS_H
#define BDSN_CONNECT_PARAMS_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "bds_ltv_define.h"
#include "bds_client_context.h"

typedef enum {
    NET_ERROR_OK = 0,
    NET_ERROR_IO_BREAK = -6003,
    NET_ERROR_CONNECT_FAILED = -6002
} NET_ERROR_NO;

typedef enum {
    LTV_STATUS_OK = 0,
    LTV_STATUS_RESPONSE_TIMEOUT = -6004
} LTV_STATUS;

typedef enum {
    TP_WS = 0,
    ESP_WS = 1,
    TP_HTTP = 2
} transport_type;

/**
 * 配置参数
 */
typedef struct {
    char uri[HOST_LENGTH + PATH_LENGTH];
    char host[HOST_LENGTH];
    int port;
    char path[PATH_LENGTH];
    bool auto_keep_alive;
} bdsn_connection_params ;

/**
 * (error/status）接口参数
 */
typedef struct connection_event
{
    char sn[SN_LENGTH];
    int16_t err_no;
    uint16_t status;
    char* info;
} bdsn_connection_event;

bdsn_connection_event* bdsn_connection_event_clone(bdsn_connection_event* event);
void bdsn_connection_event_destroy(bdsn_connection_event* event, bool is_pointer);

/**
 * 数据接口参数
 */
typedef struct connection_data {
    char sn[SN_LENGTH];
    LTVPackage package;
} bdsn_connection_data;

int bdsn_build_message(char* sn, int data_type, int data_len, uint8_t *data, bdsn_connection_data *out);

bdsn_connection_data* bdsn_connection_data_clone(bdsn_connection_data* data);
void bdsn_connection_data_destroy(bdsn_connection_data* data, bool is_pointer);

typedef void (*bdsn_connection_data_func)(void *custom, bdsn_connection_data *data);
typedef void (*bdsn_connection_event_func)(void *custom, bdsn_connection_event *event);

/**
 * listener
 */
typedef struct connection_listener
{
    void *custom;
    bdsn_connection_data_func data_func;
    bdsn_connection_event_func event_func;
} bdsn_connection_listener;

#endif // BDSN_CONNECT_PARAMS_H
