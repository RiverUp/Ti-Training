#include "TCRT5000.h"

bool tcrt1,tcrt2,tcrt3;

void init_TCRT()
{
	//��ʼ��ADC��MCLK��4��Ƶ���ⲿͨ����
	ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_MCLK,ADC_PREDIVIDER_64,ADC_DIVIDER_8,ADC_NOROUTE);
	
	
	//����ADC�洢�Ĵ���������ͨ�����ת�����ο���ѹ3.3v������ͨ��Ϊ0
	ADC14_configureSingleSampleMode(ADC_MEM10,true);
	ADC14_configureConversionMemory(ADC_MEM10,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A10,ADC_NONDIFFERENTIAL_INPUTS);
	
	//�Զ�����
	ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
	
	//����p5.5ΪTCRT�������ţ�A0ͨ����
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,GPIO_PIN3,GPIO_TERTIARY_MODULE_FUNCTION);
	
	//ʹ���������
	ADC14_enableConversion();
	ADC14_toggleConversionTrigger();  
}

float read_TCRT()
{
	//�������õ�ѹ����
	float adcV=ADC14_getResult(ADC_MEM10);
	return adcV;
}

void init_digitalTCRT()
{
	
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3,GPIO_PIN6);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1,GPIO_PIN7|GPIO_PIN6);
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5,GPIO_PIN0|GPIO_PIN2);
}

bool getTCRTValue(int num)
{
	bool value;
	switch(num)
	{
		case 1:
			value=GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN6);
			break;
		case 2:
			value=GPIO_getInputPinValue(GPIO_PORT_P5,GPIO_PIN2);
			break;
		case 3:
			value=GPIO_getInputPinValue(GPIO_PORT_P5,GPIO_PIN0);
			break;
		case 4:
			value=GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN6);
			break;
		case 5:
			value=GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN7);
			break;
		default:
			value=0;
			break;
	}
	return value;
}

