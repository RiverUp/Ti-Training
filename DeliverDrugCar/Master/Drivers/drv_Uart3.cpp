#include "Basic.hpp"
#include "drv_Uart1.hpp"
#include "Commulink.hpp"
#include "drv_MPU6050.hpp"
#include "Control.hpp"
#include "drv_Uart2.hpp"
#include "GUI.hpp"

static u8 readNum;
static char str[50];

void init_drv_Uart3(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	// 使能USART1，GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	// 重映射使能
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	// USART1_TX   GPIOD.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	// USART1_RX	  GPIOD.9初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
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
	USART_Init(USART3, &USART_InitStructure);
	// 开启串口接受中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	// 使能串口1
	USART_Cmd(USART3, ENABLE);
}

// USART1 全局中断服务函数
extern "C" void USART3_IRQHandler(void)
{
	u8 com_data;
	u8 i;
	char str[20];
	static u8 RxCounter1 = 0;
	static u16 RxBuffer1[10] = {0};
	static u8 RxState = 0;
	static u8 RxFlag1 = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 接收中断
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); // 清除中断标志
		com_data = USART_ReceiveData(USART3);

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
					
					switch (readNum)
					{
					case 1:
					case 2:
						StopCrossNum=1;
						CloseWard=true;
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
						StopCrossNum=2;
						CloseWard=false;
						//init_drv_Uart2(115200);
						IdentifiedNum = readNum;
						break;
					case 11:
					case 12:
						JudgingFlag=false;
						JudgingCount=0;
						TurnSignal=readNum;
						ReadyStopFlag=true;
						TargetVelocity=20;
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