#pragma once
#include "stm32f10x.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
	extern "C" {
#endif
		
#define SYSCLK 72000000
#define HCLK 72000000
#define PCLK1 36000000
#define PCLK2 72000000
#define ADCCLK 36000000
#define APB1CLK 72000000
#define APB1TIMERCLK 72000000
#define APB2CLK 72000000
#define APB2TIMERCLK 72000000
#define TIMEBASECLK 12000000
#define USART2345CLK 36000000
#define USART1CLK 72000000
		
		
/*RTC µ ± ±÷”*/
typedef struct
{
	uint16_t Year;
	uint8_t Month;
	uint8_t Date;
	uint8_t WeekDay;
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
	uint32_t SubSeconds;
	uint8_t TimeFormat;
	
	double Seconds_f;
}RTC_TimeStruct;

#ifdef __cplusplus
	}
#endif