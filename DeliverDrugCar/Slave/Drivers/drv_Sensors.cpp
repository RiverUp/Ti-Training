#include "drv_MPU6050.hpp"
#include "drv_ExtIIC.hpp"
#include "drv_Sensors.hpp"
#include "delay.h"
#include "Basic.hpp"
#include "drv_LED.hpp"
#include "inv_mpu.h"

void init_drv_Sensors()
{
	/*MPU6050初始化*/
		MPU_Init();
		//等待MPU6050初始化完成
		while(mpu_dmp_init());
	/*MPU6050初始化*/
}
