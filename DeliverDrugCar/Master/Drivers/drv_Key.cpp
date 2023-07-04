#include "stm32f10x.h"
#include "drv_Key.hpp"
#include "Basic.hpp"
#include "delay.h"

//长按标识
int tran = 0;
static u16 Press_count;

/*
 * 函数功能：按键扫描
 * time：双击等待时间
 * 返回值：按键状态 0：无动作 1：单击 2：双击 
 */
u8 click_N_Double (u16 time)
{
	static	u16 flag_key=0,count_key=0,double_key=0;	
	static	u16 count_single=0,Forever_count=0;
	//长按标志位未置1
	if(KEY==0)  							
		Forever_count++;   					
    else       
		Forever_count=0;
	//按键按下，且按键按下标志为0	
	if(KEY == 0 && flag_key == 0)			
		flag_key=1;	
	//第一次为0
	if(count_key == 0)						
	{
		//按键按下一次，double_key加一次，count=1
		if(flag_key==1) 
		{
			double_key++;					
			count_key=1;					
		}
		//双击执行的指令
		if(double_key==2) 
		{
			double_key=0;
			count_single=0;
			return 2;
		}
	}
	//按键未按下
	if(KEY == 1)			
		flag_key=0,count_key=0;	
	//按键已经按下一次	
	if(double_key == 1)						
	{
		//超过等待时间
		count_single++;						
		//单击执行的指令
		if(count_single>time&&Forever_count<time)
		{
			double_key=0;
			count_single=0;	
			return 1;
		}
		if(Forever_count>time)
		{
			double_key=0;
			count_single=0;	
		}
	}	
	return 0;
}

/*
 * 函数功能：按键扫描（无硬件驱动）
 * 返回值：按键状态 0：无动作 1：单击
 */
u8 click(void)
{
	//按键按松开标志
	static u8 flag_key=1;
	//按键按下
	if(flag_key&&KEY==0)
	{
		flag_key=0;
		return 1;	
	}
	//无按键按下
	else if(1==KEY)			
		flag_key=1;
		return 0;
}

/*
 * 函数功能：长按检测
 * 返回值：按键状态 0：无动作 1：单击 2：长按2s
 */
u8 Long_Press(void)
{
	static u16 Long_Press_count,Long_Press;
	//长按标志位未置1
	if(Long_Press==0&&KEY==0) 
	{		
		Press_count++;
		Long_Press_count++;   
	}
	else                       
		Long_Press_count=0; 
	if(Long_Press_count>200)		
	{
		Long_Press=1;
		Long_Press_count=0;
		return 2;
	}	
	else if(Long_Press_count==1)
	{
		Press_count=0;
		return 1;
	}		
	//长按标志位置1
	if(Long_Press==1)     
	{
		Long_Press=0;
	}
	return 0;
}

//按键初始化函数
void init_drv_Key(void)
{
	//KEY  PC0
	//开启GPIOC外设时钟,开启复用功能时钟
	RCC->APB2ENR|=(1<<4)|(1<<0);
	//KEY初始化
	os_delay(1e-2);
	//配置引脚输出模式
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);              
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;                       
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;                           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}