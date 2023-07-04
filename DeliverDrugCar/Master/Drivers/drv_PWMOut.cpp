#include "drv_PwmOut.hpp"
#include "Basic.hpp"

//关断所有电机
void PWM_DisableAll()
{
	TIM1->CCR3=0000;
	TIM1->CCR4=0000;
}
//PWM最小转速
void PWM_PullDownAll()
{
	TIM1->CCR3=0;
	TIM1->CCR4=0;
}

//PWM最大转速
void PWM_PullUpAll()
{
	TIM1->CCR3=7000;
	TIM1->CCR4=7000;
}

//函数功能：初始化电机接口
void init_drv_Motor(void)
{
	// PD4  ---> AIN1
	// PC12 ---> AIN2
	// PD11 ---> BIN1
	// PD10 ---> BIN2
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_10|GPIO_Pin_11;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_Init(GPIOD, &GPIO_InitStructure);			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOE,GPIO_Pin_10);	
}

/*
	 PWMB( TIM1_CH3) -- PE13
	 PWMA( TIM1_CH4) -- PE14
*/
void init_drv_PWMOut()
{
	/*配置TIM*/ 
	//使能定时器1
	RCC->APB2ENR|=(1<<11);
	os_delay(0.01);
	//关断所有电机
	PWM_DisableAll();
	/*配置GPIO*/  	
	os_delay(0.01);
	//设置定时器的重映射
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//GPIO初始化
	//使能PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);	 		    	
	//推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;; 	 		
	//IO口速度为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14; 		
	//初始化GPIOE
	GPIO_Init(GPIOE, &GPIO_InitStructure);					
	
	TIM_TimeBaseStructure.TIM_Period = 7199;
	//设置预分频值
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	//初始化 TIMx
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 	
	//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	//比较输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_Pulse = 0;                            
	//输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); //初始化外设 TIM1 OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能预装载寄存器
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //初始化外设 TIM1 OC4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能预装载寄存器
	//确定让TIM1输出PWM
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	//使能 TIM1
	TIM_Cmd(TIM1, ENABLE); 
	/*配置TIM*/ 
	PWM_PullDownAll();
}