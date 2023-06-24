#include "rtthread.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HC-SR04.h"
#include "Encoder.h"
#include "Control.h"


//检测到上升沿触发中断
void PORT2_IRQHandler(void)
{
	uint32_t status=GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
	GPIO_clearInterrupt(GPIO_PORT_P2,status);
	
	//hc-sr04的相关中断
	if(status&GPIO_PIN5)
	{
		//使能TA0中断，关闭PORT2中断
		Interrupt_enableInterrupt(INT_TA0_N);
		Interrupt_disableInterrupt(INT_PORT2);
		//更换引脚功能
		GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
		//开始计数
		Timer_A_startCounter(TIMER_A0_BASE,TIMER_A_CONTINUOUS_MODE);
	}
	
	//左编码器中断
	// if(status&GPIO_PIN3)
	// {
	// 	if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN3)==GPIO_INPUT_PIN_LOW)
	// 	{
	// 		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)==GPIO_INPUT_PIN_LOW)
	// 		{
	// 			encoder_left++;
	// 		}
	// 		else
	// 		{
	// 			encoder_left--;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)
	// 		{
	// 			encoder_left++;
	// 		}
	// 		else
	// 		{
	// 			encoder_left--;
	// 		}
	// 	}
	// }
	// else if(status&GPIO_PIN4)
	// {
	// 	if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN4)==GPIO_INPUT_PIN_LOW)
	// 	{
	// 		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN3)==GPIO_INPUT_PIN_HIGH)
	// 		{
	// 			encoder_left++;
	// 		}
	// 		else
	// 		{
	// 			encoder_left--;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if(GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN3)==GPIO_INPUT_PIN_LOW)
	// 		{
	// 			encoder_left++;
	// 		}
	// 		else
	// 		{
	// 			encoder_left--;
	// 		}
	// 	}
	// }
}

void PORT3_IRQHandler(void)
{
	uint32_t status=GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
	GPIO_clearInterrupt(GPIO_PORT_P3,status);
	
	//右编码器中断
	if(status&GPIO_PIN5)
	{
		if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN5)==GPIO_INPUT_PIN_LOW)
		{
			if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN7)==GPIO_INPUT_PIN_LOW)
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
			if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN7)==GPIO_INPUT_PIN_HIGH)
			{
				encoder_right++;
			}
			else
			{
				encoder_right--;
			}
		}
	}
	else if(status&GPIO_PIN7)
	{
		if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN7)==GPIO_INPUT_PIN_LOW)
		{
			if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN5)==GPIO_INPUT_PIN_HIGH)
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
			if(GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN5)==GPIO_INPUT_PIN_LOW)
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

//左编码器
void PORT4_IRQHandler(void)
{
	uint32_t status=GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
	GPIO_clearInterrupt(GPIO_PORT_P4,status);
	
	if(status&GPIO_PIN1)
	{
		if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN1)==GPIO_INPUT_PIN_LOW)
		{
			if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)==GPIO_INPUT_PIN_LOW)
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
			if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)==GPIO_INPUT_PIN_HIGH)
			{
				encoder_left++;
			}
			else
			{
				encoder_left--;
			}
		}
	}
	else if(status&GPIO_PIN6)
	{
		if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN6)==GPIO_INPUT_PIN_LOW)
		{
			if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)
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
			if(GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN1)==GPIO_INPUT_PIN_LOW)
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
  MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	Control();
}

//检测到下降沿触发中断
void TA0_N_IRQHandler(void)
{
	//清除中断标志位
	Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_2);
	//获得计数值
	countValue=Timer_A_getCaptureCompareCount(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_2);
	//更改引脚功能为下拉电阻输入
	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2,GPIO_PIN5);
	//清楚计数值，停止捕获
	Timer_A_clearTimer(TIMER_A0_BASE);
	Timer_A_stopTimer(TIMER_A0_BASE);
	Interrupt_disableInterrupt(INT_TA0_N);
//	if(AbleToConvert->value==0)
//	{
//		//发送信号量，告知线程获得测量
//		rt_sem_release(AbleToConvert);
//	}
}