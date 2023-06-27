#include "key.h"

//按键初始化函数
void KEY_Init(void) 
{
	//IO初始化
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4 );
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY1按下
//2，KEY2按下
//注意此函数有响应优先级,KEY1>KEY2!!
uint8_t KEY_Scan(uint8_t mode)
{
	uint16_t i;
	static uint8_t key_up = 1; //按键按松开标志
	if (mode)
		key_up = 1; //支持连按
	if (key_up && (KEY1 == 0 || KEY2 == 0 ))
	{
		for (i = 0; i < 5000; i++); //去抖动
		key_up = 0;
		if (KEY1 == 0)
			return KEY1_PRES;
		else if (KEY2 == 0)
			return KEY2_PRES;
//		else if (KEY3 == 0)
//			return KEY3_PRES;
//		else if (KEY4 == 0)
//			return KEY4_PRES;
	}
	else if (KEY1 == 1 && KEY2 == 1)
		key_up = 1;
	return 0; // 无按键按下
}
