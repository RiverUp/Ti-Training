#include "Filter.h"



float Kalman_TCRT(float tcrt_measure)
{
	//���ֵ
	static float tcrt;
	float Q_tcrt=0.001; 		// ����������Э����
	float R_tcrt=0.5;				// ����������Э���� �Ȳ���ƫ��
	//Э������ʼֵ
	static float P_tcrt=1;
	
	
	P_tcrt+=Q_tcrt*10;
	//�����������ĸ
	float S_tcrt=P_tcrt+R_tcrt;
	//����������
	float K_tcrt=P_tcrt/S_tcrt;
	
	P_tcrt-=K_tcrt*P_tcrt;
	
	tcrt+=K_tcrt*(tcrt_measure-tcrt);
	
	return tcrt;
	
}