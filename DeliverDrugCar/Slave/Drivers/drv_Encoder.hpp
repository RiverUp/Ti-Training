#ifndef __ENCODER_H
#define __ENCODER_H

#include <sys.h>	 

//不可大于65535 因为F103的定时器是16位的。
#define ENCODER_TIM_PERIOD (u16)(65535)   

/*
 * 函数功能：把TIM3初始化为编码器接口模式
 * 入口参数：无
 * 返回  值：无
 */
void drv_Encoder_Init_TIM3(void);

/*
 * 函数功能：把TIM4初始化为编码器接口模式
 * 入口参数：无
 * 返回  值：无
 */
void drv_Encoder_Init_TIM4(void);

/*
 * 函数功能：单位时间读取编码器计数
 * 入口参数：TIMX：定时器
 * 返回  值：速度值
 */
int Read_Encoder(u8 TIMX);

#endif
