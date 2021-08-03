#include "esp_system.h"
#include "tas5805m.h"
#include "i2c_bus.h"
#include "userconfig.h"
#include "esp_heap_caps.h"
#include "tas5805_agc_drc3_eq0.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define		PA_DEFAULT_CONFIG_FILE			"/sdcard/tas5805m.cfg"
#define		PA_CMD_MAX_LEN		255

#define		STATUS_NOTHING		0
#define		STATUS_W_PREPARING	1

static int get_val_from_string(char *p_str, int len, unsigned char *p_val, int val_len)
{
    int i = 0;
    int got_val_cnt = 0;
    int cur_val = 0;
    for (;;) {
        for (; i < len ; i++) {
            if (' ' == *(p_str+i)) 
            {
                i++;
                break;
            }
        }

        if (i >= len) 
        {
            return got_val_cnt;
        }
        cur_val = 0;
        for (; i < len; i++) 
        {
            char cur_ch = *(p_str + i);
            //printf("==%c -cur_val:%x", cur_ch, cur_val);

            if (cur_ch >= 'a' && cur_ch <= 'f') 
            {
                cur_val *= 16;
                cur_val += cur_ch - 'a' + 10;
            } 
            else if (cur_ch >= '0' && cur_ch <= '9') 
            {
                cur_val *= 16;
                cur_val += cur_ch - '0';
            } 
            else
            {
                break;
            }
        }
        //printf("cur_val:0x%x\n", cur_val);
        if (got_val_cnt < val_len) 
        {
            *(p_val + got_val_cnt) =  cur_val;
            got_val_cnt++;
            if (got_val_cnt == val_len)
            {
                return got_val_cnt;
            }
        } 
        else 
        {
            //printf("got_val_cnt:%d\n", got_val_cnt);
            return got_val_cnt;
        }
    }
}

void tas5805m_pa_set_config_from_file(char *p_config_file)
{
    FILE *fp_config = NULL;
    char *p_pa_cmd = NULL;
    size_t read_len = 0;
    unsigned char *p_cmd_data = NULL;
    int cmd_data_len = 0;
    //int i = 0;
    int status = STATUS_NOTHING;
    if (NULL == p_config_file)
    {
        p_config_file = PA_DEFAULT_CONFIG_FILE;
    }

    fp_config = fopen(p_config_file, "rb");
    if (NULL == fp_config) 
    {
        printf("read pa config:%s failed", p_config_file);
        return ;
    }
#if 0
    //p_pa_cmd = (char *)heap_caps_malloc(PA_CMD_MAX_LEN, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    p_pa_cmd = (char *)heap_caps_malloc(PA_CMD_MAX_LEN, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    if(NULL == p_pa_cmd)
    {
        printf("%s--OOM\n", __func__);
        fclose(fp_config);
        return;
    }
#endif
    //p_cmd_data = (unsigned char*)malloc(PA_CMD_MAX_LEN);
    //p_cmd_data = (unsigned char *)heap_caps_malloc(PA_CMD_MAX_LEN, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    p_cmd_data = (unsigned char *)heap_caps_malloc(PA_CMD_MAX_LEN, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    if (NULL == p_cmd_data) 
    {
        printf("%s--OOM22\n", __func__);
        fclose(fp_config);
        return;
    }
    int n_first_cmd = 0;
    while (1) {
        //char start_ch = 0;
        int val1 = 0;
        int val2 = 0;
        int val3 = 0;
        //memset(p_pa_cmd, 0, PA_CMD_MAX_LEN);
        p_pa_cmd = NULL;
        int read_result = __getline(&p_pa_cmd, &read_len, fp_config);
        if (read_result < 0 || NULL == p_pa_cmd) 
        {
            printf("read failed or end\n");
            break;
        }
        //printf("read: %s\n", p_pa_cmd);
        if ('w' == *p_pa_cmd) 
        {
            if (STATUS_W_PREPARING == status) 
            {
                //printf("need w rite latest data firstly!!!!data_len:%d\n", cmd_data_len);
                //here call i2c write.
                if (cmd_data_len >= 3) 
                {
                    i2c_bus_write_bytes(0, TAS5805M_ADDR, p_cmd_data + 1, 1, p_cmd_data + 2, cmd_data_len - 2);
                    if (0 == n_first_cmd) 
                    {
                        n_first_cmd = 1;
                        vTaskDelay(50 / portTICK_RATE_MS);
                        printf("first cmd, so delay 50ms\n");
                    }
                }

                //sscanf(p_pa_cmd, "%c %x %x %x", &start_ch, &val1, &val2, &val3);
                //*p_pa_cmd = val1;
                //*(p_pa_cmd+1) = val2;
                //*(p_pa_cmd + 2) = val3;
                get_val_from_string(p_pa_cmd, read_len, p_cmd_data, 3);
                //printf("read11 data:0x%x 0x%x 0x%x\n", *p_cmd_data, *(p_cmd_data + 1), *(p_cmd_data + 2));
            } 
            else 
            {
                //sscanf(p_pa_cmd, "%c %x %x %x", &start_ch, &val1, &val2, &val3);
                //printf("read12 data:0x%x 0x%x 0x%x\n", val1, val2, val3);
                /*
                *p_pa_cmd = val1;
                *(p_pa_cmd+1) = val2;
                *(p_pa_cmd + 2) = val3;
                */
                get_val_from_string(p_pa_cmd, read_len, p_cmd_data, 3);
                //printf("read12 data:0x%x 0x%x 0x%x\n", *p_cmd_data, *(p_cmd_data + 1), *(p_cmd_data + 2));
            }
            status = STATUS_W_PREPARING;
            cmd_data_len = 3;
        } 
        else if ('>' == *p_pa_cmd) 
        {
            if (STATUS_W_PREPARING == status) 
            {
                /*
                sscanf(p_pa_cmd, "%c %x", &start_ch, &val1);
                printf(">read data:0x%x\n", val1);
                *(p_pa_cmd + cmd_data_len) = val1;
                */
                get_val_from_string(p_pa_cmd, read_len, p_cmd_data + cmd_data_len, 1);
                //printf("read data:0x%x \n", *(p_cmd_data + cmd_data_len));
                cmd_data_len++;
            } 
            else 
            {
                printf("file format error\n");
            }
        } 
        else {

            if (STATUS_W_PREPARING == status) 
            {
                //here call i2c write
                if (cmd_data_len >= 3)
                {
                    i2c_bus_write_bytes(0, TAS5805M_ADDR, p_cmd_data + 1, 1, p_cmd_data + 2, cmd_data_len - 2);
                }

                status = STATUS_NOTHING;
                cmd_data_len = 0;
            }
        }
        if (p_pa_cmd) 
        {
            free(p_pa_cmd);
            p_pa_cmd = NULL;
        }
    }
    printf("finish write pa config\n");
    if (fp_config)
    {
        fclose(fp_config);
    }
    if (p_pa_cmd)
    {
        free(p_pa_cmd);
    }
    if (p_cmd_data)
    {
        free(p_cmd_data);
    }

    printf("finish to free\n");
}

void tas5805m_pa_set_config_from_code(char **p_config, size_t cnt)
{
    char *p_pa_cmd = NULL;
    size_t read_len = 0;
    unsigned char *p_cmd_data = NULL;
    int cmd_data_len = 0;
    //int i = 0;
    int j = 0;
    int status = STATUS_NOTHING;
    if (NULL == p_config) 
    {
        printf("invalid argument p_config!\n");
        return;
    }

    p_cmd_data = (unsigned char *)heap_caps_malloc(PA_CMD_MAX_LEN, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    if (NULL == p_cmd_data) 
    {
        printf("%s: alloc failed!\n", __func__);
        return;
    }
    int n_first_cmd = 0;
    while (j < cnt) 
    {
        //char start_ch = 0;
        int val2 = 0;
        int val3 = 0;
        p_pa_cmd = NULL;
        p_pa_cmd = p_config[j++];
        //printf("cnt=%d, j=%d, cmd=%s.\n", cnt, j, p_pa_cmd);
        read_len = strlen(p_pa_cmd);

        if ('w' == *p_pa_cmd) 
        {
            if (STATUS_W_PREPARING == status) 
            {
                if (cmd_data_len >= 3) 
                {
                    i2c_bus_write_bytes(0, TAS5805M_ADDR, p_cmd_data + 1, 1, p_cmd_data + 2, cmd_data_len - 2);
                    if (0 == n_first_cmd) 
                    {
                        n_first_cmd = 1;
                        vTaskDelay(50 / portTICK_RATE_MS);
                        printf("first cmd, so delay 50ms\n");
                    }
                }
            }

            get_val_from_string(p_pa_cmd, read_len, p_cmd_data, 3);
            status = STATUS_W_PREPARING;
            cmd_data_len = 3;
        } 
        else if ('>' == *p_pa_cmd) 
        {
            if (STATUS_W_PREPARING == status) 
            {
                get_val_from_string(p_pa_cmd, read_len, p_cmd_data + cmd_data_len, 1);
                cmd_data_len++;
            } 
            else 
            {
                printf("file format error\n");
            }
        } 
        else 
        {
            if (STATUS_W_PREPARING == status) 
            {
                if (cmd_data_len >= 3)
                {
                    i2c_bus_write_bytes(0, TAS5805M_ADDR, p_cmd_data + 1, 1, p_cmd_data + 2, cmd_data_len - 2);
                }

                status = STATUS_NOTHING;
                cmd_data_len = 0;
            }
        }
    }

    printf("finish write pa config\n");
    if (p_cmd_data) 
    {
        free(p_cmd_data);
    }

    printf("finish to free\n");
}

void transmit_registers(tas5805m_cfg_reg *r, int n)
{
    int i = 0;
    while (i < n) 
    {
        switch (r[i].offset) 
        {
        case CFG_META_SWITCH:
            // Used in legacy applications.  Ignored here.
            break;
        case CFG_META_DELAY:
            vTaskDelay(r[i].value / portTICK_RATE_MS);
            break;
        case CFG_META_BURST:
            i2c_bus_write_bytes(0, TAS5805M_ADDR, (unsigned char *)(&r[i+1].offset), 1,
                                (unsigned char *)(&r[i+1].value), r[i].value);
            //i2c_write((unsigned char *)&r[i+1], r[i].param);
            i +=  (r[i].value / 2) + 1;
            break;
        case CFG_END_1:
            if(CFG_END_2 == r[i+1].offset && CFG_END_3 == r[i+2].offset)
            {
                printf("end of tms5805m reg---%d\n", i);
                break;
            }
        default:
            i2c_bus_write_bytes(0, TAS5805M_ADDR, (unsigned char *)(&r[i].offset), 1,
                                (unsigned char *)(&r[i].value), 1);
            //i2c_write((unsigned char *)&r[i], 2);
            break;
        }
        i++;
    }
    printf("%s------all write %d reg\n", __func__, i);
}

void tas5805m_pa_set_config_from_code_2()
{
    transmit_registers(tas5805m_registers, sizeof(tas5805m_registers) / sizeof(tas5805m_registers[0]));
}
