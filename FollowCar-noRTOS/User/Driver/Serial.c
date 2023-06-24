#include "Serial.h"

//���ò�����Ϊ115200
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
	//����smclkΪ12mhz
	CS_setDCOFrequency(CS_12MHZ);
	
	//����p1.2��p1.3Ϊuart����
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
	
	//��ʼ������
	UART_initModule(EUSCI_A0_BASE, &uartConfig);
	UART_enableModule(EUSCI_A0_BASE);
	
	//ʹ���ж�
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


