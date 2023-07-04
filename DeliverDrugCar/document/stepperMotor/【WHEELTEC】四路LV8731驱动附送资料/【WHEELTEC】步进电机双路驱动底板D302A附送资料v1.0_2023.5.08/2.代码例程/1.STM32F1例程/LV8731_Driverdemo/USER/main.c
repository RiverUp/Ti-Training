# include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "moto.h"
#include "gpio.h"

/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
//------------接线说明---------------
//LV8731_模块    -----------   STM32F1主板1引脚

//    5V      -----------    5V
//    GND     -----------    GND
//    STEP2   -----------    PB6
//    STEP1   -----------    PB7
//    ST      -----------    PB14
//    DIR1    -----------    PB15
//    DIR2    -----------    PB13

int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;       //电机PWM变量 应是Motor

int main(void)
{
    SystemInit();				     												    //======配置系统时钟为72M
    delay_init();            												    //======初始化延时函数
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	  //======设置中断优先级分组2
    uart_init(9600);		        											  //======串口初始化
    Gpio_Init();																				//======GPIO初始化
    pwm_int_TIM4(65535,14);     												//======初始化pwm输出 72000 000 /(65535+1)*(14+1)																		  //======ADC1的初始化
    Final_Moto1=1000;																		//======初始步进电机速度
    while(1)
    {
        control(1,100,0);                //电机  1  电机的转速(0-900)0顺时针转
        control(2,500,0);                //电机  2  电机的转速(0-900)0逆时针转

		printf("Final_Moto1=%d\r\n",Final_Moto1);
		printf("Final_Moto2=%d\r\n",Final_Moto2);
    }
}




