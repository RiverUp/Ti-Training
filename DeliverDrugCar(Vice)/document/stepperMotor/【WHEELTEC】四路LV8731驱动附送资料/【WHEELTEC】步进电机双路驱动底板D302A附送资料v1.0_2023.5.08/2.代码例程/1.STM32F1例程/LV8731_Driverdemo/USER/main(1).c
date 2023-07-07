# include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"
#include "adc1.h"
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

//------------����˵��---------------

//   LV8731_ģ��  ----------  ���A
//    AOUT1   ------------  ����ߵ�Դ+
//    AOUT2   ------------  ����ߵ�Դ-

//   LV8731_ģ��  ----------  ���B
//    BOUT1   ------------  ����ߵ�Դ+
//    BOUT2   ------------  ����ߵ�Դ-

//   LV8731_ģ��  ----------  ���C
//    COUT1   ------------  ����ߵ�Դ+
//    COUT2   ------------  ����ߵ�Դ-

//   LV8731_ģ��  ----------  ���D
//    DOUT1   ------------  ����ߵ�Դ+
//    DOUT2   ------------  ����ߵ�Դ-
int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;       //���PWM���� Ӧ��Motor

int main(void)
{
    u16 adcx;
    float vcc;
    SystemInit();				     												    //======����ϵͳʱ��Ϊ72M
    delay_init();            												    //======��ʼ����ʱ����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	  //======�����ж����ȼ�����2
    uart_init(9600);		        											  //======���ڳ�ʼ��
    Gpio_Init();																				//======GPIO��ʼ��
    pwm_int_TIM4(65535,14);     												//======��ʼ��pwm��� 72000 000 /(65535+1)*(14+1)
    adc1_Init();																			  //======ADC1�ĳ�ʼ��
    Final_Moto1=1000;																		//======��ʼ��������ٶ�
    while(1)
    {
        control(1,100,1);                //A���  1=motoA   �����ת��(0-900)1��ת
        control(2,500,0);                //B���  2=motoB   �����ת��(0-900)0��ת
        control(3,400,0);                //C���  3=motoC   �����ת��(0-900)0��ת
        control(4,900,1);                //D���  4=motoD   �����ת��(0-900)1��ת

			printf("Final_Moto1=%d\r\n",Final_Moto1);
			printf("Final_Moto2=%d\r\n",Final_Moto2);
			printf("Final_Moto3=%d\r\n",Final_Moto3);
			printf("Final_Moto4=%d\r\n",Final_Moto4);
        adcx=Get_adc1_Average(ADC_Channel_0,10);  //ACDͨ��0 ��10��adcƽ��ֵ
//	 printf("ADC��ֵ=%d \r\n",adcx);      			//��ӡadc��ֵ
        vcc=(float)adcx*(3.3*11/4096);     				//��ǰ��ѹ
        printf("��ǰ��ѹ=%6.2f V \r\n",vcc);				//��ӡ��ǰ��ѹ
    }
}




