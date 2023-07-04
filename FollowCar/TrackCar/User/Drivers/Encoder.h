#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*
p4.1->E2A
p4.6->E2B
p3.5->E1A
p3.7->E1B
*/

extern int encoder_left;
extern int encoder_right;


void init_encoder_left();
void init_encoder_right();
//side->0:ÓÒ£»1£º×ó
int read_encoder(uint8_t side);