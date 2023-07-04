#include "sys.h"
#include "usart.h"	  

#if 1
__ASM (".global __use_no_semihosting");    

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	//循环发送,直到发送完毕 
	while((USART1->SR&0X40)==0){}  
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

