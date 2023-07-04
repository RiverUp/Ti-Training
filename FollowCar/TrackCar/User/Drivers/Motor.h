#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*
p2.4->PWMA
p2.6->PWMB
p4.7->AIN1
p4.5->AIN2
p5.4->BIN1
p5.5->BIN2
*/

void init_motor();
void set_pwm_trail();
void set_pwm(int motor_left,int motor_right);
int limit_pwm(int IN,int max,int min);

