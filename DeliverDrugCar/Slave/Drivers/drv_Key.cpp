#include "stm32f10x.h"
#include "drv_Key.hpp"
#include "Basic.hpp"
#include "delay.h"

//������ʶ
int tran = 0;
static u16 Press_count;

/*
 * �������ܣ�����ɨ��
 * time��˫���ȴ�ʱ��
 * ����ֵ������״̬ 0���޶��� 1������ 2��˫�� 
 */
u8 click_N_Double (u16 time)
{
	static	u16 flag_key=0,count_key=0,double_key=0;	
	static	u16 count_single=0,Forever_count=0;
	//������־λδ��1
	if(KEY==0)  							
		Forever_count++;   					
    else       
		Forever_count=0;
	//�������£��Ұ������±�־Ϊ0	
	if(KEY == 0 && flag_key == 0)			
		flag_key=1;	
	//��һ��Ϊ0
	if(count_key == 0)						
	{
		//��������һ�Σ�double_key��һ�Σ�count=1
		if(flag_key==1) 
		{
			double_key++;					
			count_key=1;					
		}
		//˫��ִ�е�ָ��
		if(double_key==2) 
		{
			double_key=0;
			count_single=0;
			return 2;
		}
	}
	//����δ����
	if(KEY == 1)			
		flag_key=0,count_key=0;	
	//�����Ѿ�����һ��	
	if(double_key == 1)						
	{
		//�����ȴ�ʱ��
		count_single++;						
		//����ִ�е�ָ��
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
 * �������ܣ�����ɨ�裨��Ӳ��������
 * ����ֵ������״̬ 0���޶��� 1������
 */
u8 click(void)
{
	//�������ɿ���־
	static u8 flag_key=1;
	//��������
	if(flag_key&&KEY==0)
	{
		flag_key=0;
		return 1;	
	}
	//�ް�������
	else if(1==KEY)			
		flag_key=1;
		return 0;
}

/*
 * �������ܣ��������
 * ����ֵ������״̬ 0���޶��� 1������ 2������2s
 */
u8 Long_Press(void)
{
	static u16 Long_Press_count,Long_Press;
	//������־λδ��1
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
	//������־λ��1
	if(Long_Press==1)     
	{
		Long_Press=0;
	}
	return 0;
}

//������ʼ������
void init_drv_Key(void)
{
	//KEY  PC0
	//����GPIOC����ʱ��,�������ù���ʱ��
	RCC->APB2ENR|=(1<<4)|(1<<0);
	//KEY��ʼ��
	os_delay(1e-2);
	//�����������ģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);              
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;                       
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;                           
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}