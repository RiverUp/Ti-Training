#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
 
//待发送帧数据缓存区
extern unsigned char DataScope_OutPut_Buffer[42];	   
//写通道数据至待发送帧数据缓存区
void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    
//发送帧数据生成函数
unsigned char DataScope_Data_Generate(unsigned char Channel_Number);   
 
#endif 



