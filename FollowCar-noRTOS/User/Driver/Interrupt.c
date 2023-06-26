#include "rtthread.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HC-SR04.h"
#include "Encoder.h"
#include "Control.h"

// ��⵽�����ش����ж�
void PORT2_IRQHandler(void)
{
	uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
	GPIO_clearInterrupt(GPIO_PORT_P2, status);

	// hc-sr04������ж�
	if (status & GPIO_PIN5)
	{
		
		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN5)==1)
		{
			
			GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
			TriggerCounter=0;
			Timer32_startTimer(TIMER32_1_BASE,false);
			//Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_CONTINUOUS_MODE);
			
		}
		else
		{
			GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
			GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
			Timer32_haltTimer(TIMER32_1_BASE);
			HCSRCountValue=TriggerCounter;
			NextTiggerHCSRFlag=true;
//			Timer_A_stopTimer(TIMER_A0_BASE);			
//			HCSRCountValue=Timer_A_getCounterValue(TIMER_A0_BASE);
//			Timer_A_clearTimer(TIMER_A0_BASE);

			
		}
		//GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		// ʹ��TA0�жϣ��ر�PORT2�ж�
		//GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);
		// �������Ź���
		// ��ʼ����
	}

	// ��������ж�
	//  if(status&GPIO_PIN3)
	//  {
	//  	if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN3)==GPIO_INPUT_PIN_LOW)
	//  	{
	//  		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)==GPIO_INPUT_PIN_LOW)
	//  		{
	//  			encoder_left++;
	//  		}
	//  		else
	//  		{
	//  			encoder_left--;
	//  		}
	//  	}
	//  	else
	//  	{
	//  		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)
	//  		{
	//  			encoder_left++;
	//  		}
	//  		else
	//  		{
	//  			encoder_left--;
	//  		}
	//  	}
	//  }
	//  else if(status&GPIO_PIN4)
	//  {
	//  	if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)==GPIO_INPUT_PIN_LOW)
	//  	{
	//  		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN3)==GPIO_INPUT_PIN_HIGH)
	//  		{
	//  			encoder_left++;
	//  		}
	//  		else
	//  		{
	//  			encoder_left--;
	//  		}
	//  	}
	//  	else
	//  	{
	//  		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN3)==GPIO_INPUT_PIN_LOW)
	//  		{
	//  			encoder_left++;
	//  		}
	//  		else
	//  		{
	//  			encoder_left--;
	//  		}
	//  	}
	//  }
}

void PORT3_IRQHandler(void)
{
	uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
	GPIO_clearInterrupt(GPIO_PORT_P3, status);

	// �ұ������ж�
	if (status & GPIO_PIN5)
	{
		if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7) == GPIO_INPUT_PIN_LOW)
			{
				encoder_right++;
			}
			else
			{
				encoder_right--;
			}
		}
		else
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7) == GPIO_INPUT_PIN_HIGH)
			{
				encoder_right++;
			}
			else
			{
				encoder_right--;
			}
		}
	}
	else if (status & GPIO_PIN7)
	{
		if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7) == GPIO_INPUT_PIN_LOW)
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == GPIO_INPUT_PIN_HIGH)
			{
				encoder_right++;
			}
			else
			{
				encoder_right--;
			}
		}
		else
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
			{
				encoder_right++;
			}
			else
			{
				encoder_right--;
			}
		}
	}
}

// �������
void PORT4_IRQHandler(void)
{
	uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
	GPIO_clearInterrupt(GPIO_PORT_P4, status);

	if (status & GPIO_PIN1)
	{
		if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6) == GPIO_INPUT_PIN_LOW)
			{
				encoder_left++;
			}
			else
			{
				encoder_left--;
			}
		}
		else
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6) == GPIO_INPUT_PIN_HIGH)
			{
				encoder_left++;
			}
			else
			{
				encoder_left--;
			}
		}
	}
	else if (status & GPIO_PIN6)
	{
		if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6) == GPIO_INPUT_PIN_LOW)
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1) == GPIO_INPUT_PIN_HIGH)
			{
				encoder_left++;
			}
			else
			{
				encoder_left--;
			}
		}
		else
		{
			if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1) == GPIO_INPUT_PIN_LOW)
			{
				encoder_left++;
			}
			else
			{
				encoder_left--;
			}
		}
	}
}

void T32_INT1_IRQHandler(void)
{
	MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
	Control();
	
}

void T32_INT2_IRQHandler(void)
{
	MAP_Timer32_clearInterruptFlag(TIMER32_1_BASE);
	TriggerCounter++;
}

// ��⵽�½��ش����ж�
void TA0_N_IRQHandler(void)
{
	GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
	// Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_2);
	// ����жϱ�־λ
	Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
	// ��ü���ֵ
	HCSRCountValue = Timer_A_getCaptureCompareCount(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
	// �������Ź���Ϊ������������
	//GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN5);
	// �������ֵ��ֹͣ����
	Timer_A_clearTimer(TIMER_A0_BASE);
	Timer_A_stopTimer(TIMER_A0_BASE);
//	Interrupt_disableInterrupt(INT_TA0_N);
//	Interrupt_enableInterrupt(INT_PORT2);
	
}
