//
// Created by ligaofeng on 19-11-8.
//

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#include "bds_player.h"
#include "result_out.h"

// sdk
#include "bds_client_context.h"
#include "bds_client.h"
#include "bds_common_utility.h"
#include "bds_hal_self_test.h"

#include "command_monitor.h"

#define  TAG  "AUDIO_CASE"

/**
 * case: 播放flash中音频，同时录制原始音频，输出到pc端检测
 */
#define PLAYER_PCM_SIZE         ( 1 * 96 * 2000 )  // chann:1 sample:48k bitrate:16，2s数据
#define RECORDER_PCM_SIZE       ( 3 * 32 * 2000 + 1024)  //chann:3 sample:16k bitrate:16，2s数据,冗余1024
#define PLAYER_FRAME_SIZE       ( 1024 )

extern const uint8_t audio_1k_2s_wav_start[] asm("_binary_1k_2s_wav_start");
extern const uint8_t audio_1k_2s_wav_end[]   asm("_binary_1k_2s_wav_end");

const char *pa_mic_header = RESULT_PA_MIC_DATA;
const char *mic_seal_header = RESULT_SEAL;
const char *testcase_header = NULL;

const char *pa_mic_cmd = CMD_PA_MIC;
const char *mic_seal_cmd = CMD_SEAL;
const char *mic_cmd = NULL;

static void player_exec(void *params) {
    int ret = 0;
    int pcm_length = 0;
    unsigned char *buffer = (unsigned char *)audio_1k_2s_wav_start;
    
    ESP_LOGI(TAG, "player start(%d)", audio_1k_2s_wav_end - audio_1k_2s_wav_start);
    ret = duer_audio_dac_start_play(0, 1, 48000, 16);
    if (ret != 0)
    {
        out_print_result(testcase_header, FAILED, "init dac fail!");
        vTaskDelete(NULL);
        return;
    }
    
    while(1) {
        // write to dac
        ret = duer_audio_dac_write(0, buffer, PLAYER_FRAME_SIZE);
        if (ret < 0) {
            out_print_result(testcase_header, FAILED, "player write pa failed");
            break;
        }
        pcm_length += PLAYER_FRAME_SIZE;
        buffer += PLAYER_FRAME_SIZE;
        if ((pcm_length > PLAYER_PCM_SIZE) || (buffer > audio_1k_2s_wav_end)) {
            break;
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
    ret = duer_audio_dac_wait_play_end();
    duer_audio_dac_unload_driver();
    vTaskDelete(NULL);
}

static void recorder_exec(void* params) {
    // start recorder
    bds_pcm_params recorder_pam = {
            .channel = 3,
            .sample = 16000,
            .bitrate = 16
    };
    int mini_buffer_size = 0;
    int pcm_length = 0;
    bds_self_test_handle handle = NULL;
    unsigned char* out_buffer = NULL;
    int out_length = 0;
    int ret = 0;
    
    ESP_LOGI(TAG, "recorder start");
    handle = bds_pcm_open(&recorder_pam, &mini_buffer_size);
    if (handle == NULL) {
        out_print_result(testcase_header, FAILED, "bds pcm reader open fail");
        vTaskDelete(NULL);
        return;
    }

    out_buffer = heap_caps_malloc(RECORDER_PCM_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (out_buffer == NULL) {
        out_print_result(testcase_header, FAILED, "malloc fail");
        vTaskDelete(NULL);
    }

    while (1) {
        if ((pcm_length + mini_buffer_size) > RECORDER_PCM_SIZE) {
            break;
        }
        ret = bds_pcm_read(handle, out_buffer + pcm_length, &out_length);
        if (ret < 0) {
            out_print_result(testcase_header, FAILED, "mic read error");
            break;
        }
        pcm_length += out_length;
        if (pcm_length >= RECORDER_PCM_SIZE) {
            break;
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
    bds_pcm_close(handle);

    data_out_exec(testcase_header, out_buffer, RECORDER_PCM_SIZE);
    heap_caps_free(out_buffer);
    out_print_data_ctl(testcase_header);
    vTaskDelete(NULL);
}

// extern int g_dsp_check;
int start_player_recorder_case(int type, const char* cmdline) {
    // init dsp
    int ret = 0;

    int  vol = 0;
    static char testing = 0;
    int res = -1;
    mic_cmd = (type == 0) ? pa_mic_cmd : mic_seal_cmd;
    testcase_header = (type == 0) ? pa_mic_header : mic_seal_header;
    
    size_t preLen = strlen(mic_cmd) + 1;
    size_t inputVolLen = strlen(cmdline) - preLen;

    
    if (testing == 1)
    {
        // out_print_result(RESULT_MIC, FAILED, "testing");
        goto exit;
    }

    testing = 1;
    if (inputVolLen > 0 && inputVolLen <= 3) {
        for (int i = 0; i < inputVolLen; i++)
        {
            vol = vol * 10 + cmdline[i+preLen] - 0x30;
        }
    } else {
        vol = 70;
    }
    
    // if (g_dsp_check == 1) {
    //     // out_print_result(RESULT_MIC, FAILED, "dsp error");
    //     goto exit;
    // }

    ret = duer_audio_dac_load_driver();
    duer_audio_dac_set_volume(vol); //100 -> 70 05-21
    if (ret != 0) {
        // out_print_result(RESULT_PA, FAILED, "pa driver load error");
        goto exit;
    }
    // start recorder task
    ret = xTaskCreate(&recorder_exec, "recorder_task", 1024 * 4, NULL, 5, NULL);
    if (ret != pdPASS) {
        out_print_result(testcase_header, FAILED, "start recorder_task fail");
        goto exit;
    }

    // start player task
    ret = xTaskCreate(&player_exec, "player_task", 1024 * 4, NULL, 5, NULL);
    if (ret != pdPASS) {
        out_print_result(testcase_header, FAILED, "start player_task fail");
        goto exit;
    }
    testing = 0;
    res = 0;
exit:
    return res;
}
