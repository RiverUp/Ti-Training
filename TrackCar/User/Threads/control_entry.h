#ifndef __CONTROL_ENTRY_H
#define __CONTROL_ENTRY_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"
#include "Motor.h"
#include "Encoder.h"


void control_entry();
void control();
int veloity_left();
int veloity_right();
int turn();

#endif