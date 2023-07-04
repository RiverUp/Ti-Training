#include "drv_ADC.hpp"
#include "Basic.hpp"

/*
 * 函数功能：AD采样
 * 入口参数: ch：ADC1 的通道
 * 返回  值：AD转换结果
 */	 		
u16 Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	//ADC1,ADC通道,采样时间为239.5周期	  
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );			
	//使能指定的ADC1的软件转换启动功能			
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		 
	//等待转换结束
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//返回最近一次ADC1规则组的转换结果
	return ADC_GetConversionValue(ADC1);	
}
	

/*
 * 函数功能：读取电池电压 
 * 入口参数: 无
 * 返回  值：电池电压 单位MV
 */
int Get_battery_volt(void)   
{  
	//电池电压
	int Volt;
	//电阻分压
	Volt=Get_Adc(Battery_Ch)*3.3*11/10*12.3*100/4096;	
	return Volt;
}

void init_drv_ADC(void)
{
	/*
	Current_ADC -- PC2 ADC1_INP12
	*/
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  
	//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
	//设置模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	//模拟输入引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_DeInit(ADC1);  
	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	
	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	//ADC数据右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	//顺序进行规则转换的ADC通道的数目
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Init(ADC1, &ADC_InitStructure);	
	//使能指定的ADC1
	ADC_Cmd(ADC1, ENABLE);	
	//使能复位校准  
	ADC_ResetCalibration(ADC1);		 
	//等待复位校准结束
	while(ADC_GetResetCalibrationStatus(ADC1));		
	//开启AD校准
	ADC_StartCalibration(ADC1);	 
	//等待校准结束
	while(ADC_GetCalibrationStatus(ADC1));	 
}