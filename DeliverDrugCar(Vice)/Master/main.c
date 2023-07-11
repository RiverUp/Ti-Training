#include "main.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// #include "MyMpu6050.h"
#include "Serial.h"
#include "Flag.h"
#include "Sg90.h"
#include "Mpu6050.h"

/**
 * main.c
 */
int main(void)
{

	MAP_FPUEnable();
	MAP_FPULazyStackingEnable();

	// 16mhz
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	IntMasterEnable();

	initSerial();
	initMpu6050();

	// 配置小灯用于测试
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

	// rotateSg90(90);
	while (1)
	{
		readMpu6050(ax,ay,az,gx,gy,gz,temperature);
		// 检测Mpu6050是否有数据
		char mpu_data[50];
		sprintf(mpu_data, "pitch: %.2f, roll: %.2f, yaw: %.2f\r\n", *ax, *ay, *az);
		sendMsgBySerial(mpu_data);
		// 处理串口指令
		if (SerialCompleteFlag)
		{
			if (strcmp(serialDataBuffer, "on") == 0)
			{
				// if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
				// 	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
				// else
				// 	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
			}
			if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
			else
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
			int angle = atoi(serialDataBuffer);
			rotateSg90(angle);
			SerialCompleteFlag = false;
			sendMsgBySerial(serialDataBuffer);
			memset(serialDataBuffer, 0, SERIAL_BUFFER_SIZE);
			serialBufferPtr = 0;
		}
	}
}