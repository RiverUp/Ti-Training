/******************************************************************************
//MSP432P401R
//GPIO输入 板载按键
*******************************************************************************/

#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define KEY1 BITBAND_PERI(P1IN, 1) 		//读取按键1
#define KEY2 BITBAND_PERI(P1IN, 4) 		//读取按键2
#define KEY1_PRES 1 					//KEY1按下
#define KEY2_PRES 2 					//KEY2按下


void KEY_Init(void);       				//IO初始化
uint8_t KEY_Scan(uint8_t); 				//按键扫描函数

#endif
