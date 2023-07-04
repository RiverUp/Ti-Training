#ifndef __SERIAL_H
#define __SERIAL_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void initSerial(void);
void sendText(char *string);

#endif
