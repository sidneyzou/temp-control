#include "head.h"


pid_t g_pid;

void Init_pid()
{
	g_pid.goal = 0;
	g_pid.now = 0;
	g_pid.err = 0;
	g_pid.err_last = 0;
	g_pid.kp = 0.2;
	g_pid.ki = 0;
	g_pid.kd = 0;
	
	
}

u16 pid(float goal,float now)
{
	u16 ret = 0;
	g_pid.goal = goal;
	g_pid.now  = now;
	
	g_pid.err = g_pid.goal - g_pid.now;
	
	g_pid.integral += g_pid.err;
	
	ret = g_pid.kp * g_pid.err + g_pid.ki * g_pid.integral + g_pid.kd * (g_pid.err - g_pid.err_last);
	
	g_pid.err_last = g_pid.err;
	
	return ret;
}
