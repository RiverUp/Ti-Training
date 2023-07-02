#include "Control.h"

int MaxTurn1= 1500;
int MaxTurn2 =2000;
int ChangeIntervalTurn1 =250;   
int ChangeIntervalTurn2 =1800;

int StraightV= 11;
int TurnV1 =11;
int TurnV2 =10;


int DecelerationTimes=0;

bool FollowFlag=true;
bool SetFlag=false;
bool StopFlag;
bool CrossFlag;			//遇到岔道
bool OverTakeFlag;
bool ReadyOverTakeFlag;
int OverTakeCount;
int CrossAccelerateCount;
int CrossAccelerateTimes=200;
bool CrossRushOrNot=true;
int CrossNums;			//遇到的岔道个数
bool DecelerationFlag1;
bool DecelerationFlag2;
int DecelerationCounter;

int Mission=0;

float target_encoder_value=40;
float Velocity_Kp=100,Velocity_Ki=22;
float Distance_Kp=2.5;
int turnPwm=0;
int distancePwm;
float adc;
bool t1,t2,t3,t4,t5;
bool last_t1,last_t2,last_t3,last_t4,last_t5;

int sum_EnL,sum_EnR,EnCounter;

void Control()
{
	
		
	int left=read_encoder(1);
	int right=read_encoder(0);
	
	int velocity_value=velocity(left,right);
	int turn_value=turn2();
	
	
	int pwma, pwmb;
	if(Mission==1||Mission==2||(Mission==3&&FollowFlag)||Mission==4||Mission==5)
	{
		int distance_value=distance(HCSRCountValue);
		if(Mission==2&&HCSRCountValue<120&&!SetFlag)
		{
			SetFlag=true;
			MaxTurn1=1500;
			MaxTurn2=2000;
			ChangeIntervalTurn1=250;
			ChangeIntervalTurn2=1800;
			StraightV=11;
			TurnV1=11;
			TurnV2=10;
			Velocity_Kp=100;
			Velocity_Ki=22;
			CrossAccelerateTimes=200;
			DecelerationTimes=0;	
		}
		target_encoder_value+=distance_value;
		target_encoder_value=limit_pwm(target_encoder_value,StraightV,StraightV-5);
		pwma = velocity_value + turn_value-125;
		pwmb = velocity_value - turn_value+125;
	}
	if(Mission==3&&!FollowFlag)
	{
		pwma = velocity_value + turn_value-125;
		pwmb = velocity_value - turn_value+125;
	}
	
		
//		if(CrossFlag)
//		{
//			set_pwm(velocity_value+1875,velocity_value+2125);
//			delay_ms(CrossAccelerateTimes);
//			CrossFlag=false;
//		}
//	}
//	if(Mission==2)
//	{
//		int distance_value=distance(HCSRCountValue);
//		target_encoder_value+=distance_value;
//		target_encoder_value=limit_pwm(target_encoder_value,StraightV,StraightV-5);
//		pwma = velocity_value + turn_value-125;
//		pwmb = velocity_value - turn_value+125;
//		
//	}
	pwma=limit_pwm(pwma,8000,-8000);
	pwmb=limit_pwm(pwmb,8000,-8000);
  
	set_pwm(pwma,pwmb);
	
}

int velocity(int current_encoder_left,int current_encoder_right)
{
	static float current_bias,encoder_bias,encoder_integral;
	current_bias=target_encoder_value-(current_encoder_left+current_encoder_right)/2;
	encoder_bias=0.84*encoder_bias+current_bias*0.16;
	encoder_integral+=encoder_bias;
	encoder_integral=limit_pwm(encoder_integral,4500,-4500);
	
	int velocity=encoder_bias*Velocity_Kp+encoder_integral*Velocity_Ki;
	
	return velocity;
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

int distance(int current_dis)
{
	static float current_bias,distance_bias;
	current_bias=current_dis-HCSRStandardValue;
	distance_bias=0.84*distance_bias+current_bias*0.16;

	distancePwm=distance_bias*Distance_Kp;

	return distancePwm;
}

int turn2()
{
	int gray_state;
	t1=getTCRTValue(1);
  t2=getTCRTValue(2);
	t3=getTCRTValue(3);
	t4=getTCRTValue(4);
	t5=getTCRTValue(5);
	
	gray_state=t1*10000+t2*1000+t3*100+t4*10+t5*1;
	
	switch(gray_state)
	{
		case 0:     
			turnPwm=0;
			target_encoder_value=StraightV;
			break;//00000
		case 1:     
			target_encoder_value=TurnV2;
			DecelerationFlag2=true;
			if(turnPwm<-MaxTurn2)
			{
				turnPwm=-MaxTurn2;
			}
			else
			{
				turnPwm-=ChangeIntervalTurn2;
			}
			if(CrossFlag&CrossRushOrNot)
			{
				turnPwm=0;
			}
			if(!CrossRushOrNot&&CrossAccelerateCount>CrossAccelerateTimes/8)
			{
				if(turnPwm<-MaxTurn1)
				{
					turnPwm=-MaxTurn1;
				}
				else
				{
					turnPwm-=2500;
				}
			}
			break;//00001
		case 11:
			target_encoder_value=TurnV1;
			if(!CrossFlag)
			{
				DecelerationFlag1=true;
			}
			
			if(turnPwm<-MaxTurn1)
			{
				turnPwm=-MaxTurn1;
			}
			else
			{
				turnPwm-=2500;
			}
			break;//00011
		case 10:  
			target_encoder_value=TurnV1;
			if(!CrossFlag)
			{
				DecelerationFlag1=true;
			}
			
			if(turnPwm<-MaxTurn1)
			{
				turnPwm=-MaxTurn1;
			}
			else
			{
				turnPwm-=ChangeIntervalTurn1;
			}
			if(!CrossRushOrNot&&CrossAccelerateCount>CrossAccelerateTimes/5)
			{
				if(turnPwm<-MaxTurn1)
				{
					turnPwm=-MaxTurn1;
				}
				else
				{
					turnPwm-=800;
				}
			}
			break;//00010
		//岔口
		case 110: 
		case 1100:
		case 1010:
			if(CrossFlag&&CrossRushOrNot)
			{
				turnPwm=0;
//				if(turnPwm<-800)
//				{
//					turnPwm=-800;
//				}
//				else
//				{
//					turnPwm-=100;
//				}
			}
			if(!CrossRushOrNot&&CrossAccelerateCount>CrossAccelerateTimes/8)
			{
				if(turnPwm<-MaxTurn1)
				{
					turnPwm=-MaxTurn1;
				}
				else
				{
					turnPwm-=800;
				}
			}
			break;//00110
		case 100:   
			turnPwm=0;
			target_encoder_value=StraightV;
			break;//00100
		case 1000:  
			target_encoder_value=TurnV1;
			if(!CrossFlag)
			{
				DecelerationFlag1=true;
			}
			
			if(turnPwm>MaxTurn1)
			{
				turnPwm=MaxTurn1;
			}
			else
			{
				turnPwm+=ChangeIntervalTurn1;
			}			
			break;//01000
		case 11000: 
			target_encoder_value=TurnV1;
			if(!CrossFlag)
			{
				DecelerationFlag1=true;
			}
			
			if(turnPwm>MaxTurn1)
			{
				turnPwm=MaxTurn1;
			}
			else
			{
				turnPwm+=ChangeIntervalTurn1;
			}
			if(OverTakeFlag)
			{
				turnPwm=0;
			}
			break;//11000
		case 10000: 
			target_encoder_value=TurnV2;
			DecelerationFlag2=true;
			if(turnPwm>MaxTurn2)
			{
				turnPwm=MaxTurn2;
			}
			else
			{
				turnPwm+=ChangeIntervalTurn2;
			}
			if(!(CrossFlag&&CrossRushOrNot))
			{
				if(turnPwm<800)
				{
					turnPwm=800;
				}
				else
				{
					turnPwm+=80;
				}
			}
			break;//10000 01010
		case 1110:
		case 11110:
		case 1111:
			CrossFlag=true;
			if(ReadyOverTakeFlag)
			{
				OverTakeFlag=true;
				ChangeIntervalTurn1=250;
				ChangeIntervalTurn2=1800;
				StraightV=15;
				TurnV1=14;
				TurnV2=12;
				Velocity_Kp=100;
				Velocity_Ki=22;
				CrossAccelerateTimes=200;
				DecelerationTimes=15;
				ReadyOverTakeFlag=false;
				CrossRushOrNot=false;
			}
			break;
		default:break;
	}
	if(DecelerationFlag2)
	{
		if(DecelerationCounter>DecelerationTimes)
		{
			DecelerationFlag2=false;
			DecelerationCounter=0;
		}
		else
		{
			target_encoder_value=TurnV2;
			DecelerationCounter++;
		}
		
	}
	
	if(DecelerationFlag1)
	{
		if(DecelerationCounter>DecelerationTimes)
		{
			DecelerationFlag1=false;
			DecelerationCounter=0;
		}
		else
		{
			target_encoder_value=TurnV2;
			DecelerationCounter++;
		}
		
	}
	
	if(OverTakeFlag)
	{
		if(OverTakeCount<400)
		{
			OverTakeCount++;
		}
		else
		{
			OverTakeFlag=false;
			CrossRushOrNot=true;
			MaxTurn1=1500;
			MaxTurn2=2000;
			ChangeIntervalTurn1=250;
			ChangeIntervalTurn2=1800;
			StraightV=11;
			TurnV1=11;
			TurnV2=10;
			Velocity_Kp=100;
			Velocity_Ki=22;
			CrossAccelerateTimes=200;
			DecelerationTimes=0;	
		}
//		if(OverTakeCount==300)
//		{
//			CrossRushOrNot=true;
//		}
	}
	if(CrossFlag)
	{
		if(CrossAccelerateCount<CrossAccelerateTimes)
		{
			CrossAccelerateCount++;
		}
		else
		{
			CrossFlag=false;
			CrossAccelerateCount=0;
		}
	}
	
	return turnPwm;
}

int turn()
{
	//t1=1;
	t1=getTCRTValue(1);
  	t2=getTCRTValue(2);
	t3=getTCRTValue(3);
	t4=getTCRTValue(4);
	t5=getTCRTValue(5);

	if(t2==1&&t3==1&&t4==1)
	{
		CrossFlag=true;
	}
	if(CrossFlag)
	{
		if(CrossAccelerateCount<CrossAccelerateTimes)
		{
			t1=0;
			ChangeIntervalTurn1=150;
			CrossAccelerateCount++;
			
		}
		else
		{
			CrossFlag=false;
			ChangeIntervalTurn1=200;
			CrossAccelerateCount=0;
		}
		
	}
	
	if(t1+t2+t3+t4+t5==0)
	{
		turnPwm=0;
		target_encoder_value=StraightV;
	}
	else
	{
		if(t1==1&&t2==0&&t3==0&&t4==0&&t5==0)
		{
			target_encoder_value=TurnV2;
			DecelerationFlag2=true;
			if(turnPwm>MaxTurn2)
			{
				turnPwm=MaxTurn2;
			}
			else
			{
				turnPwm+=ChangeIntervalTurn2;
			}
		}
		if(t1==0&&t2==1&&t3==0&&t4==0&&t5==0)
		{
			target_encoder_value=TurnV1;
			if(!CrossFlag)
			{
				DecelerationFlag1=true;
			}
			
			if(turnPwm>MaxTurn1)
			{
				turnPwm=MaxTurn1;
			}
			else
			{
				turnPwm+=ChangeIntervalTurn1;
			}
		}
		if(t1==0&&t2==0&&t3==0&&t4==1&&t5==0)
		{
			target_encoder_value=TurnV1;
			if(!CrossFlag)
			{
				DecelerationFlag1=true;
			}
			if(turnPwm<-MaxTurn1)
			{
				turnPwm=-MaxTurn1;
			}
			else
			{
				turnPwm-=ChangeIntervalTurn1;
			}
		}
		if(t1==0&&t2==0&&t3==0&&t4==0&&t5==1)
		{
			target_encoder_value=TurnV2;
			DecelerationFlag2=true;
			if(turnPwm<-MaxTurn2)
			{
				turnPwm=-MaxTurn2;
			}
			else
			{
				turnPwm-=ChangeIntervalTurn2;
			}
		}
		if(t3==1)
		{
			turnPwm=0;
			DecelerationCounter=0;
			DecelerationFlag1=false;
			DecelerationFlag2=false;
			target_encoder_value=StraightV;
		}
//		if(t2==1&&t3==1&&t4==1)
//		{
//			CrossFlag=true;
//		}
//		if(CrossFlag)
//		{
//			if(CrossAccelerateCount<CrossAccelerateTimes)
//			{
//				turnPwm=0;
//				
//				CrossAccelerateCount++;
//			}
//			else
//			{
//				CrossFlag=false;
//				CrossAccelerateCount=0;
//			}
//			
//		}
	}
	
	if(DecelerationFlag2)
	{

		target_encoder_value=TurnV2;
		DecelerationCounter++;
	}
	if(DecelerationCounter>DecelerationTimes)
	{
		DecelerationFlag2=false;
		DecelerationCounter=0;
	}
	if(DecelerationFlag1)
	{

		target_encoder_value=TurnV2;
		DecelerationCounter++;
	}
	if(DecelerationCounter>DecelerationTimes)
	{
		DecelerationFlag1=false;
		DecelerationCounter=0;
	}
	
	return turnPwm;
}

void init_control()
{
  GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
	
	Timer32_initModule(TIMER32_0_BASE,TIMER32_PRESCALER_1,TIMER32_32BIT,TIMER32_PERIODIC_MODE);
	Interrupt_enableInterrupt(INT_T32_INT1);
	Timer32_setCount(TIMER32_0_BASE,120000);//10ms
  Timer32_enableInterrupt(TIMER32_0_BASE);
  //Timer32_startTimer(TIMER32_BASE, false);
}