#include "Basic.hpp"
#include "drv_Uart1.hpp"
#include "Commulink.hpp"
#include "drv_MPU6050.hpp"
#include "Control.hpp"
#include "GUI.hpp"

u8 Uart1_Receive;
u8 Rec;
char USART1_RX_BUF[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
u16 USART1_RX_STA;				   // 接收状态标记
bool BeginReceiveFlag;
/*
 * 函数功能：串口1初始化
 * 入口参数：波特率
 * 返回  值：无
 */
void init_drv_Uart1(u32 bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructurea;
	USART_InitTypeDef USART_InitStructure;
	//使能USART1，GPIOA时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	
	USART_DeInit(USART3);
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructurea.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructurea.NVIC_IRQChannelCmd=ENABLE;
  NVIC_InitStructurea.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructurea.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructurea);
	
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	//初始化串口1
  USART_Init(USART1, &USART_InitStructure); 
	//开启串口接受中断
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//使能串口1 
  USART_Cmd(USART1, ENABLE);                    
}

/*
 * 函数功能：串口1接收中断
 * 入口参数：无
 * 返回  值：无
 */
extern "C" void USART1_IRQHandler(void)
{
	// 接收到数据
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		Rec = USART_ReceiveData(USART1); // 读取接收到的数据
		USART_SendData(USART1, Rec);	 // 接收数据再返回
		// 起始为x
		if (Rec == 'x')
		{
			BeginReceiveFlag = true;
		}
		if (BeginReceiveFlag)
		{
			if ((USART1_RX_STA & 0x8000) == 0) // 接收未完成 1000 0000 0000 0000
			{
				if (USART1_RX_STA & 0x4000) // 接收到了0x0d  0100 0000 0000 0000
				{
					if (Rec != 0x0a)
						USART1_RX_STA = 0; // 接收错误,重新开始 有0d无0a
					else
					{

						USART1_RX_STA |= 0x8000; // 接收完成了  0d后面是0a
						int msgLength = USART1_RX_STA & 0x3fff;
						// 处理函数
						int angle = atoi(USART1_RX_BUF);

						rotate_sg90(angle);

						USART1_RX_STA = 0;
						memset(USART1_RX_BUF, 0, msgLength);
						BeginReceiveFlag = false;

						// 测试sg90舵机
						//rt_sem_release(Sg90Response);
					}
				}
				else // 还没收到0X0D
				{
					if (Rec == 0x0d)
						USART1_RX_STA |= 0x4000; // 再次判断接收到的是不是0x0d
					else
					{
						USART1_RX_BUF[USART1_RX_STA & 0x3FFF] = Rec; // 0011 1111 1111 1111
						USART1_RX_STA++;
						if (USART1_RX_STA > (USART_REC_LEN - 1))
							USART1_RX_STA = 0; // 接收数据错误,重新开始接收
					}
				}
			}
		}
	}
}
