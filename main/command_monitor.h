
#ifndef BAIDU_TOOL_MONITOR_CLI_H
#define BAIDU_TOOL_MONITOR_CLI_H

#ifdef __cplusplus
extern "C" {
#endif
#define USER_DATA0_PARTITION     ( 0x25 )
#define USER_DATA1_PARTITION     ( 0x26 )
#define PROFILE_PARTITION        ( 0x29 )

#define MAX_DEVICE_SN_LENGTH 31

#define CMD_PRE             "prod_test "
#define RESULT_PRE          CMD_PRE

#define CMD_TASK            CMD_PRE"task"
#define CMD_PA_MIC          CMD_PRE"pa_mic"
#define CMD_FLASH           CMD_PRE"flash"
#define CMD_WIFI            CMD_PRE"wifi"
#define CMD_DSP             CMD_PRE"dsp"
#define CMD_NOLINEAR        CMD_PRE"nolinear"
#define CMD_KEY             CMD_PRE"key"
#define CMD_IR              CMD_PRE"ir"
#define CMD_IRX             CMD_PRE"irx"
#define CMD_GET_SN          CMD_PRE"get_sn"
#define CMD_SET_SN          CMD_PRE"set_sn"
#define CMD_BT              CMD_PRE"bt"
#define CMD_PWR             CMD_PRE"power"
#define CMD_RST             CMD_PRE"reset"
#define CMD_OFF             CMD_PRE"off"
#define CMD_FAIL            CMD_PRE"fail"
#define CMD_INTR            CMD_PRE"intr"
#define CMD_QUIT1           CMD_PRE"quit1"
#define CMD_QUIT2           CMD_PRE"quit2"
#define CMD_LED             CMD_PRE"led "      //don't remove space
#define CMD_LEDX            CMD_PRE"ledx "     //don't remove space
#define CMD_PROFILE         CMD_PRE"profile"
#define RD_PROFIEL          CMD_PRE"rdprof"
#define CMD_VERSION         CMD_PRE"version"
#define CMD_SEAL            CMD_PRE"mic_seal"

//test for IR material. we use these in special case!!	
#define CMD_IR_RX           "ir_rx"
#define CMD_IR_TX           "ir_tx"
#define CMD_IR_TX_SIMPLE    "rt"
#define CMD_IR_RX_EXIT      "ir_rx_ex"
#define CMD_IR_TX_EXIT      "ir_tx_ex"
#define CMD_IR_RX_EXIT_2    "rx_ex"
#define CMD_IR_TX_EXIT_2    "tx_ex"

// #define RESULT_PA           RESULT_PRE"pa "
// #define RESULT_MIC          RESULT_PRE"mic "
#define RESULT_PA_MIC_DATA  RESULT_PRE"pa_mic "

#define RESULT_TASK         RESULT_PRE"task "
#define RESULT_FLASH        RESULT_PRE"flash "
#define RESULT_WIFI         RESULT_PRE"wifi "
#define RESULT_DSP          RESULT_PRE"dsp "
#define RESULT_NOLINEAR     RESULT_PRE"nolinear "
#define RESULT_KEY_UP       RESULT_PRE"key V+ "
#define RESULT_KEY_DOWN     RESULT_PRE"key V- "
#define RESULT_KEY_MUTE     RESULT_PRE"key Mute "
#define RESULT_KEY_PLAY     RESULT_PRE"key Play "
#define RESULT_IO_HEART     RESULT_PRE"key Heart "
#define RESULT_IR           RESULT_PRE"ir"

#define RESULT_GET_SN       RESULT_PRE"get_sn"
#define RESULT_SET_SN       RESULT_PRE"set_sn"
#define RESULT_USB_DET      RESULT_PRE"power"
#define RESULT_USB_DIS      RESULT_PRE"power"
#define RESULT_PWR_LEVEL	RESULT_PRE"power"
#define RESULT_RST_LEVEL    RESULT_PRE"reset"
#define RESULT_OFF_LEVEL    RESULT_PRE"off"
#define RESULT_FAIL_LEVEL   RESULT_PRE"fail"
#define RESULT_INTR_LEVEL   RESULT_PRE"intr"
#define RESULT_LED_LEVEL    RESULT_PRE"led"
//#define RESULT_LEDX_LEVEL   RESULT_PRE"ledx"
#define RESULT_QUIT1_LEVEL  RESULT_PRE"quit1"
#define RESULT_QUIT2_LEVEL  RESULT_PRE"quit2"
#define RESULT_PROFILE_LEVEL RESULT_PRE"profile"
#define RESULT_VERSION_LEVEL RESULT_PRE"version"
#define RESULT_SEAL         RESULT_PRE"mic_seal "

#define CASCADE(module, subject) (CMD_PRE module subject)

#define _NEW_TEST_    1

int monitor_init();

#ifdef __cplusplus
}
#endif

#endif // BAIDU_TOOL_MONITOR_CLI_H
