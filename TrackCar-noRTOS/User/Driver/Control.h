#ifndef __CONTROL_H
#define __CONTROL_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Motor.h"
#include "Encoder.h"
#include "TCRT5000.h"
#include "oled.h"
#include "HC-SR04.h"

extern int turnPwm;
extern float adc;
extern bool t1,t2,t3;
extern int mission;
extern int distancePwm;
extern int pwma, pwmb;
extern bool StopFlag;
extern bool CrossFlag;
extern int CrossNums;

void Control();
int velocity_left(int left);
int velocity_right(int right);
int turn();
void init_control();
int velocity(int current_encoder_left,int current_encoder_right);
int distance(int current_dist);

#endif