#include "Basic.hpp"
#include "drv_Uart2.hpp"
#include "Commulink.hpp"
u8 Cx=0,Cy=0,Cw=0,Ch=0,Ci=0;
char str[10];
char str1[10];
/*
 * 函数功能：串口2初始化
 * 入口参数：波特率
 * 返回  值：无
 */
void init_drv_Uart2(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	// 使能USART2，GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	// 重映射使能
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
	// USART2_TX   GPIOD.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	// USART2_RX	  GPIOD.6初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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
	// 初始化串口2
	USART_Init(USART2, &USART_InitStructure);
	// 开启串口接受中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	// 使能串口2
	USART_Cmd(USART2, ENABLE);
}

extern "C" void USART2_IRQHandler(void)
{
	u8 com_data;
	u8 i;
	static u8 RxCounter1 = 0;
	static u16 RxBuffer1[12] = {0};
	static u8 RxState = 0;
	static u8 RxFlag1 = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		com_data = USART_ReceiveData(USART2);
	
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
				
				Cx = RxBuffer1[RxCounter1 - 6];
				Cy = RxBuffer1[RxCounter1 - 5];
				Cw = RxBuffer1[RxCounter1 - 4];
				Ch = RxBuffer1[RxCounter1 - 3];
				Ci = RxBuffer1[RxCounter1-2];
			}
		}
		else if (RxState == 3) // 检测是否接受到结束标志
		{
			if (RxBuffer1[RxCounter1 - 1] == 0x5B)
			{
				//USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); // 关闭DTSABLE中断
				if (RxFlag1)
				{
					
					if (Cw)
						Track_Bias = Cx;
					else
						Track_Bias = -Cx;
					if(myabs(Track_Bias)>200)
					{
						Track_Bias=0;
					}
					// Ci检测是否为路口,计数经过了多少个路口
					switch (Ci)
					{
					case 1:
					case 2:
					case 3:
						if (!PassCrossFlag&&!Flag_Stop)
						{
							CrossFlag = true;
							PassCrossFlag = true;
							CrossNum++;
						}
						break;
					case 0:
						CrossFlag = false;
						break;
					case 4:
						if(!ArrivedFlag&&!PassCrossFlag)
						{
							ArrivedFlag = true;
							TrackFlag=false;
							ArrivedNum++;
							//TargetVelocity=0;
							Turn180Flag=true;
							Track_Bias=0;
							ReturnFlag=true;
							if(ArrivedNum==2)
							{
								ArrivedNum=0;
							}
						}
						break;
					default:
						break;
					}
				}
				RxFlag1 = 0;
				RxCounter1 = 0;
				RxState = 0;
				//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
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
		
		//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}