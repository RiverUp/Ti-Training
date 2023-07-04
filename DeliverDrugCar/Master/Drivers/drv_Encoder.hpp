#ifndef __ENCODER_H
#define __ENCODER_H

#include <sys.h>	 

//���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
#define ENCODER_TIM_PERIOD (u16)(65535)   

/*
 * �������ܣ���TIM3��ʼ��Ϊ�������ӿ�ģʽ
 * ��ڲ�������
 * ����  ֵ����
 */
void drv_Encoder_Init_TIM3(void);

/*
 * �������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
 * ��ڲ�������
 * ����  ֵ����
 */
void drv_Encoder_Init_TIM4(void);

/*
 * �������ܣ���λʱ���ȡ����������
 * ��ڲ�����TIMX����ʱ��
 * ����  ֵ���ٶ�ֵ
 */
int Read_Encoder(u8 TIMX);

#endif
