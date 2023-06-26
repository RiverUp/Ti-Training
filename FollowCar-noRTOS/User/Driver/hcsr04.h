#ifndef _HCSR04_H
#define _HCSR04_H
#include "stdint.h"     
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Delay.h"

void HCSR04_Init(void);				//初始化函数
float Distance(void);				//测距函数
extern unsigned char ctl_ctl; 		//延时变量
extern unsigned char ctl; 			//延时变量
extern uint8_t Flag_Target;			//延时变量
#endif
