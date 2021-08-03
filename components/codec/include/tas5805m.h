#ifndef __TAS5805MP_H__
#define __TAS5805MP_H__

#include "driver/i2c.h"
/* TAS5805M address */
#define TAS5805M_ADDR     0x5c

int tas5805mInit(void);

int tas5805mSetVolume(int value);
int tas5805mGetVolume(int *value);

#endif //__TAS5805MP_H__
