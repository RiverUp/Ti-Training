#ifndef __HC_SR04_H
#define __HC_SR04_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"

//p2.5��echo��p3.0��trig

extern uint32_t countValue;
extern rt_sem_t AbleToConvert;

//��ʼ��
void init_hc_sr04(void);
//��������
void trigger_measure(void);
//ת������-��param����ʱ������ļ���
float read_hc_sr04(uint32_t countValue);


#endif
