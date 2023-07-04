#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"

/*
		GND   <--->   电源地
		VCC   <--->   3.3v电源
		SCL   <--->   SCLK PE3
		SDA   <--->   MOSI PE5
		RES   <--->   PE4
		DC    <--->   PE2
		CS    <--->   PE1
		BLK   <--->   PE6
*/

//设置横屏或者竖屏显示 
//0或1为竖屏 2或3为横屏
#define USE_HORIZONTAL 0  
#define LCD_W 240
#define LCD_H 240
//SCL=SCLK
#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_3)
#define LCD_SCLK_Set() GPIO_SetBits(GPIOE,GPIO_Pin_3)
//SDA=MOSI
#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define LCD_MOSI_Set() GPIO_SetBits(GPIOE,GPIO_Pin_5)
//RES
#define LCD_RES_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define LCD_RES_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_4)
//DC
#define LCD_DC_Clr()   GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define LCD_DC_Set()   GPIO_SetBits(GPIOE,GPIO_Pin_2)
//CS
#define LCD_CS_Clr()   GPIO_ResetBits(GPIOE,GPIO_Pin_1)
#define LCD_CS_Set()   GPIO_SetBits(GPIOE,GPIO_Pin_1)
//BLK
#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define LCD_BLK_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_6)

/*
 * 函数功能：初始化IO口管脚
 * addr:器件地址 
 * reg:寄存器地址
 * len:写入长度
 * buf:数据区
 * 返回值：0,正常 其他,错误代码
 */
void LCD_GPIO_Init(void);
/*
 * 函数功能：LCD串行数据写入函数
 * dat:要写入的串行数据
 * 返回值：无
 */
void LCD_Writ_Bus(u8 dat);
/*
 * 函数功能：LCD写入数据
 * dat: 写入的数据
 * 返回值：无
 */
void LCD_WR_DATA8(u8 dat);
/*
 * 函数功能：LCD写入16位数据
 * dat: 写入的数据
 * 返回值：无
 */
void LCD_WR_DATA(u16 dat);
/*
 * 函数功能：LCD写入命令
 * dat: dat 写入的命令
 * 返回值：无
 */
void LCD_WR_REG(u8 dat);
/*
 * 函数功能：设置起始和结束地址
 * x1,x2：设置列的起始和结束地址
 * y1,y2：设置行的起始和结束地址
 * 返回值：无
 */
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);
/*
 * 函数功能：初始化LCD
 */
void init_drv_LCD(void);
#endif




