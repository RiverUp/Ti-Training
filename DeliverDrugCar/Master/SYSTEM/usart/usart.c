#include "sys.h"
#include "usart.h"	  

#if 1
__ASM (".global __use_no_semihosting");    

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	//ѭ������,ֱ��������� 
	while((USART1->SR&0X40)==0){}  
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

