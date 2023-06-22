#include "control_entry.h"

#define SAMPLE_INTERVAL 10//10ms²ÉÑù¼ä¸ô

float target_encoder_value=-20;
float Velocity_Kp=330,Velocity_Ki=4;

void control_entry()
{
	init_encoder_left();
	init_encoder_right();
	init_motor();
	rt_timer_t control_timer=rt_timer_create("Control",control,RT_NULL,SAMPLE_INTERVAL,RT_TIMER_FLAG_PERIODIC);
	if(control_timer!=RT_NULL)
	{
		rt_timer_start(control_timer);
	}
	while(1){}
}


void control()
{
	int encoder_left,encoder_right;
	encoder_left=read_encoder(1); 
	encoder_right=read_encoder(0);	
	int pwma=veloity_left(encoder_left)+turn();
	int pwmb=veloity_right(encoder_right)-turn();
	pwma=limit_pwm(pwma,8000,-8000);
	pwmb=limit_pwm(pwmb,8000,-8000);
	set_pwm(pwma,pwmb);
}

int veloity_left(int current_encoder_value)
{
	static float velocity,encoder_bias,current_bias;
	static float encoder_integral;
	current_bias=target_encoder_value-current_encoder_value;
	encoder_bias=0.84*encoder_bias+0.16*current_bias;
	encoder_integral+=encoder_bias;
	if(encoder_integral>10000)
	{
		encoder_integral = 10000;
	}
	else if (encoder_integral < -10000)
	{
    encoder_integral = -10000;
  }
	
	velocity=encoder_bias*Velocity_Kp+encoder_integral*Velocity_Ki;
	
	return velocity;
}

int veloity_right(int current_encoder_value)
{
	static float velocity,encoder_bias,current_bias;
	static float encoder_integral;
	current_bias=target_encoder_value-current_encoder_value;
	encoder_bias=0.84*encoder_bias+0.16*current_bias;
	encoder_integral+=encoder_bias;
	if(encoder_integral>10000)
	{
		encoder_integral = 10000;
	}
	else if (encoder_integral < -10000)
	{
    encoder_integral = -10000;
  }
	
	velocity=encoder_bias*Velocity_Kp+encoder_integral*Velocity_Ki;
	
	return velocity;
}

int turn()
{
	return 0;
}