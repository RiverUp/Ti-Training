#ifndef __MPU6050_H
#define __MPU6050_H

#define IMU_MPU6050_ID   0x68
#define IMU_ICM20689_ID  0x98
//****************************************
// MPU6050
//****************************************
// acceleration due to gravity in m/s/s
#define GRAVITY_MSS 9.80665f
// Gyroscope scale (uncertain where the 0.01745 value comes from)
#define GYRO_SCALE_2000  (0.0174532f / 16.4f)
#define GYRO_SCALE_1000  (0.0174532f / 32.8f)
#define GYRO_SCALE_500   (0.0174532f / 65.5f)
#define SCALE_250  			 (0.0174532f / 131f)
// Accelerometer scale adjustment
#define ACCEL_SCALE_16G   (GRAVITY_MSS / 2048.0f)
#define ACCEL_SCALE_8G    (GRAVITY_MSS / 4096.0f)
#define ACCEL_SCALE_4G    (GRAVITY_MSS / 8192.0f)
#define ACCEL_SCALE_2G    (GRAVITY_MSS / 16384.0f)
#define	SMPLRT_DIV		0x19
#define	MPU_CONFIG		0x1A
#define	GYRO_CONFIG		0x1B
#define	ACCEL_CONFIG  0x1C
#define ACCEL_CONFIG2 0x1D
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B
#define	WHO_AM_I		  0x75
#define USER_CTRL		  0x6A
#define INT_PIN_CFG		0x37


#define MPU_SELF_TESTX_REG		0X0D
#define MPU_SELF_TESTY_REG		0X0E
#define MPU_SELF_TESTZ_REG		0X0F
#define MPU_SELF_TESTA_REG		0X10
#define MPU_SAMPLE_RATE_REG		0X19
#define MPU_CFG_REG				0X1A
#define MPU_GYRO_CFG_REG		0X1B
#define MPU_ACCEL_CFG_REG		0X1C
#define MPU_MOTION_DET_REG		0X1F
#define MPU_FIFO_EN_REG			0X23
#define MPU_I2CMST_CTRL_REG		0X24
#define MPU_I2CSLV0_ADDR_REG	0X25
#define MPU_I2CSLV0_REG			0X26
#define MPU_I2CSLV0_CTRL_REG	0X27
#define MPU_I2CSLV1_ADDR_REG	0X28
#define MPU_I2CSLV1_REG			0X29
#define MPU_I2CSLV1_CTRL_REG	0X2A
#define MPU_I2CSLV2_ADDR_REG	0X2B
#define MPU_I2CSLV2_REG			0X2C
#define MPU_I2CSLV2_CTRL_REG	0X2D
#define MPU_I2CSLV3_ADDR_REG	0X2E
#define MPU_I2CSLV3_REG			0X2F
#define MPU_I2CSLV3_CTRL_REG	0X30
#define MPU_I2CSLV4_ADDR_REG	0X31
#define MPU_I2CSLV4_REG			0X32
#define MPU_I2CSLV4_DO_REG		0X33
#define MPU_I2CSLV4_CTRL_REG	0X34
#define MPU_I2CSLV4_DI_REG		0X35

#define MPU_I2CMST_STA_REG		0X36
#define MPU_INTBP_CFG_REG		0X37
#define MPU_INT_EN_REG			0X38
#define MPU_INT_STA_REG			0X3A

#define MPU_ACCEL_XOUTH_REG		0X3B
#define MPU_ACCEL_XOUTL_REG		0X3C
#define MPU_ACCEL_YOUTH_REG		0X3D
#define MPU_ACCEL_YOUTL_REG		0X3E
#define MPU_ACCEL_ZOUTH_REG		0X3F
#define MPU_ACCEL_ZOUTL_REG		0X40

#define MPU_TEMP_OUTH_REG		0X41
#define MPU_TEMP_OUTL_REG		0X42

#define MPU_GYRO_XOUTH_REG		0X43
#define MPU_GYRO_XOUTL_REG		0X44
#define MPU_GYRO_YOUTH_REG		0X45
#define MPU_GYRO_YOUTL_REG		0X46
#define MPU_GYRO_ZOUTH_REG		0X47
#define MPU_GYRO_ZOUTL_REG		0X48

#define MPU_I2CSLV0_DO_REG		0X63
#define MPU_I2CSLV1_DO_REG		0X64
#define MPU_I2CSLV2_DO_REG		0X65
#define MPU_I2CSLV3_DO_REG		0X66

#define MPU_I2CMST_DELAY_REG	0X67
#define MPU_SIGPATH_RST_REG		0X68	
#define MPU_MDETECT_CTRL_REG	0X69
#define MPU_USER_CTRL_REG		0X6A
#define MPU_PWR_MGMT1_REG		0X6B
#define MPU_PWR_MGMT2_REG		0X6C
#define MPU_FIFO_CNTH_REG		0X72
#define MPU_FIFO_CNTL_REG		0X73
#define MPU_FIFO_RW_REG			0X74
#define MPU_DEVICE_ID_REG		0X75

#define MPU_ADDR				0X68

#include "main.h"

extern float *ax,*ay,*az;
extern float *gx,*gy,*gz;
extern float *temperature;

void initMpu6050();
void readMpu6050(float *ax, float *ay, float *az, float *gx, float *gy,float *gz,float *temperature);
void i2cWrite(uint8_t addr, uint8_t regAddr, uint8_t data) ;
uint8_t i2cRead(uint8_t addr, uint8_t regAddr);
void i2cReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length);
void Single_WriteI2C(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address);
short int Double_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address);

#endif // !__MPU6050_H
