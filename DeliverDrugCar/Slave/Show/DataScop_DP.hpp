#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
 
//������֡���ݻ�����
extern unsigned char DataScope_OutPut_Buffer[42];	   
//дͨ��������������֡���ݻ�����
void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    
//����֡�������ɺ���
unsigned char DataScope_Data_Generate(unsigned char Channel_Number);   
 
#endif 



