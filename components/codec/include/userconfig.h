#ifndef ESP32_USER_CONFIG_H
#define ESP32_USER_CONFIG_H

#include "sdkconfig.h"

//I2C
#define IIC_CLK                 (23)
#define IIC_DATA                (18)

//I2S0
#if CONFIG_CUPID_BOARD_V2
#define IIS_SCLK                (5)
#define IIS_DSIN                (26)
#define IIS_LCLK                (25)
#define IIS_DSOUT               (-1)
#else
#define IIS_SCLK                (4)        //5(cupid)  -> 4(tcl)
#define IIS_DSIN                (16)       //26(cupid) -> 16(tcl)
#define IIS_LCLK                (13)       //25(cupid) ->13(tcl)
#define IIS_DSOUT               (39)       //-1(cupid) ->39(tcl)
#endif

//I2S1
#define IIS1_SCLK               (33)
#define IIS1_DSIN               (-1)
#define IIS1_LCLK               (32)
#define IIS1_DSOUT              (35)

//DSP
#if CONFIG_CUPID_BOARD_V2
#define GPIO_DSP_IRQ            GPIO_NUM_35
#define GPIO_DSP_IRQ_SEL        GPIO_SEL_35
#define GPIO_DSP_RST            GPIO_NUM_21
#define GPIO_DSP_RST_SEL        GPIO_SEL_21
#else
#define GPIO_DSP_IRQ            GPIO_NUM_38  //35(cupid) -> 38(tcl)
#define GPIO_DSP_IRQ_SEL        GPIO_SEL_38
#define GPIO_DSP_RST            GPIO_NUM_27  //21(cupid) -> 27(tcl)
#define GPIO_DSP_RST_SEL        GPIO_SEL_27
#endif

//POWER
#define GPIO_POWER_ONOFF        (GPIO_NUM_19)
#define GPIO_POWER_ONOFF_SEL    (GPIO_SEL_19)

//PA
#if CONFIG_CUPID_BOARD_V2
#define GPIO_PA_RST_SEL         GPIO_SEL_2
#define GPIO_PA_RST             GPIO_NUM_2
#else
#define GPIO_PA_RST_SEL         GPIO_SEL_12
#define GPIO_PA_RST             GPIO_NUM_12
#endif

#endif //ESP32_USER_CONFIG_H
