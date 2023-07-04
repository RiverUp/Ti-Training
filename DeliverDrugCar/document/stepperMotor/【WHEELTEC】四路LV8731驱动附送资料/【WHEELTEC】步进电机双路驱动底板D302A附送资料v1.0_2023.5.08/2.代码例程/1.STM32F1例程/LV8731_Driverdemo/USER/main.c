# include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "moto.h"
#include "gpio.h"

/**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
//------------����˵��---------------
//LV8731_ģ��    -----------   STM32F1����1����

//    5V      -----------    5V
//    GND     -----------    GND
//    STEP2   -----------    PB6
//    STEP1   -----------    PB7
//    ST      -----------    PB14
//    DIR1    -----------    PB15
//    DIR2    -----------    PB13

int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;       //���PWM���� Ӧ��Motor

int main(void)
{
    SystemInit();				     												    //======����ϵͳʱ��Ϊ72M
    delay_init();            												    //======��ʼ����ʱ����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	  //======�����ж����ȼ�����2
    uart_init(9600);		        											  //======���ڳ�ʼ��
    Gpio_Init();																				//======GPIO��ʼ��
    pwm_int_TIM4(65535,14);     												//======��ʼ��pwm��� 72000 000 /(65535+1)*(14+1)																		  //======ADC1�ĳ�ʼ��
    Final_Moto1=1000;																		//======��ʼ��������ٶ�
    while(1)
    {
        control(1,100,0);                //���  1  �����ת��(0-900)0˳ʱ��ת
        control(2,500,0);                //���  2  �����ת��(0-900)0��ʱ��ת

		printf("Final_Moto1=%d\r\n",Final_Moto1);
		printf("Final_Moto2=%d\r\n",Final_Moto2);
    }
}




