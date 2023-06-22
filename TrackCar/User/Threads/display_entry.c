#include "display_entry.h"

//extern int encoder_right,encoder_left;

void display_entry()
{
	init();
	OLED_Init();
	OLED_Clear();
	char text1[20];
	char text2[20];
	char text3[20];
	char text4[20];
	
	while(1)
	{
		int encoder_left,encoder_right;
		encoder_left=read_encoder(1);
		encoder_right=read_encoder(0);
		sprintf(text1,"r:%2d ",encoder_right);
		sprintf(text2,"l:%2d ",encoder_left);
		OLED_ShowString(0,0,(unsigned char *)text1);
		OLED_ShowString(0,2,(unsigned char *)text2);
		//rt_thread_mdelay(1);
	}
	
}