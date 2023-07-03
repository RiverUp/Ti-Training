#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "rtthread.h"
#include "HC-SR04.h"
#include "Delay.h"
#include "Serial.h"
#include "HC-05.h"
#include "Encoder.h"
#include "oled.h"
#include "Motor.h"
#include "TCRT5000.h"
#include "Semaphore.h"
#include "Filter.h"
#include "Control.h"
#include "k210.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "key.h"


int main(void)
{
	WDT_A_hold(WDT_A_BASE);
	Interrupt_enableMaster();
	Delay_Init();
  initSerial();
	init_motor();
	init_encoder_left();
	init_encoder_right();
	init();
	OLED_Init();
	init_control();
	init_TCRT();
	init_hc_sr04();
	init_hc05();
	KEY_Init();
//	init_K210();
	
	//init_TCRT();
	
	
	char text1[20];
	char text2[20];
	char text3[20];
	char text4[20];
	char text5[20];
	
	//SYSCTL->REBOOT_CTL|=SYSCTL_REBOOT_CTL_REBOOT;
	while(1)
	{
		
		//°´¼üÅÐ¶Ï
		int keyState=KEY_Scan(0);
		if(keyState==KEY1_PRES)
		{
			encoder_left=0;
			encoder_right=0;
			Mission++;
			switch(Mission)
			{
				case 1:
					MaxTurn1=1500;
					MaxTurn2=2000;
					ChangeIntervalTurn1=250;
					ChangeIntervalTurn2=1800;
					StraightV=11;
					TurnV1=11;
					TurnV2=10;
					Velocity_Kp=80;
					Velocity_Ki=22;
					CrossAccelerateTimes=160;
					DecelerationTimes=0;		
					break;
				case 2:
					MaxTurn1=1500;
					MaxTurn2=2000;
					ChangeIntervalTurn1=250;
					ChangeIntervalTurn2=1800;
					StraightV=10;
					TurnV1=9;
					TurnV2=8;
					Velocity_Kp=80;
					Velocity_Ki=22;
					CrossAccelerateTimes=180;
					DecelerationTimes=0;
					break;
				case 3:
					MaxTurn1=1500;
					MaxTurn2=2000;
					ChangeIntervalTurn1=250;
					ChangeIntervalTurn2=2500;
					StraightV=11;
					TurnV1=11;
					TurnV2=10;
					Velocity_Kp=80;
					Velocity_Ki=22;
					CrossAccelerateTimes=200;
					DecelerationTimes=0;
					break;
				case 4:
					break;
			}
			sendMsgByBlueTooth("r");
			Timer32_startTimer(TIMER32_0_BASE, false);
			
		}
		//À¶ÑÀÐÅÏ¢ÅÐ¶Ï
		if(BTRecCompleteFlag)
		{
			OLED_ShowString(0,6,(unsigned char*)btdata);
			if(!strcmp(btdata,"bk"))
			{
				btdata[dataPtr]='0';
				sendMsgByBlueTooth(btdata);
			}
			if(!strcmp(btdata,"r"))
			{
				//btdata[dataPtr]='0';
				//sendMsgByBlueTooth(btdata);
				Timer32_startTimer(TIMER32_0_BASE, false);
			}
			dataPtr=0;
			memset(btdata,0,strlen(btdata));
			BTRecCompleteFlag=false;
		}
		//³¬Éù
		if(NextTiggerHCSRFlag)
		{
			trigger_measure();
			//GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		}
		if(StopFlag)
		{
			set_pwm(0,0);
			Timer32_haltTimer(TIMER32_0_BASE);
			sendMsgByBlueTooth("s");
			CrossNums=0;
			StopFlag=false;
		}
//		sprintf(text1,"t1:%d ",t1);
//		sprintf(text2,"t2:%d ",t2);
//		sprintf(text3,"t3:%d ",t3);
		
		sprintf(text1,"r:%2d ",encoder_right);
		sprintf(text2,"l:%2d ",encoder_left);
//		sprintf(text3,"adc:%.2f     ",adc);
		sprintf(text3,"turnPwm:%d     ",turnPwm);
		//k210
//		if(K210_RecCompleteFlag)
//		{
//			K210_RecCompleteFlag=false;
//			OLED_ShowString(0,6,(unsigned char *)k210_data);
//			if(k210_data[0]==2)
//			{
//				CrossFlag=true;
//				CrossNums++;
//			}
//			data_k210_Ptr=0;
//			memset(k210_data,0,strlen(k210_data));
//		}
		//OLED_ShowString(0,4,(unsigned char*)btdata);
//		sprintf(text3,"distance:%d     ",HCSRCountValue);
//		OLED_ShowString(0,0,(unsigned char *)text1);
//		OLED_ShowString(0,2,(unsigned char *)text2);
//		OLED_ShowString(0,4,(unsigned char *)text3);
		//delay_ms(10);
//		float adc1=read_TCRT(1);
//		float adc2=read_TCRT(2);
//		float adc3=read_TCRT(3);
//		float adc4=read_TCRT(4);
//		float adc5=read_TCRT(5);
		
//		sprintf(text3,"distance:%d     ",HCSRCountValue);
		
//		sprintf(text1,"adc1:%.1f",adc1);
//		sprintf(text2,"adc2:%.1f",adc2);
//		sprintf(text3,"adc3:%.1f",adc3);
//		sprintf(text4,"adc4:%.1f",adc4);
//		sprintf(text5,"adc5:%.1f",adc5);
//		//sprintf(text3,"turnPwm:%d     ",turnPwm);
//		
		OLED_ShowString(0,0,(unsigned char *)text1);
		OLED_ShowString(0,2,(unsigned char *)text2);
		OLED_ShowString(0,4,(unsigned char *)text3);
//		OLED_ShowString(0,6,(unsigned char *)text4);
//		OLED_ShowString(64,0,(unsigned char *)text5);
	}
}