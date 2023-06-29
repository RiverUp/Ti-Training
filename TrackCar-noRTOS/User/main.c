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
	init_digitalTCRT();
	init_hc_sr04();
	init_hc05();
	KEY_Init();
	init_K210();
	
	//init_TCRT();
	
	
	char text1[20];
	char text2[20];
	char text3[20];
	char text4[20];
	
	//SYSCTL->REBOOT_CTL|=SYSCTL_REBOOT_CTL_REBOOT;
	while(1)
	{
		
		//°´¼üÅÐ¶Ï
		int keyState=KEY_Scan(0);
		if(keyState==KEY1_PRES)
		{
			//BTBeginConnectFlag=true;
			Timer32_startTimer(TIMER32_BASE, false);
			GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		}
		if(BTBeginConnectFlag)
		{
			delay_ms(100);
			sendMsgByBlueTooth("r0");
		}
		//À¶ÑÀÐÅÏ¢ÅÐ¶Ï
		if(BTRecCompleteFlag)
		{
			OLED_ShowString(0,4,(unsigned char*)btdata);
			if(!strcmp(btdata,"bk"))
			{
				btdata[dataPtr]='0';
				sendMsgByBlueTooth(btdata);
			}
			if(!strcmp(btdata,"r"))
			{
				Timer32_startTimer(TIMER32_BASE, false);
				BTBeginConnectFlag=false;
				btdata[dataPtr]='0';
				sendMsgByBlueTooth(btdata);
			}
			{
				Timer32_startTimer(TIMER32_BASE, false);
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
		//k210¼ì²â£¨µ½²íµÀ£©
		if(K210_RecCompleteFlag)
		{
			if(k210_data[0]==2)
			{
				CrossFlag=true;
				CrossNums++;
			}
			K210_RecCompleteFlag=false;
			OLED_ShowString(0,6,(unsigned char *)k210_data);
			data_k210_Ptr=0;
			memset(k210_data,0,strlen(k210_data));
		}
		
		if(StopFlag)
		{
			StopFlag=false;
			Timer32_haltTimer(TIMER32_BASE);
			sendMsgByBlueTooth("s0");
		}
		
		
		
		
//		sprintf(text1,"t1:%d ",t1);
//		sprintf(text2,"t2:%d ",t2);
//		sprintf(text3,"t3:%d ",t3);
		
		sprintf(text1,"r:%2d ",encoder_right);
		sprintf(text2,"l:%2d ",encoder_left);
//		sprintf(text3,"adc:%.2f     ",adc);
		sprintf(text3,"turnPwm:%d     ",turnPwm);
//		sprintf(text4,"distPwm:%d     ",distancePwm);
//		sprintf(text2,"pa:%d ",pwma);
//		sprintf(text3,"pb:%d ",pwmb);
//		sprintf(text3,"distance:%d     ",HCSRCountValue);
		
		OLED_ShowString(0,0,(unsigned char *)text1);
		OLED_ShowString(0,2,(unsigned char *)text2);
		OLED_ShowString(0,4,(unsigned char *)text3);
		//OLED_ShowString(0,6,(unsigned char *)text4);
		//delay_ms(10);
		
	}
}