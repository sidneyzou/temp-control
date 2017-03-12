#ifndef __LED_H
#define __LED_H

#include	"config.h"

sbit unlock_pin   = P3^5;

#define UNLOCK_ON()      unlock_pin   = 0
#define UNLOCK_OFF()     unlock_pin   = 1
#endif