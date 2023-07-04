#include "Basic.hpp"

//玫瑰少年
void Play(void);
//主任务
void init_Trumpet();
/*
	扬声器器频率调节函数
	freq:蜂鸣器频率
*/
void set_TrumpetFreq( unsigned short freq );
/*
	扬声器鸣响函数
	on:是否鸣响
*/
void set_TrumpetOnOff( bool on );