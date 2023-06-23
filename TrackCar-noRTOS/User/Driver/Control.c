#include "Control.h"

float target_encoder_value=10;
float Velocity_Kp=20,Velocity_Ki=5;

void Control()
{
	int left=read_encoder(1);
	int right=read_encoder(0);
	
	int turn_value=turn();
	
	int pwma=velocity_left(left)+turn_value;
	int pwmb=velocity_right(right)-turn_value;
	pwma=limit_pwm(pwma,8000,-8000);
	pwmb=limit_pwm(pwmb,8000,-8000);
	set_pwm(pwma,pwmb);
	
}

int velocity_left(int current_encoder_value)
{
	static float velocity,last_bias,current_bias;
	current_bias=target_encoder_value-current_encoder_value;
	velocity+=current_bias*Velocity_Kp+(current_bias-last_bias)*Velocity_Ki;
	last_bias=current_bias;
	
	return velocity;
}

int velocity_right(int current_encoder_value)
{
	static float velocity,last_bias,current_bias;
	static float encoder_integral;
	current_bias=target_encoder_value-current_encoder_value;
	
	velocity+=current_bias*Velocity_Kp+(current_bias-last_bias)*Velocity_Ki;
	last_bias=current_bias;
	
	return velocity;
}

int turn()
{
	bool t1=getTCRTValue(1);
	bool t2=getTCRTValue(2);
	bool t3=getTCRTValue(3);
	static int turnPwm=0;
	
	if(t1|t2|t3==0)
	{
		turnPwm=0;
	}
	else
	{
		if(t2==1)
		{
			turnPwm=0;
		}
		if(t3==1)
		{
			if(turnPwm>2000)
			{
				turnPwm=2000;
			}
			else
			{
				turnPwm+=250;
			}
		}
		if(t1==1)
		{
			if(turnPwm<-2000)
			{
				turnPwm=-2000;
			}
			else
			{
				turnPwm-=250;
			}
		}
	}
	
	
	return turnPwm;
}

void init_control()
{
  GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	
	Timer32_initModule(TIMER32_BASE,TIMER32_PRESCALER_1,TIMER32_32BIT,TIMER32_PERIODIC_MODE);
	Interrupt_enableInterrupt(INT_T32_INT1);
	Timer32_setCount(TIMER32_BASE,120000);//10ms
  Timer32_enableInterrupt(TIMER32_BASE);
  Timer32_startTimer(TIMER32_BASE, false);
}