//
// Created by ligaofeng on 19-11-14.
//
#include "freertos/FreeRTOS.h"
#include "case_interface.h"
#include "result_out.h"
#include "command_monitor.h"
#include "userconfig.h"

// sdk
#include "bds_client_context.h"
#include "bds_client.h"
#include "bds_common_utility.h"
#include "bds_hal_self_test.h"

#define TAG   "DSP_INTR"

int start_dsp_intr(void) {
    
    // DSP interrupt testcase
    if (bds_gpio_intr_test(GPIO_DSP_IRQ, 100) != 0)
    {
        out_print_result(RESULT_INTR_LEVEL, FAILED, "dsp error");
        return -1;
    }
    else
    {
        out_print_result(RESULT_INTR_LEVEL, SUCCESS, "dsp interrupt detected");
        return 0;
    }
}
