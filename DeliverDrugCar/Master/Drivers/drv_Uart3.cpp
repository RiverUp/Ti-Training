#include "Basic.hpp"
#include "drv_Uart3.hpp"
#include "Commulink.hpp"

/*
 * 函数功能：串口3初始化
 * 入口参数：波特率
 * 返回  值：无
 */
void init_drv_Uart3(u32 bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//使能USART3，GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//重映射使能
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
	//USART3_TX   GPIOD.8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //USART3_RX	  GPIOD.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	//初始化串口3
  USART_Init(USART3, &USART_InitStructure); 
	//开启串口接受中断
  //USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	//使能串口3 
  USART_Cmd(USART3, ENABLE);                    
}