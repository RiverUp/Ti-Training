#include "Encoder.h"

int encoder_left;
int encoder_right;

void init_encoder_left()
{
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN1|GPIO_PIN6);
	GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN1|GPIO_PIN6);
	GPIO_enableInterrupt(GPIO_PORT_P4,GPIO_PIN1|GPIO_PIN6);
	Interrupt_enableInterrupt(INT_PORT4);
}

void init_encoder_right()
{
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3,GPIO_PIN5|GPIO_PIN7);
	GPIO_clearInterruptFlag(GPIO_PORT_P3,GPIO_PIN5|GPIO_PIN7);
	GPIO_enableInterrupt(GPIO_PORT_P3,GPIO_PIN5|GPIO_PIN7);
	Interrupt_enableInterrupt(INT_PORT3);
}

int read_encoder(uint8_t side)
{
	int coder_value=0;
	if(!side)
	{
		coder_value=encoder_right;
		encoder_right=0;
	}
	else
	{
		coder_value=encoder_left;
		encoder_left=0;
	}
	return coder_value;
}


