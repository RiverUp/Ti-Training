#include "HC-SR04.h"
#include "rtthread.h"
#include "stdio.h"
#include "Serial.h"
#include "Delay.h"

uint32_t HCSRCountValue;
uint32_t HCSRCountMultiValue;
uint32_t HCSRCountAveraValue;
uint32_t TriggerCounter;
bool NextTiggerHCSRFlag = 1;

// p2.5为输入引脚
const Timer_A_CaptureModeConfig hcsrCaptureConfig =
	{
		TIMER_A_CAPTURECOMPARE_REGISTER_2,
		TIMER_A_CAPTUREMODE_NO_CAPTURE,
		TIMER_A_CAPTURE_INPUTSELECT_CCIxA,
		TIMER_A_CAPTURE_SYNCHRONOUS,
		TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
		TIMER_A_OUTPUTMODE_OUTBITVALUE
	};
// 连续模式，SMCLK为时钟源，12分频————1mhz
const Timer_A_ContinuousModeConfig hcsrContinuousConfig =
	{
		TIMER_A_CLOCKSOURCE_SMCLK,
		TIMER_A_CLOCKSOURCE_DIVIDER_12,
		TIMER_A_TAIE_INTERRUPT_DISABLE,
		TIMER_A_SKIP_CLEAR
	};

void init_hc_sr04(void)
{
	// p2.5下拉输入，上升沿触发外部中断
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN5);
	GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN5);
	GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
	GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN5);
	Interrupt_enableInterrupt(INT_PORT2);
	
	// p3.0输出
	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);

	Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
	Interrupt_enableInterrupt(INT_T32_INT2);
	Timer32_setCount(TIMER32_1_BASE, 120); // 10us
	Timer32_enableInterrupt(TIMER32_1_BASE);
}
void trigger_measure(void)
{
	// p3.0输出20us高电平触发hcsr04
	GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
	NextTiggerHCSRFlag=false;
	delay_us(20);
	GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);
}

