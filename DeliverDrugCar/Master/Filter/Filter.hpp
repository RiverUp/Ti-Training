#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

//定义最大样本数
#define DELTA_MAX_SAMPLES 12
#define M_PI_FLOAT  3.14159265358979323846f    
//二阶巴特沃斯滤波器的品质因数Q
#define BIQUAD_Q (1.0f / sqrtf(2.0f))   
#define UNUSED(x) (void)(x)

float Kalman_Filter_x(float Accel,float Gyro);		
float Complementary_Filter_x(float angle_m, float gyro_m);
float Kalman_Filter_y(float Accel,float Gyro);		
float Complementary_Filter_y(float angle_m, float gyro_m);

//低通滤波器的系数
typedef struct pt1Filter_s 
{
	float state;	//滤波结果
	float RC;			//模电RC滤波的电阻和电容值的积
	float dT;			//数据更新时间隔（秒）
}pt1Filter_t;

//IIR 滤波器的系数
typedef struct biquadFilter_s 
{
	float b0, b1, b2, a1, a2;
	float d1, d2;
}biquadFilter_t;

typedef enum 
{
	FILTER_PT1 = 0,
	FILTER_BIQUAD,
}filterType_e;

//滤波器的种类
typedef enum 
{
	FILTER_LPF,						//低通滤波器
	FILTER_NOTCH					//带通滤波器
}biquadFilterType_e;

typedef float (*filterApplyFnPtr)(void *filter, float input);
float nullFilterApply(void *filter, float input);
void pt1FilterInit(pt1Filter_t *filter, uint8_t f_cut, float dT);
float pt1FilterApply(pt1Filter_t *filter, float input);
float pt1FilterApply4(pt1Filter_t *filter, float input, uint8_t f_cut, float dT);
void biquadFilterInitLPF(biquadFilter_t *filter, float filterFreq, uint32_t sampleDeltaUs);
float biquadFilterApply(biquadFilter_t *filter, float input);

		
#ifdef __cplusplus
	}
#endif
		
#endif
