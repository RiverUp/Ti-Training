#include "Basic.hpp"

/*声光提示*/
	enum LEDSignal
	{
		LEDSignal_Start1 ,
		LEDSignal_Start2 ,
		
		LEDSignal_Continue1 ,
		LEDSignal_Success1 ,
		
		LEDSignal_Err1 ,
		LEDSignal_Err2
	};
	enum LEDMode
	{
		//关闭自动模式
		LEDMode_Manual ,
		
		//正常模式
		LEDMode_Normal1 ,
		LEDMode_Normal2 ,
		
		//飞行模式
		LEDMode_Flying1 ,
		LEDMode_Flying2 ,
		
		//正在处理
		LEDMode_Processing1 ,
		LEDMode_Processing2 ,
	};
	void sendLedSignal( LEDSignal signal );
	void setLedMode( LEDMode mode );
	void setLedManualCtrl( float R, float G, float B, bool BuzzerOn, uint16_t BuzzerFreq );
/*声光提示*/
	
void Commulink_Server();