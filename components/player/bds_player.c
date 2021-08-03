/**
 * Copyright (2017) Baidu Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// Author: WangMujin (WangMuJin@baidu.com)
//
// Description: Implementation of SPIL_OUTPUT codec APIs

#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>
#include <soc/dport_reg.h>
#include "bds_player.h"
#include "audio_resample.h"
#include "userconfig.h"
#include "tas5805m.h"
#include "es8156.h"
#include "es7243e.h"


#define I2S_NUM                 I2S_NUM_0

#define SPIL_OUTPUT_SAMPLE_RATE    (48000)

#define DMA_BUF_COUNT           (5)
#define DMA_BUF_LEN             (500)
#define DMA_BUF_CHANNEL         (2)
#define DMA_BUF_BITS_PER_SAMPLE (32)

//on SPIL_OUTPUT, we need to resample the audio data to 48k sample rate
//so to a 8k sample rate AAC, the decoded pcm size is (16/8 * 2 * 1024), and it need 24k after resample
#define CODEC_OUTPUT_BUFSIZE    (24 * 1024)

static int s_codec_inited = 0;
static int s_channel = 1;
static int s_sample_rate = 0;
static int s_bits_per_sample = 0;
static int s_backup_channel = 1;
static int s_backup_sample_rate = 0;
static int s_backup_bits_per_sample = 0;

static short *output_buf_16 = NULL;
static short *output_buf_32 = NULL;

static RESAMPLE s_resample = {0};

static int i2s0_init(void)
{
    int res = 0;
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = SPIL_OUTPUT_SAMPLE_RATE, //if the codec is NPCA110, then the sample-rate must be set as 48K
        .bits_per_sample = DMA_BUF_BITS_PER_SAMPLE,      //if the codec is NPCA110, then the bits must be set as 32
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        //when dma_buf_count = 3 and dma_buf_len = 300, then 3 * 4 * 300 * 2 Bytes internal RAM will be used.
        //The multiplier 2 is for Rx buffer and Tx buffer together.
        .dma_buf_count = DMA_BUF_COUNT,
        .dma_buf_len = DMA_BUF_LEN,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
        .use_apll = 1,
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = IIS_SCLK,
        .ws_io_num = IIS_LCLK,
        .data_out_num = IIS_DSIN,
        .data_in_num = IIS_DSOUT
    };

    res = i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    if (res) {
        printf("\n\n\n i2s driver install error!!!\n\n");
        return res;
    }
    res |= i2s_set_pin(0, &pin_config);

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    WRITE_PERI_REG(PIN_CTRL, 0xFFF0);

    i2s_zero_dma_buffer(0);

    return res;
}

void tas5805m_init_task()
{
    //tas5805mInit();
    ES8156_codec_init();
    es7243e_adc_init();
    vTaskDelete(NULL);
}

static int codec_init(void) {
    int ret = 0;

    ret = i2s0_init();
    if (ret != 0) {
        printf("Failed to init I2s0 driver\n");
        return ret;
    }
    //ret = tas5805mInit();
    ret = ES8156_codec_init();
    ret = es7243e_adc_init();
    s_codec_inited = 1;

    return ret;
}

static int codec_uninit() {
    i2s_driver_uninstall(I2S_NUM);

    return 0;
}
#if 0
static int SPIL_OUTPUT_set_sample_rate(int sample_rate) {
    return 0;
}

static int SPIL_OUTPUT_set_i2s_clk(int sample_rate, int bits_per_sample, int channel) {
    return 0;
}
#endif
static int SPIL_OUTPUT_zero_dma_buffer(void) {
    int ret = 0;

    ret = i2s_zero_dma_buffer(I2S_NUM);

    if (ret == ESP_OK) {
        return 0;
    } else {
        return -1;
    }
}
#if 0
static int SPIL_OUTPUT_read_data(char *buf, int size) {
    return 0;
}
#endif
static int SPIL_OUTPUT_write_data(const char *buf, int size) {
    return i2s_write_bytes(I2S_NUM, (const void*)buf, size, portMAX_DELAY);
}

static int dac_write_zero_data() {
    char *buf = NULL;
    int size  = 0;

    size = (DMA_BUF_CHANNEL * DMA_BUF_LEN * DMA_BUF_BITS_PER_SAMPLE / 8);
    buf = (char *)heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (buf == NULL) {
        printf("Failed to malloc memory\n");
        return -1;
    }

    int i = 0;
    for (i = 0; i < DMA_BUF_COUNT; i++) {
        SPIL_OUTPUT_write_data((const char*)buf, size);
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return 0;
}

int duer_audio_dac_backup_parameter()
{
    s_backup_channel = s_channel;
    s_backup_sample_rate = s_sample_rate;
    s_backup_bits_per_sample = s_bits_per_sample;

    return 0;
}

int duer_audio_dac_restore_parameter()
{
    s_channel = s_backup_channel;
    s_sample_rate = s_backup_sample_rate;
    s_bits_per_sample = s_backup_bits_per_sample;

    return 0;
}

int duer_audio_dac_start_play(int type, int channel, int sample_rate, int bits_per_sample) {
    printf("TAS5805M sample_rate:%d, bits_per_sample:%d, channel:%d\n", sample_rate, bits_per_sample, channel);

    s_channel = channel;
    s_sample_rate = sample_rate;
    s_bits_per_sample = bits_per_sample;

    memset((void *)&s_resample, 0, sizeof(s_resample));

    return 0;
}

int duer_audio_dac_write(int type, unsigned char *buf, size_t size) {
    int ret = 0;
    int output_sample_rate = SPIL_OUTPUT_SAMPLE_RATE;
    int output_channel = s_channel;
    int output_size = 0;
    int i = 0;

    if (output_buf_16 == NULL || output_buf_32 == NULL) {
        printf("faild to init buffer for codec write\n");
        goto EXIT;
    }

    if (output_channel != 1 && output_channel != 2) {
        printf("unsupport channel %d", output_channel);
        goto EXIT;
    }

    if (s_channel == 1) {
        output_size = resampling_process_up_channel((short *)buf, output_buf_16, s_sample_rate,
                      output_sample_rate, size / 2, &s_resample);
    } else if (s_channel == 2) {
        output_size = resampling_process((short *)buf, output_buf_16, s_sample_rate,
                                         output_sample_rate, size / 2, output_channel, &s_resample);
    }

    for (i = 0; i < output_size; i++) {
        output_buf_32[2 * i + 1] = output_buf_16[i];
    }

    ret = SPIL_OUTPUT_write_data((const char*)output_buf_32, output_size * 4);

EXIT:
    return ret;
}

int duer_audio_dac_pause_play(void) {
    dac_write_zero_data();

    return 0;
}

int duer_audio_dac_release_play(void) {
    return 0;
}

int duer_audio_dac_stop_play(void) {
    return 0;
}

int duer_audio_dac_wait_play_end(void) {
    dac_write_zero_data();

    SPIL_OUTPUT_zero_dma_buffer();

    return 0;
}

int duer_audio_dac_load_driver(void) {
    int ret = 0;
    printf("=================================%s======\n", __func__);
    ret = codec_init();
    if (ret != 0) {
        printf("Failed to load codec driver(%d)\n", ret);
        goto EXIT;
    }

    if (output_buf_16 == NULL) {
        output_buf_16 = (short *)heap_caps_malloc(CODEC_OUTPUT_BUFSIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (output_buf_16 == NULL) {
            printf("failed to malloc buffer for codec write\n");
            ret = -1;
            goto EXIT;
        }
    }

    if (output_buf_32 == NULL) {
        output_buf_32 = (short *)heap_caps_malloc(CODEC_OUTPUT_BUFSIZE * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (output_buf_32 == NULL) {
            printf("failed to malloc buffer for codec write\n");
            ret = -1;
            goto EXIT;
        }
    }

EXIT:
    return ret;
}

int duer_audio_dac_unload_driver(void) {
    int ret = 0;

    if (output_buf_16 != NULL) {
        free(output_buf_16);
        output_buf_16 = NULL;
    }

    if (output_buf_32 != NULL) {
        free(output_buf_32);
        output_buf_32 = NULL;
    }

    ret = codec_uninit();
    if (ret != 0) {
        printf("Failed to unload codec driver\n");
    }

    s_codec_inited = 0;

    return ret;
}

//static int g_spil_out_vol = 0;
#define VOL_STEP_NUM_MAX		21
static uint16_t g_vol_tbl[VOL_STEP_NUM_MAX] =
{
    0xff, 0x9f, 0x8f, 0x7f, 0x6f, 0x5f, 0x5c, 0x5a,
    0x58, 0x54, 0x50, 0x4c, 0x4a, 0x38, 0x37, 0x36,
    0x35, 0x34, 0x33, 0x32, 0x31
};


int duer_audio_dac_get_volume(int *vol) {
    int ret = 0;
    int reg_vol = 0;
    uint8_t u8_reg_vol = 0;
    int i = 0;
    
    //ret = tas5805mGetVolume(&reg_vol);
    ret = ES8156_codec_get_voice_volume(&reg_vol);
    if (0 == ret)
    {
        for (i = 0; i < VOL_STEP_NUM_MAX; i++)
        {
            if (reg_vol >= g_vol_tbl[i])
                break;
        }

        printf("%s-vol_tbl_ind:%d\n", __func__, i);
        if (i < VOL_STEP_NUM_MAX)
            *vol = reg_vol;
        else
            *vol = VOL_STEP_NUM_MAX - 1;
    }
    return ret;
}

int duer_audio_dac_set_volume(int vol) {
    int ret = 0;
    int volume = 0;

    volume = vol / 5;		//app has 100 steps , wo change to 20 steps
    if (volume >= VOL_STEP_NUM_MAX)
        volume = VOL_STEP_NUM_MAX - 1;
    if (volume < 0)
        volume = 0;

    vol = volume * 5;

    //ret = tas5805mSetVolume(g_vol_tbl[volume]);
    ret = ES8156_codec_set_voice_volume(g_vol_tbl[volume]);
    return 0;
}

int duer_audio_dac_adj_volume(int adj_vol) {
    int ret = 0;
    printf("%s do nothing!!!\n", __func__);
    return ret;
}

int duer_audio_adc_set_pwr(int enable) {
    return 0;
}
