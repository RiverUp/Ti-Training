#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"
#include "Serial/Serial.h"
#include "serial_entry.h"
#include "tcrt_entry.h"
#include "stdbool.h"

//rt_thread_t blink_thread = RT_NULL;

float Voltage;
bool sendTextTrigger;
rt_event_t SendVoltageEvent;

//static void blink_entry()
//{
//	WDT_A_hold(WDT_A_BASE);
//	GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
//	while(1)
//	{
//		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
//		rt_thread_mdelay(500);
//	}
//}
int main(void)
{
	//设置主时钟为12mhz
	//CS_setDCOFrequency(CS_12MHZ);
	//关闭看门狗
	WDT_A_hold(WDT_A_BASE);
	MAP_FPU_enableModule();
  MAP_FPU_enableLazyStacking();
//	initSerial();
	rt_thread_t tcrt_thread=rt_thread_create("tcrt",tcrt_entry,RT_NULL,1024,20,5);
	rt_thread_t serial_thread=rt_thread_create("serial",serial_entry,RT_NULL,1024,20,25);
	if(tcrt_thread!=RT_NULL)
	{
		rt_thread_startup(tcrt_thread);
	}
	if(serial_thread!=RT_NULL)
	{
		rt_thread_startup(serial_thread);
	}
	while(1)
	{}
//	blink_thread=rt_thread_create("blink",blink_entry,RT_NULL,1024,25,5);
//	if(blink_thread!=RT_NULL)
//	{
//		rt_thread_startup(blink_thread);
//	}
}

