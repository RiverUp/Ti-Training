#pragma once

#include "sys.h"
#include <stdint.h>
#include "TimeBase.h"

#ifdef __cplusplus
	extern "C" {
#endif

//�·��������ݱ�
extern u8 const mon_table[12];	

u8 SystemClock_Config(void);        							//��ʼ��ϵͳʱ��,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);												//ƽ��,�����ж�
u8 RTC_Get_Week(u16 year,u8 month,u8 day);				//��ȡ���ں�		 

//ʱ���ʼ������
void init_TimeBase();

#ifdef __cplusplus
	}
#endif