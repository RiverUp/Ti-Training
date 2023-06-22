#include "HC-SR04.h"
#include "rtthread.h"
#include "stdio.h"
#include "Serial.h"
#include "Delay.h"

uint32_t countValue;
rt_sem_t AbleToConvert;

//p2.5Ϊ��������
const Timer_A_CaptureModeConfig hcsrCaptureConfig=
{
	TIMER_A_CAPTURECOMPARE_REGISTER_2,
	TIMER_A_CAPTUREMODE_FALLING_EDGE,
	TIMER_A_CAPTURE_INPUTSELECT_CCIxA,
	TIMER_A_CAPTURE_SYNCHRONOUS,
	TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
	TIMER_A_OUTPUTMODE_OUTBITVALUE
};
//����ģʽ��SMCLKΪʱ��Դ��12��Ƶ��������1mhz
const Timer_A_ContinuousModeConfig hcsrContinueConfig=
{
	TIMER_A_CLOCKSOURCE_SMCLK,
	TIMER_A_CLOCKSOURCE_DIVIDER_12,
	TIMER_A_TAIE_INTERRUPT_DISABLE,
	TIMER_A_SKIP_CLEAR
};

void init_hc_sr04(void)
{
	//p2.5�������룬�����ش����ⲿ�ж�
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2,GPIO_PIN5);
	GPIO_clearInterruptFlag(GPIO_PORT_P2,GPIO_PIN5);
	GPIO_selectInterruptEdge(GPIO_PORT_P2,GPIO_PIN5,GPIO_LOW_TO_HIGH_TRANSITION);
	GPIO_enableInterrupt(GPIO_PORT_P2,GPIO_PIN5);
	Interrupt_enableInterrupt(INT_PORT2);

	//p3.0���
	GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0);

	//��ʼ��ʱ��Ϊ��������ģʽ
	Timer_A_initCapture(TIMER_A0_BASE,&hcsrCaptureConfig);
	Timer_A_configureContinuousMode(TIMER_A0_BASE,&hcsrContinueConfig);
}
void trigger_measure(void)
{
	//p3.0���1ms�ߵ�ƽ����hcsr04
	GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN0);
	rt_thread_mdelay(1);
	GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0);
}
//��λΪcm
float read_hc_sr04(uint32_t countValue)
{
	float distance=countValue*0.017;
	return distance;
}


