#include "HC-05.h"

//

char btdata[20];
uint8_t dataPtr;
bool BTRecCompleteFlag;

//���ò�����Ϊ9600
const eUSCI_UART_ConfigV1 uartConfigForHC05 =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 6
        2,                                       // UCxBRF = 8
        0x0,                                       // UCxBRS = 0x20
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};


//����p3.2(RX),p3.3(TX)
void init_hc05(void)
{
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN3|GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION);
	UART_initModule(EUSCI_A2_BASE,&uartConfigForHC05);
	UART_enableModule(EUSCI_A2_BASE);
		//ʹ���ж�
	UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	Interrupt_enableInterrupt(INT_EUSCIA2);
}

void sendMsgByBlueTooth(char *msg)
{
	while(*msg)
	{
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		UART_transmitData(EUSCI_A2_BASE,*msg);
		msg++;
	} 
}
