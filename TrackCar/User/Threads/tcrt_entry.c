#include "tcrt_entry.h"
#include "TCRT5000/TCRT5000.h"
#include "rtthread.h"
#include "stdbool.h"

extern float Voltage;
extern bool sendTextTrigger;

rt_sem_t SendVoltageSem;

void tcrt_entry()
{
	init_TCRT();
	GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	SendVoltageSem=rt_sem_create("SendVoltage",0,RT_IPC_FLAG_PRIO);
	
	rt_timer_t ADCSampleTimer=rt_timer_create("ADCSampleTimer",sampleVoltage,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);
	if(ADCSampleTimer!=RT_NULL)
	{
		rt_timer_start(ADCSampleTimer);
	}

}

void sampleVoltage()
{
	Voltage=read_TCRT();
	if(SendVoltageSem->value==0)
	{
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		rt_sem_release(SendVoltageSem);
	}
		sendTextTrigger=true;
}