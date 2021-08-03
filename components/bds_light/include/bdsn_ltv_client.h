//
// Created by ligaofeng on 19-2-1.
//

#ifndef SPEECH_NET_BDSN_LTV_CLENT_H
#define SPEECH_NET_BDSN_LTV_CLENT_H

#include "bdsn_ltv_client_params.h"

typedef void* bdsn_ltv_client;

bdsn_ltv_client bdsn_ltv_client_create(char *name, transport_type tp_type);

void bdsn_ltv_config(bdsn_ltv_client handle_t, bdsn_connection_params *params);

int bdsn_ltv_open(bdsn_ltv_client  handle_t, const char* sn);

int bdsn_ltv_close(bdsn_ltv_client  handle_t, const char* sn);

int bdsn_ltv_get_status(bdsn_ltv_client  handle_t);

int bdsn_ltv_send_data(bdsn_ltv_client  handle_t, bdsn_connection_data *data_message);

int bdsn_ltv_client_destory(bdsn_ltv_client  handle_t);

void bdsn_ltv_reg_listener(bdsn_ltv_client  handle_t, bdsn_connection_listener *listener);

void bdsn_ltv_unreg_listener(bdsn_ltv_client  handle_t);

#endif // SPEECH_NET_BDSN_LTV_CLENT_H
