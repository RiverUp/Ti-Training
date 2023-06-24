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
//������->p5.2,p1.7,p5.0
//ѭ��ģ���������ߣ�ģ������ߵ�ƽ�����ָʾ��Ϩ��
void init_digitalTCRT();
//123,������
bool getTCRTValue(int num);

#endif // !__TCRT5000.h