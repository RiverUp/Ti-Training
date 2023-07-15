#include "drv_Main.hpp"
#include "drv_LED.hpp"
#include "drv_LCD.hpp"
#include "drv_Key.hpp"
#include "drv_MPU6050.hpp"
#include "drv_Encoder.hpp"
#include "drv_ExtIIC.hpp"
#include "drv_Sensors.hpp"
#include "drv_PwmOut.hpp"
#include "drv_ADC.hpp"
#include "drv_Uart1.hpp"
#include "drv_Uart2.hpp"
#include "drv_Uart3.hpp"
#include "drv_Uart4.hpp"
#include "Commulink.hpp"
#include "GUI.hpp"

void init_drv_Main()
{
	//LED初始化函数
	init_drv_LED();
	//初始化屏幕
	init_drv_LCD();
	//初始化GUI界面
	init_GUI();
	//初始化按键
	init_drv_Key();
	//初始化电机接口
	init_drv_Motor();
	//初始化PWM
	init_drv_PWMOut();
	//初始化传感器
	init_drv_Sensors();
	//初始化编码器
	drv_Encoder_Init_TIM3();
	drv_Encoder_Init_TIM4();
	//初始化ADC
	init_drv_ADC();
	//初始化串口
	init_drv_Uart1(115200);
	init_drv_Uart2(115200);
	init_drv_Uart3(115200);
	init_drv_Uart4(115200);
	
}