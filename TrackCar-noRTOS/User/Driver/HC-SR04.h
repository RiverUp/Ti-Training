#ifndef __HC_SR04_H
#define __HC_SR04_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"

//p2.5��echo��p3.0��trig
#define HCSRStandardValue 120
extern uint32_t HCSRCountValue;
extern uint32_t HCSRCountMultiValue;
extern uint32_t HCSRCountAveraValue;
extern uint32_t TriggerCounter;
extern bool NextTiggerHCSRFlag;


//��ʼ��
void init_hc_sr04(void);
//��������
void trigger_measure(void);
//ת������-��param����ʱ������ļ���
float read_hc_sr04(uint32_t countValue);


#endif

