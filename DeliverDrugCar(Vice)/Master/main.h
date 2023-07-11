#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"
#include "inc/hw_pwm.h"
#include "inc/hw_adc.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ssi.h"
#include "inc/hw_qei.h"
#include "inc/tm4c123gh6pm.h"

#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/i2c.h"
#include "utils/uartstdio.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/mpu6050.h"

// #include "Drivers/Mpu6050/iic_hard.h"
// #include "Drivers/Mpu6050/mpu6050.h"
// #include "Drivers/Mpu6050/inv_mpu.h"

#include "Drivers/Mpu6050.h"

//中断优先级定义
#define  USER_INT0  0x00
#define  USER_INT1  0x20
#define  USER_INT2  0x40
#define  USER_INT3  0x60
#define  USER_INT4  0x80
#define  USER_INT5  0xA0
//延时函数定义
#define delayms(xms)	SysCtlDelay(xms*(80000/3)) /*ms级延时*/
#define delayus(xus)	SysCtlDelay(xus*(80/3)) /*ms级延时*/


#endif
