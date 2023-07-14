#include "Basic.hpp"
#include "stm32f10x.h"

//初始化完成指示
static uint16_t InitializationStatus_Lock = 0;
//初始化未完成不能进行初始化操作
static bool InitializationCompleted = false;
//判断是否初始化完成
bool getInitializationCompleted()
{ 
	return InitializationCompleted; 
}
//设置初始化完成函数
void setInitializationCompleted()
{ 
	if(InitializationStatus_Lock == 0)
		InitializationCompleted = true; 
}
//递归自锁
void LockInitializationStatus()
{ 
	++InitializationStatus_Lock; 
}
//递归解锁
void UnLockInitializationStatus()
{ 
	--InitializationStatus_Lock; 
}


//初始化系统外设
void init_Basic()
{	
	//系统时钟初始化
	SystemInit();
	//延时时钟初始化
	delay_init();	
	//打开SWD接口,关闭复用功能
	JTAG_Set(SWD_ENABLE);        
	//设置系统中断优先级分组4	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	//系统时钟初始化
	init_TimeBase();
}