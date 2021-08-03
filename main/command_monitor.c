#include "command_monitor.h"
#include "case_interface.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include "rom/uart.h"
#include "result_out.c"
#include "bds_client.h"

typedef enum monitor_cmd {
    CMD_HELP = '0',
    CMD_LED_BULE = '1'
} monitor_cmd;

static void monitor_help() {
    printf("*************   command monitor help  ***********\n");
//    printf("Here's monitor menu, Enter your KEY:\n");
//    printf("[a] : %s case; \n", CASE_NAME_PLAYER_RECORDER);
//    printf("[b] : %s case; \n", CASE_NAME_BLUETOOTH);
//    printf("[c] : xxxx case; \n");
//    printf(" \n");
    printf("************* command monitor help end ***********\n\n");
}

int my_getline(char* line, int max_size)
{
    int c = 0;
    int len = 0;
    while((c = getchar()) != EOF && len < max_size) {
        line[len++] = c;
        if('#' == c)
        {
            break;
        }
    }
    line[len] = '\0';
    return len;
}

static void trim_trailing_space(char* str)
{
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace((int) *end))
    {
        *end = 0;
        --end;
    }
}

bool string_equal(char *cmdline, char* target)
{
#if 1
    int i = 0;
    while (cmdline[i] != '\0') {
        if (cmdline[i] != target[i])
        {
            return false;
        }
        i++;
    }
    return true;
#else
    int i, j, bs;
    bs = 0;
    i = strlen(cmdline);
    j = strlen(target);
    while (j >= 0) {
        if (cmdline[i] != target[j]) {
            if (cmdline[i] == '\b') {
                bs++;
                i--;
            }
            else {
                if (bs == 0) {
                    return false;
                }
                else {
                    i = i - bs;
                    bs = 0;
                }
            }
        }
        else {
            i--;
            j--;
        }
    }
    return true;
#endif    
}

bool string_content(char *a, char *p)
{
    char *ptemp = p;
    while (*a != '\0')
    {
        if (*a == *p)
        {
            a++;
            p++;
        }
        else
        {
            a++;
            p = ptemp;
        }
        if (*p == '\0')
        {
            return true;
        }
    }
    return false;
}

void check_task_status() {
    char deviceSn[MAX_DEVICE_SN_LENGTH + 1] = {'\0'};
    size_t length = MAX_DEVICE_SN_LENGTH;
    int snRet = get_sn_case(deviceSn, &length);

    if (snRet != ESP_OK) {
        snprintf(deviceSn, MAX_DEVICE_SN_LENGTH, "SN_ERR:%d", snRet);
    }

    out_print_result(RESULT_TASK, SUCCESS, deviceSn);
}

static void command_task(void *pvParameters)
{
    (void)pvParameters;
    bds_client_handle_t bds_client_handle = NULL;
    
    check_task_status();

    //led_red_on();
    for (;;) {
        char cmdline[256] = { 0 };
        while(strlen(cmdline) == 0)
        {
            /* Flush anything already in the RX buffer */
            while(uart_rx_one_char((uint8_t *) cmdline) == OK) {
            }
            /* Read input */
            UartRxString((uint8_t*) cmdline, sizeof(cmdline) - 1);
            trim_trailing_space(cmdline);
            if(strlen(cmdline) == 0) {
                /* if input was newline, print a new menu */
                monitor_help();
            }
        }
        
        if (strstr(cmdline, "profile") != NULL)
        {
            printf("cmdline: prod_test profile ...\n");
        }
        else
        {
            printf("cmdline:%s \n", cmdline);
        }
        if (string_equal(cmdline, CMD_TASK)) {
            check_task_status();
        } 
        else if (string_content(cmdline, CMD_PA_MIC)) 
        {
            start_player_recorder_case(0, cmdline);
        } 
        else if (string_content(cmdline, CMD_SEAL)) 
        {
            start_player_recorder_case(1, cmdline);
        } 
        else if (string_equal(cmdline, CMD_DSP)) 
        {
            start_dsp_check_case(bds_client_handle);
        } 
        else if (string_content(cmdline, CMD_NOLINEAR)) 
        {
            start_nolinear_case(cmdline);
        }
        else if (string_equal(cmdline, CMD_GET_SN)) 
        {
            char deviceSn[MAX_DEVICE_SN_LENGTH + 1] = {'\0'};
            size_t length = MAX_DEVICE_SN_LENGTH;
            get_sn_case(deviceSn, &length);
        } 
        else if (string_content(cmdline, CMD_SET_SN)) 
        {
            set_sn_case(cmdline);
        }
        else if (string_equal(cmdline, CMD_RST)) 
        {
            start_reset_system();
        }
        else if (string_equal(cmdline, CMD_OFF)) 
        {
            start_poweroff_system();
        }
        else if (string_equal(cmdline, CMD_FAIL)) 
        {
            start_test_fail();
        }
        else if (string_equal(cmdline, CMD_INTR)) 
        {
            start_dsp_intr();
        }
        else if (string_equal(cmdline, CMD_QUIT1)) 
        {
            start_quit1_case();
        }
        else if (string_equal(cmdline, CMD_QUIT2)) 
        {
            start_quit2_case();
        }
        else if (string_content(cmdline, CMD_PROFILE)) 
        {
            start_profile_case(cmdline);
        }
        else if (string_equal(cmdline, RD_PROFIEL)) 
        {
            profile_read();
        }
        else if (string_equal(cmdline, CMD_VERSION)) 
        {
            start_version_case();
        }
        else if (string_content(cmdline, CMD_SEAL)) 
        {
            start_mic_seal_case(cmdline);
        }

//        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

int monitor_init()
{
    int ret = xTaskCreatePinnedToCore(command_task, "command_task", 4 * 1024, NULL,
                            5, NULL, 0);

    if (ret != pdPASS) {
        printf("start command_task fail!!");
        return -1;
    }

    return 0;
}
