#include "TrailEntry.hpp"


void trail_entry(void *parameter)
{
	Sg90Response=rt_sem_create("Sg90Response",0,RT_IPC_FLAG_PRIO);
	
	while(1)
	{
		if(rt_sem_take(Sg90Response,RT_WAITING_FOREVER) == RT_EOK)
		{
			int msgLength=USART1_RX_STA&0x3fff;
			//´¦Àíº¯Êý
			int angle=atoi(USART1_RX_BUF);
			
			rotate_sg90(angle);
			
			USART1_RX_STA=0;
			memset(USART1_RX_BUF,0,msgLength);
		}
	}
}
