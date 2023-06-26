#ifndef _HCSR04_H
#define _HCSR04_H
#include "stdint.h"     
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Delay.h"

void HCSR04_Init(void);				//��ʼ������
float Distance(void);				//��ຯ��
extern unsigned char ctl_ctl; 		//��ʱ����
extern unsigned char ctl; 			//��ʱ����
extern uint8_t Flag_Target;			//��ʱ����
#endif
