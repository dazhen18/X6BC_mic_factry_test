//
// Created by ligaofeng on 19-11-14.
//
#include "case_interface.h"
#include "result_out.h"
#include "command_monitor.h"

// sdk
#include "bds_client_context.h"
#include "bds_client.h"
#include "bds_common_utility.h"
#include "bds_hal_self_test.h"

static bds_client_handle_t dsp_sdk_init(void) {
    int ret = -1;
    bds_client_context_t context = {};
    bds_client_handle_t handle = NULL;

    handle = bds_client_create(&context);
    if (handle == NULL) {
        out_print_result(RESULT_DSP, FAILED, "bds client create fail");
    } else {
        bds_client_params_t params = {
            .launch_mode = 0,
            .dsp_subtype = 0x24,
        };
        // init dsp
        bds_client_config(handle, &params);
        bds_set_log_level(2);
        ret = bds_client_start(handle);
        if (ret) {
            out_print_result(RESULT_DSP, FAILED, "bds client start fail");
        } else {
            bds_switch_self_test(true);
        }
    }
    
    return handle;
}

int start_dsp_check_case(bds_client_handle_t handle) {
    if (handle != NULL)
    {
        out_print_result(RESULT_DSP, SUCCESS, "dsp run success");
        return 0;
    }
    // DSP&SDK init
    handle = dsp_sdk_init();
    if (handle == NULL) {
        out_print_result(RESULT_DSP, FAILED, "dsp boot error");
        return -1;
    } else {
        out_print_result(RESULT_DSP, SUCCESS, "dsp run success");
        return 0;
    }
}