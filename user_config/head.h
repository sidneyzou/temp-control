
#ifndef		__HEAD_H
#define		__HEAD_H

#include "config.h"

#include "time.h"
#include "delay.h"
#include "uart.h"
#include "entry.h"
#include "oled.h"
#include "eeprom.h"
#include "display.h"
#include "ds1302.h"
#include "key.h"
#include "ds18b20.h"
#include "pid.h"


sbit led_pin    = P1^5;

sbit heat_pin = P2^1;

#define HEAT_ON()      heat_pin   = 0
#define HEAT_OFF()     heat_pin   = 1

#define LED_ON()      led_pin   = 0
#define LED_OFF()     led_pin   = 1

/*****************************∫Í∂®“Â***********************************/
#endif
