#include "moto.h"
#include "sys.h"
extern int Final_Moto1,Final_Moto2,Final_Moto3,Final_Moto4;

/**************************************************************************
�������ܣ����������ת ��ת�ٵĵ���
��ڲ�����
		moto��1���Ƶ��A,2���Ƶ��B
		speed�����Ƶ���ٶȣ�0-900��
		direction��moto1��0Ϊ˳ʱ�� 1Ϊ��ʱ��
		           moto2��1Ϊ˳ʱ�� 0Ϊ��ʱ��
����  ֵ�� ��
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




