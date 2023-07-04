#include "Basic.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "drv_LCD.hpp"
#include "GUI_Images.hpp"
#include "GUI.hpp"
#include "Font.hpp"
#include "Commulink.hpp"
#include "drv_MPU6050.hpp"
#include "Control.hpp"	

//获取RTC时间
extern "C" RTC_TimeStruct Get_RTC_Time();
//陀螺仪姿态
extern float Pitch,Roll,Yaw;	  

/*
 * 函数功能：在指定区域填充颜色
 * xsta,ysta：起始坐标
 * xend,yend：终止坐标
 * color：要填充的颜色 
 */
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	//设置显示范围
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/*
 * 函数功能：在指定位置画点
 * x,y：画点坐标
 * color：点的颜色 
 */
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	//设置光标位置 
	LCD_Address_Set(x,y,x,y);
	LCD_WR_DATA(color);
} 


/*
 * 函数功能：画线
 * x1,y1：起始坐标
 * x2,y2：终止坐标
 * color：线的颜色 
 */
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	//计算坐标增量 
	delta_x=x2-x1; 
	delta_y=y2-y1;
	//画线起点坐标
	uRow=x1;
	uCol=y1;
	//设置单步方向 
	if(delta_x>0)incx=1; 
	//垂直线 
	else if (delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	//水平线 
	else if (delta_y==0)incy=0;
	else {incy=-1;delta_y=-delta_y;}
	//选取基本增量坐标轴 
	if(delta_x>delta_y)distance=delta_x; 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		//画点
		LCD_DrawPoint(uRow,uCol,color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/*
 * 函数功能：画矩形
 * x1,y1：起始坐标
 * x2,y2：终止坐标
 * color：矩形的颜色 
 */
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/*
 * 函数功能：画圆
 * x0,y0：圆心坐标
 * r：半径
 * color：圆的颜色 
 */
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);                        
		LCD_DrawPoint(x0+b,y0-a,color);                        
		LCD_DrawPoint(x0-a,y0+b,color);                            
		LCD_DrawPoint(x0-a,y0-b,color);                          
		LCD_DrawPoint(x0+b,y0+a,color);                           
		LCD_DrawPoint(x0+a,y0-b,color);             
		LCD_DrawPoint(x0+a,y0+b,color);              
		LCD_DrawPoint(x0-b,y0+a,color);             
		a++;
		//判断要画的点是否过远
		if((a*a+b*b)>(r*r))
		{
			b--;
		}
	}
}

/*
 * 函数功能：显示单个字符
 * x,y：显示坐标
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 * mode：0非叠加模式  1叠加模式
 */
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	//一个字符所占字节大小
	u16 i,TypefaceNum;
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	//得到偏移后的值
	num=num-' ';    
	//设置光标位置 
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       
		else if(sizey==16)temp=ascii_1608[num][i];		
		else if(sizey==24)temp=ascii_2412[num][i];		 
		else if(sizey==32)temp=ascii_3216[num][i];		
		else return;
		for(t=0;t<8;t++)
		{
			//非叠加模式
			if(!mode)
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			//叠加模式
			else
			{
				//画一个点
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/*
 * 函数功能：显示字符串
 * *p：要显示的字符串
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 * mode：0非叠加模式  1叠加模式
 */
void LCD_ShowString(u16 x,u16 y,const char *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/*
 * 函数功能：显示数字
 * m：底数
 * n：指数
 */
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}

/*
 * 函数功能：显示整数变量
 * x,y：显示坐标
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 * len：要显示的位数
 */
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/*
 * 函数功能：显示两位小数变量
 * x,y：显示坐标
 * num：要显示的字符
 * fc：字的颜色
 * bc：字的背景色
 * sizey：字号
 */
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/*
 * 函数功能：显示图片
 * x,y：起点坐标
 * length：图片长度
 * width：图片宽度
 * pic[]：图片数组    
 */
void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}

void GUI_Server()
{
	RTC_TimeStruct RTC_Time;
	char FU[]={":"};
	char str[50];
	float VDDA = 0;
	//获取时间
		RTC_Time = Get_RTC_Time();
		LCD_ShowIntNum(105,43,RTC_Time.Year,4,PURPLE,WHITE,12);
		LCD_ShowString(129,43,FU,PURPLE,WHITE,12,0);
		LCD_ShowIntNum(135,43,RTC_Time.Month,2,PURPLE,WHITE,12);
		LCD_ShowString(147,43,FU,PURPLE,WHITE,12,0);
		LCD_ShowIntNum(153,43,RTC_Time.Date,2,PURPLE,WHITE,12);
		
		LCD_ShowIntNum(171,43,RTC_Time.Hours,2,PURPLE,WHITE,12);
		LCD_ShowString(183,43,FU,PURPLE,WHITE,12,0);
		LCD_ShowIntNum(189,43,RTC_Time.Minutes,2,PURPLE,WHITE,12);
		LCD_ShowString(201,43,FU,PURPLE,WHITE,12,0);
		LCD_ShowIntNum(207,43,RTC_Time.Seconds,2,PURPLE,WHITE,12);
	/*获取姿态*/
		//获取姿态角
		//roll
		sprintf( str, "%3.1f", Pitch);
		LCD_ShowString(134,76,str,RED,WHITE,16,0);
		//pitch
		sprintf( str, "%3.1f", Roll);
		LCD_ShowString(134,92,str,RED,WHITE,16,0);
		//yaw
		sprintf( str, "%3.1f", Yaw);
		LCD_ShowString(134,108,str,RED,WHITE,16,0);
	// /*获取姿态*/
	
	/*获取编码器的计数*/
		sprintf( str, "%4d", Encoder_Left);
		LCD_ShowString(134,124,str,RED,WHITE,16,0);
		sprintf( str, "%4d", Encoder_Right);
		LCD_ShowString(134,140,str,RED,WHITE,16,0);
	/*获取编码器的计数*/
	
	/*获取电机的速度*/
		sprintf( str, "%4.1f", Velocity_Left);
		LCD_ShowString(134,156,str,RED,WHITE,16,0);
		sprintf( str, "%4.1f", Velocity_Right);
		LCD_ShowString(134,172,str,RED,WHITE,16,0);
	/*获取电机的速度*/
	
	/*获取PWM赋值的速度*/
		sprintf( str, "%4d", Motor_Left);
		LCD_ShowString(134,188,str,RED,WHITE,16,0);
		sprintf( str, "%4d", Motor_Right);
		LCD_ShowString(134,204,str,RED,WHITE,16,0);
	/*获取PWM赋值的速度*/
	
	/*获取角速度*/
		sprintf( str, "%4.1f", Gyro_Balance);
		LCD_ShowString(134,220,str,RED,WHITE,16,0);
	/*获取角速度*/
	
	/*获取电池电压*/
		VDDA = ((float)Voltage)/100;
		sprintf( str, "%4.2f", VDDA);
		LCD_ShowString(134,60,str,RED,WHITE,16,0);
		sprintf( str, "V");
		LCD_ShowString(174,60,str,RED,WHITE,16,0);
//		if(VDDA<11.7)
//		{
//			sprintf(str,"can't work, please recharge");
//			LCD_ShowString(0,200,str,RED,WHITE,16,0);
//		}
	/*获取电池电压*/
}

	
void init_GUI()
{
	char WhuFly[]={"Whu-Car"};
	char Auther[]={"Auther:WX"};
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	char VDDA[]={"VDDA: 0.0"};
	char Rol[]={"Pit:  0.0"};
	char Pit[]={"Rol:  0.0"};
	char YaW[]={"Yaw:  0.0"};
	char EnL[]={"EnL:  0.0"};
	char EnR[]={"EnR:  0.0"};
	char VL[]={"VL:   0.0"};
	char VR[]={"VR:   0.0"};
	char MtL[]={"MtL:  0.0"};
	char MtR[]={"MtR:  0.0"};
	char Gyro[]={"Gyro: 0.0"};
	LCD_ShowString(130,0,WhuFly,RED,WHITE,24,0);
	LCD_ShowString(135,24,Auther,BLUE,WHITE,16,0);
	LCD_ShowPicture(0,0,91,57,gImage_Competition);
	LCD_ShowString(94,60,VDDA,RED,WHITE,16,0);
	LCD_ShowString(94,76,Rol,RED,WHITE,16,0);
	LCD_ShowString(94,92,Pit,RED,WHITE,16,0);
	LCD_ShowString(94,108,YaW,RED,WHITE,16,0);
	LCD_ShowString(94,124,EnL,RED,WHITE,16,0);
	LCD_ShowString(94,140,EnR,RED,WHITE,16,0);
	LCD_ShowString(94,156,VL,RED,WHITE,16,0);
	LCD_ShowString(94,172,VR,RED,WHITE,16,0);
	LCD_ShowString(94,188,MtL,RED,WHITE,16,0);
	LCD_ShowString(94,204,MtR,RED,WHITE,16,0);
	LCD_ShowString(94,220,Gyro,RED,WHITE,16,0);
}