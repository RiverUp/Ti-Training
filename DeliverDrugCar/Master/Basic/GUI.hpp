#pragma once

#include "Basic.hpp"
	
/*RGB565��ɫ*/
#define  BLACK	0x0000			// ��ɫ
#define  NAVY	0x000F				// ����ɫ
#define  DGREEN  0x03E0     // ����ɫ
#define  DCYAN   0x03EF     // ����ɫ
#define  MAROON  0x7800     // ���ɫ
#define  PURPLE  0x780F     // ��ɫ
#define  OLIVE   0x7BE0     // �����
#define  LGRAY   0xC618     // �Ұ�ɫ
#define  DGRAY   0x7BEF     // ���ɫ
#define  BLUE   0x001F      // ��ɫ
#define  GREEN   0x07E0     // ��ɫ
#define  CYAN   0x07FF      // ��ɫ
#define  RED    0xF800      // ��ɫ
#define  MAGENTA  0xF81F    // Ʒ��
#define  YELLOW  0xFFE0     // ��ɫ
#define  WHITE   0xFFFF     // ��ɫ
/*RGB565��ɫ*/

/*
 * �������ܣ���ָ�����������ɫ
 * xsta,ysta����ʼ����
 * xend,yend����ֹ����
 * color��Ҫ������ɫ 
 */
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
/*
 * �������ܣ���ָ��λ�û���
 * x,y����������
 * color�������ɫ 
 */
void LCD_DrawPoint(u16 x,u16 y,u16 color);
/*
 * �������ܣ�����
 * x1,y1����ʼ����
 * x2,y2����ֹ����
 * color���ߵ���ɫ 
 */
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
/*
 * �������ܣ�������
 * x1,y1����ʼ����
 * x2,y2����ֹ����
 * color�����ε���ɫ 
 */
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
/*
 * �������ܣ���Բ
 * x0,y0��Բ������
 * r���뾶
 * color��Բ����ɫ 
 */
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
/*
 * �������ܣ���ʾ�����ַ�
 * x,y����ʾ����
 * num��Ҫ��ʾ���ַ�
 * fc���ֵ���ɫ
 * bc���ֵı���ɫ
 * sizey���ֺ�
 * mode��0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);
/*
 * �������ܣ���ʾ�ַ���
 * *p��Ҫ��ʾ���ַ���
 * num��Ҫ��ʾ���ַ�
 * fc���ֵ���ɫ
 * bc���ֵı���ɫ
 * sizey���ֺ�
 * mode��0�ǵ���ģʽ  1����ģʽ
 */
void LCD_ShowString(u16 x,u16 y,const char *p,u16 fc,u16 bc,u8 sizey,u8 mode);
/*
 * �������ܣ���ʾ����
 * m������
 * n��ָ��
 */
u32 mypow(u8 m,u8 n);
/*
 * �������ܣ���ʾ��������
 * x,y����ʾ����
 * num��Ҫ��ʾ���ַ�
 * fc���ֵ���ɫ
 * bc���ֵı���ɫ
 * sizey���ֺ�
 * len��Ҫ��ʾ��λ��
 */
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);
/*
 * �������ܣ���ʾ��λС������
 * x,y����ʾ����
 * num��Ҫ��ʾ���ַ�
 * fc���ֵ���ɫ
 * bc���ֵı���ɫ
 * sizey���ֺ�
 */
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);
/*
 * �������ܣ���ʾͼƬ
 * x,y���������
 * length��ͼƬ����
 * width��ͼƬ���
 * pic[]��ͼƬ����    
 */
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);
	
//��ʼ��GUI
void init_GUI();
void GUI_Server();