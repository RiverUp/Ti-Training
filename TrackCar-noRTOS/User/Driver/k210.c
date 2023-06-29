#include "k210.h"	

char k210_data[20];
uint8_t data_k210_Ptr;
bool K210_RecCompleteFlag;

//设置波特率为115200
const eUSCI_UART_ConfigV1 uartConfig_K210 =
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


 

//UART2:K210
void init_K210()
{

	//设置smclk为12mhz
	CS_setDCOFrequency(CS_12MHZ);
	
	//配置uart引脚 rx=p9.6,tx=p9.7
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9,
            GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	
	//初始化配置
	UART_initModule(K210_UART, &uartConfig_K210);
	UART_enableModule(K210_UART);
	
	//使能中断
	UART_enableInterrupt(K210_UART, EUSCI_A_UART_RECEIVE_INTERRUPT);
	Interrupt_enableInterrupt(INT_EUSCIA3);
	Interrupt_enableMaster();
}



void EUSCIA3_IRQHandler()
{
	uint32_t status=UART_getEnabledInterruptStatus(K210_UART);
	
	if(status&EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
	{
    UART_clearInterruptFlag(K210_UART,EUSCI_A_UART_RECEIVE_INTERRUPT);				//turn on interrupt for k210
		uint8_t dat=UART_receiveData(K210_UART);																	//receive the data one by one
		if(dat!='0')																																	//judge receive over or not
		{
			k210_data[data_k210_Ptr++]=dat;
		}
		else
		{
			K210_RecCompleteFlag=true;
		}
			
	}

//		MAP_UART_transmitData(K210_UART, MAP_UART_receiveData(K210_UART));
}

//K210数据发送函数
void K210_Translate(uint8_t *Tx)
{
	for(int i=0;i<8;i++)
	{
		MAP_UART_transmitData(K210_UART,Tx[i]);
	}
}
 
