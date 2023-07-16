#include "TimerTrail.h"

void init_timer0(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE,TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PERIODIC);
    //1ms 一次中断
    TimerLoadSet(TIMER0_BASE,TIMER_B,SysCtlClockGet()/1000);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMB_TIMEOUT);
    IntEnable(INT_TIMER0B);
    TimerEnable(TIMER0_BASE,TIMER_B);
}