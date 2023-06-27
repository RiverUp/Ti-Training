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
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

bool TriggerHCSRFlag;

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
	init_hc05();
	//init_TCRT();
	
	
	char text1[20];
	char text2[20];
	char text3[20];
	char text4[20];

	char blt[20];
	while(1)
	{
		if(BTRecCompleteFlag)
		{
			OLED_ShowString(0,4,(unsigned char*)btdata);
			if(!strcmp(btdata,"bk"))
			{
				btdata[dataPtr]='0';
				sendMsgByBlueTooth(btdata);
			}
			dataPtr=0;
			memset(btdata,0,strlen(btdata));
			BTRecCompleteFlag=false;
		}
//		sprintf(text1,"t1:%d ",t1);
//		sprintf(text2,"t2:%d ",t2);
//		sprintf(text3,"t3:%d ",t3);
		
		sprintf(text1,"r:%2d ",encoder_right);
		sprintf(text2,"l:%2d ",encoder_left);
//		sprintf(text3,"adc:%.2f     ",adc);
		sprintf(text3,"turnPwm:%d     ",turnPwm);
		OLED_ShowString(0,0,(unsigned char *)text1);
		OLED_ShowString(0,2,(unsigned char *)text2);
		//OLED_ShowString(0,4,(unsigned char *)text3);
		//delay_ms(10);
		
	}
}