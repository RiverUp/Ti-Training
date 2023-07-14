#include "Filter.hpp"
#include <math.h>

#include <stdio.h>

/**************************************************************************
�������ܣ���ȡx��Ƕȼ��׿������˲�
��ڲ��������ٶȻ�ȡ�ĽǶȡ����ٶ�
����  ֵ��x����ٶ�
**************************************************************************/
float dt=0.005;		  //ÿ5ms����һ���˲�                 
float Kalman_Filter_x(float Accel,float Gyro)		
{
	//static float angle_dot;
	static float angle;
	float Q_angle=0.001; 		// ����������Э����
	float Q_gyro=0.003;			// ����������Э���� ����������Э����Ϊһ��һ�����о���
	float R_angle=0.5;			// ����������Э���� �Ȳ���ƫ��
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	//�״�Э�������
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(Gyro - Q_bias) * dt; // �Ƕ�������ƻ���
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // ����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - angle;	// zk-������ƣ��Ƕ�ƫ��
	//�󿨶�������
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;	// �󿨶��������ĸ
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;			// ���㿨��������
	//����Э�������
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	//���ÿ������������Ź���
	angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	//angle_dot   = Gyro - Q_bias; //���ֵ(�������)��΢��=���ٶ�
	return angle;
}

/**************************************************************************
�������ܣ�һ�׻����˲�
��ڲ��������ٶȻ�ȡ�ĽǶȡ����ٶ�
����  ֵ��x����ٶ�
**************************************************************************/
float Complementary_Filter_x(float angle_m, float gyro_m)
{
	static float angle;
	float K1 =0.02; 
	angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * dt);
	return angle;
}

/**************************************************************************
�������ܣ���ȡy��Ƕȼ��׿������˲�
��ڲ��������ٶȻ�ȡ�ĽǶȡ����ٶ�
����  ֵ��y����ٶ�
**************************************************************************/
float Kalman_Filter_y(float Accel,float Gyro)		
{
	//static float angle_dot;
	static float angle;
	float Q_angle=0.001; 		// ����������Э����
	float Q_gyro=0.003;			// 0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
	float R_angle=0.5;			// ����������Э���� �Ȳ���ƫ��
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(Gyro - Q_bias) * dt; //�������
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // ����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	   //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	//angle_dot   = Gyro - Q_bias;	//���ֵ(�������)��΢��=���ٶ�
	return angle;
}

/**************************************************************************
�������ܣ�һ�׻����˲�
��ڲ��������ٶȻ�ȡ�ĽǶȡ����ٶ�
����  ֵ��y����ٶ�
**************************************************************************/
float Complementary_Filter_y(float angle_m, float gyro_m)
{
	static float angle;
	float K1 =0.02; 
	angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * dt);
	return angle;
}

#define M_LN2_FLOAT 0.69314718055994530942f
#define M_PI_FLOAT  3.14159265358979323846f
//���װ�����˹�˲����Ĵ���
#define BIQUAD_BANDWIDTH 1.9f           
//���װ�����˹�˲�����Ʒ������Q
#define BIQUAD_Q (1.0f / sqrtf(2.0f))   
#define UNUSED(x) (void)(x)

/*
 * �������ܣ��ͷ��˲���
 */
float nullFilterApply(void *filter, float input)
{
    UNUSED(filter);
    return input;
}

/*
 * �������ܣ���ͨ�˲���������
 * filter���˲�������ز���
 * f_cut����ֹƵ��
 * dT�����ݸ���ʱ������룩
 */
void pt1FilterInit(pt1Filter_t *filter, uint8_t f_cut, float dT)
{
	//�ο�ģ��RC��ֹƵ�ʹ�ʽ
	filter->RC = 1.0f / ( 2.0f * M_PI_FLOAT * f_cut );
	//���ݸ���ʱ������룩
	filter->dT = dT;
}

/*
 * �������ܣ�Ӧ�õ�ͨ�˲�
 * filter���˲�������ز���
 * input�������������
 */
float pt1FilterApply(pt1Filter_t *filter, float input)
{
	//dt�̶�ʱ����ֹƵ��Խ�ͣ�kԽС��������������ı仯Խ������Ƶȥ���ˣ�
  //��ֹƵ�ʹ̶�ʱ��dTԽ��kԽС��������������ı仯Խ������Ƶȥ���ˣ�
	filter->state = filter->state + filter->dT / (filter->RC + filter->dT) * (input - filter->state);
	return filter->state;
}

/*
 * �������ܣ�Ӧ�õ�ͨ�˲�
 * filter���˲�������ز���
 * input�������������
 * f_cut����ֹƵ��
 */
float pt1FilterApply4(pt1Filter_t *filter, float input, uint8_t f_cut, float dT)
{
	//��ǰ���㲢�������
	if (!filter->RC) 
	{
			pt1FilterInit(filter, f_cut, dT);
	}
	filter->state = filter->state + filter->dT / (filter->RC + filter->dT) * (input - filter->state);
	return filter->state;
}

/*
 * �������ܣ�˫�����˲���������
 * filter���˲�������ز���
 * filterFreq���˲���������Ƶ��
 * sampleDeltaUs���˲����Ĳ���ϵ��
 * Q���˲�����Ʒ������
 * filterType���˲���������
 */
static void biquadFilterInit(biquadFilter_t *filter, float filterFreq, uint32_t sampleDeltaUs, float Q, biquadFilterType_e filterType)
{
    //���ò���Ƶ��
    const float sampleHz = 1 / ((float)sampleDeltaUs * 0.000001f);
    const float omega = 2 * M_PI_FLOAT * filterFreq / sampleHz;
    const float sn = sinf(omega);
    const float cs = cosf(omega);
    const float alpha = sn / (2 * Q);
		//�˲�����ϵ��
    float b0, b1, b2, a0, a1, a2;
		//�˲���������
    switch (filterType) 
		{
        default:
				//��ͨ�˲���
        case FILTER_LPF:
            b0 = (1 - cs) / 2;
            b1 = 1 - cs;
            b2 = (1 - cs) / 2;
            a0 = 1 + alpha;
            a1 = -2 * cs;
            a2 = 1 - alpha;
            break;
				//��ͨ�˲���
        case FILTER_NOTCH:
            b0 =  1;
            b1 = -2 * cs;
            b2 =  1;
            a0 =  1 + alpha;
            a1 = -2 * cs;
            a2 =  1 - alpha;
            break;
    }
    //Ԥ�ȼ���ϵ��
    filter->b0 = b0 / a0;
    filter->b1 = b1 / a0;
    filter->b2 = b2 / a0;
    filter->a1 = a1 / a0;
    filter->a2 = a2 / a0;
    //���ʼ����
    filter->d1 = filter->d2 = 0;
}

//˫�����˲���������
void biquadFilterInitLPF(biquadFilter_t *filter, float filterFreq, uint32_t sampleDeltaUs)
{
    biquadFilterInit(filter, filterFreq, sampleDeltaUs, BIQUAD_Q, FILTER_LPF);
}

/*
 * �������ܣ�Ӧ��˫�����˲���
 * filter���˲�������ز���
 * input�������������
 */
float biquadFilterApply(biquadFilter_t *filter, float input)
{
    const float result = filter->b0 * input + filter->d1;
    filter->d1 = filter->b1 * input - filter->a1 * result + filter->d2;
    filter->d2 = filter->b2 * input - filter->a2 * result;
    return result;
}

