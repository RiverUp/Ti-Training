#pragma once

#ifdef __cplusplus
	extern "C" {
#endif

#include "sys.h"
#include "stdbool.h"
#include "stdint.h"
		
#define IIC_CONFIG 0																								//Ϊ1����ʹ��Ӳ��IIC
#define READ_SDA    PBin(11)  																			//����SDA 
#define IIC_SCL     PBout(10) 																			//SCL
#define IIC_SDA     PBout(11) 																			//SDA	
#define SCL_PORT GPIOB																							//SCL�˿�
#define SDA_PORT GPIOB																							//SDA�˿�
#define SCL_PIN GPIO_Pin_10																					//SCL�ܽ�
#define SDA_PIN GPIO_Pin_11																					//SDA�ܽ�	 
#define IIC_SCL_High()  GPIO_SetBits(SCL_PORT,SCL_PIN) 							//SCL_High
#define IIC_SCL_Low()   GPIO_ResetBits(SCL_PORT,SCL_PIN) 						//SCL_Low
#define IIC_SDA_High()  GPIO_SetBits(SDA_PORT,SDA_PIN) 							//SDA_High
#define IIC_SDA_Low()   GPIO_ResetBits(SDA_PORT,SDA_PIN) 						//SDA_Low
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<12;}	//SDA����ģʽ
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)3<<12;}  //SDA���ģʽ
#define SCL_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=(u32)3<<8;} 	//SCL���ģʽ


/*���IIC����*/
		
	/*
	 * �������ܣ�IIC ��ʱ����
	 */
	void IIC_Delay(void);
	
	/*
	 * �������ܣ���ȡָ���豸ָ���Ĵ�����һ��ֵ 
	 * I2C_Addr���豸IIC��ַ
	 * addr���Ĵ�����ַ
	 * ����ֵ:res����ȡ������
	 */
	unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
	/*
	 * �������ܣ���ʼ�����IIC
	 */
	void IIC_Init(void);
	/*
	 * �������ܣ�����IIC��ʼ�ź�
	 */
	void IIC_Start(void);
		  
	/*
	 * �������ܣ�����IICֹͣ�ź�
	 */
	void IIC_Stop(void);

	/*
	 * �������ܣ��ȴ�Ӧ���źŵ���
	 * ����ֵ:1������Ӧ��ʧ�� 0������Ӧ��ɹ�
	 */
	u8 IIC_Wait_Ack(void);

	/*
	 * �������ܣ�����ACKӦ��
	 */
	void IIC_Ack(void);

	/*
	 * �������ܣ�������ACKӦ��
	 */	    
	void IIC_NAck(void);

	/*
	 * �������ܣ�IIC����һ���ֽ�
	 * ����ֵ:���شӻ�����Ӧ�� 1����Ӧ�� 0����Ӧ��	
	 */ 
	void IIC_Send_Byte(u8 txd);
	
	/*
	 * �������ܣ���1���ֽ� 
	 * ack��ack=1ʱ������ACK��ack=0������nACK 
	 * ����ֵ:receive����ȡ������
	 */
	u8 IIC_Read_Byte(unsigned char ack);
	
	/*
	 * �������ܣ�IIC����д�ֽ� 
	 * addr:������ַ 
	 * reg:�Ĵ�����ַ
	 * len:д�볤��
	 * buf:������
	 * ����ֵ��0,���� ����,�������
	 */
	u8 Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);

	/*
	 * �������ܣ�IIC�������ֽ� 
	 * addr:������ַ 
	 * reg:Ҫ��ȡ�ļĴ�����ַ
	 * len:Ҫ��ȡ�ĳ���
	 * buf:��ȡ�������ݴ洢��
	 * ����ֵ��0,���� ����,�������
	 */
	u8 Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
	
/*���IIC����*/

#ifdef __cplusplus
	}
#endif
	

	