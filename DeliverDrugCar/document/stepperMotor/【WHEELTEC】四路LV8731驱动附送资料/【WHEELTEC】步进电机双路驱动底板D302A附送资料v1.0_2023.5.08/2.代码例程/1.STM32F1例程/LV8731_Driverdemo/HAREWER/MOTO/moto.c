#include "moto.h"
#include "sys.h"
extern int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;

/**************************************************************************
函数功能：电机的正反转 和转速的调整
入口参数：
		moto：1控制电机A,2控制电机B
		speed：控制电机速度（0-900）
		direction：moto1：0为顺时针 1为逆时针
		           moto2：1为顺时针 0为逆时针
返回  值： 无
**************************************************************************/

static int h,i,j,k;
void control(int moto,int speed,int direction)
{

    if(moto==1) {
//	if(Final_Moto1>1000-speed)
//	  h++;
        Final_Moto1=1000-speed;
        PBout(15)=direction;
    }
    if(moto==2) {
//	if(Final_Moto2>1000-speed)
//	  i++;
        Final_Moto2=1000-speed;
        PBout(13)=direction;
    }
}




