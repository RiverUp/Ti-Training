#pragma once

#include "sys.h"
#include <stdint.h>
#include "TimeBase.h"

#ifdef __cplusplus
	extern "C" {
#endif

//月份日期数据表
extern u8 const mon_table[12];	

u8 SystemClock_Config(void);        							//初始化系统时钟,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);												//平年,闰年判断
u8 RTC_Get_Week(u16 year,u8 month,u8 day);				//获取星期号		 

//时间初始化函数
void init_TimeBase();

#ifdef __cplusplus
	}
#endif