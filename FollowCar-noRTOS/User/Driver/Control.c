#include "Control.h"

#define MaxTurn1 2700
#define MaxTurn2 2700
#define ChangeIntervalTurn1 300
#define ChangeIntervalTurn2 600

#define StraightV 80
#define TurnV1 40
#define TurnV2 10

float target_encoder_value = 40;
float Velocity_Kp = 100, Velocity_Ki;
// float Velocity_Kp=10,Velocity_Ki=3000;
int turnPwm = 0;
float adc;
bool t1, t2, t3, t4, t5;

int sum_EnL, sum_EnR, EnCounter;

void Control()
{

	int left = read_encoder(1);
	int right = read_encoder(0);

	int velocity_value = velocity(left, right);
	int turn_value = turn();

	// adc=read_TCRT();

	//	int pwma=velocity_left(left)+turn_value;
	//	int pwmb=velocity_right(right)-turn_value;

	int pwma = velocity_value + turn_value;
	int pwmb = velocity_value - turn_value;
	pwma = limit_pwm(pwma, 8000, -8000);
	pwmb = limit_pwm(pwmb, 8000, -8000);

	//set_pwm(pwma,pwmb);
}

int velocity(int current_encoder_left, int current_encoder_right)
{
	static float current_bias, encoder_bias, encoder_integral;
	current_bias = target_encoder_value - (current_encoder_left + current_encoder_right) / 2;
	encoder_bias = 0.84 * encoder_bias + current_bias * 0.16;
	encoder_integral += encoder_bias;
	encoder_integral = limit_pwm(encoder_integral, 4500, -4500);

	int velocity = encoder_bias * Velocity_Kp + encoder_integral * Velocity_Ki;

	return velocity;
}

int velocity_left(int current_encoder_value)
{
	static float velocity, last_bias, current_bias;
	current_bias = target_encoder_value - current_encoder_value;
	velocity += current_bias * Velocity_Kp + (current_bias - last_bias) * Velocity_Ki;
	last_bias = current_bias;

	return velocity;
}

int velocity_right(int current_encoder_value)
{
	static float velocity, last_bias, current_bias;
	static float encoder_integral;
	current_bias = target_encoder_value - current_encoder_value;

	velocity += current_bias * Velocity_Kp + (current_bias - last_bias) * Velocity_Ki;
	last_bias = current_bias;

	return velocity;
}

int turn()
{
	// t1=1;
	t1 = getTCRTValue(1);
	t2 = getTCRTValue(2);
	t3 = getTCRTValue(3);
	t4 = getTCRTValue(4);
	t5 = getTCRTValue(5);

	if (t1 + t2 + t3 + t4 + t5 == 0)
	{
		turnPwm = 0;
		target_encoder_value = StraightV;
	}
	else
	{
		if (t3 == 1)
		{
			turnPwm = 0;
			target_encoder_value = StraightV;
		}
		if (t1 == 1)
		{
			target_encoder_value = TurnV2;
			if (turnPwm > MaxTurn2)
			{
				turnPwm = MaxTurn2;
			}
			else
			{
				turnPwm += ChangeIntervalTurn2;
			}
		}
		if (t2 == 1)
		{
			target_encoder_value = TurnV1;
			if (turnPwm > MaxTurn1)
			{
				turnPwm = MaxTurn1;
			}
			else
			{
				turnPwm += ChangeIntervalTurn1;
			}
		}
		if (t4 == 1)
		{
			target_encoder_value = TurnV1;
			if (turnPwm < -MaxTurn1)
			{
				turnPwm = -MaxTurn1;
			}
			else
			{
				turnPwm -= ChangeIntervalTurn1;
			}
		}
		if (t5 == 1)
		{
			target_encoder_value = TurnV2;
			if (turnPwm < -MaxTurn2)
			{
				turnPwm = -MaxTurn2;
			}
			else
			{
				turnPwm -= ChangeIntervalTurn2;
			}
		}
	}

	return turnPwm;
}

void init_control()
{
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

	Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
	Interrupt_enableInterrupt(INT_T32_INT1);
	Timer32_setCount(TIMER32_0_BASE, 120000); // 10ms
	Timer32_enableInterrupt(TIMER32_0_BASE);
	Timer32_startTimer(TIMER32_0_BASE, false);
}
