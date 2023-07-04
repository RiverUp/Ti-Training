#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "sys.h"

#define KEY PCin(0)   
//是否使用硬件去抖
#define Is_Detrembling 0

void init_drv_Key(void);       //按键初始化

/*
 * 函数功能：按键扫描
 * time：双击等待时间
 * 返回值：按键状态 0：无动作 1：单击 2：双击 
 */
u8 click_N_Double (u16 time);
/*
 * 函数功能：按键扫描（无硬件驱动）
 * 返回值：按键状态 0：无动作 1：单击
 */
u8 click(void);
/*
 * 函数功能：长按检测
 * 返回值：按键状态 0：无动作 1：长按2s
 */
u8 Long_Press(void);
