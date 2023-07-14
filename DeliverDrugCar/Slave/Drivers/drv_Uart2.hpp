#pragma once

#include <stdint.h>
#include "Basic.hpp"
#include "GUI.hpp"
#include "Control.hpp"

/*
 * 函数功能：串口2初始化
 * 入口参数：波特率
 * 返回  值：无
 */
extern u8 Cx,Cy,Cw,Ch,Ci;
extern float Track_Bias;
 
void init_drv_Uart2(u32 bound);
