#pragma once

#ifdef __cplusplus
	extern "C" {
#endif

#include "sys.h"

//���AD0��(9��)�ӵ�,IIC��ַΪ0X68(���������λ)
//�����V3.3,��IIC��ַΪ0X69(���������λ)
#define MPU_ADDR										0X69
#define devAddr  										0xD0
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs�Ĵ���,�ɶ�ȡ�汾��,�Ĵ����ֲ�δ�ᵽ
#define MPU_PROD_ID_REG				0X0C	//prod id�Ĵ���,�ڼĴ����ֲ�δ�ᵽ
#define MPU_SELF_TESTX_REG		0X0D	//�Լ�Ĵ���X
#define MPU_SELF_TESTY_REG		0X0E	//�Լ�Ĵ���Y
#define MPU_SELF_TESTZ_REG		0X0F	//�Լ�Ĵ���Z
#define MPU_SELF_TESTA_REG		0X10	//�Լ�Ĵ���A
#define MPU_SAMPLE_RATE_REG		0X19	//����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG						0X1A	//���üĴ���
#define MPU_GYRO_CFG_REG			0X1B	//���������üĴ���
#define MPU_ACCEL_CFG_REG			0X1C	//���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG		0X1F	//�˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG				0X23	//FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG		0X24	//IIC�������ƼĴ���
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG				0X26	//IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG				0X29	//IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG				0X2C	//IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG				0X2F	//IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG				0X32	//IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG		0X33	//IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG		0X35	//IIC�ӻ�4�����ݼĴ���

#define MPU_I2CMST_STA_REG		0X36	//IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG			0X37	//�ж�/��·���üĴ���
#define MPU_INT_EN_REG				0X38	//�ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG				0X3A	//�ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG		0X3B	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG		0X3C	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG		0X3D	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG		0X3E	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG		0X3F	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG		0X40	//���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG			0X41	//�¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG			0X42	//�¶�ֵ��8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG		0X43	//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG		0X44	//������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG		0X45	//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG		0X46	//������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG		0X47	//������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG		0X48	//������ֵ,Z���8λ�Ĵ���

#define MPU_I2CSLV0_DO_REG		0X63	//IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG		0X64	//IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG		0X65	//IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG		0X66	//IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG	0X67	//IIC������ʱ����Ĵ���
#define MPU_SIGPATH_RST_REG		0X68	//�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG	0X69	//�˶������ƼĴ���
#define MPU_USER_CTRL_REG			0X6A	//�û����ƼĴ���
#define MPU_PWR_MGMT1_REG			0X6B	//��Դ����Ĵ���1
#define MPU_PWR_MGMT2_REG			0X6C	//��Դ����Ĵ���2 
#define MPU_FIFO_CNTH_REG			0X72	//FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG			0X73	//FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG				0X74	//FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG			0X75	//����ID�Ĵ���

//��ʼ��MPU6050
void init_drv_MPU6050();

/* IIC���� */
	
	/*
	 * �������ܣ�IICдһ���ֽ� 
	 * reg���Ĵ�����ַ
	 * data������
	 * ����ֵ:0,���� ����,�������
	 */
	u8 Write_Byte(u8 reg,u8 data);
	/*
	 * �������ܣ�IIC��һ���ֽ� 
	 * reg���Ĵ�����ַ
	 * ����ֵ:����������
	 */
	u8 Read_Byte(u8 reg);

/* IIC���� */

/* MPU6050���� */
	
	/*
	 * �������ܣ���ʼ��MPU6050
	 * ����ֵ:0,�ɹ� ����,�������
	 */
	u8 MPU_Init(void);
	/*
	 * �������ܣ�����MPU6050�����Ǵ����������̷�Χ 
	 * fsr��0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
	 * ����ֵ:0,���óɹ� ����,����ʧ�� 
	 */
	u8 MPU_Set_Gyro_Fsr(u8 fsr);
	/*
	 * �������ܣ�����MPU6050���ٶȴ����������̷�Χ
	 * fsr��0,��2g;1,��4g;2,��8g;3,��16g
	 * ����ֵ:0,���óɹ� ����,����ʧ�� 
	 */
	u8 MPU_Set_Accel_Fsr(u8 fsr);
	/*
	 * �������ܣ�����MPU6050�����ֵ�ͨ�˲���
	 * lpf�����ֵ�ͨ�˲�Ƶ��(Hz)
	 * ����ֵ:0,���óɹ� ����,����ʧ�� 
	 */
	u8 MPU_Set_LPF(u16 lpf);
	/*
	 * �������ܣ�����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
	 * rate��4~1000(Hz)
	 * ����ֵ:0,���óɹ� ����,����ʧ�� 
	 */
	u8 MPU_Set_Rate(u16 rate);

	/*
	 * �������ܣ��õ��¶�ֵ
	 * ����ֵ:�¶�ֵ(������10��)
	 */
	int MPU_Get_Temperature(void);
	/*
	 * �������ܣ��õ�������ֵ(ԭʼֵ)
	 * gx,gy,gz��������x,y,z���ԭʼ����(������)
	 * ����ֵ:0,�ɹ� ����,������� 
	 */
	u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
	/*
	 * �������ܣ��õ����ٶ�ֵ(ԭʼֵ)
	 * gx,gy,gz��������x,y,z���ԭʼ����(������)
	 * ����ֵ:0,�ɹ� ����,������� 
	 */
	u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
	/*
	 * �������ܣ���ȡMPU6050����DMP����̬��Ϣ
	 */
	void Read_DMP(void);
	/*
	 * �������ܣ���ȡMPU6050�汾��Ϣ
	 */
	void Mpu6050FindRevision(void);
	
	/*
	 * �������ܣ���ȡMPU6050ԭʼ��Ϣ
	 */
	uint8_t MPU6050_read(int32_t* acc, int32_t* gyro);
	
	/*
	 * �������ܣ���ȡ�Ƕ�	
	 * way����ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
	 */
	void Get_Angle(uint8_t way);
/*MPU6050����*/

#ifdef __cplusplus
	}
#endif