#include "Sg90.h"

// int UnitValue;

// // PB6进行PWM输出
// void initSg90()
// {
//     SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//     GPIOPinConfigure(GPIO_PB6_M0PWM0);
//     GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
//     PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

//     // pwm 50hz->20ms
//     PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, (SysCtlClockGet() / 50));

//     // PWMIntEnable(PWM0_BASE, PWM_INT_GEN_0);
//     // PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);
//     // IntEnable(INT_PWM0_0);

//     // 0.1%单位量
//     UnitValue = ((SysCtlClockGet() / 50) / 1000);
//     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0_BIT, (1000-50) * UnitValue);

//     PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
//     PWMGenEnable(PWM0_BASE, PWM_GEN_0);
// }

// void rotateSg90(int angle)
// {
//     int value = 5 * angle / 9 + 25;
//     value=1000-value;
//     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0_BIT, value * UnitValue);
//     //PWMGenEnable(PWM0_BASE, PWM_GEN_0);
// }

void initSg90()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_A, SysCtlClockGet() / 50);
    TimerMatchSet(TIMER3_BASE,TIMER_A,TimerLoadGet(TIMER3_BASE,TIMER_A)/(1-0.025));
    TimerEnable(TIMER3_BASE, TIMER_A);
}

void rotateSg90(int angle)
{
    float value = 5 * angle / 9 + 25;
    value/=1000;
    TimerMatchSet(TIMER3_BASE,TIMER_A,TimerLoadGet(TIMER3_BASE,TIMER_A)/(1-value));
}
