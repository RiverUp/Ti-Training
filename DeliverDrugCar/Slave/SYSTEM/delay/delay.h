#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  

#ifdef __cplusplus    
extern "C" {         
#endif
	
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
	
#ifdef __cplusplus
}
#endif

#endif





























