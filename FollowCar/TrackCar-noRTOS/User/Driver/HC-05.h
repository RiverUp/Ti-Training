#ifndef __HC_05_H
#define __HC_05_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"

extern char btdata[20];
extern uint8_t dataPtr;
extern bool BTRecCompleteFlag;

void init_hc05(void);
void sendMsgByBlueTooth(char *msg);
void handleMsgFromBlueTooth(void);
bool connectDevice();

#endif