#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "config.h"

typedef enum blink {
	BLINK_NONE,
	BLINK_YEAR,
	BLINK_MON,
	BLINK_DAY,
	BLINK_HOUR,
	BLINK_MIN,
	BLINK_SEC, 
	BLINK_NUM	
};
void ShowMenu(void);

void ShowTemp(void);

void ShowTimer (enum blink mode);
#endif
