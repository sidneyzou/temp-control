#ifndef __PID_H_
#define __PID_H_
#define u8 unsigned char 
#define u16  unsigned int

typedef struct pid {
	
	float goal;
	float now;
	float err;
	float err_last;
	float kp,ki,kd;
	float integral;
} pid_t;

void Init_pid();

u16 pid(float goal,float now);


#endif