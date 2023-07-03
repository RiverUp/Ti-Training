#include "Control.h"

int MaxTurn1= 1500;
int MaxTurn2 =2000;
int ChangeIntervalTurn1 =250;   
int ChangeIntervalTurn2 =1800;

int StraightV= 11;
int TurnV1 =11;
int TurnV2 =10;




bool StopFlag;
bool CrossFlag;			//遇到岔道
bool WaitFlag;			//第二次遇到不置CrossFlag
int CrossAccelerateCount;
int CrossAccelerateTimes=200;
bool OverTakeFlag;
int OverTakeCount;
int WaitCount;
bool CrossRushOrNot=true;
int CrossNums;			//遇到的岔道个数
bool DecelerationFlag1;
bool DecelerationFlag2;
int DecelerationTimes=0;
int DecelerationCounter;

int Mission=1; 

float target_encoder_value;
float Velocity_Kp=100,Velocity_Ki=22;
float Distance_Kp=0.1;
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
//	if(Mission==1)
//	{
		
		pwma = velocity_value + turn_value;
		pwmb = velocity_value - turn_value;
//		if(CrossFlag)
//		{
//			set_pwm(velocity_value+1875,velocity_value+2125);
//			delay_ms(CrossAccelerateTimes);
//			CrossFlag=false;
//		}
//	}
//	if(Mission==2)
//	{
//		pwma = velocity_value + turn_value;//+distance_value;
//		pwmb = velocity_value - turn_value;//+distance_value;
//		int distance_value=distance(HCSRCountValue);
//		target_encoder_value+=distance_value;
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
			if(CrossFlag&&CrossRushOrNot)
			{
				if(turnPwm<-800)
				{
					turnPwm=-800;
				}
				else
				{
					turnPwm-=80;
				}
			}
			if(!CrossRushOrNot&&CrossAccelerateCount>CrossAccelerateTimes/5)
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
			if(CrossRushOrNot)
			{
				if(turnPwm<-MaxTurn1)
				{
					turnPwm=-MaxTurn1;
				}
				else
				{
					turnPwm-=ChangeIntervalTurn1;
				}
			}
			else
			{
				if(turnPwm<-MaxTurn1)
				{
					turnPwm=-MaxTurn1;
				}
				else
				{
					turnPwm-=300;
				}
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
			if(!CrossRushOrNot&&WaitCount>CrossAccelerateTimes/4&&WaitCount<200)
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
			if(Mission==1&&CrossNums==1&&!WaitFlag)
			{
				StopFlag=true;
			}
			if(Mission==2&&CrossNums==2&&!WaitFlag)
			{
				StopFlag=true;
			}
			if(Mission==3&&!WaitFlag)
			{
				//告诉从车超车
				if(CrossNums==1)
				{
					sendMsgByBlueTooth("i");
				}
				//加速走内圈超车
				if(CrossNums==2)
				{
					sendMsgByBlueTooth("o");
					OverTakeFlag=true;
					CrossRushOrNot=false;
					MaxTurn1=1500;
					MaxTurn2=2000;
					ChangeIntervalTurn1=250;
					ChangeIntervalTurn2=1800;
					StraightV=15;
					TurnV1=14;
					TurnV2=12;
					target_encoder_value=StraightV;
					Velocity_Kp=100;
					Velocity_Ki=22;
					CrossAccelerateTimes=200;
					DecelerationTimes=10;
				}
				if(CrossNums==3)
				{
					StopFlag=true;
				}
			}
			if(Mission==4&&!WaitFlag)
			{
				if(CrossNums==1)
				{
					Timer32_haltTimer(TIMER32_0_BASE);
					set_pwm(0,0);
					sendMsgByBlueTooth("s");
					WaitCount=5*CrossAccelerateTimes;
					delay_ms(5000);
					Timer32_startTimer(TIMER32_0_BASE,false);
					sendMsgByBlueTooth("r");
					
				}
				if(CrossNums==2)
				{
					CrossNums++;
					break;
				}
				if(CrossNums==3)
				{
					StopFlag=true;
				}
			}
			if(!WaitFlag)
			{
				CrossFlag=true;
				WaitFlag=true;
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
			CrossNums++;
		}
		//转内圈
		if(CrossAccelerateCount==(CrossAccelerateTimes/3*2)&&!CrossRushOrNot)
		{
			//turnPwm-=2000;
		}
		
	}
	if(WaitFlag)
	{
		if(WaitCount<7*CrossAccelerateTimes)
		{
			WaitCount++;
		}
		else
		{
			
			WaitFlag=false;
			WaitCount=0;
		}
		if(WaitCount>4*CrossAccelerateTimes)//3->4
		{
			CrossRushOrNot=true;
		}
		
	}
	if(OverTakeFlag)
	{
		if(OverTakeCount<450)
		{
			OverTakeCount++;
		}
		else
		{
			//加完速告诉从车开始跟随
			sendMsgByBlueTooth("f");
			OverTakeFlag=false;
			CrossRushOrNot=true;
			MaxTurn1=1500;
			MaxTurn2=2000;
			ChangeIntervalTurn1=250;
			ChangeIntervalTurn2=1800;
			StraightV=10;
			TurnV1=10;
			TurnV2=10;
			Velocity_Kp=100;
			Velocity_Ki=22;
			CrossAccelerateTimes=160;
			DecelerationTimes=0;
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