#pragma once

#include "Basic.hpp"
	
/*RGB565颜色*/
#define  BLACK	0x0000			// 黑色
#define  NAVY	0x000F				// 深蓝色
#define  DGREEN  0x03E0     // 深绿色
#define  DCYAN   0x03EF     // 深青色
#define  MAROON  0x7800     // 深红色
#define  PURPLE  0x780F     // 紫色
#define  OLIVE   0x7BE0     // 橄榄绿
#define  LGRAY   0xC618     // 灰白色
#define  DGRAY   0x7BEF     // 深灰色
#define  BLUE   0x001F      // 蓝色
#define  GREEN   0x07E0     // 绿色
#define  CYAN   0x07FF      // 青色
#define  RED    0xF800      // 红色
#define  MAGENTA  0xF81F    // 品红
#define  YELLOW  0xFFE0     // 黄色
#define  WHITE   0xFFFF     // 白色
/*RGB565颜色*/

/*
 * 函数功能：在指定区域填充颜色
 * xsta,ysta：起始坐标
 * xend,yend：终止坐标
 * color：要填充的颜色 
 */
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
/*
 * 函数功能：在指定位置画点
 * x,y：画点坐标
 * color：点的颜色 
 */
void LCD_DrawPoint(u16 x,u16 y,u16 color);
/*
 * 函数功能：画线
 * x1,y1：起始坐标
 * x2,y2：终止坐标
 * color：线的颜色 
 */
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
/*
 * 函数功能：画矩形
 * x1,y1：起始坐标
 * x2,y2：终止坐标
 * color：矩形的颜色 
 */
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
/*
 * 函数功能：画圆
 * x0,y0：圆心坐标
 * r：半径
 * color：圆的颜色 
 */
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
/*
 * 函数功能：显示单个字符
 * x,y：显示坐标
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 * mode：0非叠加模式  1叠加模式
 */
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);
/*
 * 函数功能：显示字符串
 * *p：要显示的字符串
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 * mode：0非叠加模式  1叠加模式
 */
void LCD_ShowString(u16 x,u16 y,const char *p,u16 fc,u16 bc,u8 sizey,u8 mode);
/*
 * 函数功能：显示数字
 * m：底数
 * n：指数
 */
u32 mypow(u8 m,u8 n);
/*
 * 函数功能：显示整数变量
 * x,y：显示坐标
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 * len：要显示的位数
 */
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);
/*
 * 函数功能：显示两位小数变量
 * x,y：显示坐标
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 */
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);
/*
 * 函数功能：显示图片
 * x,y：起点坐标
 * length：图片长度
 * width：图片宽度
 * pic[]：图片数组    
 */
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);
	
//初始化GUI
void init_GUI();
void GUI_Server();