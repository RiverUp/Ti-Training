#include "Show.hpp"
#include "DataScop_DP.hpp"
#include "Control.hpp"

/*
 * 函数功能：虚拟示波器往上位机发送数据 关闭显示屏
 * 入口参数：无
 * 返回  值：无
 * 用要显示的数据替换0,最多可打印10个数据
 */
void DataScope(void)
{   
	//计数需要的变量
	int Send_Count,i;
	#if Debug_pos
		DataScope_Get_Channel_Data( Encoder_Left, 1 );      
		DataScope_Get_Channel_Data( Target_Position_L, 2 );
		DataScope_Get_Channel_Data( Encoder_Right, 3 );              
		DataScope_Get_Channel_Data( Target_Position_R , 4 );  	
	#elif Debug_vel
		DataScope_Get_Channel_Data( Encoder_Left, 1 );              
		DataScope_Get_Channel_Data( Target_Velocity_L , 2 );   
		DataScope_Get_Channel_Data( Encoder_Right, 3 );              
		DataScope_Get_Channel_Data( Target_Velocity_R , 4 );   
	#endif
	//打印几个数据就在这里改为几
	Send_Count = DataScope_Data_Generate(4);
	for( i = 0 ; i < Send_Count; i++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
	}
}

