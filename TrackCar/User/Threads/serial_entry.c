#include "Serial/Serial.h"
#include "serial_entry.h"
#include "rtthread.h"
#include "stdio.h"
#include "stdbool.h"
#include "TCRT5000/TCRT5000.h"

extern rt_sem_t SendDatSem;
extern float Voltage;
extern float Voltage_filted;

void serial_entry()
{
	initSerial();
	rt_uint32_t e;
	char text_sent[20];
	while(1)
	{
		if(rt_sem_take(SendDatSem,RT_WAITING_FOREVER)==RT_EOK)
//		if(sendTextTrigger)
		{
			sprintf(text_sent,"%f %f\r\n",Voltage,Voltage_filted);
			sendText(EUSCI_A0_BASE, text_sent);
		}
	}
	
}