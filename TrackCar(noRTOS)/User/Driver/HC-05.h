#ifndef __HC_05_H
#define __HC_05_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"

extern rt_sem_t ProcessBtData;
extern char btdata[20];

void init_hc05(void);
void sendMsgByBlueTooth(char *msg);
void handleMsgFromBlueTooth(void);
bool connectDevice();

#endif