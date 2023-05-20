#include "tcrt_entry.h"
#include "TCRT5000/TCRT5000.h"
#include "rtthread.h"
#include "stdbool.h"
#include "Filter/Filter.h"

extern float Voltage;
extern float Voltage_filted;

rt_sem_t SendDatSem;
rt_sem_t ProcDatSem;
enum Car_State {
	White,Black,Off_Ground
};

void tcrt_entry()
{
	init_TCRT();
	GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	ProcDatSem=rt_sem_create("ProcessDat",0,RT_IPC_FLAG_PRIO);
	SendDatSem=rt_sem_create("SendDatSerial",0,RT_IPC_FLAG_PRIO);
	enum Car_State state;
	
	rt_timer_t ADCSampleTimer=rt_timer_create("ADCSampleTimer",sampleVoltage,RT_NULL,TCRT_SAMPLE_INTERVAL,RT_TIMER_FLAG_PERIODIC);
	if(ADCSampleTimer!=RT_NULL)
	{
		rt_timer_start(ADCSampleTimer);
	}
	while(1)
	{
		if(rt_sem_take(ProcDatSem,RT_WAITING_FOREVER)==RT_EOK)
		{
			Voltage=Voltage/BITS*3.3;
			Voltage_filted=Kalman_TCRT(Voltage);
			
			if(Voltage_filted<THRESHOLD1)
			{
				state=White;
			}
			else if(Voltage_filted>THRESHOLD2)
			{
				state=Off_Ground;
			}
			else
			{
				state=Black;
			}
			
			if(SendDatSem->value==0)
			{
				rt_sem_release(SendDatSem);
			}
			
			
		}	
	}

}

void sampleVoltage()
{
	Voltage=read_TCRT();
	if(ProcDatSem->value==0)
	{
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		rt_sem_release(ProcDatSem);
	}
}