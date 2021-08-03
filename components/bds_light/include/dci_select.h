/***************************************************************************
*
* Copyright (c) 2019 Baidu.com, Inc. All Rights Reserved
*
**************************************************************************/
/**
* @file dci_select.h
* @author luoyou01(com@baidu.com)
* @date 2019/07/09
**/

#ifndef  _DCI_SELECT_H_
#define  _DCI_SELECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PLATFORM_ESP32 
#define CALIBRATED_DEV_NUM 9
#define CALIBRATED_PAIR_NUM (CALIBRATED_DEV_NUM-1) * CALIBRATED_DEV_NUM / 2
#define MAX_SUPPORT_DEV_NUM 64
#define MAX_SUPPORT_PAIR_NUM (MAX_SUPPORT_DEV_NUM - 1) * MAX_SUPPORT_DEV_NUM / 2

enum DCI_STATE {
    DCI_STATE_INIT = 1,
    DCI_STATE_RUNNING = 2,
    DCI_STATE_UNINIT = 3,
    DCI_STATE_READ_CONFIG_FILE = 4,
    DCI_STATE_AVAILABLE = 5
};

typedef struct Config_Param {
    short update_id1;            // 设备1的id （小）
    short update_id2;            // 设备2的id （大）
    short band_use_id[2];        // 频带id
    short calibration_value;   // 校准值
} Config_Param;

typedef struct objDciJudge {
    Config_Param config_list[MAX_SUPPORT_PAIR_NUM];
    unsigned short config_file_version;
    short max_dev_num;
    bool log_print_on;
    volatile short state;
} objDciJudge;

objDciJudge* bd_dci_init_api(const char* ptr_config_file);

int bd_dci_uninit_api(objDciJudge* ptr);

#ifdef PLATFORM_ESP32
int bd_dci_read_config_file_api(objDciJudge* srv, const char* file_buffer, int file_size);
#else
/*  Parameters:
    ptr_config_file : 配置文件路径，包括文件名
*/
int bd_dci_read_config_file_api(objDciJudge* srv, const char* ptr_config_file);
#endif

/*  Parameters:
    config_version : 返回本地配置文件版本号
    state ： 返回决策库运行状态 （ 参照枚举 DCI_STATE ， 若为 DCI_STATE_READ_CONFIG_FILE 状态， 则不可下载配置文件或运行select）
*/
int bd_dci_get_config_version_and_state(objDciJudge* srv, unsigned short* config_version,
                                        short* state);

int bd_dci_get_log(float* dci_feature[], int device_num, float* dci_log[]);

// @brief dci select Process
/*  Parameters:
    float **dci_feature: Two-dimensional array, the size of dci_feature is device_num * feature_num
                         dci_feature[n] represents the dci feature of n-th device, length is feature_num
    int device_num: number of devices
    int feature_num: length of dci feature
    int *dci_credibility: dci select result, length is device_num, credibility of the devices, 0~100
*/
/*  Parameters:
    float **dci_feature
        输入DCI唤醒词特征信息，二维float数组，维度为 device_num * feature_num
        dci_feature[n][m]代表第n个设备的第m个特征
    int device_num
        设备数量
    int feature_num
        DCI特征长度，最大300，现阶段默认为300
    int *dci_credibility
        DCI返回结果，dci_credibility[n]分别表示第n个设备的DCI得分，0~100，分数越高代表设备距离说话人越近
    int *debug_info
        DCI返回debug信息，用于调试
    return
        最优的设备号，0 ~ device_num-1
*/
int bd_dci_select_new_api(objDciJudge* ptr, float* dci_feature[], char* dci_dev_id_string[],
                          int device_num, int feature_num, int* dci_credibility, int* debug_info);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //_DCI_SELECT_H_

