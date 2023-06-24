#ifndef __CONTROL_H
#define __CONTROL_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Motor.h"
#include "Encoder.h"
#include "TCRT5000.h"
#include "oled.h"

extern int turnPwm;
extern float adc;
extern bool t1,t2,t3;

void Control();
int velocity_left(int left);
int velocity_right(int right);
int turn();
void init_control();
int velocity(int current_encoder_left,int current_encoder_right);


#endif