#pragma once

#include <stdint.h>

// PD4  ---> AIN1
// PC12 ---> AIN2
// PD11 ---> BIN1
// PD10 ---> BIN2
// PE10 ---> STBY
// PD12 ---> E2A  T4C1
// PD13 ---> E2B  T4C2
// PC6  ---> E1A  T3C1
// PC7  ---> E1B  T3C2
// PE14 ---> PWMA T1C4
// PE13 ---> PWMB T1C3
// PC2  ---> ADC  ADC123IN12 

#define PWMChannelsCount 2
#define BIN2   PDout(10)
#define BIN1   PDout(11)
#define AIN2   PCout(12)
#define AIN1   PDout(4)
#define PWMA   TIM1->CCR4  //PE14
#define PWMB   TIM1->CCR3  //PE13

//PWM最小转速
void PWM_PullDownAll();
//PWM最大转速
void PWM_PullUpAll();
//关断所有电机
void PWM_DisableAll();
//初始化PWM输出
void init_drv_PWMOut();
//函数功能：初始化电机接口
void init_drv_Motor(void);