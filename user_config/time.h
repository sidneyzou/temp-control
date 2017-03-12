#ifndef __TIME_H
#define __TIME_H

#include "config.h"

typedef struct time {
	u8 time_100ms;
	u8 time_200ms;
	u8 time_500ms;
	u8 time_1000ms;
}time_t;

void TimerInit(void);

//ʱ���־λ����
void TimerFlagClear(void);
#endif
