/***********************************************
*            OLED用到的头文件和函数声明
***********************************************/

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
/* Standard Includes */

#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
//#include "stdlib.h"	  
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0
//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  


//void delay_ms(unsigned int ms);


//OLED控制用函数
void init(void);                     //OLED端口定义
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_CS_Clr(void);
void OLED_CS_Set(void);
void OLED_RST_Clr(void);
void OLED_RST_Set(void);
void OLED_DC_Clr(void);
void OLED_DC_Set(void);
void OLED_SCLK_Clr(void);
void OLED_SCLK_Set(void);
void OLED_SDIN_Clr(void);
void OLED_SDIN_Set(void);
#endif  
