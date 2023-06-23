#include "Delay.h"

static uint8_t  fac_us=0;  


//初始化延时函数
void Delay_Init(void)
{
	fac_us = SystemCoreClock / 1000000;      
}


//延时us
void delay_us(uint32_t nus)
{
	uint32_t temp;

	SysTick->LOAD=(uint32_t)(nus*fac_us);  
	SysTick->VAL=0x00;         
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}


//延时ms
void delay_ms(uint16_t nms)
{
	uint16_t i = 0;
	for(i=0;i<nms;i++)
		delay_us(1000);

}

