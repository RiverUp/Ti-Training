#include "Filter.hpp"
#include <math.h>

#include <stdio.h>

/**************************************************************************
函数功能：获取x轴角度简易卡尔曼滤波
入口参数：加速度获取的角度、角速度
返回  值：x轴角速度
**************************************************************************/
float dt=0.005;		  //每5ms进行一次滤波                 
float Kalman_Filter_x(float Accel,float Gyro)		
{
	//static float angle_dot;
	static float angle;
	float Q_angle=0.001; 		// 过程噪声的协方差
	float Q_gyro=0.003;			// 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	float R_angle=0.5;			// 测量噪声的协方差 既测量偏差
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	//首次协方差矩阵
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(Gyro - Q_bias) * dt; // 角度先验估计积分
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // 先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - angle;	// zk-先验估计，角度偏差
	//求卡尔曼增益
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;	// 求卡尔曼增益分母
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;			// 计算卡尔曼增益
	//更新协方差矩阵
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	//利用卡尔曼进行最优估计
	angle	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	//angle_dot   = Gyro - Q_bias; //输出值(后验估计)的微分=角速度
	return angle;
}

/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度获取的角度、角速度
返回  值：x轴角速度
**************************************************************************/
float Complementary_Filter_x(float angle_m, float gyro_m)
{
	static float angle;
	float K1 =0.02; 
	angle = K1 * angle_m+ (1-K1) * (angle + gyro_m * dt);
	return angle;
}

/**************************************************************************
函数功能：获取y轴角度简易卡尔曼滤波
入口参数：加速度获取的角度、角速度
返回  值：y轴角速度
**************************************************************************/
float Kalman_Filter_y(float Accel,float Gyro)		
{
	//static float angle_dot;
	static float angle;
	float Q_angle=0.001; 		// 过程噪声的协方差
	float Q_gyro=0.003;			// 0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	float R_angle=0.5;			// 测量噪声的协方差 既测量偏差
	char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	angle+=(Gyro - Q_bias) * dt; //先验估计
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // 先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	Angle_err = Accel - angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle	+= K_0 * Angle_err;	   //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	//angle_dot   = Gyro - Q_bias;	//输出值(后验估计)的微分=角速度
	return angle;
}

/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度获取的角度、角速度
返回  值：y轴角速度
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
//二阶巴特沃斯滤波器的带宽
#define BIQUAD_BANDWIDTH 1.9f           
//二阶巴特沃斯滤波器的品质因数Q
#define BIQUAD_Q (1.0f / sqrtf(2.0f))   
#define UNUSED(x) (void)(x)

/*
 * 函数功能：释放滤波器
 */
float nullFilterApply(void *filter, float input)
{
    UNUSED(filter);
    return input;
}

/*
 * 函数功能：低通滤波器的设置
 * filter：滤波器的相关参数
 * f_cut：截止频率
 * dT：数据更新时间隔（秒）
 */
void pt1FilterInit(pt1Filter_t *filter, uint8_t f_cut, float dT)
{
	//参考模电RC截止频率公式
	filter->RC = 1.0f / ( 2.0f * M_PI_FLOAT * f_cut );
	//数据更新时间隔（秒）
	filter->dT = dT;
}

/*
 * 函数功能：应用低通滤波
 * filter：滤波器的相关参数
 * input：新输入的数据
 */
float pt1FilterApply(pt1Filter_t *filter, float input)
{
	//dt固定时，截止频率越低，k越小，数据最终输出的变化越慢（高频去掉了）
  //截止频率固定时，dT越大，k越小，数据最终输出的变化越慢（高频去掉了）
	filter->state = filter->state + filter->dT / (filter->RC + filter->dT) * (input - filter->state);
	return filter->state;
}

/*
 * 函数功能：应用低通滤波
 * filter：滤波器的相关参数
 * input：新输入的数据
 * f_cut：截止频率
 */
float pt1FilterApply4(pt1Filter_t *filter, float input, uint8_t f_cut, float dT)
{
	//先前计算并储存参数
	if (!filter->RC) 
	{
			pt1FilterInit(filter, f_cut, dT);
	}
	filter->state = filter->state + filter->dT / (filter->RC + filter->dT) * (input - filter->state);
	return filter->state;
}

/*
 * 函数功能：双二阶滤波器的设置
 * filter：滤波器的相关参数
 * filterFreq：滤波器的中心频率
 * sampleDeltaUs：滤波器的采样系数
 * Q：滤波器的品质因数
 * filterType：滤波器的种类
 */
static void biquadFilterInit(biquadFilter_t *filter, float filterFreq, uint32_t sampleDeltaUs, float Q, biquadFilterType_e filterType)
{
    //设置采样频率
    const float sampleHz = 1 / ((float)sampleDeltaUs * 0.000001f);
    const float omega = 2 * M_PI_FLOAT * filterFreq / sampleHz;
    const float sn = sinf(omega);
    const float cs = cosf(omega);
    const float alpha = sn / (2 * Q);
		//滤波器的系数
    float b0, b1, b2, a0, a1, a2;
		//滤波器的种类
    switch (filterType) 
		{
        default:
				//低通滤波器
        case FILTER_LPF:
            b0 = (1 - cs) / 2;
            b1 = 1 - cs;
            b2 = (1 - cs) / 2;
            a0 = 1 + alpha;
            a1 = -2 * cs;
            a2 = 1 - alpha;
            break;
				//带通滤波器
        case FILTER_NOTCH:
            b0 =  1;
            b1 = -2 * cs;
            b2 =  1;
            a0 =  1 + alpha;
            a1 = -2 * cs;
            a2 =  1 - alpha;
            break;
    }
    //预先计算系数
    filter->b0 = b0 / a0;
    filter->b1 = b1 / a0;
    filter->b2 = b2 / a0;
    filter->a1 = a1 / a0;
    filter->a2 = a2 / a0;
    //零初始样本
    filter->d1 = filter->d2 = 0;
}

//双二阶滤波器的设置
void biquadFilterInitLPF(biquadFilter_t *filter, float filterFreq, uint32_t sampleDeltaUs)
{
    biquadFilterInit(filter, filterFreq, sampleDeltaUs, BIQUAD_Q, FILTER_LPF);
}

/*
 * 函数功能：应用双二阶滤波器
 * filter：滤波器的相关参数
 * input：新输入的数据
 */
float biquadFilterApply(biquadFilter_t *filter, float input)
{
    const float result = filter->b0 * input + filter->d1;
    filter->d1 = filter->b1 * input - filter->a1 * result + filter->d2;
    filter->d2 = filter->b2 * input - filter->a2 * result;
    return result;
}

