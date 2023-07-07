#include "main.h"

#include "string.h"
#include "stdlib.h"

#include "Serial.h"
#include "Flag.h"
#include "Sg90.h"

/**
 * main.c
 */
int main(void)
{

	MAP_FPUEnable();
	MAP_FPULazyStackingEnable();

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	IntMasterEnable();

	initSerial();
	initSg90();

	// 配置小灯用于测试
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

	//rotateSg90(90);
	while (1)
	{
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
