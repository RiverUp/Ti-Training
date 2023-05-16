#include "TCRT5000.h"

void init_TCRT()
{
	//��ʼ��ADC��MCLK��4��Ƶ���ⲿͨ����
	ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_MCLK,ADC_PREDIVIDER_64,ADC_DIVIDER_8,ADC_NOROUTE);
	
	//����ADC�洢�Ĵ���������ͨ�����ת�����ο���ѹ3.3v������ͨ��Ϊ0
	ADC14_configureSingleSampleMode(ADC_MEM0,true);
	ADC14_configureConversionMemory(ADC_MEM0,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A0,ADC_NONDIFFERENTIAL_INPUTS);
	
	//�Զ�����
	ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
	
	//����p5.5ΪTCRT�������ţ�A0ͨ����
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,GPIO_PIN5,GPIO_TERTIARY_MODULE_FUNCTION);
	
	//ʹ���������
	ADC14_enableConversion();
	ADC14_toggleConversionTrigger();  
}

float read_TCRT()
{
	//�������õ�ѹ����
	float adcV=ADC14_getResult(ADC_MEM0);
	return adcV;
	
}