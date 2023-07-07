#include "main.h"

#define SERIAL_BUFFER_SIZE 20

extern char serialDataBuffer[];
extern int serialBufferPtr;

void initSerial(void) ;
void sendMsgBySerial(char *msg);
