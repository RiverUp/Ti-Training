#include "delay.h"

//us��ʱ������	
static u8  fac_us=0;					
//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
static u16 fac_ms=0;							
	   
//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()
{
	SysTick->CTRL&=~(1<<2);	
	fac_us=9;					
	//��OS��,����ÿ��ms��Ҫ��systickʱ����
	fac_ms=(u16)fac_us*1000;				   
}						

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	//ʱ�����
	SysTick->LOAD=nus*fac_us; 						
	//��ռ�����	
	SysTick->VAL=0x00;        					
	//��ʼ����
	SysTick->CTRL=0x01;		  
	do
	{
		temp=SysTick->CTRL;
		//�ȴ�ʱ�䵽�� 
	}while((temp&0x01)&&!(temp&(1<<16)));		  
	//�رռ�����
	SysTick->CTRL=0x00;	
	//��ռ�����
	SysTick->VAL =0X00;      					 	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
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


