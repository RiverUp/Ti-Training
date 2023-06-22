#include "hcsr_entry.h"

void hcsr_entry()
{
	init_hc_sr04();
	ableForNextHCSRSample=rt_sem_create("AbleForNextHCSRSample",0,RT_IPC_FLAG_PRIO);
	while(1)
	{
		trigger_measure();
		rt_sem_take(ableForNextHCSRSample,50);
	}
}