#include "HC-05.h"

//


rt_sem_t ProcessBtData;
uint8_t state=0;//判断是否换行
char btdata[20];
uint8_t dataPtr;

//设置波特率为9600
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


//连接p3.2(RX),p3.3(TX)
void init_hc05(void)
{
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2|GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION);
	UART_initModule(EUSCI_A2_BASE,&uartConfigForHC05);
	UART_enableModule(EUSCI_A2_BASE);
		//使能中断
	UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	Interrupt_enableInterrupt(INT_EUSCIA0);
}

void sendMsgByBlueTooth(char *msg)
{
	while(msg)
	{
		UART_transmitData(EUSCI_A2_BASE,*msg);
		msg++;
	} 
}

//收到蓝牙信息通过中断回调处理
void EUSCIA2_IRQHandler()
{
	uint32_t status=UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
	
	if(status&EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
	{
    UART_clearInterruptFlag(EUSCI_A2_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT);
		uint8_t dat=UART_receiveData(EUSCI_A2_BASE);
		if(dat=='\r')
		{
			state=1;
		}
		if(dat=='\n'&&state==1)
		{
			state=2;
		}
	  btdata[dataPtr++]=dat;
		if(state==2)
		{
			rt_sem_release(ProcessBtData);
			UART_disableInterrupt(EUSCI_A2_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT);
			state=0;
		}
			
		//MAP_UART_transmitData(EUSCI_A0_BASE, UART_receiveData(EUSCI_A0_BASE));
		//handleMsgFromBlueTooth();
	}
}
