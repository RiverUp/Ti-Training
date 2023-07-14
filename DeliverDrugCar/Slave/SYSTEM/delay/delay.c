#include "delay.h"

//us延时倍乘数	
static u8  fac_us=0;					
//ms延时倍乘数,在ucos下,代表每个节拍的ms数
static u16 fac_ms=0;							
	   
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
	SysTick->CTRL&=~(1<<2);	
	fac_us=9;					
	//非OS下,代表每个ms需要的systick时钟数
	fac_ms=(u16)fac_us*1000;				   
}						

//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	//时间加载
	SysTick->LOAD=nus*fac_us; 						
	//清空计数器	
	SysTick->VAL=0x00;        					
	//开始倒数
	SysTick->CTRL=0x01;		  
	do
	{
		temp=SysTick->CTRL;
		//等待时间到达 
	}while((temp&0x01)&&!(temp&(1<<16)));		  
	//关闭计数器
	SysTick->CTRL=0x00;	
	//清空计数器
	SysTick->VAL =0X00;      					 	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;         
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       				
} 


