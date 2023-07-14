#pragma once

#ifdef __cplusplus
	extern "C" {
#endif

#include "sys.h"

//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位)
//如果接V3.3,则IIC地址为0X69(不包含最低位)
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
#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
#define MPU_PROD_ID_REG				0X0C	//prod id寄存器,在寄存器手册未提到
#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG						0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG			0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG			0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG				0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG				0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG				0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG				0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG				0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG				0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG			0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG				0X38	//中断使能寄存器
#define MPU_INT_STA_REG				0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG			0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG			0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG			0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG			0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG			0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG			0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG			0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG				0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG			0X75	//器件ID寄存器

//初始化MPU6050
void init_drv_MPU6050();

/* IIC操作 */
	
	/*
	 * 函数功能：IIC写一个字节 
	 * reg：寄存器地址
	 * data：数据
	 * 返回值:0,正常 其他,错误代码
	 */
	u8 Write_Byte(u8 reg,u8 data);
	/*
	 * 函数功能：IIC读一个字节 
	 * reg：寄存器地址
	 * 返回值:读到的数据
	 */
	u8 Read_Byte(u8 reg);

/* IIC操作 */

/* MPU6050操作 */
	
	/*
	 * 函数功能：初始化MPU6050
	 * 返回值:0,成功 其他,错误代码
	 */
	u8 MPU_Init(void);
	/*
	 * 函数功能：设置MPU6050陀螺仪传感器满量程范围 
	 * fsr：0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
	 * 返回值:0,设置成功 其他,设置失败 
	 */
	u8 MPU_Set_Gyro_Fsr(u8 fsr);
	/*
	 * 函数功能：设置MPU6050加速度传感器满量程范围
	 * fsr：0,±2g;1,±4g;2,±8g;3,±16g
	 * 返回值:0,设置成功 其他,设置失败 
	 */
	u8 MPU_Set_Accel_Fsr(u8 fsr);
	/*
	 * 函数功能：设置MPU6050的数字低通滤波器
	 * lpf：数字低通滤波频率(Hz)
	 * 返回值:0,设置成功 其他,设置失败 
	 */
	u8 MPU_Set_LPF(u16 lpf);
	/*
	 * 函数功能：设置MPU6050的采样率(假定Fs=1KHz)
	 * rate：4~1000(Hz)
	 * 返回值:0,设置成功 其他,设置失败 
	 */
	u8 MPU_Set_Rate(u16 rate);

	/*
	 * 函数功能：得到温度值
	 * 返回值:温度值(扩大了10倍)
	 */
	int MPU_Get_Temperature(void);
	/*
	 * 函数功能：得到陀螺仪值(原始值)
	 * gx,gy,gz：陀螺仪x,y,z轴的原始读数(带符号)
	 * 返回值:0,成功 其他,错误代码 
	 */
	u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
	/*
	 * 函数功能：得到加速度值(原始值)
	 * gx,gy,gz：陀螺仪x,y,z轴的原始读数(带符号)
	 * 返回值:0,成功 其他,错误代码 
	 */
	u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
	/*
	 * 函数功能：读取MPU6050内置DMP的姿态信息
	 */
	void Read_DMP(void);
	/*
	 * 函数功能：读取MPU6050版本信息
	 */
	void Mpu6050FindRevision(void);
	
	/*
	 * 函数功能：读取MPU6050原始信息
	 */
	uint8_t MPU6050_read(int32_t* acc, int32_t* gyro);
	
	/*
	 * 函数功能：获取角度	
	 * way：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
	 */
	void Get_Angle(uint8_t way);
/*MPU6050操作*/

#ifdef __cplusplus
	}
#endif