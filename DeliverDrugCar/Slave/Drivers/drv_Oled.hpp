#pragma once

#include "stdint.h"
//图像长宽
#define LCD_W 240
#define LCD_H 240

/*上锁保证通信连续性
	上锁之后必须解锁
	Sync_waitTime：超时时间
*/
bool Lock_Oled( double Sync_waitTime = -1 );
void UnLock_Oled();

/*清屏为Color颜色
	Sync_waitTime：超时时间
*/
bool LCD_Clear(uint16_t Color, double Sync_waitTime = -1);

/*在(x1,y1)-(x2,y2)矩形区域写入图像pic
	Sync_waitTime：超时时间
*/
bool LCD_WritePicture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* pic, double Sync_waitTime = -1);

static void LCD_Writ_Bus( void* buffer, uint32_t size, bool DisableMInc );
bool Lock_Oled( double Sync_waitTime );
void UnLock_Oled();
//LCD写8字节数据
bool LCD_WR_DATA8( uint8_t dat, double Sync_waitTime = -1 );
//LCD写16字节数据
bool LCD_WR_DATA( uint16_t dat, double Sync_waitTime = -1 );
static bool LCD_WR_REG( uint8_t dat, double Sync_waitTime = -1 );
void LCD_Address_Set( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );
bool LCD_Clear( uint16_t Color, double Sync_waitTime);
bool LCD_WritePicture( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* pic, double Sync_waitTime );

void init_drv_Oled();