/*
*
* Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

/*  ---------------------------------------------------------------------------------------
*   |                                                                                       |
*   |   The file includes functions and variables to configure tas5805mP.                    |
*   |                                                                                       |
*   ----------------------------------------------------------------------------------------
*/
#include <string.h>
#include "esp_log.h"
#include "tas5805m.h"
#include "i2c_bus.h"
#include "userconfig.h"

//#define tas5805m_CFG_FROM_FILE
#ifdef tas5805m_CFG_FROM_FILE
extern void tas5805m_pa_set_config_from_file(char *p_config_file);
#else
//#include "TAS5805M_DefaultReg.h"
extern void tas5805m_pa_set_config_from_code(char **p_config, size_t cnt);
extern void tas5805m_pa_set_config_from_code_2();
#endif

#define tas5805m_TAG "TAS5805M"

#define tas5805m_ASSERT(a, format, b, ...) \
    if ((a) != 0) { \
        ESP_LOGE(tas5805m_TAG, format, ##__VA_ARGS__); \
        return b;\
    }

static i2c_config_t i2c_cfg = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = IIC_DATA,
    .scl_io_num = IIC_CLK,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000
};
#define  MUTE_REG_ADDR			0X03
#define  MASTER_VOL_REG_ADDR	0X4C
#define  MUTE_TIME_REG_ADDR		0X51
static unsigned char NpcVolume[2] = {0x4c, 0x00};
#if 1
static int tas5805mReadReg_2(uint8_t regAdd, uint8_t *pData, int len)
{
    //uint8_t data;
    int res = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    res |= i2c_master_start(cmd);
    res |= i2c_master_write_byte(cmd, TAS5805M_ADDR, 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_write_byte(cmd, regAdd, 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_stop(cmd);
    res |= i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    res |= i2c_master_start(cmd);
    res |= i2c_master_write_byte(cmd, TAS5805M_ADDR | 0x01, 1 /*ACK_CHECK_EN*/);
    int i = 0;
    for (i = 0; i < len - 1; i++) {
        res |= i2c_master_read_byte(cmd, &pData[i], 0);
    }
    res |= i2c_master_read_byte(cmd, &pData[i], 0x01/*NACK_VAL*/);
    res |= i2c_master_stop(cmd);
    res |= i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    tas5805m_ASSERT(res, "TAS5805MReadReg_2 error", -1);
    return res;
}
#endif
static int tas5805mWriteReg(uint8_t *data)
{
    int res = 0;
    res |= i2c_bus_write_bytes(0, TAS5805M_ADDR, &data[0], 1, &data[1], 1);
    tas5805m_ASSERT(res, "TAS5805MWriteReg error", -1);

    return res;
}

static i2c_bus_handle_t i2c_handler;
int tas5805mInit(void)
{
    int res = 0;
#if 1
    ESP_LOGI(tas5805m_TAG, "tas5805mInit\n");
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = GPIO_PA_RST_SEL;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);
    gpio_set_level(GPIO_PA_RST, 0);
    vTaskDelay(10 / portTICK_RATE_MS);
    gpio_set_level(GPIO_PA_RST, 1);
    vTaskDelay(20 / portTICK_RATE_MS);
#endif
    vTaskDelay(100 / portTICK_RATE_MS);
    i2c_handler = i2c_bus_create(0, &i2c_cfg);
    if (i2c_handler == NULL) {
        ESP_LOGW(tas5805m_TAG, "failed to create i2c bus handler\n");
        return -1;
    }

#ifdef tas5805m_CFG_FROM_FILE
    tas5805m_pa_set_config_from_file(NULL);
#else
    //tas5805m_pa_set_config_from_code(TAS5733_DEFAULT_CFG, sizeof(TAS5733_DEFAULT_CFG) / sizeof(char *));
    tas5805m_pa_set_config_from_code_2();
#endif
    ESP_LOGI(tas5805m_TAG, "tas5805mInit end\n");
    return res;
}

//- If data is 000h (a mute command):
//    - 1 msec before Setup Complete
//    - 120 msec if this command is sent after Setup Complete
//- If data is not 000h (any other value): a 1 msec interval should be used
// 0 ~ 499
int tas5805mSetVolume(int  value)
{
    int res = 0;

    NpcVolume[1] = (unsigned char )value;
    ESP_LOGI(tas5805m_TAG, "SetVolume--[%02x %02x], %d", NpcVolume[0], NpcVolume[1], value);
    res |= tas5805mWriteReg(NpcVolume);

    uint8_t read_vol[2];
    tas5805mReadReg_2(0x03, (uint8_t*)&read_vol, 1);
    //printf("READ 03: 0X%x\n", read_vol[0]);
    tas5805mReadReg_2(0x4c, (uint8_t*)&read_vol, 1);
    //printf("READ 4C: 0X%x\n", read_vol[0]);
    tas5805mReadReg_2(0x4e, (uint8_t*)&read_vol, 1);
    //printf("READ 4e: 0X%x\n", read_vol[0]);
    tas5805mReadReg_2(0x4f, (uint8_t*)&read_vol, 1);
    //printf("READ 4f: 0X%x\n", read_vol[0]);
    return res;
}

int tas5805mGetVolume(int *value)
{
    uint8_t read_vol[2];
    tas5805mReadReg_2(0x4c, (uint8_t*)&read_vol, 1);

    ESP_LOGI(tas5805m_TAG, "read_vol:0x%x", read_vol[0]);
    *value = read_vol[0];
    return 0;
}
