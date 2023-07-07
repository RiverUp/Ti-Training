#include "pwm.h"
#include "SYS.h"
#include "usart.h"
extern int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;

/**************************************************************************
�������ܣ�pwm��ʼ��TIM4
��ڲ�����arr����Ϊһ��ʱ��Ƶ�ʵ����ֵ  psc�� Ԥ��Ƶֵ
����  ֵ����
**************************************************************************/


void pwm_int_TIM4(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;                 //����ṹ��GPIO_InitStructure
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //����ṹTIM_TimeBaseStructure
    NVIC_InitTypeDef NVIC_InitStructure;									 //����ṹNVIC_InitStructure
    TIM_OCInitTypeDef  TIM_OCInitStructure;               //����ṹTIM_OCInitStructure

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //ʹ�ܶ�ʱ��4ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 | GPIO_Pin_7;          //PB6 PB7 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO���ٶ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     	//����ģʽ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);              //GBIOB��ʼ��

    TIM_TimeBaseStructure.TIM_Period = arr;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;   //PWMTIM���巭תģʽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_CounterMode_Up;       //����TIM����Ƚϼ���Ϊ��
    TIM_OCInitStructure.TIM_OutputState=	TIM_OutputState_Enable;   //�Ƚ����ʹ��

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStructure��ָ���Ĳ�����ʼ������TIM4
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);	    //ʹ��Ԥװ�ؼĴ���

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStructure��ָ���Ĳ�����ʼ������TIM4
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);	    //ʹ��Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM4, ENABLE);      //ʹ���Զ�װ������λ

    TIM_Cmd(TIM4, ENABLE);   //������ʱ��TIM4

    TIM_ITConfig(TIM4, TIM_IT_CC1|TIM_IT_CC2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}


/**************************************************************************
�������ܣ�TIM4�жϷ����� ���������ƵPWM���
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM4_IRQHandler(void)
{
    float Capture1,Capture2;
    if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );//���TIMx���жϴ�����λ
        Capture1 = TIM_GetCapture1(TIM4);
        TIM_SetCompare1(TIM4, Capture1 + Final_Moto2 );//����TIMx�Զ���װ�ؼĴ���ֵ
    }
    if(TIM_GetITStatus(TIM4,TIM_IT_CC2)!=RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2 );//���TIMx���жϴ�����λ
        Capture2 = TIM_GetCapture2(TIM4);
        TIM_SetCompare2(TIM4, Capture2 + Final_Moto1 );//����TIMx�Զ���װ�ؼĴ���ֵ
    }
}

