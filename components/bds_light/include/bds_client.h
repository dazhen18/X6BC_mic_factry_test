/*
 * bds_client.h
 *
 *  Created on: Jan 21, 2019
 *      Author: liweigao
 */

#ifndef MAIN_CLIENT_BDS_CLIENT_H_
#define MAIN_CLIENT_BDS_CLIENT_H_

#include <stdint.h>

#include "bds_client_context.h"
#include "bds_client_event.h"
#include "bds_client_command.h"
#include "bds_client_params.h"

typedef void* bds_client_handle_t;
typedef int32_t (*bds_client_event_listener_f)(bds_client_event_t *event, void *custom);

bds_client_handle_t bds_client_create(bds_client_context_t *context);

void bds_client_config(bds_client_handle_t handle, bds_client_params_t *params);

void bds_client_set_event_listener(bds_client_handle_t handle,
        bds_client_event_listener_f listener, void *custom);

int32_t bds_client_start(bds_client_handle_t handle);

int32_t bds_client_send(bds_client_handle_t handle, bds_client_command_t *command);

int32_t bds_client_stop(bds_client_handle_t handle);

int32_t bds_client_destroy(bds_client_handle_t handle);

#endif /* MAIN_CLIENT_BDS_CLIENT_H_ */
