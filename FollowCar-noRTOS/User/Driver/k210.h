#ifndef __K210_H
#define __K210_H
#define K210_UART EUSCI_A3_BASE   							 	// K210使用串口
#define K210_Uart_RX_LENGTH_MAX 50 								// K210 UART最大接收长度
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h> 
#include "delay.h"
#include "Serial.h"
#include "string.h"

//extern uint8_t double_centre=0;//whether cross or not
extern char k210_data[20];
extern uint8_t data_k210_Ptr;
extern bool K210_RecCompleteFlag;


void init_K210(void);
void K210_Translate(uint8_t *Tx);
#endif
