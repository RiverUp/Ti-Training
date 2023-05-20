#include "motor_entry.h"
#include "Motor/Motor.h"
#include "rtthread.h"

extern rt_sem_t SendDatSem;

void motor_entry()
{
	init_motor();
	while(1)
	{
		if(rt_sem_take(SendDatSem,RT_WAITING_FOREVER)==RT_EOK)
		{
			//根据循迹调整输出
		}
	}
	
}