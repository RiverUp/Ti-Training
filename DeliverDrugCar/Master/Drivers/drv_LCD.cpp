#include "drv_LCD.hpp"
#include "delay.h"

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

/* LCD 操作*/

	/*
	 * 函数功能：初始化IO口管脚
	 * addr:器件地址 
	 * reg:寄存器地址
	 * len:写入长度
	 * buf:数据区
	 * 返回值：0,正常 其他,错误代码
	 */
	void LCD_GPIO_Init(void)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE,GPIO_Pin_1);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;	 
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_5);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_6;
		GPIO_Init(GPIOE, &GPIO_InitStructure);	 
		GPIO_SetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_5);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_Init(GPIOE, &GPIO_InitStructure);	 
		GPIO_SetBits(GPIOE,GPIO_Pin_4);
	}

	/*
	 * 函数功能：LCD串行数据写入函数
	 * dat:要写入的串行数据
	 * 返回值：无
	 */
	void LCD_Writ_Bus(u8 dat) 
	{	
		u8 i;
		LCD_CS_Clr();
		for(i=0;i<8;i++)
		{			  
			LCD_SCLK_Clr();
			if(dat&0x80)
			{
				 LCD_MOSI_Set();
			}
			else
			{
				 LCD_MOSI_Clr();
			}
			LCD_SCLK_Set();
			dat<<=1;
		}	
		LCD_CS_Set();	
	}

	/*
	 * 函数功能：LCD写入数据
	 * dat: 写入的数据
	 * 返回值：无
	 */
	void LCD_WR_DATA8(u8 dat)
	{
		LCD_Writ_Bus(dat);
	}

	/*
	 * 函数功能：LCD写入16位数据
	 * dat: 写入的数据
	 * 返回值：无
	 */
	void LCD_WR_DATA(u16 dat)
	{
		LCD_Writ_Bus(dat>>8);
		LCD_Writ_Bus(dat);
	}

	/*
	 * 函数功能：LCD写入命令
	 * dat: dat 写入的命令
	 * 返回值：无
	 */
	void LCD_WR_REG(u8 dat)
	{
		//写命令
		LCD_DC_Clr();
		LCD_Writ_Bus(dat);
		//写数据
		LCD_DC_Set();
	}

	/*
	 * 函数功能：设置起始和结束地址
	 * x1,x2：设置列的起始和结束地址
	 * y1,y2：设置行的起始和结束地址
	 * 返回值：无
	 */
	void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
	{
		if(USE_HORIZONTAL==0)
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//储存器写
		}
		else if(USE_HORIZONTAL==1)
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1+80);
			LCD_WR_DATA(y2+80);
			LCD_WR_REG(0x2c);//储存器写
		}
		else if(USE_HORIZONTAL==2)
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//储存器写
		}
		else
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1+80);
			LCD_WR_DATA(x2+80);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//储存器写
		}
	}

/* LCD 操作*/
	
/*
 * 函数功能：初始化LCD
 */
void init_drv_LCD(void)
{
	//初始化GPIO
	LCD_GPIO_Init();
	//复位
	LCD_RES_Clr();
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	//打开背光
	LCD_BLK_Set();
	delay_ms(100);
	
	//初始化
	LCD_WR_REG(0x11); 
	delay_ms(120);              
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);			
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);			
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x33); 			

	LCD_WR_REG(0xB7);			
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);			
	//VCOM = 1.35V
	LCD_WR_DATA8(0x32); 
					
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);			
	//GVDD=4.8V  颜色深度
	LCD_WR_DATA8(0x15); 
				
	LCD_WR_REG(0xC4);			
	//VDV, 0x20:0v
	LCD_WR_DATA8(0x20); 

	LCD_WR_REG(0xC6);			
	//0x0F:60Hz      
	LCD_WR_DATA8(0x0F);   	

	LCD_WR_REG(0xD0);			
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1); 

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29);
} 