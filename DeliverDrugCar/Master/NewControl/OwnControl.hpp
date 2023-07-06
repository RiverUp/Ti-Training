#ifndef __OWNCONTROL_H
#define __OWNCONTROL_H

#include "rtthread.h"

int TurnOwn(int targetAngle, int currentAngle);
int VelocityOwn(int targetVelocity, int currentVelocity);

#endif // !__OWNCOTROL