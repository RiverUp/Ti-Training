#include "Show.hpp"
#include "DataScop_DP.hpp"
#include "Control.hpp"

/*
 * �������ܣ�����ʾ��������λ���������� �ر���ʾ��
 * ��ڲ�������
 * ����  ֵ����
 * ��Ҫ��ʾ�������滻0,���ɴ�ӡ10������
 */
void DataScope(void)
{   
	//������Ҫ�ı���
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
	//��ӡ�������ݾ��������Ϊ��
	Send_Count = DataScope_Data_Generate(4);
	for( i = 0 ; i < Send_Count; i++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
	}
}

