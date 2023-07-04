#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"
#include "stdbool.h"
#include "Delay.h"
#include "control_entry.h"
#include "display_entry.h"

//rt_thread_t blink_thread = RT_NULL;

#define DEBUG 1


float Voltage;
float Voltage_filted;
rt_event_t SendVoltageEvent;


int main(void)
{
	//设置主时钟为12mhz
	//CS_setDCOFrequency(CS_12MHZ);
	//关闭看门狗
	WDT_A_hold(WDT_A_BASE);
//	MAP_FPU_enableModule();
//  MAP_FPU_enableLazyStacking();
	Delay_Init();
	Interrupt_enableMaster();
	
	rt_thread_t control_thread=rt_thread_create("Control",control_entry,RT_NULL,1024,25,25);
	if(control_thread!=RT_NULL)
	{
		rt_thread_startup(control_thread);
	}
	
	rt_thread_t display_thread=rt_thread_create("Display",display_entry,RT_NULL,1024,25,25);
	if(display_thread!=RT_NULL)
	{
		rt_thread_startup(display_thread);
	}
	
}

