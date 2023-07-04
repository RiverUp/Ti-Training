#pragma once

#include <string.h>
#include <stdio.h>
#include "Basic.h"
#include "stm32f10x.h"
#include "delay.h"
#include "TimeBase.h"
#include "TimeBase.hpp"


/*
	配置寄存器
	reg:寄存器指针
	value:要填入指定位中的值
	offset:要填入的位的偏移
	value_length:要填入的位数
*/
inline void set_register( volatile unsigned int& reg , const unsigned char value , const unsigned char offset , const unsigned char value_length )
{
	//最终偏移地址
	unsigned char offset_end_bit = offset + value_length;
	//依次置位
	for(unsigned char i = offset ; i < offset_end_bit ; ++ i)
		reg &= ~(1<<i);
	reg |= (value << offset);
}

//延时函数
inline void os_delay(double t)
{
	delay_us(1);
}

//初始化完成指示
bool getInitializationCompleted();
//初始化未完成不能进行初始化操作
void setInitializationCompleted();
//自锁状态
void LockInitializationStatus();
//解锁状态
void UnLockInitializationStatus();
//系统时钟配置函数
void init_Basic();

