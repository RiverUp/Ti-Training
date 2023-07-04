#include "Serial.h"

//设置波特率为115200
const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        6,                                     // BRDIV = 6
        8,                                       // UCxBRF = 8
        0x20,                                       // UCxBRS = 0x20
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};

void initSerial()
{
	//设置smclk为12mhz
	CS_setDCOFrequency(CS_12MHZ);
	
	//配置p1.2，p1.3为uart引脚
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
	
	//初始化配置
	UART_initModule(EUSCI_A0_BASE, &uartConfig);
	UART_enableModule(EUSCI_A0_BASE);
	
	//使能中断
//	UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
//	Interrupt_enableInterrupt(INT_EUSCIA0);
//	Interrupt_enableMaster();
}

void sendText(char *string)
{
	while(*string)
	{
		UART_transmitData(EUSCI_A0_BASE,*string);
		string++;
	}
}

//void EUSCIA0_IRQHandler()
//{
//	uint32_t status=UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
//	
//	if(status&EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//	{
//    UART_clearInterruptFlag(EUSCI_A0_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT);
//		MAP_UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
//	}
//}


