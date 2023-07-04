#include "sys.h"
#include "TimeBase.h" 
#include "TimeBase.hpp" 		
#include "stdlib.h"
#include "drv_LED.hpp"

/*
 * �������ܣ�ʵʱʱ������,��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������,BKP->DR1���ڱ����Ƿ��һ�����õ�����
 * reg���Ĵ�����ַ
 * data������
 * ����ֵ������������:�������
 */
u8 SystemClock_Config(void)
{
	//ʹ�ܵ�Դʱ��	   
	RCC->APB1ENR|=1<<28;      
	//ʹ�ܱ���ʱ��	    
	RCC->APB1ENR|=1<<27;     
	//���⣺�ȴ�Ӳ����ҵ��پ�����޸�Ϊ���پ���
	//�����ڲ��������� 
	RCC->CSR|=1<<0;         
	//�ȴ��ڲ�ʱ�Ӿ���	 
	while((!(RCC->CSR&0X02)));
	//������ز�������
	RTC_TimeStruct RTC_Time;
	//��ȡʱ��
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	//�õ��������е�ֵ(������)
	timecount=RTC->CNTH;
	timecount<<=16;
	timecount+=RTC->CNTL;			 
	//�õ�����(��������Ӧ��)
	temp=timecount/86400;   
	//����һ��
	if(daycnt!=temp)
	{	  
		daycnt=temp;
		//��1970�꿪ʼ
		temp1=1970;	
		while(temp>=365)
		{				 
			//������
			if(Is_Leap_Year(temp1))
			{
				//�����������0
				if(temp>=366)
					temp-=366;
				else 
					break;  
			}
			//ƽ�� 
			else 
				temp-=365;	  
			temp1++;  
		}   
		//�õ����
		RTC_Time.Year = temp1;
		temp1=0;
		//������һ����
		while(temp>=28)
		{
			//�����ǲ�������/2�·�
			if(Is_Leap_Year(RTC_Time.Year)&&temp1==1)
			{
				//�����������
				if(temp>=29)
					temp-=29;
				else 
					break; 
			}
			else 
			{
				//ƽ��
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];
				else 
					break;
			}
			temp1++;  
		}
		//�õ��·�
		RTC_Time.Month = temp1+1;
		//�õ����� 
		RTC_Time.Date = temp+1;
	}
	//�õ������� 
	temp=timecount%86400; 
	//��ȡ����   	
	RTC_Time.WeekDay=RTC_Get_Week(RTC_Time.Year,RTC_Time.Month,RTC_Time.Date);
	//Сʱ	
	RTC_Time.Hours = temp/3600;
	//����
	RTC_Time.Minutes = (temp%3600)/60;
	//����
	RTC_Time.Seconds = (temp%3600)%60;
	//��һ������
	if(BKP->DR1!=0X5050 || RTC_Time.Month==0 || RTC_Time.Date==0)
	{	 
		RTC_Time.Year = 2022;
		RTC_Time.Month = 12;
		RTC_Time.Date = 28;
		RTC_Time.Hours = 8;
		RTC_Time.Minutes = 14;
		RTC_Time.Seconds = 50;
		RTC_Time.WeekDay = 3;
		//ȡ��������д����
		PWR->CR|=1<<8;           
		//����������λ	 
		RCC->BDCR|=1<<16;          
		//����������λ����	  	
		RCC->BDCR&=~(1<<16);  
		
		//�����ڲ��������� 
		RCC->CSR|=1<<0;         
		//�ȴ��ڲ�ʱ�Ӿ���	 
	  while((!(RCC->CSR&0X02)));
		
		//�����ⲿ�������� 
		//RCC->BDCR|=1<<0;      
		//�ȴ��ⲿʱ�Ӿ���	 		
	  //while((!(RCC->BDCR&0X02)));
		
		//�����λ��ʶ 
		RCC->CSR|=1<<24;  
		//LSI��ΪRTCʱ�� 	    
		RCC->BDCR|=1<<9; 
		//RTCʱ��ʹ��	  
		RCC->BDCR|=1<<15;
		
		//�ȴ�RTC�Ĵ����������	 
	  while(!(RTC->CRL&(1<<5))){}
		//�ȴ�RTC�Ĵ���ͬ��  
    while(!(RTC->CRL&(1<<3))){}
		//�ȴ�RTC�Ĵ����������	 			
    while(!(RTC->CRL&(1<<5)));
		//��������	
		RTC->CRL|=1<<4;            
		RTC->PRLH=0X0000;
		//ʱ���������ã�����ֵ 32767
		RTC->PRLL=32767;        
		//����ʱ��		
		u16 t;
		u32 seccount=0;
		//��������ݵ��������
		for(t=1970;t<RTC_Time.Year;t++)	
		{
			//�����������
			if(Is_Leap_Year(t))
				seccount+=31622400;
			//ƽ���������
			else 
				seccount+=31536000;			  
		}
		RTC_Time.Month-=1;
		//��ǰ���·ݵ����������
		for(t=0;t<RTC_Time.Month;t++)	  
		{
			//�·����������
			seccount+=(u32)mon_table[t]*86400;
			//����2�·�����һ���������	 
			if(Is_Leap_Year(RTC_Time.Year)&&t==1)
				seccount+=86400;  
		}
		//��ǰ�����ڵ���������� 
		seccount+=(u32)(RTC_Time.Date-1)*86400;
		//Сʱ������
		seccount+=(u32)RTC_Time.Hours*3600;
		//����������
		seccount+=(u32)RTC_Time.Minutes*60;	 
		//�������Ӽ���ȥ
		seccount+=RTC_Time.Seconds;											
		//ʹ�ܵ�Դʱ��
		RCC->APB1ENR|=1<<28;
		//ʹ�ܱ���ʱ��
		RCC->APB1ENR|=1<<27;
		//ȡ��������д����
		PWR->CR|=1<<8;    
		//�������� 
		RTC->CRL|=1<<4;   
		RTC->CNTL=seccount&0xffff;
		RTC->CNTH=seccount>>16;
		//���ø���
		RTC->CRL&=~(1<<4);
		//�ȴ�RTC�Ĵ���������� 
		while(!(RTC->CRL&(1<<5)));
		//���ø���			
		RTC->CRL&=~(1<<4);   
		//�ȴ�RTC�Ĵ����������		 									  			
		while(!(RTC->CRL&(1<<5)));   
		BKP->DR1=0X5050;  
	}
	//ok
	return 0; 

}	


/*
 * �������ܣ��ж��Ƿ������꺯��
 * �·�   1  2  3  4  5  6  7  8  9  10 11 12
 * ����   31 29 31 30 31 30 31 31 30 31 30 31
 * ������ 31 28 31 30 31 30 31 31 30 31 30 31
 * year�����
 * ����ֵ��������ǲ�������.1,��.0,����
 */
u8 Is_Leap_Year(u16 year)
{			  
	//�����ܱ�4����
	if(year%4==0) 
	{ 
		if(year%100==0) 
		{ 
			//�����00��β,��Ҫ�ܱ�400����
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

//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*
 * �������ܣ�������������ڼ�,���빫�����ڵõ�����(ֻ����1901-2099��)
 * year,month,day������������
 * ����ֵ�����ں�	
 */																 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	yearH=year/100;	yearL=year%100; 
	//Ϊ21����,�������100  
	if (yearH>19)
		yearL+=100;
	//����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)
		temp2--;
	return(temp2%7);
}			  

//ʱ�ӿռ�
namespace __global_time
{
	const uint16_t timer_load = 0xFFF0;
	volatile uint16_t current_time_part_1 = 0;
	const double TIM2sec_scale = 1.0 / (double)TIMEBASECLK;
	const double TIM2sec_part1_scale = (double)65536 / (double)TIMEBASECLK;
}

//RTCʱ���ȡ
static RTC_TimeStruct RTC_Time;
static bool RTC_Updated = false;

//�ж�RTC�Ƿ����
extern "C" bool get_RTC_Updated()
{ 
	return RTC_Updated; 
}

//��ȡRTCʱ��
extern "C" RTC_TimeStruct Get_RTC_Time()
{
	if((RTC->CRL&(1<<3))!=0)
	{
		static u16 daycnt=0;
		u32 timecount=0; 
		u32 temp=0;
		u16 temp1=0;	  
		//�õ��������е�ֵ(������)
		timecount=RTC->CNTH;
		timecount<<=16;
		timecount+=RTC->CNTL;			 
		//�õ�����(��������Ӧ��)
		temp=timecount/86400;   
		//����һ��
		if(daycnt!=temp)
		{	  
			daycnt=temp;
			//��1970�꿪ʼ
			temp1=1970;	
			while(temp>=365)
			{				 
				//������
				if(Is_Leap_Year(temp1))
				{
					//�����������
					if(temp>=366)
						temp-=366;
					else 
						break;  
				}
				//ƽ�� 
				else 
					temp-=365;	  
				temp1++;  
			}   
			//�õ����
			RTC_Time.Year = temp1;
			temp1=0;
			//������һ����
			while(temp>=28)
			{
				//�����ǲ�������/2�·�
				if(Is_Leap_Year(RTC_Time.Year)&&temp1==1)
				{
					//�����������
					if(temp>=29)
						temp-=29;
					else 
						break; 
				}
				else 
				{
					//ƽ��
					if(temp>=mon_table[temp1])
						temp-=mon_table[temp1];
					else 
						break;
				}
				temp1++;  
			}
			//�õ��·�
			RTC_Time.Month = temp1+1;
			//�õ����� 
			RTC_Time.Date = temp+1;
		}
		//�õ������� 
		temp=timecount%86400; 
		//��ȡ����   	
		RTC_Time.WeekDay=RTC_Get_Week(RTC_Time.Year,RTC_Time.Month,RTC_Time.Date);
		//Сʱ	
		RTC_Time.Hours = temp/3600;
		//����
		RTC_Time.Minutes = (temp%3600)/60;
		//����
		RTC_Time.Seconds = (temp%3600)%60;
		RTC->CRL&=~(1<<3);
	}
	return RTC_Time;
}

//����RTC���ں�ʱ��
extern "C" void Set_RTC_Time(RTC_TimeStruct* T)
{
	
	u16 t;
	u32 seccount=0;
	//��������ݵ��������
	for(t=1970;t<T->Year;t++)	
	{
		//�����������
		if(Is_Leap_Year(t))
			seccount+=31622400;
		//ƽ���������
		else 
			seccount+=31536000;			  
	}
	T->Month-=1;
	//��ǰ���·ݵ����������
	for(t=0;t<T->Month;t++)	  
	{
		//�·����������
		seccount+=(u32)mon_table[t]*86400;
		//����2�·�����һ���������	 
		if(Is_Leap_Year(T->Year)&&t==1)
			seccount+=86400;  
	}
	//��ǰ�����ڵ���������� 
	seccount+=(u32)(T->Date-1)*86400;
	//Сʱ������
	seccount+=(u32)T->Hours*3600;
	//����������
	seccount+=(u32)T->Minutes*60;	 
	//�������Ӽ���ȥ
	seccount+=T->Seconds;											
	//ʹ�ܵ�Դʱ��
	RCC->APB1ENR|=1<<28;
	//ʹ�ܱ���ʱ��
	RCC->APB1ENR|=1<<27;
	//ȡ��������д����
	PWR->CR|=1<<8;    
	//�������� 
	RTC->CRL|=1<<4;   
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	//���ø���
	RTC->CRL&=~(1<<4);
	//�ȴ�RTC�Ĵ���������� 
	while(!(RTC->CRL&(1<<5)));
	//������֮�����һ������ 	
	Get_RTC_Time();
	RTC_Updated = true;
}

//��ʱ��2���жϴ�����
extern "C" void TIM2_IRQHandler()
{
	if(TIM2->SR & (1<<0))
	{		
		++__global_time::current_time_part_1;
	}
	TIM2->SR = 0;
	__DSB();
}

//ʱ�ӳ�ʼ������
void init_TimeBase()
{		
	u8 Res = 0;
	//���õ�Ƭ��ʱ��
	Res = SystemClock_Config();
	//��ʼ��ʧ�ܿ���
	while(Res);
	//��TIM2����ȫ�ּ�ʱ��
	RCC->APB1ENR |= (1<<0);
	for( volatile int i = 0 ; i < 1000 ; ++i );
	//��Ƶ��12Mhz
	TIM2->PSC = 6;
	TIM2->ARR = __global_time::timer_load;	
	TIM2->EGR = (1<<0);
	TIM2->SR = 0;
	//�򿪶�ʱ���жϣ�����������ȼ���
	TIM2->DIER = 1<<0;
	NVIC_SetPriority( TIM2_IRQn , 0 );
	NVIC_EnableIRQ( TIM2_IRQn );
	//�򿪶�ʱ��
	TIM2->CR1 = (1<<7) | (1<<0);
}












