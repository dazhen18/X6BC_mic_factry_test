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
    short update_id1;            // �豸1��id ��С��
    short update_id2;            // �豸2��id ����
    short band_use_id[2];        // Ƶ��id
    short calibration_value;   // У׼ֵ
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
    ptr_config_file : �����ļ�·���������ļ���
*/
int bd_dci_read_config_file_api(objDciJudge* srv, const char* ptr_config_file);
#endif

/*  Parameters:
    config_version : ���ر��������ļ��汾��
    state �� ���ؾ��߿�����״̬ �� ����ö�� DCI_STATE �� ��Ϊ DCI_STATE_READ_CONFIG_FILE ״̬�� �򲻿����������ļ�������select��
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
        ����DCI���Ѵ�������Ϣ����άfloat���飬ά��Ϊ device_num * feature_num
        dci_feature[n][m]�����n���豸�ĵ�m������
    int device_num
        �豸����
    int feature_num
        DCI�������ȣ����300���ֽ׶�Ĭ��Ϊ300
    int *dci_credibility
        DCI���ؽ����dci_credibility[n]�ֱ��ʾ��n���豸��DCI�÷֣�0~100������Խ�ߴ����豸����˵����Խ��
    int *debug_info
        DCI����debug��Ϣ�����ڵ���
    return
        ���ŵ��豸�ţ�0 ~ device_num-1
*/
int bd_dci_select_new_api(objDciJudge* ptr, float* dci_feature[], char* dci_dev_id_string[],
                          int device_num, int feature_num, int* dci_credibility, int* debug_info);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //_DCI_SELECT_H_

