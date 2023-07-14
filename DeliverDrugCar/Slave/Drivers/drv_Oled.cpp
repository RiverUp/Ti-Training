#include "Basic.hpp"
#include "drv_Oled.hpp"
#include "Commulink.hpp"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"

//访问互斥锁
static SemaphoreHandle_t OledMutex = xSemaphoreCreateRecursiveMutex();
//发送完成标志
static EventGroupHandle_t events = xEventGroupCreate();
#define TxCompleteEvent (1<<0)
__attribute__ ((aligned (4))) static uint8_t scratch_buff[32]; 
//超时时间
#define TIMEOUT 2.0
//设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define USE_HORIZONTAL 0

/*
	PA15 -- SPI3_NSS  -- OLCD_DC
	PB3  -- SPI3_SCLK -- OLCD_SCL
	PB4  -- SPI3_MISO -- OLED_REST
	PB5  -- SPI3_MOSI -- OLCD_SDA
	PA5  -- OLCD_BLK  -- OLCD_BLK
	PA4  -- OLCD_CS   -- OLCD_CS
	*/
/*LCD操作*/
	#define OLED_DC_Clr()   GPIOA->BSRR= (1<<31)//DC
	#define OLED_DC_Set()   GPIOA->BSRR= (1<<15)

	#define OLED_RST_Clr()  GPIOB->BSRR = (1<<20)//RES
	#define OLED_RST_Set()  GPIOB->BSRR = (1<<4)

	#define OLED_BLK_Clr()  GPIOA->BSRR = (1<<21)//BLK
	#define OLED_BLK_Set()  GPIOA->BSRR = (1<<5)
	
	#define OLED_CS_Clr()   GPIOA->BSRR = (1<<20)//CS
	#define OLED_CS_Set()   GPIOA->BSRR = (1<<4)
/*LCD操作*/

/*SPI通信函数*/
	extern "C" void DMA2_Channel2_IRQHandler()
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		if(DMA2->ISR & (1<<5))
		{
			//清空DMA状态
			DMA2->IFCR = (1<<7) | (1<<6) | (1<<5) | (1<<4);
			xHigherPriorityTaskWoken = xEventGroupSetBitsFromISR( events, TxCompleteEvent, &xHigherPriorityTaskWoken );
		}
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}

	static void LCD_Writ_Bus( void* buffer, uint32_t size, bool DisableMInc )
	{
		OLED_CS_Clr();
		uint32_t total_size = size;
		uint32_t sd_length;	
		while( size > 0 )
		{
			//计算单次发送数量
			if( size > 65472 )
				sd_length = 65472;
			else
				sd_length = size;
			
			//关闭DMA
			DMA2_Channel2->CCR &= ~(1<<0);
			//地址递增模式
			if( DisableMInc )
			{
				DMA2_Channel2->CCR &= ~(1<<7);
				DMA2_Channel2->CCR |= (0b01<<10);
			}
			else
			{
				DMA2_Channel2->CCR |= (1<<7);
				DMA2_Channel2->CCR &= ~(0b11<<10);
			}
			//关闭SPI外设
			SPI3->CR1 &= ~(1<<6);
			//禁止 Tx DMA
			SPI3->CR2 &=~(1<<1);
			
			//清空DMA状态
			DMA2->IFCR = (1<<7) | (1<<6) | (1<<5) | (1<<4);
			//设置DMA存储器地址
			if(DisableMInc)
				DMA2_Channel2->CMAR = (uint32_t)buffer;
			else
				DMA2_Channel2->CMAR = (uint32_t)buffer + (total_size - size);
			//清空SPI状态
			SPI3->SR = (0b0000<<3);
			//设置DMA传输数量
			DMA2_Channel2->CNDTR = sd_length;
			//等待发送缓冲为空
			while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
			//使能DMA
			DMA2_Channel2->CCR |= (1<<0);
			//使能 Tx DMA
			SPI3->CR2 |=(1<<1);
			//打开SPI外设开始传输
			SPI3->CR1|=(1<<6);
			SPI3->CR1|=(1<<9);
			SPI3->CR2|=(1<<7);
			//等待DMA传输完成，等待该事件组第一位，退出此函数前将事件位清零
			xEventGroupWaitBits( events, TxCompleteEvent, pdTRUE, pdFALSE, TIMEOUT*configTICK_RATE_HZ );
			size -= sd_length;
		}
		OLED_CS_Set();
	}
/*SPI通信函数*/
	
/*OLED操作函数*/
	bool Lock_Oled( double Sync_waitTime )
	{
		uint32_t Sync_waitTicks;
		if( Sync_waitTime > 0 )
			Sync_waitTicks = Sync_waitTime*configTICK_RATE_HZ;
		else
			Sync_waitTicks = portMAX_DELAY;
		if( xSemaphoreTakeRecursive(OledMutex,Sync_waitTicks) == pdTRUE )
			return true;
		return false;
	}
	void UnLock_Oled()
	{
		xSemaphoreGiveRecursive(OledMutex);
	}
	
	static bool LCD_Writ_Bus( void* buffer, uint32_t size, double Sync_waitTime = -1 )
	{	
		if( Lock_Oled(Sync_waitTime) )
		{			
			LCD_Writ_Bus( buffer, size, false );
			UnLock_Oled();
			return true;
		}
		return false;
	} 

	//LCD写8字节数据
	bool LCD_WR_DATA8( uint8_t dat, double Sync_waitTime)
	{
		OLED_DC_Set();//写数据
		((uint8_t*)scratch_buff)[0] = dat;
		return LCD_Writ_Bus( &scratch_buff, 1, Sync_waitTime );
	}
	//LCD写16字节数据
	bool LCD_WR_DATA( uint16_t dat, double Sync_waitTime)
	{
		OLED_DC_Set();//写数据
		//dat = __REV16(dat);
		((uint16_t*)scratch_buff)[0] = dat;
		return LCD_Writ_Bus( scratch_buff ,2, Sync_waitTime );
	}

	static bool LCD_WR_REG( uint8_t dat, double Sync_waitTime)
	{
		OLED_DC_Clr();//写命令
		((uint8_t*)scratch_buff)[0] = dat;
		return LCD_Writ_Bus( &scratch_buff, 1, Sync_waitTime );
	}

	/******************************************************************************
				函数说明：设置起始和结束地址
				入口数据：x1,x2 设置列的起始和结束地址
									y1,y2 设置行的起始和结束地址
				返回值：  无
	******************************************************************************/
	void LCD_Address_Set( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
	{
		if(USE_HORIZONTAL==0)
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//储存器写
		}
		else if(USE_HORIZONTAL==1)
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1+80);
			LCD_WR_DATA(y2+80);
			LCD_WR_REG(0x2c);//储存器写
		}
		else if(USE_HORIZONTAL==2)
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1);
			LCD_WR_DATA(x2);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//储存器写
		}
		else
		{
			LCD_WR_REG(0x2a);//列地址设置
			LCD_WR_DATA(x1+80);
			LCD_WR_DATA(x2+80);
			LCD_WR_REG(0x2b);//行地址设置
			LCD_WR_DATA(y1);
			LCD_WR_DATA(y2);
			LCD_WR_REG(0x2c);//储存器写
		}
	}
	
	bool LCD_Clear( uint16_t Color, double Sync_waitTime)
	{
		if( Lock_Oled(Sync_waitTime) )
		{
			LCD_Address_Set( 0, 0, LCD_W-1, LCD_H-1 );
			OLED_DC_Set();//写数据
			((uint16_t*)scratch_buff)[0] = Color;
			LCD_Writ_Bus( scratch_buff, LCD_W*LCD_H*2, true );
			UnLock_Oled();
			return true;
		}
		return false;
	}

	bool LCD_WritePicture( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* pic, double Sync_waitTime )
	{
		if( Lock_Oled(Sync_waitTime) )
		{
			LCD_Address_Set( x1, y1, x2, y2 );
			OLED_DC_Set();//写数据
			LCD_Writ_Bus( pic, (x2-x1+1)*(y2-y1+1)*2 );
			UnLock_Oled();
			return true;
		}
		return false;
	}
	
/*OLED操作函数*/

void init_drv_Oled()
{
	/*GPIO配置*/
	/*
	PA15 -- SPI3_NSS  -- OLCD_DC
	PB3  -- SPI3_SCLK -- OLCD_SCL 
	PB4  -- SPI3_MISO -- OLED_REST
	PB5  -- SPI3_MOSI -- OLCD_SDA 
	PA5  -- OLCD_BLK  -- OLCD_BLK
	PA4  -- OLCD_CS   -- OLCD_CS
	*/
	/*使能GPIOA和GPIOB时钟*/
	RCC->APB2ENR |= (1<<2);
	os_delay(1e-2);
	RCC->APB2ENR |= (1<<3);
	os_delay(1e-2);

	/*PB3,PB5复用推挽输出模式*/
	set_register( GPIOB->CRL, 0b10, 14, 2 );	//PB3
	set_register( GPIOB->CRL, 0b10, 22, 2 );	//PB5

	/*PB4,PA15,PA4,PA5通用输出模式*/
	set_register( GPIOB->CRL, 0b01, 18, 2 );	//PB4
	set_register( GPIOA->CRL, 0b01, 18, 2 );	//PA4
	set_register( GPIOA->CRL, 0b01, 22, 2 );	//PA5
	set_register( GPIOA->CRH, 0b01, 30, 2 );	//PA15
	
	/*高速输出*/
	set_register( GPIOB->CRL, 0b11, 12, 2 );	//PB3
	set_register( GPIOB->CRL, 0b11, 16, 2 );	//PB4
	set_register( GPIOB->CRL, 0b11, 20, 2 );	//PB5
	set_register( GPIOA->CRL, 0b11, 16, 2 );	//PA4
	set_register( GPIOA->CRL, 0b11, 20, 2 );	//PA5
	set_register( GPIOA->CRH, 0b11, 28, 2 );	//PA15
	OLED_CS_Set();
	/*GPIO配置*/

	/*SPI3配置*/
	/*SPI3EN：SPI3 外设时钟使能*/
	RCC->APB1ENR|=(1<<15);
	os_delay(1e-2);
	//单线双向,只发模式，使用8位数据帧格式，NSS电平无效，8分频，配置为主设备，空闲状态时，SCK保持高电平，数据采样从第二个时钟边沿开始
	SPI3->CR1 = (1<<15) | (1<<14) | (0<<11)| (1<<9)| (1<<8) | (0b010<<3) | (1<<2) | (1<<1) | (1<<0) ;
	//使能发送DMA
	SPI3->CR2 = (1<<1);
	//模式故障标志清零
	SPI3->SR = (0<<5);

	/*SPI3配置*/

	/*DMA配置*/
	/*使能DMA2时钟*/
	RCC->AHBENR|=(1<<1);
	os_delay(1e-2);
	
	//外设地址
	DMA2_Channel2->CPAR=(uint32_t)(&SPI3->DR);
	//优先级低，存储器8位字节，每次数据传输后，存储器地址指针递增，外设地址指针固定，存储器到外设,开启发送完成中断
	DMA2_Channel2->CCR = (0b00<<12) | (0b00<<10) | (1<<7) | (0<<6) | (1<<4) | (1<<1);	
	//清空DMA状态
	DMA2->IFCR = (1<<7) | (1<<6) | (1<<5) | (1<<4);
	//使能DMA2通道2中断
	NVIC_SetPriority(DMA2_Channel2_IRQn,5);
	NVIC_EnableIRQ(DMA2_Channel2_IRQn);
	/*DMA配置*/

	/*OLED初始化*/
	//OLED Reset
	OLED_BLK_Clr();
	OLED_RST_Clr();
	os_delay(0.2);
	OLED_RST_Set();
	os_delay(0.2);

	//sleep out
	LCD_WR_REG(0x11); 
	os_delay(0.12);

	//memory data access control
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)
	{
	LCD_WR_DATA8(0x00);
	}
	else if(USE_HORIZONTAL==1)
	{
	LCD_WR_DATA8(0xC0);
	}
	else if(USE_HORIZONTAL==2)
	{
	LCD_WR_DATA8(0x70);
	}
	else 
	{
	LCD_WR_DATA8(0xA0);
	}

	//RGB 5-6-5
	LCD_WR_REG(0x3A); 
	LCD_WR_DATA8(0x65);

	//porch setting
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	//Gate Control
	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0x72);

	//VCOM Setting
	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x3D);	//Vcom=1.625v

	//LCM Control
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	//VDV and VRH Command Enable
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	//VRH Set
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x19);   

	//VDV Set
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);  

	//Frame Rate Control in Normal Mode
	LCD_WR_REG(0xC6); 
	LCD_WR_DATA8(0x0F);  //60mhz

	//Power Control 1
	LCD_WR_REG(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	//Positive Voltage Gamma Control
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	//Negative Voltage Gamma Control
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	//Display Inversion On
	LCD_WR_REG(0x21); 
	LCD_WR_REG(0x29); 

	//LSB
	LCD_WR_REG(0xB0);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(1<<3);

	LCD_Clear(0xffff);
	OLED_BLK_Set();
	/*OLED初始化*/
	//sendLedSignal(LEDSignal_Start2);
}