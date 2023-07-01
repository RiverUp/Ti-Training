#ifndef __CONTROL_H
#define __CONTROL_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Motor.h"
#include "Encoder.h"
#include "TCRT5000.h"
#include "oled.h"
#include "HC-SR04.h"
#include "Delay.h"

extern bool OverTakeFlag;
extern bool ReadyOverTakeFlag;
extern int OverTakeCount;
extern int turnPwm;
extern float adc;
extern bool t1,t2,t3;
extern int Mission;
extern bool StopFlag;
extern bool CrossFlag;
extern bool FollowFlag;
extern int CrossNums;
extern int MaxTurn1;
extern int MaxTurn2;
extern int ChangeIntervalTurn1;   
extern int ChangeIntervalTurn2;

extern int StraightV;
extern int TurnV1;
extern int TurnV2;
extern float Velocity_Kp,Velocity_Ki;

extern int CrossAccelerateTimes,DecelerationTimes;

extern float target_encoder_value;

void Control();
int velocity_left(int left);
int velocity_right(int right);
int turn();
void init_control();
int velocity(int current_encoder_left,int current_encoder_right);
int distance(int current_dis);
int turn2();

#endif