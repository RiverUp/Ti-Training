#include "Basic.hpp"
#include "drv_Uart4.hpp"
#include "Commulink.hpp"

/*
 * 函数功能：串口4初始化
 * 入口参数：波特率
 * 返回  值：无
 */
void init_drv_Uart4(u32 bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//使能USART4，GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//USART4_TX   GPIOC.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //USART4_RX	  GPIOC.11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	//初始化串口4
  USART_Init(UART4, &USART_InitStructure); 
	//开启串口接受中断
  //USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
	//使能串口4
  USART_Cmd(UART4, ENABLE);                    
}