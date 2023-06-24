#ifndef __TCRT5000_h
#define __TCRT5000_h 

#include <stdint.h>
#include <stdbool.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//p5.5->analog

#define BITS  16384          //2^13
#define THRESHOLD1 1
#define THRESHOLD2 2

void init_TCRT();
float read_TCRT();
//左中右->p5.2,p1.7,p5.0
//循迹模块遇到黑线，模块输出高电平，输出指示灯熄灭
void init_digitalTCRT();
//123,左中右
bool getTCRTValue(int num);

#endif // !__TCRT5000.h