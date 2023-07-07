#include "Serial.h"

char serialDataBuffer[SERIAL_BUFFER_SIZE];
int serialBufferPtr;

void initSerial()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTFIFODisable(UART0_BASE);
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX);
}

void sendMsgBySerial(char *msg)
{
    while(*msg)
    {
        UARTCharPut(UART0_BASE, *msg++);
    }
}
