#include "Commulink.hpp"
#include "Basic.hpp"
#include "drv_LED.hpp"
#include <map>

using namespace std;

/*声光提示*/
	//LED信号状态指示
	static float ledSignalCounter = -1;	static LEDSignal ledSignal;
	//LED模式
	static LEDMode ledmode = LEDMode_Processing1;
	//R G B 状态指示
	static float ledR = 0, ledG = 0, ledB = 0;
	//蜂鸣器状态
	static bool buzzerOn = false;	
	//蜂鸣器频率
	static uint16_t buzzerFreq;
	//设置LED信号
	void sendLedSignal( LEDSignal signal )
	{
		ledSignalCounter = 0;
		ledSignal = signal;
	}
	//LED模式设置
	void setLedMode( LEDMode mode )
	{
		ledmode = mode;
	}
	//相关设置
	void setLedManualCtrl( float R, float G, float B, bool BuzzerOn, uint16_t BuzzerFreq )
	{
		ledR = R;	
		ledG = G;	
		ledB = B;
		buzzerOn = BuzzerOn;	
		buzzerFreq = BuzzerFreq;
		ledmode = LEDMode_Manual;
	}
	//LED刷新函数
	static void LEDRefresh(float dt)
	{
		if( ledSignalCounter >= 0 )
		{
			switch(ledSignal)
			{
				//开始LED1
				case LEDSignal_Start1:
				{
					if( ledSignalCounter > 0.3 )
					{
						ledSignalCounter = -1;
						return;
					}
					if( ledSignalCounter < 0.15f )
					{
						set_BuzzerFreq(900);
						set_BuzzerOnOff(true);
						set_LedBrightness( 0, 0, 100 );
					}
					else
					{
						set_BuzzerFreq(1500);
						set_BuzzerOnOff(true);
						set_LedBrightness( 0, 100, 0 );
					}
					break;
				}
				//开始LED2
				case LEDSignal_Start2:
				{
					if( ledSignalCounter > 0.45 )
					{
						ledSignalCounter = -1;
						return;
					}
					if( ledSignalCounter < 0.15 )
					{
						set_BuzzerFreq(800);
						set_BuzzerOnOff(true);
						set_LedBrightness( 100, 0, 0 );
					}
					else if( ledSignalCounter < 0.3 )
					{
						set_BuzzerFreq(1000);
						set_BuzzerOnOff(true);
						set_LedBrightness( 0, 100, 0 );
					}
					else
					{
						set_BuzzerFreq(1200);
						set_BuzzerOnOff(true);
						set_LedBrightness( 0, 0, 100 );
					}
					break;
				}
				//持续LED1
				case LEDSignal_Continue1:
				{
					if( ledSignalCounter > 0.8 )
					{
						ledSignalCounter = -1;
						return;
					}
					set_BuzzerFreq(1500);				
					if( ledSignalCounter < 0.2f )
					{
						set_LedBrightness( 0, 0, 100 );
						set_BuzzerOnOff(true);
					}
					else if( ledSignalCounter < 0.4f )
					{
						set_LedBrightness( 0, 0, 0 );
						set_BuzzerOnOff(false);
					}
					else if( ledSignalCounter < 0.6f )
						set_LedBrightness( 0, 0, 100 );
					else
						set_LedBrightness( 0, 0, 0 );
					break;
				}
				//成功LED1
				case LEDSignal_Success1:
				{
					if( ledSignalCounter > 0.8 )
					{
						ledSignalCounter = -1;
						return;
					}
					set_BuzzerFreq(1500);
					if( ledSignalCounter < 0.2f )
					{						
						set_LedBrightness( 0, 100, 0 );
						set_BuzzerOnOff(true);
					}
					else if( ledSignalCounter < 0.4f )
					{
						set_LedBrightness( 0, 0, 0 );
						set_BuzzerOnOff(false);
					}
					else if( ledSignalCounter < 0.6f )
					{
						set_LedBrightness( 0, 100, 0 );
						set_BuzzerOnOff(true);
					}
					else
					{
						set_LedBrightness( 0, 0, 0 );
						set_BuzzerOnOff(false);
					}
					break;
				}
				//错误LED1
				case LEDSignal_Err1:
				{
					if( ledSignalCounter > 1.0 )
					{
						ledSignalCounter = -1;
						return;
					}
					set_BuzzerFreq(800);
					set_BuzzerOnOff(true);
					if( ledSignalCounter < 0.25f )
						set_LedBrightness( 100, 0, 0 );
					else if( ledSignalCounter < 0.5f )
						set_LedBrightness( 0, 0, 0 );
					else if( ledSignalCounter < 0.75f )
						set_LedBrightness( 100, 0, 0 );
					else
						set_LedBrightness( 0, 0, 0 );
					break;
				}
				//错误LED2
				case LEDSignal_Err2:
				{
					//低电量报警
					if( ledSignalCounter > 0.3 )
					{
						ledSignalCounter = -1;
						return;
					}
					set_BuzzerFreq(2000);
					set_BuzzerOnOff(true);
					if( ledSignalCounter < 0.15f )
						set_LedBrightness( 100, 0, 0 );
					else if( ledSignalCounter < 0.3f )
						set_LedBrightness( 0, 0, 0 );
					if( ledSignalCounter < 0.35f )
						set_LedBrightness( 100, 0, 0 );
					else if( ledSignalCounter < 0.5f )
						set_LedBrightness( 0, 0, 0 );					
					break;					
				}				
			}
			ledSignalCounter += dt;
			return;
		}
		//成功之后进入正常模式
		static float counter = 0;
		switch(ledmode)
		{
			/*正常模式*/
				case LEDMode_Normal1:
				{
					if( counter > 2 )
						counter = 0;
					set_BuzzerOnOff(false);
					if( counter < 1 )
						set_LedBrightness( 0 , counter*100 , 0 );
					else
						set_LedBrightness( 0 , 200 - counter*100 , 0 );
					break;
				}
				case LEDMode_Normal2:
				{
					if( counter > 2 )
						counter = 0;
					set_BuzzerOnOff(false);
					if( counter < 1 )
						set_LedBrightness( 0 , 0 , counter*100 );
					else
						set_LedBrightness( 0 , 200 - counter*100 , 0 );
					break;
				}
			/*正常模式*/
				
			/*运动模式*/
				//黄
				case LEDMode_Flying1:
				{
					if( counter > 1.4 )
						counter = 0;
					set_BuzzerOnOff(false);
					//闪烁效果
					if( counter < 1 )
						set_LedBrightness( 0 , counter*0 , 0-counter*0 );
					else if( counter < 1.1 )
						set_LedBrightness( 80 , 80 , 0 );
					else if( counter < 1.2 )
						set_LedBrightness( 0 , 0 , 0 );
					else if( counter < 1.3 )
						set_LedBrightness( 80 , 80 , 0 );
					else
						set_LedBrightness( 0 , 0 , 0 );
					break;
				}
				//绿
				case LEDMode_Flying2:
				{
					if( counter > 1.4 )
						counter = 0;
					set_BuzzerOnOff(false);
					//闪烁效果
					if( counter < 1 )
						set_LedBrightness( 0 , counter*0 , 0-counter*0 );
					else if( counter < 1.1 )
						set_LedBrightness( 0 , 0 , 100 );
					else if( counter < 1.2 )
						set_LedBrightness( 0 , 0 , 0 );
					else if( counter < 1.3 )
						set_LedBrightness( 0 , 0 , 100 );
					else
						set_LedBrightness( 0 , 0 , 0 );
					break;
				}
			/*运动模式*/	
				
			/*处理中*/
				case LEDMode_Processing1:
				{
					if( counter > 0.5 )
						counter = 0;
					set_BuzzerOnOff(false);
					set_LedBrightness( 0 , 0 , counter*200 );
					break;
				}
				case LEDMode_Processing2:
				{
					if( counter > 0.5 )
						counter = 0;
					set_BuzzerOnOff(false);
					if( counter < 0.25 )
						set_LedBrightness( 0 , 100-counter*400 , counter*400 );
					else
						set_LedBrightness( 0 , (counter-0.25)*400 , 100-(counter-0.25)*400 );
					break;
				}
			/*处理中*/
				
			/*默认保持上次状态*/
				default:
				{	
					//用户手动控制
					if(buzzerOn)
						set_BuzzerFreq(buzzerFreq);
					set_BuzzerOnOff(buzzerOn);
					set_LedBrightness( ledR, ledG, ledB );
					break;
				}
			/*默认保持上次状态*/
		}
		counter += dt;
	}
/*声光提示*/

void Commulink_Server()
{
	//刷新led声光提示
	LEDRefresh(0.1);
}
	