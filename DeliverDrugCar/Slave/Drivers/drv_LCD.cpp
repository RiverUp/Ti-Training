#include "drv_LCD.hpp"
#include "delay.h"

/*
		GND   <--->   ��Դ��
		VCC   <--->   3.3v��Դ
		SCL   <--->   SCLK PE3
		SDA   <--->   MOSI PE5
		RES   <--->   PE4
		DC    <--->   PE2
		CS    <--->   PE1
		BLK   <--->   PE6
*/

/* LCD ����*/

	/*
	 * �������ܣ���ʼ��IO�ڹܽ�
	 * addr:������ַ 
	 * reg:�Ĵ�����ַ
	 * len:д�볤��
	 * buf:������
	 * ����ֵ��0,���� ����,�������
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
	 * �������ܣ�LCD��������д�뺯��
	 * dat:Ҫд��Ĵ�������
	 * ����ֵ����
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
	 * �������ܣ�LCDд������
	 * dat: д�������
	 * ����ֵ����
	 */
	void LCD_WR_DATA8(u8 dat)
	{
		LCD_Writ_Bus(dat);
	}

	/*
	 * �������ܣ�LCDд��16λ����
	 * dat: д�������
	 * ����ֵ����
	 */
	void LCD_WR_DATA(u16 dat)
	{
		LCD_Writ_Bus(dat>>8);
		LCD_Writ_Bus(dat);
	}

	/*
	 * �������ܣ�LCDд������
	 * dat: dat д�������
	 * ����ֵ����
	 */
	void LCD_WR_REG(u8 dat)
	{
		//д����
		LCD_DC_Clr();
		LCD_Writ_Bus(dat);
		//д����
		LCD_DC_Set();
	}

	/*
	 * �������ܣ�������ʼ�ͽ�����ַ
	 * x1,x2�������е���ʼ�ͽ�����ַ
	 * y1,y2�������е���ʼ�ͽ�����ַ
	 * ����ֵ����
	 */
	void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
	{
		if(USE_HORIZONTAL==0)
		{
			LCD_WR_REG(0x2a);//�е�ַ����
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//�е�ַ����
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//������д
		}
		else if(USE_HORIZONTAL==1)
		{
			LCD_WR_REG(0x2a);//�е�ַ����
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//�е�ַ����
			LCD_WR_DATA(y1+80);
			LCD_WR_DATA(y2+80);
			LCD_WR_REG(0x2c);//������д
		}
		else if(USE_HORIZONTAL==2)
		{
			LCD_WR_REG(0x2a);//�е�ַ����
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//�е�ַ����
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//������д
		}
		else
		{
			LCD_WR_REG(0x2a);//�е�ַ����
			LCD_WR_DATA(x1+80);
			LCD_WR_DATA(x2+80);
			LCD_WR_REG(0x2b);//�е�ַ����
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//������д
		}
	}

/* LCD ����*/
	
/*
 * �������ܣ���ʼ��LCD
 */
void init_drv_LCD(void)
{
	//��ʼ��GPIO
	LCD_GPIO_Init();
	//��λ
	LCD_RES_Clr();
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	//�򿪱���
	LCD_BLK_Set();
	delay_ms(100);
	
	//��ʼ��
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
	//GVDD=4.8V  ��ɫ���
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