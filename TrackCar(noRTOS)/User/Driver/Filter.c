#include "Filter.h"



float Kalman_TCRT(float tcrt_measure)
{
	//输出值
	static float tcrt;
	float Q_tcrt=0.001; 		// 过程噪声的协方差
	float R_tcrt=0.5;				// 测量噪声的协方差 既测量偏差
	//协方差及其初始值
	static float P_tcrt=1;
	
	
	P_tcrt+=Q_tcrt*10;
	//卡尔曼增益分母
	float S_tcrt=P_tcrt+R_tcrt;
	//卡尔曼增益
	float K_tcrt=P_tcrt/S_tcrt;
	
	P_tcrt-=K_tcrt*P_tcrt;
	
	tcrt+=K_tcrt*(tcrt_measure-tcrt);
	
	return tcrt;
	
}