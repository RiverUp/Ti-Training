#include "Basic.hpp"
#include "drv_Uart1.hpp"
#include "Commulink.hpp"
#include "drv_MPU6050.hpp"
#include "Control.hpp"
#include "GUI.hpp"

static u8 readNum;
static char str[50];

void init_drv_Uart1(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	// 使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	// USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	// USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 初始化串口1
	USART_Init(USART1, &USART_InitStructure);
	// 开启串口接受中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// 使能串口1
	USART_Cmd(USART1, ENABLE);
}

// USART1 全局中断服务函数
extern "C" void  USART1_IRQHandler(void)
{
	u8 com_data;
	u8 i;
	char str[20];
	static u8 RxCounter1 = 0;
	static u16 RxBuffer1[10] = {0};
	static u8 RxState = 0;
	static u8 RxFlag1 = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
		com_data = USART_ReceiveData(USART1);
		
		sprintf(str,"%d",com_data);
		LCD_ShowString(0,176,str,WHITE,BLACK,16,0);
		//LCD_Fill(0,0,240,240,BLUE);

		if (RxState == 0 && com_data == 0x2C) // 0x2c帧头
		{
			RxState = 1;
			RxBuffer1[RxCounter1++] = com_data;
		}

		else if (RxState == 1 && com_data == 0x12) // 0x12帧头
		{
			RxState = 2;
			RxBuffer1[RxCounter1++] = com_data;
		}

		else if (RxState == 2)
		{
			RxBuffer1[RxCounter1++] = com_data;

			if (RxCounter1 >= 10 || com_data == 0x5B) // RxBuffer1接受满了,接收数据结束
			{
				RxState = 3;
				RxFlag1 = 1;

				readNum = RxBuffer1[RxCounter1 - 2];
			}
		}

		else if (RxState == 3) // 检测是否接受到结束标志
		{
			if (RxBuffer1[RxCounter1 - 1] == 0x5B)
			{
				// USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); // 关闭DTSABLE中断
				if (RxFlag1)
				{
					// 业务代码
					IdentifiedNum = readNum;
					switch (readNum)
					{
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
						break;
					case 11:
						break;
					case 12:
						break;
					default:
						break;
					}
				}
				RxFlag1 = 0;
				RxCounter1 = 0;
				RxState = 0;
				// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			}
			else // 接收错误
			{
				RxState = 0;
				RxCounter1 = 0;
				for (i = 0; i < 10; i++)
				{
					RxBuffer1[i] = 0x00; // 将存放数据数组清零
				}
			}
		}

		else // 接收异常
		{
			RxState = 0;
			RxCounter1 = 0;
			for (i = 0; i < 10; i++)
			{
				RxBuffer1[i] = 0x00; // 将存放数据数组清零
			}
		}
	}
}
