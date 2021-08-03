//
// Created by ligaofeng on 19-11-15.
//

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

#define  TAG  "NONLINEAR_CASE"
/**
 * case: 播放flash中音频，同时录制原始音频，输出到pc端检测
 */
#define PLAYER_PCM_SIZE         ( 1 * 96 * 10000 )  // 1通道，10s数据
#define RECORDER_PCM_SIZE         ( 3 * 32 * 10000 + 4096)  // 3通道，10s数据,冗余4096
#define PLAYER_FRAME_SIZE       ( 1024 )

extern const uint8_t audio_log_10s_mono_wav_start[] asm("_binary_log_10s_mono_wav_start");
extern const uint8_t audio_log_10s_mono_wav_end[]   asm("_binary_log_10s_mono_wav_end");

static void player_exec(void *params) {
    int ret = 0;
    int pcm_length = 0;
    unsigned char *buffer = (unsigned char *)audio_log_10s_mono_wav_start;
    
    ESP_LOGI(TAG, "player start(%d)", audio_log_10s_mono_wav_end - audio_log_10s_mono_wav_start);
    ret = duer_audio_dac_start_play(0, 1, 48000, 16);
    if (ret != 0)
    {
        out_print_result(RESULT_NOLINEAR, FAILED, "init dac fail!");
        vTaskDelete(NULL);
        return;
    }
    
    while(1) {
        // write to dac
        ret = duer_audio_dac_write(0, buffer, PLAYER_FRAME_SIZE);
        if (ret < 0) {
            out_print_result(RESULT_NOLINEAR, FAILED, "write dac fail!");
            break;
        }
        pcm_length += PLAYER_FRAME_SIZE;
        buffer += PLAYER_FRAME_SIZE;
        if ((pcm_length > PLAYER_PCM_SIZE) || (buffer > audio_log_10s_mono_wav_end)) {
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
        out_print_result(RESULT_NOLINEAR, FAILED, "bds pcm reader open fail");
        vTaskDelete(NULL);
        return;
    }

    out_buffer = heap_caps_malloc(RECORDER_PCM_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (out_buffer == NULL) {
        out_print_result(RESULT_NOLINEAR, FAILED, "malloc fail");
        vTaskDelete(NULL);
    }

    while (1) {
        if ((pcm_length + mini_buffer_size) > RECORDER_PCM_SIZE) {
            break;
        }
        ret = bds_pcm_read(handle, out_buffer + pcm_length, &out_length);
        if (ret < 0) {
            out_print_result(RESULT_NOLINEAR, FAILED, "mic read error");
            break;
        }
        pcm_length += out_length;
        if (pcm_length >= RECORDER_PCM_SIZE) {
            break;
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
    bds_pcm_close(handle);

    data_out_exec(RESULT_NOLINEAR, out_buffer, RECORDER_PCM_SIZE);
    heap_caps_free(out_buffer);
    out_print_data_ctl(RESULT_NOLINEAR);
    vTaskDelete(NULL);
}

int start_nolinear_case(const char* cmdline) {
    // init dsp
    int  ret = 0;
    int  vol = 0;
    static char testing = 0;
    int res = -1;
    size_t preLen = strlen(CMD_NOLINEAR) + 1;
    size_t inputVolLen = strlen(cmdline) - preLen;

    if (testing == 1)
    {
        out_print_result(RESULT_NOLINEAR, FAILED, "testing");
        goto exit;
    }

    testing = 1;
    if (inputVolLen > 0 && inputVolLen <= 3) {
        for (int i = 0; i < inputVolLen; i++)
        {
            vol = vol * 10 + cmdline[i+preLen] - 0x30;
        }
    } else {
        vol = 100;
    }
    ret = duer_audio_dac_load_driver();
    duer_audio_dac_set_volume(vol);  //100
    if (ret != 0) {
        out_print_result(RESULT_NOLINEAR, FAILED, "pa driver load error");
        goto exit;
    }
    // start recorder task
    ret = xTaskCreate(&recorder_exec, "recorder_task", 1024 * 8, NULL, 5, NULL);
    if (ret != pdPASS) {
        out_print_result(RESULT_NOLINEAR, FAILED, "start recorder_task fail");
        goto exit;
    }
    // start player task
    ret = xTaskCreate(&player_exec, "player_task", 1024 * 8, NULL, 5, NULL);
    if (ret != pdPASS) {
        out_print_result(RESULT_NOLINEAR, FAILED, "start player_task fail");
        goto exit;
    }
    testing = 0;
    res = 0;
exit:
    return res;
}
