#include "Serial/Serial.h"
#include "serial_entry.h"
#include "rtthread.h"
#include "stdio.h"
#include "stdbool.h"
#include "TCRT5000/TCRT5000.h"

extern rt_sem_t SendVoltageSem;
extern float Voltage;
extern bool sendTextTrigger;

void serial_entry()
{
	initSerial();
	rt_uint32_t e;
	char text_sent[20];
	while(1)
	{
		if(rt_sem_take(SendVoltageSem,RT_WAITING_FOREVER)==RT_EOK)
//		if(sendTextTrigger)
		{
			Voltage=Voltage/BITS*3.3;
			sprintf(text_sent,"voltage is %f\r\n",Voltage);
			sendText(EUSCI_A0_BASE, text_sent);
			sendTextTrigger=false;
		}
	}
	
}