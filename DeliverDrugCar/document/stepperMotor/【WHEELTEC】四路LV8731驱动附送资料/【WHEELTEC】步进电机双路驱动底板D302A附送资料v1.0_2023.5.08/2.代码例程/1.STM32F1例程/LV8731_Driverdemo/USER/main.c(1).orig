# include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "adc1.h"
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
//    VA      -----------    PB6
//    VB      -----------    PB7
//    VC      -----------    PB8
//    VD      -----------    PB9
//    ADC     -----------    PA0
//    ST      -----------    PA1
//    DA      -----------    PA2
//    DB      -----------    PA3
//    DC      -----------    PA6
//    DD      -----------    PA7

//------------接线说明---------------

//   LV8731_模块  ----------  电机A
//    AOUT1   ------------  电机线电源+
//    AOUT2   ------------  电机线电源-

//   LV8731_模块  ----------  电机B
//    BOUT1   ------------  电机线电源+
//    BOUT2   ------------  电机线电源-

//   LV8731_模块  ----------  电机C
//    COUT1   ------------  电机线电源+
//    COUT2   ------------  电机线电源-

//   LV8731_模块  ----------  电机D
//    DOUT1   ------------  电机线电源+
//    DOUT2   ------------  电机线电源-
int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;       //电机PWM变量 应是Motor

int main(void)
{
    u16 adcx;
    float vcc;
    SystemInit();				     												    //======配置系统时钟为72M
    delay_init();            												    //======初始化延时函数
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	  //======设置中断优先级分组2
    uart_init(9600);		        											  //======串口初始化
    Gpio_Init();																				//======GPIO初始化
    pwm_int_TIM4(65535,14);     												//======初始化pwm输出 72000 000 /(65535+1)*(14+1)
    adc1_Init();																			  //======ADC1的初始化
    Final_Moto1=1000;																		//======初始步进电机速度
    while(1)
    {
        control(1,100,1);                //A电机  1=motoA   电机的转速(0-900)1正转
        control(2,500,0);                //B电机  2=motoB   电机的转速(0-900)0反转
        control(3,400,0);                //C电机  3=motoC   电机的转速(0-900)0反转
        control(4,900,1);                //D电机  4=motoD   电机的转速(0-900)1正转

			printf("Final_Moto1=%d\r\n",Final_Moto1);
			printf("Final_Moto2=%d\r\n",Final_Moto2);
			printf("Final_Moto3=%d\r\n",Final_Moto3);
			printf("Final_Moto4=%d\r\n",Final_Moto4);
        adcx=Get_adc1_Average(ADC_Channel_0,10);  //ACD通道0 求10次adc平均值
//	 printf("ADC的值=%d \r\n",adcx);      			//打印adc的值
        vcc=(float)adcx*(3.3*11/4096);     				//求当前电压
        printf("当前电压=%6.2f V \r\n",vcc);				//打印当前电压
    }
}




