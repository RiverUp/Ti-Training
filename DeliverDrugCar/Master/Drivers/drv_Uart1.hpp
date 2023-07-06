#pragma once

#include <stdint.h>
#include "Basic.hpp"
#include "rtthread.h"
#include "OwnSemaphore.hpp"
#include <stdlib.h>
#include "Sg90.hpp"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200

#ifdef __cplusplus
	extern "C" {
#endif
			
extern char  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	bit15接收完成标志（0x0a） bit14（0x0d） bit13~bit0 有效数据个数		
		
/*
 * 函数功能：串口初始化
 * 入口参数：波特率
 * 返回  值：无
 */
void init_drv_Uart1(u32 bound);
		
		
		
#ifdef __cplusplus
	}
#endif

