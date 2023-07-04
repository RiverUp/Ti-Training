#pragma once

#ifdef __cplusplus
	extern "C" {
#endif

#include "sys.h"
#include "stdbool.h"
#include "stdint.h"
		
#define IIC_CONFIG 0																								//为1代表使用硬件IIC
#define READ_SDA    PBin(11)  																			//输入SDA 
#define IIC_SCL     PBout(10) 																			//SCL
#define IIC_SDA     PBout(11) 																			//SDA	
#define SCL_PORT GPIOB																							//SCL端口
#define SDA_PORT GPIOB																							//SDA端口
#define SCL_PIN GPIO_Pin_10																					//SCL管脚
#define SDA_PIN GPIO_Pin_11																					//SDA管脚	 
#define IIC_SCL_High()  GPIO_SetBits(SCL_PORT,SCL_PIN) 							//SCL_High
#define IIC_SCL_Low()   GPIO_ResetBits(SCL_PORT,SCL_PIN) 						//SCL_Low
#define IIC_SDA_High()  GPIO_SetBits(SDA_PORT,SDA_PIN) 							//SDA_High
#define IIC_SDA_Low()   GPIO_ResetBits(SDA_PORT,SDA_PIN) 						//SDA_Low
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<12;}	//SDA输入模式
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)3<<12;}  //SDA输出模式
#define SCL_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=(u32)3<<8;} 	//SCL输出模式


/*软件IIC操作*/
		
	/*
	 * 函数功能：IIC 延时函数
	 */
	void IIC_Delay(void);
	
	/*
	 * 函数功能：读取指定设备指定寄存器的一个值 
	 * I2C_Addr：设备IIC地址
	 * addr：寄存器地址
	 * 返回值:res：读取的数据
	 */
	unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
	/*
	 * 函数功能：初始化软件IIC
	 */
	void IIC_Init(void);
	/*
	 * 函数功能：产生IIC起始信号
	 */
	void IIC_Start(void);
		  
	/*
	 * 函数功能：产生IIC停止信号
	 */
	void IIC_Stop(void);

	/*
	 * 函数功能：等待应答信号到来
	 * 返回值:1，接收应答失败 0，接收应答成功
	 */
	u8 IIC_Wait_Ack(void);

	/*
	 * 函数功能：产生ACK应答
	 */
	void IIC_Ack(void);

	/*
	 * 函数功能：不产生ACK应答
	 */	    
	void IIC_NAck(void);

	/*
	 * 函数功能：IIC发送一个字节
	 * 返回值:返回从机有无应答 1，有应答 0，无应答	
	 */ 
	void IIC_Send_Byte(u8 txd);
	
	/*
	 * 函数功能：读1个字节 
	 * ack：ack=1时，发送ACK，ack=0，发送nACK 
	 * 返回值:receive：读取的数据
	 */
	u8 IIC_Read_Byte(unsigned char ack);
	
	/*
	 * 函数功能：IIC连续写字节 
	 * addr:器件地址 
	 * reg:寄存器地址
	 * len:写入长度
	 * buf:数据区
	 * 返回值：0,正常 其他,错误代码
	 */
	u8 Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);

	/*
	 * 函数功能：IIC连续读字节 
	 * addr:器件地址 
	 * reg:要读取的寄存器地址
	 * len:要读取的长度
	 * buf:读取到的数据存储区
	 * 返回值：0,正常 其他,错误代码
	 */
	u8 Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
	
/*软件IIC操作*/

#ifdef __cplusplus
	}
#endif
	

	