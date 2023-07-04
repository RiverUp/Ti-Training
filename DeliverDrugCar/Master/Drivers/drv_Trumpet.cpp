#include "Commulink.hpp"
#include "Basic.hpp"
#include "drv_LED.hpp"
#include <map>
#include "drv_Trumpet.hpp"
#include "delay.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef const uint16_t uc16;

//õ������
void Play(void)
{
	//��7  1   2   3   4   5   6   7  ��1 ��2 ��3 ��4 ��5 ������
	uc16 tone[] = {247,262,294,330,349,392,440,294,523,587,659,698,784,1000};//��Ƶ���ݱ�
	//õ������
	u8 music[]=
	{
		8,7,5,3,2,3,5,3,3,13,
		2,3,6,2,3,6,3,3,13,
		2,3,6,2,3,6,5,3,1,7,5,3,
		2,3,5,3,3,13,
		2,3,6,2,3,6,3,3,13,
		2,3,6,2,3,6,5,3,13,13,13
	};     
	u8 time[] = 
	{       
		4,4,4,4,2,2,2,1,4,2,
		1,1,2,1,1,2,2,4,2,
		1,1,4,1,1,1,1,4,4,4,4,4,
		2,2,2,2,4,2,
		1,1,2,1,1,2,2,4,2,
		1,1,2,1,1,2,2,4,4,4,4,4
		
	};     
	u32 yanshi;
	u16 i,e;
	yanshi = 10;
	for(i=0;i<sizeof(music)/sizeof(music[0]);i++)
	{
		set_TrumpetOnOff(false);
		for(e=0;e<((u16)time[i])*tone[music[i]]/yanshi;e++)
		{
			//��������
			set_TrumpetOnOff(true);
			set_TrumpetFreq((u32)tone[music[i]]);
			delay_us(2000);
		}      
	}
}

/*
	��������Ƶ�ʵ��ں���
	freq:������Ƶ��
*/
void set_TrumpetFreq( unsigned short freq )
{
	float PWM2 = (float)TIM1->CCR2/(float)TIM1->ARR;
	float PWM3 = (float)TIM1->CCR3/(float)TIM1->ARR;
	float PWM4 = (float)TIM1->CCR4/(float)TIM1->ARR;
	if( freq < 200 )
		freq = 200;
	else if(freq!=1000)
	{
		TIM1->ARR = 1e6 / freq;
		if( TIM1->CCR1 != 0 )
			TIM1->CCR1 = TIM1->ARR / 2;
		TIM1->CCR2 = PWM2*TIM1->ARR;
		TIM1->CCR3 = PWM3*TIM1->ARR;
		TIM1->CCR4 = PWM4*TIM1->ARR;
	}
	else
	{
		TIM1->CCR1 = 0;
	}
}

/*
	���������캯��
	on:�Ƿ�����
*/
void set_TrumpetOnOff( bool on )
{
	if(on)
		TIM1->CCR1 = TIM1->ARR / 2;
	else
		TIM1->CCR1 = 0;
}