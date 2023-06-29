#include "TCRT5000.h"

bool tcrt1,tcrt2,tcrt3;
static uint16_t resultbuffer[8];

void init_TCRT()
{
	//初始化ADC（MCLK，4分频，外部通道）
	ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_MCLK,ADC_PREDIVIDER_64,ADC_DIVIDER_8,ADC_NOROUTE);
	//memset(resultbuffer, 0x00 , 8 * sizeof(uint16_t));
	
	MAP_REF_A_setReferenceVoltage(REF_A_VREF2_5V);
	REF_A_enableReferenceVoltage();
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7,GPIO_TERTIARY_MODULE_FUNCTION);
	
	//配置ADC存储寄存器——单通道多次转换，参考电压3.3v，输入通道为0
	//ADC14_configureSingleSampleMode(ADC_MEM18,true);
	//ADC14_configureSingleSampleMode(ADC_MEM19,true);
	//ADC14_configureSingleSampleMode(ADC_MEM20,true);
	//ADC14_configureSingleSampleMode(ADC_MEM23,true);
	//ADC14_configureSingleSampleMode(ADC_MEM22,true);
	
	ADC14_configureMultiSequenceMode(ADC_MEM18,ADC_MEM23, true);
	
	ADC14_configureConversionMemory(ADC_MEM18,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A18,ADC_NONDIFFERENTIAL_INPUTS);
	ADC14_configureConversionMemory(ADC_MEM19,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A19,ADC_NONDIFFERENTIAL_INPUTS);
	ADC14_configureConversionMemory(ADC_MEM20,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A20,ADC_NONDIFFERENTIAL_INPUTS);
	ADC14_configureConversionMemory(ADC_MEM23,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A23,ADC_NONDIFFERENTIAL_INPUTS);
	ADC14_configureConversionMemory(ADC_MEM22,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A22,ADC_NONDIFFERENTIAL_INPUTS);
	
	//自动触发
	ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
	
	//设置p4.3为TCRT输入引脚（A0通道）
	//GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,GPIO_PIN3,GPIO_TERTIARY_MODULE_FUNCTION);
	//GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,GPIO_PIN2,GPIO_TERTIARY_MODULE_FUNCTION);
	//GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,GPIO_PIN5,GPIO_TERTIARY_MODULE_FUNCTION);
	//GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,GPIO_PIN6,GPIO_TERTIARY_MODULE_FUNCTION);
	//GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,GPIO_PIN7,GPIO_TERTIARY_MODULE_FUNCTION);
	
	//使能软件触发
	ADC14_enableConversion();
	ADC14_toggleConversionTrigger();  
}

bool read_TCRT(int num)
{
	float adcV;
	bool value;
	switch(num)
	{
		case 1:
      adcV=ADC14_getResult(ADC_MEM18);
		  if(adcV>1000)
				value=1;
			else 
				value=0;
		  break;
		case 2:
			adcV=ADC14_getResult(ADC_MEM19);
		  if(adcV>1000)
				value=1;
			else 
				value=0;
		  break;
		case 3:
			adcV=ADC14_getResult(ADC_MEM20);
		  if(adcV>1500)
				value=1;
			else 
				value=0;
		  break;
		case 4:
			adcV=ADC14_getResult(ADC_MEM23);
		  if(adcV>1500)
				value=1;
			else 
				value=0;
		  break;
		case 5:
			adcV=ADC14_getResult(ADC_MEM22);
		  if(adcV>1500)
				value=1;
			else 
				value=0;
		  break;
		default:
			value=0;
		  break;
	}
	//现在先用电压调参
	return value;
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
			value=GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN7);
			break;
		case 5:
			value=GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN6);
			break;
		default:
			value=0;
			break;
	}
	return value;
}

