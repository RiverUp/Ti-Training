#ifndef __SERIAL_H
#define __SERIAL_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void initSerial();
void sendText(uint32_t moduleInstance,char *string);

#endif