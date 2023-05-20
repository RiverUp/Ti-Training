#ifndef __TCRT5000_h
#define __TCRT5000_h 

#include <stdint.h>
#include <stdbool.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define BITS  16384          //2^13
#define THRESHOLD1 1
#define THRESHOLD2 2

void init_TCRT();
float read_TCRT();

#endif // !__TCRT5000.h