#include "drv_Encoder.hpp"
#include "stm32f10x_gpio.h"
#include "Control.hpp"	
/*
 * �������ܣ���TIM3��ʼ��Ϊ�������ӿ�ģʽ
 * ��ڲ�������
 * ����  ֵ����
 */
void drv_Encoder_Init_TIM3(void)
{
	// PC6  ---> E1A  T3C1
	// PC7  ---> E1B  T3C2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//��ӳ��ʹ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);					      
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	//�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 
	//ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM���ϼ���  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//ʹ�ñ�����ģʽ3
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
	//�˲�10
  TIM_ICInitStructure.TIM_ICFilter = 10;	
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	//���TIM�ĸ��±�־λ
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM3,0);
  TIM_Cmd(TIM3, ENABLE); 
}

/*
 * �������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
 * ��ڲ�������
 * ����  ֵ����
 */
void drv_Encoder_Init_TIM4(void)
{
	// PD12 ---> E2A  T4C1
	// PD13 ---> E2B  T4C2
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�ܶ�ʱ��4��ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//ʹ��PB�˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//��ӳ��ʹ��
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);					      
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 10;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //Reset counter
  TIM_SetCounter(TIM4,0);
  TIM_Cmd(TIM4, ENABLE); 
}
/*
 * �������ܣ���λʱ���ȡ����������
 * ��ڲ�����TIMX����ʱ��
 * ����  ֵ���ٶ�ֵ
 */
int Read_Encoder(u8 TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
		 #if Debug
				#if Debug_pos
				 case 2:  Encoder_TIM= (short)TIM2 -> CNT;break;
				 case 3:  Encoder_TIM= (short)TIM3 -> CNT;break;	
				 case 4:  Encoder_TIM= (short)TIM4 -> CNT;break;	
				 default: Encoder_TIM=0;
				#elif Debug_vel
				 case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
				 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
				 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
				 default: Encoder_TIM=0;
		    #endif
		 #else
		 case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 default: Encoder_TIM=0;
		 #endif
	 }
		return Encoder_TIM;
}
/*
 * �������ܣ�TIM4�жϷ�����
 * ��ڲ�������
 * ����  ֵ����
 */
extern "C" void TIM4_IRQHandler(void)
{ 		    		  		
	//����ж�	
	if(TIM4->SR&0X0001)
	{    				   				     	    	
	}				   
	//����жϱ�־λ 
	TIM4->SR&=~(1<<0);	    
}
/*
�������ܣ�TIM3�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
extern "C" void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)
	{    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);	    
}
