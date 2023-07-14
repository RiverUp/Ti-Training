#include "Basic.hpp"
#include "drv_ExtIIC.hpp"
#include "Commulink.hpp"
#include "inv_mpu.h"
#include "delay.h"
#include <limits>

//IIC传输配置
	#define IIC_NACK (0<<10)
	#define IIC_ACK (1<<10)
	#define IIC_STOP (1<<9)
	#define IIC_START (1<<8)

//IIC状态
	#define IIC_OUT (1<<14)
	#define IIC_PECERR (1<<12)
	#define IIC_OVR (1<<11)
	#define IIC_NACKF (1<<10)
	#define IIC_ARLO (1<<9)
	#define IIC_BERR (1<<8)
	#define IIC_TE (1<<7)
	#define IIC_RXNE (1<<6)
	#define IIC_STOPF (1<<4)
	#define IIC_BTF (1<<2)
	#define IIC_ADDR (1<<1)
	#define IIC_SB (1<<0)
	

using namespace std;

/*软件IIC操作*/
		
	/*
	 * 函数功能：IIC 延时函数
	 */
	void IIC_Delay(void)
	{
		delay_us(2);
	}
	
	/*
	 * 函数功能：读取指定设备指定寄存器的一个值 
	 * I2C_Addr：设备IIC地址
	 * addr：寄存器地址
	 * 返回值:res：读取的数据
	 */
	unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
	{
		unsigned char res=0;
		IIC_Start();	
		//发送写命令
		IIC_Send_Byte(I2C_Addr);	   		
		res++;
		IIC_Wait_Ack();
		//发送地址
		IIC_Send_Byte(addr); res++;  		
		IIC_Wait_Ack();	  
		IIC_Start();
		//进入接收模式	
		IIC_Send_Byte(I2C_Addr+1); res++;   		   
		IIC_Wait_Ack();
		res=IIC_Read_Byte(0);	   
		//产生一个停止条件
		IIC_Stop();							
		return res;
	}

	/*
	 * 函数功能：初始化软件IIC
	 */
	void IIC_Init(void)
	{					     
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;                            
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		IIC_SCL=1;
		IIC_SDA=1;
	}

	/*
	 * 函数功能：产生IIC起始信号
	 */
	void IIC_Start(void)
	{
		//SDA线输出
		SDA_OUT();     	
		IIC_SDA=1;	  	  
		IIC_SCL=1;
		delay_us(4);
		//开始：当CLK为高，数据由高到低
		IIC_SDA=0;		
		delay_us(4);
		//钳住I2C总线，准备发送或接收数据 
		IIC_SCL=0;		
	}

		  
	/*
	 * 函数功能：产生IIC停止信号
	 */
	void IIC_Stop(void)
	{
		//SDA线输出
		SDA_OUT();
		IIC_SCL=0;
		//停止：当CLK为高，数据由低到高
		IIC_SDA=0;
		delay_us(4);
		IIC_SCL=1; 
		//发送I2C总线结束信号
		IIC_SDA=1;
		delay_us(4);							   	
	}

	/*
	 * 函数功能：等待应答信号到来
	 * 返回值:1，接收应答失败 0，接收应答成功
	 */
	u8 IIC_Wait_Ack(void)
	{
		u8 ucErrTime=0;
		//SDA设置为输入  
		SDA_IN();      
		IIC_SDA=1;delay_us(1);	   
		IIC_SCL=1;delay_us(1);	 
		while(READ_SDA)
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				IIC_Stop();
				return 1;
			}
		}
		//时钟输出0 
		IIC_SCL=0;	   
		return 0;  
	} 

	/*
	 * 函数功能：产生ACK应答
	 */
	void IIC_Ack(void)
	{
		IIC_SCL=0;
		SDA_OUT();
		IIC_SDA=0;
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
	}

	/*
	 * 函数功能：不产生ACK应答
	 */	    
	void IIC_NAck(void)
	{
		IIC_SCL=0;
		SDA_OUT();
		IIC_SDA=1;
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
	}	

	/*
	 * 函数功能：IIC发送一个字节
	 * 返回值:返回从机有无应答 1，有应答 0，无应答	
	 */ 
	void IIC_Send_Byte(u8 txd)
	{                        
		u8 t;   
		SDA_OUT(); 	    
		IIC_SCL=0;                                                                 
		for(t=0;t<8;t++)
		{              
			IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			delay_us(2);                                                          
			IIC_SCL=1;
			delay_us(2); 
			IIC_SCL=0;	
			delay_us(2);
		}	 
	} 	
	
	/*
	 * 函数功能：读1个字节 
	 * ack：ack=1时，发送ACK，ack=0，发送nACK 
	 * 返回值:receive：读取的数据
	 */
	u8 IIC_Read_Byte(unsigned char ack)
	{
		unsigned char i,receive=0;
		SDA_IN();                                                                  
		for(i=0;i<8;i++ )
		{
				IIC_SCL=0; 
				delay_us(2);
		IIC_SCL=1;
				receive<<=1;
				if(READ_SDA)receive++;   
		delay_us(1); 
		}					 
		if (!ack)
				IIC_NAck();                                                           
		else
				IIC_Ack();                                                            
		return receive;
	}
	
	/*
	 * 函数功能：IIC连续写字节 
	 * addr:器件地址 
	 * reg:寄存器地址
	 * len:写入长度
	 * buf:数据区
	 * 返回值：0,正常 其他,错误代码
	 */
	u8 Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
	{
		u8 i; 
		IIC_Start(); 
		//发送器件地址+写命令
		IIC_Send_Byte((addr<<1)|0);	
		//等待应答
		if(IIC_Wait_Ack())	
		{
			IIC_Stop();		 
			return 1;		
		}
		//写寄存器地址
		IIC_Send_Byte(reg);	
		//等待应答
		IIC_Wait_Ack();		
		for(i=0;i<len;i++)
		{
			//发送数据
			IIC_Send_Byte(buf[i]);	
			//等待ACK
			if(IIC_Wait_Ack())		
			{
				IIC_Stop();	 
				return 1;		 
			}		
		}    
		IIC_Stop();	 
		return 0;	
	} 

	/*
	 * 函数功能：IIC连续读字节 
	 * addr:器件地址 
	 * reg:要读取的寄存器地址
	 * len:要读取的长度
	 * buf:读取到的数据存储区
	 * 返回值：0,正常 其他,错误代码
	 */
	u8 Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
	{ 
		IIC_Start(); 
		//发送器件地址+写命令	
		IIC_Send_Byte((addr<<1)|0);
		//等待应答
		if(IIC_Wait_Ack())	
		{
			IIC_Stop();		 
			return 1;		
		}
		//写寄存器地址
		IIC_Send_Byte(reg);	
		//等待应答
		IIC_Wait_Ack();		
		IIC_Start();
		//发送器件地址+读命令	
		IIC_Send_Byte((addr<<1)|1);
		//等待应答 
		IIC_Wait_Ack();		
		while(len)
		{
			//读数据,发送nACK 
			if(len==1)*buf=IIC_Read_Byte(0);
			//读数据,发送ACK 
			else *buf=IIC_Read_Byte(1);		 
			len--;
			buf++; 
		}    
		//产生一个停止条件
		IIC_Stop(); 
		return 0;	
	}
	
/*软件IIC操作*/