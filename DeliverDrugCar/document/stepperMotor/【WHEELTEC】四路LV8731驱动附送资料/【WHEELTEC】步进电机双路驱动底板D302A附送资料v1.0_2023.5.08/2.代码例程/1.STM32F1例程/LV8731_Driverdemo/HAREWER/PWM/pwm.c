#include "pwm.h"
#include "SYS.h"
#include "usart.h"
extern int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;

/**************************************************************************
函数功能：pwm初始化TIM4
入口参数：arr：设为一个时钟频率的最大值  psc： 预分频值
返回  值：无
**************************************************************************/


void pwm_int_TIM4(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;                 //定义结构体GPIO_InitStructure
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //定义结构TIM_TimeBaseStructure
    NVIC_InitTypeDef NVIC_InitStructure;									 //定义结构NVIC_InitStructure
    TIM_OCInitTypeDef  TIM_OCInitStructure;               //定义结构TIM_OCInitStructure

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //使能定时器4时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 | GPIO_Pin_7;          //PB6 PB7 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO口速度
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     	//复用模式输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);              //GBIOB初始化

    TIM_TimeBaseStructure.TIM_Period = arr;    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;   //PWMTIM脉冲翻转模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_CounterMode_Up;       //设置TIM输出比较极性为高
    TIM_OCInitStructure.TIM_OutputState=	TIM_OutputState_Enable;   //比较输出使能

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStructure中指定的参数初始化外设TIM4
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);	    //使能预装载寄存器

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStructure中指定的参数初始化外设TIM4
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);	    //使能预装载寄存器

    TIM_ARRPreloadConfig(TIM4, ENABLE);      //使能自动装载允许位

    TIM_Cmd(TIM4, ENABLE);   //启动定时器TIM4

    TIM_ITConfig(TIM4, TIM_IT_CC1|TIM_IT_CC2, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}


/**************************************************************************
函数功能：TIM4中断服务函数 步进电机调频PWM输出
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{
    float Capture1,Capture2;
    if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );//清除TIMx的中断待处理位
        Capture1 = TIM_GetCapture1(TIM4);
        TIM_SetCompare1(TIM4, Capture1 + Final_Moto2 );//设置TIMx自动重装载寄存器值
    }
    if(TIM_GetITStatus(TIM4,TIM_IT_CC2)!=RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2 );//清除TIMx的中断待处理位
        Capture2 = TIM_GetCapture2(TIM4);
        TIM_SetCompare2(TIM4, Capture2 + Final_Moto1 );//设置TIMx自动重装载寄存器值
    }
}

