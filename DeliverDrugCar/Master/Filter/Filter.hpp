#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

//�������������
#define DELTA_MAX_SAMPLES 12
#define M_PI_FLOAT  3.14159265358979323846f    
//���װ�����˹�˲�����Ʒ������Q
#define BIQUAD_Q (1.0f / sqrtf(2.0f))   
#define UNUSED(x) (void)(x)

float Kalman_Filter_x(float Accel,float Gyro);		
float Complementary_Filter_x(float angle_m, float gyro_m);
float Kalman_Filter_y(float Accel,float Gyro);		
float Complementary_Filter_y(float angle_m, float gyro_m);

//��ͨ�˲�����ϵ��
typedef struct pt1Filter_s 
{
	float state;	//�˲����
	float RC;			//ģ��RC�˲��ĵ���͵���ֵ�Ļ�
	float dT;			//���ݸ���ʱ������룩
}pt1Filter_t;

//IIR �˲�����ϵ��
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

//�˲���������
typedef enum 
{
	FILTER_LPF,						//��ͨ�˲���
	FILTER_NOTCH					//��ͨ�˲���
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
