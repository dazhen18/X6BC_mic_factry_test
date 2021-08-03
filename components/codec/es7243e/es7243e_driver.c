/*************************************************************************
 *
 * Copyright (c) 2019 Baidu.com, Inc. All Rights Reserved
 *
 *************************************************************************/
/**
 * @file: es7243e_driver.c
 * @author: changli@baidu.com
 * @brief: ADC driver for analogic mic input.
 **/

//#include "es7243e_driver.h"
#include <string.h>
#include "esp_log.h"
#include "i2c_bus.h"
#include "es7243e_driver.h"

#define ES7243E_TAG  "ES7243E_ADC"

#define IIC_CLK     23
#define IIC_DATA    18

static i2c_config_t s_i2c_cfg = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = IIC_DATA,
    .scl_io_num = IIC_CLK,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000
};

static int write_reg(uint8_t reg_addr,uint8_t reg_val)
{
    int ret = 0;
    ret |= i2c_bus_write_bytes(0, ES7243_ADDR, &reg_addr, 1, &reg_val, 1);
    if (ret != 0) {
        ESP_LOGE(ES7243E_TAG, "write reg error!");
    }
    return ret;
}

static int es7243e_read(uint8_t regAdd, uint8_t *pData, int len)
{
    //uint8_t data;
    int res = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    res |= i2c_master_start(cmd);
    res |= i2c_master_write_byte(cmd, ES7243_ADDR, 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_write_byte(cmd, regAdd, 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_stop(cmd);
    res |= i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    res |= i2c_master_start(cmd);
    res |= i2c_master_write_byte(cmd, ES7243_ADDR | 0x01, 1 /*ACK_CHECK_EN*/);
    int i = 0;
    for (i = 0; i < len - 1; i++) {
        res |= i2c_master_read_byte(cmd, &pData[i], 0);
    }
    res |= i2c_master_read_byte(cmd, &pData[i], 0x01/*NACK_VAL*/);
    res |= i2c_master_stop(cmd);
    res |= i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return res;
}

int es7243e_read_reg(uint8_t reg_addr)
{
    uint8_t read_value[2] = {0,0};

    es7243e_read(reg_addr, (uint8_t*)&read_value, 1);
    return read_value[0];
}

int es7243e_write_reg(uint8_t reg_addr,uint8_t reg_val)
{
    return write_reg(reg_addr,reg_val);

}

int es7243e_init(void)
{
    i2c_bus_handle_t i2c_handler = i2c_bus_create(0, &s_i2c_cfg);
    if (i2c_handler == NULL) {
        ESP_LOGE(ES7243E_TAG, "failed to create i2c bus handler\n");
        return -1;
    }

    ESP_LOGI(ES7243E_TAG, "ES7243E driver init start...\n");
    int ret = 0;
    /*
    ret |= write_reg(0x00, 0x1e); //reset, slave mode
    ret |= write_reg(0x0b, 0xcc); //mute, i2s-16bit
    ret |= write_reg(0x16, 0xf0); //enable PGA1 PGA2
    ret |= write_reg(0x20, 0x11); //PGA1=3db
    ret |= write_reg(0x21, 0x11); //PGA2=3db
    ret |= write_reg(0x0b, 0x0c); //unmute, i2s-16bit*/

    ret |= write_reg(0xf9, 0x00);
    ret |= write_reg(0x02, 0x80);
    ret |= write_reg(0x03, 0x20);
    ret |= write_reg(0x0d, 0x00);
    ret |= write_reg(0x04, 0x03);
    ret |= write_reg(0x05, 0x00);
    ret |= write_reg(0x09, 0xe0);
    ret |= write_reg(0x0a, 0xa0);
    ret |= write_reg(0x0b, 0x0c);
    ret |= write_reg(0x0e, 0xbf);
    ret |= write_reg(0x0f, 0x80);
    ret |= write_reg(0x14, 0x0c);
    ret |= write_reg(0x15, 0x0c);

    ret |= write_reg(0x18, 0x26);
    ret |= write_reg(0x17, 0x02);
    ret |= write_reg(0x19, 0x77);
    ret |= write_reg(0x1a, 0xf4);
    ret |= write_reg(0x1b, 0x66);
    ret |= write_reg(0x1c, 0x44);
    ret |= write_reg(0x1e, 0x00);
    ret |= write_reg(0x1f, 0x0c);

    ret |= write_reg(0x20, 0x18);
    ret |= write_reg(0x21, 0x18);
    ret |= write_reg(0x00, 0x80);
    ret |= write_reg(0x01, 0x3a);
    ret |= write_reg(0x16, 0x3f);
    ret |= write_reg(0x16, 0x00);

    vTaskDelay(30 / portTICK_PERIOD_MS);
    ret |= write_reg(0x16, 0x03);
    vTaskDelay(30 / portTICK_PERIOD_MS);
    ret |= write_reg(0x16, 0x00);

    if (ret == 0) {
        ESP_LOGI(ES7243E_TAG, "Initializing es7243e is successful.");
    } else {
        ESP_LOGE(ES7243E_TAG, "Initializing es7243e is failed.");
    }

    ESP_LOGI(ES7243E_TAG, "ES7243E driver init end...\n");
    return ret;
}
