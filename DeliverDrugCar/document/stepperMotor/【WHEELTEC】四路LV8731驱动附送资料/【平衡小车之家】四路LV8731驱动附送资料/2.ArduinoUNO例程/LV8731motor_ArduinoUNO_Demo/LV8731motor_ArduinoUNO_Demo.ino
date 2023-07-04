
//////////////////LV8731引脚接线///////////////////////////
//DATA_ABC牛角座-----------ArduinoUNO主板引脚
//    ST(使能)-----------------2
//    ADC---------------------A0
//    VA(STEPA)---------------4
//    VB(STEPB)---------------5
//    VC(STEPC)---------------6
//    5V----------------------5V
//    GND---------------------GND
//    DA(FRA)-----------------8
//    DB(FRB)-----------------9
//    DC(FRC)-----------------10

//DATA_D牛角座--------------ArduinoUNO主板引脚
//    5V-----------------------(与DATA_ABC牛角座5V引脚导通，两者可以只接通1个)
//    GND----------------------(与DATA_ABC牛角座GND引脚导通，两者可以只接通1个)
//    ST-----------------------(与DATA_ABC牛角座ST引脚导通，两者可以只接通1个)
//    DD(FRD)------------------11
//    VD(STEPD)----------------7



//定义引脚名称
#define ST_ABC 2   //使能引脚，该引脚时高电平才允许控制电机，低电平时电机停止
//#define ST_D   12  //使能引脚，与ST_ABC导通
#define ADC    A0   //用于测电源电压
#define STEPA  4   //电机A转速控制引脚，输出高频方波控制转速，频率越高速度越快
#define STEPB  5   //电机B转速控制引脚，输出高频方波控制转速，频率越高速度越快
#define STEPC  6   //电机C转速控制引脚，输出高频方波控制转速，频率越高速度越快
#define STEPD  7  //电机D转速控制引脚，输出高频方波控制转速，频率越高速度越快
#define FRA    8   //电机A转向控制引脚，高电位平时电机A顺时针旋转，低电位平时电机A逆时针旋转
#define FRB    9   //电机B转向控制引脚，高电位平时电机B顺时针旋转，低电位平时电机B逆时针旋转
#define FRC    10   //电机C转向控制引脚，高电位平时电机C顺时针旋转，低电位平时电机C逆时针旋转
#define FRD    11  //电机D转向控制引脚，高电位平时电机D顺时针旋转，低电位平时电机D逆时针旋转

/**************************************************************************
函数功能：初始化 相当于STM32里面的Main函数 作者：平衡小车之家
入口参数：无
返回  值：无
**************************************************************************/
void setup() 
{
  Serial.begin(9600);
  pinMode(ST_ABC, OUTPUT);
  pinMode(ADC,    INPUT);  //设置为输入引脚，读取电压模拟量
  pinMode(STEPA,  OUTPUT);
  pinMode(STEPB,  OUTPUT);
  pinMode(STEPC,  OUTPUT);
  pinMode(FRA,    OUTPUT);
  pinMode(FRB,    OUTPUT);
  pinMode(FRC,    OUTPUT);
  pinMode(FRD,    OUTPUT);
  pinMode(STEPD,  OUTPUT);

  //初始化LV8731电机驱动模块
  digitalWrite(ST_ABC, 1);
  digitalWrite(STEPA, 0);
  digitalWrite(STEPB, 0);
  digitalWrite(STEPC, 0);
  digitalWrite(STEPD, 0);
  digitalWrite(FRA, 0);   //电机A旋转方向为逆时针
  digitalWrite(FRB, 0);   //电机B旋转方向为逆时针
  digitalWrite(FRC, 0);   //电机C旋转方向为逆时针
  digitalWrite(FRD, 0);   //电机D旋转方向为逆时针

}


//定义相关全局变量
int frequency_khz=20; //频率设置变量，单位：KHZ
int delay_us=1000/(frequency_khz*2); //频率转化为延迟时间，一升一降，两次延迟，单位：微秒
                                     //1秒=1000毫秒(ms),1秒=1000000 微秒(μs),1秒=1000000000 纳秒(ns)
double V; //电池电压变量，单位：伏特


/**************************************************************************
函数功能：主循环程序体
入口参数：无
返回  值：无
**************************************************************************/
void loop() 
{
//  V=analogRead(ADC);      //读取模拟引脚10模拟量
//  Serial.print(V/29.45);  //对模拟量转换并通过串口输出
//  Serial.println("V");

//此段作用为向转速控制引脚输出高频方波
  digitalWrite(STEPA, HIGH); 
//  digitalWrite(STEPB, HIGH); 
//  digitalWrite(STEPC, HIGH); 
//  digitalWrite(STEPD, HIGH); 
  delayMicroseconds(delay_us); //延迟时间决定频率
  
  digitalWrite(STEPA, LOW);
//  digitalWrite(STEPB, LOW);
//  digitalWrite(STEPC, LOW);
//  digitalWrite(STEPD, LOW);
  delayMicroseconds(delay_us); //延迟时间决定频率
//此段作用为向转速控制引脚输出方波
}
