#include "Control.hpp"
#include "drv_ADC.hpp"
#include "drv_Encoder.hpp"
#include "drv_MPU6050.hpp"
#include "drv_Key.hpp"
#include "drv_LED.hpp"
#include "GUI.hpp"
#include <stack>
using namespace std;

#define APB2TIMERCLK 72000000
#define AIN1 PDout(4)
#define AIN2 PCout(12)
#define BIN1 PDout(11)
#define BIN2 PDout(10)
#define LEFTTURN 1
#define RIGHTTURN 2
// 识别到路口至正好行驶到路口上进行旋转的延时时间
int StopDelayTimes = 110;

// 在第几个路口停
int StopCrossNum;
// 识别到的病房号
int IdentifiedNum;
// 是否为近端病房
bool CloseWard;
// 延时计数
int StopDelayCount;
// 标识开始延时
bool ReadyStopFlag;

bool Mark180Flag;

float TargetYaw;
float TargetVelocity = 30;
bool TrackFlag = true;
bool Turn180Flag;
bool TurnLeft90Flag;
bool TurnRight90Flag;
bool CrossFlag;
bool GoBackFlag;
int GoBackCount;
int GoBackTimes = 200;
bool ArrivedFlag;
bool ReturnFlag;
bool JudgingFlag;
// 数字识别转向方向
int TurnSignal;
int JudgingCount;
int JudgingTimes = 400;
int ArrivedNum;
int CrossNum = 0;
int ArrivedCount;
bool PassCrossFlag;
int PassCrossTimes = 400;
int PassCrossCount;
int ArrivedTimes;

int Trace[5];
int TracePtr;
int TraceBit;

extern "C" void TIM8_UP_IRQHandler(void)
{
    if (TIM_GetFlagStatus(TIM8, TIM_FLAG_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
        if (getInitializationCompleted() == false)
        {
            return;
        }
        Get_Angle(Way_Angle);
        Encoder_Left = -Read_Encoder(4);
        Encoder_Right = -Read_Encoder(3);
        Key();
        if (Flag_Stop)
        {
            Led_Flash(0);
        }
        else
        {
            Led_Flash(100);
        }

        // 路口判断
        if (CrossNum == StopCrossNum && CrossFlag && !ReturnFlag)
        {
            // 不是近端病房就停下来识别,第三个路口也不不识别，直接左转
            if (!CloseWard && StopCrossNum != 3)
            {
                // Flag_Stop = true;
                GoBackFlag = true;
            }
            // 是近端病房就开始停止延时
            else
            {
                ReadyStopFlag = true;
            }
            CrossFlag = false;
        }
        GoBack();
        if (CrossFlag && ReturnFlag)
        {
            if (TracePtr - 1 >= 0)
            {
                ReadyStopFlag = true;
            }
        }
        // openmv长时间识别不出就算直行
        if (JudgingFlag)
        {
            JudgingCount++;
            if (JudgingCount >= JudgingTimes)
            {
                TargetVelocity = 30;
                if (CrossNum == 2)
                {
                    Flag_Stop = false;
                    JudgingFlag = false;
                    JudgingCount = 0;
                    StopCrossNum = 3;
                    PassCrossCount = 0;
                    PassCrossFlag = true;
                    StopDelayTimes = 100;
                }
                if (CrossNum == 4)
                {
                    Flag_Stop = false;
                    JudgingFlag = false;
                    JudgingCount = 0;
                    StopCrossNum = 6;
                    TargetVelocity = 0;
                    Turn180Flag = true;
                    Trace[TracePtr - 1] = RIGHTTURN;
                }
            }
        }
        // 识别路口的延时
        if (PassCrossFlag)
        {
            if (PassCrossCount > PassCrossTimes)
            {
                PassCrossCount = 0;
                PassCrossFlag = false;
            }
            else
            {
                PassCrossCount++;
            }
        }
        // 开始延时
        if (ReadyStopFlag)
        {
            StopDelayCount++;
            if (StopDelayCount == StopDelayTimes)
            {
                // 停车+复位
                TargetVelocity = 0;
                StopDelayCount = 0;
                ReadyStopFlag = false;
                // 转向标识设置
                TrackFlag = false;
                // 近端病房
                if (!ReturnFlag && CloseWard)
                {
                    if (IdentifiedNum == 1)
                        TurnLeft90Flag = true;
                    if (IdentifiedNum == 2)
                        TurnRight90Flag = true;
                }
                else if (!ReturnFlag && !CloseWard)
                {
                    // 判别左转或遇到第3个十字路口
                    if (TurnSignal == 11)
                        TurnLeft90Flag = true;
                    if (TurnSignal == 12)
                        TurnRight90Flag = true;
                    if (StopCrossNum == 3)
                    {
                        // 下一个路口要判断数字
                        StopCrossNum = 4;
                        TurnLeft90Flag = true;
                    }
                    TurnSignal = 0;
                }
                // 回程的转向控制
                else if (ReturnFlag)
                {
                    TracePtr--;
                    if (TracePtr >= 0)
                    {
                        int direction = Trace[TracePtr];
                        if (direction == LEFTTURN)
                            TurnRight90Flag = true;
                        else
                        {
                            TurnLeft90Flag = true;
                        }
                    }
                }
            }
        }
        if (TurnLeft90Flag)
        {
            if (Yaw > 170)
            {
                TargetYaw = -Yaw + 90;
            }
            else
            {
                TargetYaw = Yaw + 90;
            }
            TurnLeft90Flag = false;
            Flag_Left = true;
            if (!ReturnFlag)
            {
                Trace[TracePtr++] = LEFTTURN;
            }
        }
        if (TurnRight90Flag)
        {
            if (Yaw < -170)
            {
                TargetYaw = -Yaw - 90;
            }
            else
            {
                TargetYaw = Yaw - 90;
            }
            TurnRight90Flag = false;
            Flag_Right = true;
            if (!ReturnFlag)
            {
                Trace[TracePtr++] = RIGHTTURN;
            }
        }
        if (Turn180Flag)
        {
            ArrivedCount++;
            if (ArrivedCount == 50)
            {
                TargetVelocity = 0;
                if (Yaw < 0)
                    TargetYaw = Yaw + 180;
                else
                    TargetYaw = Yaw - 180;
                Turn180Flag = false;
                Flag_Left = true;
                ArrivedCount = 0;
                //								Mark180Flag=true;
            }
        }
        if (Flag_Left == 1 || Flag_Right == 1)
        {
            // 转向后恢复前进
            if (myabs(Yaw - TargetYaw) < 15)
            {
                Flag_Left = 0;
                Flag_Right = 0;
                Track_Bias = 0;
                Set_Pwm(0, 0);
                TargetVelocity = 30;
                TrackFlag = true;
                if (ArrivedFlag)
                {
                    TargetVelocity = 0;
                }
                StopDelayTimes = 110;
                if (ArrivedNum == 2)
                {
                    ArrivedNum = 0;
                    Flag_Stop = true;
                }
            }
        }

        Get_Velocity_Form_Encoder(Encoder_Left, Encoder_Right);

        Velocity_Pwm = Velocity(Encoder_Left, Encoder_Right);
        Rotate_Turn_Pwm = RotateTurn(Gyro_Turn);
        Track_Turn_PWM = TrackTurn(Track_Bias);

        Motor_Left = Velocity_Pwm - Rotate_Turn_Pwm + Track_Turn_PWM;
        Motor_Right = Velocity_Pwm + Rotate_Turn_Pwm - Track_Turn_PWM;
        Voltage = Get_battery_volt();
        Motor_Left = PWM_Limit(Motor_Left, 6900, -6900);
        Motor_Right = PWM_Limit(Motor_Right, 6900, -6900);
        if (Pick_Up(Acceleration_Z, Angle_Balance, Encoder_Left, Encoder_Right))
        {
            Flag_Stop = 1;
        }
        if (Put_Down(Angle_Balance, Encoder_Left, Encoder_Right))
        {
            Flag_Stop = 0;
        }
        if (Turn_Off(Angle_Balance, Voltage) == 0)
            Set_Pwm(Motor_Left, Motor_Right);
    }
}
void GoBack()
{
    if (GoBackFlag)
    {
        TargetVelocity = -20;
				TrackFlag=false;
        GoBackCount++;
        if (GoBackCount >= GoBackTimes)
        {
            TargetVelocity = 0;
            GoBackCount = 0;
            GoBackFlag = false;
            JudgingFlag = true;
            PassCrossCount = 0;
            PassCrossFlag = true;
						TrackFlag=true;
        }
    }
}

/*
 * 函数功能：直立PD控制
 * 入口参数：Angle:角度；Gyro：角速度
 * 返回  值：balance：直立控制PWM
 */
int Balance(float Angle, float Gyro)
{
    float angle_bias, gyro_bias;
    int balance;
    angle_bias = Middle_angle - Angle;
    gyro_bias = 0 - Gyro;
    balance = -Balance_Kp / 100 * angle_bias - gyro_bias * Balance_Kd / 100;
    return balance;
}
/*
 * 函数功能：速度控制PWM
 * 入口参数：encoder_left：左轮编码器读数；encoder_right：右轮编码器读数
 * 返回  值：速度控制PWM
 */
int Velocity(int encoder_left, int encoder_right)
{
    static float velocity, encoder_least, encoder_bias;
    static float encoder_integral, target_velocity;
    encoder_least = TargetVelocity - (encoder_left + encoder_right);
    // 低通滤波
    encoder_bias *= 0.84;
    encoder_bias += encoder_least * 0.16;
    encoder_integral += encoder_bias;
    if (encoder_integral > 10000)
    {
        encoder_integral = 10000;
    }
    else if (encoder_integral < -10000)
    {
        encoder_integral = -10000;
    }
    velocity = -encoder_bias * Velocity_Kp / 100 - encoder_integral * Velocity_Ki / 100; // seems have problems
    if (Turn_Off(Angle_Balance, Voltage) == 1 || Flag_Stop == 1)
        encoder_integral = 0;
    return velocity;
}
/*
 * 函数功能：转向控制
 * 入口参数：Z轴陀螺仪
 * 返回  值：转向控制PWM
 */
int RotateTurn(float gyro)
{
    static float Turn_Target, turn, Turn_Amplitude = 54;
    float Kp = Rotate_Turn_Kp, Kd;
    if (1 == Flag_Left)
    {
        Turn_Target = -Turn_Amplitude / Flag_velocity;
    }
    else if (1 == Flag_Right)

    {
        Turn_Target = Turn_Amplitude / Flag_velocity;
    }
    else
    {
        Turn_Target = 0;
    }
    if (1 == Flag_front || 1 == Flag_back)
        Kd = Rotate_Turn_Kd;
    else
        Kd = 0;

    turn = Turn_Target * Kp / 100 + gyro * Kd / 100;
    return turn;
}

int TrackTurn(float bias)
{
    if (TrackFlag)
    {
        static float general_bias, turn, last_bias;
        general_bias = 0.84 * bias + 0.16 * last_bias;
        last_bias = bias;

        turn = general_bias * Track_Turn_Kp / 100;

        return turn;
    }
    else
    {
        return 0;
    }
}

/*
 * 函数功能：赋值给PWM寄存器
 * 入口参数：左轮PWM、右轮PWM
 * 返回  值：无
 */
void Set_Pwm(int motor_left, int motor_right)
{
    int pwmb, pwma;
    if (motor_left > 0)
    {
        BIN1 = 0;
        BIN2 = 1;
    }
    else
    {
        BIN1 = 1;
        BIN2 = 0;
    }
    pwmb = myabs(motor_left);
    TIM_SetCompare3(TIM1, pwmb);
    if (motor_right > 0)
    {
        AIN1 = 0;
        AIN2 = 1;
    }
    else
    {
        AIN1 = 1;
        AIN2 = 0;
    }
    pwma = myabs(motor_right);
    TIM_SetCompare4(TIM1, pwma);
}
/*
 * 函数功能：限制PWM赋值
 * 入口参数：IN：输入参数  max：限幅最大值  min：限幅最小值
 * 返回  值：限幅后的值
 */
int PWM_Limit(int IN, int max, int min)
{
    int OUT;
    if (IN > max)
    {
        OUT = max;
    }
    else if (IN < min)
    {
        OUT = min;
    }
    else
    {
        OUT = IN;
    }
    return OUT;
}
/*
 * 函数功能：按键修改小车运行状态
 * 入口参数：无
 * 返回  值：无
 */
void Key(void)
{
    u8 temp;
    temp = click_N_Double(50);
    if (temp == 1)
    {
        Track_Bias = 0;
        if (ArrivedFlag)
        {
            ArrivedFlag = false;
            TargetVelocity = 30;
        }
        else
        {
            TurnRight90Flag = TurnLeft90Flag = 0;
            Flag_Right = false;
            Flag_Left = false;
            CrossNum = 0;
            Flag_Stop = !Flag_Stop;
        }
    }
}
/*
 * 函数功能：异常关闭电机
 * 入口参数：angle：小车倾角；voltage：电压
 * 返回  值：1：异常  0：正常
 */
u8 Turn_Off(float angle, int voltage)
{
    u8 temp;
    if (angle < -40 || angle > 40 || Flag_Stop == 1 || voltage < 1170)
    {
        temp = 1;
        AIN1 = 0;
        AIN2 = 0;
        BIN1 = 0;
        BIN2 = 0;
    }
    else
    {
        temp = 0;
    }
    return temp;
}

/*
 * 函数功能：绝对值函数
 * 入口参数：a：需要计算绝对值的数
 * 返回  值：无符号整型
 */
int myabs(int a)
{
    int temp;
    if (a < 0)
    {
        temp = -a;
    }
    else
    {
        temp = a;
    }
    return temp;
}
/*
 * 函数功能：检测小车是否被拿起
 * 入口参数：Acceleration：z轴加速度；Angle：平衡的角度；encoder_left：左编码器计数；encoder_right：右编码器计数
 * 返回  值：1:小车被拿起  0：小车未被拿起
 */
int Pick_Up(float Acceleration, float Angle, int encoder_left, int encoder_right)
{
    static u16 flag, count0, count1, count2;
    if (flag == 0)
    {
        // 静止
        if (myabs(encoder_left) + myabs(encoder_right) < 30)
        {
            count0++;
        }
        else
        {
            count0 = 0;
        }
        if (count0 > 10)
        {
            flag = 1, count0 = 0;
        }
    }
    // 在0度附近被拿起
    if (flag == 1)
    {
        if (++count1 > 200)
        {
            count1 = 0;
            flag = 0;
        }
        if (Acceleration > 26000 && (Angle > (-20 + Middle_angle) && Angle < (20 + Middle_angle)))
        {
            flag = 2;
        }
    }
    // 正反馈达到最大转速
    if (flag == 2)
    {
        if (++count2 > 200)
        {
            count2 = 0;
            flag = 0;
        }
        if (myabs(encoder_left + encoder_right) > 70)
        {
            flag = 0;
            return 1;
        }
    }
    return 0;
}
/*
 * 函数功能：检测小车是否被放下
 * 入口参数：平衡角度；左编码器读数；右编码器读数
 * 返回  值：1：小车放下   0：小车未放下
 */
int Put_Down(float Angle, int encoder_left, int encoder_right)
{
    static int flag, count;
    if (Flag_Stop == 0)
    {
        return 0;
    }
    if (flag == 0)
    {
        if (Angle > (-10 + Middle_angle) && Angle > (10 + Middle_angle) && encoder_right == 0 && encoder_left == 0)
        {
            flag = 1;
        }
    }
    if (flag == 1)
    {
        if (++count > 50)
        {
            count = 0;
            flag = 0;
        }
        if (encoder_left > 3 && encoder_right > 3 && encoder_left < 40 && encoder_right < 40)
        {
            flag = 0;
            count = 0;
            return 1;
        }
    }
    return 0;
}
/*
 * 函数功能：编码器读数转换为速度（mm/s）
 * 入口参数：无
 * 返回  值：无
 */
void Get_Velocity_Form_Encoder(int encoder_left, int encoder_right)
{
    float rotation_speed_L, rotation_speed_R;
    rotation_speed_L = encoder_left * Control_Frequency / EncoderMultiples / Reduction_Ratio / Encoder_precision;
    Velocity_Left = rotation_speed_L * PI * Diameter_67;
    rotation_speed_R = encoder_right * Control_Frequency / EncoderMultiples / Reduction_Ratio / Encoder_precision;
    Velocity_Right = rotation_speed_R * PI * Diameter_67;
}
/*
 * 函数功能：选择小车运行模式
 * 入口参数：encoder_left：左编码器读数  encoder_right：右编码器读数
 * 返回  值：无
 */
void Choose(int encoder_left, int encoder_right);

void init_drv_ControlSystem()
{
    RCC->APB2ENR |= (1 << 13);
    // 需要问一下chatgpt
    //  os_delay(1e-2);
    TIM8->PSC = (APB2TIMERCLK / 1000000) - 1;
    TIM8->ARR = 1e6 / 200 - 1;
    TIM8->DIER = 1 << 0;
    TIM8->CR1 = 1 << 0;
    NVIC_SetPriority(TIM8_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM8_UP_IRQn);
}

int Set_Postion_PID_L(int position, int target)
{
    static float bias, pwm, integral_bias, last_bias;
    bias = target - position;
    integral_bias += bias;
    if (integral_bias > 3000)
    {
        integral_bias = 3000;
    }
    else if (integral_bias < -3000)
    {
        integral_bias = -3000;
    }
    pwm = Position_KP * bias + Position_KI * integral_bias + Position_KD * (bias - last_bias);
    last_bias = bias;
    return pwm;
}

int Set_Postion_PID_R(int position, int target)
{
    static float bias, pwm, integral_bias, last_bias;
    bias = target - position;
    integral_bias += bias;
    if (integral_bias > 3000)
    {
        integral_bias = 3000;
    }
    else if (integral_bias < -3000)
    {
        integral_bias = -3000;
    }
    pwm = Position_KP * bias + Position_KI * integral_bias + Position_KD * (bias - last_bias);
    last_bias = bias;
    return pwm;
}
// 增量式PID：P是D，I是P；
int Set_Incremental_PI_L(int incremental, int target)
{
    static float bias, pwm, last_bias;
    bias = target - incremental;
    pwm += Velocity_KP * (bias - last_bias) + Velocity_KI * bias;
    last_bias = bias;
    return pwm;
}
int Set_Incremental_PI_R(int incremental, int target)
{
    static float bias, pwm, last_bias;
    bias = target - incremental;
    pwm += Velocity_KP * (bias - last_bias) + Velocity_KI * bias;
    last_bias = bias;
    return pwm;
}
