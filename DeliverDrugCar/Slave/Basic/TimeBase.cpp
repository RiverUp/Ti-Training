#include "sys.h"
#include "TimeBase.h" 
#include "TimeBase.hpp" 		
#include "stdlib.h"
#include "drv_LED.hpp"

/*
 * 函数功能：实时时钟配置,初始化RTC时钟,同时检测时钟是否工作正常,BKP->DR1用于保存是否第一次配置的设置
 * reg：寄存器地址
 * data：数据
 * 返回值：正常，其他:错误代码
 */
u8 SystemClock_Config(void)
{
	//使能电源时钟	   
	RCC->APB1ENR|=1<<28;      
	//使能备份时钟	    
	RCC->APB1ENR|=1<<27;     
	//问题：等待硬件外挂低速晶振后，修改为低速晶振
	//开启内部低速振荡器 
	RCC->CSR|=1<<0;         
	//等待内部时钟就绪	 
	while((!(RCC->CSR&0X02)));
	//日历相关参数配置
	RTC_TimeStruct RTC_Time;
	//获取时间
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	//得到计数器中的值(秒钟数)
	timecount=RTC->CNTH;
	timecount<<=16;
	timecount+=RTC->CNTL;			 
	//得到天数(秒钟数对应的)
	temp=timecount/86400;   
	//超过一天
	if(daycnt!=temp)
	{	  
		daycnt=temp;
		//从1970年开始
		temp1=1970;	
		while(temp>=365)
		{				 
			//是闰年
			if(Is_Leap_Year(temp1))
			{
				//闰年的秒钟数0
				if(temp>=366)
					temp-=366;
				else 
					break;  
			}
			//平年 
			else 
				temp-=365;	  
			temp1++;  
		}   
		//得到年份
		RTC_Time.Year = temp1;
		temp1=0;
		//超过了一个月
		while(temp>=28)
		{
			//当年是不是闰年/2月份
			if(Is_Leap_Year(RTC_Time.Year)&&temp1==1)
			{
				//闰年的秒钟数
				if(temp>=29)
					temp-=29;
				else 
					break; 
			}
			else 
			{
				//平年
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];
				else 
					break;
			}
			temp1++;  
		}
		//得到月份
		RTC_Time.Month = temp1+1;
		//得到日期 
		RTC_Time.Date = temp+1;
	}
	//得到秒钟数 
	temp=timecount%86400; 
	//获取星期   	
	RTC_Time.WeekDay=RTC_Get_Week(RTC_Time.Year,RTC_Time.Month,RTC_Time.Date);
	//小时	
	RTC_Time.Hours = temp/3600;
	//分钟
	RTC_Time.Minutes = (temp%3600)/60;
	//秒钟
	RTC_Time.Seconds = (temp%3600)%60;
	//第一次配置
	if(BKP->DR1!=0X5050 || RTC_Time.Month==0 || RTC_Time.Date==0)
	{	 
		RTC_Time.Year = 2022;
		RTC_Time.Month = 12;
		RTC_Time.Date = 28;
		RTC_Time.Hours = 8;
		RTC_Time.Minutes = 14;
		RTC_Time.Seconds = 50;
		RTC_Time.WeekDay = 3;
		//取消备份区写保护
		PWR->CR|=1<<8;           
		//备份区域软复位	 
		RCC->BDCR|=1<<16;          
		//备份区域软复位结束	  	
		RCC->BDCR&=~(1<<16);  
		
		//开启内部低速振荡器 
		RCC->CSR|=1<<0;         
		//等待内部时钟就绪	 
	  while((!(RCC->CSR&0X02)));
		
		//开启外部低速振荡器 
		//RCC->BDCR|=1<<0;      
		//等待外部时钟就绪	 		
	  //while((!(RCC->BDCR&0X02)));
		
		//清除复位标识 
		RCC->CSR|=1<<24;  
		//LSI作为RTC时钟 	    
		RCC->BDCR|=1<<9; 
		//RTC时钟使能	  
		RCC->BDCR|=1<<15;
		
		//等待RTC寄存器操作完成	 
	  while(!(RTC->CRL&(1<<5))){}
		//等待RTC寄存器同步  
    while(!(RTC->CRL&(1<<3))){}
		//等待RTC寄存器操作完成	 			
    while(!(RTC->CRL&(1<<5)));
		//允许配置	
		RTC->CRL|=1<<4;            
		RTC->PRLH=0X0000;
		//时钟周期设置，理论值 32767
		RTC->PRLL=32767;        
		//设置时间		
		u16 t;
		u32 seccount=0;
		//把所有年份的秒钟相加
		for(t=1970;t<RTC_Time.Year;t++)	
		{
			//闰年的秒钟数
			if(Is_Leap_Year(t))
				seccount+=31622400;
			//平年的秒钟数
			else 
				seccount+=31536000;			  
		}
		RTC_Time.Month-=1;
		//把前面月份的秒钟数相加
		for(t=0;t<RTC_Time.Month;t++)	  
		{
			//月份秒钟数相加
			seccount+=(u32)mon_table[t]*86400;
			//闰年2月份增加一天的秒钟数	 
			if(Is_Leap_Year(RTC_Time.Year)&&t==1)
				seccount+=86400;  
		}
		//把前面日期的秒钟数相加 
		seccount+=(u32)(RTC_Time.Date-1)*86400;
		//小时秒钟数
		seccount+=(u32)RTC_Time.Hours*3600;
		//分钟秒钟数
		seccount+=(u32)RTC_Time.Minutes*60;	 
		//最后的秒钟加上去
		seccount+=RTC_Time.Seconds;											
		//使能电源时钟
		RCC->APB1ENR|=1<<28;
		//使能备份时钟
		RCC->APB1ENR|=1<<27;
		//取消备份区写保护
		PWR->CR|=1<<8;    
		//允许配置 
		RTC->CRL|=1<<4;   
		RTC->CNTL=seccount&0xffff;
		RTC->CNTH=seccount>>16;
		//配置更新
		RTC->CRL&=~(1<<4);
		//等待RTC寄存器操作完成 
		while(!(RTC->CRL&(1<<5)));
		//配置更新			
		RTC->CRL&=~(1<<4);   
		//等待RTC寄存器操作完成		 									  			
		while(!(RTC->CRL&(1<<5)));   
		BKP->DR1=0X5050;  
	}
	//ok
	return 0; 

}	


/*
 * 函数功能：判断是否是闰年函数
 * 月份   1  2  3  4  5  6  7  8  9  10 11 12
 * 闰年   31 29 31 30 31 30 31 31 30 31 30 31
 * 非闰年 31 28 31 30 31 30 31 31 30 31 30 31
 * year：年份
 * 返回值：该年份是不是闰年.1,是.0,不是
 */
u8 Is_Leap_Year(u16 year)
{			  
	//必须能被4整除
	if(year%4==0) 
	{ 
		if(year%100==0) 
		{ 
			//如果以00结尾,还要能被400整除
			if(year%400==0)
				return 1; 	   
			else 
				return 0;   
		}
		else 
			return 1;   
	}
	else 
		return 0;	
}	 

//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*
 * 函数功能：获得现在是星期几,输入公历日期得到星期(只允许1901-2099年)
 * year,month,day：公历年月日
 * 返回值：星期号	
 */																 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	yearH=year/100;	yearL=year%100; 
	//为21世纪,年份数加100  
	if (yearH>19)
		yearL+=100;
	//所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)
		temp2--;
	return(temp2%7);
}			  

//时钟空间
namespace __global_time
{
	const uint16_t timer_load = 0xFFF0;
	volatile uint16_t current_time_part_1 = 0;
	const double TIM2sec_scale = 1.0 / (double)TIMEBASECLK;
	const double TIM2sec_part1_scale = (double)65536 / (double)TIMEBASECLK;
}

//RTC时间获取
static RTC_TimeStruct RTC_Time;
static bool RTC_Updated = false;

//判断RTC是否更新
extern "C" bool get_RTC_Updated()
{ 
	return RTC_Updated; 
}

//获取RTC时间
extern "C" RTC_TimeStruct Get_RTC_Time()
{
	if((RTC->CRL&(1<<3))!=0)
	{
		static u16 daycnt=0;
		u32 timecount=0; 
		u32 temp=0;
		u16 temp1=0;	  
		//得到计数器中的值(秒钟数)
		timecount=RTC->CNTH;
		timecount<<=16;
		timecount+=RTC->CNTL;			 
		//得到天数(秒钟数对应的)
		temp=timecount/86400;   
		//超过一天
		if(daycnt!=temp)
		{	  
			daycnt=temp;
			//从1970年开始
			temp1=1970;	
			while(temp>=365)
			{				 
				//是闰年
				if(Is_Leap_Year(temp1))
				{
					//闰年的秒钟数
					if(temp>=366)
						temp-=366;
					else 
						break;  
				}
				//平年 
				else 
					temp-=365;	  
				temp1++;  
			}   
			//得到年份
			RTC_Time.Year = temp1;
			temp1=0;
			//超过了一个月
			while(temp>=28)
			{
				//当年是不是闰年/2月份
				if(Is_Leap_Year(RTC_Time.Year)&&temp1==1)
				{
					//闰年的秒钟数
					if(temp>=29)
						temp-=29;
					else 
						break; 
				}
				else 
				{
					//平年
					if(temp>=mon_table[temp1])
						temp-=mon_table[temp1];
					else 
						break;
				}
				temp1++;  
			}
			//得到月份
			RTC_Time.Month = temp1+1;
			//得到日期 
			RTC_Time.Date = temp+1;
		}
		//得到秒钟数 
		temp=timecount%86400; 
		//获取星期   	
		RTC_Time.WeekDay=RTC_Get_Week(RTC_Time.Year,RTC_Time.Month,RTC_Time.Date);
		//小时	
		RTC_Time.Hours = temp/3600;
		//分钟
		RTC_Time.Minutes = (temp%3600)/60;
		//秒钟
		RTC_Time.Seconds = (temp%3600)%60;
		RTC->CRL&=~(1<<3);
	}
	return RTC_Time;
}

//设置RTC日期和时间
extern "C" void Set_RTC_Time(RTC_TimeStruct* T)
{
	
	u16 t;
	u32 seccount=0;
	//把所有年份的秒钟相加
	for(t=1970;t<T->Year;t++)	
	{
		//闰年的秒钟数
		if(Is_Leap_Year(t))
			seccount+=31622400;
		//平年的秒钟数
		else 
			seccount+=31536000;			  
	}
	T->Month-=1;
	//把前面月份的秒钟数相加
	for(t=0;t<T->Month;t++)	  
	{
		//月份秒钟数相加
		seccount+=(u32)mon_table[t]*86400;
		//闰年2月份增加一天的秒钟数	 
		if(Is_Leap_Year(T->Year)&&t==1)
			seccount+=86400;  
	}
	//把前面日期的秒钟数相加 
	seccount+=(u32)(T->Date-1)*86400;
	//小时秒钟数
	seccount+=(u32)T->Hours*3600;
	//分钟秒钟数
	seccount+=(u32)T->Minutes*60;	 
	//最后的秒钟加上去
	seccount+=T->Seconds;											
	//使能电源时钟
	RCC->APB1ENR|=1<<28;
	//使能备份时钟
	RCC->APB1ENR|=1<<27;
	//取消备份区写保护
	PWR->CR|=1<<8;    
	//允许配置 
	RTC->CRL|=1<<4;   
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	//配置更新
	RTC->CRL&=~(1<<4);
	//等待RTC寄存器操作完成 
	while(!(RTC->CRL&(1<<5)));
	//设置完之后更新一下数据 	
	Get_RTC_Time();
	RTC_Updated = true;
}

//定时器2的中断处理函数
extern "C" void TIM2_IRQHandler()
{
	if(TIM2->SR & (1<<0))
	{		
		++__global_time::current_time_part_1;
	}
	TIM2->SR = 0;
	__DSB();
}

//时钟初始化函数
void init_TimeBase()
{		
	u8 Res = 0;
	//配置单片机时钟
	Res = SystemClock_Config();
	//初始化失败卡死
	while(Res);
	//打开TIM2用作全局计时器
	RCC->APB1ENR |= (1<<0);
	for( volatile int i = 0 ; i < 1000 ; ++i );
	//分频至12Mhz
	TIM2->PSC = 6;
	TIM2->ARR = __global_time::timer_load;	
	TIM2->EGR = (1<<0);
	TIM2->SR = 0;
	//打开定时器中断（必须最高优先级）
	TIM2->DIER = 1<<0;
	NVIC_SetPriority( TIM2_IRQn , 0 );
	NVIC_EnableIRQ( TIM2_IRQn );
	//打开定时器
	TIM2->CR1 = (1<<7) | (1<<0);
}












